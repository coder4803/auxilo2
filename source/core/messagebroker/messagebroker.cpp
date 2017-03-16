#include <QDataStream>

#include "messagebroker.h"
#include "connection.h"

namespace Core {

MessageBroker::MessageBroker(QObject *parent) :
   QObject(parent)
{
   m_server = new QTcpServer(this);
   connect(m_server, SIGNAL(newConnection()),
           this, SLOT(onNewConnection()));
   m_server->listen(QHostAddress::Any, 13803);
}

void MessageBroker::onNewConnection()
{
   while (m_server->hasPendingConnections()) {
      QTcpSocket* socket = m_server->nextPendingConnection();

      Connection* connection = new Connection(socket, this);
      connect(connection, SIGNAL(joinToGroup(QString)),
              this, SLOT(onJoinToGroup(QString)));
      connect(connection, SIGNAL(publishMessage(QByteArray,QString)),
              this, SLOT(onPublishMessage(QByteArray,QString)));
      connect(connection, SIGNAL(deliveMessage(QByteArray,QString)),
              this, SLOT(onDeliverMessage(QByteArray,QString)));
      connect(connection, SIGNAL(disconnected()),
              this, SLOT(onDisconnect()));

      m_connections.append(connection);
   }
}

void MessageBroker::onJoinToGroup(QString groupName)
{
   if (!m_groups.keys().contains(groupName)) {
      m_groups.insert(groupName, new Group(this));
   }

   Connection* connection = dynamic_cast<Connection*>(this->sender());
   m_groups.value(groupName)->addConnection(connection);
}

void MessageBroker::onPublishMessage(QByteArray payload, QString group)
{
   QByteArray data;
   QDataStream stream(&data, QIODevice::WriteOnly);

   stream << group;
   stream << payload;

   if (m_groups.keys().contains(group)) {
      m_groups.value(group)->publishMessage(data);
   }
}

void MessageBroker::onDeliverMessage(QByteArray payload, QString group)
{
   m_groups.value(group)->deliverMessage(payload);
}

void MessageBroker::onDisconnect()
{
   Connection* connection = dynamic_cast<Connection*>(this->sender());
   foreach (Group* group, m_groups) {
      group->removeConnection(connection);
   }
}

} // Core
