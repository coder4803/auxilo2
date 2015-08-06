#include <QVariant>

#include "message.h"
#include "statechangedackmessage.h"

#ifndef UTILS_STATECHANGEDMESSAGE_H
#define UTILS_STATECHANGEDMESSAGE_H

namespace Utils {

/*!
 * \brief The StateChangedMessage class
 * This class can be used to create and parse state change messages.
 */
class StateChangedMessage : public Message
{
public:
   /*!
    * \brief Constructor.
    * This constructor is used to create state change message from scratch.
    * \param label Label for state name that target understands.
    * \param value New value.
    * \param ackGroup Name of group to send ack message.
    */
   StateChangedMessage(QString label,
                       QVariant value,
                       QString ackGroup = QString());

   /*!
    * \brief Constructor.
    * This constructor is used to create state change message from
    * received binary data.
    * \param data Received binary data.
    */
   StateChangedMessage(QByteArray data);

   //! Copy-constructor
   StateChangedMessage(const StateChangedMessage& other);

   //! Copy-assignment
   StateChangedMessage operator=(const StateChangedMessage& other);

   /*!
    * \brief Retuns label. Label is alias for state name.
    * \return Label.
    */
   QString label() const { return m_label; }

   /*!
    * \brief Returns new state value.
    * \return New state value.
    */
   QVariant value() const { return m_value; }

   /*!
    * \brief Returns acknowledge group's name.
    * \return Name of acknowledge group.
    */
   QString ackGroup() const { return m_ackGroup; }

   /*!
    * \brief Returns acknowledge id.
    * \return Acknowledge id.
    */
   quint32 ackId() const { return m_ackId; }

   /*!
    * \brief Creates acknowledge message for this message.
    * It is highly recommended to use this method to create acknowledge
    * messages.
    * \param result Result of changing state.
    * \return  Acknowledge message for this message.
    */
   StateChangedAckMessage createAckMessage(
         StateChangedAckMessage::Result result) const;

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
   QString m_label;
   QVariant m_value;
   QString m_ackGroup;
   quint32 m_ackId;
};

}

#endif
