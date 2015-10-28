#include "message.h"

#ifndef UTILS_SIGNALACKMESSAGE_H
#define UTILS_SIGNALACKMESSAGE_H

namespace Utils {

/*!
 * \brief The SignalAckMessage class.
 * This class can be used to create and parse SignalAck messages.
 */
class SignalAckMessage : public Message
{
   Q_GADGET
   Q_ENUMS(Result)
public:
   /*!
    * \brief Result of executing signal.
    */
   enum Result {
      SUCCEEDED,          //!< Signal was executed successfully.
      SIGNAL_NOT_FOUND,   //!< Signal was not found.
      INVALID_PARAMETERS, //!< Signal had invalid parameters.
      FAILED              //!< Failed because of some other reason.
   };

   /*!
    * \brief Constructor.
    * Creates Acknowledge message for Signal message.
    * \param ackId Acknowledge id.
    * \param result Result of signal execution.
    */
   SignalAckMessage(quint32 ackId,
                    Result result);

   /*!
    * \brief Constructor.
    * This is used to parse SignalAckMessage from binary data.
    * \param data Received binary data.
    */
   SignalAckMessage(QByteArray data);

   //! Copy-constructor
   SignalAckMessage(const SignalAckMessage& other);

   //! Copy-assignment
   SignalAckMessage operator=(const SignalAckMessage& other);

   /*!
    * \brief Returns acknowledge id.
    * \return Acknowledge id.
    */
   quint32 ackId() const { return m_ackId; }

   /*!
    * \brief Returns result of acknowledge.
    * \return Acknowledge result.
    */
   Result result() const { return m_result; }

   /*!
    * \brief Returns message in binary format.
    * \return Message in binary format.
    */
   QByteArray data() const;

   /*!
    * \brief Returns message in string format.
    * This is useful when debugging.
    * \return Message in string format.
    */
   QString string() const;

private:
   quint32 m_ackId;
   Result m_result;
};

} // Utils

#endif
