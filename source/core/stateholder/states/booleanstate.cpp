#include "booleanstate.h"

namespace Core {

const QStringList BooleanState::TRUE_STRINGS(
      QStringList() << "1" << "true" << "on");

const QStringList BooleanState::FALSE_STRINGS(
      QStringList() << "0" << "false" << "off");

BooleanState::BooleanState(QString name,
                           bool persisted,
                           QObject* parent) :
   State(name, false, persisted, parent)
{
}

bool BooleanState::setOption(const QString name,
                             const QString value)
{
   // Default option is used to set states default value at the beginning.
   if (name.toLower() == "default") {
      if (TRUE_STRINGS.contains(value)) {
         setDefaultValue(true);
      } else if (FALSE_STRINGS.contains(value)) {
         setDefaultValue(false);
      } else {
         qCritical("Invalid bool value: %s", value.toLatin1().data());
         return false;
      }
   } else {
      qCritical("Unknown option: %s", name.toLatin1().data());
      return false;
   }

   return true;
}

bool BooleanState::setState(const QVariant& value)
{
   if (value.type() == QVariant::Bool) {
      setValue(value);
      return true;
   }

   return false;
}

QVariant BooleanState::getState() const
{
   return value();
}

} // Core
