#include <QVariant>

#include "message.h"
#include "setstateackmessage.h"

#ifndef UTILS_SETSTATEMESSAGE_H
#define UTILS_SETSTATEMESSAGE_H

namespace Utils {

/*!
 * \brief The SetStateMessage class
 * This class can be used to create and parse SetState messages.
 */
class SetStateMessage : public Message
{
public:
   /*!
    * \brief Constructor.
    * This constructor is used to create state set message from scratch.
    * \param name Name of state.
    * \param value Value for state.
    * \param ackGroup Group name to send acknowledge message.
    */
   SetStateMessage(QString name,
                   QVariant value,
                   QString ackGroup = QString());

   /*!
    * \brief Constructor.
    * This constructor is used to create state set message from
    * received binary data.
    * \param data Received binary data.
    */
   SetStateMessage(QByteArray data);

   /*!
    * \brief Returns state name.
    * \return State name.
    */
   QString name() const { return m_name; }

   /*!
    * \brief Returns state value.
    * \return State value.
    */
   QVariant value() const { return m_value; }

   /*!
    * \brief Returns acknowledge group name.
    * \return Acknowledge group to send aknowledge message.
    */
   QString ackGroup() const { return m_ackGroup; }

   /*!
    * \brief Returns acknowledge id.
    * \return Acknowledge id.
    */
   quint32 ackId() const { return m_ackId; }

   /*!
    * \brief Creates acknowledge message for this message.
    * It is highly recommended to use this method to create
    * acknowledge messages.
    * \param result Result of setting state.
    * \return Acknowledge message for this message.
    */
   SetStateAckMessage createAckMessage(SetStateAckMessage::Result result) const;

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
   QString m_name;
   QVariant m_value;
   QString m_ackGroup;
   quint32 m_ackId;
};

} // Utils

#endif
