/* scriptapiqobjectwrapper.cc
 * 
 * This is the implementation file for the ScriptApiQObjectWrapper class defined
 * in scriptapiqobjectwrapper.hh.
 * 
 * Author: Perttu Paarlahti:    perttu.paarlahti@gmail.com
 */

#include "scriptapiqobjectwrapper.hh"
#include <QDebug>

// Register custom types, so they can be converted into QScriptValue.
Q_DECLARE_METATYPE(Utils::StateResponseMessage::State)

namespace SignalHandler
{

ScriptApiQObjectWrapper::ScriptApiQObjectWrapper(ScriptAPI* api,
                                                 QScriptEngine* engine,
                                                 QObject* parent):
    QObject(parent), api_(api), eng_(engine)
{
    Q_ASSERT(api != nullptr);
    Q_ASSERT(engine != nullptr);
    
    qScriptRegisterMetaType(eng_, &toScriptValue, &fromScriptValue);
}


ScriptApiQObjectWrapper::~ScriptApiQObjectWrapper()
{    
}


QDateTime ScriptApiQObjectWrapper::dateTimeNow() const
{
    return api_->dateTimeNow();
}

QScriptValue ScriptApiQObjectWrapper::getStateOf(const QString& stateName)
{
    Utils::StateResponseMessage::State st = api_->getStateOf(stateName);
    return eng_->toScriptValue(st);
}


QScriptValue ScriptApiQObjectWrapper::getStates(const QStringList& states)
{   
    // Convert ScriptAPI::StateMap into QVariantMap
    ScriptAPI::StateMap sts = api_->getStates(states);
    QVariantMap variants;
    for (auto it = sts.begin(); it != sts.end(); ++it){
        variants.insert(it.key(), QVariant::fromValue(it.value()) );
    }
    
    // Convert to QScriptValue
    return eng_->toScriptValue(variants);
}


int ScriptApiQObjectWrapper::setState(const QString& stateName, 
                                      const QVariant& value)
{
    return api_->setState(stateName, value);
}


int ScriptApiQObjectWrapper::sendSignal(const QString& signalName,
                                        const QStringList& args)
{
    return api_->sendSignal(signalName, args);
}

} //Namespace SignalHandler

// Conversion functions
// ---------------------------------------------------------------------------

QScriptValue toScriptValue(QScriptEngine* engine, 
                           const Utils::StateResponseMessage::State& state)
{
    Q_ASSERT(engine != nullptr);
    
    QScriptValue obj = engine->newObject();
    obj.setProperty("available", state.available);
    obj.setProperty("value", engine->newVariant(state.value) );
    obj.setProperty("devicesUpToDate", state.devicesAreUpToDate);
    return obj;
}


void fromScriptValue(const QScriptValue& obj, 
                     Utils::StateResponseMessage::State& state)
{
    state.available = obj.property("available").toBool();
    state.value = obj.property("value").toVariant();
    state.devicesAreUpToDate = obj.property("devicesUpToDate").toBool();
}
