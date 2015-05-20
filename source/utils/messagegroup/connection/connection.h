#include <QObject>
#include <QTimer>
#include <QMutex>

#include "amqp/amqp.h"

#ifndef UTILS_CONNECTION_H
#define UTILS_CONNECTION_H

namespace Utils {

/*!
 * \brief The Connection class
 * Class handles connection to RabbitMQ beam.
 */
class Connection : public QObject
{
   Q_OBJECT
   Q_DISABLE_COPY(Connection)
public:
   /*!
    * \brief getClient returns pointer to QAMQP::Client object.
    * This method should be used only by MessageGroup class!
    * \return Pointer to QAMQP::Client.
    */
   static QAMQP::Client* getClient();

   /*!
    * \brief setHost can be used to define RabbitMQ server address and port.
    * Note that this method must be called before creating SigalGroups.
    * However it is not necessary to call this function at all if default
    * values are fine (5672@127.0.0.1).
    * Calling this function will create instance of Connection and connect
    * to RabbitMQ server.
    * \param address RabbitMQ server address
    * \param port RabbitMQ server port
    */
   static void setHost(QString address,
                       quint16 port = DEFAULT_PORT);

   /*!
    * \brief isConnected can be used to check if connection exists.
    * \return True if connection is ok, otherwise false.
    */
   static bool isConnected();

private slots:
   void connectToBeam();
   void onConnected();
   void onDisconnected();

private:
   static const QString DEFAULT_ADDRESS;
   static const quint16 DEFAULT_PORT;
   static const quint16 DEFAULT_RETRY_INTERVAL;

   // Singleton class
   Connection(QString address,
              quint16 port);
   ~Connection();

   static bool instanceExists();
   static Connection* instance(QString address = DEFAULT_ADDRESS,
                               quint16 port = DEFAULT_PORT);

   QAMQP::Client* client();

   static QMutex m_mutex;
   static Connection* m_instance;
   QAMQP::Client* m_client;

   quint32 m_retryInterval;
   QTimer m_connectionTimer;
};

} // Utils

#endif
