#include "messagesender.h"
#include <QMessageBox>

namespace MessageTool {

MessageSender::MessageSender(QStandardItemModel& groupModel,
                             QWidget* parent) :
   QWidget(parent),
   m_groupModel(groupModel),
   m_groupView( new QTreeView(this) ),
   m_msgWidgetPool(),
   m_activeMsgWidget(nullptr),
   m_targetGroupLabel( new QLabel(tr("Send to group:"), this) ),
   m_targetGroupEdit( new QLineEdit(this) ),
   m_messageTypeLabel( new QLabel(tr("Message type:"), this) ),
   m_messageTypeBox( new QComboBox(this) ),
   m_mainLayout( new QGridLayout(this) )
{
    this->initWidgets();
}


void MessageSender::onMessageAccepted()
{
    Utils::Message* msg = m_activeMsgWidget->getMessage();
    if (msg == nullptr){
        QMessageBox::information(this, tr("Error"),
                                 m_activeMsgWidget->errorString(),
                                 QMessageBox::Ok );
        return;
    }

    // Send message (make sure to avoid the memory leak).
    delete msg;
}


void MessageSender::onMsgTypeSelected(const QString& type)
{
    if (m_activeMsgWidget != nullptr){
        m_mainLayout->removeWidget( m_activeMsgWidget.get() );
        m_activeMsgWidget->hide();
        m_msgWidgetPool.release( std::move(m_activeMsgWidget) );
    }
    m_activeMsgWidget = std::move( m_msgWidgetPool.reserve(type) );

    if (m_activeMsgWidget != nullptr){
        m_mainLayout->addWidget(m_activeMsgWidget.get(), 4,2,6,2, Qt::AlignTop | Qt::AlignLeft);
        m_activeMsgWidget->show();
    }
}


bool MessageSender::initWidgets()
{
    // Target group layot
    m_mainLayout->addWidget(m_targetGroupLabel, 2,2,1,1, Qt::AlignTop);
    m_mainLayout->addWidget(m_targetGroupEdit, 2,3,1,1, Qt::AlignTop);

    // Message type layout
    m_mainLayout->addWidget(m_messageTypeLabel, 3,2,1,1, Qt::AlignTop);
    m_mainLayout->addWidget(m_messageTypeBox, 3,3,1,1, Qt::AlignTop);

    const QStringList TYPES = {tr("SignalMessage"), tr("SignalAckMessage"),
                               tr("ConfRequestMessage"), tr("ConfResponseMessage")};

    foreach (QString s, TYPES) {
        m_messageTypeBox->addItem(s);
    }

    // Message editor
    m_activeMsgWidget = m_msgWidgetPool.reserve(m_messageTypeBox->itemText(0));
    m_mainLayout->addWidget(m_activeMsgWidget.get(), 4,2,6,2, Qt::AlignTop | Qt::AlignLeft);

    m_mainLayout->addWidget(m_groupView, 1,1,8,1);

    connect(m_activeMsgWidget.get(), SIGNAL(messageAccepted()),
            this, SLOT(onMessageAccepted()));

    connect(m_messageTypeBox, SIGNAL(activated(QString)),
             this, SLOT(onMsgTypeSelected(QString) ) );

    return true;
}

} // MessageTool
