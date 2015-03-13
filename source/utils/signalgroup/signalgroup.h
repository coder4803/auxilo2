#include <QObject>
#include <QString>
#include <QByteArray>

#include "amqp/amqp.h"
#include "amqp/amqp_exchange.h"
#include "amqp/amqp_queue.h"

#include "connection/connection.h"

#ifndef UTILS_SIGNALGROUP_H
#define UTILS_SIGNALGROUP_H

namespace Utils {

class SignalGroup : public QObject
{
   Q_OBJECT
   Q_DISABLE_COPY(SignalGroup)
public:
   enum GroupType {
      Publisher,
      Subscriber,
      Both
   };

   SignalGroup(QString name, GroupType type, QObject* parent = NULL);
   ~SignalGroup();

   bool publish(QByteArray data);

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
   GroupType m_type;

   bool m_queueReady;
   bool m_exchangeReady;
};

} // Utils

#endif
