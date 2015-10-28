#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include <QStandardItemModel>

#include "globals.h"

namespace MessageTool {

class GroupModel : public QStandardItemModel
{
   Q_OBJECT
public:
   explicit GroupModel(const QString& deviceFileName,
                       QObject* parent = NULL);

   QStringList getGroups() const;

   Globals::MessageType getMessageTypeByGroup(const QString& group) const;

   QString getMessageTypeNameByGroup(const QString& group) const;

signals:
   void messageReceived(Globals::MessageType messageType,
                        QByteArray payload,
                        QString group);

private slots:
   void handleMessage(QByteArray payload, QString group);

private:
   bool loadDeviceNames(const QString& deviceFileName,
                        QSet<QString>& interfaces,
                        QSet<QString>& devices);

   QStandardItem* newGroup(Globals::MessageType messageType,
                           QString group);

   void generateGroupTree(QSet<QString>& interfaceNames,
                          QSet<QString>& deviceNames);

   void subscribeToGroups();

   //! Quick lookup table for message types
   //! Format: group name, message type
   QHash<QString, Globals::MessageType> m_messageTypeTable;
};

} // MessageTool

#endif
