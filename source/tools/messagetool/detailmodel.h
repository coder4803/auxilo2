#ifndef DETAILMODEL_H
#define DETAILMODEL_H

#include <QAbstractTableModel>

#include "message.h"
#include "messagemodel.h"
#include "globals.h"

namespace MessageTool {

class DetailModel : public QStandardItemModel
{
   Q_OBJECT
public:
   explicit DetailModel(QObject* parent = NULL);
   ~DetailModel();

   void setDetailData(Globals::MessageType messageType,
                      const QByteArray& data);

   void clearContent();

   void enableEditing(bool editable);

   QStringList getOptions(int row);

   QString getAddButtonText();

   QByteArray getMessageData();

public slots:
   void addEmptyRow();

private:
   Globals::MessageType m_messageType;
   bool m_ductile;

   void newRow(QString name,
               QVariant value,
               QStringList options = QStringList(),
               bool editable = false);

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

   QByteArray createConfRequestMessage();
   QByteArray createConfResponseMessage();
   QByteArray createSignalMessage();
   QByteArray createSignalAckMessage();
   QByteArray createLogMessage();
   QByteArray createRequestStateMessage();
   QByteArray createStateResponseMessage();
   QByteArray createSetStateMessage();
   QByteArray createSetStateAckMessage();
   QByteArray createStateChangedMessage();
   QByteArray createStateChangedAckMessage();
};

} // MessageTool

#endif
