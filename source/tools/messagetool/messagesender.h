#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>

namespace MessageTool {

class MessageSender : public QWidget
{
   Q_OBJECT
public:
   explicit MessageSender(QStandardItemModel& groupModel,
                          QWidget* parent = 0);

signals:

public slots:

private:
   bool initWidgets();

   QStandardItemModel& m_groupModel;
   QTreeView* m_groupView;
};

} // MessageTool

#endif
