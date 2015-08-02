
/* badmessage.hh
 * 
 * This is the implementation file for the BadMessage class defined in 
 * badmessage.hh
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 07-Mar-2015
 * Last modified: 07-Mar-2015
 */

#include "badmessage.hh"

namespace SignalHandler
{

BadMessage::BadMessage(QString invalid_message):
    std::exception(), invalid_msg_(invalid_message)
{
}


BadMessage::~BadMessage() noexcept
{
}


const char* BadMessage::what() const noexcept
{
    return "SignalHandler::BadMessage";
}


QString BadMessage::getInvalidMessage() const
{
    return invalid_msg_;
}

} // Namespace SignalHandler
