#include "signalhandlerbuilder.hh"

namespace SignalHandler
{

SignalHandlerBuilder::SignalHandlerBuilder():
    mx_(), cv_(), received_()
{
}


SignalHandlerBuilder::~SignalHandlerBuilder()
{    
}


ModelInterface* SignalHandlerBuilder::create()
{
    return nullptr;
}


void SignalHandlerBuilder::onMessageReceived(const QByteArray& data)
{
    
}


} // Namespace SignalHandler
