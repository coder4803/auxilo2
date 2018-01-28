#include <QTimer>

#include "udp.h"

namespace Plugins {

const QString UDP::PARAMETER_LOCAL_PORT("localport");
const QString UDP::PARAMETER_REMOTE_PORT("remoteport");
const QString UDP::PARAMETER_REMOTE_IP_ADDRESS("remoteipaddress");

int UDP::m_instanceCounter = 0;
QHash<quint16, SharedUdpSocket*> UDP::m_sockets;
QMutex UDP::m_socketMutex;

UDP::UDP(const Utils::ParameterSet& parameters,
         QObject* parent) :
   Communication(parent),
   m_localPort(0),
   m_remotePort(0),
   m_remoteIpAddress(QHostAddress::Null)
{
   // Read parameters.
   try {
      // Convert local port.
      m_localPort = parameters.parameter<quint16>(PARAMETER_LOCAL_PORT);

      // Convert remote port.
      m_remotePort = parameters.parameter<quint16>(PARAMETER_REMOTE_PORT, 0);

      // Convert remote ip address.
      if (parameters.contains(PARAMETER_REMOTE_IP_ADDRESS)) {
         m_remoteIpAddress = QHostAddress(
                  parameters.parameter<QString>(PARAMETER_REMOTE_IP_ADDRESS));
      }

      qDebug("-Local port: %i", m_localPort);
      qDebug("-Remote port: %i", m_remotePort);
      qDebug("-Remote ip address: %s",
             m_remoteIpAddress.toString().toLatin1().data());

   } catch (QException& e) {
      qCritical("Invalid parameters for UDP.");
      throw e;
   }

   // Connections binded to same port use same socket.
   QMutexLocker locker(&m_socketMutex);
   if (!m_sockets.contains(m_localPort)) {
      SharedUdpSocket* newSocket = new SharedUdpSocket(m_localPort);

      m_sockets.insert(m_localPort, newSocket);
      qDebug() << "Created UDP socket for port" << m_localPort;
   }

   m_socket = m_sockets.value(m_localPort);

   connect(m_socket, SIGNAL(dataReceived(QByteArray,QHostAddress,quint16)),
           this, SLOT(onDataReceived(QByteArray,QHostAddress,quint16)));

   // Inform protocol that socket is ready for listening.
   QMetaObject::invokeMethod(this, "onConnected", Qt::QueuedConnection);

   ++m_instanceCounter;
}

UDP::~UDP()
{
   QMutexLocker locker(&m_socketMutex);

   --m_instanceCounter;
   if (m_instanceCounter <= 0) {
      foreach (SharedUdpSocket* socket, m_sockets) {
         socket->deleteLater();
      }

      m_sockets.clear();
   }
}

bool UDP::isConnected(qint32 connectionId) const
{
   Q_UNUSED(connectionId)
   return true;
}

bool UDP::sendData(const QByteArray& data,
                   qint32 connectionId)
{
   Q_UNUSED(connectionId)

   QMutexLocker locker(&m_socketMutex);

   qint64 result = m_socket->writeDatagram(data, m_remoteIpAddress,
                                           m_remotePort);
   if (result < data.length()) {
      return false;
   }

   return true;
}

void UDP::onConnected()
{
   emit connected(-1);
}

void UDP::onDataReceived(const QByteArray& data,
                         const QHostAddress& host,
                         quint16 port)
{
   // Use sender's ip address from first received message.
   if (m_remoteIpAddress == QHostAddress::Null) {
      m_remoteIpAddress = host;
   }

   // Use sender's port from first received message.
   if (m_remotePort == 0) {
      m_remotePort = port;
   }

   emit dataReceived(data, -1);
}

} // Plugins
