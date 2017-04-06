#include <QTextStream>
#include <QDataStream>

#include "httpmessage.h"

const int HTTPMessage::HEADER_FIELD_MAX_LENGTH(1024);

HTTPMessage::HTTPMessage()
{
}

HTTPMessage::~HTTPMessage()
{
}

int HTTPMessage::buildFromData(const QByteArray& data)
{
   QTextStream headerStream(data);

   // Read messages first line.
   m_firstLine = headerStream.readLine(HEADER_FIELD_MAX_LENGTH);

   // Go through header lines.
   bool readingHeader = true;
   while (readingHeader) {
      QString line = headerStream.readLine(HEADER_FIELD_MAX_LENGTH);

      if (line.isEmpty()) {
         readingHeader = false;
      } else {
         if (headerStream.atEnd()) {
            qDebug() << "Incomplete header line: " << line;
            return -1;
         }

         QStringList parts = line.split(":");

         if (parts.size() < 2) {
            qDebug() << "Invalid header field:" << line;
            return -1;
         }

         QString name = parts.at(0);
         QString value = parts.at(1).trimmed();

         m_headers.insert(name, value);
      }
   }

   // Read content if exists.
   QDataStream contentStream(data);
   int headerLength = headerStream.pos();
   if (m_headers.contains("Content-Length")) {
      bool ok = false;
      int contentLength = m_headers.value("Content-Length").toInt(&ok);
      if (!ok) {
         qDebug() << "Invalid Content-Length field:"
                  << m_headers.value("Content-Length");
         return -1;
      }

      m_content.resize(contentLength);
      contentStream.skipRawData(headerLength);
      contentStream.readRawData(m_content.data(), contentLength);

      if (contentStream.status() != QDataStream::Ok) {
         qDebug("Received incomplete message!");
         return -1;
      }
   }

   return headerLength + m_content.length();
}

void HTTPMessage::setFirstLine(const QString& data)
{
   m_firstLine = data + "\r\n";
}

bool HTTPMessage::contains(const QString& header)
{
   return m_headers.contains(header);
}

void HTTPMessage::setHeader(const QString& name,
                            const QVariant& value)
{
   m_headers.insert(name, value);
}

void HTTPMessage::setContent(const QByteArray& content)
{
   m_content = content;
}

QByteArray HTTPMessage::data() const
{
   QString header = m_firstLine;

   foreach (QString name, m_headers.keys()) {
      header += name + ": " + m_headers.value(name).toString() + "\r\n";
   }

   QByteArray message;
   message += QString(header + "\r\n").toLatin1().data();
   message += m_content;
   return message;
}
