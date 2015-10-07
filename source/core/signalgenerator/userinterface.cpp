#include "userinterface.h"
#include "consoleui.h"
#include <QDebug>

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
    Q_UNUSED(argc); Q_UNUSED(argv);
    qDebug() << "Ui-plugin loader not implemented.";
    return nullptr;
}

} // Namespace SignalGenerator
