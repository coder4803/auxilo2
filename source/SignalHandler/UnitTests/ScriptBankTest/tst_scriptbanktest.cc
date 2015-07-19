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
    
    // Verify that functionality with faulty ParamSet
    void builderFailTest();
    void builderFailTest_data();
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
    
    // Invalid priority
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
        QTest::newRow("Invalid priority") << params;
    }
}


QTEST_APPLESS_MAIN(ScriptBankTest)

#include "tst_scriptbanktest.moc"
