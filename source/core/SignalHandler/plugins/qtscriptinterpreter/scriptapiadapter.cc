/* scriptapiadapter.cc
 * 
 * This is the implementation file for the ScriptApiAdapter class defined
 * in scriptapiadapter.hh.
 * 
 * Author: Perttu Paarlahti:    perttu.paarlahti@gmail.com
 */

#include "scriptapiadapter.hh"
#include <QDebug>

// Register custom types, so they can be converted into QScriptValue.
Q_DECLARE_METATYPE(Utils::StateResponseMessage::State)

namespace QtScriptPlugin
{

ScriptApiAdapter::ScriptApiAdapter(SignalHandler::ScriptAPI* api,
                                   QScriptEngine* engine,
                                   QObject* parent):
    QObject(parent), api_(api), eng_(engine)
{
    Q_ASSERT(api != nullptr);
    Q_ASSERT(engine != nullptr);
    
    qScriptRegisterMetaType(eng_, &toScriptValue, &fromScriptValue);
}


ScriptApiAdapter::~ScriptApiAdapter()
{    
}


QDateTime ScriptApiAdapter::dateTimeNow() const
{
    return api_->dateTimeNow();
}

QScriptValue ScriptApiAdapter::getStateOf(const QString& stateName)
{
    Utils::StateResponseMessage::State st = api_->getStateOf(stateName);
    return eng_->toScriptValue(st);
}


QScriptValue ScriptApiAdapter::getStates(const QStringList& states)
{   
    // Convert ScriptAPI::StateMap into QVariantMap
    SignalHandler::ScriptAPI::StateMap sts = api_->getStates(states);
    QVariantMap variants;
    for (auto it = sts.begin(); it != sts.end(); ++it){
        variants.insert(it.key(), QVariant::fromValue(it.value()) );
    }
    
    // Convert to QScriptValue
    return eng_->toScriptValue(variants);
}


int ScriptApiAdapter::setState(const QString& stateName, 
                                      const QVariant& value)
{
    return api_->setState(stateName, value);
}


int ScriptApiAdapter::sendSignal(const QString& signalName,
                                        const QStringList& args)
{
    return api_->sendSignal(signalName, args);
}

} //Namespace QtScriptPlugin

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
