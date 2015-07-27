#include "messagesender.h"

namespace MessageTool {

MessageSender::MessageSender(QStandardItemModel& groupModel,
                             QWidget* parent) :
   QWidget(parent),
   m_groupModel(groupModel)
{
}

bool MessageSender::initWidgets()
{
   m_groupView = new QTreeView(this);

   return true;
}

} // MessageTool
