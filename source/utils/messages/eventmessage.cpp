/* eventmessage.cpp
 *
 * This is the implementation file for the EventMessage class defined in
 * eventmessage.h.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 25-Oct-2015
 */

#include "eventmessage.h"
#include <QDataStream>


namespace Utils {

EventMessage::EventMessage(const QString &signal,
                           const QDateTime &timestamp,
                           quint32 interval_sec,
                           quint32 repeat) :
    Message(), m_signalName(signal), m_timestamp(timestamp),
    m_interval(interval_sec), m_repeat(repeat)
{
    Q_ASSERT( !m_signalName.isEmpty() );
    Q_ASSERT( m_timestamp.isValid() );
    Q_ASSERT( m_interval== 0 ? m_repeat==0 : true );
}


EventMessage::EventMessage(QByteArray data) :
    Message()
{
    QDataStream stream(data);
    stream >> m_signalName;
    stream >> m_timestamp;
    stream >> m_interval;
    stream >> m_repeat;

    Q_ASSERT( !m_signalName.isEmpty() );
    Q_ASSERT( m_timestamp.isValid() );
    Q_ASSERT( m_interval==0 ? m_repeat==0 : true);
}


EventMessage::~EventMessage()
{
}


EventMessage::EventMessage(const EventMessage &other) :
    Message(),
    m_signalName(other.signalName()),
    m_timestamp(other.timestamp()),
    m_interval(other.interval()),
    m_repeat(other.repeat())
{
}


EventMessage &EventMessage::operator=(const EventMessage &other)
{
    this->m_signalName = other.signalName();
    this->m_timestamp = other.timestamp();
    this->m_interval = other.interval();
    this->m_repeat = other.repeat();
    return *this;
}


QString EventMessage::signalName() const
{
    return m_signalName;
}


QDateTime EventMessage::timestamp() const
{
    return m_timestamp;
}


quint32 EventMessage::interval() const
{
    return m_interval;
}


quint32 EventMessage::repeat() const
{
    return m_repeat;
}


QByteArray EventMessage::data() const
{
    QByteArray msg;
    QDataStream stream(&msg, QIODevice::WriteOnly);
    stream << this->signalName()
           << this->timestamp()
           << this->interval()
           << this->repeat();
    return msg;
}


QString EventMessage::string() const
{
    QString message("EventMessage:\n");
    message += QString("- Signal name: %1\n").arg(m_signalName);
    message += QString("- timestamp: %1\n").arg(m_timestamp.toString("dd-MM-yyyy hh:mm:ss"));
    message += QString("- interval(sec): %1\n").arg(m_interval);
    message += QString("- repeat: %1\n").arg(m_repeat);
    return message;
}

} // namespace Utils
