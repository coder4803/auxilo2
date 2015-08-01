#include "state.h"

#ifndef BOOLEANSTATE_H
#define BOOLEANSTATE_H

namespace Core {

/*!
 * \brief The BooleanState class.
 * This class describes a boolean state.
 */
class BooleanState : public State
{
public:
   /*!
    * \brief Constructor.
    * \param name State name.
    * \param persisted If state shoud be persisted.
    * \param parent Parent object.
    */
   BooleanState(QString name,
                bool persisted,
                QObject* parent = NULL);

   /*!
    * \brief Checks and sets options for boolean state.
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

private:
   //! List of strings that mean true.
   static const QStringList TRUE_STRINGS;

   //! List of strings that mean false.
   static const QStringList FALSE_STRINGS;
};

} // Core

#endif
