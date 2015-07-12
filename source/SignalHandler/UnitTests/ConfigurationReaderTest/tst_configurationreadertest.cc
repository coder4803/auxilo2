#include <QString>
#include <QtTest>

#include "configurationreader.hh"


class ConfigurationReaderTest : public QObject
{
    Q_OBJECT
    
public:
    ConfigurationReaderTest();
    
signals:
    
    void confResponseMessage(QByteArray payload);
    
private Q_SLOTS:
    
    // Test constructor.
    void constructorTest();
    void constructorTest_data();
    
    // Test ConfResponseMessage handling without actual messager server.
    void bareConfigurationTest();
    void bareConfigurationTest_data();
};


ConfigurationReaderTest::ConfigurationReaderTest()
{
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
    QTest::newRow("test object") << static_cast<QObject*> (this);
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
    
    connect(this, SIGNAL(confResponseMessage(QByteArray)),
            &reader, SLOT(onConfMessageReceived(QByteArray)), 
            Qt::DirectConnection);
    
    emit this->confResponseMessage( msg.data() ); 
    Utils::ParameterSet conf = reader.getConfiguration();
    
    QCOMPARE(conf.featureName(), feature);
    QCOMPARE(conf.parameters(), parameters);
}


void ConfigurationReaderTest::bareConfigurationTest_data()
{
    using Params = QHash<QString,QString>;
    QTest::addColumn<Params>("parameters");
    QTest::addColumn<QString>("feature");
    
    QTest::newRow("Empty params") << Params() << QString("feature_name");
    
    QTest::newRow("1 parameter") 
            << Params({{QString("name0"), QString("value0")}}) 
            << QString("feature_name");
    
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

QTEST_APPLESS_MAIN(ConfigurationReaderTest)

#include "tst_configurationreadertest.moc"
