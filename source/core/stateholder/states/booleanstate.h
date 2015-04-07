#include "state.h"

#ifndef BOOLEANSTATE_H
#define BOOLEANSTATE_H

namespace Core {

class BooleanState : public State
{
public:
   BooleanState(QString name,
                bool persisted,
                QObject* parent = NULL);

   bool setOption(const QString name,
                  const QString value);

   bool setState(const QVariant& state);
   QVariant getState() const;

private:
   static const QStringList TRUE_STRINGS;
   static const QStringList FALSE_STRINGS;
};

} // Core

#endif
