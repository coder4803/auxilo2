#include <QString>
#include <QtTest>

#include "signal.hh"
#include "signalparser.hh"


class SignalTest : public QObject
{
    Q_OBJECT
    
public:
    SignalTest();
    
private Q_SLOTS:
    
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


void SignalTest::validParseTest()
{
    QFETCH(QString, message);
    QFETCH(unsigned int, priority);
    QFETCH(unsigned int, scriptID);
    QFETCH(std::vector<QString>, parameters);
    
    SignalHandler::Signal s = SignalHandler::SignalParser::parse(message);
    
    QCOMPARE(s.getPriority(), priority);
    QCOMPARE(s.getScriptID(), scriptID);
    QCOMPARE(s.getParameters(), parameters);
}


void SignalTest::validParseTest_data()
{
    QTest::addColumn<QString>("message");
    QTest::addColumn<unsigned int>("priority");
    QTest::addColumn<unsigned int>("scriptID");
    QTest::addColumn<std::vector<QString>>("parameters");
    
    //              Row Name                 Message                    Priority    ScriptID        Parameters
    QTest::newRow("pr0, ID0, 0args")     << QString("0;0")               << 0u     << 0u     << std::vector<QString>();
    QTest::newRow("pr0, ID0, 3args")     << QString("0;0;a;ab;")         << 0u     << 0u     << std::vector<QString>({QString("a"), QString("ab"), QString("")});    
    QTest::newRow("pr0, IDMid, 0args")   << QString("0;10")              << 0u     << 10u    << std::vector<QString>();
    QTest::newRow("pr0, IDMid, 3args")   << QString("0;10;a;ab;")        << 0u     << 10u    << std::vector<QString>({QString("a"), QString("ab"), QString("")});
    QTest::newRow("pr0, IDMax, 0args")   << QString("0;65535")           << 0u     << 65535u << std::vector<QString>();
    QTest::newRow("pr0, IDMax, 3args")   << QString("0;65535;a;ab;")     << 0u     << 65535u << std::vector<QString>({QString("a"), QString("ab"), QString("")});
    
    
    QTest::newRow("prMid, ID0, 0args")   << QString("10;0")              << 10u    << 0u     << std::vector<QString>();
    QTest::newRow("prMid, ID0, 3args")   << QString("10;0;a;ab;")        << 10u    << 0u     << std::vector<QString>({QString("a"), QString("ab"), QString("")});
    QTest::newRow("prMid, IDMid, 0args") << QString("10;10")             << 10u    << 10u    << std::vector<QString>();
    QTest::newRow("prMid, IDMid, 3args") << QString("10;10;a;ab;")       << 10u    << 10u    << std::vector<QString>({QString("a"), QString("ab"), QString("")});    
    QTest::newRow("prMid, IDMax, 0args") << QString("10;65535")          << 10u    << 65535u << std::vector<QString>();
    QTest::newRow("prMid, IDMax, 3args") << QString("10;65535;a;ab;")    << 10u    << 65535u << std::vector<QString>({QString("a"), QString("ab"), QString("")});    
    
    
    QTest::newRow("prMax, ID0, 0args")   << QString("65535;0")           << 65535u << 0u     << std::vector<QString>();
    QTest::newRow("prMax, ID0, 3args")   << QString("65535;0;a;ab;")     << 65535u << 0u     << std::vector<QString>({QString("a"), QString("ab"), QString("")});    
    QTest::newRow("prMax, IDMid, 0args") << QString("65535;10")          << 65535u << 10u    << std::vector<QString>();
    QTest::newRow("prMax, IDMid, 3args") << QString("65535;10;a;ab;")    << 65535u << 10u    << std::vector<QString>({QString("a"), QString("ab"), QString("")});    
    QTest::newRow("prMax, IDMax, 0args") << QString("65535;65535")       << 65535u << 65535u << std::vector<QString>();
    QTest::newRow("prMax, IDMax, 3args") << QString("65535;65535;a;ab;") << 65535u << 65535u << std::vector<QString>({QString("a"), QString("ab"), QString("")});    
}



void SignalTest::invalidParseTest()
{
    QFETCH(QString, message);
    
    try{
        SignalHandler::Signal s = SignalHandler::SignalParser::parse(message);
        QFAIL("No exception from invalid message");
    }
    catch(SignalHandler::BadMessage& e){
        QCOMPARE(e.what(), "SignalHandler::BadMessage");
        std::string msg = e.getInvalidMessage();
        QCOMPARE(msg, message.toStdString());
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
    QTest::newRow("only 1 field")          << QString("0");
    QTest::newRow("negative priority")     << QString("-1;0");
    QTest::newRow("priority not a number") << QString("a;0");
    QTest::newRow("negative scriptID")     << QString("0;-1");
    QTest::newRow("scriptID not a number") << QString("0;a");
}


void SignalTest::comparisonTest()
{
    SignalHandler::Signal s1(0,0, std::vector<QString>() );
    SignalHandler::Signal s2(1,0, std::vector<QString>() ); 
    SignalHandler::Signal s3(0,0, std::vector<QString>() );
    
    QVERIFY2(s1 < s2, "Comparison does not work");
    QVERIFY2(!(s2 < s1), "Comparison does not work");
    QVERIFY2(!(s1 < s3), "Comparison does not work");
    QVERIFY2(!(s3 < s1), "Comparison does not work");
}


QTEST_APPLESS_MAIN(SignalTest)

#include "tst_signaltest.moc"
