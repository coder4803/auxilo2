#include <algorithm>
#include <QDataStream>

#include <QCoreApplication>
#include <QElapsedTimer>

#include "orvibosockets20.h"

namespace Plugins {

const QString OrviboSocketS20::PARAMETER_MAC("mac");
const QString OrviboSocketS20::PARAMETER_STATE("stateName");
const QString OrviboSocketS20::PARAMETER_SIGNAL("sendToggleSignal");

const QString OrviboSocketS20::POWER_LABEL("state");
const QString OrviboSocketS20::TOGGLE_SIGNAL_NAME("toggled");
const QString OrviboSocketS20::POWER_SIGNAL_NAME("power");

const int OrviboSocketS20::SUBSCRIBE_INTERVAL(10 * 1000); // 10 seconds
const int OrviboSocketS20::RETRY_INTERVAL(200); // 200 ms
const int OrviboSocketS20::RETRY_COUNT(5);

const QByteArray OrviboSocketS20::START_BYTES = QByteArray::fromHex("6864");
const QByteArray OrviboSocketS20::MAC_PADDING = QByteArray::fromHex("202020202020");
const QByteArray OrviboSocketS20::ZEROS = QByteArray::fromHex("00000000");

const QByteArray OrviboSocketS20::SUBSCRIBE_ID = QByteArray::fromHex("636c");
const QByteArray OrviboSocketS20::POWER_ID = QByteArray::fromHex("6463");
const QByteArray OrviboSocketS20::POWER_REPLY_ID = QByteArray::fromHex("7366");

const QByteArray OrviboSocketS20::POWER_ON = QByteArray::fromHex("01");
const QByteArray OrviboSocketS20::POWER_OFF = QByteArray::fromHex("00");

OrviboSocketS20::OrviboSocketS20(Utils::ParameterSet parameters,
                   QObject* parent) :
   Protocol(parent),
   m_retriesLeft(0),
   m_deviceState(false),
   m_targetState(false)
{
   try {
      // MAC address.
      QString mac = parameters.parameter<QString>(PARAMETER_MAC);
      m_macAddress = QByteArray::fromHex(QByteArray(mac.toLatin1().data()));

      // State name.
      m_stateName = parameters.parameter<QString>(PARAMETER_STATE, "");

      // Send toggle signal
      m_sendToggleSignal = parameters.parameter<bool>(PARAMETER_SIGNAL, false);

      qCritical("MAC address: %s", m_macAddress.toHex().data());
      qCritical("State name: %s", m_stateName.toLatin1().data());
      qCritical("Send toggle signal: %i", m_sendToggleSignal);
   } catch (QException& e) {
      qCritical("Failed to initialize protocol.");
      throw e;
   }

   m_subscribeTimer.start(SUBSCRIBE_INTERVAL);
   connect(&m_subscribeTimer, SIGNAL(timeout()),
           this, SLOT(onSubscribe()));

   connect(&m_resendTimer, SIGNAL(timeout()),
           this, SLOT(onRetry()));
}

OrviboSocketS20::~OrviboSocketS20()
{
}

void OrviboSocketS20::handleParameters(const Utils::ParameterSet& parameters)
{
   Q_UNUSED(parameters)
}

void OrviboSocketS20::handleStateResponse(const QString& stateName,
                                          const QVariant& stateValue,
                                          bool available)
{
   Q_UNUSED(stateName)
   Q_UNUSED(stateValue)
   Q_UNUSED(available)
}

Utils::SignalAckMessage::Result
OrviboSocketS20::handleSignal(const QString& name,
                              const QStringList& parameters)
{
   Q_UNUSED(name)
   Q_UNUSED(parameters)

   return Utils::SignalAckMessage::SIGNAL_NOT_FOUND;
}

void OrviboSocketS20::handleStateChange(const QString& label,
                                        const QVariant& value)
{
   if (label.toLower() == POWER_LABEL) {
      if (value.type() == QVariant::Bool) {
         m_targetState = value.toBool();

         if (m_deviceState != m_targetState) {
            sendStateToDevice();
         } else {
            emit acknowledgeStateChange(Utils::StateChangedAckMessage::SUCCEEDED,
                                        m_deviceState);
         }

         return;
      }

      emit acknowledgeStateChange(Utils::StateChangedAckMessage::INVALID_VALUE,
                                  QVariant());
   }

   emit acknowledgeStateChange(Utils::StateChangedAckMessage::INVALID_LABEL,
                               QVariant());
}

void OrviboSocketS20::handleSignalAck(Utils::SignalAckMessage::Result result,
                                      quint32 ackId)
{
   Q_UNUSED(result)
   Q_UNUSED(ackId)
}

void OrviboSocketS20::handleSetStateAck(Utils::SetStateAckMessage::Result result,
                                        quint32 ackId)
{
   Q_UNUSED(result)
   Q_UNUSED(ackId)
}

void OrviboSocketS20::connected(qint32 connectionId)
{
   Q_UNUSED(connectionId)
   onSubscribe();
}

void OrviboSocketS20::disconnected(qint32 connectionId)
{
   Q_UNUSED(connectionId)
}

void OrviboSocketS20::dataReceived(QByteArray data,
                                   qint32 connectionId)
{
   Q_UNUSED(connectionId)

   // Check message start bytes.
   if (data.mid(0, 2) != START_BYTES) {
      return;
   }

   // Check MAC address.
   if (data.mid(6, 6) != m_macAddress) {
      return;
   }

   // Command type.
   QByteArray commandType = data.mid(4, 2);

   if (commandType == POWER_REPLY_ID) {
      m_deviceState = (data.at(22) == 1) ? true : false;

      if (m_deviceState == m_targetState && m_resendTimer.isActive()) {
         m_resendTimer.stop();
         emit acknowledgeStateChange(Utils::StateChangedAckMessage::SUCCEEDED,
                                     m_deviceState);
         return;
      }
   } else if (commandType == SUBSCRIBE_ID) {
      m_deviceState = (data.at(23) == 1) ? true : false;

      if (!m_resendTimer.isActive()) {
         sendStateToDevice();
         return;
      }
   }

   // If device state was changed from physical device.
   if (!m_resendTimer.isActive() && m_deviceState != m_targetState) {
      // Update target state according to device state.
      m_targetState = m_deviceState;

      // Send signal to core. New state is delivered as parameter.
      if (m_sendToggleSignal) {
         QString state = QVariant(m_deviceState).toString();
         emit sendSignal(TOGGLE_SIGNAL_NAME, QStringList() << state);
      }

      // Send state change.
      if (!m_stateName.isEmpty()) {
         emit setStateValue(m_stateName, m_deviceState, false);
      }
   }
}

void OrviboSocketS20::onSubscribe()
{
   sendControlMessage(generateSubscribeMessage());
}

void OrviboSocketS20::onRetry()
{
   --m_retriesLeft;
   if (m_retriesLeft > 0) {
      sendControlMessage(generatePowerMessage());
   } else {
      m_resendTimer.stop();
   }
}

QByteArray OrviboSocketS20::generatePowerMessage()
{
   QByteArray payload;

   payload += POWER_ID;
   payload += m_macAddress;
   payload += MAC_PADDING;
   payload += ZEROS;

   if (m_targetState) {
      payload += POWER_ON;
   } else {
      payload += POWER_OFF;
   }

   return payload;
}

QByteArray OrviboSocketS20::generateSubscribeMessage()
{
   QByteArray payload;

   payload += SUBSCRIBE_ID;
   payload += m_macAddress;
   payload += MAC_PADDING;
   payload += reversed(m_macAddress);
   payload += MAC_PADDING;

   return payload;
}

void OrviboSocketS20::sendStateToDevice()
{
   if (!m_resendTimer.isActive()) {
      sendControlMessage(generatePowerMessage());
      m_resendTimer.start(RETRY_INTERVAL);
   }

   m_retriesLeft = RETRY_COUNT;
}

bool OrviboSocketS20::sendControlMessage(const QByteArray& payload)
{
   QByteArray length;
   QDataStream stream(&length, QIODevice::WriteOnly);

   stream.setByteOrder(QDataStream::BigEndian);
   stream << static_cast<quint16>(payload.size() + 4);

   QByteArray data;
   data += START_BYTES;
   data += length;
   data += payload;

   return sendData(data);
}

QByteArray OrviboSocketS20::reversed(const QByteArray& array)
{
   QByteArray reversed(array.size(), 0);
   std::reverse_copy(array.constBegin(), array.constEnd(), reversed.begin());
   return reversed;
}

} // Plugins
