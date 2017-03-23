#include <QTimer>
#include <QSet>

#include "protocols.h"

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
    * \brief Handles state responses.
    * \param stateName State name.
    * \param stateValue State value.
    * \param available Is state available.
    */
   void handleStateResponse(const QString& stateName,
                            const QVariant& stateValue,
                            bool available);

   /*!
    * \brief Handles state change notifications.
    * \param label State label name.
    * \param value State value.
    */
   void handleStateChange(const QString& label,
                          const QVariant& value);

protected slots:
   /*!
    * \brief Handles connected signal from communication plugin.
    * \param connectionId Connection id.
    */
   void connected(qint32 connectionId);

   /*!
    * \brief Handles disconnected signal from communication plugin.
    * \param connectionId Connection id.
    */
   void disconnected(qint32 connectionId);

   /*!
    * \brief Handles data received from device.
    * \param data Received data.
    * \param connectionId Connection id.
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

   /*!
    * \brief Requests values for given states.
    * \param states List of state names.
    */
   void requestStates(QSet<QString> states);

   /*!
    * \brief Sends error message to User interface (WebUI).
    * \param message Error message.
    * \param connectionId Connection id.
    */
   void sendErrorMessage(QString message,
                         int connectionId);

   //! List of users.
   QHash<qint32, User*> m_users;
};

} // Plugins

DEFINE_PROTOCOL_PLUGIN(WebUI)

#endif
