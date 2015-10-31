#include "doublestate.h"

namespace Core {

DoubleState::DoubleState(QString name,
                         bool persisted,
                         QObject* parent) :
   State(name, 0, persisted, parent)
{
}

bool DoubleState::setOption(const QString& name,
                            const QString& value)
{
   if (name.toLower() == "default") {
      bool ok = false;
      value.toDouble(&ok);
      if (!ok) {
         qCritical("Invalid double value: %s", value.toLatin1().data());
         return false;
      }

      setDefaultValue(value);
   } else {
      qCritical("Unknown option: %s", name.toLatin1().data());
      return false;
   }

   return true;
}

bool DoubleState::setState(const QVariant& value)
{
   bool ok = false;
   value.toDouble(&ok);
   if (!ok) {
      return false;
   }

   setValue(value);
   return true;
}

QVariant DoubleState::getState() const
{
   return value();
}

} // Core
