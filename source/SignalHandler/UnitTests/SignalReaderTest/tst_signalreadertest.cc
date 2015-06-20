#include <QString>
#include <QtTest>

class SignalReaderTest : public QObject
{
    Q_OBJECT
    
public:
    SignalReaderTest();
    
private Q_SLOTS:
    void testCase1();
};

SignalReaderTest::SignalReaderTest()
{
}

void SignalReaderTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(SignalReaderTest)

#include "tst_signalreadertest.moc"
