#ifndef MESSAGEBROKER_H
#define MESSAGEBROKER_H

#include <QObject>
#include <QTcpServer>
#include <QHash>
#include <QSet>

#include "connection.h"
#include "group.h"

namespace Core {

/*!
 * \brief The MessageBroker class.
 * MessageBroker's task is to deliver messages between other core applications.
 * It works a lot like simple IRC-server but has some extra features.
 * This is main class of message broker. It takes care of handling connections
 * and groups. It also handles data delivery on high level.
 */
class MessageBroker : public QObject
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param parent Parent object.
    */
   explicit MessageBroker(QObject *parent = 0);

private slots:
   /*!
    * \brief Handles new connections.
    */
   void onNewConnection();

   /*!
    * \brief Inserts connection to a group.
    * Note that "connection" is the sender of the signal!
    * \param groupName Name of group the connection is inserted to.
    */
   void onJoinToGroup(QString groupName);

   /*!
    * \brief Publishes message to a group.
    * \param payload Message to publish.
    * \param group Name of group the message is published to.
    */
   void onPublishMessage(QByteArray payload, QString group);

   /*!
    * \brief Delivers message to group
    * \param payload Message to delive.
    * \param group Name of group the message is delivered to.
    */
   void onDeliverMessage(QByteArray payload, QString group);

   /*!
    * \brief Handles client disconnections.
    */
   void onDisconnect();

private:
   //! Server socket.
   QTcpServer* m_server;

   //! List of connections.
   QList<Connection*> m_connections;

   //! Hash of groups.
   QHash<QString, Group*> m_groups;
};

} // Core

#endif
