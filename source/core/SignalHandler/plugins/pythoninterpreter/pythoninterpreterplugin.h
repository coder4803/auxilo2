#ifndef PYTHONINTERPRETERPLUGIN_H
#define PYTHONINTERPRETERPLUGIN_H

#include <QObject>
#include "interpreterplugin.hh"

namespace PythonPlugin
{

class PythonInterpreterPlugin :
        public QObject,
        public SignalHandler::InterpreterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SIGNALHANDLER_INTERPRETERPLUGIN_IID)
    Q_INTERFACES( SignalHandler::InterpreterPlugin )

public:

    PythonInterpreterPlugin(QObject* parent = 0);

    virtual ~PythonInterpreterPlugin();

    virtual SignalHandler::ScriptInterpreter* createInterpreter();

    virtual QString getLangId() const;
};

}

#endif // PYTHONINTERPRETERPLUGIN_H
