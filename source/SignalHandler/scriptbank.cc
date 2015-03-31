
/* scriptbank.cc
 * 
 * This is the imlementation file for classes defined in scriptbank.hh.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 05-Mar-2015
 * Last Modified: 31-Mar-2015
 */

#include "scriptbank.hh"

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
    else{
        return it->second.script;
    }
}


unsigned int ScriptBank::getPriorityOf(const QString& scriptID) const
{
    ScriptData::const_iterator it = scripts_.find(scriptID);
    
    if (it == scripts_.end()){
        throw UnknownScript(scriptID);
    }
    else{
        return it->second.priority;
    }
}


} // Namespace SignalHandler
