#include <QString>
#include <QStringList>
#include <QtTest>
#include <memory>
#include <chrono>
#include <thread>
#include <algorithm>

#include "signalqueue.hh"
#include "prioritylibrarystub.hh"
#include "prioritysubjectstub.hh"
#include "signalreader.hh"
#include "signalmessage.h"
#include "testsender.hh"



Q_DECLARE_METATYPE (std::vector<Utils::SignalMessage>)
Q_DECLARE_METATYPE (std::vector<std::vector<Utils::SignalMessage> >)
Q_DECLARE_METATYPE (std::shared_ptr<PriorityLibraryStub>)


class SignalReaderTest : public QObject
{
    Q_OBJECT
    
public:
    SignalReaderTest();
    
public Q_SLOTS:
    void onTestFailed(const char* msg);
    
Q_SIGNALS:
    void sendSignal(QByteArray msg);
    
private Q_SLOTS:
    
    // Test cnstructor and destructor
    void constructorTest();
    
    // Test Signal handling without dealing with the actual message client. 
    // (one thread only).
    void bareSignalHandlingTest();
    void bareSignalHandlingTest_data();
    
    // Test parallel signalhandling without dealing with the actual 
    // message client.
    void bareSignalHandlingParallelTest();
    void bareSignalHandlingParallelTest_data();
    
    /* Thease tests do not work becase of deadlock problems with Qt signals.
     * There is no easy way to fix this in the test program.
     * Correct functionality is verified by hand in actual program.
    
    // Test functionality, when sending one message at a time.
    void serialTest();
    void serialTest_data();
    
    // Test functionality, when sending messages concurrently.
    void parallelTest();
    void parallelTest_data();
    
    */
    
private:
    // Aid functions:
    
    // Verify, that messages in the queue match the original messages.
    void verifyQueue(SignalHandler::SignalQueue* queue,
                     PriorityLibraryStub* lib,
                     const std::vector<Utils::SignalMessage>& messages);
    
    // Verify that given signal matches the original message.
    void verifySignal(const SignalHandler::Signal& s, 
                      const Utils::SignalMessage& msg, 
                      const SignalHandler::ScriptPriorityLibrary* lib);
    
    // Send given messages from another thread 
    // (using directly connected signal).
    void emitSignals(std::mutex* mx, std::condition_variable* cv, bool* ready,
                     const std::vector<Utils::SignalMessage> msgs);
    
    
    
    // Send given messages using the actual message client (in its own thread).
    void sendMessages(std::mutex* mx, std::condition_variable* cv, bool* ready,
                      const std::vector<Utils::SignalMessage> msgs);
};


SignalReaderTest::SignalReaderTest()
{
    Utils::Connection::setHost("127.0.0.1");
}


void SignalReaderTest::onTestFailed(const char* msg)
{
    QFAIL(msg);
}


void SignalReaderTest::constructorTest()
{
    using namespace SignalHandler;
    
    std::shared_ptr<SignalQueue> queue(new SignalQueue);
    std::shared_ptr<ScriptPriorityLibrary> lib(new PriorityLibraryStub);
    std::shared_ptr<PrioritySubjectStub> sub(new PrioritySubjectStub);
    SignalReader* reader_ptr = nullptr;
    
    {
        SignalReader reader(queue, lib.get(), sub.get());
        reader_ptr = &reader;
        QVERIFY2(sub->hasClient(reader_ptr), "Observer was not registered.");
    }
    // reader scope ends -> observer is unregistered.
    QVERIFY2(!sub->hasClient(reader_ptr), "Observer was not unregistered.");
}


void SignalReaderTest::bareSignalHandlingTest()
{
    using namespace SignalHandler;
    QFETCH (std::vector<Utils::SignalMessage>, messages);
    QFETCH (std::shared_ptr<PriorityLibraryStub>, library);
    
    std::shared_ptr<SignalQueue> queue(new SignalQueue);
    std::shared_ptr<PrioritySubjectStub> sub(new PrioritySubjectStub);
    SignalReader reader(queue, library.get(), sub.get());
    
    connect(this, SIGNAL(sendSignal(QByteArray)), 
            &reader, SLOT(onMessageReceived(QByteArray)) );
    
    // Send signals
    for (unsigned i=0; i<messages.size(); ++i){
        emit this->sendSignal(messages.at(i).data());
    }
    
    // Verify results.
    this->verifyQueue(queue.get(), library.get(), messages);
}


