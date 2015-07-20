#include <QString>
#include <QtTest>
#include <memory>
#include <QDir>
#include <QFile>

#include "scriptbankbuilder.hh"
#include "confresponsemessage.h"


Q_DECLARE_METATYPE(Utils::ParameterSet)
Q_DECLARE_METATYPE(const char*)

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
};


ScriptBankTest::ScriptBankTest()
{
    // Create test file
    QFile f("tmp_file.txt");
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    f.write("This is a temporary test file");
    f.close();
}


ScriptBankTest::~ScriptBankTest()
{
    // remove test file
    QDir dir;
    dir.remove("tmp_file.txt");
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
    const QString NAME_PRE = ScriptBankBuilder::SCRIPT_NAME_PREFIX;
    const QString PATH_POST = ScriptBankBuilder::PATH_POSTFIX;
    const QString PRIORITY_POST = ScriptBankBuilder::PRIORITY_POSTFIX;
    const QString LANG_POST = ScriptBankBuilder::LANG_POSTFIX;
    QTest::addColumn<Utils::ParameterSet>("params");
    
    // File doesn't open
    {
        Utils::ParameterSet params(QString("SignalHandler"));    
        QString name("non_exixtent_name");
        params.appendParameters(QHash<QString,QString>( 
            {
                {NAME_PRE+QString("0"), name},
                {name+PATH_POST, QString("does_not_exist.txt")},
                {name+PRIORITY_POST, QString::number(0)},
                {name+LANG_POST, QString("QtScript")}
            } ) );
        QTest::newRow("file doesn't exist") << params;
    }
    
    // No path declared
    {
        Utils::ParameterSet params(QString("SignalHandler"));    
        QString name("no_path");
        params.appendParameters(QHash<QString,QString>( 
            {
                {NAME_PRE+QString("0"), name},
                {name+PRIORITY_POST, QString::number(0)},
                {name+LANG_POST, QString("QtScript")}
            } ) );
        QTest::newRow("No path") << params;
    }
    
    // No priority declared
    {
        Utils::ParameterSet params(QString("SignalHandler"));    
        QString name("no_priority");
        params.appendParameters(QHash<QString,QString>( 
            {
                {NAME_PRE+QString("0"), name},
                {name+PATH_POST, QString("tmp_file.txt")},
                {name+LANG_POST, QString("QtScript")}
            } ) );
        QTest::newRow("No priority") << params;
    }
    
    // No lang declared
    {
        Utils::ParameterSet params(QString("SignalHandler"));    
        QString name("no_lang");
        params.appendParameters(QHash<QString,QString>( 
            {
                {NAME_PRE+QString("0"), name},
                {name+PATH_POST, QString("tmp_file.txt")},
                {name+PRIORITY_POST, QString::number(0)}
            } ) );
        QTest::newRow("No lang") << params;
    }
    
    // NaN priority
    {
        Utils::ParameterSet params(QString("SignalHandler"));    
        QString name("invalid_priority");
        params.appendParameters(QHash<QString,QString>( 
            {
                {NAME_PRE+QString("0"), name},
                {name+PATH_POST, QString("tmp_file.txt")},
                {name+PRIORITY_POST, QString("Not a number")},
                {name+LANG_POST, QString("QtScript")}
            } ) );
        QTest::newRow("NaN priority") << params;
    }
    
    // Negative priority
    {
        Utils::ParameterSet params(QString("SignalHandler"));    
        QString name("invalid_priority");
        params.appendParameters(QHash<QString,QString>( 
            {
                {NAME_PRE+QString("0"), name},
                {name+PATH_POST, QString("tmp_file.txt")},
                {name+PRIORITY_POST, QString("-3")},
                {name+LANG_POST, QString("QtScript")}
            } ) );
        QTest::newRow("negative priority") << params;
    }
}


