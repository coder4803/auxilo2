#include <QString>
#include <QtTest>

#include "signal.hh"
#include "signalparser.hh"
#include "priorityupdatesubject.hh"
#include "scriptprioritylibrary.hh"
#include <memory>


// Dummy subject, for tests could compile.
class DummyUpdateSubject : public SignalHandler::PriorityUpdateSubject
{
public:
    DummyUpdateSubject(): PriorityUpdateSubject(), client_(nullptr){}
    virtual ~DummyUpdateSubject(){}
    
    virtual void registerClient(SignalHandler::PriorityUpdateObserver* obs) 
    {
        client_ = obs;
    }
    
    virtual void unregisterClient(SignalHandler::PriorityUpdateObserver* obs) 
    {
        Q_UNUSED(obs);
        client_ = nullptr;
    }
    
    SignalHandler::PriorityUpdateObserver* getClient() const 
    {
        return client_;
    }
    
private:
    SignalHandler::PriorityUpdateObserver* client_;
};



class DummyLib : public SignalHandler::ScriptPriorityLibrary
{
public:
    DummyLib() : SignalHandler::ScriptPriorityLibrary() {}
    virtual ~DummyLib() {}
    virtual unsigned getPriorityOf(unsigned int scriptID) const
    {
        Q_UNUSED(scriptID);
        return 1u;
    }
};


class SignalTest : public QObject
{
    Q_OBJECT
    
public:
    SignalTest();
    
private Q_SLOTS:
    
    // Test Parser constructor and destructor.
    void parserConstructionDestruction();
    
    // Test parsing valid messages
    void validParseTest();
    void validParseTest_data();
    
    // Test parsing invalid messages
    void invalidParseTest();
    void invalidParseTest_data();
    
    // Test priority comparison.
    void comparisonTest();
};


SignalTest::SignalTest()
{
}


void SignalTest::parserConstructionDestruction()
{
    DummyLib lib;
    DummyUpdateSubject subject;
    SignalHandler::PriorityUpdateObserver* nil(nullptr);
    {
        // Construction with subject.
        SignalHandler::SignalParser parser(&lib, &subject);
        QCOMPARE(subject.getClient(), &parser);
    }
    QCOMPARE(subject.getClient(), nil);
    
    // Construction without subject
    SignalHandler::SignalParser parser(&lib, nullptr);
    QCOMPARE(subject.getClient(), nil);
}


void SignalTest::validParseTest()
{
    DummyLib lib;
    
    QFETCH(QString, message);
    QFETCH(unsigned int, scriptID);
    QFETCH(std::vector<QString>, parameters);
    
    SignalHandler::SignalParser parser(&lib, nullptr);
    SignalHandler::Signal s = parser.parse(message);
    
    QCOMPARE(s.getPriority(), lib.getPriorityOf(scriptID));
    QCOMPARE(s.getScriptID(), scriptID);
    QCOMPARE(s.getParameters(), parameters);
}


void SignalTest::validParseTest_data()
{
    QTest::addColumn<QString>("message");
    QTest::addColumn<unsigned int>("scriptID");
    QTest::addColumn<std::vector<QString>>("parameters");
    
    //              Row Name                 Message         ScriptID        Parameters
    QTest::newRow("ID0, 0args")     << QString("0")           << 0u     << std::vector<QString>();
    QTest::newRow("ID0, 3args")     << QString("0;a;ab;")     << 0u     << std::vector<QString>({QString("a"), QString("ab"), QString("")});    
    QTest::newRow("IDMid, 0args")   << QString("10")          << 10u    << std::vector<QString>();
    QTest::newRow("IDMid, 3args")   << QString("10;a;ab;")    << 10u    << std::vector<QString>({QString("a"), QString("ab"), QString("")});
    QTest::newRow("IDMax, 0args")   << QString("65535")       << 65535u << std::vector<QString>();
    QTest::newRow("IDMax, 3args")   << QString("65535;a;ab;") << 65535u << std::vector<QString>({QString("a"), QString("ab"), QString("")});  
}



void SignalTest::invalidParseTest()
{
    DummyLib lib;
    SignalHandler::SignalParser parser(&lib, nullptr);
    
    QFETCH(QString, message);
    
    try{
        SignalHandler::Signal s = parser.parse(message);
        QFAIL("No exception from invalid message");
    }
    catch(SignalHandler::BadMessage& e){
        QCOMPARE(e.what(), "SignalHandler::BadMessage");
        QCOMPARE(e.getInvalidMessage(), message);
    }
    catch(...){
        QFAIL("Exception is of a wrong type");
    }
}


void SignalTest::invalidParseTest_data()
{
    QTest::addColumn<QString>("message");
    
    //              Row Name                    Message
    QTest::newRow("empty")                 << QString("");
    QTest::newRow("negative scriptID")     << QString("-1");
    QTest::newRow("scriptID not a number") << QString("a");
}


void SignalTest::comparisonTest()
{
    SignalHandler::Signal s1(0,0, std::vector<QString>() );
    SignalHandler::Signal s2(1,0, std::vector<QString>() ); 
    SignalHandler::Signal s3(0,0, std::vector<QString>() );
    
    QVERIFY2(s2 < s1, "Comparison does not work");
    QVERIFY2(!(s1 < s2), "Comparison does not work");
    QVERIFY2(!(s1 < s3), "Comparison does not work");
    QVERIFY2(!(s3 < s1), "Comparison does not work");
}


QTEST_APPLESS_MAIN(SignalTest)

#include "tst_signaltest.moc"
