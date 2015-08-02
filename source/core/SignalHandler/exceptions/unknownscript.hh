
/* unknownscript.hh
 * 
 * This header file defines exception class to signal unmatching script IDs.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 07-Mar-2015
 * Last modified: 31-Mar-2015
 */

#ifndef UNKNOWNSCRIPT_HH
#define UNKNOWNSCRIPT_HH

#include <exception>
#include <QString>

namespace SignalHandler
{

/*!
 * \brief The UnknownScript class
 * Exception class to signal attempt to retrieve script with unknown ID.
 * Class implements std::exception interface.
 */
class UnknownScript : std::exception
{
public:
    
    /*!
     * \brief UnknownScript Constructor.
     * \param scriptID Unknown ID that caused exception.
     * \pre -
     */
    explicit UnknownScript(const QString& scriptID);
    
    //! Destructor
    virtual ~UnknownScript() noexcept;
    
    /*!
     * \brief what Reimplements std::exception interface.
     * \return Exception class name.
     * \pre -
     */
    virtual const char* what() const noexcept;
    
    /*!
     * \brief getUnknownID Get unknown script ID-number.
     * \return ID-number that caused the exception.
     * \pre -
     */
    QString getUnknownID() const noexcept;
    
    
private:
    QString scriptID_;
};


} // Namespace SignalHandler

#endif // UNKNOWNSCRIPT_HH
