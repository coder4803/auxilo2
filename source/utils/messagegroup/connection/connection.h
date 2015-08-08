#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QHash>
#include <QSet>
#include <QTcpSocket>

#ifndef UTILS_CONNECTION_H
#define UTILS_CONNECTION_H

namespace Utils {

class MessageGroup;

/*!
 * \brief The Connection class
 * Class handles connection to Message broker.
 */
class Connection : public QObject
{
   Q_OBJECT
public:
   /*!
    * \brief Returns instance of Connection.
    * \return Instance of connection.
    */
   static Connection* getInstance();

   /*!
    * \brief Deletes Connection instance.
    */
   static void deleteInstance();

   /*!
    * \brief setHost can be used to define RabbitMQ server address and port.
    * It is not necessary to call this function at all if default
    * values are fine (13803@127.0.0.1).
    * Calling this function will create instance of Connection.
    * \param address Message broker address
    * \param port Message broker port
    */
   static void setHost(QString address,
                       quint16 port = DEFAULT_PORT);

   /*!
    * \brief Tells if connection to messagebroker is established.
    * \return True if connection is established to message broker,
    * otherwise false.
    */
   bool isConnected() const;

   /*!
    * \brief Registers message group to connection system.
    * \param groupName Name of message group.
    * \param messageGroup Pointer to MessageGroup instance to register.
    */
   void registerMessageGroup(QString groupName,
                             MessageGroup* messageGroup);

   /*!
    * \brief Unregister message group from connection system.
    * \param groupName Name of message group.
    * \param messageGroup Pointer to MessageGroup instance to unregister.
    */
   void unregisterMessageGroup(QString groupName,
                               MessageGroup* messageGroup);

   /*!
    * \brief Sends message to message system.
    * \param payload Message payload.
    */
   Q_INVOKABLE void sendMessage(QByteArray payload);

signals:
   void connected();

public slots:
   void connectToHost();

private slots:
   void onConnected();
   void onDisconnected();
   void onMessageReceived();

private:
   static const QString DEFAULT_ADDRESS;
   static const quint16 DEFAULT_PORT;
   static const quint16 DEFAULT_RETRY_INTERVAL;

   // Singleton class
   explicit Connection();
   ~Connection();

   static QMutex m_instanceMutex;
   static Connection* m_instance;

   static QString m_address;
   static quint16 m_port;

   QMutex m_socketMutex;
   QTcpSocket* m_socket;
   QTimer m_connectionTimer;

   QByteArray m_dataBuffer;
   QHash<QString, QSet<MessageGroup*> > m_groups;
};

} // Utils

#endif
