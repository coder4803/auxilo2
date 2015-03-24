#include "signalgroup.h"

namespace Utils {

SignalGroup::SignalGroup(QString name, GroupType type, QObject* parent) :
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
   case Both:
      enableEmitting();
      enableReceiving();
      break;
   default:
      qCritical("SignalGroup::SignalGroup(): "
                "Invalid GroupType: %i", type);
   }
}

SignalGroup::~SignalGroup()
{
}

bool SignalGroup::publish(const QByteArray data)
{
   if (!m_exchange) {
      return false;
   }

   m_exchange->publish(data, "", "application/octet-stream");

   return false;
}

bool SignalGroup::publish(const Message& message)
{
   if (m_type == Subscriber) {
      return false;
   }

   return publish(message.data());
}

void SignalGroup::publish(const QByteArray data, QString group)
{
   SignalGroup* signalGroup = new SignalGroup(group, Publisher);
   signalGroup->publish(data);
   signalGroup->deleteLater();
}

void SignalGroup::publish(const Message &message, QString group)
{
   publish(message.data(), group);
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

} // Utils
