#include <QString>
#include <QtTest>
#include <memory>
#include <QDir>
#include <QFile>

#include "scriptbankbuilder.hh"
#include "confresponsemessage.h"
#include "configuration.hh"
#include <QXmlInputSource>
#include "confxmlhandler.hh"
#include "scriptbank.hh"


Q_DECLARE_METATYPE(Utils::ParameterSet)

const QString SCRIPT_CONTENT = "This is script from file: ";
const QString CONF_DIR = "./confDir";
const QString CONF_FILE1 = CONF_DIR + "/signals1.xml";
const QString CONF_FILE2 = CONF_DIR + "/signals2.xml";
const QString INVALID_FILE = CONF_DIR + "/invalid.xml";
const QString SCRIPT_PATH = CONF_DIR + "/scripts";

class ScriptBankTest : public QObject
{
    Q_OBJECT
    
public:
    ScriptBankTest();
    ~ScriptBankTest();
    
private Q_SLOTS:
    
    // Verify correct functionality with faulty ParamSet
    void builderFailTest();
    void builderFailTest_data();
    
    // Verify Builder and Bank functionality with valid configuration.
    void validConfTest();
    void validConfTest_data();
    
    // Verify Bank functionality when querring non-existent sctipts.
    void badQuerryTest();
    
    
private:
    
    void createConfFiles();
};


ScriptBankTest::ScriptBankTest()
{
    using namespace SignalHandler;
    QDir().mkdir(CONF_DIR);
    QDir().mkdir(SCRIPT_PATH);
    this->createConfFiles();
    
    // Create 10 test script files
    for (int i=0; i<10; ++i){
        QFile f(SCRIPT_PATH + "/signal" + QString::number(i) + QString(".txt"));
        f.open(QIODevice::WriteOnly);
        QString data = SCRIPT_CONTENT + QString::number(i);
        f.write(QByteArray(data.toStdString().c_str()));
        f.close();
    }
}


ScriptBankTest::~ScriptBankTest()
{
}


void ScriptBankTest::builderFailTest()
{
    using namespace SignalHandler;
    QFETCH(Utils::ParameterSet, params);
    
    std::shared_ptr<ScriptBankInterface> bank(nullptr);
    QVERIFY_EXCEPTION_THROWN(bank.reset(ScriptBankBuilder::create(params)),
                             ScriptBankBuilderError);
    QVERIFY2(bank == nullptr, "ScriptBank was created from invalid conf.");
}


void ScriptBankTest::builderFailTest_data()
{
    using namespace SignalHandler;
    QTest::addColumn<Utils::ParameterSet>("params");
    
    QTest::newRow("empty") << Utils::ParameterSet();
    
    {
        Utils::ParameterSet params;
        params.appendParameter(Conf::CONF_PATH, CONF_FILE1);
        QTest::newRow("missing script path") << params;
    }
    
    {
        Utils::ParameterSet params;
        params.appendParameter(Conf::SCRIPT_PATH, SCRIPT_PATH);
        QTest::newRow("missing conf path") << params;
    }
    
    {
        Utils::ParameterSet params;
        params.appendParameter(Conf::SCRIPT_PATH, SCRIPT_PATH);
        params.appendParameter(Conf::CONF_PATH, "does_not_exist.xml");
        QTest::newRow("conf file not exist") << params;
    }
    
    {
        Utils::ParameterSet params;
        params.appendParameter(Conf::SCRIPT_PATH, "not_a_valid_path");
        params.appendParameter(Conf::CONF_PATH, CONF_FILE1);
        QTest::newRow("incorrect script path1") << params;
    }
    
    {
        Utils::ParameterSet params;
        params.appendParameter(Conf::SCRIPT_PATH, "not_a_valid_path");
        params.appendParameter(Conf::CONF_PATH, CONF_FILE2);
        QTest::newRow("incorrect script path2") << params;
    }
    
    {
        Utils::ParameterSet params;
        params.appendParameter(Conf::SCRIPT_PATH, SCRIPT_PATH);
        params.appendParameter(Conf::CONF_PATH, INVALID_FILE);
        QTest::newRow("invalid conf_file") << params;
    }
    
}


