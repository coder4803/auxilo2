
/* signalparser.cc
 * 
 * This is the implementation file for the SignalParser class and its 
 * exception class BadMessage.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 04-Mar-2015
 * Last Modified: 04-Mar-2015
 */


#include "signalparser.hh"
#include <vector>
#include <QStringList>

namespace SignalHandler 
{


Signal SignalParser::parse(const QString& message)
{
    QStringList fields = message.split(';');
    if (fields.size() < 2){
        // too few fields.
        throw BadMessage( message.toStdString() );
    }    
    
    // Get priority number
    bool ok(true);
    unsigned int priority = fields.at(0).toUInt(&ok);
    if (!ok){
        // priority field is not an unsigned integer.
        throw BadMessage( message.toStdString() );
    }
    
    // Get scriptID
    unsigned int scriptID = fields.at(1).toUInt(&ok);
    if (!ok){
        // scriptID field is not an unsigned integer.
        throw BadMessage( message.toStdString() );
    }
    
    // Get additional parameters
    std::vector<QString> parameters;
    for (int i = 2; i < fields.size(); ++i){
        parameters.push_back( fields.at(i) );
    }
    
    return Signal(priority, scriptID, parameters);
}


// BadMessage methods
// ----------------------------------------------------------------------------

BadMessage::BadMessage(std::string invalid_message):
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


const std::string& BadMessage::getInvalidMessage() const noexcept
{
    return invalid_msg_;
}


} // Namespace SignalHandler
