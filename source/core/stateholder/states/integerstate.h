#include "state.h"

#ifndef INTEGERSTATE_H
#define INTEGERSTATE_H

namespace Core {

/*!
 * \brief The IntegerState class.
 * This class describes an integer state.
 */
class IntegerState : public State
{
public:
   /*!
    * \brief Constructor.
    * \param name State name.
    * \param persisted If state shoud be persisted.
    * \param parent Parent object.
    */
   IntegerState(QString name,
                bool persisted,
                QObject* parent = NULL);

   /*!
    * \brief Checks and sets options for integer state.
    * \param name Name of option.
    * \param value Value of option.
    * \return True if option is valid, otherwise false.
    */
   bool setOption(const QString& name,
                  const QString& value);

   /*!
    * \brief Sets state value.
    * \param state State value.
    * \return True on success, false on failure.
    */
   bool setState(const QVariant& state);

   /*!
    * \brief Returns state value.
    * \return State value.
    */
   QVariant getState() const;
};

} // Core

#endif // INTEGERSTATE_H
