#include "pythoninterpreter.h"
#include "scriptapiadapter.h"

namespace PythonPlugin
{

PythonInterpreter::PythonInterpreter() :
    SignalHandler::ScriptInterpreter()
{ 
}


PythonInterpreter::~PythonInterpreter()
{
}


PythonInterpreter::run(const QString &script,
                       const QStringList &args,
                       SignalHandler::ScriptAPI *services)
{
    ScriptApiAdapter adapter(services);
    // Init interpreter, make api and arguments available to scripts
    // Run sctipt.
}

} // Namespace
