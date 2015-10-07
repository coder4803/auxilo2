/* evententity.cpp
 * This is the implementation file for the SignalGenerator::EventEntity struct
 * defined in evententity.h.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 28-Sep-2015
 */

#include "evententity.h"
#include <QStringList>

namespace SignalGenerator
{

EventEntity::EventEntity() :
    signal(), timestamp(), interval(), repeat(0)
{
}


EventEntity::EventEntity(const QString &s, const QDateTime &ts,
                         const QString &i, unsigned r) :
    signal(s), timestamp(ts), interval(i), repeat(r)
{
}


bool EventEntity::isValidInterval(QString &interval)
{
    static const QStringList ALLOWED_UNITS =
    {
        "second", "minute", "hour", "day", "week", "month", "year"
    };

    if (interval.endsWith("s") ){
        interval = interval.remove(interval.length()-1, 1);
    }
    QStringList fields = interval.split(" ");
    if (fields.size() != 2) return false;

    // Check factor
    bool ok(false);
    fields.at(0).toUInt(&ok);
    if (!ok) return false;

    // Check unit.
    if (ALLOWED_UNITS.contains( fields.at(1) ) ){
        return true;
    }
    return false;
}


QDateTime EventEntity::parseDateTime(const QString &str)
{
    QDateTime result = QDateTime::fromString(str, "dd-MM-yyyy hh:mm:ss");
    if ( !result.isValid() ){
        result.setDate( QDate::currentDate() );
        QTime time = QTime::fromString(str, "hh:mm:ss");
        if (time.isValid()){
            result.setTime(time);
        }
        else return QDateTime();
    }
    return result;
}


bool EventEntity::operator==(const EventEntity &other) const
{
    return this->signal == other.signal &&
            this->timestamp == other.timestamp &&
            this->interval == other.interval &&
            this->repeat == other.repeat;
}

} // Namespace SignalGenerator
