#include "connection.h"

namespace Utils {

const int DEFAULT_RETRY_INTERVAL = 5000; // 5s

Connection* Connection::m_instance = NULL;

QAMQP::Client* Connection::getClient()
{
   return instance()->client();
}

Connection::~Connection()
{
   m_instance = NULL;
}

void Connection::setHost(QString host,
                         quint16 port)
{
   if (instanceExists()) {
      qCritical("Connection: host addres must be "
                "set before creating groups!");
      return;
   }

   instance(host, port);
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

Connection::Connection(QString host,
                       quint16 port) :
   m_host(host),
   m_port(port),
   m_retryInterval(DEFAULT_RETRY_INTERVAL)
{
   m_client = new QAMQP::Client(this);
   m_client->setHost(host);
   m_client->setPort(DEFAULT_PORT);

   connect(m_client, SIGNAL(connected()), this, SLOT(onConnected()));
   connect(m_client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

   connect(&m_connectionTimer, SIGNAL(timeout()), this, SLOT(connectToBeam()));
   connectToBeam();
}

Connection* Connection::instance(QString host,
                                 quint16 port)
{
    if (!m_instance) {
       m_instance = new Connection(host, port);
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
