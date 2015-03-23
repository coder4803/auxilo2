#include <QStringList>
#include <QDataStream>

#include "signalmessage.h"

namespace Utils {

SignalMessage::SignalMessage(QString signalName,
                             QStringList parameters,
                             QString ackGroup) :
   Message(),
   m_signalName(signalName),
   m_parameters(parameters),
   m_ackGroup(ackGroup),
   m_ackId(0)
{
   if (!ackGroup.isEmpty()) {
      m_ackId = this->nextAckId();
   }
}

SignalMessage::SignalMessage(QByteArray data) :
   Message()
{
   QDataStream stream(data);
   stream >> m_signalName;
   stream >> m_parameters;
   stream >> m_ackGroup;

   if (!m_ackGroup.isEmpty()) {
      stream >> m_ackId;
   }
}

QString SignalMessage::parameter(quint8 index) const
{
   Q_ASSERT(m_parameters.size() > index);
   return m_parameters.at(index);
}

SignalAckMessage SignalMessage::createAckMessage(
      SignalAckMessage::Result result) const
{
   return SignalAckMessage(m_ackId, result);
}

QByteArray SignalMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);

   stream << m_signalName;
   stream << m_parameters;
   stream << m_ackGroup;

   if (!m_ackGroup.isEmpty()) {
      stream << m_ackId;
   }

   return message;
}

QString SignalMessage::string() const
{
   QString message("SignalMessage:\n");
   message += QString("- Name: %1\n").arg(m_signalName);

   if (!m_ackGroup.isEmpty()) {
      message +=(QString("- AckGroup: %1\n")).arg(m_ackGroup);
      message += QString("- Ack id: %1\n").arg(m_ackId);
   }

   for (int i = 0; i < m_parameters.size(); ++i) {
      message += (QString("- Param %1: %2\n").arg(i+1)
                                             .arg(m_parameters.at(i)));
   }

   return message;
}

} // Utils
