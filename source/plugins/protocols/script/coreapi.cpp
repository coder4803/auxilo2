#include "coreapi.h"
#include "script.h"

namespace Plugins {

CoreAPI::CoreAPI(QObject* parent) :
   QObject(parent),
   m_script(qobject_cast<Script*>(parent))
{
}

CoreAPI::~CoreAPI()
{
}

void CoreAPI::requestDeviceParameters()
{
   Q_ASSERT(m_script != NULL);
   emit m_script->requestDeviceParameters();
}

void CoreAPI::requestStateValue(QString stateName)
{
   Q_ASSERT(m_script != NULL);
   emit m_script->requestStateValue(stateName);
}

void CoreAPI::setStateValue(QString stateName,
                            QVariant value,
                            bool ackRequired)
{
   Q_ASSERT(m_script != NULL);
   emit m_script->setStateValue(stateName, value, ackRequired);
}

void CoreAPI::sendSignal(QString name,
                         QStringList parameters,
                         bool ackRequired)
{
   Q_ASSERT(m_script != NULL);
   emit m_script->sendSignal(name, parameters, ackRequired);
}

void CoreAPI::acknowledgeStateChange(QString stateName,
                                     Utils::StateChangedAckMessage::Result result,
                                     QVariant value)
{
   Q_ASSERT(m_script != NULL);
   emit m_script->acknowledgeStateChange(stateName, result, value);
}

void CoreAPI::sendLog(QString message,
                      Utils::LogMessage::LogType type)
{
   Q_ASSERT(m_script != NULL);
   emit m_script->sendLog(message, type);
}

} // Plugins
