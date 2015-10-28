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

private:

};

} // MessageTool

#endif
