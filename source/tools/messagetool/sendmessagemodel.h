#ifndef SENDMODEL_H
#define SENDMODEL_H

#include <QStandardItemModel>

#include "globals.h"

namespace MessageTool {

class SendMessageModel : public QStandardItemModel
{
   Q_OBJECT
public:
   explicit SendMessageModel(QObject *parent = 0);

   void newMessage();

   bool removeMessage(int row);

   bool setTargetGroup(int row, QString targetGroup);

   bool setDescription(int row, QString description);

   bool setPayload(int row, QByteArray payload);

   QString getGroup(int row);

   QString getDescription(int row);

   QByteArray getPayload(int row);

signals:

public slots:

};

} // MessageTool

#endif
