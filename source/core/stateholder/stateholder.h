#include <QObject>
#include <QHash>
#include <QString>

#include <messagegroup.h>

#include "states/state.h"

#ifndef STATEHOLDER_H
#define STATEHOLDER_H

namespace Core {

/*!
 * \brief The StateHolder class.
 * This is main class of state holder. It handles incoming messages on high
 * level and triggers state updates.
 */
class StateHolder : public QObject
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param serverAddress RabbitMQ server address.
    * \param parent Parent object
    */
   StateHolder(QString serverAddress,
               QObject* parent = NULL);

   //! Destructor.
   ~StateHolder();

private slots:
   /*!
    * \brief Handles configuration response messages.
    * \param payload Message payload.
    */
   void handleConfResponseMessage(QByteArray payload);

   /*!
    * \brief Handles set state messages.
    * \param payload Message payload.
    */
   void handleSetStateMessage(QByteArray payload);

   /*!
    * \brief Handles state request messages.
    * \param payload Message payload.
    */
   void handleRequestStateMessage(QByteArray payload);

   /*!
    * \brief Handles state changed ack messages.
    * \param payload Message payload.
    */
   void handleStateChangedAckMessage(QByteArray payload);

   /*!
    * \brief Sends configuration request to configuraion manager.
    */
   void requestParameters();

   /*!
    * \brief Goes through all states and executes all necessary updates.
    */
   void updateStates();

private:
   //! Configuration file path parameter name.
   static const QString PARAMETER_CONFPATH;

   //! Default configuration file path.
   static const QString DEFAULT_CONF_FILE;

   //! Defines how often to check if device states are up-to-date.
   static const quint32 STATE_CHECK_INTERVAL;

   //! Defines how often configurations are requested until received.
   static const quint32 CONF_REQUEST_INTERVAL;

   //! Feature name (for requesting configuarions).
   static const QString FEATURE_NAME;

   //! Data base connection name.
   static const QString DATABASE_CONNECTION_NAME;

   //! Timer for requesting parameters.
   QTimer m_confRequestTimer;

   //! State configuration path.
   QString m_configPath;

   // Listened configuration groups.
   Utils::MessageGroup* m_confResponseGroup;
   Utils::MessageGroup* m_setStateGroup;
   Utils::MessageGroup* m_stateRequestGroup;
   Utils::MessageGroup* m_stateChangedAckGroup;

   // Container for states.
   QHash<QString, State*> m_states;

   // State update timer.
   QTimer m_updateTimer;

   /*!
    * \brief Loads state configurations from configuration file.
    * \return True on success, false on failure.
    */
   bool loadStateConfigurations();

   /*!
    * \brief Opens database for persistent state values.
    * \return True on success, false on failure.
    */
   bool openPersistentDatabase();

   /*!
    * \brief Initializes state holder.
    * \return True on success, false on failure.
    */
   bool initStateHolder();
};

} // Core

#endif
