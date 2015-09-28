/* confreadertest.cpp
 * Unit tests for the SignalGenerator::ConfReader and related classes
 * (ConfReader, ConfXmlHandler and EvetEntity).
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 28-Sep-2015
 */

#include <QString>
#include <QtTest>

#include "confreader.h"
#include "confresponsemessage.h"
#include <QSignalSpy>


Q_DECLARE_METATYPE(Utils::ParameterSet)
Q_DECLARE_METATYPE(QList<SignalGenerator::EventEntity>)


class ConfReaderTest : public QObject
{
    Q_OBJECT

public:
    ConfReaderTest();

signals:

    void testSignal(QByteArray data);

private Q_SLOTS:

    // Test EventEntity::isValidInterval
    void isValidIntervalTest();
    void isValidIntervalTest_data();

    // Test EventEntity::parseDateTime
    void parseDateTimeTest();
    void parseDateTimeTest_data();

    // Test ConfReader receiving invalid configuration.
    void invalidConfigurationTest();
    void invalidConfigurationTest_data();

    // Test ConfReader receiving valid configuration.
    void validConfigurationTest();
    void validConfigurationTest_data();
};


ConfReaderTest::ConfReaderTest()
{
}


void ConfReaderTest::isValidIntervalTest()
{
    QFETCH(QString, interval);
    QFETCH(bool, result);

    bool actual = SignalGenerator::EventEntity::isValidInterval(interval);
    QVERIFY(actual == result);
    QVERIFY( !interval.endsWith("s") );
}


void ConfReaderTest::isValidIntervalTest_data()
{
    QTest::addColumn<QString>("interval");
    QTest::addColumn<bool>("result");

    // Valid
    QTest::newRow("sec")            << "1 second"       << true;
    QTest::newRow("secs")           << "10 seconds"     << true;
    QTest::newRow("min")            << "1 minute"       << true;
    QTest::newRow("mins")           << "10 minutes"     << true;
    QTest::newRow("hour")           << "1 hour"         << true;
    QTest::newRow("hours")          << "10 hours"       << true;
    QTest::newRow("day")            << "1 day"          << true;
    QTest::newRow("days")           << "10 days"        << true;
    QTest::newRow("month")          << "1 month"        << true;
    QTest::newRow("months")         << "10 months"      << true;
    QTest::newRow("year")           << "1 year"         << true;
    QTest::newRow("years")          << "10 years"       << true;
    // Invalid
    QTest::newRow("no factor")      << " second"        << false;
    QTest::newRow("no unit")        << "10"             << false;
    QTest::newRow("unknown unit")   << "10 unknown"     << false;
    QTest::newRow("empty")          << ""               << false;
}


void ConfReaderTest::parseDateTimeTest()
{
    QFETCH(QString, str);
    QFETCH(QDate, date);
    QFETCH(QTime, time);
    QFETCH(bool, expected);

    QDateTime datetime = SignalGenerator::EventEntity::parseDateTime(str);
    QVERIFY(datetime.isValid() == expected);
    if (expected == false){
        return;
    }
    QCOMPARE(datetime.date(), date);
    QCOMPARE(datetime.time(), time);
}


void ConfReaderTest::parseDateTimeTest_data()
{
   QTest::addColumn<QString>("str");
   QTest::addColumn<QDate>("date");
   QTest::addColumn<QTime>("time");
   QTest::addColumn<bool>("expected");

   QTest::newRow("full datetime")   << "01-01-2000 00:00:00"    << QDate(2000, 1, 1)    << QTime(0,0,0)     << true;
   QTest::newRow("time only")       << "12:00:00"               << QDate::currentDate() << QTime(12,0,0)    << true;
   QTest::newRow("empty")           << ""                       << QDate()              << QTime()          << false;
}


