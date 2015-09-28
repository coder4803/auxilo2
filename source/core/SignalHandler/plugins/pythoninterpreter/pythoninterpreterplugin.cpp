#include "pythoninterpreterplugin.h"

namespace PythonPlugin
{


PythonInterpreterPlugin::PythonInterpreterPlugin(QObject *parent) :
    QObject(parent), SignalHandler::InterpreterPlugin()
{
}


PythonInterpreterPlugin::~PythonInterpreterPlugin()
{
}


SignalHandler::ScriptInterpreter *PythonInterpreterPlugin::createInterpreter()
{
    return nullptr;
}


QString PythonInterpreterPlugin::getLangId() const
{
    return QString("python");
}

} // Namespace PythonPlugin
