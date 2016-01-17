/* businesslogic.h
 *
 * This header defines the BusinessLogic class. This class realises the
 * ModelInterface and manages all components necessary for the application's
 * core functionality.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 13-Nov-2015
 */

#ifndef BUSINESSLOGIC_H
#define BUSINESSLOGIC_H

#include <QObject>
#include "modelinterface.h"
#include "confreader.h"
#include "eventmanager.h"
#include "eventreceiver.h"


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

    /*!
     * \brief Constructor.
     * \param clearEvents if true, then database is cleared from all events.
     * \param address Server address.
     */
    BusinessLogic(bool clearEvents, QObject* parent = 0);

    //! Destructor.s
    virtual ~BusinessLogic();

    // Implements the ModelInterface
    virtual QSqlTableModel* getEventTable();
    virtual QSqlQueryModel* getTaskList();


private slots:

    /*!
     * \brief Responses on new configuration response message.
     */
    void onConfigurationReceived();

private:

    ConfReader confReader_;
    EventManager eventManager_;
    EventReceiver receiver_;
};

}

#endif // BUSINESSLOGIC_H
