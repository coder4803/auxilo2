#include "message.h"

#ifndef UTILS_SETSTATEACKMESSAGE_H
#define UTILS_SETSTATEACKMESSAGE_H

namespace Utils {

/*!
 * \brief The SetStateAckMessage class
 * This class can be used to create and parse SetStateAck messages.
 */
class SetStateAckMessage : public Message
{
public:
   /*!
    * \brief Result of setting state.
    */
   enum Result {
      SUCCEEDED,     //!< Setting state succeeded.
      ILLEGAL_VALUE, //!< Tried to set illegal value.
      FAILED         //!< Failed because of some other reason.
   };

   /*!
    * \brief Constructor.
    * This Constructor is used to create state set message from scratch.
    * \param ackId Acknowledge id.
    * \param result Result of setting state.
    */
   SetStateAckMessage(quint32 ackId,
                      Result result);

   /*!
    * \brief Constructor.
    * This constructor is used to create message from received binary data.
    * \param data Received binary data.
    */
   SetStateAckMessage(QByteArray data);

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
