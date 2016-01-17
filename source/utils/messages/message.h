#include <QObject>

#ifndef UTILS_MESSAGE_H
#define UTILS_MESSAGE_H

namespace Utils {

/*
 * Here is a list of constants which define message group names
 */

/*!
 * \brief Name of group for sending configuration requests.
 * This group is listened by configuration manager. Manager will handle the
 * message and send response to a group that is specified in the request
 * message.
 */
const QString CONF_REQUEST_GROUP("confRequest");

/*!
 * \brief Name of group for sending configuration response to State Holder.
 * This group is listened by State Holder. This could be defined in State
 * Holder source code but we want use it also in MessageTool so it is defined here.
 */
const QString SH_CONF_RESPONSE_GROUP("stateHolderConf");

/*!
 * \brief Name of group for sending set state messages.
 * This group is listened by state holder. State holder will set state value
 * according to message information.
 */
const QString SET_STATE_GROUP("setState");

/*!
 * \brief Name of group for sending state requests.
 * This group is listened by state holder. State holder will handle the message
 * and send response to a group that is specified in the request message.
 */
const QString REQUEST_STATE_GROUP("requestState");

/*!
 * \brief Name of group for sending state changed acknowledges.
 * This group is listened by State Holder. This could be defined in State
 * Holder source code but we want use it also in MessageTool so it is defined here.
 */
const QString SH_STATE_CHANGED_ACK_GROUP("stateChangedAck");

/*!
 * \brief Name of group for sending signals to system.
 * This group is listened by signal handler. Signal handler will handle the
 * message and send ack message if required.
 */
const QString SIGNAL_HANDLER_GROUP("signalHandler");

/*!
 * \brief Group name of signal handler confresponse messages.
 */
const QString SIGNAL_HANDLER_CONF("signalHandlerConf");

/*!
 * \brief Name of group for sending dynamic events.
 *  This group is listened by Signal Generator. Only EventMessages are allowed
 *  in this group.
 */
const QString SIGNAL_GENERATOR_GROUP("signalGenerator");

/*!
 * \brief Name of group for sending log messages.
 * This group is listened by message logger. Logger will handle message
 * according to its configurations.
 */
const QString LOG_MESSAGE_GROUP("writeLog");

/*!
 * \brief Name of group for sending configuration responses to Interface.
 * This group is listened by Interface. This could be defined in Interface
 * source code but we want use it also in MessageTool so it is defined here.
 */
const QString INTERFACE_CONF_RESPONSE_GROUP("interfaceConf");

/*!
 * \brief Post fix for device signal group name. Each device listen for
 * signal from <DEVICE_NAME><SIGNAL_POST_FIX> group.
 */
const QString SIGNAL_POST_FIX("_signal");

/*!
 * \brief Post fix for device state changed group name. Each device listen
 * for state changed messages from <DEVICE_NAME><STATE_CHANGED_POST_FIX>
 * group.
 */
const QString STATE_CHANGED_POST_FIX("_stateChanged");

/*!
 * \brief Name of group for sending device configuration responses.
 * This group is listened by Interface that controls the device. Interface
 * will deliver configurations to the device. This could be defined in
 * Interface source code but we want use it also in MessageTool so it is
 * defined here.
 */
const QString CONF_RESPONSE_POST_FIX("_confResponse");

/*!
 * \brief Post fix for devices state response group name.
 * This group is listened by Interface that controls the device. Interface
 * will deliver response to the device. This could be defined in Interface
 * source code but we want use it also in MessageTool so it is defined here.
 */
const QString STATE_RESPONSE_POST_FIX("_stateResponse");

/*!
 * \brief Post fix for device signal acknowledge message group name.
 * This group is listened by Interface that controls the device. Interface
 * will deliver acknowledge to the device. This could be defined in Interface
 * source code but we want use it also in MessageTool so it is defined here.
 */
const QString SIGNAL_ACK_POST_FIX("_signalAck");

/*!
 * \brief Post fix for device set state acknowledge message group name.
 * This group is listened by Interface that controls the device. Interface
 * will deliver acknowledge to the device. This could be defined in Interface
 * source code but we want use it also in MessageTool so it is defined here.
 */
const QString SET_STATE_ACK_POST_FIX("_setStateAck");

/*!
 * \brief The Message class
 * Base class for all messages between applications.
 */
class Message
{
   Q_GADGET
public:
   //! Destructor.
   virtual ~Message() {}

   /*!
    * \brief Returns message in binary format.
    * \return Message in binary format.
    */
   virtual QByteArray data() const = 0;

   /*!
    * \brief Returns message in string format.
    * This is usefull when debugging.
    * \return Message in string format.
    */
   virtual QString string() const = 0;

protected:
   /*!
    * \brief Returns unique acknowledge id.
    * This id is used to link acknowledge message
    * to its original "parent" messages.
    * \return Unique acknowledge id.
    */
   quint32 nextAckId() { return ++m_ackIdCounter; }

private:
   static quint32 m_ackIdCounter;
};

} // Utils
#endif
