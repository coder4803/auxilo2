#include "qtscriptinterpreterplugin.hh"
#include "qtscriptinterpreter.hh"

namespace QtScriptPlugin
{

QtScriptInterpreterPlugin::QtScriptInterpreterPlugin(QObject *parent) : 
    QObject(parent), SignalHandler::InterpreterPlugin()
{
}


QtScriptInterpreterPlugin::~QtScriptInterpreterPlugin()
{    
}


SignalHandler::ScriptInterpreter* QtScriptInterpreterPlugin::createInterpreter()
{
    return new QtScriptInterpreter();
}


QString QtScriptInterpreterPlugin::getLangId() const
{
    return QString("qtscript");
}

} // Namespace QtScriptPlugin
