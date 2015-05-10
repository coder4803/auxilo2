#include <QXmlSimpleReader>
#include <QException>
#include <QFile>

#include "messagegroup.h"
#include "confrequestmessage.h"
#include "confresponsemessage.h"

#include "confreader.h"
#include "confmanager.h"

namespace Core {

const QString ConfManager::CONF_REQUEST_GROUP("confRequest");

ConfManager::ConfManager(const QString& coreConfigFile,
                         const QString& deviceConfigFile,
                         const QString& serverAddress,
                         QObject* parent) :
   QObject(parent),
   m_coreConfigPath(coreConfigFile),
   m_deviceConfigPath(deviceConfigFile),
   m_group(NULL)
{
   // Check that configuartio files exist
   if (!QFile::exists(m_coreConfigPath)) {
      qCritical("Error: Core configuartion file doesn't exist: %s",
                m_coreConfigPath.toLatin1().data());
      throw QException();
   }

   if (!QFile::exists(m_deviceConfigPath)) {
      qCritical("Warning: Device configuartion file doesn't exist: %s",
                m_deviceConfigPath.toLatin1().data());
   }

   // Set RabbitMQ server address
   Utils::Connection::setHost(serverAddress);

   // Listen for configuration requests
   m_group = new Utils::MessageGroup(CONF_REQUEST_GROUP,
                                     Utils::MessageGroup::Subscriber,
                                     this);
   connect(m_group, SIGNAL(messageReceived(QByteArray, QString)),
           this, SLOT(handleConfRequestMessage(QByteArray)));

   qCritical("\nConfmanager is running...");
}

void ConfManager::handleConfRequestMessage(QByteArray payload)
{
   Utils::ConfRequestMessage request(payload);

   // Read parameters from configuration file
   QHash<QString, QString> parameters;
   bool success = readParameters(request.featureName(), parameters,
                                 request.isCoreFeature());

   // Create response message
   Utils::ParameterSet parameterSet;
   parameterSet.setFeatureName(request.featureName());
   parameterSet.appendParameters(parameters);
   Utils::ConfResponseMessage response(parameterSet);

   // Set result of configuration request
   if (!success) {
      response.setResult(Utils::ConfResponseMessage::INVALID_PARAMETERS);
   } else if (parameters.isEmpty()) {
      response.setResult(Utils::ConfResponseMessage::NO_PARAMETERS);
   }

   Utils::MessageGroup::publish(response, request.responseGroup());
}

bool ConfManager::readParameters(QString featureName,
                                 QHash<QString, QString>& parameters,
                                 bool isCoreFeature)
{
   // Select configuration file based on isCoreFeature flag
   QFile file(m_deviceConfigPath);
   if (isCoreFeature) {
      file.setFileName(m_coreConfigPath);
   }

   if (!file.exists()) {
      qCritical("Couldn't find %s.", file.fileName().toLatin1().data());
      return false;
   }

   QXmlInputSource source(&file);
   QXmlSimpleReader xmlReader;
   ConfReader confReader(featureName, parameters);

   xmlReader.setContentHandler(&confReader);
   xmlReader.setErrorHandler(&confReader);
   if (!xmlReader.parse(source)) {
      qCritical("Error while reading configurations: %s",
                confReader.getErrorMsg().toLatin1().data());

      parameters.clear();
      return false;
   }

   return true;
}

} // Core
