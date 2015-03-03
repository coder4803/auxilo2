#include <QObject>
#include <QString>
#include <QByteArray>

#include "amqp/amqp.h"
#include "amqp/amqp_exchange.h"
#include "amqp/amqp_queue.h"

#include "connection/connection.h"

#ifndef SYSTEM_SIGNALGROUP_H
#define SYSTEM_SIGNALGROUP_H

namespace System {

enum SignalGroupType {
   Publisher,
   Subscriber,
   Both
};

class SignalGroup : public QObject
{
   Q_OBJECT
public:
   SignalGroup(QString name, SignalGroupType type, QObject* parent = NULL);
   ~SignalGroup();

   void publish(const QByteArray& data);

signals:
   void messageReceived(QByteArray data, QString group);
   void ready();

private slots:
   void onQueueDeclared();
   void onExchangeDeclared();
   void onMessageReceived(QAMQP::Queue* queue);

private:
   bool establishConnection();
   void enableEmitting();
   void enableReceiving();

   QAMQP::Client* m_client;
   QAMQP::Exchange* m_exchange;
   QAMQP::Queue* m_queue;

   QString m_name;
   SignalGroupType m_type;

   bool m_queueReady;
   bool m_exchangeReady;
};

}

#endif
