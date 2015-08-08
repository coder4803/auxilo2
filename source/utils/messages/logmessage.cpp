#include <QDataStream>

#include "logmessage.h"

namespace Utils {

LogMessage::LogMessage(QString message,
                       LogType type = INFO,
                       QString sender) :
   m_message(message),
   m_type(type),
   m_sender(sender)
{
}

LogMessage::LogMessage(QByteArray data)
{
   QDataStream stream(data);

   int type;
   stream >> type;
   m_type = static_cast<LogType>(type);

   stream >> m_message;
   stream >> m_sender;
}

LogMessage::LogMessage(const LogMessage& other) :
   m_message(other.message()),
   m_type(other.type()),
   m_sender(other.sender())
{
}

LogMessage
LogMessage::operator=(const LogMessage& other)
{
   if (&other != this) {
      m_message = other.message();
      m_type = other.type();
      m_sender = other.sender();
   }

   return *this;
}

QByteArray LogMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);
   stream << static_cast<int>(m_type);
   stream << m_message;
   stream << m_sender;

   return message;
}

QString LogMessage::string() const
{
   QString message("LogMessage:\n");
   message += QString("- Message: %1\n").arg(m_message);
   message += QString("- Type: %1\n").arg(m_type);
   message += QString("- Sender: %1\n").arg(m_sender);

   return message;
}

} // Utils
