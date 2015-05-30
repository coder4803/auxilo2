#include "scriptapiqobjectwrapper.hh"

namespace SignalHandler
{

ScriptApiQObjectWrapper::ScriptApiQObjectWrapper(ScriptAPI* api, 
                                                 QObject* parent):
    QObject(parent), api_(api)
{
}


ScriptApiQObjectWrapper::~ScriptApiQObjectWrapper()
{    
}


ScriptAPI::DateTime ScriptApiQObjectWrapper::dateTimeNow() const
{
    return api_->dateTimeNow();
}

Utils::StateResponseMessage::State 
ScriptApiQObjectWrapper::getStateOf(const QString& stateName)
{
    return api_->getStateOf(stateName);
}


ScriptAPI::StateMap 
ScriptApiQObjectWrapper::getStates(const QStringList& states)
{
    return api_->getStates(states);
}


int ScriptApiQObjectWrapper::setState(const QString& stateName, 
                                      const QVariant& value)
{
    return api_->setState(stateName, value);
}


int ScriptApiQObjectWrapper::sendSignal(const QString& signalName,
                                        const QStringList& args)
{
    return api_->sendSignal(signalName, args);
}

} //Namespace SignalHandler
