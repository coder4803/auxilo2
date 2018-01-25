#include <QScriptEngine>

#include "parametersprototype.h"

Q_DECLARE_METATYPE(Utils::ParameterSet*)

namespace Plugins {

ParametersPrototype::ParametersPrototype(QObject *parent)
    : QObject(parent)
{
}

ParametersPrototype::~ParametersPrototype()
{
}

Utils::ParameterSet* ParametersPrototype::thisParameterSet() const
{
   return qscriptvalue_cast<Utils::ParameterSet*>(thisObject().data());
}

int ParametersPrototype::count() const
{
   return thisParameterSet()->parameters().count();
}

bool ParametersPrototype::contains(const QString& name) const
{
   return thisParameterSet()->contains(name);
}

QStringList ParametersPrototype::keys() const
{
   return thisParameterSet()->parameters().keys();
}

QScriptValue ParametersPrototype::parameter(const QString& key) const
{
    return thisParameterSet()->parameter<QString>(key, QString());
}

} // Plugins
