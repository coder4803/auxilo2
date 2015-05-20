#include <QCoreApplication>
#include <QPluginLoader>
#include <QDir>

#include "messagegroup.h"
#include "confrequestmessage.h"
#include "stateresponsemessage.h"
#include "signalmessage.h"
#include "statechangedmessage.h"

#include "interface.h"

namespace Core {

const int Interface::CONF_REQUEST_INTERVAL(2000);
const QString Interface::CONF_RESPONSE_GROUP("interfaceConf");

Interface::Interface(QString serverAddress,
                     QString featureName) :
   m_confResponseGroup(NULL),
   m_featureName(featureName)
{
   Utils::Connection::setHost(serverAddress);

   // Initialize confResponse message group.
   m_confResponseGroup =
         new Utils::MessageGroup(CONF_RESPONSE_GROUP,
                                 Utils::MessageGroup::Subscriber);
   connect(m_confResponseGroup, SIGNAL(messageReceived(QByteArray, QString)),
           this, SLOT(handleConfResponseMessage(QByteArray)));

   // Request parameters when response group is ready.
   connect(m_confResponseGroup, SIGNAL(ready()),
           this, SLOT(requestParameters()));
}

Interface::~Interface()
{
   stopDevices();
   unloadPlugins();

   qCritical("Program is terminated.");
}

void Interface::onPublish(const QByteArray& message,
                          QString group)
{
   Utils::MessageGroup::publish(message, group);
}

void Interface::requestParameters()
{
   // Request parameters from confmanager.
   Utils::ConfRequestMessage request(CONF_RESPONSE_GROUP, m_featureName, true);
   Utils::MessageGroup::publish(request, Utils::CONF_REQUEST_GROUP);

   // Request parameters until received.
   if (!m_confRequestTimer.isActive()) {
      connect(&m_confRequestTimer, SIGNAL(timeout()),
              this, SLOT(requestParameters()));
      m_confRequestTimer.start(CONF_REQUEST_INTERVAL);
   }

   qCritical("Waiting for parameters...");
}

void Interface::handleConfResponseMessage(QByteArray payload)
{
   Utils::ConfResponseMessage response(payload);
   QString featureName = response.parameteSet().featureName();

   if (featureName == m_featureName) {
      // Stop requesting parameters.
      m_confRequestTimer.stop();

      if (!loadPlugins()) {
         qCritical("Failed to load plugins.");
         emit QCoreApplication::exit(EXIT_FAILURE);
      }

      Utils::ParameterSet parameters = response.parameteSet();
      if(!initDevices(parameters)) {
         qCritical("Failed to initialize devices, quitting program.");
         emit QCoreApplication::exit(EXIT_FAILURE);
      }
   } else {
      qWarning("Reseived parameters with unknown feature name.");
   }
}

bool Interface::loadPlugins()
{
   qCritical("\nLoading plugins...");

   if (!loadCommunicationPlugins()) {
      qCritical("Failed to load communication plugins.");
      return false;
   }

   if (!loadProtocolPlugins()) {
      qCritical("Failed to load protocol plugins.");
      return false;
   }

   qCritical("All plugins loaded successfully.");

   return true;
}

bool Interface::loadCommunicationPlugins()
{
   QDir communicationPluginDir(QCoreApplication::applicationDirPath() +
                               "/plugins/communication/");

   if (!communicationPluginDir.exists()) {
      qCritical("Communication plugin folder %s doesn't exist.",
                communicationPluginDir.path().toLatin1().data());
      return false;
   }

   qCritical("Communication plugins:");

   QStringList files = communicationPluginDir.entryList(QDir::Files);
   foreach (QString file, files) {
      QPluginLoader loader(communicationPluginDir.absoluteFilePath(file));

      QObject* plugin = loader.instance();
      if (!plugin) {
         qDebug("Couldn't load plugin: %s", file.toLatin1().data());
         continue;
      }

      // If communication plugin.
      Plugins::CommunicationInterface* communicationPlugin =
            qobject_cast<Plugins::CommunicationInterface*>(plugin);
      if (communicationPlugin) {
         QString name = communicationPlugin->name().toLower();
         m_communicationPlugins.insert(name, communicationPlugin);
         qCritical(" - %s", name.toLatin1().data());
         continue;
      }

      qDebug("Unknown plugin interface: %s", file.toLatin1().data());
   }

   return true;
}

bool Interface::loadProtocolPlugins()
{
   QDir protocolPluginDir(QCoreApplication::applicationDirPath() +
                               "/plugins/protocol/");

   if (!protocolPluginDir.exists()) {
      qCritical("Protocol plugin folder %s doesn't exist.",
                protocolPluginDir.path().toLatin1().data());
      return false;
   }

   qCritical("Protocol plugins:");

   QStringList files = protocolPluginDir.entryList(QDir::Files);
   foreach (QString file, files) {
      QPluginLoader loader(protocolPluginDir.absoluteFilePath(file));

      QObject* plugin = loader.instance();
      if (!plugin) {
         qDebug("Couldn't load plugin: %s", file.toLatin1().data());
         continue;
      }

      // If protocol plugin.
      Plugins::ProtocolInterface* protocolPlugin =
            qobject_cast<Plugins::ProtocolInterface*>(plugin);
      if (protocolPlugin) {
         QString name = protocolPlugin->name().toLower();
         m_protocolPlugins.insert(name, protocolPlugin);
         qCritical(" - %s", name.toLatin1().data());
         continue;
      }

      qDebug("Unknown plugin interface: %s", file.toLatin1().data());
   }

   return true;
}

void Interface::unloadPlugins()
{
   // Unload communication plugins.
   qCritical("Unloading communication plugins...");
   QDir communicationPluginDir(QCoreApplication::applicationDirPath() +
                               "/plugins/communication/");

   if (!communicationPluginDir.exists()) {
      return;
   }

   QStringList files = communicationPluginDir.entryList(QDir::Files);
   foreach (QString file, files) {
      QPluginLoader loader(communicationPluginDir.absoluteFilePath(file));
      if (loader.isLoaded()) {
         loader.unload();
      }
   }

   // Unload protocol plugins.
   qCritical("Unloading protocol plugins...");
   QDir protocolPluginDir(QCoreApplication::applicationDirPath() +
                               "/plugins/protocol/");

   if (!protocolPluginDir.exists()) {
      return;
   }

   files = protocolPluginDir.entryList(QDir::Files);
   foreach (QString file, files) {
      QPluginLoader loader(protocolPluginDir.absoluteFilePath(file));
      if (loader.isLoaded()) {
         loader.unload();
      }
   }
}

bool Interface::initDevices(const Utils::ParameterSet& parameters)
{
   qCritical("\nInitializing devices...");

   // Get list of devices.
   QStringList devices;
   foreach (QString parameter, parameters.parameterList()) {
      int index = parameter.indexOf(".");
      if (index != -1) {
         QString deviceName = parameter.mid(0, index);
         if (!deviceName.isEmpty() && !devices.contains(deviceName)) {
            devices.append(deviceName);
         }
      }
   }

   // Handle devices.
   foreach (QString deviceName, devices) {
      qCritical("Loading device: %s...", deviceName.toLatin1().data());

      // Read parameters
      QString tmp;
      QString connectionType;
      Utils::ParameterSet communicationParameters;
      QString protocolType;
      Utils::ParameterSet protocolParameters;

      try {
         // Get connection type.
         tmp = deviceName + ".connection.type";
         connectionType = parameters.parameter<QString>(tmp).toLower();

         // Get connection parameters
         tmp = deviceName + ".connection.";
         communicationParameters = parameters.getSection(tmp);

         // Get protocol type
         tmp = deviceName + ".protocol.type";
         protocolType = parameters.parameter<QString>(tmp).toLower();

         // Get protocol parameters.
         tmp = deviceName + ".protocol.";
         protocolParameters = parameters.getSection(tmp);
      } catch (QException& e) {
         qCritical("Invalid parameters, quitting");
         QCoreApplication::exit(EXIT_FAILURE);
      }

      // Get communication plugin.
      if (!m_communicationPlugins.contains(connectionType)) {
         qCritical("No plugin for connection type: %s",
                   connectionType.toLatin1().data());
         return false;
      }

      Plugins::CommunicationInterface* communicationPlugin =
            m_communicationPlugins.value(connectionType);

      // Get protocol plugin.
      if (!m_protocolPlugins.contains(protocolType)) {
         qCritical("No plugin for protocol type: %s",
                   protocolType.toLatin1().data());
         return false;
      }

      Plugins::ProtocolInterface* protocolPlugin =
            m_protocolPlugins.value(protocolType);

      // Initialize device.
      try {
         Device* device = new Device(deviceName,
                                           communicationPlugin,
                                           communicationParameters,
                                           protocolPlugin,
                                           protocolParameters);

         // All messages are routed through main thread.
         connect(device, SIGNAL(publish(QByteArray,QString)),
                 this, SLOT(onPublish(QByteArray,QString)));

         bool result = false;
         QMetaObject::invokeMethod(device, "start",
                                   Qt::BlockingQueuedConnection,
                                   Q_RETURN_ARG(bool, result));
         if (!result) {
            throw QException();
         }

         m_devices.insert(deviceName, device);
      } catch (QException& e) {
         qCritical("Failed to initialize device %s.",
                   deviceName.toLatin1().data());
         return false;
      }
   }

   qCritical("All devices initialized successfully.");
   qCritical("\nInterface is running...");
   return true;
}

void Interface::stopDevices()
{
   qCritical("Stopping devices...");
   foreach (Device* device, m_devices) {
      device->stop();
   }
}

} // Core
