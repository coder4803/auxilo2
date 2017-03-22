#include <QObject>
#include <QList>
#include <QUdpSocket>

#include "communications.h"

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
   //! Emits connected signal.
   void onConnected();

   //! Handles readyRead signal from communication socket.
   void onDataReceived();

private:
   //! Constant for local port parameter name.
   static const QString PARAMETER_LOCAL_PORT;

   //! Constant for local ip address parameter name.
   static const QString PARAMETER_LOCAL_IP_ADDRESS;

   //! Constant for remote port parameter name.
   static const QString PARAMETER_REMOTE_PORT;

   //! Constant for remote ip address parameter name.
   static const QString PARAMETER_REMOTE_IP_ADDRESS;

   //! Default maximum size for received datagram size. Bigger are ignored.
   static const int DEFAULT_DATAGRAM_MAX_SIZE;

   //! UDP socket.
   QUdpSocket m_socket;

   //! Local port.
   quint16 m_localPort;

   //! Local interface to listen.
   QHostAddress m_localIpAddress;

   //! Remote port.
   quint16 m_remotePort;

   //! Remote ip address.
   QHostAddress m_remoteIpAddress;

   //! Buffer for received datagram
   QByteArray m_buffer;
};

} // Plugins

DEFINE_COMMUNICATION_PLUGIN(UDP)

#endif
