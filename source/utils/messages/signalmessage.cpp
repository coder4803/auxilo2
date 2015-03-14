#include <QStringList>
#include <QDataStream>

#include "signalmessage.h"

namespace Utils {

SignalMessage::SignalMessage(QString signalName,
                             QStringList parameters,
                             QObject* parent) :
   Message(parent),
   m_signalName(signalName),
   m_parameters(parameters)
{
}

SignalMessage::SignalMessage(QByteArray data,
                             QObject* parent) :
   Message(parent)
{
   QDataStream stream(data);
   stream >> m_signalName;
   stream >> m_parameters;
}

SignalMessage::~SignalMessage()
{
}

void SignalMessage::setSignalName(QString name)
{
   m_signalName = name;
}

void SignalMessage::appendParameter(QString value)
{
   m_parameters.append(value);
}

QString SignalMessage::signalName() const
{
   return m_signalName;
}

quint32 SignalMessage::numberOfParameters() const
{
   return m_parameters.size();
}

QString SignalMessage::parameter(quint8 index) const
{
   Q_ASSERT(m_parameters.size() > index);
   return m_parameters.at(index);
}

QStringList SignalMessage::parameters() const
{
   return m_parameters;
}

QByteArray SignalMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);

   stream << m_signalName;
   stream << m_parameters;

   return message;
}

QString SignalMessage::string() const
{
   QString message("SignalMessage:\n");
   message.append(QString("- Name: %1\n").arg(m_signalName));

   for (int i = 0; i < m_parameters.size(); ++i) {
      message.append(QString("- Param %1: %2\n").
                     arg(i+1).arg(m_parameters.at(i)));
   }

   return message;
}

} // Utils
