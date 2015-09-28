#ifndef PYTHONINTERPRETER_H
#define PYTHONINTERPRETER_H

#include "scriptinterpreter.hh"

namespace PythonPlugin
{

class PythonInterpreter : public SignalHandler::ScriptInterpreter
{
public:

    PythonInterpreter();

    virtual ~PythonInterpreter();

    virtual run(const QString &script,
                const QStringList &args,
                SignalHandler::ScriptAPI *services);
};

} // Namespace PythonPlugin

#endif // PYTHONINTERPRETER_H
