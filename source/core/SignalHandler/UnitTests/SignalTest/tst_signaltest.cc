#include <QString>
#include <QtTest>

#include "signal.hh"

class SignalTest : public QObject
{
    Q_OBJECT
    
public:
    SignalTest();
    
private Q_SLOTS:
    
    // Test priority comparison.
    void comparisonTest();
};


SignalTest::SignalTest()
{
}


void SignalTest::comparisonTest()
{
    using SignalHandler::Signal;
    
    Signal s1(0, QString("a"), QStringList(), Signal::AckInfo() );
    Signal s2(1, QString("b"), QStringList(), Signal::AckInfo() ); 
    Signal s3(0, QString("c"), QStringList(), Signal::AckInfo() );
    
    QVERIFY2(s2 < s1, "Comparison does not work");
    QVERIFY2(!(s1 < s2), "Comparison does not work");
    QVERIFY2(!(s1 < s3), "Comparison does not work");
    QVERIFY2(!(s3 < s1), "Comparison does not work");
}


QTEST_APPLESS_MAIN(SignalTest)

#include "tst_signaltest.moc"
