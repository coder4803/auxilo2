#include <QObject>
#include <QWebSocketServer>

#include "communications.h"

#ifndef WSSERVER_H
#define WSSERVER_H

namespace Plugins {

/*!
 * \brief The WSServer class.
 *
 * This class wraps WebSocket server communication
 * to general communication interface.
 */
class WSServer : public Communication
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param parameters Communication parameters.
    * \param parent Parent object.
    */
   explicit WSServer(const Utils::ParameterSet& parameters,
                      QObject* parent = NULL);

   //! Destructor
   ~WSServer();

   /*!
    * \brief Tells if connection is established.
    * \param connectionId Connection id. WSServer communication can handle
    * multiple connections. This id is used to separate these connections.
    * \return True if connection is established, otherwise false.
    */
   bool isConnected(qint32 connectionId = -1) const;

   /*!
    * \brief Sends data to target "device".
    * \param data Data to send.
    * \param connectionId Connection id. WSServer communication can handle
    * multiple connections. This id is used to separate these connections.
    * \return True if sending succeeded, otherwise false.
    */
   bool sendData(const QByteArray& data,
                 qint32 connectionId = -1);

private slots:
   //! Handles new connections.
   void onNewConnection();

   //! Handles all disconnect signals from QWebSocket.
   void onDisconnected();

   //! Handles textMessageReceived signal from QWebSocket.
   void onTextMessageReceived(const QString& message);

private:
   //! Returns next free connection id.
   qint32 nextConnectionId();

   //! Constant for maximum number of simultaneously connections parameter name.
   static const QString PARAMETER_MAX_CONNECTIONS;

   //! Constant for local port parameter name.
   static const QString PARAMETER_LOCAL_PORT;

   //! Constant for server name parameter name.
   static const QString PARAMETER_SERVER_NAME;

   //! Constant for default maximum number of connections.
   static const quint16 DEFAULT_MAX_CONNECTIONS;

   //! Constant for default server name.
   static const QString DEFAULT_SERVER_NAME;

   //! Server socket.
   QWebSocketServer* m_server;

   //! Connection socket container.
   QHash<QWebSocket*, quint32> m_connections;

   //! Server name.
   QString m_serverName;

   //! Local port.
   quint16 m_localPort;

   //! Maximum number of simultaneously connections.
   quint16 m_maxConnections;

   //! Connection id counter.
   qint32 m_connectionIdCounter;
};

} // Plugins

DEFINE_COMMUNICATION_PLUGIN(WSServer)

#endif
