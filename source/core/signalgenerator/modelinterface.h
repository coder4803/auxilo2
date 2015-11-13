/* modelinterface.h
 *
 * This header defines the ModelInterface interface, that is used to
 * communicate with the application business logic.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 13-Nov-2015
 */

#ifndef MODELINTERFACE_H
#define MODELINTERFACE_H

#include <QSqlTableModel>
#include <QSqlQueryModel>


namespace SignalGenerator
{

/*!
 * \brief The ModelInterface class
 * Abstract interface to communicate with application's
 * business logic.s
 */
class ModelInterface
{
public:

    //! Destructor.
    virtual ~ModelInterface() {}

    /*!
     * \brief Get model of database's event table.
     * \return Table model, that contains all events in the database.
     *  The ownership is not transfered.
     */
    virtual QSqlTableModel* getEventTable() = 0;

    /*!
     * \brief Get next occuring events.
     * \return Query model, that contains events ordered by occurance time.
     *  Ownership is not transfered.
     */
    virtual QSqlQueryModel* getTaskList() = 0;
};

}

#endif // MODELINTERFACE_H

