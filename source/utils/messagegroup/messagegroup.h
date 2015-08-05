#include <QObject>
#include <QString>
#include <QByteArray>
#include <QMutex>

#include "../messages/message.h"

#include "connection/connection.h"

#ifndef UTILS_MESSAGEGROUP_H
#define UTILS_MESSAGEGROUP_H

namespace Utils {

/*!
 * \brief The MessageGroup class
 * Each instance of MessageGroup Class represents one group in RabbitMQ
 * communication system. Group can be initialized either publish or
 * subscribe messages. It can also do both at same time.
 */
class MessageGroup : public QObject
{
   Q_OBJECT
public:
   /*!
    * \brief The GroupType enum
    * This is used to define group type.
    */
   enum GroupType
   {
      Publisher,
      Subscriber,
      Both
   };

   /*!
    * \brief Constructor.
    * \param name Group's name.
    * \param type Group's type (publisher, subscriber or both).
    * \param parent Parent object.
    */
   MessageGroup(QString name,
               GroupType type,
               QObject* parent = NULL);

   /*!
    * Destructor.
    */
   ~MessageGroup();

   /*!
    * \brief Writes data to group.
    * \param data Data to write.
    */
   void publish(const QByteArray data);

   /*!
    * \brief Writes message to group.
    * \param message Message to write.
    */
   void publish(const Message& message);

   /*!
    * \brief Writes data to specified group.
    * This method can be used send data to temporary group
    * (i.e. reply and ack).
    * \param message Data to write.
    * \param group Target group name.
    */
   static void publish(const QByteArray data,
                       const QString group);

   /*!
    * \brief Writes message to specified group.
    * This method can be used send message to temporary group
    * (i.e. reply and ack).
    * \param message Message to write.
    * \param group Target group name.
    */
   static void publish(const Message& message,
                       const QString group);

signals:
   /*!
    * \brief messageReceived is emitted when new message is received.
    * \param data Message data.
    * \param group Group name. This is useful if one slot handles
    * multiple signal groups.
    */
   void messageReceived(QByteArray data,
                        QString group);

   /*!
    * \brief ready is emitted when group is ready to publis and/or subscribe.
    */
   void ready();

private slots:
   void onConnected();

private:
   QString m_name;
   GroupType m_type;

   bool m_ready;
};

} // Utils

#endif
