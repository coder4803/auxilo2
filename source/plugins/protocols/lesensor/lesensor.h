#include <QTimer>

#include "protocols.h"

#ifndef LESENSOR_H
#define LESENSOR_H

namespace Plugins {

/*!
 * \brief The LESensor class.
 * This class describes LESensor protocol.
 */
class LESensor : public Protocol
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param parameters Unused.
    * \param parent Parent object.
    */
   explicit LESensor(Utils::ParameterSet parameters,
                     QObject* parent = NULL);
   //! Destructor.
   ~LESensor();

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
    * \return Always SIGNAL_NOT_FOUND.
    */
   Utils::SignalAckMessage::Result handleSignal(const QString& name,
                                                const QStringList& parameters);

   /*!
    * \brief Empty implementation.
    * \param label Unused.
    * \param value Unused.
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
    * \brief Empty implementation.
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

private:
   /*!
    * \brief Parses temperature message.
    * \param message Message payload.
    */
   void parseTemperature(QString message);

   /*!
    * \brief Parses brightness message.
    * \param message Message payload.
    */
   void parseBrightness(QString message);

   /*!
    * \brief Parses motion message.
    * \param message Message payload.
    */
   void parseMotion(QString message);

   /*!
    * \brief Parses enter message.
    * \param message Message payload.
    */
   void parseEnter(QString message);

   /*!
    * \brief Parses leave message.
    * \param message Message payload.
    */
   void parseLeave(QString message);

   //! Buffer for received data.
   QByteArray m_buffer;
};

} // Plugins

DEFINE_PROTOCOL_PLUGIN(LESensor)

#endif
