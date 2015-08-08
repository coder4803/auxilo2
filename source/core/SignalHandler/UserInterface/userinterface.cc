#include "userinterface.hh"
#include "uifactory.hh"
#include <QDebug>

std::unique_ptr<UserInterface> UserInterface::instance_(nullptr);

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

