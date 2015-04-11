#include "connection.h"

namespace Utils {

const QString Connection::DEFAULT_ADDRESS = "127.0.0.1";
const quint16 Connection::DEFAULT_PORT = 5672;
const quint16 Connection::DEFAULT_RETRY_INTERVAL = 5000; // 5s

Connection* Connection::m_instance = NULL;

QAMQP::Client* Connection::getClient()
{
   return instance()->client();
}

Connection::~Connection()
{
   m_instance = NULL;
}

void Connection::setHost(QString address,
                         quint16 port)
{
   if (instanceExists()) {
      qCritical("Connection: host addres must be "
                "set before creating groups!");
      return;
   }

   instance(address, port);
}

bool Connection::isConnected()
{
   if (instanceExists()) {
      return instance()->getClient()->isConnected();
   }

   return false;
}

void Connection::connectToBeam()
{
   m_client->open();
   m_connectionTimer.start(m_retryInterval);
}

void Connection::onConnected()
{
   m_connectionTimer.stop();
}

void Connection::onDisconnected()
{
   m_connectionTimer.start(m_retryInterval);
}

Connection::Connection(QString address,
                       quint16 port) :
   m_retryInterval(DEFAULT_RETRY_INTERVAL)
{
   m_client = new QAMQP::Client(this);
   m_client->setHost(address);
   m_client->setPort(port);

   connect(m_client, SIGNAL(connected()), this, SLOT(onConnected()));
   connect(m_client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

   connect(&m_connectionTimer, SIGNAL(timeout()), this, SLOT(connectToBeam()));
   connectToBeam();
}

Connection* Connection::instance(QString address,
                                 quint16 port)
{
    if (!m_instance) {
       m_instance = new Connection(address, port);
    }

    return m_instance;
}

bool Connection::instanceExists()
{
   if (m_instance) {
      return true;
   }

   return false;
}

QAMQP::Client* Connection::client()
{
   return m_client;
}

} // Utils
