#include <QObject>

#ifndef UTILS_MESSAGE_H
#define UTILS_MESSAGE_H

namespace Utils {

/*!
 * \brief The Message class
 * Base class for all messages between applications.
 */
class Message
{
public:
   /*!
    * \brief Constructor.
    */
   Message() {}

   //! Destructor.
   virtual ~Message() {}

   /*!
    * \brief Returns message in binary format.
    * \return Message in binary format.
    */
   virtual QByteArray data() const = 0;

   /*!
    * \brief Returns message in string format.
    * This is usefull when debugging.
    * \return Message in string format.
    */
   virtual QString string() const = 0;

protected:
   /*!
    * \brief Returns unique acknowledge id.
    * This id is used to link acknowledge message
    * to its original "parent" messages.
    * \return Unique acknowledge id.
    */
   quint32 nextAckId() { return ++m_ackIdCounter; }

private:
   static quint32 m_ackIdCounter;
};

} // Utils
#endif
