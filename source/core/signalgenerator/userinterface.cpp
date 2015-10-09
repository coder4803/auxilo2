/* userinterface.cpp
 * This is the implementation file for the UserInterface singleton class
 * defined in userinterface.h.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 09-Oct-2015
 */

#include "userinterface.h"
#include "consoleui.h"
#include "uiplugininterface.h"
#include <QPluginLoader>
#include <QDebug>

namespace
{

// Message handler redirects Qt's printing functions to the used ViewInterface.
void messageHandler(QtMsgType type,
                    const QMessageLogContext& context,
                    const QString& msg)
{
    Q_UNUSED (context);

    switch (type) {
    case QtWarningMsg:
        SignalGenerator::UserInterface::getInstance()->warning(msg);
        break;

    case QtDebugMsg:
        SignalGenerator::UserInterface::getInstance()->debug(msg);
        break;

    case QtCriticalMsg:
        SignalGenerator::UserInterface::getInstance()->critical(msg);
        break;

    case QtFatalMsg:
        SignalGenerator::UserInterface::getInstance()->critical(msg);
        break;

    default:
        break;
    }
}

} // Anonymous namespace


namespace SignalGenerator
{

std::unique_ptr<UserInterface> UserInterface::instance_(nullptr);


QCoreApplication *UserInterface::initUI(int &argc, char *argv[])
{
    QCoreApplication* app(nullptr);

    if (std::find_if(argv+1, argv+argc,
                     [](char* str){return QString(str) == "--ui";}) != argv+argc)
    {
        // Load plugin.
        app = loadUiPlugin(argc, argv);
        if (app == nullptr) return nullptr;
    }
    else{
        app = new QCoreApplication(argc, argv);
        instance_.reset( new UserInterface(new ConsoleUI) );
    }

    // Check if UI needs to be in verbose mode.
    if (std::find_if(argv+1, argv+argc,
                     [](char* str){return QString(str) == "-v";}) != argv+argc)
    {
        instance_->setVerbose(true);
    }
    qInstallMessageHandler(messageHandler);
    return app;
}


ViewInterface *UserInterface::getInstance()
{
    return instance_.get();
}


UserInterface::~UserInterface()
{
}


void UserInterface::debug(const QString &msg)
{
    view_->debug(msg);
}

void UserInterface::warning(const QString &msg)
{
    view_->warning(msg);
}

void UserInterface::critical(const QString &msg)
{
    view_->critical(msg);
}

void UserInterface::fatal(const QString &msg)
{
    view_->fatal(msg);
}

void UserInterface::setVerbose(bool value)
{
    view_->setVerbose(value);
}

void UserInterface::setTableModel(QSqlTableModel *model)
{
    view_->setTableModel(model);
}


UserInterface::UserInterface(ViewInterface* view):
    ViewInterface(), view_(nullptr)
{
    view_.reset(view);
}


QCoreApplication *UserInterface::loadUiPlugin(int &argc, char *argv[])
{
    // Get ui-plugin name
    auto it = std::find_if(argv+1, argv+argc,
                           [](char* str){return QString(str)=="--ui";} );
    ++it;
    if (it == argv+argc){
        qCritical() << "--ui parameter given, but no ui name given. Quiting.";
        return nullptr;
    }
    QString uiName(*it);

    // Load plugin
    QString fileName = "../plugins/userinterfaces/signalgenerator/ui" + uiName;
    QPluginLoader loader(fileName);
    if ( !loader.load() ){
        qCritical() << "Failed to load plugin file" << fileName
                    << "(" << loader.errorString() << ")";
        return nullptr;
    }

    // Cast to UiPlugin
    UiPluginInterface* plugin(nullptr);
    plugin = qobject_cast<UiPluginInterface*>(loader.instance());
    if (plugin == nullptr){
        qCritical() << "Failed to instantiate UiPlugin from" << fileName;
        return nullptr;
    }

    // Create App and view.
    QCoreApplication* app = plugin->createApp(argc, argv);
    if (app == nullptr){
        qCritical() << "UiPlugin" << uiName
                    << "did not produce proper instance of QCoreApplication.";
        return nullptr;
    }

    UserInterface::instance_.reset( new UserInterface(plugin->createView()) );
    if (UserInterface::instance_->view_ == nullptr){
        qCritical() << "UiPlugin" << uiName
                    << "did not produce a proper instance of ViewInterface";
        delete app;
        return nullptr;
    }

    return app;
}

} // Namespace SignalGenerator
