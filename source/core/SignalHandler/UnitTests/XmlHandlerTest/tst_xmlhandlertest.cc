#include <QString>
#include <QtTest>
#include <functional>
#include "confxmlhandler.hh"
#include "configuration.hh"
#include <QFile>
#include <QDir>

Q_DECLARE_METATYPE(SignalHandler::ScriptBankInterface::ScriptData)
Q_DECLARE_METATYPE(QXmlParseException)
Q_DECLARE_METATYPE(QXmlAttributes)
Q_DECLARE_METATYPE(std::function<void(SignalHandler::ConfXmlHandler*)>)
Q_DECLARE_METATYPE(std::vector<QString>)
Q_DECLARE_METATYPE(std::vector<SignalHandler::ScriptBankInterface::ScriptInfo>)


class XmlHandlerTest : public QObject
{
    Q_OBJECT
    
public:
    XmlHandlerTest();
    ~XmlHandlerTest();
    
private Q_SLOTS:
    
    // Test constructor.
    void constructorTest();
    void constructorTest_data();
    
    // Test fatalError implementation.
    void fatalErrorTest();
    void fatalErrorTest_data();
    
    // Test startElement implementation.
    void startElementTest();
    void startElementTest_data();
    
    // Verify that handler works with QXmlSimpleParser.
    void integrationTestWithParser();
    void integrationTestWithParser_data();
    
private:
    
    void produceXmlFile(std::vector<QString> names,
                        std::vector<SignalHandler::ScriptBankInterface::ScriptInfo> infos);
};



XmlHandlerTest::XmlHandlerTest()
{
}

XmlHandlerTest::~XmlHandlerTest()
{   
}


void XmlHandlerTest::constructorTest()
{
    using namespace SignalHandler;
    QFETCH(ScriptBankInterface::ScriptData, data);
    
    ConfXmlHandler handler(&data);
    QVERIFY2(handler.errorString() == QString(),
             "Constructed handler has a non-empty error string.");
}


void XmlHandlerTest::constructorTest_data()
{
    using SignalHandler::ScriptBankInterface;
    QTest::addColumn<ScriptBankInterface::ScriptData>("data");
    
    QTest::newRow("empty data") << ScriptBankInterface::ScriptData();
}


void XmlHandlerTest::fatalErrorTest()
{
    QFETCH(QXmlParseException, exception);
    
    SignalHandler::ScriptBankInterface::ScriptData data;
    SignalHandler::ConfXmlHandler handler(&data);
    
    QVERIFY(handler.fatalError(exception) == false);
    
    QCOMPARE(handler.errorString(),
             QString("Error while loading script: line %1, column %2")
             .arg(exception.lineNumber())
             .arg(exception.columnNumber()) );
}


void XmlHandlerTest::fatalErrorTest_data()
{
    QTest::addColumn<QXmlParseException>("exception");
    
    QTest::newRow("default exception") << QXmlParseException();
    
    QXmlParseException error("test_exception", 1, 1, "test_p", "test_s");
    QTest::newRow("explicit exception values") << error;
}


void XmlHandlerTest::startElementTest()
{
    using namespace SignalHandler;
    QFETCH(QString, qName);
    QFETCH(QXmlAttributes, atts);
    QFETCH(bool, result);
    QFETCH(std::function<void(ConfXmlHandler*)>, initFn);
    QFETCH(ScriptBankInterface::ScriptData, data);
    
    int original_size = data.size();
    ConfXmlHandler handler(&data);
    initFn(&handler);
    
    QCOMPARE(handler.startElement(QString(), QString(), qName, atts), result);
    QCOMPARE(handler.errorString().isEmpty(), result);
    
    // If started element was a signal-element, verify that data is modified.
    if (qName == "signal" && result == true){
        QCOMPARE (data.size(), original_size + 1);
        QString name, lang, path;
        unsigned priority(Conf::DEFAULT_PRIORITY);
        bool readFromDisk(false);
        
        for (int i=0; i<atts.count(); ++i){
            if (atts.qName(i) == Conf::SIGNAL_NAME){
                name = atts.value(i);
            }
            else if (atts.qName(i) == Conf::SIGNAL_PATH){
                path = atts.value(i);
            }
            else if (atts.qName(i) == Conf::SIGNAL_LANG){
                lang = atts.value(i);
            }
            else if (atts.qName(i) == Conf::SIGNAL_PRIORITY){
                priority = atts.value(i).toUInt();
            }
            else if (atts.qName(i) == Conf::SIGNAL_TO_MEM){
                readFromDisk = atts.value(i)=="true" ? false : true;
            }
        }
        
        QVERIFY2(data.find(name) != data.end(), "signal was not found");
        ScriptBankInterface::ScriptInfo info = data.value(name);
        QCOMPARE(info.script, path);
        QCOMPARE(info.language, lang);
        QCOMPARE(info.priority, priority);
        QCOMPARE(info.from_file, readFromDisk);
    }
}


