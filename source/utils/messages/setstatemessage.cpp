#include "setstatemessage.h"
#include "setstateackmessage.h"

namespace Utils {

SetStateMessage::SetStateMessage(QString name,
                                 QVariant value,
                                 QString ackGroup) :
   Message(),
   m_name(name),
   m_value(value),
   m_ackGroup(ackGroup),
   m_ackId(0)
{
   if (!ackGroup.isEmpty()) {
      m_ackId = this->nextAckId();
   }
}

SetStateMessage::SetStateMessage(QByteArray data) :
   Message(),
   m_ackId(0)
{
   QDataStream stream(data);

   stream >> m_name;
   stream >> m_value;
   stream >> m_ackGroup;

   if (!m_ackGroup.isEmpty()) {
      stream >> m_ackId;
   }
}

SetStateMessage::SetStateMessage(const SetStateMessage& other) :
   m_name(other.name()),
   m_value(other.value()),
   m_ackGroup(other.ackGroup()),
   m_ackId(other.ackId())
{
}

SetStateMessage
SetStateMessage::operator=(const SetStateMessage& other)
{
   if (&other != this) {
      m_name = other.name();
      m_value = other.value();
      m_ackGroup = other.ackGroup();
      m_ackId = other.ackId();
   }

   return *this;
}

SetStateAckMessage SetStateMessage::createAckMessage(SetStateAckMessage::Result result) const
{
   return SetStateAckMessage(m_ackId, result);
}

QByteArray SetStateMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);

   stream << m_name;
   stream << m_value;
   stream << m_ackGroup;

   if (!m_ackGroup.isEmpty()) {
      stream << m_ackId;
   }

   return message;
}

QString SetStateMessage::string() const
{
   QString message("SetStateMessage:\n");
   message += QString("- Name: %1\n").arg(m_name);
   message += QString("- Value: 0x%1\n").arg(m_value.toByteArray().toHex().data());

   if (!m_ackGroup.isEmpty()) {
      message += QString("- Ack group: %1\n").arg(m_ackGroup);
      message += QString("- Ack id: %1\n").arg(m_ackId);
   }

   return message;
}

} // Utils
