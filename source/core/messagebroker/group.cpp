#include "group.h"
#include "connection.h"

namespace Core {

Group::Group(QObject *parent) :
   QObject(parent)
{
}

void Group::addConnection(Connection* connection)
{
   m_iter = m_connections.insert(connection);
}

void Group::removeConnection(Connection* connection)
{
   m_connections.remove(connection);
   m_iter = m_connections.begin();
}

void Group::publishMessage(const QByteArray& payload)
{
   // Send to all listeners
   foreach (Connection* connection, m_connections) {
      connection->sendMessage(payload);
   }
}

void Group::deliverMessage(const QByteArray& payload)
{
   // Send only to one listener
   ++m_iter;
   if (m_iter == m_connections.end()) {
      m_iter = m_connections.begin();
   }

   (*m_iter)->sendMessage(payload);
}

}
