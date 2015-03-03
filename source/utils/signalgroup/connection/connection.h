#include <QObject>
#include <QTimer>

#include "amqp/amqp.h"

#ifndef SYSTEM_CONNECTION_H
#define SYSTEM_CONNECTION_H

namespace System {

class Connection : public QObject
{
   Q_OBJECT
public:
   static QAMQP::Client* getClient();
   ~Connection();

private slots:
   void connectToBeam();
   void onConnected();
   void onDisconnected();

signals:
   void connected();

private:
   // Singleton, disable copy and assign
   explicit Connection();
   Connection(Connection const&);
   void operator=(Connection const&);

   QAMQP::Client* client();

   static Connection* m_instance;
   QAMQP::Client* m_client;

   QTimer m_connectionTimer;
};

} // System

#endif
