#include <QObject>
#include <QHash>
#include <QHostAddress>

#include "communications.h"
#include "sharedudpsocket.h"

#ifndef UDP_H
#define UDP_H

namespace Plugins {

class UDP : public Communication
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor
    * \param parameters Communication parameters.
    * \param parent Parent object.
    */
   explicit UDP(const Utils::ParameterSet& parameters,
                QObject* parent = NULL);

   //! Destructor
   ~UDP();

   /*!
    * \brief This method returns always true.
    * \param connectionId Unused.
    * \return Always true.
    */
   bool isConnected(qint32 connectionId = -1) const;

   /*!
    * \brief Sends data to target "device".
    * \param data Data to send.
    * \param connectionId Unused.
    * \return True if sending succeeded, otherwise false.
    */
   bool sendData(const QByteArray& data,
                 qint32 connectionId = -1);

private slots:
   //! Emits connected signal.
   void onConnected();

   /*!
    * \brief Handles dataReceived signal from socket.
    * \param data Received data.
    * \param host Sender address.
    * \param port Sender port.
    */
   void onDataReceived(const QByteArray &data,
                       const QHostAddress &host,
                       quint16 port);

private:
   //! Constant for local port parameter name.
   static const QString PARAMETER_LOCAL_PORT;

   //! Constant for remote port parameter name.
   static const QString PARAMETER_REMOTE_PORT;

   //! Constant for remote ip address parameter name.
   static const QString PARAMETER_REMOTE_IP_ADDRESS;

   //! Default maximum size for received datagram size. Bigger are ignored.
   static const int DEFAULT_DATAGRAM_MAX_SIZE;

   //! Counts number of instances of UDP class.
   static int m_instanceCounter;

   //! Container for UDP sockets.
   static QHash<quint16, SharedUdpSocket*> m_sockets;

   //! Protects UDP sockets.
   static QMutex m_socketMutex;

   //! UDP socket.
   SharedUdpSocket* m_socket;

   //! Local port.
   quint16 m_localPort;

   //! Remote port.
   quint16 m_remotePort;

   //! Remote ip address.
   QHostAddress m_remoteIpAddress;

   //! Returns (or creates) UDP socket for given port.
   SharedUdpSocket* getSocket(quint16 localPort);
};

} // Plugins

DEFINE_COMMUNICATION_PLUGIN(UDP)

#endif
