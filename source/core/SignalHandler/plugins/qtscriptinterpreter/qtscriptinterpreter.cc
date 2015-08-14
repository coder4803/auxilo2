/* qtscriptwrapper.cc
 * 
 * This is the implementation file for the QtScriptWrapper class defined in
 * qtscriptwrapper.hh.
 * 
 * Author: Perttu Paarlahti:    perttu.paarlahti@gmail.com
 * Last Modified: 27-May-2015
 */

#include "qtscriptinterpreter.hh"
#include "scriptapiadapter.hh"
#include "scriptrunexceptions.hh"
#include <QDebug>


namespace QtScriptPlugin
{

QtScriptInterpreter::QtScriptInterpreter() : 
    ScriptInterpreter()
{
}


QtScriptInterpreter::~QtScriptInterpreter()
{   
}


int QtScriptInterpreter::run(const QString& script, 
                         const QStringList& args, 
                         SignalHandler::ScriptAPI* services)
{
    // Create script engine and set ScriptApi as its global property.
    QScriptEngine engine;
    ScriptApiAdapter api(services, &engine);
    QScriptValue script_api = engine.newQObject(&api);
    engine.globalObject().setProperty("Auxilo2", script_api);
    
    // Set script arguments
    QScriptValue script_args = qScriptValueFromSequence(&engine, args);
    engine.globalObject().setProperty("argv", script_args);
    engine.globalObject().setProperty("argc", engine.newVariant(args.size()) );
    
    
    // Run script.
    engine.evaluate(script);
    if ( engine.hasUncaughtException() ){
        throw SignalHandler::BadScript();
    }
    
    return 0;
    // TODO: Invalid argument handling (is needed?).
    // Return value?
}


QString QtScriptInterpreter::getLangID() const
{
    return QString("qtscript");
}

} // Namespace QtScriptPlugin
