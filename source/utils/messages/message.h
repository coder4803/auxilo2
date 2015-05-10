#include <QObject>

#ifndef UTILS_MESSAGE_H
#define UTILS_MESSAGE_H

namespace Utils {

/*
 * Here is a list of constants which define message group names
 */

/*!
 * \brief Name of group for sending configuration requests.
 * This group is listened by configuration manager. Manager will handle the
 * message and send response to a group that is specified in the request
 * message.
 */
const QString CONF_REQUEST_GROUP("confRequest");

/*!
 * \brief Name of group for sending set state messages.
 * This group is listened by state holder. State holder will set state value
 * according to message information.
 */
const QString SET_STATE_GROUP("setState");

/*!
 * \brief Name of group for sending state requests.
 * This group is listened by state holder. State holder will handle the message
 * and send response to a group that is specified in the request message.
 */
const QString REQUEST_STATE_GROUP("requestState");

/*!
 * \brief The Message class
 * Base class for all messages between applications.
 */
class Message
{
public:
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
