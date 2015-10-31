#include <QLayout>
#include <QHeaderView>
#include <QSplitter>
#include <QLayout>
#include <QSet>

#include "messageviewer.h"
#include "detailmodel.h"

namespace MessageTool {

MessageViewer::MessageViewer(QStandardItemModel& groupModel,
                             QStandardItemModel& messageModel,
                             QWidget* parent) :
   QWidget(parent),
   m_groupModel(groupModel),
   m_groupView(NULL),
   m_messageModel(messageModel),
   m_messageView(NULL),
   m_detailModel(NULL),
   m_detailView(NULL)
{
   initWidgets();
}

void MessageViewer::onMessageSelected(QModelIndex current)
{
   QModelIndex dataIndex = m_messageProxyModel->index(current.row(), 3);
   QByteArray data = m_messageProxyModel->data(dataIndex,
                                               Qt::DisplayRole).toByteArray();

   bool ok = false;
   int messageInt = m_messageProxyModel->data(dataIndex, Qt::UserRole).toInt(&ok);
   if (!ok) {
      qCritical("Invalid message type: %i", messageInt);
      return;
   }

   Globals::MessageType messageType =
         static_cast<Globals::MessageType>(messageInt);

   m_detailModel->setDetailData(messageType, data);
   m_detailView->resizeColumnsToContents();
   m_detailView->horizontalHeader()->setStretchLastSection(true);
}

void MessageViewer::onGroupSelectionChanged()
{
   QModelIndexList indexes = m_groupView->selectionModel()->selectedIndexes();

   QSet<QString> groups;
   foreach (QModelIndex index, indexes) {
      groups.insert(index.data().toString());

      // Include all child groups
      for (int i = 0; i < m_groupModel.rowCount(index); ++i) {
         groups.insert(index.child(i, 0).data().toString());
      }
   }

   m_messageProxyModel->setGroupFilterList(groups.toList());
   m_messageProxyModel->setSourceModel(&m_messageModel);
   m_messageView->resizeColumnToContents(1);
   m_detailModel->clearContent();
}

void MessageViewer::onClearButtonClicked()
{
   m_messageModel.removeRows(0, m_messageModel.rowCount());
}

void MessageViewer::initWidgets()
{
   // Group list
   m_groupView = new QTreeView(this);
   m_groupView->setModel(&m_groupModel);
   m_groupView->setHeaderHidden(true);
   m_groupView->setSelectionMode(QAbstractItemView::ExtendedSelection);
   m_groupView->setEditTriggers(QAbstractItemView::NoEditTriggers);
   m_groupView->setMinimumWidth(200);

   connect(m_groupView->selectionModel(),
           SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
           this, SLOT(onGroupSelectionChanged()));

   // Message list
   m_messageProxyModel = new MessageFilterProxyModel(this);
   m_messageProxyModel->setSourceModel(&m_messageModel);
   m_messageView = new QTableView(this);
   m_messageView->setModel(m_messageProxyModel);
   m_messageView->setSelectionMode(QAbstractItemView::SingleSelection);
   m_messageView->setSelectionBehavior(QAbstractItemView::SelectRows);
   m_messageView->setEditTriggers(QAbstractItemView::NoEditTriggers);
   m_messageView->verticalHeader()->hide();
   m_messageView->verticalHeader()->setDefaultSectionSize(20);
   m_messageView->setColumnHidden(3, true); // Hide payload column
   m_messageView->resizeColumnsToContents();
   m_messageView->horizontalHeader()->setStretchLastSection(true);

   connect(m_messageView->selectionModel(),
           SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
           this, SLOT(onMessageSelected(QModelIndex)));

   // Message details
   m_detailModel = new DetailModel(this);
   m_detailView = new QTableView(this);
   m_detailView->setModel(m_detailModel);
   m_detailView->setSelectionBehavior(QAbstractItemView::SelectRows);
   m_detailView->setEditTriggers(QAbstractItemView::NoEditTriggers);
   m_detailView->verticalHeader()->hide();
   m_detailView->verticalHeader()->setDefaultSectionSize(20);
   m_detailView->resizeColumnsToContents();
   m_detailView->horizontalHeader()->setStretchLastSection(true);

   // Clear messages button
   m_clearMessagesPushButton = new QPushButton("Clear messages");
   connect(m_clearMessagesPushButton, SIGNAL(clicked()),
           this, SLOT(onClearButtonClicked()));

   // Bottom layout
   QHBoxLayout* bottomLayout = new QHBoxLayout();
   bottomLayout->addStretch();
   bottomLayout->addWidget(m_clearMessagesPushButton);

   // Splitter for views
   QSplitter* splitter = new QSplitter(this);
   splitter->addWidget(m_groupView);
   splitter->addWidget(m_messageView);
   splitter->addWidget(m_detailView);
   splitter->setSizes(QList<int>() << 1 << 1000 << 1000);

   // Create layout
   QVBoxLayout* mainLayout = new QVBoxLayout();
   mainLayout->addWidget(splitter);
   mainLayout->addLayout(bottomLayout);

   this->setLayout(mainLayout);
}

} // MessageTool
