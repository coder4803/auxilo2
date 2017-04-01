#include "device.h"

#include "confrequestmessage.h"
#include "requeststatemessage.h"
#include "setstatemessage.h"
#include "signalmessage.h"
#include "statechangedmessage.h"
#include "logmessage.h"

Device::Device(QString name,
               Plugins::CommunicationInterface* communicationPlugin,
               const Utils::ParameterSet& communicationParameters,
               Plugins::ProtocolInterface* protocolPlugin,
               const Utils::ParameterSet& protocolParameters) :
   m_communicationPlugin(communicationPlugin),
   m_protocolPlugin(protocolPlugin),
   m_communicationParameters(communicationParameters),
   m_protocolParameters(protocolParameters),
   m_name(name),
   m_communication(NULL),
   m_protocol(NULL),
   m_lastStateChangedMessage(QString(), QVariant(), QString()),
   m_confResponseGroup(NULL),
   m_stateResponseGroup(NULL),
   m_signalGroup(NULL),
   m_stateChangedGroup(NULL),
   m_signalAckGroup(NULL),
   m_setStateAckGroup(NULL)
{
   initMessageGroups();

   // Start thread.
   moveToThread(&m_thread);
   m_thread.start();
}

Device::~Device()
{
}

bool Device::start()
{
   if (!createCommunication()) {
      return false;
   }

   if (!createProtocol()) {
      return false;
   }

   return true;
}

void Device::stop(quint32 timeout)
{
   m_thread.quit();
   m_thread.wait(timeout);
}

void Device::handleConfResponseMessage(QByteArray payload)
{
   Utils::ConfResponseMessage response(payload);

   Utils::ParameterSet parameters = response.parameterSet();
   m_protocol->handleParameters(parameters);
}

void Device::handleStateResponseMessage(QByteArray payload)
{
   Utils::StateResponseMessage response(payload);

   QStringList stateNames = response.states().keys();
   foreach (QString name, stateNames) {
      Utils::StateResponseMessage::State state = response.state(name);
      m_protocol->handleStateResponse(name, state.value, state.available);
   }
}

void Device::handleSignalMessage(QByteArray payload)
{
   Utils::SignalMessage message(payload);

   QString name = message.signalName();
   QStringList parameters = message.parameters();

   Utils::SignalAckMessage::Result result =
         m_protocol->handleSignal(name, parameters);

   Utils::SignalAckMessage ack = message.createAckMessage(result);
   emit publish(ack.data(), message.ackGroup());
}

void Device::handleStateChangedMessage(QByteArray payload)
{
   Utils::StateChangedMessage message(payload);

   QString label = message.label();
   QVariant value = message.value();

   m_lastStateChangedMessage = message;

   m_protocol->handleStateChange(label, value);
}

void Device::handleSignalAckMessage(QByteArray payload)
{
   Utils::SignalAckMessage message(payload);
   m_protocol->handleSignalAck(message.result(), message.ackId());
}

void Device::handleSetStateAckMessage(QByteArray payload)
{
   Utils::SetStateAckMessage message(payload);
   m_protocol->handleSetStateAck(message.result(), message.ackId());
}

void Device::requestDeviceParameters()
{
   QString responseGroupName = m_name + Utils::CONF_RESPONSE_POST_FIX;
   Utils::ConfRequestMessage request(responseGroupName, m_name, false);
   emit publish(request.data(), Utils::CONF_REQUEST_GROUP);
}

void Device::requestStateValue(QString stateName)
{
   QString responseGroupName = m_name + Utils::STATE_RESPONSE_POST_FIX;
   Utils::RequestStateMessage request(responseGroupName, stateName);
   emit publish(request.data(), Utils::REQUEST_STATE_GROUP);
}

void Device::setStateValue(QString stateName,
                           QVariant value,
                           bool ackRequired)
{
   QString responseGroupName;
   if (ackRequired) {
      responseGroupName = m_name + Utils::SET_STATE_ACK_POST_FIX;
   }

   Utils::SetStateMessage message(stateName, value, responseGroupName);
   emit publish(message.data(), Utils::SET_STATE_GROUP);

   // "Return" acknowledge id for protocol if required.
   if (ackRequired) {
      Plugins::Protocol* protocol = dynamic_cast<Plugins::Protocol*>(sender());
      protocol->setLastAckId(message.ackId());
   }
}

void Device::sendSignal(QString name,
                        QStringList parameters,
                        bool ackRequired)
{
   QString ackGroupName;
   if (ackRequired) {
      ackGroupName = m_name + Utils::SIGNAL_ACK_POST_FIX;
   }

   Utils::SignalMessage message(name, m_name, parameters, ackGroupName);
   emit publish(message.data(), Utils::SIGNAL_HANDLER_GROUP);

   // "Return" acknowledge id for protocol if required.
   if (ackRequired) {
      Plugins::Protocol* protocol = dynamic_cast<Plugins::Protocol*>(sender());
      protocol->setLastAckId(message.ackId());
   }
}