void SignalReaderTest::bareSignalHandlingTest_data()
{
    using namespace SignalHandler;
    
    QTest::addColumn<std::vector<Utils::SignalMessage> >("messages");
    QTest::addColumn<std::shared_ptr<PriorityLibraryStub> >("library");
    
    std::vector<Utils::SignalMessage> msgs;
    std::shared_ptr<PriorityLibraryStub> lib(new PriorityLibraryStub);
    
    // Generate test data.
    for (unsigned i=0; i<100; ++i){
        QString name = QString("script ") + QString::number(i);
        lib->addScript(name, i);
        msgs.push_back(Utils::SignalMessage(name, QString("sender"), 
                                            QStringList(), 
                                            QString("test_ack_group")));
    }
    
    QTest::newRow("100 signals") << msgs << lib;
}


void SignalReaderTest::bareSignalHandlingParallelTest()
{
    using namespace SignalHandler;
    QFETCH(std::vector<std::vector<Utils::SignalMessage> >, messages);
    QFETCH(std::shared_ptr<PriorityLibraryStub>, library);
    
    bool ready(false);
    std::mutex mx;
    std::condition_variable cv;
    std::shared_ptr<SignalQueue> queue(new SignalQueue);
    std::shared_ptr<PrioritySubjectStub> sub(new PrioritySubjectStub);
    SignalReader reader(queue, library.get(), sub.get());
    
    // Inter-thread signals must be connected using Qt::DirectConnection.
    connect(this, SIGNAL(sendSignal(QByteArray)), 
            &reader, SLOT(onMessageReceived(QByteArray)),
            Qt::DirectConnection);
    
    // Create threads
    std::vector<std::thread> threads( messages.size() );
    for (unsigned i=0; i<messages.size(); ++i){
        threads[i] = std::move( std::thread(&SignalReaderTest::emitSignals, 
                                            this, &mx, &cv, 
                                            &ready, messages[i]));
    }
    
    // Allow threads to start sending signals.
    mx.lock();
    ready = true;
    cv.notify_all();
    mx.unlock();
    
    // Wait for threads to finish.
    for (unsigned i=0; i<threads.size(); ++i){
        threads[i].join();
    }
    
    // Verify results
    std::vector<Utils::SignalMessage> all_msgs;
    for (unsigned i=0; i<messages.size(); ++i){
        std::move(messages[i].begin(), messages[i].end(), 
                  std::back_inserter(all_msgs));
    }
    this->verifyQueue(queue.get(), library.get(), all_msgs);
}


void SignalReaderTest::bareSignalHandlingParallelTest_data()
{
    using Utils::SignalMessage;
    QTest::addColumn<std::vector<std::vector<SignalMessage> > > ("messages");
    QTest::addColumn<std::shared_ptr<PriorityLibraryStub> > ("library");
    
    // Generate test data.
    std::shared_ptr<PriorityLibraryStub> lib(new PriorityLibraryStub);
    std::vector<std::vector<SignalMessage> > msgs(10);
    int j=0;
    for (unsigned i=0; i<1000; ++i){
        if (i != 0 && i%100 == 0){
            ++j;
        }
        QString name = QString("script ") + QString::number(i);
        lib->addScript(name, i);
        msgs[j].push_back(SignalMessage(name, QString("sender"), 
                                        QStringList(), 
                                        QString("test_ack_group")));
    }
    
    QTest::newRow("10x100 messages") << msgs << lib;
}