void ScriptBankTest::validConfTest()
{
    using namespace SignalHandler;
    QFETCH(Utils::ParameterSet, params);
    
    // Create bank.
    std::shared_ptr<ScriptBankInterface> bank;
    try{
        bank.reset(ScriptBankBuilder::create(params));
    }
    catch (ScriptBankBuilderError& e){
        qDebug() << e.getMessage();
        QFAIL("ScriptBankBuilderError from valid configuration.");
    }
    catch(...){
        QFAIL("Unknown exception from valid configuration.");
    }
    
    // Get correct results
    ScriptBankInterface::ScriptData correct_data;
    QFile f(params.parameter<QString>(Conf::CONF_PATH.toLower()));
    QXmlInputSource source(&f);
    ConfXmlHandler h(&correct_data);
    QXmlSimpleReader r;
    r.setContentHandler(&h);
    r.setErrorHandler(&h);
    r.parse(&source);
    
    // Compare results
    for (auto it = correct_data.begin(); it != correct_data.end(); ++it) {
        try{
            unsigned priority = it.value().priority;
            QCOMPARE(bank->getLanguage(it.key()), it.value().language);
            QCOMPARE(bank->getPriorityOf(it.key()), priority);
            
            QString f_name = params.parameter<QString>(Conf::SCRIPT_PATH.toLower());
            f_name.append("/"+it.key()+".txt");
            QFile f(f_name);
            f.open(QIODevice::ReadOnly);
            QString content = f.readAll();
            QCOMPARE(bank->getScript(it.key()), content);
        }
        catch(UnknownScript&){
            QFAIL("Script was not found");
        }
        catch(...){
            QFAIL("Unknown exception.");
        }
    }
}


void ScriptBankTest::validConfTest_data()
{
    using namespace SignalHandler;
    QTest::addColumn<Utils::ParameterSet>("params");
    
    {
        Utils::ParameterSet params;
        params.appendParameter(Conf::CONF_PATH, CONF_FILE1);
        params.appendParameter(Conf::SCRIPT_PATH, SCRIPT_PATH);
        QTest::newRow("file1") << params;
    }
    
    {
        Utils::ParameterSet params;
        params.appendParameter(Conf::CONF_PATH, CONF_FILE2);
        params.appendParameter(Conf::SCRIPT_PATH, SCRIPT_PATH);
        QTest::newRow("file2") << params;
    }
}


void ScriptBankTest::badQuerryTest()
{
    // Build an empty ScriptBank
    using namespace SignalHandler;
    std::shared_ptr<ScriptBankInterface> bank(new ScriptBank());
    
    // Verify
    QVERIFY_EXCEPTION_THROWN(bank->getScript("not_exist"), UnknownScript);
    QVERIFY_EXCEPTION_THROWN(bank->getLanguage("not_exist"), UnknownScript);
    QVERIFY_EXCEPTION_THROWN(bank->getPriorityOf("not_exist"), UnknownScript);
}


void ScriptBankTest::createConfFiles()
{
    using namespace SignalHandler;
    QFile conf1(CONF_FILE1);
    conf1.open(QIODevice::WriteOnly);
    conf1.write("<signals>\n");
    for (int i=0; i<10; ++i){
        QString line;
        line = QString("<signal %1=\"%2\" %3=\"%4\" %5=\"%6\" %7=\"%8\" %9=\"%10\"/>\n")
                .arg(Conf::SIGNAL_NAME).arg(QString("signal")+QString::number(i))
                .arg(Conf::SIGNAL_PATH).arg("/signal"+QString::number(i)+".txt")
                .arg(Conf::SIGNAL_LANG).arg("QtScript")
                .arg(Conf::SIGNAL_PRIORITY).arg(QString::number(i))
                .arg(Conf::SIGNAL_TO_MEM).arg("false");
        conf1.write(line.toStdString().c_str());        
    }
    conf1.write("</signals>\n");
    conf1.close();
    
    QFile conf2(CONF_FILE2);
    conf2.open(QIODevice::WriteOnly);
    conf2.write("<signals>\n");
    for (int i=0; i<10; ++i){
        QString line;
        line = QString("<signal %1=\"%2\" %3=\"%4\" %5=\"%6\"/>\n")
                .arg(Conf::SIGNAL_NAME).arg(QString("signal")+QString::number(i))
                .arg(Conf::SIGNAL_PATH).arg("/signal"+QString::number(i)+".txt")
                .arg(Conf::SIGNAL_LANG).arg("QtScript");
        conf2.write(line.toStdString().c_str());
    }
    conf2.write("</signals>\n");
    conf2.close();
    
    QFile conf3(INVALID_FILE);
    conf3.open(QIODevice::WriteOnly);
    conf3.write("This is not a valid xml file <7<890d7<0x<>>>></>Z");
    conf3.close();
}


QTEST_APPLESS_MAIN(ScriptBankTest)

#include "tst_scriptbanktest.moc"
