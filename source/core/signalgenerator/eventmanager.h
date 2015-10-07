#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <QList>
#include <QtSql/QSqlTableModel>
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

    bool addDynamicEvent(const EventEntity& event);

    QSqlTableModel* getTableModel() const;


private:

    static const QString DB_NAME_;
    static const QString TABLE_;

    bool openDatabase(const QString& db_name);
};


}

#endif // EVENTMANAGER_H
