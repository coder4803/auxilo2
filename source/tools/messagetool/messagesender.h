#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include <QWidget>
#include <QTableView>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "groupmodel.h"
#include "sendmessagemodel.h"
#include "detailmodel.h"

namespace MessageTool {

class MessageSender : public QWidget
{
   Q_OBJECT
public:
   explicit MessageSender(GroupModel& groupModel,
                          QWidget* parent = 0);

private slots:
   void onNewMessage();
   void onDeleteMessage();
   void onSendMessage();

   void onMessageSelected(QModelIndex current);
   void onTargetGroupChanged(QString group);
   void onDescriptionChanged(QString description);
   void onDetailFieldEdited();

private:
   void initWidgets();

   GroupModel& m_groupModel;

   SendMessageModel* m_sendMessageModel;
   QTableView* m_sendMessageView;

   QPushButton* m_newMessagePushButton;
   QPushButton* m_deleteMessagePushButton;
   QPushButton* m_sendMessagePushButton;

   DetailModel* m_detailModel;
   QTableView* m_detailView;

   QComboBox* m_targetGroupComboBox;
   QLabel* m_messageTypeLabel;
   QLineEdit* m_descriptionLineEdit;

   QPushButton* m_addRowPushButton;

   int m_selectedRow;
   bool m_rowChanged;
};

} // MessageTool

#endif
