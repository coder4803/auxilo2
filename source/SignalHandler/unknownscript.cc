
/* unknownscript.hh
 * 
 * This is the implementation file for classes defined in unknownscript.hh
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 07-Mar-2015
 * Last modified: 07-Mar-2015
 */

#include "unknownscript.hh"


namespace SignalHandler
{

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


} // Namespace SignalHandler
