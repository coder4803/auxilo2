#include "lesensor.h"

namespace Plugins {

LESensor::LESensor(Utils::ParameterSet parameters,
                   QObject* parent) :
   Protocol(parent)
{
   Q_UNUSED(parameters)
}

LESensor::~LESensor()
{
}

void LESensor::handleParameters(const Utils::ParameterSet& parameters)
{
   Q_UNUSED(parameters)
}

void LESensor::handleStateResponse(const QString& stateName,
                                   const QVariant& stateValue,
                                   bool available)
{
   Q_UNUSED(stateName)
   Q_UNUSED(stateValue)
   Q_UNUSED(available)
}

Utils::SignalAckMessage::Result
LESensor::handleSignal(const QString& name,
                       const QStringList& parameters)
{
   Q_UNUSED(name)
   Q_UNUSED(parameters)
   return Utils::SignalAckMessage::SIGNAL_NOT_FOUND;
}

void LESensor::handleStateChange(const QString& label,
                                 const QVariant& value)
{
   Q_UNUSED(label)
   Q_UNUSED(value)
   emit acknowledgeStateChange(Utils::StateChangedAckMessage::INVALID_LABEL,
                               value);
}

void LESensor::handleSignalAck(Utils::SignalAckMessage::Result result,
                               quint32 ackId)
{
   Q_UNUSED(result)
   Q_UNUSED(ackId)
}

void LESensor::handleSetStateAck(Utils::SetStateAckMessage::Result result,
                                 quint32 ackId)
{
   Q_UNUSED(result)
   Q_UNUSED(ackId)
}

void LESensor::connected(qint32 connectionId)
{
   Q_UNUSED(connectionId)
}

void LESensor::disconnected(qint32 connectionId)
{
   Q_UNUSED(connectionId)
}

void LESensor::dataReceived(QByteArray data,
                            qint32 connectionId)
{
   Q_UNUSED(connectionId)

   m_buffer += data;

   while (!m_buffer.isEmpty()) {
      // Find start of message.
      int start = m_buffer.indexOf("$");
      if (start == -1) {
         m_buffer.clear();
         return;
      }

      // Find end of message.
      int end = m_buffer.indexOf("\n", start);
      if (end == -1) {
         return;
      }

      // Get message payload (data between start and end characters).
      QByteArray message = m_buffer.mid(start + 1, end - start - 1);
      m_buffer = m_buffer.mid(end + 1);

      // Parse message.
      if (message.startsWith("te=")) {
         parseTemperature(message);
      } else if (message.startsWith("br=")) {
         parseBrightness(message);
      } else if (message.startsWith("mo=")) {
         parseMotion(message);
      } else if (message.startsWith("in=")) {
         parseEnter(message);
      } else if (message.startsWith("ou=")) {
         parseLeave(message);
      }
   }
}

void LESensor::parseTemperature(QString message)
{
   bool ok = false;

   // Parse temperature.
   QString temperatureString = message.mid(3);
   double temperature = temperatureString.toDouble(&ok);
   if (!ok) {
      qCritical("Failed to parse temperature value %s",
                temperatureString.toLatin1().data());
      return;
   }

   temperatureString = QString::number(temperature, 'g', 3);

   // Send signal.
   sendSignal("temperature", QStringList() << temperatureString);
}

void LESensor::parseBrightness(QString message)
{
   bool ok = false;

   // Parse temperature.
   QString brightnessString = message.mid(3);
   int brightness = brightnessString.toInt(&ok);
   if (!ok) {
      qCritical("Failed to parse brightness value %s",
                brightnessString.toLatin1().data());
      return;
   }

   brightnessString = QString::number(brightness);

   // Send signal
   sendSignal("brightness", QStringList() << brightnessString);
}

void LESensor::parseMotion(QString message)
{
   bool ok = false;

   // Parse motion value
   QString motionString = message.mid(3);
   int motionInteger = QVariant(motionString).toInt(&ok);
   if (!ok) {
      qCritical("Failed to parse motion value %s",
                motionString.toLatin1().data());
      return;
   }

   bool motion = QVariant(motionInteger).toBool();
   motionString = motion ? "true" : "false";

   // Send signal
   sendSignal("motion", QStringList() << motionString);
}

void LESensor::parseEnter(QString message)
{
   QString macAddress = message.mid(3);
   sendSignal("enter", QStringList() << macAddress);
}

void LESensor::parseLeave(QString message)
{
   QString macAddress = message.mid(3);
   sendSignal("leave", QStringList() << macAddress);
}

} // Plugins