void ScriptBankTest::validConfTest()
{
    using namespace SignalHandler;
    const QString NAME_PREFIX = ScriptBankBuilder::SCRIPT_NAME_PREFIX;
    const QString PRIORITY = ScriptBankBuilder::PRIORITY_POSTFIX;
    const QString LANG = ScriptBankBuilder::LANG_POSTFIX;
    QFETCH(Utils::ParameterSet, params);
    QFETCH(QString, script_content);
    
    // Try to create instance of ScripBank.
    std::shared_ptr<ScriptBankInterface> bank(nullptr);
    try{
        bank.reset(ScriptBankBuilder::create(params));
    }
    catch (ScriptBankBuilderError&){
        QFAIL("ScriptBankError from a valid configuration.");
    }
    catch(...){
        QFAIL("Unknown exception from a valid configuration");
    }
    QVERIFY2(bank != nullptr, "Bank was not created.");
    
    // Verify Bank content
    QHash<QString,QString> conf = params.parameters();
    QStringList names =  params.getSection(NAME_PREFIX).parameters().values();
    
    foreach (QString name, names) {
        try{
            QCOMPARE(bank->getScript(name), script_content);
            QCOMPARE(bank->getPriorityOf(name), 
                     conf.value(name+PRIORITY).toUInt() );
            QCOMPARE(bank->getLanguage(name), conf.value(name+LANG));
        }
        catch(UnknownScript&){
            QFAIL("Unknown script");
        }
        catch(...){
            QFAIL("Unknown exception from valid querry.");
        }
    }
    
}


void ScriptBankTest::validConfTest_data()
{
    using SignalHandler::ScriptBankBuilder;
    const QString CONTENT = "This is a temporary test file";
    const QString PATH = "tmp_file.txt";
    QTest::addColumn<Utils::ParameterSet>("params");
    QTest::addColumn<QString>("script_content");
    
    QTest::newRow("empty params") << Utils::ParameterSet() << QString();
    
    // One script
    {
        Utils::ParameterSet params;
        const QString NAME = "only_script";
        params.appendParameter(ScriptBankBuilder::SCRIPT_NAME_PREFIX+"0", NAME);
        params.appendParameter(NAME + ScriptBankBuilder::PATH_POSTFIX, PATH);
        params.appendParameter(NAME + ScriptBankBuilder::PRIORITY_POSTFIX,
                               QString::number(10));
        params.appendParameter(NAME + ScriptBankBuilder::LANG_POSTFIX,
                               QString("QtScript"));
        QTest::newRow("1 param") << params << CONTENT;
    }
    
    // 10 scripts
    {
        Utils::ParameterSet params;
        for (int i=0; i<10; ++i){
            const QString N = QString::number(i);
            const QString NAME = QString("script_name") + N;
            params.appendParameter(ScriptBankBuilder::SCRIPT_NAME_PREFIX+N, NAME);
            params.appendParameter(NAME+ScriptBankBuilder::PATH_POSTFIX, PATH);
            params.appendParameter(NAME+ScriptBankBuilder::PRIORITY_POSTFIX, N);
            params.appendParameter(NAME + ScriptBankBuilder::LANG_POSTFIX,
                                   QString("QtScript"));
        }
        QTest::newRow("10 params") << params << CONTENT;
    }
}


void ScriptBankTest::badQuerryTest()
{
    // Build an empty ScriptBank
    using namespace SignalHandler;
    std::shared_ptr<ScriptBankInterface> bank;
    try{
        bank.reset( ScriptBankBuilder::create(Utils::ParameterSet()) );
    }
    catch (ScriptBankBuilderError&){
        QFAIL("Builder failed with valid configuration");
    }
    catch (...){
        QFAIL("Builder threw unknown type of exception");
    }
    QVERIFY2(bank != nullptr, "Bank was not created.");
    
    // Verify
    QVERIFY_EXCEPTION_THROWN(bank->getScript("not_exist"), UnknownScript);
    QVERIFY_EXCEPTION_THROWN(bank->getLanguage("not_exist"), UnknownScript);
    QVERIFY_EXCEPTION_THROWN(bank->getPriorityOf("not_exist"), UnknownScript);
}


QTEST_APPLESS_MAIN(ScriptBankTest)

#include "tst_scriptbanktest.moc"
