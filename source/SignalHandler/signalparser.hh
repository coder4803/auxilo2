
/* signalparser.hh
 * 
 * This header file declares the SignalParser class and its exception class
 * BadMessage.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 04-Mar-2015
 * Last Modified: 04-Mar-2015
 */

#ifndef SIGNALPARSER_HH
#define SIGNALPARSER_HH

#include "signal.hh"
#include <QString>
#include <exception>


namespace SignalHandler
{

/*!
 * \brief The SignalParser class
 * Class that converts incoming messages into Signal objects.
 */
class SignalParser
{
public:
    
    //! Default constructor
    SignalParser() = default;
    
    //! Default destructor
    ~SignalParser() = default;
    
    //! Default copy-constructor
    SignalParser(const SignalParser&) = default;
    
    //! Default copy-assignment operator.
    SignalParser& operator = (const SignalParser&) = default;
    
    /*!
     * \brief parse Converts given message into a Signal object.
     * \param message Message to be converted.
     * \return Converted Signal.
     * \pre -
     * \exception BadMessage is thrown, if message is invalid.
     * 
     * Valid Messages are of form:
     * priority;scriptID; ...arbitary number of arguments seperated with ';'
     * Where priority and scriptID are non-negative integers.
     */
    static Signal parse(const QString& message);
};




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
    BadMessage(std::string invalid_message);
    
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
    const std::string& getInvalidMessage() const noexcept;
    
private:
    std::string invalid_msg_;
};

} // Namespace SignalHandler

#endif // SIGNALPARSER_HH
