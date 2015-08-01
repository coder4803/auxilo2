#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../../exceptions/scriptrunexceptions.hh"
#include "../../ScriptLangWrapper/qtscriptwrapper.hh"
#include "fakeapi.hh"
#include <memory>
#include <functional>


Q_DECLARE_METATYPE(std::function<bool(FakeAPI&)>)

class QtScriptTest : public QObject
{
    Q_OBJECT
    
public:
    QtScriptTest();
    
private Q_SLOTS:
    
    // Verify that QtScriptWrapper throws exception on invalid script.
    void invalidScriptTest();
    void invalidScriptTest_data();
    
    // Verify that QtScripWrapper is able to invoke all ScriptAPI methods
    // without exceptions.
    void ApiCallTest();
    void ApiCallTest_data();
    
    // Verify, that arguments passed to the script can be used in script.
    void argumentUsageTest();
    void argumentUsageTest_data();
    
private:
    std::shared_ptr<FakeAPI> api_;
};

QtScriptTest::QtScriptTest() : api_(new FakeAPI)
{
}

void QtScriptTest::invalidScriptTest()
{
    QFETCH(QString, script);
    QFETCH(QStringList, args);
    
    SignalHandler::QtScriptWrapper wrapper;
    
    try
    {
        wrapper.run(script, args, api_.get());
        api_->clear();
        QFAIL("No exception from invalid script");
    }
    catch (SignalHandler::BadScript&){
        // OK!
        api_->clear();
    }
    catch (...){
        api_->clear();
        QFAIL("Exception is of a wrong type");
    }
}


void QtScriptTest::invalidScriptTest_data()
{
    QTest::addColumn<QString>("script");
    QTest::addColumn<QStringList>("args");
    
    QTest::newRow("syntax_error")   << QString("adfDASJFALKJDF") << QStringList();
}


void QtScriptTest::ApiCallTest()
{
    QFETCH(QString, script);
    QFETCH(QStringList, args);
    QFETCH(QList<int>, verification_indecies);
    
    SignalHandler::QtScriptWrapper wrapper;
    try {
        wrapper.run(script, args, api_.get());
        QVERIFY2(api_->verifyCalledOnly(verification_indecies),
                 "Incorrect functions called.");
        api_->clear();
    }
    catch(SignalHandler::BadScript&){
        api_->clear();
        QFAIL("Exception from valid script.");
    }
    catch(...){
        api_->clear();
        QFAIL("Unknown exception.");
    }
}


void QtScriptTest::ApiCallTest_data()
{
    QTest::addColumn<QString>("script");
    QTest::addColumn<QStringList>("args");
    QTest::addColumn<QList<int> >("verification_indecies");
    
    
    QTest::newRow("datetime") << QString("function main(){Auxilo2.dateTimeNow();} "
                                         "main();")
                              << QStringList() 
                              << QList<int>({0});
    
    QTest::newRow("getStateOf") << QString("function f1(){Auxilo2.setState(\"name\",1);}"
                                           "function f2(){Auxilo2.getStateOf(\"name\");}"
                                           "function main(){f1(); f2();}"
                                           "main();")
                                << QStringList()
                                << QList<int>({1,3});
    
    QTest::newRow("getStates") << QString("function f1(){Auxilo2.setState(\"name1\",1); Auxilo2.setState(\"name2\",2);}"
                                          "function f2(){Auxilo2.getStates(Array(\"name1\", \"name2\"));}"
                                          "function main(){f1(); f2();}"
                                          "main();")
                               << QStringList()
                               << QList<int>({2,3});
                              
    QTest::newRow("setState") << QString("function main(){Auxilo2.setState(\"name\", 1);}"
                                         "main();")
                              << QStringList()
                              << QList<int>({3});
    
    QTest::newRow("sendSignal") << QString("function main(){Auxilo2.sendSignal(\"name\", Array(12,13));}"
                                           "main();")
                                << QStringList()
                                << QList<int>({4});
}

void QtScriptTest::argumentUsageTest()
{
    QFETCH(QString, script);
    QFETCH(QStringList, args);
    
    SignalHandler::QtScriptWrapper wrapper;
    try {
        wrapper.run(script, args, api_.get());
        for (int i=0; i<args.size(); ++i){
            QString key = QString("arg")+QString::number(i+1);
            QVERIFY2(api_->getStateOf(key).value.toString() == args.at(i),
                     "Argument has a wrong value.");
        }
        api_->clear();
    }
    catch (SignalHandler::BadScript&){
        api_->clear();
        QFAIL("Exception from a valid script");
    }
    catch (...){
        api_->clear();
        QFAIL("Unknown exception.");
    }
}


void QtScriptTest::argumentUsageTest_data()
{
    QTest::addColumn<QString>("script");
    QTest::addColumn<QStringList>("args");
    
    QTest::newRow("1 arg") << QString("function main(){Auxilo2.setState(\"arg1\", argv[0]);}"
                                      "main();")
                           << QStringList({"1"});
    
    QTest::newRow("2 args") << QString("function main(){Auxilo2.setState(\"arg1\", argv[0]);"
                                                       "Auxilo2.setState(\"arg2\", argv[1]);}"
                                       "main();"
                                       )
                            << QStringList({"asd", "fgh"});
}

QTEST_MAIN(QtScriptTest)

#include "tst_qtscripttest.moc"
