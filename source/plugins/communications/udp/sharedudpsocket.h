#include <QThread>
#include <QUdpSocket>

namespace Plugins {

class SharedUdpSocket : public QObject
{
   Q_OBJECT
public:
   SharedUdpSocket(quint16 localPort);
   ~SharedUdpSocket();

   /*!
    * \brief Sends data to target address.
    * \param data Data to send.
    * \param host Remote ip address.
    * \param port Remote port.
    * \return True if sending succeeded, otherwise false.
    */
   qint64 writeDatagram(const QByteArray& data,
                        const QHostAddress& host,
                        quint16 port);

private slots:
   /*!
    * \brief Inits and binds UDP socket.
    * \param localPort Port to bind the socket.
    */
   void initSocket(quint16 localPort);

   /*!
    * \brief Handles writing datagram in socket's thread.
    * \param data Data to send.
    * \param host Remote address.
    * \param port Remote port.
    * \return Bytes written.
    */
   qint64 write(const QByteArray& data,
                const QHostAddress& host,
                quint16 port);

   /*!
    * \brief Handles readyRead signal from communication socket.
    */
   void onDataReceived();

private:
   //! UDP Socket.
   QUdpSocket* m_socket;

   //! Each instance runs in its own thread.
   QThread m_thread;

signals:
   /*!
    * \brief Signal is emitted whenever data is received.
    * \param data Received data.
    * \param host Sender's address.
    * \param port Sender's port.
    */
   void dataReceived(const QByteArray& data,
                     const QHostAddress& host,
                     quint16 port);
};

} // Plugins
