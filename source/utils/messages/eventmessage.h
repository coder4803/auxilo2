/* eventmessage.h
 *
 * This header defines the EventMessage class, that can be used to send
 * dynamic events to the signalgenerator.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 25-Oct-2015
 */

#ifndef EVENTMESSAGE_H
#define EVENTMESSAGE_H

#include "message.h"
#include <QDateTime>

namespace Utils
{

/*!
 * \brief The EventMessage class
 * This message class can be used to set dynamic envents for the
 * signalgenerator.
 */
class EventMessage : public Message
{
public:

    /*!
     * \brief Constructor. Constructs EventMessage from scratch.
     * \param signal Signal name.
     * \param timestamp First timestamp.
     * \param interval_sec Repetition interval in seconds. 0 means no repeat.
     * \param repeat Repetition times. 0 means infinite repeat or no repeat
     *  depending on interval.
     * \pre timestamp is a valid QDateTime. signal is not an empty string.
     *  if interval == 0, then repeat must be 0 too.
     * \post Event message is constructed and has the given values as its
     *  attributes.
     */
    EventMessage(const QString& signal,
                 const QDateTime& timestamp,
                 quint32 interval_sec,
                 quint32 repeat);

    /*!
     * \brief Constructor. Constructs EventMessage from a binary representation.
     * \param data Binary representation of an EventMessage.
     * \pre data represents a valid EventMessage.
     * \post EventMessage is constructed from a binary form.
     */
    EventMessage(QByteArray data);

    /*!
     * \brief Destructor.
     */
    virtual ~EventMessage();

    /*!
     * \brief Copy-constructor.
     * \param other Original EventMessage.
     * \pre None.
     */
    EventMessage(const EventMessage& other);

    /*!
     * \brief Assignment operator.
     * \param other The right hand side object.
     * \return reference to this.
     * \pre None.
     */
    EventMessage& operator=(const EventMessage& other);

    /*!
     * \brief Returns the signal name.
     * \return The signal name associated with the event.
     */
    QString signalName() const;

    /*!
     * \brief Returns the first timestamp.
     * \return Returns the event's first occurance time.
     */
    QDateTime timestamp() const;

    /*!
     * \brief Returns the event interval.
     * \return Event's occurance interval in seconds.
     */
    quint32 interval() const;

    /*!
     * \brief Returns event's repeat times.
     * \return number of event repetitions.
     */
    quint32 repeat() const;

    /*!
     * \brief Implements the Message interface.
     * \return EventMessage's binary representation.
     */
    virtual QByteArray data() const;

    /*!
     * \brief Implements the Message interface.
     * \return EventMessage's string representation.
     */
    virtual QString string() const;


private:

    QString m_signalName;
    QDateTime m_timestamp;
    quint32 m_interval;
    quint32 m_repeat;
};

} // Namespace Utils

#endif // EVENTMESSAGE_H
