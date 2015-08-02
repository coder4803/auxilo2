#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "configurationreader.hh"

Q_DECLARE_METATYPE(std::shared_ptr<Utils::ConfResponseMessage>)


/*!
 * \brief The ConfigurationReaderTest class
 *  This is a test Qt-Test class for testing the ConfigurationReader class.
 *  Class tests construction and receiving ConfResponseMessages.
 *  Receiving simultanious messages is tested also.
 */
class ConfigurationReaderTest : public QObject
{
    Q_OBJECT
    
public:
    ConfigurationReaderTest();
    
public Q_SLOTS:
    
    // This slot replaces non-thread-safe QSignalSpy.
    void onConfReady();
    
signals:
    
    //! This signal emulates sending message via messager app.
    void confResponseMessage(QByteArray payload);
    
private Q_SLOTS:
    
    //! Test constructor.
    void constructorTest();
    void constructorTest_data();
    
    //! Test ConfResponseMessage handling without actual messager server.
    void bareConfigurationTest();
    void bareConfigurationTest_data();
    
    //! Verify that sending another ConfResponseMessage replaces older one.
    void bareReconfigurationTest();
    void bareReconfigurationTest_data();
    
    //! Test concurrent configuration.
    void concurrentConfigurationTest();
    void concurrentConfigurationTest_data();
    
    
private:
    
    // Thease variables are needed to replace non-thread-safe SignalSpy.
    std::mutex mx_;
    int signalCount_;
    
    // Send ConfResponseMessage (emit directly connected signal).
    void sendMsg(bool* ready, std::mutex* mx, 
                 std::condition_variable* cv,
                 Utils::ConfResponseMessage* msg);
    
    // Reset signalCounter_
    void resetCounter();
};


ConfigurationReaderTest::ConfigurationReaderTest() : mx_(), signalCount_(0)
{
}


void ConfigurationReaderTest::onConfReady()
{
    std::lock_guard<std::mutex> lock(mx_);
    ++signalCount_;
}


void ConfigurationReaderTest::constructorTest()
{
    QFETCH(QObject*, parent);
    SignalHandler::ConfigurationReader reader(parent);
    
    QVERIFY2(reader.getConfiguration().parameters().empty(),
             "Configuration is not empty after construction.");
    
    QCOMPARE(reader.getConfiguration().featureName(),
             Utils::ParameterSet().featureName());
    
    QCOMPARE(reader.parent(), parent);
}


void ConfigurationReaderTest::constructorTest_data()
{
    QTest::addColumn<QObject*>("parent");
    QTest::newRow("null parent") << static_cast<QObject*> (nullptr);
    QTest::newRow("real parent") << static_cast<QObject*> (this);
}


void ConfigurationReaderTest::bareConfigurationTest()
{
    using Params = QHash<QString,QString>;
    QFETCH (Params, parameters);
    QFETCH (QString, feature);
    
    Utils::ParameterSet paramset(feature);
    paramset.appendParameters(parameters);
    Utils::ConfResponseMessage msg(paramset);
    
    SignalHandler::ConfigurationReader reader;
    QSignalSpy spy(&reader, SIGNAL(configurationUpdated()) );
    QVERIFY(spy.isValid());
    
    connect(this, SIGNAL(confResponseMessage(QByteArray)),
            &reader, SLOT(onConfMessageReceived(QByteArray)), 
            Qt::DirectConnection);
    
    emit this->confResponseMessage( msg.data() );
    Utils::ParameterSet conf = reader.getConfiguration();
    
    QCOMPARE(spy.count(), 1);
    QCOMPARE(conf.featureName(), feature);
    QCOMPARE(conf.parameters(), parameters);
}


