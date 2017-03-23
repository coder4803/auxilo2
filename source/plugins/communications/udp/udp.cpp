#include <QTimer>
#include <QTcpSocket>

#include "udp.h"

namespace Plugins {

const QString UDP::PARAMETER_LOCAL_PORT("localport");
const QString UDP::PARAMETER_LOCAL_IP_ADDRESS("localipaddress");
const QString UDP::PARAMETER_REMOTE_PORT("remoteport");
const QString UDP::PARAMETER_REMOTE_IP_ADDRESS("remoteipaddress");

UDP::UDP(const Utils::ParameterSet& parameters,
         QObject* parent) :
   Communication(parent),
   m_localPort(0),
   m_localIpAddress(QHostAddress::Any),
   m_remotePort(0),
   m_remoteIpAddress(QHostAddress::Null)
{
   // Read parameters.
   try {
      // Convert local port.
      m_localPort = parameters.parameter<quint16>(PARAMETER_LOCAL_PORT);

      // Convert local ip address.
      m_localIpAddress = QHostAddress(
               parameters.parameter<QString>(PARAMETER_LOCAL_IP_ADDRESS,
                                             QString("0.0.0.0")));

      // Convert remote port.
      m_remotePort = parameters.parameter<quint16>(PARAMETER_REMOTE_PORT, 0);

      // Convert remote ip address.
      if (parameters.contains(PARAMETER_REMOTE_IP_ADDRESS)) {
         m_remoteIpAddress = QHostAddress(
                  parameters.parameter<QString>(PARAMETER_REMOTE_IP_ADDRESS));
      }

      qCritical("-Local port: %i", m_localPort);
      qCritical("-Local ip address: %s",
                m_localIpAddress.toString().toLatin1().data());
      qCritical("-Remote port: %i", m_remotePort);
      qCritical("-Remote ip address: %s",
                m_remoteIpAddress.toString().toLatin1().data());

   } catch (QException& e) {
      qCritical("Invalid parameters for UDP.");
      throw e;
   }

   // Bind socket
   if (!m_socket.bind(m_localPort)) {
      qCritical("Failed to bind port: %i", m_localPort);
      throw QException();
   }

   connect(&m_socket, SIGNAL(readyRead()),
           this, SLOT(onDataReceived()));

   // Inform protocol that socket is ready for listening.
   QMetaObject::invokeMethod(this, "onConnected", Qt::QueuedConnection);
}

UDP::~UDP()
{
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

   qint64 result = m_socket.writeDatagram(data, m_remoteIpAddress,
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

void UDP::onDataReceived()
{
   QHostAddress ip;
   quint16 port;

   while (m_socket.hasPendingDatagrams()) {
      qint64 datagramLength = m_socket.pendingDatagramSize();
      QByteArray buffer(datagramLength, 0);

      qint64 length = m_socket.readDatagram(buffer.data(), buffer.size(),
                                            &ip, &port);
      // Discard invalid messages.
      if (length != datagramLength) {
         continue;
      }

      // Use sender's ip address from first received message.
      if (m_remoteIpAddress == QHostAddress::Null) {
         m_remoteIpAddress = ip;
      }

      // Use sender's port from first received message.
      if (m_remotePort == 0) {
         m_remotePort = port;
      }

      emit dataReceived(buffer, -1);
   }
}

} // Plugins