void XmlHandlerTest::startElementTest_data()
{
    using namespace SignalHandler;
    using InitF = std::function<void(ConfXmlHandler*)>;
    
    QTest::addColumn<QString>("qName");
    QTest::addColumn<QXmlAttributes>("atts");
    QTest::addColumn<bool>("result");
    QTest::addColumn<InitF>("initFn");
    QTest::addColumn<ScriptBankInterface::ScriptData>("data");
    
    // Define common init-functions.
    InitF noInit = [](ConfXmlHandler* h){Q_UNUSED(h);};
    
    InitF startSignals = [](ConfXmlHandler* h)
        {h->startElement("", "", "signals", QXmlAttributes()); };
    
    InitF startEndSignals = [](ConfXmlHandler* h)
        {
            h->startElement("", "", "signals", QXmlAttributes());
            h->endElement("", "", "signals");
        };
    
    InitF startSig = [](ConfXmlHandler* h)
    {
        QXmlAttributes atts;
        atts.append(Conf::SIGNAL_NAME, "", "", "testSignal");
        atts.append(Conf::SIGNAL_PATH, "", "", "testpath/testscript.py");
        atts.append(Conf::SIGNAL_LANG, "", "", "python");
        h->startElement("", "", "signals", QXmlAttributes());
        h->startElement("", "", "signal", atts);
    };
    
    // Attributes containing valid mandatory signal attributes.
    QXmlAttributes atts;
    atts.append(Conf::SIGNAL_NAME, QString(), QString(), "mySignal0");
    atts.append(Conf::SIGNAL_PATH, QString(), QString(), "scripts/mySignal.py");
    atts.append(Conf::SIGNAL_LANG, QString(), QString(), "python");
    
    
    // empty signals-element.
    QTest::newRow("empty signals") << "signals" << QXmlAttributes() << true
                                   << noInit
                                   << ScriptBankInterface::ScriptData();
    
    // nested signals
    QTest::newRow("nested signals") << "signals" << QXmlAttributes() << false
                                    << startSignals
                                    << ScriptBankInterface::ScriptData();
    
    // redefine scripts
    QTest::newRow("redefine signals") << "signals" << QXmlAttributes() << false
                                      << startEndSignals
                                      << ScriptBankInterface::ScriptData();
    
    // unknown element outside signals
    QTest::newRow("unknown element before") << "unknown" << QXmlAttributes()
                                            << false << noInit
                                            << ScriptBankInterface::ScriptData();
    
    QTest::newRow("unknown element after") << "unknown" << QXmlAttributes()
                                           << false << startEndSignals
                                           << ScriptBankInterface::ScriptData();
    
    // unknown element inside signals
    QTest::newRow("unknown element inside")<< "unknown" << QXmlAttributes()
                                           << false << startSignals
                                           << ScriptBankInterface::ScriptData();
    
    // Signal no arguments
    QTest::newRow("empty signal") << "signal" << QXmlAttributes() << false
                                  << startSignals
                                  << ScriptBankInterface::ScriptData();
    
    // Nested signal
    QTest::newRow("nested signal") << "signal" << atts << false
                                   << startSig
                                   << ScriptBankInterface::ScriptData();
    
    QTest::newRow("nested unknown") << "unknown" << QXmlAttributes() << false
                                    << startSig
                                    << ScriptBankInterface::ScriptData();
    
    // valid signal with mandatory parameters outside signals.
    QTest::newRow("signal mandatory before") << "signal" << atts << false
                                             << noInit
                                             << ScriptBankInterface::ScriptData();
    
    QTest::newRow("signal mandatory after") << "signal" << atts << false
                                            << startEndSignals
                                            << ScriptBankInterface::ScriptData();
    
    // valid signal with mandatory parameters inside signals.
    QTest::newRow("signal mandatory inside") << "signal" << atts << true
                                             << startSignals
                                             << ScriptBankInterface::ScriptData();
    
    // Invalid priority
    QXmlAttributes atts2 = atts;
    atts2.append(Conf::SIGNAL_PRIORITY, "", "", "Not a number");
    QTest::newRow("NaN priority") << "signal" << atts2 << false
                                  << startSignals
                                  << ScriptBankInterface::ScriptData();
    atts2.clear();
    atts2 = atts;
    atts2.append(Conf::SIGNAL_PRIORITY, "", "", QString::number(-1));
    QTest::newRow("negative priority") << "signal" << atts2 << false
                                       << startSignals
                                       << ScriptBankInterface::ScriptData();
    // Invalid readToMem
    atts2.clear();
    atts2 = atts;
    atts2.append(Conf::SIGNAL_TO_MEM, "", "", "not 'true' or 'false'");
    QTest::newRow("invalid readToDisk") << "signal" << atts2 << false
                                        << startSignals
                                        << ScriptBankInterface::ScriptData();
    
    // Valid signal with optional parameters outside signals
    atts.append(Conf::SIGNAL_PRIORITY, "", "", QString::number(0));
    atts.append(Conf::SIGNAL_TO_MEM, "", "", "false");
    
    QTest::newRow("signal optional before") << "signal" << atts << false
                                            << noInit
                                            << ScriptBankInterface::ScriptData();
    
    QTest::newRow("signal optional after") << "signal" << atts << false
                                           << startEndSignals
                                           << ScriptBankInterface::ScriptData();
    
    // Valid signal with optional parameters inside signals
    QTest::newRow("signal optional inside") << "signal" << atts << true
                                            << startSignals
                                            << ScriptBankInterface::ScriptData();
    
    // Non-empty data (no dublicates)
    ScriptBankInterface::ScriptData data;
    ScriptBankInterface::ScriptInfo info{"script", "python", 0, false};
    data.insert("mySignal1", info);
    QTest::newRow("previous data, no duplicates") << "signal" << atts << true
                                                  << startSignals
                                                  << data;
    
    // Non-empty data with a dublicate
    data.insert("mySignal0", info);
    QTest::newRow("previous data, dublicates") << "signal" << atts << false
                                               << startSignals
                                               << data;
}


