#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>

#include "messagewidgetpool.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <memory>

namespace MessageTool {

class MessageSender : public QWidget
{
   Q_OBJECT
public:
   explicit MessageSender(QStandardItemModel& groupModel,
                          QWidget* parent = 0);

signals:

public slots:

private slots:

    void onMessageAccepted();

    void onMsgTypeSelected(int type);

    void onGroupSelected();

private:
   bool initWidgets();

   QStandardItemModel& m_groupModel;
   QTreeView* m_groupView;

    MessageWidgetPool m_msgWidgetPool;
    std::unique_ptr<MessageWidget> m_activeMsgWidget;

    // Child widgets:
    QLabel* m_targetGroupLabel;
    QLabel* m_targetGroupDisplay;
    QLabel* m_messageTypeLabel;
    QComboBox* m_messageTypeBox;
    QLabel* m_warningLabel;

    QGridLayout* m_mainLayout;

    static const QHash<QString, int> TYPE_INDEXES;
};

} // MessageTool

#endif
