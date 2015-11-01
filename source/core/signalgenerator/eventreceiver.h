#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <QObject>
#include "eventmanager.h"
#include "messagegroup.h"
#include "eventmessage.h"


namespace SignalGenerator
{

/*!
 * \brief The EventReceiver class
 *  This class receives and handles incoming EventMessages.
 */
class EventReceiver : public QObject
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor.
     * \param manager Pointer to the EventManager.
     * \param parent QObject's parent.
     * \pre manager != nullptr.
     * \post EventReceiver is constructed, but will not receive messages until
     *  start() is called.
     */
    EventReceiver(EventManager* manager, QObject* parent=0);

    /*!
     * \brief Destructor.
     */
    virtual ~EventReceiver();

    /*!
     * \brief Start receiving EventMessages.
     * \pre The EventReceiver is not started (isStarted() == false).
     * \post EventReceiver is ready to receive EventMessages.
     */
    void start();

    /*!
     * \brief Checks if EventReceiver is currently ready to receive messages.
     * \return True, if EventReceiver is started.
     */
    bool isStarted() const;

    /*!
     * \brief Stop receiving EventMessages.
     * \pre None.
     * \post EventReceiver will not receive EventMessages until
     *  start() is called again.
     */
    void stop();


private slots:

    /*!
     * \brief Handles incoming EventMessages.
     * \param data EventMessage's binary representation.
     */
    void onEventMessageReceived(QByteArray data);


private:

    EventManager* manager_;
    Utils::MessageGroup* group_;
};

} // namespace SignalGenerator

#endif // EVENTRECEIVER_H
