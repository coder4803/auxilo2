#include <QDataStream>

#include "statechangedackmessage.h"

namespace Utils {

StateChangedAckMessage::StateChangedAckMessage(quint32 ackId,
                                               Result result) :
   Message(),
   m_ackId(ackId),
   m_result(result)
{
}

StateChangedAckMessage::StateChangedAckMessage(QByteArray data) :
   Message(),
   m_ackId(0),
   m_result(SUCCEEDED)
{
   QDataStream stream(data);
   stream >> m_ackId;

   int tmp;
   stream >> tmp;
   m_result = static_cast<Result>(tmp);
}

StateChangedAckMessage::StateChangedAckMessage(const StateChangedAckMessage& other) :
   m_ackId(other.ackId()),
   m_result(other.result())
{
}

StateChangedAckMessage
StateChangedAckMessage::operator=(const StateChangedAckMessage& other)
{
   if (&other != this) {
      m_ackId = other.ackId();
      m_result = other.result();
   }

   return *this;
}

QByteArray StateChangedAckMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);
   stream << m_ackId;
   stream << static_cast<int>(m_result);

   return message;
}

QString StateChangedAckMessage::string() const
{
   QString message("StateChangedAckMessage:\n");
   message += QString("- AckId: %1\n").arg(m_ackId);
   message += QString("- Result: %1\n").arg(m_result);

   return message;
}

} // Utils
