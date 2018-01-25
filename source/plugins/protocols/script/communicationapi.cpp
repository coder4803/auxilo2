#include "communicationapi.h"
#include "script.h"

namespace Plugins {

CommunicationAPI::CommunicationAPI(QObject* parent) :
   QObject(parent),
   m_script(qobject_cast<Script*>(parent))
{
}

CommunicationAPI::~CommunicationAPI()
{
}

bool CommunicationAPI::isConnected(qint32 connectionId) const
{
   Q_ASSERT(m_script != NULL);
   return m_script->isConnected(connectionId);
}

bool CommunicationAPI::sendData(const QByteArray& data,
                                qint32 connectionId)
{
   Q_ASSERT(m_script != NULL);
   return m_script->sendData(data, connectionId);
}

} // Plugins
