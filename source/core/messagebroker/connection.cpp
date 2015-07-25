#include "connection.h"
#include "globals.h"

namespace Core {

Connection::Connection(QTcpSocket* socket, QObject* parent) :
   QObject(parent),
   m_socket(socket)
{
   connect(m_socket, SIGNAL(connected()),
           this, SLOT(onConnected()));

   connect(m_socket, SIGNAL(disconnected()),
           this, SLOT(onDisconnected()));

   connect(m_socket, SIGNAL(readyRead()),
           this, SLOT(onReadyRead()));
}

void Connection::sendMessage(QByteArray payload)
{
   if (m_socket->write(payload) < payload.size()) {
      m_socket->disconnectFromHost();
   }
}

void Connection::onConnected()
{
   m_buffer.clear();
}

void Connection::onDisconnected()
{
   delete m_socket;
   emit disconnected();
}

void Connection::onReadyRead()
{
   QByteArray data;
   qint64 bytesAvailable = m_socket->bytesAvailable();

   while (bytesAvailable > 0) {
      data += m_socket->read(bytesAvailable);
      if (data.isEmpty()) {
         m_socket->disconnectFromHost();
         return;
      }

      bytesAvailable = m_socket->bytesAvailable();
   }

   handleReceivedData(data);
}

void Connection::handleReceivedData(const QByteArray& data)
{
   quint32 handledBytes = 0;

   m_buffer += data;
   QDataStream stream(data);

   while (!m_buffer.isEmpty()) {
      quint8 command = 0;
      bool result = false;

      stream >> command;
      switch(command) {
      case Utils::Join:
         result = handleJoinMessage(stream);
         break;
      case Utils::Publish:
         result = handlePublishMessage(stream);
         break;
      case Utils::Delive:
         result = handleDeliveMessage(stream);
         break;
      default:
         m_socket->disconnectFromHost();
         return;
      }

      if (!result) {
         break;
      }

      handledBytes = stream.device()->pos();
   }

   m_buffer = m_buffer.mid(handledBytes);
}

bool Connection::handleJoinMessage(QDataStream& stream)
{
   QString groupName;
   stream >> groupName;

   if (stream.status() != QDataStream::Ok) {
      return false;
   }

   emit joinToGroup(groupName);

   return true;
}

bool Connection::handlePublishMessage(QDataStream& stream)
{
   QString groupName;
   stream >> groupName;

   QByteArray payload;
   stream >> payload;

   if (stream.status() != QDataStream::Ok) {
      return false;
   }

   emit publishMessage(payload, groupName);

   return true;
}

bool Connection::handleDeliveMessage(QDataStream& stream)
{
   QString groupName;
   stream >> groupName;

   QByteArray payload;
   stream >> payload;

   if (stream.status() != QDataStream::Ok) {
      return false;
   }

   emit deliveMessage(payload, groupName);

   return true;
}

} // Core
