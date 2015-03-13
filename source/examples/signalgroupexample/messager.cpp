#include "messager.h"

Messager::Messager(QString mode,
                   QObject *parent) :
   QObject(parent),
   m_group(NULL),
   m_counter(0)
{
   // Host (address and port) can be changed with static setHost method.
   // Note that it can be set before creating any signa groups!
   Utils::Connection::setHost("127.0.0.1");

   // Initialize selected mode
   if (mode == "-p") {
      // Create publish type group
      m_group = new Utils::SignalGroup("examplegroup", Utils::SignalGroup::Publisher, this);

      // ready-signal is emitted when group is ready for publishing and/or receiving
      connect(m_group, SIGNAL(ready()), this, SLOT(onGroupReady()));

      // In this case we use timer to send messages once per second
      connect(&m_timer, SIGNAL(timeout()), this, SLOT(onSendMessage()));

      qCritical("Running in publisher mode...");
   } else {
       // Create subscribe type group
       m_group = new Utils::SignalGroup("examplegroup", Utils::SignalGroup::Subscriber, this);

       // SignalGroup emits messageReceived every time it receives a message
       connect(m_group, SIGNAL(messageReceived(QByteArray, QString)),
               this, SLOT(onMessageReceived(QByteArray)));

       qCritical("Running in subscriber mode...");
   }
}

void Messager::onGroupReady()
{
   // Publisher starts to send messages every second
   m_timer.start(1000);
}

void Messager::onMessageReceived(QByteArray data)
{
   qCritical("Received: %s (HEX: %s)", data.data(), data.toHex().data());
}

void Messager::onSendMessage()
{
   ++m_counter;
   QByteArray message(QString("message %1").arg(m_counter).toLatin1().data());

   // Message is sent with publish method
   m_group->publish(message);

   qCritical("Sent: %s", message.data());
}
