#include "stringstate.h"

namespace Core {

StringState::StringState(QString name,
                         bool persisted,
                         QObject* parent) :
   State(name, 0, persisted, parent)
{
}

bool StringState::setOption(const QString& name,
                            const QString& value)
{
   if (name.toLower() == "default") {
      setDefaultValue(value);
   } else {
      qCritical("Unknown option: %s", name.toLatin1().data());
      return false;
   }

   return true;
}

bool StringState::setState(const QVariant& value)
{
   setValue(value);
   return true;
}

QVariant StringState::getState() const
{
   return value();
}

} // Core
