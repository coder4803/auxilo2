#include <QObject>

#ifndef COMMUNICATIONAPI_H
#define COMMUNICATIONAPI_H

namespace Plugins {

class Script;

/*!
 * \brief CommunicationAPI wraps Communication API functions for scripts.
 */
class CommunicationAPI : public QObject
{
   Q_OBJECT
public:
   CommunicationAPI(QObject* parent);
   ~CommunicationAPI();

   Q_INVOKABLE bool isConnected(qint32 connectionId = -1) const;

   Q_INVOKABLE bool sendData(const QByteArray& data,
                             qint32 connectionId = -1);

private:
   Script* m_script;
};

} // Plugins

#endif
