#include <QObject>
#include <QList>
#include <QTimer>
#include <QTcpSocket>

#include "communications.h"

#ifndef TCPSERVER_H
#define TCPSERVER_H

namespace Plugins {

/*!
 * \brief The TCPClient class.
 * This class wraps TCP client communication to general communication interface.
 */
class TCPClient : public Communication
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param parameters Communication parameters.
    * \param parent Parent object.
    */
   explicit TCPClient(const Utils::ParameterSet& parameters,
                      QObject* parent = NULL);

   /*!
    * \brief Tells if connection is established.
    * \param connectionId Unused.
    * \return True if connection is established, otherwise false.
    */
   bool isConnected(qint32 connectionId = -1) const;

   /*!
    * \brief Empty implementation.
    * \param connectionId Unused.
    */
   void readData(qint32 connectionId = -1);

   /*!
    * \brief Sends data to target "device".
    * \param data Data to send.
    * \param connectionId Unused.
    * \return True if sending succeeded, otherwise false.
    */
   bool sendData(const QByteArray& data,
                 qint32 connectionId = -1);

private slots:
   //! Connects to remote host.
   void connectToRemoteHost();

   //! Handles connected signal from communication socket.
   void onConnected();

   //! Handles disconnected signal from communication socket.
   void onDisconnected();

   /*!
    * \brief Handles state changed signal from communication socket.
    * \param state Current state of communication socket.
    */
   void stateChanged(QAbstractSocket::SocketState state);

   //! Handles readyRead signal from communication socket.
   void onDataReceived();

private:
   //! Constant for remote address parameter name.
   static const QString PARAMETER_REMOTE_ADDRESS;

   //! Constant for remote port parameter name.
   static const QString PARAMETER_REMOTE_PORT;

   //! Communication socket.
   QTcpSocket m_socket;

   //! Host address.
   QString m_remoteAddress;

   //! Host port.
   quint16 m_remotePort;
};

} // Plugins

DEFINE_COMMUNICATION_PLUGIN(TCPClient)

#endif
