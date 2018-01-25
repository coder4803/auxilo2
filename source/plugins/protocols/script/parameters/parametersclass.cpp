#include <QScriptClassPropertyIterator>
#include <QScriptEngine>

#include "parametersclass.h"
#include "parametersprototype.h"

Q_DECLARE_METATYPE(Utils::ParameterSet)
Q_DECLARE_METATYPE(Utils::ParameterSet*)
Q_DECLARE_METATYPE(Plugins::ParametersClass*)

namespace Plugins {

ParametersClass::ParametersClass(QScriptEngine *engine)
    : QObject(engine), QScriptClass(engine)
{
   qScriptRegisterMetaType<Utils::ParameterSet>(engine, toScriptValue,
                                                fromScriptValue);

   m_prototype = engine->newQObject(new ParametersPrototype(this),
                                    QScriptEngine::QtOwnership,
                                    QScriptEngine::SkipMethodsInEnumeration
                                    | QScriptEngine::ExcludeSuperClassMethods
                                    | QScriptEngine::ExcludeSuperClassProperties);
   QScriptValue global = engine->globalObject();
   m_prototype.setPrototype(global.property("Object").property("prototype"));

   m_constructor = engine->newFunction(construct, m_prototype);
   m_constructor.setData(engine->toScriptValue(this));
}

ParametersClass::~ParametersClass()
{
}

QScriptValue ParametersClass::constructor() const
{
   return m_constructor;
}

QScriptValue ParametersClass::prototype() const
{
   return m_prototype;
}

QString ParametersClass::name() const
{
   return QLatin1String("Parameters");
}

QScriptClass::QueryFlags
ParametersClass::queryProperty(const QScriptValue &object,
                               const QScriptString &name,
                               QueryFlags flags,
                               uint *id)
{
   Q_UNUSED(id)

   Utils::ParameterSet* parameterSet =
           qscriptvalue_cast<Utils::ParameterSet*>(object.data());
   if (!parameterSet) {
      return 0;
   }

   // ParameterSet is read-only
   if (flags & HandlesWriteAccess) {
      flags &= ~HandlesWriteAccess;
   }

   if (parameterSet->contains(name)) {
      return flags;
   } else {
      return 0;
   }
}

QScriptValue ParametersClass::property(const QScriptValue &object,
                                       const QScriptString &name,
                                       uint id)
{
   Q_UNUSED(id)

   Utils::ParameterSet* parameterSet =
           qscriptvalue_cast<Utils::ParameterSet*>(object.data());
   if (!parameterSet) {
      return QScriptValue();
   }

   if (parameterSet->contains(name)) {
      return parameterSet->parameter<QString>(name);
   }

   return QScriptValue();
}

QScriptValue::PropertyFlags
ParametersClass::propertyFlags(const QScriptValue& object,
                               const QScriptString& name,
                               uint id)
{
   Q_UNUSED(object)
   Q_UNUSED(name)
   Q_UNUSED(id)

   return QScriptValue::ReadOnly | QScriptValue::Undeletable;
}

QScriptValue ParametersClass::newInstance(const Utils::ParameterSet& ps)
{
   QScriptValue data = engine()->newVariant(QVariant::fromValue(ps));
   return engine()->newObject(this, data);
}

QScriptValue ParametersClass::construct(QScriptContext *context,
                                        QScriptEngine *eng)
{
   Q_UNUSED(eng)

   ParametersClass* cls = qscriptvalue_cast<ParametersClass*>(
               context->callee().data());
   if (cls) {
      return cls->newInstance(Utils::ParameterSet());
   }

   return QScriptValue();
}

QScriptValue ParametersClass::toScriptValue(QScriptEngine *eng,
                                            const Utils::ParameterSet &ps)
{
   QScriptValue constructor = eng->globalObject().property("Parameters");
   ParametersClass *cls = qscriptvalue_cast<ParametersClass*>(constructor.data());
   if (cls) {
      return cls->newInstance(ps);
   }

   return eng->newVariant(QVariant::fromValue(ps));
}

void ParametersClass::fromScriptValue(const QScriptValue &obj,
                                      Utils::ParameterSet &ps)
{
   ps = qvariant_cast<Utils::ParameterSet>(obj.toVariant());
}

} // Plugins
