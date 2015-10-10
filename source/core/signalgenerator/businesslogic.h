#ifndef BUSINESSLOGIC_H
#define BUSINESSLOGIC_H

#include <QObject>
#include "modelinterface.h"
#include "confreader.h"
#include "eventmanager.h"


namespace SignalGenerator
{

/*!
 * \brief The BusinessLogic class
 * The central logic coomponent, that encapsulates all other
 * components into one.
 */
class BusinessLogic : public QObject, public ModelInterface
{
    Q_OBJECT

public:

    BusinessLogic(bool clearEvents = false, QObject* parent = 0);

    virtual ~BusinessLogic();

    // Implements the ModelInterface
    virtual QSqlTableModel* getEventTable();
    virtual QSqlQueryModel* getTaskList();


private slots:

    void onConfigurationReceived();

private:

    ConfReader confReader_;
    EventManager eventManager_;
};

}

#endif // BUSINESSLOGIC_H
