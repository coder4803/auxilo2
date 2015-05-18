#include <QObject>
#include <QList>

#include "device.h"
#include "messagegroup.h"
#include "confresponsemessage.h"
#include "statechangedmessage.h"

#include "plugininterfaces.h"

#ifndef INTERFACE_H
#define INTERFACE_H

namespace Core {

/*!
 * \brief The Interface class
 * Main class of interface application. This class takes care of requesting
 * parameters and loading plugins. It also takes care of initializing
 * devices on high level.
 */
class Interface : public QObject
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param serverAddress RabbitMQ server address
    * \param featureName Feature name for asking parameters from confmanager.
    */
   explicit Interface(QString serverAddress,
                      QString featureName);

   //! Destructor
   ~Interface();

private slots:
   /*!
    * \brief Pulishes message to RabbitMQ system.
    * \param message Message to publish.
    * \param group Name of group the message is published to.
    */
   void onPublish(const QByteArray& message, QString group);

   /*!
    * \brief Sends configuration request to RabbitMQ system.
    */
   void requestParameters();

   /*!
    * \brief Handles configuration response message.
    * \param payload Configuration resonse message payload.
    */
   void handleConfResponseMessage(QByteArray payload);

private:
   /*!
    * \brief Handles loading of plugins on high level.
    * \return True if plugins were loaded successfully, otherwise false.
    */
   bool loadPlugins();

   /*!
    * \brief Loads available communication plugins.
    * \return True if loading succeeded, otherwise false.
    */
   bool loadCommunicationPlugins();

   /*!
    * \brief Loads available protocol plugins.
    * \return True if loading succeeded, otherwise false.
    */
   bool loadProtocolPlugins();

   /*!
    * \brief Unloads all communication and protocol plugins.
    */
   void unloadPlugins();

   /*!
    * \brief Initializes all devices (based on received configurations).
    * \param parameters Configuration parameters for creating devices.
    * \return True if initialization succeeded, otherwise false.
    */
   bool initDevices(const Utils::ParameterSet& parameters);

   /*!
    * \brief Closes connections and threads.
    */
   void stopDevices();

   //! Interval to request configurations until received.
   static const int CONF_REQUEST_INTERVAL;

   //! Name of group to listen for configuration response.
   static const QString CONF_RESPONSE_GROUP;

   //! Configuration response message group.
   Utils::MessageGroup* m_confResponseGroup;

   //! Timer for requesting configurations.
   QTimer m_confRequestTimer;

   //! Interface feature name.
   QString m_featureName;

   //! Communication plugin container.
   QHash<QString, Plugins::CommunicationInterface*> m_communicationPlugins;

   //! Protocol plugin container.
   QHash<QString, Plugins::ProtocolInterface*> m_protocolPlugins;

   //! Device congainer.
   QHash<QString, Device*> m_devices;
};

} // Core

#endif
