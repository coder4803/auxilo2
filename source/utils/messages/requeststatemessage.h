#include <QStringList>

#include "message.h"

#ifndef UTILS_REQUESTSTATEMESSAGE_H
#define UTILS_REQUESTSTATEMESSAGE_H

namespace Utils {

/*!
 * \brief The RequestStateMessage class.
 * This class can be used to create and parse State request messages.
 */
class RequestStateMessage : public Message
{
public:
   /*!
    * \brief Constructor.
    * This constructor is used to create state message from scratch.
    * \param responseGroup Name of group to send state response.
    * \param state Name of requested state.
    */
   RequestStateMessage(QString responseGroup,
                       QString state);

   /*!
    * \brief Constructor.
    * This constructor is used to create state message from scratch.
    * \param responseGroup Name of group to send state response.
    * \param states List of states to request.
    */
   RequestStateMessage(QString responseGroup,
                       QStringList states);

   /*!
    * \brief Constructor.
    * This constructor is used to parse state request message from
    * received binary data.
    * \param data Received binary data.
    */
   RequestStateMessage(QByteArray data);

   /*!
    * \brief Appends state to state request list.
    * \param state Name of state to request.
    */
   void appendState(QString state);

   /*!
    * \brief Appends list of states to state request list.
    * \param states List of states to request.
    */
   void appendStates(QStringList states);

   /*!
    * \brief Returns response group name.
    * \return Group name to send state response.
    */
   QString responseGroup() { return m_responseGroup; }

   /*!
    * \brief Returns list of requested states.
    * \return List of requested states.
    */
   const QStringList& states() { return m_states; }

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
   QString m_responseGroup;
   QStringList m_states;
};

} // Utils

#endif
