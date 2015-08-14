
/* scriptrunexceptions.hh
 * 
 * This header defines and implements exception classes that indicate errors 
 * with running scripts.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 02-April-2015
 * Last modified : 02-April-2015
 */

#ifndef SCRIPTRUNEXCEPTIONS_HH
#define SCRIPTRUNEXCEPTIONS_HH

#include <exception>

namespace SignalHandler
{

/*!
 * \brief The InvalidParameters class
 * Exception that indicates that parameters passed to the script were invalid.
 */
class InvalidParameters : public std::exception
{
public:
    
    //! Constructor
    InvalidParameters() {}
    
    //! Constructor
    virtual ~InvalidParameters() noexcept {}
    
    //! Implements the std::exception interface.
    virtual const char* what() const noexcept 
    {
        return "SignalHandler::InvalidParameters";
    }
};


/*!
 * \brief The BadScript class
 * Exception class that indicates crashing or unrunnable script.
 */
class BadScript : public std::exception
{
public:
    
    //! Constructor
    BadScript() {}
    
    //! Constructor
    virtual ~BadScript() noexcept {}
    
    //! Implements the std::exception interface.
    virtual const char* what() const noexcept 
    {
        return "SignalHandler::BadScript";
    }
};

        
}
#endif // SCRIPTRUNEXCEPTIONS_HH
