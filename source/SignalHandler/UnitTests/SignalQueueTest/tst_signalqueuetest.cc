#include <QString>
#include <QtTest>

#include "signalqueue.hh"
#include <vector>
#include <algorithm>
#include <thread>


Q_DECLARE_METATYPE(SignalHandler::Signal)

class SignalQueueTest : public QObject
{
    Q_OBJECT
    
public:
    SignalQueueTest();
    
private Q_SLOTS:
    
    // Test construction
    void constructorTest();
    
    // Test functionality with serialized operations
    void serialTest();
    void serialTest_data();
    
    // Test functionality with parallel read and write operations.
    void parallelTest();
    void parallelTest_data();
    
    // Test parallel writeing.
    void parallerWriteTest();
    void parallerWriteTest_data();
};


SignalQueueTest::SignalQueueTest()
{
}


void SignalQueueTest::constructorTest()
{
    SignalHandler::SignalQueue q;
    QVERIFY2(q.empty(), "New SignalQueue is not empty!");
}


void SignalQueueTest::serialTest()
{
    static std::vector<SignalHandler::Signal> all_input;
    static SignalHandler::SignalQueue q;
    
    // Fill SignalQueue
    QFETCH(SignalHandler::Signal, input);
    q.push(input);
    QVERIFY(!q.empty());
    all_input.push_back(input);
    
    // Pop all signals
    std::vector<SignalHandler::Signal> output;
    while (!q.empty()){
        output.push_back(q.pop());
    }
    
    std::vector<SignalHandler::Signal> expected_output = all_input;
    QVERIFY(expected_output.size() == output.size());
    QVERIFY(q.empty());
    
    // Sort expected output by priority.
    std::sort(expected_output.begin(), expected_output.end());
    std::reverse(expected_output.begin(), expected_output.end());
    
    // Verify order.
    for (unsigned i=0; i<output.size(); ++i){
        QCOMPARE(output[i].getScriptID(), expected_output[i].getScriptID());
        QCOMPARE(output[i].getPriority(), expected_output[i].getPriority());
        QCOMPARE(output[i].getParameters(), expected_output[i].getParameters());
    }
    
    // Re-fill queue
    for (SignalHandler::Signal s : all_input){
        q.push(s);
    }
}


void SignalQueueTest::serialTest_data()
{
    using SignalHandler::Signal;
    QTest::addColumn<SignalHandler::Signal>("input");
    
    QTest::newRow("s0") << Signal(9, QString("0"), QStringList(), Signal::AckInfo() );
    QTest::newRow("s1") << Signal(8, QString("1"), QStringList(), Signal::AckInfo() );
    QTest::newRow("s2") << Signal(7, QString("2"), QStringList(), Signal::AckInfo() );
    QTest::newRow("s3") << Signal(6, QString("3"), QStringList(), Signal::AckInfo() );
    QTest::newRow("s4") << Signal(5, QString("4"), QStringList(), Signal::AckInfo() );
    QTest::newRow("s5") << Signal(4, QString("5"), QStringList(), Signal::AckInfo() );
    QTest::newRow("s6") << Signal(3, QString("6"), QStringList(), Signal::AckInfo() );
    QTest::newRow("s7") << Signal(2, QString("7"), QStringList(), Signal::AckInfo() );
    QTest::newRow("s8") << Signal(1, QString("8"), QStringList(), Signal::AckInfo() );
    QTest::newRow("s9") << Signal(0, QString("9"), QStringList(), Signal::AckInfo() );
}



void read(bool* ready, std::mutex* ready_mx, std::condition_variable* cv,
          SignalHandler::SignalQueue* input,
          std::vector<SignalHandler::Signal>* output)
{
    // Wait for race to begin
    std::unique_lock<std::mutex> lock(*ready_mx);
    while(!*ready) cv->wait(lock);
    lock.unlock();
    
    // Read 10 signals
    for (int i =0; i<10; ++i){
        SignalHandler::Signal s = input->pop();
        output->push_back(s);
    }
}


void write(bool* ready, std::mutex* ready_mx, std::condition_variable* cv,
           const std::vector<SignalHandler::Signal>& input,
           SignalHandler::SignalQueue* output)
{
    // Wait for race to begin.
    std::unique_lock<std::mutex> lock(*ready_mx);
    while (!*ready) cv->wait(lock);
    lock.unlock();
    
    //Write all input signals
    for (unsigned i=0; i<input.size(); ++i){
        output->push( input[i] );
    }
}


