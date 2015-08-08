#include <QTimer>
#include <QSet>

#include "protocol.h"

#ifndef WEBUI_H
#define WEBUI_H

namespace Plugins {

/*!
 * \brief The WebUI class.
 * This class describes WebUI protocol.
 */
class WebUI : public Protocol
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param parameters Unused.
    * \param parent Parent object.
    */
   explicit WebUI(Utils::ParameterSet parameters,
                     QObject* parent = NULL);
   //! Destructor.
   ~WebUI();

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
   struct User
   {
      QByteArray buffer;
      QSet<QString> registeredStates;
      QSet<QString> requestedStates;
   };

   /*!
    * \brief Parses register message.
    * \param message Message payload.
    */
   void parseReg(QString message,
                 User* user);

   /*!
    * \brief Parses get message.
    * \param message Message payload.
    */
   void parseGet(QString message,
                 User* user);

   /*!
    * \brief Parses set message.
    * \param message Message payload.
    */
   void parseSet(QString message);

   /*!
    * \brief Parses error message.
    * \param message Message payload.
    */
   void parseErr(QString message);

   void requestStates(QSet<QString> states);

   void sendErrorMessage(QString message,
                         int connectionId);

   //! List of users
   QHash<qint32, User*> m_users;
};

} // Plugins

#endif
