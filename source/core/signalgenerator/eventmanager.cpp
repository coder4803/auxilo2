/* eventmanager.cpp
 *
 * This is the implementation file for the EventManager class defined in
 * eventmanager.h.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 25-Oct-2015
 */

#include "eventmanager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlResult>
#include <QException>
#include <QDebug>
#include "signalmessage.h"
#include "messagegroup.h"


namespace SignalGenerator
{

const QString EventManager::DB_NAME_ ("signalGenerator.db");
const QString EventManager::TABLE_ ("events");


EventManager::EventManager() :
    model_(nullptr), taskList_(nullptr), nextEvents_(), nextTimestamp_(),
    timer_(this)
{
    if (!this->openDatabase(DB_NAME_) ){
        return;
    }
    this->clearStaticEvents();
    this->clearExpiredEvents();

    connect(&timer_, SIGNAL(timeout()), this, SLOT(onTimeout()) );
    timer_.setInterval(1000);
    timer_.start();
}


EventManager::~EventManager()
{
    QSqlDatabase::database(DB_NAME_).close();
}


bool EventManager::setStaticEvents(const QList<EventEntity> &events)
{
    this->clearStaticEvents();

    foreach (EventEntity e, events) {
        QString interval = e.interval.isEmpty() ? "NULL" : "\"" + e.interval + "\"";
        QString repeat = e.repeat==0 ? "NULL" : QString::number(e.repeat);

        QSqlQuery q("INSERT INTO " + TABLE_ +
                    " (signal, timestamp, interval, repeat, static) " +
                    "VALUES(" +
                    "\"" + e.signal + "\", " +
                    "\"" + e.timestamp.toString("yyyy-MM-dd hh:mm:ss") + "\", " +
                    interval + ", " + repeat + ", " + "1);");

        if (q.lastError().type() != QSqlError::NoError){
            qCritical() << "Failed to set static events:"
                        << q.lastError().text().toLatin1().data();
            return false;
        }
    }

    this->clearExpiredEvents();
    this->findNextEvents();
    return true;
}


bool EventManager::clearStaticEvents()
{
    QSqlQuery q("DELETE FROM " + TABLE_ + " WHERE (static == 1);");

    if (q.lastError().type() != QSqlError::NoError){
        qCritical() << "Failed to clear static events:"
                    << q.lastError().text().toLatin1().data();
        return false;
    }

    model_->select();
    return true;
}


bool EventManager::clearAll()
{
    QSqlQuery q("DELETE FROM " + TABLE_ + ";");

    if (q.lastError().type() != QSqlError::NoError){
        qCritical() << "Failed to clear events:"
                    << q.lastError().text().toLatin1().data();
        return false;
    }

    model_->select();
    return true;
}


bool EventManager::addDynamicEvent(const EventEntity &event)
{
    QString interval = event.interval.isEmpty() ? "NULL" : "\"" + event.interval + "\"";
    QString repeat = event.repeat==0 ? "NULL" : QString::number(event.repeat);

    QSqlQuery q("INSERT INTO " + TABLE_ +
                " (signal, timestamp, interval, repeat, static) " +
                "VALUES(" +
                "\"" + event.signal + "\", " +
                "\"" + event.timestamp.toString("yyyy-MM-dd hh:mm:ss") +"\", "+
                interval + ", " + repeat + ", " + "0);");

    if (q.lastError().type() != QSqlError::NoError){
        qCritical() << "Failed to insert dynamic event:"
                    << q.lastError().text().toLatin1().data();
        return false;
    }

    clearExpiredEvents();
    this->findNextEvents();
    return true;
}


QSqlTableModel *EventManager::getTableModel() const
{
    return model_.get();
}


QSqlQueryModel *EventManager::getTaskList()
{
    taskList_.reset(new QSqlQueryModel);
    taskList_->setQuery("SELECT timestamp, signal, interval, repeat "
                        "FROM " + TABLE_ +
                        " ORDER BY timestamp ASC;");

    if (taskList_->lastError().type() != QSqlError::NoError){
        qCritical() << "Failed to create task list.";
        return nullptr;
    }
    return taskList_.get();
}


bool EventManager::isValid() const
{
    return QSqlDatabase::database().isValid();
}


void EventManager::onTimeout()
{
    if (!nextTimestamp_.isValid() ||
            nextTimestamp_ > QDateTime::currentDateTime() )
    {
        return;
    }
    if (nextEvents_.isEmpty()){
        return;
    }

    foreach (int id, nextEvents_) {
        this->generate(id);
    }
    this->clearExpiredEvents();
    this->findNextEvents();
}



bool EventManager::openDatabase(const QString &db_name)
{
    Q_ASSERT( !db_name.isEmpty() );
    qCritical() << "Opening database:" << db_name;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    if (!db.isValid()){
        qCritical() << "Failed to load database driver 'SQLITE'.";
        return false;
    }
    db.setDatabaseName(db_name);

    if (!db.open()){
        qCritical() << "Failed to open database" << db_name
                    << db.lastError().text().toLatin1().data();
        return false;
    }

    QSqlQuery q ("CREATE TABLE IF NOT EXISTS " + TABLE_ + "("
                 "id        INTEGER PRIMARY KEY, "
                 "signal    TEXT    NOT NULL, "
                 "timestamp TEXT    NOT NULL, "
                 "interval  TEXT, "
                 "repeat    INTEGER, "
                 "static    INTEGER NOT NULL, "
                 "UNIQUE(signal, timestamp, interval, repeat)"
                 ");", db);

    if (!q.exec()) {
       qCritical("Failed to create event table: %s.",
                 q.lastError().text().toLatin1().data());
       db.close();
       return false;
    }

    model_.reset( new QSqlTableModel );
    model_->setTable(TABLE_);
    model_->select();

    qCritical() << "Done!";
    return true;
}


bool EventManager::clearExpiredEvents()
{
    // Get events with timestamp earlier than current time.
    QDateTime current = QDateTime::currentDateTime();
    QSqlQuery q("SELECT * FROM " + TABLE_ +
                " WHERE timestamp <= \"" +
                current.toString("yyyy-MM-dd hh:mm:ss")
                + "\";");
    if (q.lastError().type() != QSqlError::NoError){
        qCritical() << "Failed to fetch expired events:"
                    << q.lastError().text().toLatin1().data();
        return false;
    }

    // Deside to eather delete or update events.
    QList<int> toBeRemoved;
    QMap<int, QPair<QString, int>> toBeUpdated;
    while (q.next()){
        QString tmp = q.value("timestamp").toString();
        QDateTime dt = QDateTime::fromString(tmp, "yyyy-MM-dd hh:mm:ss");

        bool ok(false);
        int repeat = q.value("repeat").toInt(&ok);
        if (ok && repeat==0){
            // No more repeats left.
            toBeRemoved.append( q.value("id").toInt() );
            continue;
        }
        bool inf_repeat = !ok;

        dt = this->findNextTimestamp(dt, repeat, q.value("interval").toString());
        if (dt < current){
            toBeRemoved.append( q.value("id").toInt() );
        }
        else {
            if (inf_repeat){
                repeat = -1;
            }
            toBeUpdated.insert(q.value("id").toInt(),
                               qMakePair(dt.toString("yyyy-MM-dd hh:mm:ss"), repeat));
        }
    }

    return this->updateExpired(toBeRemoved, toBeUpdated);
}


QDateTime EventManager::findNextTimestamp(QDateTime dt,
                                          int &repeat,
                                          const QString &interval) const
{
    static const QHash<QString, int> SEC_FACTOR =
    {
        {"second", 1},
        {"minute", 60},
        {"hour", 3600},
        {"day", 3600*24},
        {"week", 3600*24*7},
        {"month", 3600*24*30},
        {"year", 3600*24*365}
    };

    if (interval.isEmpty()){
        // Not repeatable
        return dt;
    }

    QStringList parts = interval.split(" ");
    int interval_sec = parts.at(0).toInt() * SEC_FACTOR[parts.at(1)];
    QDateTime current = QDateTime::currentDateTime();

    // Infinite repeat
    if (repeat == 0){
        while (dt < current){
            dt = dt.addSecs(interval_sec);
        }
        return dt;
    }

    // Finite repeat
    while (repeat > 0 && dt < current){
        dt = dt.addSecs(interval_sec);
        --repeat;
    }
    return dt;
}


bool EventManager::updateExpired(const QList<int> &toBeRemoved,
                            const QMap<int, QPair<QString,int>> &toBeUpdated)
{
    // Delete events
    foreach (int id, toBeRemoved) {
        QSqlQuery query("DELETE FROM " + TABLE_ +
                        " WHERE id = " + QString::number(id) + ";");

        if (query.lastError().type() != QSqlError::NoError){
            qCritical() << "Failed to remove expired event: "
                        << query.lastError().text().toLatin1().data();
            return false;
        }
    }

    // Update events
    for (auto it = toBeUpdated.begin(); it != toBeUpdated.end(); ++it) {
        QString repeat_str =
                it.value().second==-1 ? "NULL" : QString::number(it.value().second);

        QSqlQuery query("UPDATE "+ TABLE_ + " SET " +
                        "timestamp = " + "\"" + it.value().first + "\", " +
                        "repeat = " + repeat_str +
                        " WHERE id = " + QString::number(it.key()) + ";");

        if (query.lastError().type() != QSqlError::NoError){
            qCritical() << "Failed to update expired event: "
                        << query.lastError().text().toLatin1().data();
            return false;
        }

    }

    model_->select();
    return true;
}


void EventManager::findNextEvents()
{
    nextEvents_.clear();
    nextTimestamp_ = QDateTime();

    QSqlQuery q("SELECT id, timestamp FROM " + TABLE_ + " ORDER BY timestamp ASC;");
    if (q.lastError().type() != QSqlError::NoError){
        qCritical() << "Failed to fetch next events:"
                    << q.lastError().text().toLatin1().data();
        return;
    }
    if (!q.next()){
        // No events.
        return;
    }

    nextTimestamp_ = QDateTime::fromString(q.value("timestamp").toString(),
                                           "yyyy-MM-dd hh:mm:ss");
    nextEvents_.append( q.value("id").toInt() );

    while (q.next()){
        QDateTime dt = QDateTime::fromString(q.value("timestamp").toString(),
                                             "yyyy-MM-dd hh:mm:ss");
        if (dt > nextTimestamp_){
            break;
        }
        nextEvents_.append( q.value("id").toInt() );
    }
}


void EventManager::generate(int id) const
{
    QSqlQuery q("SELECT signal, timestamp, interval, repeat FROM "+ TABLE_ +
                " WHERE id==" + QString::number(id) + ";");
    Q_ASSERT(q.next());

    QString signalName = q.value("signal").toString();
    Utils::SignalMessage msg(signalName, "signalGenerator");
    Utils::MessageGroup::publish(msg, Utils::SIGNAL_HANDLER_GROUP);

    qDebug() << "Generated:" << id << signalName;
}

} // Namespace SignalGenerator
