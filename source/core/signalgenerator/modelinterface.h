#ifndef MODELINTERFACE_H
#define MODELINTERFACE_H

#include <QSqlTableModel>
#include <QSqlQueryModel>


namespace SignalGenerator
{

class ModelInterface
{
public:

    virtual ~ModelInterface() {}

    virtual QSqlTableModel* getEventTable() = 0;

    virtual QSqlQueryModel* getTaskList() = 0;
};

}

#endif // MODELINTERFACE_H

