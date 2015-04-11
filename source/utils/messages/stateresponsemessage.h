#include <QHash>
#include <QVariant>

#include "message.h"

#ifndef UTILS_STATERESPONSEMESSAGE_H
#define UTILS_STATERESPONSEMESSAGE_H

namespace Utils {

/*!
 * \brief The StateResponseMessage class
 * This class can be used to create and parse state response messages.
 */
class StateResponseMessage : public Message
{
public:
   /*!
    * \brief The State struct
    * State represents one requested state.
    */
   struct State
   {
      /*!
       * \brief Creates state from scratch.
       * \param Available Does state exist.
       * \param Value State's value.
       * \param DevicesAreUpToDate Are linked devices up to date.
       */
      State(bool Available,
            QVariant Value = QVariant(),
            bool DevicesAreUpToDate = false) :
         available(Available),
         value(Value),
         devicesAreUpToDate(DevicesAreUpToDate)
      {}

      bool available;          //!< Does state exist.
      QVariant value;          //!< State's value.
      bool devicesAreUpToDate; //!< Are linked devices up to date.
   };

   /*!
    * \brief Constructor.
    */
   StateResponseMessage();

   /*!
    * \brief Constructor.
    * This constructor is used to create message from received binary data.
    * \param payload Received binary data.
    */
   StateResponseMessage(const QByteArray& payload);

   /*!
    * \brief Destructor.
    */
   ~StateResponseMessage();

   /*!
    * \brief Appends new state to response.
    * \param name State's name.
    * \param state State's state.
    */
   void appendState(QString name, State state);

   /*!
    * \brief Chekcs if list contains certain state.
    * \param name State's name.
    * \return True if response contains the state, otherwise false.
    */
   bool contains(QString name) const { return m_states.contains(name); }

   /*!
    * \brief Returns state's state.
    * \param name State's name.
    * \return State's state.
    */
   State state(QString name) const;

   /*!
    * \brief Returns hash of states.
    * \return Hash of states.
    */
   const QHash<QString, State*>& states() const { return m_states; }

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
   QHash<QString, State*> m_states;
};

} // Utils

#endif
