#ifndef QTSCRIPTINTERPRETERPLUGIN_HH
#define QTSCRIPTINTERPRETERPLUGIN_HH

#include <QObject>
#include "interpreterplugin.hh"

namespace QtScriptPlugin
{

class QtScriptInterpreterPlugin : 
        public QObject, 
        public SignalHandler::InterpreterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SIGNALHANDLER_INTERPRETERPLUGIN_IID)
    Q_INTERFACES(SignalHandler::InterpreterPlugin)
    
public:
    
    explicit QtScriptInterpreterPlugin(QObject *parent = 0);
    
    virtual ~QtScriptInterpreterPlugin();
    
    virtual SignalHandler::ScriptInterpreter* createInterpreter();
    
    virtual QString getLangId() const;
};

} // Namespace QtScriptPlugin

#endif // QTSCRIPTINTERPRETERPLUGIN_HH
