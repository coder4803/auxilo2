#include "httpreply.h"

HTTPReply::HTTPReply()
{
}

HTTPReply::HTTPReply(const QString& protocol,
                           int result,
                           const QString& resultString)
{
   // Construct and set first line data.
   m_protocol = protocol;
   m_result = result;
   m_resultString = resultString;

   QString line = QString("%1 %2 %3").arg(protocol)
           .arg(result).arg(resultString);

   HTTPMessage::setFirstLine(line);
}

HTTPReply::~HTTPReply()
{
}

int HTTPReply::buildFromData(const QByteArray& data)
{
   int result = HTTPMessage::buildFromData(data);

   // Build first line data.
   QString firstLine = HTTPMessage::firstLine();

   QTextStream stream(&firstLine);
   stream >> m_protocol;
   stream >> m_result;
   stream >> m_resultString;

   if (stream.status() != QTextStream::Ok) {
      qDebug() << "Invalid status line:" << firstLine;
      result = -1;
   }

   return result;
}
