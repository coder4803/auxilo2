
/* signal.cc
 * Implementation file for the Signal class defined in signal.hh.
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 04-Mar-2015
 * Last Modified: 04-Mar-2015
 */

#include "signal.hh"

namespace SignalHandler
{


Signal::Signal() : priority_(0), scriptID_(0), parameters_()
{
}


Signal::Signal(unsigned int priority,
               unsigned int scriptID,
               std::vector<QString> parameters):
    
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


unsigned int Signal::getScriptID() const
{
    return scriptID_;
}


std::vector<QString> Signal::getParameters() const
{
    return parameters_;
}


bool Signal::operator<(const Signal& rhs) const
{
    // Compare priorities
    return this->getPriority() > rhs.getPriority();
}

} // Namespace SignalHandler
