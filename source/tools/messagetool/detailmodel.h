#ifndef DETAILMODEL_H
#define DETAILMODEL_H

#include <QAbstractTableModel>

#include "messagemodel.h"
#include "globals.h"

namespace MessageTool {

class DetailModel : public QStandardItemModel
{
public:
   explicit DetailModel(QObject* parent = NULL);

   void setDetailData(Globals::MessageType messageType,
                      const QByteArray& data);

   void clearContent();

private:
   void newRow(QString name, QVariant value);

   void parseConfRequestMessage(const QByteArray& data);
   void parseConfResponseMessage(const QByteArray& data);
   void parseSignalMessage(const QByteArray& data);
   void parseSignalAckMessage(const QByteArray& data);
   void parseLogMessage(const QByteArray& data);
   void parseRequestStateMessage(const QByteArray& data);
   void parseStateResponseMessage(const QByteArray& data);
   void parseSetStateMessage(const QByteArray& data);
   void parseSetStateAckMessage(const QByteArray& data);
   void parseStateChangedMessage(const QByteArray& data);
   void parseStateChangedAckMessage(const QByteArray& data);
};

} // MessageTool

#endif
