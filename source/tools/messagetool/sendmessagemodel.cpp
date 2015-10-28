#include "sendmessagemodel.h"

namespace MessageTool {

SendMessageModel::SendMessageModel(QObject *parent) :
   QStandardItemModel(parent)
{
   setColumnCount(3);

   QStringList headerList;
   headerList << "Target group";
   headerList << "Description";
   headerList << "Payload";

   setHorizontalHeaderLabels(headerList);
}

void SendMessageModel::newMessage()
{
   static int counter = 0;

   QStandardItem* targetGroupItem = new QStandardItem();
   targetGroupItem->setData("confRequest", Qt::DisplayRole);

   QStandardItem* descriptionItem = new QStandardItem();
   descriptionItem->setData(QString("message_%1").arg(counter), Qt::DisplayRole);

   QStandardItem* payloadItem = new QStandardItem();
   payloadItem->setData("", Qt::DisplayRole);

   QList<QStandardItem*> row;
   row.append(targetGroupItem);
   row.append(descriptionItem);
   row.append(payloadItem);

   insertRow(this->rowCount(), row);

   ++counter;
}

bool SendMessageModel::removeMessage(int row)
{
   return removeRow(row);
}

bool SendMessageModel::setTargetGroup(int row, QString targetGroup)
{
   return setData(index(row, 0), targetGroup);
}

bool SendMessageModel::setDescription(int row, QString description)
{
   return setData(index(row, 1), description);
}

bool SendMessageModel::setPayload(int row, QByteArray payload)
{
   return setData(index(row, 2), payload);
}

QString SendMessageModel::getGroup(int row)
{
   return index(row, 0).data().toString();
}

QString SendMessageModel::getDescription(int row)
{
   return index(row, 1).data().toString();
}

QByteArray SendMessageModel::getPayload(int row)
{
   return index(row, 2).data().toByteArray();
}

} // MessageTool
