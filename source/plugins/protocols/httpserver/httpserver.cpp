#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "httprequest.h"
#include "httpreply.h"
#include "httpserver.h"

namespace Plugins {

const QString HTTPServer::PARAMETER_CONTENT_PATH("contentpath");

HTTPServer::HTTPServer(Utils::ParameterSet parameters,
                       QObject* parent) :
   Protocol(parent)
{
    try {
       // Content path.
       m_contentPath = parameters.parameter<QString>(PARAMETER_CONTENT_PATH);
       qCritical() << "Content path:" << m_contentPath;
    } catch (QException& e) {
       qCritical() << "Failed to initialize protocol.";
       throw e;
    }

    // Make sure that content paht ends with '/'. It is a security
    // issue to compare get request addresses without it!
    if (!m_contentPath.endsWith("/")) {
       m_contentPath += "/";
    }

    // Check that content path exists.
    QFileInfo fileInfo(m_contentPath);
    if (!fileInfo.isDir()) {
       qCritical() << "Content path doesn't exist:" << m_contentPath;
       throw QException();
    }

    registerContentTypes();
}

HTTPServer::~HTTPServer()
{
   QMutexLocker locker(&m_receiveBufferMutex);

   foreach (QByteArray* buffer, m_receiveBuffers) {
      delete buffer;
   }
}

void HTTPServer::disconnected(qint32 connectionId)
{
   QMutexLocker locker(&m_receiveBufferMutex);

   // Free connection's receive buffer.
   if (m_receiveBuffers.contains(connectionId)) {
       delete m_receiveBuffers.value(connectionId);
       m_receiveBuffers.remove(connectionId);
   }
}

void HTTPServer::dataReceived(QByteArray data,
                              qint32 connectionId)
{
   QMutexLocker locker(&m_receiveBufferMutex);

   // Create buffer for connection id if doesn't already exist.
   if (!m_receiveBuffers.contains(connectionId)) {
       m_receiveBuffers.insert(connectionId, new QByteArray());
   }

   // Append received data to receive buffer.
   QByteArray* buffer =  m_receiveBuffers.value(connectionId);
   buffer->append(data);

   // Try to build HTTP request message from received data.
   HTTPRequest request;
   int parsedSize = request.buildFromData(*buffer);
   if (parsedSize == -1) {
      qDebug() << "Received incomplete request:\n" << buffer->data();
      return;
   }

   // Remove parsed data from buffer.
   buffer->remove(0, parsedSize);

   // Handle request.
   if (request.command().toLower() == "get") {
      QByteArray content;
      QString type;

      qDebug() << "Page requested:" << request.address();

      if (getPageContent(request.address(), content, type)) {
         sendGetResponse(content, type, connectionId);
      } else {
         sendErrorResponse(404, "Page not found", connectionId);
      }
   } else {
      qDebug() << "Unsupported request type:" << request.command();
   }
}

bool HTTPServer::getPageContent(const QString address,
                                QByteArray& content,
                                QString& type)
{
    QString contentPath = m_contentPath + address;

    // Remove double '/' characters.
    contentPath.replace("//", "/");

    // Check that address is at legal area. Otherwise
    // user could ask any file from the server.
    QFileInfo fileInfo(contentPath);
    if (!fileInfo.absoluteFilePath().startsWith(m_contentPath)) {
       qDebug() << "Illegal address:" << address;
       return false;
    }

    // Open index.htm by default.
    if (fileInfo.isDir()) {
        if (!contentPath.endsWith("/")) {
           contentPath += "/";
        }

        contentPath += "index.htm";
    }

    // Check that file exists.
    if (!fileInfo.exists()) {
        qDebug() << "File doesn't exist:" << contentPath;
        return false;
    }

    // Load file content.
    QFile file(contentPath);
    if (!file.open(QIODevice::ReadOnly)) {
       qDebug() << "Couldn't open file:" << contentPath;
       return false;
    }

    content = file.readAll();

    // Resolve content type (default is "text/html").
    type = "text/html";
    int dotIndex = contentPath.lastIndexOf(".");
    if (dotIndex != -1) {
       QString fileType = contentPath.mid(dotIndex + 1);
       QHash<QString, QString>::const_iterator i = m_contentTypes.find(fileType);
       if (i != m_contentTypes.constEnd()) {
           type = i.value();
       }
    }

    return true;
}

bool HTTPServer::sendGetResponse(const QByteArray& content,
                                 const QString& type,
                                 int connectionId)
{
   HTTPReply reply("HTTP/1.1", 200, "OK");
   reply.setHeader("Content-Type", type);
   reply.setHeader("Content-Length", content.size());
   reply.setContent(content);

   return sendData(reply.data(), connectionId);
}

bool HTTPServer::sendErrorResponse(int errorNumber,
                                   const QString& errorText,
                                   int connectionId)
{
   QString content = QString("<h1>%1 - %2</h1>")
           .arg(errorNumber).arg(errorText);

   HTTPReply reply("HTTP/1.1", errorNumber, errorText);
   reply.setHeader("Content-Length", content.size());
   reply.setHeader("Content-Type", "text/html");
   reply.setHeader("Connection", "Closed");
   reply.setContent(content.toLatin1().data());

   return sendData(reply.data(), connectionId);
}

void HTTPServer::registerContentTypes()
{
    m_contentTypes.insert("htm", "text/html");
    m_contentTypes.insert("html", "text/html");
    m_contentTypes.insert("jpg", "image/jpeg");
    m_contentTypes.insert("jpeg", "image/jpeg");
    m_contentTypes.insert("png", "image/png");
    m_contentTypes.insert("gif", "image/gif");
}

} // Plugins
