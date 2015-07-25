#ifndef MESSAGEBROKER_H
#define MESSAGEBROKER_H

#include <QObject>
#include <QTcpServer>
#include <QHash>
#include <QSet>

#include "connection.h"
#include "group.h"

namespace Core {

class MessageBroker : public QObject
{
   Q_OBJECT
public:
   explicit MessageBroker(QObject *parent = 0);

private slots:
   void onNewConnection();
   void onJoinToGroup(QString groupName);
   void onPublishMessage(QByteArray payload, QString group);
   void onDeliverMessage(QByteArray payload, QString group);
   void onDisconnect();

private:
   QTcpServer* m_server;
   QList<Connection*> m_connections;
   QHash<QString, Group*> m_groups;
};

} // Core

#endif