void ConfReaderTest::invalidConfigurationTest()
{
    QFETCH(Utils::ParameterSet, parameters);
    qDebug() << ""; // Print empty lines to make output cleaner.
    qDebug() << "";

    Utils::ConfResponseMessage msg(parameters);
    SignalGenerator::ConfReader reader;
    QSignalSpy spy(&reader, SIGNAL(configurationReady()) );

    connect(this, SIGNAL(testSignal(QByteArray)),
            &reader, SLOT(onConfResponseReceived(QByteArray)),
            Qt::DirectConnection );

    emit this->testSignal( msg.data() );

    QCOMPARE(spy.count(), 0);
    QVERIFY(reader.getConfiguration().isEmpty());
}


void ConfReaderTest::invalidConfigurationTest_data()
{
    QTest::addColumn<Utils::ParameterSet>("parameters");

    const QString PREFIX = "../confreadertest/testfiles/invalid/";

    QStringList files = {"not_exist", "empty.xml", "no_main_element.xml",
                        "unknown_main.xml", "unknown_element.xml",
                        "no_signal.xml", "no_timestamp.xml",
                        "negative_repeat.xml", "invalid_timestamp.xml",
                        "invalid_interval.xml", "unknown_attribute.xml",
                        "dublicates.xml"};

    for (QString f_name : files){
        Utils::ParameterSet tmp;
        tmp.setFeatureName("signalGenerator");
        tmp.appendParameter("configurationFile", PREFIX+f_name);
        QTest::newRow(f_name.toStdString().data()) << tmp;
    }

    {
        Utils::ParameterSet invalid;
        invalid.setFeatureName("signalGenerator");
        // No mandatory parameter: "configurationFile"
        QTest::newRow("File not defined") << invalid;
    }
}


void ConfReaderTest::validConfigurationTest()
{
    QFETCH(Utils::ParameterSet, parameters);
    QFETCH(QList<SignalGenerator::EventEntity>, expected);
    qDebug() << ""; // Print empty lines to make output cleaner.
    qDebug() << "";

    Utils::ConfResponseMessage msg(parameters);
    SignalGenerator::ConfReader reader;
    QSignalSpy spy(&reader, SIGNAL(configurationReady()) );

    connect(this, SIGNAL(testSignal(QByteArray)),
            &reader, SLOT(onConfResponseReceived(QByteArray)),
            Qt::DirectConnection );

    emit testSignal(msg.data());
    QCOMPARE(spy.count(), 1);

    QList<SignalGenerator::EventEntity> conf = reader.getConfiguration();
    foreach (SignalGenerator::EventEntity e, conf) {
        QVERIFY( expected.contains(e) );
    }
}


void ConfReaderTest::validConfigurationTest_data()
{
    QTest::addColumn<Utils::ParameterSet>("parameters");
    QTest::addColumn< QList<SignalGenerator::EventEntity> >("expected");
    const QString PATH = "../confreadertest/testfiles/valid/";

    {
        Utils::ParameterSet params("signalGenerator");
        params.appendParameter("configurationFile", PATH+"empty.xml");
        QTest::newRow("empty") << params << QList<SignalGenerator::EventEntity>();
    }

    {
        Utils::ParameterSet params("signalGenerator");
        params.appendParameter("configurationFile", PATH+"one_event.xml");
        QList<SignalGenerator::EventEntity> data =
        {
            SignalGenerator::EventEntity("s1", QDateTime(QDate(2000,1,1), QTime(0,0,0)) )
        };
        QTest::newRow("one event") << params << data;
    }

    {
        Utils::ParameterSet params("signalGenerator");
        params.appendParameter("configurationFile", PATH+"many_events.xml");
        QList<SignalGenerator::EventEntity> data =
        {
            SignalGenerator::EventEntity("s1", QDateTime(QDate(2000,1,1), QTime(0,0,0)) ),
            SignalGenerator::EventEntity("s2", QDateTime(QDate(2001,2,2), QTime(12,0,0)), "1 second", 5),
            SignalGenerator::EventEntity("s2", QDateTime(QDate(2002,3,3), QTime(14,0,0)), "6 hour"),
            SignalGenerator::EventEntity("s3", QDateTime(QDate(2003,4,4), QTime(16,30,0)), "", 10)
        };
        QTest::newRow("many events") << params << data;
    }
}



QTEST_APPLESS_MAIN(ConfReaderTest)

#include "tst_confreadertest.moc"
