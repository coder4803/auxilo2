#include "userinterface.hh"
#include "consoleui.hh"
#include "uiplugin.hh"
#include "configuration.hh"
#include <QDebug>
#include <QPluginLoader>
#include <iterator>

namespace SignalHandler
{


std::unique_ptr<UserInterface> UserInterface::instance_(nullptr);

namespace
{

// Message handler redirects Qt's print functions to the ViewInterface.
void messageHandler(QtMsgType type, 
                    const QMessageLogContext& context, 
                    const QString& msg)
{
    Q_UNUSED (context);
    
    switch (type) {
    case QtWarningMsg:
        UserInterface::getInstance()->warning(msg);
        break;
        
    case QtDebugMsg:
        UserInterface::getInstance()->debug(msg);
        break;
        
    case QtCriticalMsg:
        UserInterface::getInstance()->critical(msg);
        break;
        
    case QtFatalMsg:
        UserInterface::getInstance()->critical(msg);
        
    default:
        break;
    }
}

} // Anonymous namespace ends.


UserInterface::~UserInterface()
{
    
}

QCoreApplication* UserInterface::initUI(int argc, char* argv[])
{
    Q_ASSERT(instance_ == nullptr);
    Q_ASSERT(argc > 0);
    Q_ASSERT(argv != nullptr);
    
    // Check verbose-parameter
    auto it1 = std::find_if(argv, argv+argc,
                            [](const char* s){return QString(s) == "-v";} );
    bool verbose = it1==argv+argc ? false : true;
    
    // check if --ui parameter is set.
    auto it = std::find_if(argv, argv+argc, 
                           [](const char* s){return QString(s) == "--ui";} );
    
    if (it == argv+argc){
        // Use default UI (ConsoleUI)
        std::unique_ptr<QCoreApplication> app(new QCoreApplication(argc, argv));
        std::unique_ptr<ViewInterface> view(new ConsoleUI(verbose));
        instance_.reset(new UserInterface( std::move(view) ) );
        return app.release();
    }
    
    // Load selected Ui-plugin.
    ++it;
    if (it == argv+argc){
        qFatal("Invalid commandline arguments.");
    }
    std::unique_ptr<QCoreApplication> app;
    app.reset( UserInterface::loadPlugin(argc, argv, QString(*it), verbose) );
    qInstallMessageHandler(messageHandler);
    return app.release();
}


UserInterface* UserInterface::getInstance()
{
    Q_ASSERT(instance_ != nullptr);
    return instance_.get();
}

void UserInterface::critical(const QString& msg)
{
    view_->critical(msg);
}

void UserInterface::debug(const QString& msg)
{
    view_->debug(msg);
}

void UserInterface::warning(const QString& msg)
{
    view_->warning(msg);
}

void UserInterface::fatal(const QString& msg)
{
    view_->fatal(msg);
}

UserInterface::UserInterface(std::unique_ptr<ViewInterface>&& view) :
    ViewInterface(), view_( std::move(view) )
{
}


QCoreApplication* UserInterface::loadPlugin(int argc, char* argv[], 
                                            const QString& name, bool verbose)
{
    QPluginLoader loader( Conf::PLUGIN_PATH + "ui_" + name );
    if (!loader.load()){
        QString msg = QString("Failed to load ui-plugin: ") + name;
        qFatal(msg.toLatin1().data());
    }
    
    SignalHandler::UiPlugin* p = 
            qobject_cast<SignalHandler::UiPlugin*>(loader.instance());
    if (p == nullptr){
        QString msg = QString("Failed to cast to UiPlugin: ") + name;
        qFatal(msg.toLatin1().data());
    }
    
    std::unique_ptr<QCoreApplication> app( p->createApp(argc, argv) );
    std::unique_ptr<ViewInterface> view( p->createView(verbose) );
    instance_.reset( new UserInterface( std::move(view) ) );
    return app.release();
}


} // Namespace SignalHandler
