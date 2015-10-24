/* eventmanager.h
 *
 * This header defines the SignalGenerator::EventManager class that is
 * responsiple for storeing and generateing events.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 25-Oct-2015
 */

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <QObject>
#include <QList>
#include <QSqlTableModel>
#include <QTimer>
#include <memory>
#include "evententity.h"

namespace SignalGenerator
{

/*!
 * \brief The EventManager class
 *  This class stores events and generates them on time.
 */
class EventManager : public QObject
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor
     * \pre None.
     * \post Opens the event database and clears static and expired events
     *  from it. If construction fails, isValid returns false.
     */
    EventManager();

    /*!
     * \brief Destructor
     * \post Closes the event database.
     */
    virtual ~EventManager();

    /*!
     * \brief Replace static events with new set of static events.
     * \param events New set of static events.
     * \return True, if operation was successful.
     * \pre Object is in a valid state (isValid() == true).
     */
    bool setStaticEvents(const QList<EventEntity>& events);

    /*!
     * \brief Remove all static events from the events table.
     * \return True, if operation was successful.
     */
    bool clearStaticEvents();

    /*!
     * \brief Remove all events from the events table, including dynamic events.
     * \return True, if operation was successful.
     */
    bool clearAll();

    /*!
     * \brief Insert new dynamic event into the events table.
     * \param event Event to be inserted.
     * \return True, if operation was successful.
     */
    bool addDynamicEvent(const EventEntity& event);

    /*!
     * \brief Get model of the events-table.
     * \return Pointer to the table model. Ownership is not transfered.
     */
    QSqlTableModel* getTableModel() const;

    /*!
     * \brief Get model of next expiring events.
     * \return QSqlQueryModel of events ordered by timestamp.
     */
    QSqlQueryModel* getTaskList();

    /*!
     * \brief Check if EventManager is in a valid state.
     * \return True, if the EventManager is in a valid state.
     */
    bool isValid() const;


private slots:

    /*!
     * \brief Slot that responses to internal timer timeout, and generates
     *  events if the timestamp matches.
     */
    void onTimeout();


private:

    static const QString DB_NAME_;
    static const QString TABLE_;

    std::unique_ptr<QSqlTableModel> model_;
    std::unique_ptr<QSqlQueryModel> taskList_;
    QList<int> nextEvents_;
    QDateTime nextTimestamp_;
    QTimer timer_;

    bool openDatabase(const QString& db_name);

    bool clearExpiredEvents();

    QDateTime findNextTimestamp(QDateTime dt,
                                int& repeat, const QString& interval) const;

    bool updateExpired(const QList<int>& toBeRemoved,
                  const QMap<int, QPair<QString,int>>& toBeUpdated);

    void findNextEvents();

    void generate(int id) const;
};

} // Namespace SignalGenerator

#endif // EVENTMANAGER_H
