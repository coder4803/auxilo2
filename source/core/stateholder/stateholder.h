#include <QObject>
#include <QHash>
#include <QString>

#include <messagegroup.h>

#include "states/state.h"

#ifndef STATEHOLDER_H
#define STATEHOLDER_H

namespace Core {

class StateHolder : public QObject
{
   Q_OBJECT
public:
   StateHolder(QString serverAddress, QObject* parent = NULL);
   ~StateHolder();

private slots:
   void handleConfResponseMessage(QByteArray payload);
   void handleSetStateMessage(QByteArray payload);
   void handleRequestStateMessage(QByteArray payload);
   void handleStateChangedAckMessage(QByteArray payload);

   void requestParameters();
   void updateStates();

private:
   static const QString PARAMETER_CONFPATH;
   static const QString DEFAULT_CONF_FILE;

   static const quint32 STATE_CHECK_INTERVAL;

   static const quint32 CONF_REQUEST_INTERVAL;
   static const QString CONF_RESPONSE_GROUP;
   static const QString FEATURE_NAME;

   static const QString DATABASE_CONNECTION_NAME;

   QTimer m_confRequestTimer;

   QString m_configPath;

   Utils::MessageGroup* m_confResponseGroup;
   Utils::MessageGroup* m_setStateGroup;
   Utils::MessageGroup* m_stateRequestGroup;
   Utils::MessageGroup* m_stateChangedAckGroup;

   QHash<QString, State*> m_states;
   QTimer m_updateTimer;

   bool loadStateConfigurations();
   bool openPersistentDatabase();
   bool initStateHolder();
};

} // Core

#endif
