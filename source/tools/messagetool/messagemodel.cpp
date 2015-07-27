#include <QDateTime>
#include <QMetaEnum>

#include "messagemodel.h"

namespace MessageTool {

MessageModel::MessageModel(QObject *parent) :
   QStandardItemModel(parent)
{
   setColumnCount(4);

   QStringList headerList;
   headerList << "Timestamp";
   headerList << "Group";
   headerList << "Message type";
   headerList << "Payload";
   setHorizontalHeaderLabels(headerList);
}

void MessageModel::onNewMessage(Globals::MessageType type,
                                QByteArray payload,
                                QString group)
{
   QStandardItem* timestampItem = new QStandardItem();
   quint64 timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
   timestampItem->setData(timestamp, Qt::DisplayRole);

   QStandardItem* groupItem = new QStandardItem();
   groupItem->setData(group, Qt::DisplayRole);

   QStandardItem* typeItem = new QStandardItem();
   QString typeString = Globals::staticMetaObject.
         enumerator(Globals::ENUMINDEX_MESSAGE_TYPE).valueToKey(type);
   typeItem->setData(typeString, Qt::DisplayRole);

   QStandardItem* payloadItem = new QStandardItem();
   payloadItem->setData(payload, Qt::DisplayRole);
   payloadItem->setData(static_cast<int>(type), Qt::UserRole);

   QList<QStandardItem*> row;
   row.append(timestampItem);
   row.append(groupItem);
   row.append(typeItem);
   row.append(payloadItem);

   insertRow(0, row);
}

} // MessageTool
