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
   StateHolder(QObject* parent = NULL);
   ~StateHolder();

private slots:
   void handleSetStateMessage(QByteArray payload);
   void handleRequestStateMessage(QByteArray payload);
   void handleStateChangedAckMessage(QByteArray payload);

   void updateStates();

private:
   static const quint32 STATE_CHECK_INTERVAL;
   static const QString SET_STATE_GROUP;
   static const QString REQUEST_STATE_GROUP;

   static const QString DATABASE_CONNECTION_NAME;

   QString m_configPath;

   Utils::MessageGroup* m_setStateGroup;
   Utils::MessageGroup* m_stateRequestGroup;
   Utils::MessageGroup* m_stateChangedAckGroup;

   QHash<QString, State*> m_states;
   QTimer m_updateTimer;

   bool loadStateConfigurations();
   bool openPersistentDatabase();
};

} // Core

#endif
