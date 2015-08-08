
/* signal.cc
 * 
 * Implementation file for the Signal class defined in signal.hh.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 04-Mar-2015
 * Last Modified: 04-April-2015
 */

#include "signal.hh"

namespace SignalHandler
{


Signal::Signal() : priority_(0), scriptID_(), parameters_(), ack_info_()
{
}


Signal::Signal(unsigned int priority,
               const QString& scriptID,
               const QStringList& parameters,
               const AckInfo& ack_info):
    
    priority_(priority), scriptID_(scriptID), parameters_(parameters),
    ack_info_(ack_info)
{
}


Signal::~Signal()
{  
}


unsigned int Signal::getPriority() const
{
    return priority_;
}


QString Signal::getScriptID() const
{
    return scriptID_;
}


QStringList Signal::getParameters() const
{
    return parameters_;
}


Signal::AckInfo Signal::getAckInfo() const
{
    return ack_info_;
}


bool Signal::operator<(const Signal& rhs) const
{
    // Compare priorities
    return this->getPriority() > rhs.getPriority();
}


Signal::AckInfo::AckInfo() : ackGroup(), ackID(0), senderName()
{
}

Signal::AckInfo::AckInfo(const QString& group, quint32 id, const QString& sender):
    ackGroup(group), ackID(id), senderName(sender)
{
}

} // Namespace SignalHandler
