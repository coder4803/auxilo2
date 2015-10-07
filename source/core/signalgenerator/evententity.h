/* evententity.h
 * This header defines the SignalGenerator::EventEntity struct that represents
 * one timed event.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 28-Sep-2015
 */

#ifndef EVENTENTITY_H
#define EVENTENTITY_H

#include <QString>
#include <QDateTime>

namespace SignalGenerator
{

/*!
 * \brief The EventEntity struct
 *  The data type to represent an timed event.
 */
struct EventEntity
{
    QString signal;
    QDateTime timestamp;
    QString interval;
    unsigned int repeat;

    /*!
     * \brief Default constructor.
     */
    EventEntity();

    /*!
     * \brief Constructor.
     * \param s Signal
     * \param ts Timestamp
     * \param i Interval
     * \param r Repeat.
     * \pre None
     * \post EventEntity fields are set according to the parameters.
     *  Note that no validaty checks are made.
     */
    EventEntity(const QString& s, const QDateTime& ts,
                const QString& i = QString(), unsigned r = 0);

    /*!
     * \brief Check if given string is a valid time interval.
     * \param interval String representation.
     * \return True, if given string is a valid time interval.
     * \pre None.
     * \post Converts interval in singular form.
     */
    static bool isValidInterval(QString& interval);

    /*!
     * \brief Convert a QDateTime object from a string.
     * \param str String representation.
     * \return A QDateTime object constructed from the string.
     *  If str is not a valid string representation, returns an invalid
     *  QDateTime object.
     * \pre None.
     */
    static QDateTime parseDateTime(const QString& str);

    /*!
     * \brief compare for equality.
     * \param other Object to compare to.
     * \return True, if objects are considered equal i.e. their all fields
     *  match each other.
     * \pre None.
     */
    bool operator==(const EventEntity& other) const;
};


} // Namespace Signalgenerator

#endif // EVENTENTITY_H
