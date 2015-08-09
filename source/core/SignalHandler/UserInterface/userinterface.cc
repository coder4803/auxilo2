#include "userinterface.hh"
#include "uifactory.hh"
#include <QDebug>

std::unique_ptr<UserInterface> UserInterface::instance_(nullptr);

namespace
{

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

void UserInterface::initUI(const QStringList& args)
{
    Q_ASSERT(instance_ == nullptr);
    
    std::unique_ptr<ViewInterface> view(nullptr);
    view.reset( UiFactory::create(args) );
    if (view == nullptr){
        qFatal("Invalid commandline arguments");
        return;
    }
    instance_.reset(new UserInterface( std::move(view) ));
    qInstallMessageHandler(messageHandler);
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

