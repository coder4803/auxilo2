#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>

namespace Core {

/*!
 * \brief The Connection class.
 * Connection describes one client connection (each application have one
 * connection to the MessageBroker).
 */
class Connection : public QObject
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor
    * \param socket Connection socket.
    * \param parent Parent object.
    */
   explicit Connection(QTcpSocket* socket, QObject* parent = NULL);

signals:
   /*!
    * \brief Signal to join in to a group.
    * \param name Name of the group.
    */
   void joinToGroup(QString name);

   /*!
    * \brief Publishes message to a group.
    * \param payload Message to publish.
    * \param group Name of group the message is published to.
    */
   void publishMessage(QByteArray payload, QString group);

   /*!
    * \brief Delivers message to a group.
    * \param payload Message to delive.
    * \param group Name of group the message is published to.
    */
   void deliveMessage(QByteArray payload, QString group);

   //! Informs about client disconnection.
   void disconnected();

public slots:
   /*!
    * \brief Sends message to client.
    * \param payload Message to send.
    */
   void sendMessage(QByteArray payload);

private slots:
   //! Clears receive buffer.
   void onConnected();

   //! Informs about disconnection.
   void onDisconnected();

   //! Receives data sent by client (on high level).
   void onReadyRead();

private:
   /*!
    * \brief Handles received message.
    * \param data Message to handle.
    */
   void handleReceivedData(const QByteArray& data);

   /*!
    * \brief Handles message that is sent when client wants to join to a group.
    * \param stream Stream to the received data.
    * \return True if valid message was handled, otherwise false.
    */
   bool handleJoinMessage(QDataStream& stream);

   /*!
    * \brief Handles publish messages.
    * \param stream Stream to the received data.
    * \return True if valid message was handled, otherwise false.
    */
   bool handlePublishMessage(QDataStream& stream);

   /*!
    * \brief Handles delive message.
    * \param stream Stream to the received data.
    * \return True if valid message was handled, otherwise false.
    */
   bool handleDeliveMessage(QDataStream& stream);

   //! Connection socket.
   QTcpSocket* m_socket;

   //! Receive buffer.
   QByteArray m_buffer;
};

} // Core

#endif
