#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include <QStandardItemModel>

#include "globals.h"

namespace MessageTool {

/*!
 * \brief The GroupModel class
 * This model holds group information.
 */
class GroupModel : public QStandardItemModel
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param deviceFileName Path to coreconf.xml.
    * \param parent Parent object.
    */
   explicit GroupModel(const QString& deviceFileName,
                       QObject* parent = NULL);

   /*!
    * \brief Returns list of groups.
    * \return List of groups.
    */
   QStringList getGroups() const;

   /*!
    * \brief Returns message type by group name.
    * \param groupt Name of the group.
    * \return Message type of given group.
    */
   Globals::MessageType getMessageTypeByGroup(const QString& group) const;

   /*!
    * \brief Returns message type name by group.
    * \param group Name of the group.
    * \return Name of message type of given group.
    */
   QString getMessageTypeNameByGroup(const QString& group) const;

signals:
   /*!
    * \brief Signal is emitted whenever a message is received.
    * \param messageType Type of received message.
    * \param payload Payload of received message.
    * \param group Name of the group that received the message.
    */
   void messageReceived(Globals::MessageType messageType,
                        QByteArray payload,
                        QString group);

private slots:
   /*!
    * \brief Handles received message (signals it forward).
    * \param payload Payload of received message.
    * \param group Name of the group that received the message.
    */
   void handleMessage(QByteArray payload, QString group);

private:
   /*!
    * \brief Loads device names from configuration file.
    * \param deviceFileName Path to the coreconfig.xml file.
    * \param interfaces Reference to interface container.
    * \param devices Reference to device container.
    * \return True if devices where loaded succesfully, otherwise false.
    */
   bool loadDeviceNames(const QString& deviceFileName,
                        QSet<QString>& interfaces,
                        QSet<QString>& devices);

   /*!
    * \brief Adds new group to this model.
    * \param messageType Message type that is accepted to the group.
    * \param group Name of the group.
    * \return Pointer to new group (QStandardItem*).
    */
   QStandardItem* newGroup(Globals::MessageType messageType,
                           QString group);

   /*!
    * \brief Generates group tree out of loaded configurations.
    * \param interfaceNames Set of interface names.
    * \param deviceNames Set of device names.
    */
   void generateGroupTree(QSet<QString>& interfaceNames,
                          QSet<QString>& deviceNames);

   /*!
    * \brief Subscribes to all known groups.
    */
   void subscribeToGroups();

   //! Quick lookup table for message types
   //! Format: group name, message type
   QHash<QString, Globals::MessageType> m_messageTypeTable;
};

} // MessageTool

#endif
