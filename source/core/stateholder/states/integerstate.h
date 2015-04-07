#include "state.h"

#ifndef INTEGERSTATE_H
#define INTEGERSTATE_H

namespace Core {

class IntegerState : public State
{
public:
   IntegerState(QString name,
                bool persisted,
                QObject* parent = NULL);

   bool setOption(const QString name,
                  const QString value);

   bool setState(const QVariant& state);
   QVariant getState() const;
};

} // Core

#endif // INTEGERSTATE_H
