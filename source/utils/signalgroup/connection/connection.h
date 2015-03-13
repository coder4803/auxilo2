#include <QObject>
#include <QTimer>

#include "amqp/amqp.h"

#ifndef UTILS_CONNECTION_H
#define UTILS_CONNECTION_H

namespace Utils {

const QString DEFAULT_ADDRESS = "127.0.0.1";
const quint16 DEFAULT_PORT = 5672;

class Connection : public QObject
{
   Q_OBJECT
   Q_DISABLE_COPY(Connection)
public:
   static QAMQP::Client* getClient();
   ~Connection();

   static void setHost(QString host,
                       quint16 port = DEFAULT_PORT);

   static bool isConnected();

private slots:
   void connectToBeam();
   void onConnected();
   void onDisconnected();

private:
   // Singleton, private constructor
   Connection(QString host,
              quint16 port);

   static bool instanceExists();
   static Connection* instance(QString host = DEFAULT_ADDRESS,
                               quint16 port = DEFAULT_PORT);

   QAMQP::Client* client();

   static Connection* m_instance;
   QAMQP::Client* m_client;

   QString m_host;
   quint16 m_port;
   quint32 m_retryInterval;
   QTimer m_connectionTimer;
};

} // Utils

#endif
