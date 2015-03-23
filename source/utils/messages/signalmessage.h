#include <QStringList>

#include "message.h"
#include "signalackmessage.h"

#ifndef UTILS_SIGNALMESSAGE_H
#define UTILS_SIGNALMESSAGE_H

namespace Utils {

/*!
 * \brief The SignalMessage class
 * This class can be used to create and parse signal messages.
 */
class SignalMessage : public Message
{
public:
   /*!
    * \brief Constructor to create message from scratch.
    * \param signalName Signal's name.
    * \param parameters List of parameters.
    * \param ackGroup Name of group to send ack message.
    */
   SignalMessage(QString signalName,
                 QStringList parameters = QStringList(),
                 QString ackGroup = QString());

   /*!
    * \brief Constructor to create message from binary data.
    * \param data Binary data.
    */
   SignalMessage(QByteArray data);

   /*!
    * \brief Append parameter to parameter list.
    * \param value Parameter's value.
    */
   void appendParameter(QString value) { m_parameters.append(value); }

   /*!
    * \brief Set acknowledge group.
    * \param group Group's name.
    */
   void setAckGroup(QString group) { m_ackGroup = group; }

   /*!
    * \brief Returns signal name.
    * \return Signal name.
    */
   QString signalName() const { return m_signalName; }

   /*!
    * \brief Returns number of parameters.
    * \return Number of parameters.
    */
   quint32 numberOfParameters() const { return m_parameters.size(); }

   /*!
    * \brief Returns value of specified parameter.
    * \param index Parameter index on list.
    * \return Parameter value.
    * \pre index is less than number of parameters.
    */
   QString parameter(quint8 index) const;

   /*!
    * \brief Return list of parameter values.
    * \return List of parameter values.
    */
   QStringList parameters() const { return m_parameters; }

   /*!
    * \brief Returns acknowledge group name.
    * \return Name of acknowledge group.
    */
   QString ackGroup() const { return m_ackGroup; }

   /*!
    * \brief Creates acknowledge message for this message.
    * It is highly recommended to use this method to create acknowledge
    * messages.
    * \param result Result of handling this message (ack status).
    * \return Acknowledge message for SignalMessage.
    */
   SignalAckMessage createAckMessage(SignalAckMessage::Result result) const;

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
   QString m_signalName;
   QStringList m_parameters;
   QString m_ackGroup;
   quint32 m_ackId;
};

} // Utils

#endif