void ConfigurationReaderTest::bareConfigurationTest_data()
{
    using Params = QHash<QString,QString>;
    QTest::addColumn<Params>("parameters");
    QTest::addColumn<QString>("feature");
    
    QTest::newRow("Empty params") << Params() << QString("empty feature");
    
    QTest::newRow("1 parameter") 
            << Params({{QString("name0"), QString("value0")}}) 
            << QString("1 parameter feature");
    
    QTest::newRow("10 parameters")
            << Params({
                          {QString("name0"), QString("value0")},
                          {QString("name1"), QString("value1")},
                          {QString("name2"), QString("value2")},
                          {QString("name3"), QString("value3")},
                          {QString("name4"), QString("value4")},
                          {QString("name5"), QString("value5")},
                          {QString("name6"), QString("value6")},
                          {QString("name7"), QString("value7")},
                          {QString("name8"), QString("value8")},
                          {QString("name9"), QString("value9")} 
                      })
            << QString("10 parameters feature");
}


void ConfigurationReaderTest::bareReconfigurationTest()
{
    using Params = QHash<QString,QString>;
    QFETCH(Params, original_params);
    QFETCH(QString, original_feature);
    QFETCH(Params, new_params);
    QFETCH(QString, new_feature);
    
    Utils::ParameterSet origSet(original_feature);
    origSet.appendParameters(original_params);
    Utils::ConfResponseMessage origMsg(origSet);
    
    Utils::ParameterSet newSet(new_feature);
    newSet.appendParameters(new_params);
    Utils::ConfResponseMessage newMsg(newSet);
    
    SignalHandler::ConfigurationReader reader(nullptr);
    QSignalSpy spy(&reader, SIGNAL(configurationUpdated()) );
    QVERIFY( spy.isValid() );
    
    connect(this, SIGNAL(confResponseMessage(QByteArray)),
            &reader, SLOT(onConfMessageReceived(QByteArray)),
            Qt::DirectConnection );
    
    // Send first configuation.
    emit this->confResponseMessage( origMsg.data() );
    Utils::ParameterSet firstConf = reader.getConfiguration();
    QCOMPARE ( firstConf.featureName(), origSet.featureName() );
    QCOMPARE ( firstConf.parameters(), origSet.parameters() );
    
    //Send second configuration.
    emit this->confResponseMessage( newMsg.data() );
    Utils::ParameterSet secondConf = reader.getConfiguration();
    QCOMPARE ( spy.count(), 2 );
    QCOMPARE ( secondConf.featureName(), newSet.featureName() );
    QCOMPARE ( secondConf.parameters(), newSet.parameters() );
}


void ConfigurationReaderTest::bareReconfigurationTest_data()
{
    using Params = QHash<QString, QString>;
    QTest::addColumn<Params>("original_params");
    QTest::addColumn<QString>("original_feature");
    QTest::addColumn<Params>("new_params");
    QTest::addColumn<QString>("new_feature");
    
    Params original_p = Params({{QString("origName"), QString("origValue")}});
    QString original_f = QString("origFeature");
    
    QTest::newRow("emptyReconf") << original_p << original_f 
                                 << Params() << QString("empty feature");
    
    QTest::newRow("non-empty reconf") << original_p << original_f
                                      << Params(
                                            {{QString("n0"), QString("val0")},
                                             {QString("n1"), QString("val1")},
                                             {QString("n2"), QString("val2")},
                                             {QString("n3"), QString("val3")},
                                             {QString("n4"), QString("val4")},
                                             {QString("n5"), QString("val5")},
                                             {QString("n6"), QString("val6")},
                                             {QString("n7"), QString("val7")},
                                             {QString("n8"), QString("val8")},
                                             {QString("n9"), QString("val9")}
                                            } )
                                       << QString("new feature");
                                   
    
}


