#include <QTimer>
#include <QSet>
#include <QReadWriteLock>

#include "protocols.h"

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

namespace Plugins {

/*!
 * \brief The HTTPServer class.
 * This class implements HTTP server protocol.
 */
class HTTPServer : public Protocol
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param parameters Unused.
    * \param parent Parent object.
    */
   explicit HTTPServer(Utils::ParameterSet parameters,
                       QObject* parent = NULL);

   //! Destructor.
   ~HTTPServer();

protected slots:
    /*!
    * \brief Handles client disconnection.
    * \param connectionId Connection id.
    */
   void disconnected(qint32 connectionId);

   /*!
    * \brief Handles data received from user.
    * \param data Received data.
    * \param connectionId Connection id.
    */
   void dataReceived(QByteArray data,
                     qint32 connectionId);

private:
   //! Constant for web page content path parameter.
   static const QString PARAMETER_CONTENT_PATH;

   //! Web page content path.
   QString m_contentPath;

   //! List of known content types.
   QHash<QString, QString> m_contentTypes;

   //! Receive buffers for each connection.
   QHash<int, QByteArray*> m_receiveBuffers;

   //! Mutex for receive buffers.
   QMutex m_receiveBufferMutex;

   /*!
    * \brief Gets page content.
    * \param address Sub address.
    * \param[out] content Page content.
    * \return True on success, false on failure.
    */
   bool getPageContent(const QString address,
                       QByteArray& content,
                       QString& type);

   /*!
    * \brief Send responses for a GET request.
    * \param content Page content.
    * \param connectionId Connection id.
    * \return True on success, false on failure.
    */
   bool sendGetResponse(const QByteArray& content,
                        const QString& type,
                        int connectionId);

   /*!
    * \brief Sends error response.
    * \param errorNumber Error code.
    * \param connectionId Connection id.
    * \return True on success, false on failure.
    */
   bool sendErrorResponse(int errorNumber,
                          const QString& errorText,
                          int connectionId);

   /*!
    * \brief Registers content types for different file specifiers.
    */
   void registerContentTypes();
};

} // Plugins

DEFINE_PROTOCOL_PLUGIN(HTTPServer)

#endif
