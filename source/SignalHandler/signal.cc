
/* signal.cc
 * Implementation file for the Signal class defined in signal.hh.
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 04-Mar-2015
 * Last Modified: 31-Mar-2015
 */

#include "signal.hh"

namespace SignalHandler
{


Signal::Signal() : priority_(0), scriptID_(), parameters_()
{
}


Signal::Signal(unsigned int priority,
               QString scriptID,
               QStringList parameters):
    
    priority_(priority), scriptID_(scriptID), parameters_(parameters)
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


bool Signal::operator<(const Signal& rhs) const
{
    // Compare priorities
    return this->getPriority() > rhs.getPriority();
}

} // Namespace SignalHandler