void ConfigurationReaderTest::concurrentConfigurationTest()
{
    using ConfMsgPtr = std::shared_ptr<Utils::ConfResponseMessage>;
    QFETCH (QList<ConfMsgPtr>, messages);
    
    SignalHandler::ConfigurationReader reader;
    connect(this, SIGNAL(confResponseMessage(QByteArray)),
            &reader, SLOT(onConfMessageReceived(QByteArray)),
            Qt::DirectConnection );
    // SignalSpy cannot be used in a multi-threaded test.
    // Connecting to onConfReady replaces QSignalSpy.
    connect(&reader, SIGNAL(configurationUpdated()),
            this, SLOT(onConfReady()), Qt::DirectConnection );
    
    // Init threads
    bool ready(false);
    std::mutex mx;
    std::condition_variable cv;
    std::vector<std::thread> threads(messages.size());
    for (int i=0; i<messages.size(); ++i){
        threads[i] = std::move(std::thread(&ConfigurationReaderTest::sendMsg,
                                           this, &ready, &mx, &cv, 
                                           messages[i].get() ) 
                               );
    }
    
    // Send messages concurrently.
    mx.lock();
    ready = true;
    cv.notify_all();
    mx.unlock();
    for (unsigned i=0; i<threads.size(); ++i){
        threads[i].join();
    }
    
    // Verify results
    Utils::ParameterSet conf = reader.getConfiguration();
    QCOMPARE(this->signalCount_, messages.size());
    
    QVERIFY2( std::find_if(messages.begin(), messages.end(),
                           [&conf](ConfMsgPtr msg)
                {   
                  return msg->parameterSet().featureName()==conf.featureName() &&
                        msg->parameterSet().parameters()==conf.parameters();
                  
                }
              ) != messages.end(),
            "Final configuration is not any of the sent configurations."
            );
    
    this->resetCounter();
}


void ConfigurationReaderTest::concurrentConfigurationTest_data()
{
    using ConfMsgPtr = std::shared_ptr<Utils::ConfResponseMessage>;
    QTest::addColumn<QList<ConfMsgPtr>>("messages");
    
    QList<ConfMsgPtr> list;
    int i=0;
    for (i=0; i<2; ++i){
        Utils::ParameterSet params(QString("feature") + QString::number(i));
        params.appendParameter(QString("name") +QString::number(i), 
                               QString("val") + QString::number(i) );
        list.append( std::make_shared<Utils::ConfResponseMessage>
                     (Utils::ConfResponseMessage(params)) );
    }
    QTest::newRow("2 messages") << list;
    
    for (; i<10; ++i){
        Utils::ParameterSet params(QString("feature") + QString::number(i));
        params.appendParameter(QString("name") +QString::number(i), 
                               QString("val") + QString::number(i) );
        list.append( std::make_shared<Utils::ConfResponseMessage>
                     (Utils::ConfResponseMessage(params)) );
    }
    QTest::newRow("10 messages") << list;
    
    for (; i<100; ++i){
        Utils::ParameterSet params(QString("feature") + QString::number(i));
        params.appendParameter(QString("name") +QString::number(i), 
                               QString("val") + QString::number(i) );
        list.append( std::make_shared<Utils::ConfResponseMessage>
                     (Utils::ConfResponseMessage(params)) );
    }
    QTest::newRow("100 messages") << list;
    
    for (; i<500; ++i){
        Utils::ParameterSet params(QString("feature") + QString::number(i));
        params.appendParameter(QString("name") +QString::number(i), 
                               QString("val") + QString::number(i) );
        list.append( std::make_shared<Utils::ConfResponseMessage>
                     (Utils::ConfResponseMessage(params)) );
    }
    QTest::newRow("500 messages") << list;
}


void ConfigurationReaderTest::sendMsg(bool* ready, std::mutex* mx, 
                                      std::condition_variable* cv, 
                                      Utils::ConfResponseMessage* msg)
{
    // Wait for permission to start.
    std::unique_lock<std::mutex> lock(*mx);
    while (!*ready){
        cv->wait(lock);
    }
    lock.unlock();
    
    emit this->confResponseMessage(msg->data());
}


void ConfigurationReaderTest::resetCounter()
{
    std::lock_guard<std::mutex> lock(mx_);
    signalCount_ = 0;
}


QTEST_APPLESS_MAIN(ConfigurationReaderTest)

#include "tst_configurationreadertest.moc"