/* Thease tests do not work becase of deadlock problems with Qt signals.
 * There is no easy way to fix this in the test program.
 * Correct functionality is verified by hand in actual program.

void SignalReaderTest::serialTest()
{
    using namespace SignalHandler;
    QFETCH (std::vector<Utils::SignalMessage>, messages);
    QFETCH (std::shared_ptr<PriorityLibraryStub>, library);
    
    TestSender sender(messages);
    connect(&sender, SIGNAL(testFailed(const char*)), 
            this, SLOT(onTestFailed(const char*)) );
    std::shared_ptr<SignalQueue> queue(new SignalQueue);
    std::shared_ptr<PrioritySubjectStub> sub(new PrioritySubjectStub);
    SignalReader reader(queue, library.get(), sub.get());
    reader.start(QString("test_group"));
    sender.startSending();
    
    // Verify results.
    this->verifyQueue(queue.get(), library.get(), messages);
}


void SignalReaderTest::serialTest_data()
{
    this->bareSignalHandlingTest_data();
}


void SignalReaderTest::parallelTest()
{
    using namespace SignalHandler;
    QFETCH(std::vector<std::vector<Utils::SignalMessage> >, messages);
    QFETCH(std::shared_ptr<PriorityLibraryStub>, library);
    
    // Create reader and start reading.
    std::shared_ptr<SignalQueue> queue(new SignalQueue);
    std::shared_ptr<PrioritySubjectStub> sub(new PrioritySubjectStub);
    SignalReader reader(queue, library.get(), sub.get());    
    reader.start("test_group");
    
    // Create threads.
    bool ready(false);
    std::mutex mx;
    std::condition_variable cv;
    std::vector<std::thread> threads(messages.size());
    for (unsigned i=0; i<threads.size(); ++i){
        threads[i] = std::move(std::thread(&SignalReaderTest::sendMessages,
                                           this, &mx, &cv, &ready,
                                           messages[i]));
    }
    
    // Send all messages.
    mx.lock();
    ready = true;
    cv.notify_all();
    mx.unlock();
    for (unsigned i=0; i<threads.size(); ++i){
        threads[i].join();
    }
    
    // Verify results.
    std::vector<Utils::SignalMessage> all_messages;
    for (unsigned i=0; i<messages.size(); ++i){
        std::move(messages[i].begin(), messages[i].end(),
                  std::back_inserter(all_messages));
    }
    this->verifyQueue(queue.get(), library.get(), all_messages);
}


void SignalReaderTest::parallelTest_data()
{
    this->bareSignalHandlingParallelTest_data();
}

*/

void SignalReaderTest::
verifyQueue(SignalHandler::SignalQueue* queue, 
            PriorityLibraryStub* lib, 
            const std::vector<Utils::SignalMessage>& messages)
{
    unsigned counter(0);
    while (!queue->empty()){
        SignalHandler::Signal s = queue->pop();
        this->verifySignal(s, messages.at(counter), lib);
        ++counter;
    }
    QCOMPARE (counter, messages.size() );
}


void SignalReaderTest::
verifySignal(const SignalHandler::Signal& s, 
             const Utils::SignalMessage& msg,
             const SignalHandler::ScriptPriorityLibrary* lib)
{
    QCOMPARE(s.getScriptID(), msg.signalName());
    QCOMPARE(s.getParameters(), msg.parameters());
    QCOMPARE(s.getAckInfo().ackID, msg.ackId());
    QCOMPARE(s.getAckInfo().ackGroup, msg.ackGroup());
    QCOMPARE(s.getPriority(), lib->getPriorityOf(s.getScriptID()));
}


void SignalReaderTest::emitSignals(std::mutex* mx, 
                                   std::condition_variable* cv, 
                                   bool* ready,
                                   const std::vector<Utils::SignalMessage> msgs)
{
    // Wait for permission to start
    std::unique_lock<std::mutex> lock(*mx);
    while (!*ready){
        cv->wait(lock);
    }
    lock.unlock();
    
    // Emit signals
    for (Utils::SignalMessage msg : msgs){
        emit this->sendSignal(msg.data());
    }
}


void SignalReaderTest::sendMessages(std::mutex* mx, 
                                    std::condition_variable* cv, 
                                    bool* ready, 
                                    const std::vector<Utils::SignalMessage> msgs)
{
    // Wait for permission to start
    std::unique_lock<std::mutex> lock(*mx);
    while (!*ready){
        cv->wait(lock);
    }
    lock.unlock();
    
    // Send messages.
    TestSender sender(msgs);
    
    connect(&sender, SIGNAL(testFailed(const char*)),
            this, SLOT(onTestFailed(const char*)),
            Qt::DirectConnection);
    
    sender.startSending();
}


QTEST_APPLESS_MAIN(SignalReaderTest)

#include "tst_signalreadertest.moc"
