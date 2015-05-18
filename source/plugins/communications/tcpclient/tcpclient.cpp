#include <QTimer>

#include "tcpclient.h"

namespace Plugins {

const QString TCPClient::PARAMETER_REMOTE_ADDRESS("remoteaddress");
const QString TCPClient::PARAMETER_REMOTE_PORT("remoteport");

TCPClient::TCPClient(const Utils::ParameterSet& parameters,
                     QObject* parent) :
   Communication(parent),
   m_remotePort(0)
{
   // Read parameters.
   try {
      m_remoteAddress = parameters.parameter<QString>(PARAMETER_REMOTE_ADDRESS);
      qCritical("-Remote address: %s", m_remoteAddress.toLatin1().data());

      m_remotePort = parameters.parameter<quint16>(PARAMETER_REMOTE_PORT);
      qCritical("-Remote port: %i", m_remotePort);
   } catch (QException& e) {
      qCritical("Invalid parameters for TCPClient.");
      throw e;
   }

   // Connect socket signals.
   connect(&m_socket, SIGNAL(connected()),
           this, SLOT(onConnected()));
   connect(&m_socket, SIGNAL(disconnected()),
           this, SLOT(onDisconnected()));
   connect(&m_socket, SIGNAL(readyRead()),
           this, SLOT(onDataReceived()));
   connect(&m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
           this, SLOT(stateChanged(QAbstractSocket::SocketState)));

   // Connect to remote host.
   QTimer::singleShot(0, this, SLOT(connectToRemoteHost()));
}

bool TCPClient::isConnected(qint32 connectionId) const
{
   Q_UNUSED(connectionId)

   if (m_socket.state() == QTcpSocket::ConnectedState) {
      return true;
   }

   return false;
}

void TCPClient::readData(qint32 connectionId)
{
   Q_UNUSED(connectionId)
}

bool TCPClient::sendData(const QByteArray& data,
                         qint32 connectionId)
{
   Q_UNUSED(connectionId)

   qint64 result = m_socket.write(data);
   if (result < data.length()) {
      return false;
   }

   return true;
}

void TCPClient::connectToRemoteHost()
{
   m_socket.connectToHost(m_remoteAddress, m_remotePort);
}

void TCPClient::onConnected()
{
   emit connected(-1);
}

void TCPClient::onDisconnected()
{
   emit disconnected(-1);
}

void TCPClient::stateChanged(QAbstractSocket::SocketState state)
{
   if (state == QAbstractSocket::UnconnectedState) {
      connectToRemoteHost();
   }
}

void TCPClient::onDataReceived()
{
   qint64 bytesAvailable = m_socket.bytesAvailable();
   if (bytesAvailable > 0) {
      QByteArray buffer = m_socket.read(bytesAvailable);
      if (buffer.isEmpty()) {
         m_socket.disconnectFromHost();
         return;
      }

      emit dataReceived(buffer, -1);
   }
}

} // Plugins
