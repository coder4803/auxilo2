#include "requeststatemessage.h"

namespace Utils {

RequestStateMessage::RequestStateMessage(QString responseGroup,
                                         QString state) :
   m_responseGroup(responseGroup)
{
   m_states.append(state);
}

RequestStateMessage::RequestStateMessage(QString responseGroup,
                                         QStringList states) :
   m_responseGroup(responseGroup),
   m_states(states)
{
}

RequestStateMessage::RequestStateMessage(QByteArray data)
{
   QDataStream stream(data);
   stream >> m_responseGroup;
   stream >> m_states;
}

RequestStateMessage::RequestStateMessage(const RequestStateMessage& other) :
   m_responseGroup(other.responseGroup()),
   m_states(other.states())
{
}

RequestStateMessage
RequestStateMessage::operator=(const RequestStateMessage& other)
{
   if (&other != this) {
      m_responseGroup = other.responseGroup();
      m_states = other.states();
   }

   return *this;
}

void RequestStateMessage::appendState(QString state)
{
   m_states.append(state);
   m_states.removeDuplicates();
}

void RequestStateMessage::appendStates(QStringList states)
{
   m_states.append(states);
   m_states.removeDuplicates();
}

QByteArray RequestStateMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);
   stream << m_responseGroup;
   stream << m_states;

   return message;
}

QString RequestStateMessage::string() const
{
   QString message("RequestStateMessage:\n");
   message += QString("- Response group: %1\n").arg(m_responseGroup);

   foreach (QString state, m_states) {
      message.append(QString("- State: %1\n").arg(state));
   }

   return message;
}

} // Utils
