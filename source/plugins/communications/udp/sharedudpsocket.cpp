#include <QException>

#include "sharedudpsocket.h"

namespace Plugins {

SharedUdpSocket::SharedUdpSocket(quint16 localPort) :
   m_socket(NULL)
{
   qRegisterMetaType<QHostAddress>("QHostAddress");

   moveToThread(&m_thread);
   m_thread.start();

   QMetaObject::invokeMethod(this, "initSocket", Qt::QueuedConnection,
                             Q_ARG(quint16, localPort));
}

SharedUdpSocket::~SharedUdpSocket()
{
}

qint64 SharedUdpSocket::writeDatagram(const QByteArray& data,
                                      const QHostAddress& host,
                                      quint16 port)
{
   qint64 result = 0;
   QMetaObject::invokeMethod(this, "write", Qt::BlockingQueuedConnection,
                             Q_RETURN_ARG(qint64, result),
                             Q_ARG(QByteArray, data),
                             Q_ARG(QHostAddress, host),
                             Q_ARG(quint16, port));

   return result;
}

void SharedUdpSocket::initSocket(quint16 localPort)
{
   m_socket = new QUdpSocket(this);
   if (!m_socket->bind(localPort)) {
      qCritical("Failed to bind port: %i", localPort);
      throw QException();
   }

   connect(m_socket, SIGNAL(readyRead()),
           this, SLOT(onDataReceived()));
}

qint64 SharedUdpSocket::write(const QByteArray& data,
                              const QHostAddress& host,
                              quint16 port)
{
   Q_ASSERT(!m_socket);
   return m_socket->writeDatagram(data, host, port);
}

void SharedUdpSocket::onDataReceived()
{
   Q_ASSERT(!m_socket);

   QHostAddress host;
   quint16 port;

   while (m_socket->hasPendingDatagrams()) {
      qint64 datagramLength = m_socket->pendingDatagramSize();
      QByteArray buffer(datagramLength, 0);

      qint64 length = m_socket->readDatagram(buffer.data(), buffer.size(),
                                             &host, &port);
      // Discard invalid messages.
      if (length != datagramLength) {
         continue;
      }

      emit dataReceived(buffer, host, port);
   }
}

} // Plugins
