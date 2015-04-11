#include <QDataStream>

#include "signalackmessage.h"

namespace Utils {

SignalAckMessage::SignalAckMessage(quint32 ackId,
                                   Result result) :
   Message(),
   m_ackId(ackId),
   m_result(result)
{
}

SignalAckMessage::SignalAckMessage(QByteArray data) :
   Message()
{
   QDataStream stream(data);
   stream >> m_ackId;

   int tmp;
   stream >> tmp;
   m_result = static_cast<Result>(tmp);
}

QByteArray SignalAckMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);
   stream << m_ackId;
   stream << static_cast<int>(m_result);

   return message;
}

QString SignalAckMessage::string() const
{
   QString message("SignalAckMessage:\n");
   message += QString("- AckId: %1\n").arg(m_ackId);
   message += QString("- Result: %1\n").arg(m_result);

   return message;
}

} // Utils
