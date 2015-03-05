
/* scriptbank.cc
 * 
 * This is the imlementation file for classes defined in scriptbank.hh.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 05-Mar-2015
 * Last Modified: 05-Mar-2015
 */

#include "scriptbank.hh"

namespace SignalHandler {

// ScriptBank implementation:
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ScriptBank::ScriptBank(const ScriptBank::ScriptData& scripts):
    scripts_(scripts)
{
    
}


ScriptBank::~ScriptBank()
{
}


bool ScriptBank::isValidID(unsigned int scriptID) const
{
    return scripts_.find(scriptID) != scripts_.end();
}


QString ScriptBank::getScript(unsigned int scriptID) const
{
    ScriptData::const_iterator it = scripts_.find(scriptID);
    if (it == scripts_.end()){
        throw UnknownScript(scriptID);
    }
    else{
        return it->second.script;
    }
}


unsigned int ScriptBank::getPriorityOf(unsigned int scriptID) const
{
    ScriptData::const_iterator it = scripts_.find(scriptID);
    
    if (it == scripts_.end()){
        throw UnknownScript(scriptID);
    }
    else{
        return it->second.priority;
    }
}


ScriptBank::ScriptInfo ScriptBank::at(unsigned int scriptID) const
{
    ScriptData::const_iterator it = scripts_.find(scriptID);    
    if (it == scripts_.end()){
        throw UnknownScript(scriptID);
    }
    else{
        return it->second;
    }
}


ScriptBank::ScriptInfo ScriptBank::operator[](unsigned int scriptID) const
{
    // No error checks are made.
    return scripts_.find(scriptID)->second;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// UnknownScript implementation:
// ---------------------------------------------------------------------------

UnknownScript::UnknownScript(unsigned int scriptID): scriptID_(scriptID)
{
}


UnknownScript::~UnknownScript() noexcept
{
}


const char* UnknownScript::what() const noexcept
{
    return "SignalHandler::UnknownScript";
}


unsigned int UnknownScript::getUnknownID() const noexcept
{
    return scriptID_;
}

// ---------------------------------------------------------------------------



} // Namespace SignalHandler
