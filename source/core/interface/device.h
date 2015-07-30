#include <QObject>
#include <QThread>

#include "plugininterfaces.h"

#include "messagegroup.h"
#include "confresponsemessage.h"
#include "stateresponsemessage.h"
#include "signalmessage.h"
#include "statechangedmessage.h"
#include "protocol.h"

#ifndef DEVICE_H
#define DEVICE_H

/*!
 * \brief The Device class
 * Each device of this class represents one external (abstract) device.
 */
class Device : public QObject
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param name Name of device.
    * \param communicationPlugin Pointer to communication plugin to use.
    * \param communicationParameters Communication configuration.
    * \param protocolPlugin Pointer to protocol plugin to use.
    * \param protocolParameters Protocol configuration.
    */
   explicit Device(QString name,
                   Plugins::CommunicationInterface* communicationPlugin,
                   const Utils::ParameterSet& communicationParameters,
                   Plugins::ProtocolInterface* protocolPlugin,
                   const Utils::ParameterSet& protocolParameters);

   //! Destructor.
   ~Device();

   /*!
    * \brief Executes initializations that has to be done in device thread.
    * \return True if initializations succeeded, otherwise false.
    */
   Q_INVOKABLE bool start();

   /*!
    * \brief Stops thread and wait for exit.
    * \param timeout Maximum tim to wait for thread to exit.
    */
   void stop(quint32 timeout = 3000);

signals:
   /*!
    * \brief Signal to publish message to specific group.
    * \param message Payload of message.
    * \param group Name fo group.
    */
   void publish(const QByteArray& message, QString group);

private slots:
   /* ######################
    * ### Core -> Device ###
    * ######################
    */

   /*!
    * \brief Handles configuration response message.
    * \param payload Payload of configuration response message.
    */
   void handleConfResponseMessage(QByteArray payload);

   /*!
    * \brief Handles state response message.
    * \param payload Payload of state response message.
    */
   void handleStateResponseMessage(QByteArray payload);

   /*!
    * \brief Handles signal message.
    * \param payload Payload of signal message.
    */
   void handleSignalMessage(QByteArray payload);

   /*!
    * \brief Handles state changed message.
    * \param payload Payload of state changed message.
    */
   void handleStateChangedMessage(QByteArray payload);

   /*!
    * \brief Handles signal ack message.
    * \param payload Payload of signal ack message.
    */
   void handleSignalAckMessage(QByteArray payload);

   /*!
    * \brief Handles set state ack message.
    * \param payload Payload of set state ack message.
    */
   void handleSetStateAckMessage(QByteArray payload);

   /* ######################
    * ### Device -> Core ###
    * ######################
    */

   /*!
    * \brief Sends request for device parameters.
    */
   void requestDeviceParameters();

   /*!
    * \brief Sends request for a state value.
    * \param stateName Name of state to request.
    */
   void requestStateValue(QString stateName);

   /*!
    * \brief Sends set state value message.
    * \param stateName Name of state.
    * \param value Value of state.
    * \param ackRequired Should receiver send acknowledge message.
    */
   void setStateValue(QString stateName,
                      QVariant value,
                      bool ackRequired);

   /*!
    * \brief Sends signal message.
    * \param name Name of signal.
    * \param parameters Signal parameters.
    * \param ackRequired Should receiver send acknowledge message.
    */
   void sendSignal(QString name,
                   QStringList parameters,
                   bool ackRequired = false);

   /*!
    * \brief Sends log message.
    * \param message Log message (payload).
    * \param type Type of log message.
    */
   void sendLog(QString message,
                Utils::LogMessage::LogType type);

private:
   /*!
    * \brief Creates communication based on parameters given in constructor.
    * \return True if creation succeeded, otherwise false.
    */
   bool createCommunication();

   /*!
    * \brief Creates protocol based on parameters given in constructor.
    * \return True if creation succeeded, otherwise false.
    */
   bool createProtocol();

   /*!
    * \brief Starts listening for bunch of message groups.
    */
   void initMessageGroups();

   //! Device thread.
   QThread m_thread;

   //! Pointer to communication plugin.
   Plugins::CommunicationInterface* m_communicationPlugin;

   //! Pointer to protocol plugin.
   Plugins::ProtocolInterface* m_protocolPlugin;

   //! Communication configuration.
   Utils::ParameterSet m_communicationParameters;

   //! Protocol configuration.
   Utils::ParameterSet m_protocolParameters;

   //! Name of device.
   QString m_name;

   //! Communication used by this device.
   Plugins::Communication* m_communication;

   //! Protocol used by this device.
   Plugins::Protocol* m_protocol;

   // Message groups.
   Utils::MessageGroup* m_confResponseGroup;
   Utils::MessageGroup* m_stateResponseGroup;
   Utils::MessageGroup* m_signalGroup;
   Utils::MessageGroup* m_stateChangedGroup;
   Utils::MessageGroup* m_signalAckGroup;
   Utils::MessageGroup* m_setStateAckGroup;
};

#endif
