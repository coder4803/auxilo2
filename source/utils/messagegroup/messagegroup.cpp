#include <QMutexLocker>

#include "messagegroup.h"
#include "globals.h"

namespace Utils {

MessageGroup::MessageGroup(QString name, GroupType type, QObject* parent) :
    QObject(parent),
    m_name(name),
    m_type(type),
    m_ready(false)
{
   if (Connection::getInstance()->isConnected()) {
      Connection::getInstance()->registerMessageGroup(m_name, this);
   }

   connect(Connection::getInstance(), SIGNAL(connected()),
           this, SLOT(onConnected()));
}

MessageGroup::~MessageGroup()
{
   Connection::getInstance()->unregisterMessageGroup(m_name, this);
}

bool MessageGroup::publish(const QByteArray data)
{
   if (m_type == Subscriber) {
      qDebug("Subscriber type groups can not be used to publish messages.");
      return false;
   }

   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);

   stream << static_cast<quint8>(Utils::Publish);
   stream << m_name;
   stream << data;

   if (!Connection::getInstance()->sendMessage(message)) {
      qDebug("Failed to publish message! Is connection ok?");
      return false;
   }

   return true;
}

bool MessageGroup::publish(const Message& message)
{
   return publish(message.data());
}

bool MessageGroup::publish(const QByteArray data, QString group)
{
   MessageGroup messageGroup(group, Publisher);
   return messageGroup.publish(data);
}

bool MessageGroup::publish(const Message& message, QString group)
{
   return publish(message.data(), group);
}

void MessageGroup::onConnected()
{
   Connection::getInstance()->registerMessageGroup(m_name, this);

   if (!m_ready) {
      m_ready = true;
      emit ready();
   }
}

} // Utils
