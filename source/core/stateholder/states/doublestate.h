#include "state.h"

#ifndef DOUBLESTATE_H
#define DOUBLESTATE_H

namespace Core {

/*!
 * \brief The DoubleState class.
 * This class describes a double state.
 */
class DoubleState : public State
{
public:
   /*!
    * \brief Constructor.
    * \param name State name.
    * \param persisted If state shoud be persisted.
    * \param parent Parent object.
    */
   DoubleState(QString name,
               bool persisted,
               QObject* parent = NULL);

   /*!
    * \brief Checks and sets options for double state.
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

#endif // DOUBLESTATE_H
