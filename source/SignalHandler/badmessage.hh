
/* badmessage.hh
 * 
 * This header file defines the BadMessage exception class that signals invalid
 * messages.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 07-Mar-2015
 * Last modified: 07-Mar-2015
 */

#ifndef BADMESSAGE_HH
#define BADMESSAGE_HH

#include <exception>
#include <QString>

namespace SignalHandler
{

/*!
 * \brief The BadMessage class
 * Exception class for signaling an invalid message. Extends std::exception.
 */
class BadMessage : std::exception
{
public:
    
    /*!
     * \brief BadMessage Constructor
     * \param invalid_message Message that caused the exception.
     * \pre -
     */
    explicit BadMessage(QString invalid_message);
    
    //! Destructor
    virtual ~BadMessage() noexcept;
    
    /*!
     * \brief what Implements std::exception interface.
     * \return Exception class name as c-string.
     * \pre -
     */
    virtual const char* what() const noexcept;
    
    /*!
     * \brief getInvalidMessage Get message that caused the exception.
     * \return Reference to the invalid message.
     * \pre -
     */
    QString getInvalidMessage() const;
    
private:
    QString invalid_msg_;
};

} // Namespace SignalHandler

#endif // BADMESSAGE_HH
