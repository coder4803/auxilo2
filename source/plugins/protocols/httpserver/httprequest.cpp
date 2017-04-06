#include "httprequest.h"

HTTPRequest::HTTPRequest()
{

}

HTTPRequest::HTTPRequest(const QString& command,
                         const QString& address,
                         const QString& protocol)
{
   m_command = command;
   m_address = address;
   m_protocol = protocol;

   QString line = QString("%1 %2 %3").arg(command)
           .arg(address).arg(protocol);

   HTTPMessage::setFirstLine(line);
}

HTTPRequest::~HTTPRequest()
{
}

int HTTPRequest::buildFromData(const QByteArray& data)
{
   int result = HTTPMessage::buildFromData(data);

   QString firstLine = HTTPMessage::firstLine();

   QTextStream stream(&firstLine);
   stream >> m_command;
   stream >> m_address;
   stream >> m_protocol;

   if (stream.status() != QTextStream::Ok) {
      qDebug() << "Invalid status line!";
      result = -1;
   }

   return result;
}
