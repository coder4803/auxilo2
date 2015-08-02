
/* scriptbank.cc
 * 
 * This is the imlementation file for classes defined in scriptbank.hh.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 05-Mar-2015
 * Last Modified: 31-Mar-2015
 */

#include "scriptbank.hh"
#include <QFile>

namespace SignalHandler {

ScriptBank::ScriptBank(const ScriptBank::ScriptData& scripts):
    ScriptBankInterface(), scripts_(scripts)
{
}


ScriptBank::~ScriptBank()
{
}


QString ScriptBank::getScript(const QString& scriptID) const
{
    ScriptData::const_iterator it = scripts_.find(scriptID);
    if (it == scripts_.end()){
        throw UnknownScript(scriptID);
    }
    else if (!it.value().from_file){
        return it.value().script;
    }
    else{
        // Read script from file.
        return ScriptBank::readFile(it.value().script);
    }
}


QString ScriptBank::getLanguage(const QString& scriptID) const
{
    ScriptData::const_iterator it = scripts_.find(scriptID);
    if (it == scripts_.end()){
        throw UnknownScript(scriptID);
    }
    else{
        return it.value().language;
    }
}


unsigned int ScriptBank::getPriorityOf(const QString& scriptID) const
{
    ScriptData::const_iterator it = scripts_.find(scriptID);
    
    if (it == scripts_.end()){
        throw UnknownScript(scriptID);
    }
    else{
        return it.value().priority;
    }
}


QString ScriptBank::readFile(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)){
        throw UnknownScript(path);
    }
    return f.readAll();
}


} // Namespace SignalHandler
