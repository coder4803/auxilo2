#include <QDataStream>

#include "confresponsemessage.h"

namespace Utils {

ConfResponseMessage::ConfResponseMessage() :
   m_result(NO_ERROR)
{
}

ConfResponseMessage::ConfResponseMessage(const QByteArray& payload)
{
   QDataStream stream(payload);

   int result;
   stream >> result;
   m_result = static_cast<Result>(result);

   quint32 numberOfParameters;
   stream >> numberOfParameters;

   for (quint32 i = 0; i < numberOfParameters; ++i) {
      QString name;
      stream >> name;

      QString value;
      stream >> value;

      m_parameters.insert(name, value);
   }
}

bool ConfResponseMessage::appendParameter(QString name,
                                          QString value)
{
   if (!m_parameters.contains(name)) {
      m_parameters.insert(name, value);
      return true;
   }

   return false;
}

void ConfResponseMessage::appendParameters(QHash<QString, QString> parameters)
{
   foreach (QString parameter, parameters.keys()) {
      appendParameter(parameter, parameters.value(parameter));
   }
}

QStringList ConfResponseMessage::parameterList(QString startsWith,
                                               bool returnShortPath) const
{
   if (startsWith == QString()) {
      return m_parameters.keys();
   }

   QStringList parameters;
   foreach (QString parameter, m_parameters.keys()) {
      if (parameter.startsWith(startsWith)) {
         if (returnShortPath) {
            parameters.append(parameter.mid(startsWith.length()));
         } else {
            parameters.append(parameter);
         }
      }
   }

   return parameters;
}

QByteArray ConfResponseMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);

   stream << m_result;
   stream << m_parameters.size();

   foreach (QString parameter, m_parameters.keys()) {
      stream << parameter;
      stream << m_parameters.value(parameter);
   }

   return message;
}

QString ConfResponseMessage::string() const
{
   QString message("ConfResponseMessage:\n");
   message += QString("Result: %1\n").arg(m_result);

   foreach (QString parameter, m_parameters.keys()) {
      message += QString("- %1: %2\n").arg(parameter)
                                      .arg(m_parameters.value(parameter));
   }

   return message;
}

} // Utils
