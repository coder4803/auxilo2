#ifndef GROUP_H
#define GROUP_H

#include <QObject>
#include <QSet>

namespace Core {

class Connection;

class Group : public QObject
{
   Q_OBJECT
public:
   explicit Group(QObject *parent = 0);

public slots:
   void addConnection(Connection* connection);
   void removeConnection(Connection* connection);
   void publishMessage(const QByteArray& payload);
   void deliverMessage(const QByteArray& payload);

private:
   QSet<Connection*> m_connections;
   QSet<Connection*>::Iterator m_iter;
};

} // Core

#endif
