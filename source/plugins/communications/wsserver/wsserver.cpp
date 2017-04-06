#include <QWebSocket>

#include "wsserver.h"

namespace Plugins {

const QString WSServer::PARAMETER_MAX_CONNECTIONS("maxconnections");
const QString WSServer::PARAMETER_LOCAL_PORT("localport");
const QString WSServer::PARAMETER_SERVER_NAME("servername");

const quint16 WSServer::DEFAULT_MAX_CONNECTIONS(5);
const QString WSServer::DEFAULT_SERVER_NAME("Auxilo II WebSocket server");

WSServer::WSServer(const Utils::ParameterSet& parameters,
                     QObject* parent) :
   Communication(parent),
   m_localPort(0),
   m_maxConnections(DEFAULT_MAX_CONNECTIONS),
   m_connectionIdCounter(0)
{
   // Read parameters.
   try {
      m_localPort = parameters.parameter<quint16>(PARAMETER_LOCAL_PORT);
      qCritical() << "-Local port:" << m_localPort;

      m_maxConnections = parameters.parameter<quint16>(PARAMETER_MAX_CONNECTIONS,
                                                       DEFAULT_MAX_CONNECTIONS);
      qCritical() << "-Maximum number of connections:" << m_maxConnections;

      m_serverName = parameters.parameter<QString>(PARAMETER_SERVER_NAME);
      qCritical() << "-Server name:" << m_serverName;
   } catch (QException& e) {
      qCritical() << "Invalid parameters for WSServer.";
      throw e;
   }

   // Create server.
   m_server = new QWebSocketServer(m_serverName, QWebSocketServer::NonSecureMode, this);
   connect(m_server, SIGNAL(newConnection()),
           this, SLOT(onNewConnection()));
   m_server->listen(QHostAddress::Any, m_localPort);
}

WSServer::~WSServer()
{
   foreach (QWebSocket* socket, m_connections.keys()) {
      socket->deleteLater();
   }
   m_connections.clear();
}

bool WSServer::isConnected(qint32 connectionId) const
{
   if (m_connections.values().contains(connectionId)) {
      if (m_connections.key(connectionId)->state() ==
         QAbstractSocket::ConnectedState) {
         return true;
      }
   }

   return false;
}

bool WSServer::sendData(const QByteArray& data,
                         qint32 connectionId)
{
   // If connection id exists.
   if (m_connections.values().contains(connectionId)) {
      QWebSocket* connection = m_connections.key(connectionId);
      if (connection) {
         // Data is always sent as text!
         qint64 result = connection->sendTextMessage(data.data());
         if (result < data.length()) {
            return false;
         }

         return true;
      }
   }

   return false;
}

void WSServer::onNewConnection()
{
   while (m_server->hasPendingConnections()) {
      QWebSocket* connection = m_server->nextPendingConnection();
      if (connection) {
         // Ignore connection if we already have max number of connections.
         if (m_connections.size() >= m_maxConnections) {
            connection->close();
            connection->deleteLater();
            return;
         }

         connect(connection, SIGNAL(disconnected()),
                 this, SLOT(onDisconnected()));
         connect(connection, SIGNAL(textMessageReceived(QString)),
                 this, SLOT(onTextMessageReceived(QString)));

         qint32 connectionId = nextConnectionId();
         m_connections.insert(connection, connectionId);
         emit connected(connectionId);
      }
   }
}

void WSServer::onDisconnected()
{
   QWebSocket* connection = dynamic_cast<QWebSocket*>(this->sender());
   qint32 connectionId = m_connections.value(connection);

   m_connections.remove(connection);
   delete connection;

   emit disconnected(connectionId);
}

void WSServer::onTextMessageReceived(const QString& message)
{
   QWebSocket* connection = dynamic_cast<QWebSocket*>(this->sender());
   qint32 connectionId = m_connections.value(connection);
   emit dataReceived(QByteArray(message.toLatin1().data()), connectionId);
}

qint32 WSServer::nextConnectionId()
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
