#include <QObject>
#include <QVariant>

#include "statechangedackmessage.h"
#include "logmessage.h"

#ifndef COREAPI_H
#define COREAPI_H

namespace Plugins {

class Script;

/*!
 * \brief CoreAPI wraps AuxiloCore API functions for scripts.
 */
class CoreAPI : public QObject
{
   Q_OBJECT
public:
   CoreAPI(QObject* parent);
   ~CoreAPI();

   Q_INVOKABLE void requestDeviceParameters();

   Q_INVOKABLE void requestStateValue(QString stateName);

   Q_INVOKABLE void setStateValue(QString stateName,
                                  QVariant value,
                                  bool ackRequired = false);

   Q_INVOKABLE void sendSignal(QString name,
                               QStringList parameters,
                               bool ackRequired = false);

   Q_INVOKABLE void acknowledgeStateChange(
         QString stateName,
         Utils::StateChangedAckMessage::Result result,
         QVariant value);

   Q_INVOKABLE void sendLog(QString message,
                            Utils::LogMessage::LogType type);

private:
   Script* m_script;
};

} // Plugins

#endif