void XmlHandlerTest::integrationTestWithParser()
{
    using namespace SignalHandler;
    QFETCH (std::vector<QString>, names);
    QFETCH (std::vector<SignalHandler::ScriptBankInterface::ScriptInfo>, infos);
    this->produceXmlFile(names, infos);
    
    // Parse xml-file
    QXmlSimpleReader reader;
    ScriptBankInterface::ScriptData data;
    ConfXmlHandler handler(&data);
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);
    QFile f("confDir/signals.xml");
    QVERIFY(f.open(QIODevice::ReadOnly));
    QXmlInputSource source(&f);
    QVERIFY(reader.parse(&source));
    
    // Verify data
    for (unsigned i=0; i<names.size(); ++i){
        QString name = names.at(i);
        ScriptBankInterface::ScriptInfo expected_info = infos.at(i);
        QVERIFY2(data.find(name) != data.end(), 
                 "Signal name not found from data!");
        ScriptBankInterface::ScriptInfo actual_info = data.value(name);
        QCOMPARE(actual_info.from_file, expected_info.from_file);
        QCOMPARE(actual_info.language, expected_info.language);
        QCOMPARE(actual_info.priority, expected_info.priority);
        QCOMPARE(actual_info.script, expected_info.script);
    }
}

void XmlHandlerTest::integrationTestWithParser_data()
{
    using Info = SignalHandler::ScriptBankInterface::ScriptInfo;
    QTest::addColumn<std::vector<QString> >("names");
    QTest::addColumn<std::vector<Info> >("infos");
    
    {
        std::vector<QString> names;
        std::vector<Info> infos;
        
        for (int i=0; i<5; ++i){
            QString name = QString("s") + QString::number(i);
            Info info;
            info.from_file = true;
            info.language = "python";
            info.priority = i;
            info.script = QString("scripts/") + name;
            names.push_back(name);
            infos.push_back(info);
        }
        
        QTest::newRow("python, from file, explicit priority") << names << infos;
    }
}

void XmlHandlerTest::
produceXmlFile(std::vector<QString> names, 
               std::vector<SignalHandler::ScriptBankInterface::ScriptInfo> infos)
{
    using namespace SignalHandler;
    QDir().mkdir("confDir");
    QFile f("confDir/signals.xml");
    f.open(QIODevice::WriteOnly);
    f.write("<signals>\n");
    for (unsigned i=0; i<names.size(); ++i){
        QString toMem = infos.at(i).from_file ? "false" : "true"; 
        QString pattern = QString("<signal %1=\"%2\" %3=\"%4\" %5=\"%6\" %7=\"%8\" %9=\"%10\"/>\n")
                .arg(Conf::SIGNAL_NAME).arg(names.at(i))
                .arg(Conf::SIGNAL_PATH).arg(infos.at(i).script)
                .arg(Conf::SIGNAL_LANG).arg(infos.at(i).language)
                .arg(Conf::SIGNAL_PRIORITY).arg(infos.at(i).priority)
                .arg(Conf::SIGNAL_TO_MEM).arg(toMem);
        f.write( QByteArray(pattern.toStdString().c_str()) );
    }
    f.write("</signals>\n");
    f.close();
}
    


QTEST_APPLESS_MAIN(XmlHandlerTest)

#include "tst_xmlhandlertest.moc"
