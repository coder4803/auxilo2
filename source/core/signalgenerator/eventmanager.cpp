#include "eventmanager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QException>
#include <QDebug>


namespace SignalGenerator
{

const QString EventManager::DB_NAME_ ("signalGenerator.db");
const QString EventManager::TABLE_ ("events");


EventManager::EventManager()
{
    if (!this->openDatabase(DB_NAME_) ){
        throw QException();
    }
    else if (!this->clearStaticEvents() ){
        QSqlDatabase::database(DB_NAME_).close();
        throw QException();
    }
}


EventManager::~EventManager()
{
    QSqlDatabase::database(DB_NAME_).close();
}


bool EventManager::setStaticEvents(const QList<EventEntity> &events)
{
    this->clearStaticEvents();
    foreach (EventEntity e, events) {
        QString interval = e.interval.isEmpty() ? "NULL" : e.interval;
        QString repeat = e.repeat==0 ? "NULL" : QString::number(e.repeat);

        QSqlQuery q("INSERT INTO " + TABLE_ + " VALUES(" +
                    e.signal + ", " +
                    e.timestamp.toString("dd-MM-yyyy hh:mm:ss") + ", " +
                    interval + ", " + repeat + ", " + "1);");

        if (!q.exec()){
            qCritical() << "Failed to clear static events:"
                        << q.lastError().text().toLatin1().data();
            return false;
        }
    }
    return true;
}


bool EventManager::clearStaticEvents()
{
    QSqlQuery q("DELETE FROM " + TABLE_ + " WHERE static=1;");

    if (!q.exec()){
        qCritical() << "Failed to clear static events:"
                    << q.lastError().text().toLatin1().data();
        return false;
    }
    return true;
}


bool EventManager::addDynamicEvent(const EventEntity &event)
{
    QString interval = event.interval.isEmpty() ? "NULL" : event.interval;
    QString repeat = event.repeat==0 ? "NULL" : QString::number(event.repeat);

    QSqlQuery q("INSERT INTO " + TABLE_ + " VALUES(" +
                event.signal + ", " +
                event.timestamp.toString("dd-MM-yyyy hh:mm:ss") + ", " +
                interval + ", " + repeat + ", " + "0);");

    if (!q.exec()){
        qCritical() << "Failed to insert dynamic event:"
                    << q.lastError().text().toLatin1().data();
        return false;
    }
    return true;
}


QSqlTableModel *EventManager::getTableModel() const
{
    QSqlDatabase db = QSqlDatabase::database(DB_NAME_);
    QSqlTableModel* model = new QSqlTableModel(nullptr, db);
    model->setTable(TABLE_);

    if (db.lastError().type() != QSqlError::NoError){
        qDebug() << db.lastError().text().toLatin1().data();
    }

    return model;
}


bool EventManager::openDatabase(const QString &db_name)
{
    Q_ASSERT( !db_name.isEmpty() );
    qCritical() << "Opening database:" << db_name;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLite");
    db.setDatabaseName(db_name);

    if (!db.open()){
        qCritical() << "Failed to open database" << db_name
                    << db.lastError().text().toLatin1().data();
        return false;
    }

    QSqlQuery q ("CREATE TABLE IF NOT EXIST " + TABLE_ + " ("
                 "signal    TEXT    NOT NULL,"
                 "timestamp TEXT    NOT NULL,"
                 "interval  TEXT,"
                 "repeat    INTEGER,"
                 "static    INTEGER NOT NULL,"
                 "PRIMARY_KEY(signal, timestamp, interval, repeat) );");

    if (!q.exec()) {
       qCritical("Failed to create event table: %s.",
                 db.lastError().text().toLatin1().data());
       db.close();
       return false;
    }

    qCritical() << "Done!";
    return true;
}

}
