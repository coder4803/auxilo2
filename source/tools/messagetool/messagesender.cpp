#include <QSplitter>
#include <QLayout>
#include <QHeaderView>

#include "messagegroup.h"
#include "messagesender.h"
#include "sendmessagemodel.h"
#include "detailmodel.h"
#include "detaildelegate.h"
#include "detailfilter.h"

namespace MessageTool {

MessageSender::MessageSender(GroupModel& groupModel,
                             QWidget* parent) :
   QWidget(parent),
   m_groupModel(groupModel),
   m_sendMessageModel(NULL),
   m_sendMessageView(NULL),
   m_detailModel(NULL),
   m_detailView(NULL),
   m_targetGroupComboBox(NULL),
   m_selectedRow(-1)
{
   initWidgets();
}

void MessageSender::onNewMessage()
{
   m_sendMessageModel->newMessage();
   if (m_sendMessageModel->rowCount() == 1) {
      m_sendMessageView->setCurrentIndex(m_sendMessageView->
                                         indexAt(QPoint(0, 0)));
   }
}

void MessageSender::onDeleteMessage()
{
   m_sendMessageModel->removeMessage(m_selectedRow);
}

void MessageSender::onSendMessage()
{
   QByteArray data = m_sendMessageModel->getPayload(m_selectedRow);
   QString group = m_sendMessageModel->getGroup(m_selectedRow);
   Utils::MessageGroup::publish(data, group);
}

void MessageSender::onMessageSelected(QModelIndex current)
{
   m_selectedRow = current.row();
   m_rowChanged = true;

   QString group = m_sendMessageModel->getGroup(m_selectedRow);
   int index = m_targetGroupComboBox->findText(group);
   if (index >= 0) {
      m_targetGroupComboBox->setCurrentIndex(index);
   }

   QString description = m_sendMessageModel->getDescription(m_selectedRow);
   m_descriptionLineEdit->setText(description);

   Globals::MessageType messageType =
         m_groupModel.getMessageTypeByGroup(group);
   QByteArray data = m_sendMessageModel->getPayload(current.row());
   m_detailModel->setDetailData(messageType, data);

   m_addRowPushButton->setText(m_detailModel->getAddButtonText());
   m_addRowPushButton->setVisible(!m_addRowPushButton->text().isEmpty());
}

void MessageSender::onTargetGroupChanged(QString group)
{
   QString text = "Message type: " +
         m_groupModel.getMessageTypeNameByGroup(group);
   m_messageTypeLabel->setText(text);

   m_sendMessageModel->setTargetGroup(m_selectedRow, group);

   if (!m_rowChanged) {
      m_sendMessageModel->setPayload(m_sendMessageView->currentIndex().row(),
                                     QByteArray());
   }

   onMessageSelected(m_sendMessageView->currentIndex());

   m_rowChanged = false;
}

void MessageSender::onDescriptionChanged(QString description)
{
   m_sendMessageModel->setDescription(m_selectedRow, description);
}

void MessageSender::onDetailFieldEdited()
{
   QByteArray data = m_detailModel->getMessageData();
   m_sendMessageModel->setPayload(m_selectedRow, data);
}

void MessageSender::initWidgets()
{
   // Message list
   m_sendMessageModel = new SendMessageModel(this);
   m_sendMessageView = new QTableView(this);
   m_sendMessageView->setModel(m_sendMessageModel);
   m_sendMessageView->hideColumn(SendMessageModel::INDEX_PAYLOAD);
   m_sendMessageView->setEditTriggers(QAbstractItemView::NoEditTriggers);
   m_sendMessageView->setSelectionBehavior(QAbstractItemView::SelectRows);
   m_sendMessageView->setSelectionMode(QAbstractItemView::SingleSelection);
   m_sendMessageView->verticalHeader()->hide();
   m_sendMessageView->verticalHeader()->setDefaultSectionSize(20);
   m_sendMessageView->horizontalHeader()->setStretchLastSection(true);

   connect(m_sendMessageView->selectionModel(),
           SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
           this, SLOT(onMessageSelected(QModelIndex)));

   m_newMessagePushButton = new QPushButton("New");
   connect(m_newMessagePushButton, SIGNAL(clicked()),
           this, SLOT(onNewMessage()));

   m_deleteMessagePushButton = new QPushButton("Delete");
   connect(m_deleteMessagePushButton, SIGNAL(clicked()),
           this, SLOT(onDeleteMessage()));

   m_sendMessagePushButton = new QPushButton("Send");
   connect(m_sendMessagePushButton, SIGNAL(clicked()),
           this, SLOT(onSendMessage()));

   QHBoxLayout* messageListEditLayout = new QHBoxLayout();
   messageListEditLayout->addWidget(m_newMessagePushButton);
   messageListEditLayout->addWidget(m_deleteMessagePushButton);
   messageListEditLayout->addStretch();
   messageListEditLayout->addWidget(m_sendMessagePushButton);

   QVBoxLayout* messageListLayout = new QVBoxLayout();
   messageListLayout->addWidget(m_sendMessageView);
   messageListLayout->addLayout(messageListEditLayout);

   QWidget* messageListLayoutWidget = new QWidget(this);
   messageListLayoutWidget->setLayout(messageListLayout);

   // Detail filter
   m_detailModel = new DetailModel(this);
   m_detailModel->enableEditing(true);
   DetailFilter* detailFilter = new DetailFilter(m_detailModel);
   detailFilter->setSourceModel(m_detailModel);

   // Detail view
   m_detailView = new QTableView(this);
   m_detailView->setModel(detailFilter);
   m_detailView->setSelectionBehavior(QAbstractItemView::SelectRows);
   m_detailView->setSelectionMode(QAbstractItemView::SingleSelection);
   m_detailView->verticalHeader()->hide();
   m_detailView->verticalHeader()->setDefaultSectionSize(20);
   m_detailView->horizontalHeader()->setStretchLastSection(true);

   DetailDelegate* detailDelegate = new DetailDelegate(m_detailView);
   m_detailView->setItemDelegate(detailDelegate);
   connect(m_detailView->itemDelegate(), SIGNAL(closeEditor(QWidget*)),
           this, SLOT(onDetailFieldEdited()));

   // Description label and line edit
   QLabel* descriptionLabel = new QLabel("Description:", this);
   m_descriptionLineEdit = new QLineEdit(this);

   // Target group label and combo box
   QLabel* targetGroupLabel = new QLabel("Target group:", this);
   m_targetGroupComboBox = new QComboBox(this);

   QStringList groups = m_groupModel.getGroups();
   groups.sort();
   foreach (QString group, groups) {
      m_targetGroupComboBox->addItem(group);
   }

   connect(m_targetGroupComboBox, SIGNAL(currentIndexChanged(QString)),
           this, SLOT(onTargetGroupChanged(QString)));
   connect(m_descriptionLineEdit, SIGNAL(textEdited(QString)),
           this, SLOT(onDescriptionChanged(QString)));

   // Add button
   m_addRowPushButton = new QPushButton();
   m_addRowPushButton->hide();

   connect(m_addRowPushButton, SIGNAL(clicked()),
           m_detailModel, SLOT(addEmptyRow()));

   // Message type label
   QString currentGroup = m_targetGroupComboBox->currentText();
   m_messageTypeLabel = new QLabel(QString("Message type: ") + m_groupModel.
                                   getMessageTypeNameByGroup(currentGroup));

   // Description layout
   QHBoxLayout* descriptionLayout = new QHBoxLayout();
   descriptionLayout->addWidget(descriptionLabel);
   descriptionLayout->addWidget(m_descriptionLineEdit);

   // Target group layout
   QHBoxLayout* targetGroupLayout = new QHBoxLayout();
   targetGroupLayout->addWidget(targetGroupLabel);
   targetGroupLayout->addWidget(m_targetGroupComboBox);
   targetGroupLayout->addStretch();

   // Add button layout
   QHBoxLayout* addButtonLayout = new QHBoxLayout();
   addButtonLayout->addStretch();
   addButtonLayout->addWidget(m_addRowPushButton);

   // Compile detail layout
   QVBoxLayout* detailLayout = new QVBoxLayout();
   detailLayout->addLayout(targetGroupLayout);
   detailLayout->addWidget(m_messageTypeLabel);
   detailLayout->addLayout(descriptionLayout);
   detailLayout->addWidget(m_detailView);
   detailLayout->addLayout(addButtonLayout);

   // Detail widget for splitter
   QWidget* detailWidget = new QWidget(this);
   detailWidget->setLayout(detailLayout);

   // Splitter for message and detail fields
   QSplitter* splitter = new QSplitter(this);
   splitter->addWidget(messageListLayoutWidget);
   splitter->addWidget(detailWidget);

   QHBoxLayout* mainLayout = new QHBoxLayout();
   mainLayout->addWidget(splitter);

   this->setLayout(mainLayout);

   // One message for starters
   onNewMessage();
}

} // MessageTool
