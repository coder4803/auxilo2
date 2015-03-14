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

/*!
 * \brief The SignalGroup class
 * Each instance of SignalGroup Class represents one group in RabbitMQ
 * communication system. Group can be initialized either publish or
 * subscribe messages. It can also do both at same time.
 */
class SignalGroup : public QObject
{
   Q_OBJECT
   Q_DISABLE_COPY(SignalGroup)
public:
   /*!
    * \brief The GroupType enum
    */
   enum GroupType {
      Publisher,
      Subscriber,
      Both
   };

   /*!
    * \brief SignalGroup
    * \param name Group's name.
    * \param type Group's type (publisher, subscriber or both)
    * \param parent Parent object.
    */
   SignalGroup(QString name, GroupType type, QObject* parent = NULL);
   ~SignalGroup();

   /*!
    * \brief publish writes data to group.
    * \param data Data to write.
    * \return Returns true if group is ready (ready-signal is emitted),
    * otherwis false.
    */
   bool publish(QByteArray data);

signals:
   /*!
    * \brief messageReceived is emitted when new message is received.
    * \param data Message data.
    * \param group Group name. This is useful if one slot handles
    * multiple signal groups.
    */
   void messageReceived(QByteArray data, QString group);

   /*!
    * \brief ready is emitted when group is ready to publis and/or subscribe.
    */
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
