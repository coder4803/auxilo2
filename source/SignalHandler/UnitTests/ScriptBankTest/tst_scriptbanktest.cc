#include <QString>
#include <QtTest>
#include <vector>
#include <memory>
#include "scriptbankbuilder.hh"

class ScriptBankTest : public QObject
{
    Q_OBJECT
    
public:
    ScriptBankTest();
    
private Q_SLOTS:
    
    // Test building ScriptBank with valid messages.
    void buildWithValidMessage();   
    void buildWithValidMessage_data();
    
    // Test error cases.
    void buildWithErrors();
    void buildWithErrors_data();
    
    // Test onopenable file
    void buildWithUnopenableFile();
    
private:
    typedef std::unique_ptr<SignalHandler::ScriptBankInterface> BankPtr;
};


ScriptBankTest::ScriptBankTest()
{
}


void ScriptBankTest::buildWithValidMessage()
{
    QFETCH(QString, message);
    QFETCH(std::vector<unsigned>, IDs);
    QFETCH(std::vector<unsigned>, priorities);
    QFETCH(std::vector<QString>, file_names);
            
    SignalHandler::ScriptBankBuilder builder;
    BankPtr bank(nullptr);
    
    try{
        BankPtr(builder.createScriptBank(message) );
    }
    catch(SignalHandler::ScriptBankBuilderFileError&){
        qDebug() << QDir::currentPath();
        QFAIL("File did not open");
    }
    catch(...){
        QFAIL("Exception from legal operation.");
    }
    
    for (unsigned i=0; i<IDs.size(); ++i){
        
        try
        {
            QFile f(file_names.at(i));
            f.open(QIODevice::ReadOnly|QIODevice::Text);
            QString script = f.readAll();
            f.close();
            
            QCOMPARE(bank->getPriorityOf(IDs.at(i)), priorities.at(i));
            QCOMPARE(bank->getScript(IDs.at(i)), script);
        }
        catch (SignalHandler::ScriptBankBuilderFileError&){
            qDebug() << "File" << file_names.at(i) << "does not open";
            QFAIL("File error.");
        }
        catch (...){
            QFAIL("Exception from legal operation.");
        }
    }
}


void ScriptBankTest::buildWithValidMessage_data()
{
    QTest::addColumn<QString>("message");
    QTest::addColumn<std::vector<unsigned> >("IDs");
    QTest::addColumn<std::vector<unsigned> >("priorities");
    QTest::addColumn<std::vector<QString> >("file_names");
    
    QTest::newRow("1script") << QString("0:1:test1.txt") 
                             << std::vector<unsigned>({0}) 
                             << std::vector<unsigned>({1}) 
                             << std::vector<QString>({QString("test1.txt")});
    
    QTest::newRow("2scripts") << QString("0:1:test1.txt;1:2:test2.txt")
                              << std::vector<unsigned>({0,1})
                              << std::vector<unsigned>({1,2})
                              << std::vector<QString>({QString("test1.txt"),
                                                       QString("test2.txt")});
}


void ScriptBankTest::buildWithErrors()
{
    QFETCH(QString, message);
    SignalHandler::ScriptBankBuilder b;
    
    try{
        BankPtr bank(b.createScriptBank(message));
        QFAIL("No exception from illegal operation!");
    }
    catch (SignalHandler::BadMessage&){
        // OK
    }
    catch (...){
        QFAIL("Exception of a wrong type");
    }
}


void ScriptBankTest::buildWithErrors_data()
{
    QTest::addColumn<QString>("message");
    
    QTest::newRow("negative ID")           << QString("-1:0:testfiles/test1.txt");
    QTest::newRow("negative priority")     << QString("0:-1:testfiles/test1.txt");
    QTest::newRow("ID not a number")       << QString("a:1:testfiles/test1.txt");
    QTest::newRow("priority not a number") << QString("0:a:testfiles/test1.txt");
    QTest::newRow("empty")                 << QString("");
    QTest::newRow("missing 1 field")       << QString("0:testfiles/test1.txt");
    QTest::newRow("missing 2 fields")      << QString("testfiles/test1.txt");
    QTest::newRow("missing file name")     << QString("0:0");
    QTest::newRow("extra fields")          << QString("0:0:testfiles/testi1.txt:");
}


void ScriptBankTest::buildWithUnopenableFile()
{
    QString message("0:0:not_exist.txt");
    
    try{
        BankPtr bp(SignalHandler::ScriptBankBuilder::createScriptBank(message));
        QFAIL("No exception");
    }
    catch (SignalHandler::ScriptBankBuilderFileError& e){
        QCOMPARE(e.what(), "SignalHandler::ScriptBankBuilderFileError");
        QCOMPARE(e.getFileName(), QString("not_exist.txt"));
    }
    catch(...){
        QFAIL("Exception is of a wrong type.");
    }
}


QTEST_APPLESS_MAIN(ScriptBankTest)

#include "tst_scriptbanktest.moc"
