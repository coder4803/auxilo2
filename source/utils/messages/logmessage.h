#include "message.h"

#ifndef UTILS_LOGMESSAGE_H
#define UTILS_LOGMESSAGE_H

namespace Utils {

/*!
 * \brief The LogMessage class
 * This class can be used to create and parse Log messages.
 */
class LogMessage : public Message
{
public:
   /*!
    * \brief Describes log type.
    */
   enum LogType {
      INFO,    //!< Info logs (harmless, maybe normal functionality).
      WARNING, //!< Warning logs (something may not work correctly).
      ERROR,   //!< Error logs (something fails but execution can continue).
      FATAL    //!< Fatal logs (execution can't be continued).
   };

   /*!
    * \brief Constructor.
    * This constructor is used to create log message from scratch.
    * \param message Log message string.
    * \param type Log type.
    * \param sender Log message sender.
    */
   LogMessage(QString message,
              LogType type,
              QString sender = QString());

   /*!
    * \brief Constructor.
    * This constructor is used to parse log message from received
    * binary data.
    * \param data Received binary data.
    */
   LogMessage(QByteArray data);

   /*!
    * \brief Returns log message string.
    * \return Log message string.
    */
   QString message() const { return m_message; }

   /*!
    * \brief Returns log message type.
    * \return Log message type.
    */
   LogType type() const { return m_type; }

   /*!
    * \brief Returns log message sender.
    * \return Log message sender.
    */
   QString sender() const { return m_sender; }

   /*!
    * \brief Returns message in binary format.
    * \return Message in binary format.
    */
   QByteArray data() const;

   /*!
    * \brief Returns message in string format.
    * This is useful when debugging.
    * \return Message in string format.
    */
   QString string() const;

private:
   QString m_message;
   LogType m_type;
   QString m_sender;
};

} // Utils

#endif
