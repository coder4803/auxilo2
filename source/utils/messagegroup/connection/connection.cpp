#include <QMutexLocker>

#include "connection.h"
#include "messagegroup.h"
#include "globals.h"

namespace Utils {

const QString Connection::DEFAULT_ADDRESS = "127.0.0.1";
const quint16 Connection::DEFAULT_PORT = 13803;
const quint16 Connection::DEFAULT_RETRY_INTERVAL = 2000; // 2s

QMutex Connection::m_instanceMutex;
Connection* Connection::m_instance = NULL;

QString Connection::m_address = Connection::DEFAULT_ADDRESS;
quint16 Connection::m_port = Connection::DEFAULT_PORT;

Connection* Connection::getInstance()
{
   if (!m_instance) {
      QMutexLocker locker(&m_instanceMutex);

      if (!m_instance) {
         m_instance = new Connection();
      }
   }

   return m_instance;
}

void Connection::deleteInstance()
{
   if (m_instance) {
      QMutexLocker locker(&m_instanceMutex);

      if (m_instance) {
         delete m_instance;
         m_instance = NULL;
      }
   }
}

void Connection::setHost(QString address,
                         quint16 port)
{
   m_address = address;
   m_port = port;

   getInstance()->connectToHost();
}

bool Connection::isConnected() const
{
   return m_socket->state() == QTcpSocket::ConnectedState;
}

void Connection::registerMessageGroup(QString groupName,
                                      MessageGroup* messageGroup)
{
   if (m_groups.keys().contains(groupName)) {
      m_groups[groupName].insert(messageGroup);
   } else {
      QSet<MessageGroup*> messageGroups;
      messageGroups.insert(messageGroup);
      m_groups.insert(groupName, messageGroups);
   }

   QByteArray data;
   QDataStream stream(&data, QIODevice::WriteOnly);
   stream << static_cast<quint8>(Utils::Join);
   stream << groupName;

   QMetaObject::invokeMethod(Connection::getInstance(), "sendMessage",
                             Q_ARG(QByteArray, data));
}

void Connection::unregisterMessageGroup(QString groupName,
                                        MessageGroup* messageGroup)
{
   m_groups[groupName].remove(messageGroup);
   if (m_groups.value(groupName).isEmpty()) {
      m_groups.remove(groupName);
   }
}

void Connection::sendMessage(QByteArray payload)
{
   QMutexLocker locker(&m_socketMutex);

   if (m_socket->state() != QAbstractSocket::ConnectedState) {
      qDebug("Not connected to message broker.");
      return;
   }

   if (m_socket->write(payload) < payload.size()) {
      qDebug("Error while sending data.");
      m_socket->disconnectFromHost();
   }
}

void Connection::connectToHost()
{
   QMutexLocker locker(&m_socketMutex);

   if (m_socket) {
      delete m_socket;
   }

   m_socket = new QTcpSocket(this);

   connect(m_socket, SIGNAL(connected()),
           this, SLOT(onConnected()));
   connect(m_socket, SIGNAL(disconnected()),
           this, SLOT(onDisconnected()));
   connect(m_socket, SIGNAL(readyRead()),
           this, SLOT(onMessageReceived()));

   m_socket->connectToHost(m_address, m_port);
}

void Connection::onConnected()
{
   m_connectionTimer.stop();
   m_dataBuffer.clear();
   emit connected();
}

void Connection::onDisconnected()
{
   m_connectionTimer.start(DEFAULT_RETRY_INTERVAL);
}

void Connection::onMessageReceived()
{
   QMutexLocker locker(&m_socketMutex);

   // Read bytes from socket.
   qint64 bytesAvailable = m_socket->bytesAvailable();
   while (bytesAvailable > 0) {
      QByteArray data = m_socket->read(bytesAvailable);
      if (data.isEmpty()) {
         m_socket->disconnectFromHost();
         return;
      }

      m_dataBuffer += data;

      bytesAvailable = m_socket->bytesAvailable();
   }

   locker.unlock();

   // Parse message.
   QDataStream stream(m_dataBuffer);
   int bytesHandled = 0;

   while (bytesHandled < m_dataBuffer.size()) {
      QString groupName;
      stream >> groupName;

      QByteArray payload;
      stream >> payload;

      if (stream.status() != QDataStream::Ok) {
         break;
      }

      // Mark data as handled.
      bytesHandled = stream.device()->pos();

      // Emit message to all group instances.
      foreach (MessageGroup* messageGroup, m_groups.value(groupName)) {
         emit messageGroup->messageReceived(payload, groupName);
      }
   }

   // Remove handled data from buffer.
   m_dataBuffer = m_dataBuffer.mid(bytesHandled);
}

Connection::Connection() :
   m_socket(NULL)
{
   qRegisterMetaType<QByteArray>("QByteArray");

   connectToHost();
   connect(&m_connectionTimer, SIGNAL(timeout()),
           this, SLOT(connectToHost()));
}

Connection::~Connection()
{
}

} // Utils
