#include <QObject>
#include <QHash>
#include <QString>

#include <messagegroup.h>

#ifndef CONFMANAGER_H
#define CONFMANAGER_H

namespace Core {

/*!
 * \brief The ConfManager class
 * This is main class of configuration class. It takes
 * care of handling configuration messages.
 */
class ConfManager : public QObject
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param coreConfigFile Core feature parameter file.
    * \param deviceConfigFile Device parameter file.
    * \param serverAddress RabbitMQ server address.
    * \param parent Parent QObject.
    */
   ConfManager(const QString& coreConfigFile,
               const QString& deviceConfigFile,
               const QString& serverAddress,
               QObject* parent = NULL);

private slots:
   /*!
    * \brief Handles configuration request messages.
    * \param payload Received raw message.
    */
   void handleConfRequestMessage(QByteArray payload);

private:
   static const QString CONF_REQUEST_GROUP;

   QString m_coreConfigPath;
   QString m_deviceConfigPath;
   Utils::MessageGroup* m_group;

   /*!
    * \brief Reads parameters from configuration file on high level.
    * \param featureName Feature to read.
    * \param parameters Reference to hash that is filled with parameters.
    * \param isCoreFeature Whether requested core feature or not.
    * \return
    */
   bool readParameters(QString featureName,
                       QHash<QString, QString>& parameters,
                       bool isCoreFeature);
};

} // Core

#endif
