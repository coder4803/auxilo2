/* qtscriptwrapper.cc
 * 
 * This is the implementation file for the QtScriptWrapper class defined in
 * qtscriptwrapper.hh.
 * 
 * Author: Perttu Paarlahti:    perttu.paarlahti@gmail.com
 * Last Modified: 27-May-2015
 */

#include "qtscriptwrapper.hh"
#include "scriptapiqobjectwrapper.hh"
#include <QDebug>

namespace SignalHandler 
{

const char* QtScriptWrapper::LANG_NAME = "QtScript";

QtScriptWrapper::QtScriptWrapper() : 
    ScriptLangWrapper()
{
}


QtScriptWrapper::~QtScriptWrapper()
{   
}


int QtScriptWrapper::run(const QString& script, 
                         const QStringList& args, 
                         ScriptAPI* services)
{
    // Create script engine and set ScriptApi as its global property.
    QScriptEngine engine;
    ScriptApiQObjectWrapper api(services);
    QScriptValue script_api = engine.newQObject(&api);
    engine.globalObject().setProperty("Auxilo2", script_api);
    
    // Create argument list
    QScriptValueList script_args;
    foreach (QString s, args){
        script_args << s;
    }
    
    // Run script.
    engine.evaluate(script);
    QScriptValue program = engine.evaluate("MainFunction");
    int result = program.construct(script_args);
    
    if ( engine.hasUncaughtException() ){
        throw BadScript();
    }
    
    return result;
    // TODO: Invalid argument handling (is needed?).
    // Specify universal starting function (MainFunction?".
}


QString QtScriptWrapper::getLangID() const
{
    return QString(LANG_NAME);
}

} // Namespace SignalHandler
