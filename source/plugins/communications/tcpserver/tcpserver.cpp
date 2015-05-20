#include <QTimer>
#include <QTcpSocket>

#include "tcpserver.h"

namespace Plugins {

const QString TCPServer::PARAMETER_MAX_CONNECTIONS("maxconnections");
const QString TCPServer::PARAMETER_LOCAL_PORT("localport");

const quint16 TCPServer::DEFAULT_MAX_CONNECTIONS(5);

TCPServer::TCPServer(const Utils::ParameterSet& parameters,
                     QObject* parent) :
   Communication(parent),
   m_localPort(0),
   m_maxConnections(DEFAULT_MAX_CONNECTIONS),
   m_connectionIdCounter(0)
{
   // Read parameters.
   try {
      m_localPort = parameters.parameter<quint16>(PARAMETER_LOCAL_PORT);
      qCritical("-Local port: %i", m_localPort);

      m_maxConnections = parameters.parameter<quint16>(PARAMETER_MAX_CONNECTIONS,
                                                       DEFAULT_MAX_CONNECTIONS);
      qCritical("-Maximum number of connections: %i", m_maxConnections);
   } catch (QException& e) {
      qCritical("Invalid parameters for TCPServer.");
      throw e;
   }

   // Create server.
   connect(&m_server, SIGNAL(newConnection()),
           this, SLOT(onNewConnection()));
   m_server.listen(QHostAddress::Any, m_localPort);
}

TCPServer::~TCPServer()
{
   foreach (QTcpSocket* socket, m_connections.keys()) {
      socket->deleteLater();
   }
   m_connections.clear();
}

bool TCPServer::isConnected(qint32 connectionId) const
{
   if (m_connections.values().contains(connectionId)) {
      if (m_connections.key(connectionId)->state() ==
          QAbstractSocket::ConnectedState) {
         return true;
      }
   }

   return false;
}

void TCPServer::readData(qint32 connectionId)
{
   Q_UNUSED(connectionId)
}

bool TCPServer::sendData(const QByteArray& data,
                         qint32 connectionId)
{
   if (m_connections.values().contains(connectionId)) {
      QTcpSocket* connection = m_connections.key(connectionId);
      if (connection) {
         qint64 result = connection->write(data);
         if (result < data.length()) {
            return false;
         }

         return true;
      }
   }

   return false;
}

void TCPServer::onNewConnection()
{
   while (m_server.hasPendingConnections()) {
      QTcpSocket* connection = m_server.nextPendingConnection();
      if (connection) {
         // Ignore connection if we already have max number of connections.
         if (m_connections.size() >= m_maxConnections) {
            connection->close();
            connection->deleteLater();
            return;
         }

         connect(connection, SIGNAL(disconnected()),
                 this, SLOT(onDisconnected()));
         connect(connection, SIGNAL(readyRead()),
                 this, SLOT(onDataReceived()));

         qint32 connectionId = nextConnectionId();
         m_connections.insert(connection, connectionId);
         emit connected(connectionId);
      }
   }
}

void TCPServer::onDisconnected()
{
   QTcpSocket* connection = dynamic_cast<QTcpSocket*>(this->sender());
   qint32 connectionId = m_connections.value(connection);

   m_connections.remove(connection);
   delete connection;

   emit disconnected(connectionId);
}

void TCPServer::onDataReceived()
{
   QTcpSocket* connection = dynamic_cast<QTcpSocket*>(this->sender());
   qint64 bytesAvailable = connection->bytesAvailable();
   if (bytesAvailable > 0) {
      QByteArray buffer = connection->read(bytesAvailable);
      if (!buffer.isEmpty()) {
         qint32 connectionId = m_connections.value(connection);
         emit dataReceived(buffer, connectionId);
      }
   }
}

qint32 TCPServer::nextConnectionId()
{
   // Find next free connection id.
   while (m_connections.values().contains(m_connectionIdCounter)) {
      ++m_connectionIdCounter;
      if (m_connectionIdCounter < 0) {
         m_connectionIdCounter = 0;
      }
   }

   return m_connectionIdCounter;
}

} // Plugins
