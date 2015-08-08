#include "message.h"

#ifndef UTILS_STATECHANGEDACKMESSAGE_H
#define UTILS_STATECHANGEDACKMESSAGE_H

namespace Utils {

/*!
 * \brief The StateChangedAckMessage class
 * This class can be used to create and parse state change
 * acknowledge messages.
 */
class StateChangedAckMessage : public Message
{
public:
   /*!
    * \brief Result of changing state.
    */
   enum Result {
      SUCCEEDED,     //!< Changing state succeeded.
      INVALID_LABEL, //!< Invalid label.
      INVALID_VALUE, //!< Invalid value.
      NO_RESPONSE,   //!< Device doesn't response.
      FAILED         //!< Failed because of some other reason.
   };

   /*!
    * \brief Constructor.
    * This constructor is used to create state change message from scratch.
    * \param ackId Acknowledge id.
    * \param result Result of changing state.
    */
   StateChangedAckMessage(quint32 ackId,
                          Result result);

   /*!
    * \brief Constructor.
    * This constructor is used to create state change message from
    * received binary data.
    * \param data Received binary data.
    */
   StateChangedAckMessage(QByteArray data);

   //! Copy-constructor
   StateChangedAckMessage(const StateChangedAckMessage& other);

   //! Copy-assignment
   StateChangedAckMessage operator=(const StateChangedAckMessage& other);

   /*!
    * \brief Returns result of acknowledge.
    * \return Acknowledge result.
    */
   Result result() const { return m_result; }

   /*!
    * \brief Retuns acknowledge id.
    * \return Acknowledge id.
    */
   quint32 ackId() const { return m_ackId; }

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