void Device::acknowledgeStateChange(Utils::StateChangedAckMessage::Result result,
                                    QVariant stateValue)
{
   if (!m_lastStateChangedMessage.ackGroup().isEmpty()) {
      if (result == Utils::StateChangedAckMessage::SUCCEEDED &&
          stateValue != m_lastStateChangedMessage.value())
      {
         result = Utils::StateChangedAckMessage::FAILED;
      }

      Utils::StateChangedAckMessage ackMessage =
            m_lastStateChangedMessage.createAckMessage(result);

      emit publish(ackMessage.data(), m_lastStateChangedMessage.ackGroup());

      m_lastStateChangedMessage =
            Utils::StateChangedMessage(QString(), QVariant(), QString());
   }
}

void Device::sendLog(QString message,
                     Utils::LogMessage::LogType type)
{
   QString senderName = "DEVICE_" + m_name;
   Utils::LogMessage log(message, type, senderName);
   emit publish(log.data(), Utils::LOG_MESSAGE_GROUP);
}

bool Device::createCommunication()
{
   // Do not create communication plugin if not specified.
   if (!m_communicationPlugin) {
       qDebug("No communication specified for %s", m_name.toLatin1().data());
       return true;
   }

   m_communication = m_communicationPlugin->create(m_communicationParameters,
                                                   this);
   if (!m_communication) {
      qCritical("Failed to create communication for %s",
                m_name.toLatin1().data());
      return false;
   }

   return true;
}

bool Device::createProtocol()
{
   m_protocol = m_protocolPlugin->create(m_protocolParameters, this);
   if (!m_protocol) {
      qCritical("Failed to create protocol for %s",
                m_name.toLatin1().data());
      return false;
   }

   m_protocol->setCommunication(m_communication);

   // Handle signals emitted by protocol.
   connect(m_protocol, SIGNAL(requestDeviceParameters()),
           this, SLOT(requestDeviceParameters()));
   connect(m_protocol, SIGNAL(setStateValue(QString,QVariant,bool)),
           this, SLOT(setStateValue(QString,QVariant,bool)));
   connect(m_protocol, SIGNAL(requestStateValue(QString)),
           this, SLOT(requestStateValue(QString)));
   connect(m_protocol, SIGNAL(sendSignal(QString,QStringList,bool)),
           this, SLOT(sendSignal(QString,QStringList,bool)));
   connect(m_protocol, SIGNAL(acknowledgeStateChange(
           Utils::StateChangedAckMessage::Result,QVariant)),
           this, SLOT(acknowledgeStateChange(
           Utils::StateChangedAckMessage::Result,QVariant)));
   connect(m_protocol, SIGNAL(sendLog(QString,Utils::LogMessage::LogType)),
           this, SLOT(sendLog(QString,Utils::LogMessage::LogType)));

   return true;
}

void Device::initMessageGroups()
{
   // Listen for configuration response messages.
   m_confResponseGroup = new Utils::MessageGroup(m_name +
                                                 Utils::CONF_RESPONSE_POST_FIX,
                                                 Utils::MessageGroup::Subscriber,
                                                 this);
   connect(m_confResponseGroup, SIGNAL(messageReceived(QByteArray,QString)),
           this, SLOT(handleConfResponseMessage(QByteArray)));

   // Listen for state response messages.
   m_stateResponseGroup = new Utils::MessageGroup(m_name +
                                                  Utils::STATE_RESPONSE_POST_FIX,
                                                  Utils::MessageGroup::Subscriber,
                                                  this);
   connect(m_stateResponseGroup, SIGNAL(messageReceived(QByteArray,QString)),
           this, SLOT(handleStateResponseMessage(QByteArray)));

   // Listen for signal messages.
   m_signalGroup = new Utils::MessageGroup(m_name + Utils::SIGNAL_POST_FIX,
                                           Utils::MessageGroup::Subscriber,
                                           this);
   connect(m_signalGroup, SIGNAL(messageReceived(QByteArray,QString)),
           this, SLOT(handleSignalMessage(QByteArray)));

   // Listen for state changed messages.
   m_stateChangedGroup = new Utils::MessageGroup(m_name +
                                                 Utils::STATE_CHANGED_POST_FIX,
                                                 Utils::MessageGroup::Subscriber,
                                                 this);
   connect(m_stateChangedGroup, SIGNAL(messageReceived(QByteArray,QString)),
           this, SLOT(handleStateChangedMessage(QByteArray)));

   // Listen for signal ack message.
   m_signalAckGroup = new Utils::MessageGroup(m_name +
                                              Utils::SIGNAL_ACK_POST_FIX,
                                              Utils::MessageGroup::Subscriber,
                                              this);
   connect(m_signalAckGroup, SIGNAL(messageReceived(QByteArray,QString)),
           this, SLOT(handleSignalAckMessage(QByteArray)));

   // Listen for set state ack message.
   m_setStateAckGroup = new Utils::MessageGroup(m_name +
                                                Utils::SET_STATE_ACK_POST_FIX,
                                                Utils::MessageGroup::Subscriber,
                                                this);
   connect(m_signalAckGroup, SIGNAL(messageReceived(QByteArray,QString)),
           this, SLOT(handleSetStateAckMessage(QByteArray)));
}
