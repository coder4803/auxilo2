#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QStandardItemModel>

#include "globals.h"

namespace MessageTool {

class MessageModel : public QStandardItemModel
{
   Q_OBJECT
public:
   explicit MessageModel(QObject *parent = 0);

public slots:
   void onNewMessage(Globals::MessageType type,
                     QByteArray payload,
                     QString group);
};

} // MessageTool

#endif