void SignalQueueTest::parallelTest()
{
    using SignalHandler::Signal;
    
    // Divide input data equally for 5 writing threads
    static int counter(0);
    static std::vector<std::vector<Signal> > inputs(5);
    
    QFETCH(Signal, input);
    inputs[counter++ % 5].push_back(input);
    
    // Actual tests are run when all data is fetched.
    if (counter < 50) return;
    
    qDebug() << "Start tests.";
    bool ready(false);
    std::mutex ready_mx;
    std::condition_variable cv;
    SignalHandler::SignalQueue q;
    std::vector<std::vector<Signal> > outputs(5);
    
    // create reader and writer threads
    std::vector<std::thread> readers(5);
    std::vector<std::thread> writers(5);
    for (int i=0; i<5; ++i){
        readers[i] = std::thread(read, &ready, &ready_mx, &cv, &q, &outputs[i]);
        writers[i] = std::thread(write, &ready, &ready_mx, &cv, inputs[i], &q);
    }
    
    // Start race
    ready_mx.lock();
    ready = true;
    ready_mx.unlock();
    cv.notify_all();
    
    // Wait until all threads are finished.
    for (int i=0; i<5; ++i){
        writers[i].join();
        readers[i].join();
    }
    
    // Gather outputs and inputs.
    std::vector<Signal> final_output;
    std::vector<Signal> all_input;
    for (int i=0; i<5; ++i){
        std::vector<Signal> tmp_o = outputs[i];
        std::vector<Signal> tmp_i = inputs[i];
        std::move(tmp_o.begin(), tmp_o.end(), std::back_inserter(final_output));
        std::move(tmp_i.begin(), tmp_i.end(), std::back_inserter(all_input));
    }
    
    // Compare inputs and outputs
    QVERIFY( all_input.size() == final_output.size() );
    QVERIFY( std::is_permutation(all_input.begin(), all_input.end(), 
                                 final_output.begin(),
                                 [](Signal s1, Signal s2)
             {
                 return s1.getScriptID() == s2.getScriptID() &&
                        s1.getPriority() == s2.getPriority() &&
                        s1.getParameters() == s2.getParameters();
             }
             ) 
            );
}


void SignalQueueTest::parallelTest_data()
{
    using SignalHandler::Signal;
    QTest::addColumn<SignalHandler::Signal>("input");
    
    QTest::newRow("s0") << Signal(9, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s1") << Signal(8, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s2") << Signal(7, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s3") << Signal(6, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s4") << Signal(5, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s5") << Signal(4, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s6") << Signal(3, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s7") << Signal(2, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s8") << Signal(1, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s9") << Signal(0, QString("0"), QStringList(), Signal::AckInfo());
    
    QTest::newRow("s10") << Signal(9, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s11") << Signal(8, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s12") << Signal(7, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s13") << Signal(6, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s14") << Signal(5, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s15") << Signal(4, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s16") << Signal(3, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s17") << Signal(2, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s18") << Signal(1, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s19") << Signal(0, QString("0"), QStringList(), Signal::AckInfo());
    
    QTest::newRow("s20") << Signal(9, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s21") << Signal(8, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s22") << Signal(7, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s23") << Signal(6, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s24") << Signal(5, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s25") << Signal(4, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s26") << Signal(3, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s27") << Signal(2, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s28") << Signal(1, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s29") << Signal(0, QString("0"), QStringList(), Signal::AckInfo());
    
    QTest::newRow("s30") << Signal(9, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s31") << Signal(8, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s32") << Signal(7, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s33") << Signal(6, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s34") << Signal(5, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s35") << Signal(4, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s36") << Signal(3, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s37") << Signal(2, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s38") << Signal(1, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s39") << Signal(0, QString("0"), QStringList(), Signal::AckInfo());
    
    QTest::newRow("s40") << Signal(9, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s41") << Signal(8, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s42") << Signal(7, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s43") << Signal(6, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s44") << Signal(5, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s45") << Signal(4, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s46") << Signal(3, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s47") << Signal(2, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s48") << Signal(1, QString("0"), QStringList(), Signal::AckInfo());
    QTest::newRow("s49") << Signal(0, QString("0"), QStringList(), Signal::AckInfo());
}



void SignalQueueTest::parallerWriteTest()
{
    using SignalHandler::Signal;
    const int INPUT_COUNT(50);
    
    static std::vector<Signal> all_input(INPUT_COUNT);
    static int counter(0);
    
    // Fetch all signals before running tests.
    QFETCH(Signal, input);
    all_input[counter] = input;
    if (++counter < INPUT_COUNT) return;
    
    // Shuffle input
    std::shuffle(all_input.begin(), all_input.end(), 
                 std::default_random_engine() );
    
    // distribute input for 5 writer threads
    std::vector<std::vector<Signal> > distributed(5);
    for (int i=0; i<INPUT_COUNT; ++i){
        distributed[i%5].push_back( all_input[i] );
    }
    
    // Start tests
    bool ready(false);
    std::mutex ready_mx;
    std::condition_variable cv;
    SignalHandler::SignalQueue q;
    
    // Crete threads
    std::vector<std::thread> threads(5);
    for (int i=0; i<5; ++i){
        threads[i] = std::thread(write, &ready, &ready_mx, 
                                 &cv, distributed[i], &q);
    }
    
    ready_mx.lock();
    ready = true;
    ready_mx.unlock();
    cv.notify_all();
    qDebug() << "Tests start.";
    
    // Wait for all threads to finish.
    for (int i=0; i<5; ++i){
        threads[i].join();
    }
    
    std::vector<Signal> result;
    while ( !q.empty() ){
        result.push_back( q.pop() );
    }
    
    // Verify that result is sorted by priority.
    QVERIFY(result.size() == INPUT_COUNT);
    Signal previous = result.front();
    for (int i=1; i<INPUT_COUNT; ++i){
        Signal s = result[i];
        QVERIFY(previous.getPriority() <= s.getPriority());
    }
}


void SignalQueueTest::parallerWriteTest_data()
{
    parallelTest_data();
}


QTEST_APPLESS_MAIN(SignalQueueTest)

#include "tst_signalqueuetest.moc"
