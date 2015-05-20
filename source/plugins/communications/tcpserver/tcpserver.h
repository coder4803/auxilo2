#include <QObject>
#include <QList>
#include <QTcpServer>

#include "communication.h"

#ifndef TCPSERVER_H
#define TCPSERVER_H

namespace Plugins {

class TCPServer : public Communication
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor
    * \param parameters Communication parameters.
    * \param parent Parent object.
    */
   explicit TCPServer(const Utils::ParameterSet& parameters,
                      QObject* parent = NULL);

   //! Destructor
   ~TCPServer();

   /*!
    * \brief Tells if connection is established.
    * \param connectionId Connection id. TCPServer communication can handle
    * multiple connections. This id is used to separate these connections.
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
    * \param connectionId Connection id. TCPServer communication can handle
    * multiple connections. This id is used to separate these connections.
    * \return True if sending succeeded, otherwise false.
    */
   bool sendData(const QByteArray& data,
                 qint32 connectionId = -1);

private slots:
   //! Handles new connections.
   void onNewConnection();

   //! Handles all disconnect signals from connection socket.
   void onDisconnected();

   //! Handles readyRead signal from communication socket.
   void onDataReceived();

private:
   //! Returns next free connection id.
   qint32 nextConnectionId();

   //! Constant for maximum number of simultaneously connections parameter name.
   static const QString PARAMETER_MAX_CONNECTIONS;

   //! Constant for local port parameter name.
   static const QString PARAMETER_LOCAL_PORT;

   //! Constant for default maximum number of connections.
   static const quint16 DEFAULT_MAX_CONNECTIONS;

   //! Server socket.
   QTcpServer m_server;

   //! Connection socket container.
   QHash<QTcpSocket*, quint32> m_connections;

   //! Local port.
   quint16 m_localPort;

   //! Maximum number of simultaneously connections.
   quint16 m_maxConnections;

   //! Connection id counter.
   qint32 m_connectionIdCounter;
};

} // Plugins

#endif
