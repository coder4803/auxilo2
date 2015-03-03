#include "connection.h"

namespace System {

const QString ADDRESS("localhost:5672");
const int RETRY_INTERVAL = 10000; // 10s

Connection* Connection::m_instance = NULL;

QAMQP::Client* Connection::getClient()
{
   if (!m_instance) {
      m_instance = new Connection;
   }

   return m_instance->client();
}

Connection::~Connection()
{
   m_instance = NULL;
}

void Connection::connectToBeam()
{
   m_client->open(QUrl(ADDRESS));
   m_connectionTimer.start(RETRY_INTERVAL);
}

void Connection::onConnected()
{
   m_connectionTimer.stop();
   emit connected();
}

void Connection::onDisconnected()
{
   m_connectionTimer.start(RETRY_INTERVAL);
}

Connection::Connection()
{
   m_client = new QAMQP::Client(this);
   connect(m_client, SIGNAL(connected()), this, SLOT(onConnected()));
   connect(m_client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

   connect(&m_connectionTimer, SIGNAL(timeout()), this, SLOT(connectToBeam()));
   connectToBeam();
}

QAMQP::Client* Connection::client()
{
   return m_client;
}

} // System
