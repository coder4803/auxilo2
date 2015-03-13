#ifndef MESSAGER_H
#define MESSAGER_H

#include <QObject>

#include "signalgroup.h"

class Messager : public QObject
{
   Q_OBJECT
public:
   explicit Messager(QString mode,
                     QObject *parent = NULL);

private slots:
   void onSendMessage();
   void onMessageReceived(QByteArray data);
   void onGroupReady();

private:
   Utils::SignalGroup* m_group;
   QTimer m_timer;
   int m_counter;
};

#endif // MESSAGER_H
