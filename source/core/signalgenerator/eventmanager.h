#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <QList>
#include <QtSql/QSqlTableModel>
#include <memory>
#include "evententity.h"

namespace SignalGenerator
{


class EventManager
{
public:

    EventManager();

    ~EventManager();

    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;

    bool setStaticEvents(const QList<EventEntity>& events);

    bool clearStaticEvents();

    bool clearAll();

    bool addDynamicEvent(const EventEntity& event);

    QSqlTableModel* getTableModel() const;

    QSqlQueryModel* getTaskList();

    bool isValid() const;


private:

    static const QString DB_NAME_;
    static const QString TABLE_;

    std::unique_ptr<QSqlTableModel> model_;
    std::unique_ptr<QSqlQueryModel> taskList_;

    bool openDatabase(const QString& db_name);

    bool clearExpiredEvents();

    QDateTime findNextTimestamp(QDateTime dt,
                                int& repeat, const QString& interval) const;

    bool updateExpired(const QList<int>& toBeRemoved,
                  const QMap<int, QPair<QString,int>>& toBeUpdated);
};


}

#endif // EVENTMANAGER_H
