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
        return;
    }
    if (!this->clearStaticEvents() ){
        QSqlDatabase::database(DB_NAME_).close();
        return;
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
        QString interval = e.interval.isEmpty() ? "NULL" : "\"" + e.interval + "\"";
        QString repeat = e.repeat==0 ? "NULL" : QString::number(e.repeat);

        QSqlQuery q("INSERT INTO " + TABLE_ + " VALUES(" +
                    "\"" + e.signal + "\", " +
                    "\"" + e.timestamp.toString("dd-MM-yyyy hh:mm:ss") + "\", " +
                    interval + ", " + repeat + ", " + "1);");

        if (q.lastError().type() != QSqlError::NoError){
            qCritical() << "Failed to set static events:"
                        << q.lastError().text().toLatin1().data();
            return false;
        }
    }

    model_->select();
    return true;
}


bool EventManager::clearStaticEvents()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q("DELETE FROM " + TABLE_ + " WHERE (static == 1);", db);

    if (!q.exec()){
        qCritical() << "Failed to clear static events:"
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

    QSqlQuery q("INSERT INTO " + TABLE_ + " VALUES(" +
                "\"" + event.signal + "\", " +
                "\"" + event.timestamp.toString("dd-MM-yyyy hh:mm:ss") + "\", " +
                interval + ", " + repeat + ", " + "0);");

    if (!q.exec()){
        qCritical() << "Failed to insert dynamic event:"
                    << q.lastError().text().toLatin1().data();
        return false;
    }

    model_->select();
    return true;
}


QSqlTableModel *EventManager::getTableModel() const
{
    return model_.get();
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
                 "signal    TEXT    NOT NULL, "
                 "timestamp TEXT    NOT NULL, "
                 "interval  TEXT, "
                 "repeat    INTEGER, "
                 "static    INTEGER NOT NULL, "
                 "PRIMARY KEY(signal, timestamp, interval, repeat)"
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

}
