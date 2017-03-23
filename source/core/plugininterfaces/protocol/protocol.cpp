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

void Protocol::setLastAckId(quint32 ackId)
{
   m_lastAckId = ackId;
}

quint32 Protocol::getLastAckId() const
{
   return m_lastAckId;
}

void Protocol::handleParameters(const Utils::ParameterSet& parameters)
{
   Q_UNUSED(parameters);
}

void Protocol::handleStateResponse(const QString& stateName,
                                   const QVariant& stateValue,
                                   bool available)
{
   Q_UNUSED(stateName);
   Q_UNUSED(stateValue);
   Q_UNUSED(available);
}

Utils::SignalAckMessage::Result
Protocol::handleSignal(const QString& name,
                       const QStringList& parameters)
{
   Q_UNUSED(name);
   Q_UNUSED(parameters);

   return Utils::SignalAckMessage::SIGNAL_NOT_FOUND;
}

void Protocol::handleStateChange(const QString& label,
                                 const QVariant& value)
{
   Q_UNUSED(label);
   Q_UNUSED(value);
}

void Protocol::handleSignalAck(Utils::SignalAckMessage::Result result,
                               quint32 ackId)
{
   Q_UNUSED(result);
   Q_UNUSED(ackId);
}

void Protocol::handleSetStateAck(Utils::SetStateAckMessage::Result result,
                                 quint32 ackId)
{
   Q_UNUSED(result);
   Q_UNUSED(ackId);
}

void Protocol::connected(qint32 connectionId)
{
   Q_UNUSED(connectionId);
}

void Protocol::disconnected(qint32 connectionId)
{
   Q_UNUSED(connectionId);
}

void Protocol::dataReceived(QByteArray data,
                            qint32 connectionId)
{
   Q_UNUSED(data);
   Q_UNUSED(connectionId);
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
