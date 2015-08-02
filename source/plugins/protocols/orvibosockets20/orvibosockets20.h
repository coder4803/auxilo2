#include <QTimer>
#include <QSet>
#include <QEventLoop>
#include <QMutex>
#include <QWaitCondition>

#include "protocol.h"

#ifndef ORVIBOSOCKETS20_H
#define ORVIBOSOCKETS20_H

namespace Plugins {

/*!
 * \brief The WebUI class.
 * This class describes WebUI protocol.
 */
class OrviboSocketS20 : public Protocol
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param parameters Protocol parameters.
    * \param parent Parent object.
    */
   explicit OrviboSocketS20(Utils::ParameterSet parameters,
                            QObject* parent = NULL);

   //! Destructor.
   ~OrviboSocketS20();

   /*!
    * \brief Empty implementation.
    * \param parameters Unused.
    */
   void handleParameters(const Utils::ParameterSet& parameters);

   /*!
    * \brief Empty implementation.
    * \param stateName Unused.
    * \param stateValue Unused.
    * \param available Unused.
    */
   void handleStateResponse(const QString& stateName,
                            const QVariant& stateValue,
                            bool available);

   /*!
    * \brief Empty implementation.
    * \param name Unused.
    * \param parameters Unused.
    * \return Returns always SIGNAL_NOT_FOUND.
    */
   Utils::SignalAckMessage::Result handleSignal(const QString& name,
                                                const QStringList& parameters);

   /*!
    * \brief Handles device state change (core -> device).
    * \param label Device's state name label.
    * \param value New state value.
    */
   void handleStateChange(const QString& label,
                          const QVariant& value);

   /*!
    * \brief Empty implementation.
    * \param result Unused.
    * \param ackId Unused.
    */
   void handleSignalAck(Utils::SignalAckMessage::Result result,
                        quint32 ackId);

   /*!
    * \brief Empty implementation.
    * \param result Unused.
    * \param ackId Unused.
    */
   void handleSetStateAck(Utils::SetStateAckMessage::Result result,
                          quint32 ackId);

protected slots:
   /*!
    * \brief Subscribes to the device.
    * \param connectionId Unused.
    */
   void connected(qint32 connectionId);

   /*!
    * \brief Empty implementation.
    * \param connectionId Unused.
    */
   void disconnected(qint32 connectionId);

   /*!
    * \brief Handles data received from device.
    * \param data Received data.
    * \param connectionId Unused.
    */
   void dataReceived(QByteArray data,
                     qint32 connectionId);

private slots:
   //! Sends subscribe message to device.
   void onSubscribe();

   //! Triggers device state update.
   void sendStateToDevice();

   //! Resends state to the device.
   void onRetry();

private:
   //! Constant for MAC address parameter name.
   static const QString PARAMETER_MAC;

   //! Constant for core state name Sparameter.
   static const QString PARAMETER_STATE;

   //! Constant for parameter that defines if toggle signal should be sent.
   static const QString PARAMETER_SIGNAL;

   //! Default value for power label name.
   static const QString POWER_LABEL;

   //! Constant for toggle signal name.
   static const QString TOGGLE_SIGNAL_NAME;

   //! Constant for power signal name.
   static const QString POWER_SIGNAL_NAME;

   //! Defines how often subscribe message is sent to the device.
   static const int SUBSCRIBE_INTERVAL;

   //! How often message is tried to send to physical device.
   static const int RETRY_INTERVAL;

   //! How many times message is tried to send.
   static const int RETRY_COUNT;

   //! Constant for message start bytes.
   static const QByteArray START_BYTES;

   //! Constant for mac padding.
   static const QByteArray MAC_PADDING;

   //! Constant for four zeros.
   static const QByteArray ZEROS;

   //! Constant for subscribe command id.
   static const QByteArray SUBSCRIBE_ID;

   //! Constant for power command id.
   static const QByteArray POWER_ID;

   //! Constant for power reply id.
   static const QByteArray POWER_REPLY_ID;

   //! Constant for power on.
   static const QByteArray POWER_ON;

   //! Constant for power off.
   static const QByteArray POWER_OFF;

   //! Device MAC-address.
   QByteArray m_macAddress;

   //! State name
   QString m_stateName;

   //! Is toggle signal sent when device's physical button is pressed.
   bool m_sendToggleSignal;

   //! Timer for keeping up subscription.
   QTimer m_subscribeTimer;

   //! Timer for resending messages;
   QTimer m_resendTimer;

   //! Number of retries left to send state to physical device.
   int m_retriesLeft;

   //! Current state of physical device.
   bool m_deviceState;

   //! Device's target state.
   bool m_targetState;

   /*!
    * \brief Generates payload for power message.
    * \return Power message payload.
    */
   QByteArray generatePowerMessage();

   /*!
    * \brief Generates payload for subscribe message.
    * \return Subscribe message payload.
    */
   QByteArray generateSubscribeMessage();

   /*!
    * \brief Sends control message to device.
    * \param payload Payload of control message.
    * \return True if sending succeeded, otherwise false.
    */
   bool sendControlMessage(const QByteArray& payload);

   /*!
    * \brief Returns reversed version of QByteArray.
    * \param array Byte array to reverse.
    * \return Reversed byte array.
    */
   QByteArray reversed(const QByteArray& array);
};

} // Plugins

#endif
