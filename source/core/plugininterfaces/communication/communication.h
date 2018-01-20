#include <QObject>

#include "confresponsemessage.h"

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

namespace Plugins {

/*!
 * \brief The Communication class.
 * This class describes interface for communications.
 */
class Communication : public QObject
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param parent Parent object.
    */
   explicit Communication(QObject* parent) : QObject(parent) {}

   /*!
    * \brief Destructor.
    */
   virtual ~Communication() {}

   /*!
    * \brief This method should return true if connection is established.
    * \param connectionId Inner connection id. This is used with connections
    * that can have multiple connections. This is common with server
    * connections. Use >= 0. -1 means undefined (or only one connection).
    * \return True if connection is established, otherwise false.
    */
   virtual bool isConnected(qint32 connectionId = -1) const = 0;

   /*!
    * \brief This method should send data to target "device".
    * \param data Data to send.
    * \param connectionId Inner connection id. This is used with connections
    * that can have multiple connections. This is common with server
    * connections. Use >= 0. -1 means undefined (or only one connection).
    * \return True if sending succeeded, otherwise false.
    */
   virtual bool sendData(const QByteArray& data,
                         qint32 connectionId = -1) = 0;

signals:
   /*!
    * \brief This signal should be used to inform when connection establishes.
    * \param connectionId Inner connection id. This is used with connections
    * that can have multiple connections. This is common with server
    * connections. Use >= 0. -1 means undefined (or only one connection).
    */
   void connected(qint32 connectionId);

   /*!
    * \brief This signal should be used to inform when connection unestablishes.
    * \param connectionId Inner connection id. This is used with connections
    * that can have multiple connections. This is common with server
    * connections. Use >= 0. -1 means undefined (or only one connection).
    */
   void disconnected(qint32 connectionId);

   /*!
    * \brief This signal is used to deliver received data to protocol.
    * \param data Received data.
    * \param connectionId Inner connection id. This is used with connections
    * that can have multiple connections. This is common with server
    * connections. Use >= 0. -1 means undefined (or only one connection).
    */
   void dataReceived(QByteArray data,
                     qint32 connectionId);
};

} // Plugins

#endif // COMMUNICATION_H
