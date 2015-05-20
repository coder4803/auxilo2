#include <QObject>
#include <QMutex>

#include "statechangedackmessage.h"
#include "setstatemessage.h"
#include "signalackmessage.h"
#include "confresponsemessage.h"
#include "logmessage.h"

#ifndef PROTOCOL_H
#define PROTOCOL_H

namespace Plugins {

class Communication;

/*!
 * \brief The Protocol class.
 * This class describes interface for Protocols.
 */
class Protocol : public QObject
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param parent Parent object.
    */
   explicit Protocol(QObject* parent);

   /*!
    * \brief Destructor.
    */
   virtual ~Protocol() {}

   /*!
    * \brief Sets communication to use by device.
    * \param communication Communication to use.
    */
   void setCommunication(Communication* communication);

   /*!
    * \brief Returns communication used by device.
    * \return Communication used by device.
    */
   Communication* getCommunication();

   /*!
    * \brief Sets last acknowledge id.
    * \param ackId Acknowledge id.
    */
   void setLastAckId(quint32 ackId) { m_lastAckId = ackId; }

   /*!
    * \brief Returns last acknowledge id.
    * This method can be used by protocol to get acknowledge id for sent
    * message. Note that this method has to be called right after sending
    * the signal to get right acknowledge id.
    * \return Last acknowledge id.
    */
   quint32 getLastAckId() const { return m_lastAckId; }

   /*!
    * \brief This method should handle device parameters.
    * Device parameters can be requested with requestDeviceParameters
    * signal.
    * \param parameters Device parameters.
    */
   virtual void handleParameters(const Utils::ParameterSet& parameters) = 0;

   /*!
    * \brief This method should handle state response message.
    * State can be requested with requestStateValue signal.
    * \param stateName Name of state.
    * \param stateValue Value of state.
    * \param available Is value available. State value is invalid if this
    * is false!
    */
   virtual void handleStateResponse(const QString& stateName,
                                    const QVariant& stateValue,
                                    bool available) = 0;

   /*!
    * \brief This method should handle signal message and return result of
    * handling.
    * \param name Name of signal.
    * \param parameters Signal parameters.
    * \return Result of handling signal message.
    */
   virtual Utils::SignalAckMessage::Result
   handleSignal(const QString& name,
                const QStringList& parameters) = 0;

   /*!
    * \brief This method should handle state changed message and return result
    * of handling.
    * \param label Device's label for state name.
    * \param value State value.
    * \return Result of handling state change message.
    */
   virtual Utils::StateChangedAckMessage::Result
   handleStateChange(const QString& label,
                     const QVariant& value) = 0;

   /*!
    * \brief This method should handle signal acknowledge message.
    * This is response for sendSignal signal.
    * \param result Result of signal message handling (handled by signal
    * handler).
    * \param ackId Acknowledge id.
    */
   virtual void handleSignalAck(Utils::SignalAckMessage::Result result,
                                quint32 ackId) = 0;

   /*!
    * \brief This method should handle set state acknowledge message.
    * This is response for setStateValue signal.
    * \param result Result of set state value message handling (handled by
    * state holder).
    * \param ackId Acknowledge id.
    */
   virtual void handleSetStateAck(Utils::SetStateAckMessage::Result result,
                                  quint32 ackId) = 0;

protected:
   /*!
    * \brief Checks if connection is established.
    * \param connectionId Inner connection id. This is used with connections
    * that can have multiple connections. This is common with server
    * connections. Use >= 0. -1 means undefined (or only one connection).
    * \return True if connection is established, otherwise false.
    */
   bool isConnected(qint32 connectionId = -1) const;

   /*!
    * \brief Tells connection to trigger reading.
    * \param connectionId Inner connection id. This is used with connections
    * that can have multiple connections. This is common with server
    * connections. Use >= 0. -1 means undefined (or only one connection).
    */
   void readData(qint32 connectionId = -1);

   /*!
    * \brief Sends data through connection.
    * \param data Data to send.
    * \param connectionId Inner connection id. This is used with connections
    * that can have multiple connections. This is common with server
    * connections. Use >= 0. -1 means undefined (or only one connection).
    * \return True if sending succeeded, otherwise false.
    */
   bool sendData(const QByteArray& data,
                 qint32 connectionId = -1);

protected slots:
   /*!
    * \brief This slot should handle connected-signal from communication.
    * \param connectionId Inner connection id. This is used with connections
    * that can have multiple connections. This is common with server
    * connections. Use >= 0. -1 means undefined (or only one connection).
    */
   virtual void connected(qint32 connectionId) = 0;

   /*!
    * \brief This slot should handle disconnected-signal from communication.
    * \param connectionId Inner connection id. This is used with connections
    * that can have multiple connections. This is common with server
    * connections. Use >= 0. -1 means undefined (or only one connection).
    */
   virtual void disconnected(qint32 connectionId) = 0;

   /*!
    * \brief This slot should handle data received from communication.
    * \param data Received data.
    * \param connectionId Inner connection id. This is used with connections
    * that can have multiple connections. This is common with server
    * connections. Use >= 0. -1 means undefined (or only one connection).
    */
   virtual void dataReceived(const QByteArray& data,
                             qint32 connectionId) = 0;

signals:
   /*!
    * \brief This signal is used to request device parameters.
    * \param deviceName Name of device.
    */
   void requestDeviceParameters(QString deviceName);

   /*!
    * \brief This signal is used to request state value from state holder.
    * \param stateName Name of state to request.
    */
   void requestStateValue(QString stateName);

   /*!
    * \brief This signal is used to set state value to state holder.
    * \param stateName Name of state.
    * \param value Value of state.
    * \param ackRequired Should state holder send acknowledge message or not.
    */
   void setStateValue(QString stateName,
                      QVariant value,
                      bool ackRequired);

   /*!
    * \brief This signal is used to send signal to signal handler.
    * \param name Name of signal.
    * \param parameters Signal parameters
    * \param ackRequired Should signal handler send acknowledge message or not.
    */
   void sendSignal(QString name,
                   QStringList parameters,
                   bool ackRequired = false);

   /*!
    * \brief This signal is used to send log message.
    * \param message Log message.
    * \param type Log type.
    */
   void sendLog(QString message,
                Utils::LogMessage::LogType type);

private:
   //! Mutex to secure handling of m_communication.
   QMutex m_mutex;

   //! Pointer to communication.
   Communication* m_communication;

   //! Acknowledge id of last sent message that requires acknowledge.
   quint32 m_lastAckId;
};

} // Plugins

#endif // PROTOCOL_H
