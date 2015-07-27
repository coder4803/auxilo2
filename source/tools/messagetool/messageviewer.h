#ifndef MESSAGEVIEWER_H
#define MESSAGEVIEWER_H

#include <QWidget>
#include <QTreeView>
#include <QTableView>
#include <QAbstractTableModel>
#include <QStandardItemModel>

#include "messagefilterproxymodel.h"
#include "detailmodel.h"

namespace MessageTool {

class MessageViewer : public QWidget
{
   Q_OBJECT
public:
   explicit MessageViewer(QStandardItemModel& groupModel,
                          QStandardItemModel& messageModel,
                          QWidget* parent = NULL);

signals:

public slots:

private slots:
   void onMessageSelected(QModelIndex current);
   void onGroupSelectionChanged();

private:
   void initWidgets();

   QStandardItemModel& m_groupModel;
   QTreeView* m_groupView;

   QStandardItemModel& m_messageModel;
   MessageFilterProxyModel* m_messageProxyModel;
   QTableView* m_messageView;

   DetailModel* m_detailModel;
   QTableView* m_detailView;
};

} // MessageTool

#endif
