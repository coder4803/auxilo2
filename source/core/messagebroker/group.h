#ifndef GROUP_H
#define GROUP_H

#include <QObject>
#include <QSet>

namespace Core {

class Connection;

/*!
 * \brief The Group class.
 * This class describes one group. It takes care of sending messages to users
 * which are joined to the group.
 */
class Group : public QObject
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param parent Parent object.
    */
   explicit Group(QObject *parent = 0);

public slots:
   /*!
    * \brief Adds new connection to this group.
    * \param connection Pointer to the connection.
    */
   void addConnection(Connection* connection);

   /*!
    * \brief Remove connection from the group.
    * \param connection Pointer to the connection to remove.
    */
   void removeConnection(Connection* connection);

   /*!
    * \brief Sends message to all group members.
    * \param payload Message to send.
    */
   void publishMessage(const QByteArray& payload);

   /*!
    * \brief Sends message to one member in the group.
    * \param payload Message to send.
    */
   void deliverMessage(const QByteArray& payload);

private:
   //! List of connections joined to this group.
   QSet<Connection*> m_connections;

   //! Connection iterator. This is used when delivering messages.
   QSet<Connection*>::Iterator m_iter;
};

} // Core

#endif
