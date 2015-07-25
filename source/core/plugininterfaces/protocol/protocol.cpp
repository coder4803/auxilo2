#include "protocol.h"
#include "communication.h"

namespace Plugins {

Protocol::Protocol(QObject* parent) :
   QObject(parent),
   m_communication(NULL),
   m_lastAckId(0)
{}

void Protocol::setCommunication(Communication* communication)
{
   QMutexLocker locker(&m_mutex);
   m_communication = communication;

   // Connect signals from communication to this protocol.S
   connect(m_communication, SIGNAL(connected(qint32)),
           this, SLOT(connected(qint32)));
   connect(m_communication, SIGNAL(disconnected(qint32)),
           this, SLOT(disconnected(qint32)));
   connect(m_communication, SIGNAL(dataReceived(QByteArray,qint32)),
           this, SLOT(dataReceived(QByteArray,qint32)));
}

bool Protocol::isConnected(qint32 connectionId) const
{
   if (m_communication) {
      m_communication->isConnected(connectionId);
   }

   return false;
}

void Protocol::readData(qint32 connectionId)
{
   QMutexLocker locker(&m_mutex);
   if (m_communication) {
      m_communication->readData(connectionId);
   }
}

bool Protocol::sendData(const QByteArray& data,
                        qint32 connectionId)
{
   QMutexLocker locker(&m_mutex);
   if (m_communication) {
      return m_communication->sendData(data, connectionId);
   }

   return false;
}

} // Plugins
