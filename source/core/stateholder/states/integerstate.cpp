#include "integerstate.h"

namespace Core {

IntegerState::IntegerState(QString name,
                           bool persisted,
                           QObject* parent) :
   State(name, 0, persisted, parent)
{
}

bool IntegerState::setOption(const QString name,
                             const QString value)
{
   if (name.toLower() == "default") {
      bool ok = false;
      value.toLongLong(&ok);
      if (!ok) {
         qCritical("Invalid integer value: %s", value.toLatin1().data());
         return false;
      }

      setDefaultValue(value);
   } else {
      qCritical("Unknown option: %s", name.toLatin1().data());
      return false;
   }

   return true;
}

bool IntegerState::setState(const QVariant& value)
{
   bool ok = false;
   value.toLongLong(&ok);
   if (!ok) {
      return false;
   }

   setValue(value);
   return true;
}

QVariant IntegerState::getState() const
{
   return value();
}

} // Core
