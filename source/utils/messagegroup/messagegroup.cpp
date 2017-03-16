#include <QDataStream>
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
   if (Connection::getInstance()->isConnected() &&
       m_type != Utils::MessageGroup::Publisher)
   {
      Connection::getInstance()->registerMessageGroup(m_name, this);
   }

   connect(Connection::getInstance(), SIGNAL(connected()),
           this, SLOT(onConnected()));
}

MessageGroup::~MessageGroup()
{
   if (m_type != Utils::MessageGroup::Publisher) {
      Connection::getInstance()->unregisterMessageGroup(m_name, this);
   }
}

void MessageGroup::publish(const QByteArray data)
{
   Q_ASSERT(m_type != Subscriber);

   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);

   stream << static_cast<quint8>(Utils::Publish);
   stream << m_name;
   stream << data;

   QMetaObject::invokeMethod(Connection::getInstance(), "sendMessage",
                             Q_ARG(QByteArray, message));
}

void MessageGroup::publish(const Message& message)
{
   return publish(message.data());
}

void MessageGroup::publish(const QByteArray data, QString group)
{
   MessageGroup messageGroup(group, Publisher);
   return messageGroup.publish(data);
}

void MessageGroup::publish(const Message& message, QString group)
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
