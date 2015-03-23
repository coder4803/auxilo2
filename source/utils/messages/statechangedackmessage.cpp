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

StateChangedAckMessage::StateChangedAckMessage(QByteArray data)
{
   QDataStream stream(data);
   stream >> m_ackId;

   int tmp;
   stream >> tmp;
   m_result = static_cast<Result>(tmp);
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
