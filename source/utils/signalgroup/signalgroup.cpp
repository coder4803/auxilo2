#include "signalgroup.h"

namespace System {

const QString ADDRESS("localhost:5672");

SignalGroup::SignalGroup(QString name, SignalGroupType type, QObject* parent) :
   QObject(parent),
   m_client(NULL),
   m_exchange(NULL),
   m_queue(NULL),
   m_name(name),
   m_type(type),
   m_queueReady(false),
   m_exchangeReady(false)
{
   m_client = Connection::getClient();

   switch (type) {
   case Publisher:
      m_queueReady = true;
      enableEmitting();
      break;
   case Subscriber:
      m_exchangeReady = true;
      enableReceiving();
      break;
   case Both:
      enableEmitting();
      enableReceiving();
      break;
   default:
      qCritical("SignalGroup::SignalGroup(): "
                "Invalid signalGroupType: %i", type);
   }
}

SignalGroup::~SignalGroup()
{
}

void SignalGroup::publish(const QByteArray& data)
{
   // Convert data to hex (for some reason we can't send zero bytes)
   QByteArray msg(data);
   if (data.contains(char(0))) {
      msg = msg.toHex();
      msg.prepend(char(1));
   } else {
      msg.prepend(char(2));
   }

   m_exchange->publish(msg, "");
}

void SignalGroup::onQueueDeclared()
{
   m_queue->bind(m_name, "");
   m_queue->consume(QAMQP::Queue::coNoAck);

   m_queueReady = true;
   if (m_exchangeReady) {
      emit ready();
   }
}

void SignalGroup::onExchangeDeclared()
{
   m_exchangeReady = true;
   if (m_queueReady) {
      emit ready();
   }
}

void SignalGroup::onMessageReceived(QAMQP::Queue* queue)
{
   QByteArray payload = queue->getMessage()->payload;

   // Convert data back from hex format
   if (payload.at(0) == 1) {
      payload = QByteArray::fromHex(payload.mid(1));
   } else {
      payload = payload.mid(1);
   }

   emit messageReceived(payload, m_name);
}

void SignalGroup::enableEmitting()
{
   m_exchange = m_client->createExchange(m_name);
   m_exchange->declare("fanout");

   connect(m_exchange, SIGNAL(declared()), this, SLOT(onExchangeDeclared()));
}

void SignalGroup::enableReceiving()
{
   m_queue = m_client->createQueue();
   m_queue->declare("", QAMQP::Queue::Exclusive);

   connect(m_queue, SIGNAL(declared()), this, SLOT(onQueueDeclared()));
   connect(m_queue, SIGNAL(messageReceived(QAMQP::Queue*)),
           this,  SLOT(onMessageReceived(QAMQP::Queue*)));
}

} // System
