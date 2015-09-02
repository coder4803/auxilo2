#include "messagesender.h"
#include "messagegroup.h"
#include <QMessageBox>
#include <QDebug>



namespace MessageTool {

const QHash<QString, int> MessageSender::TYPE_INDEXES
({{"stateHolderConf", 4},
  {"setState", 7},
  {"requestState", 11},
  {"stateChangedAck", 9},
  {"confRequest", 3},
  {"signalHandler", 1},
  {"writeLog", 5}
 });

MessageSender::MessageSender(QStandardItemModel& groupModel,
                             QWidget* parent) :
   QWidget(parent),
   m_groupModel(groupModel),
   m_groupView( new QTreeView(this) ),
   m_msgWidgetPool(),
   m_activeMsgWidget(nullptr),
   m_targetGroupLabel( new QLabel(tr("Send to group:"), this) ),
   m_targetGroupDisplay( new QLineEdit(tr("Select group from left."), this) ),
   m_messageTypeLabel( new QLabel(tr("Message type:"), this) ),
   m_messageTypeBox( new QComboBox(this) ),
   m_warningLabel( new QLabel(tr("Warning: unexpected message type"), this) ),
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
    Utils::MessageGroup::publish(msg->data(), m_targetGroupDisplay->text() );
    delete msg;
}


void MessageSender::onMsgTypeSelected(int type)
{
    QString msgName = m_messageTypeBox->itemText(type);
    if (m_activeMsgWidget != nullptr){
        m_mainLayout->removeWidget( m_activeMsgWidget.get() );
        m_activeMsgWidget->hide();
        disconnect(m_activeMsgWidget.get(), SIGNAL(messageAccepted()),
                   this, SLOT(onMessageAccepted()) );
        m_msgWidgetPool.release( std::move(m_activeMsgWidget) );
    }
    m_activeMsgWidget = std::move( m_msgWidgetPool.reserve( msgName ) );

    if (m_activeMsgWidget != nullptr){
        m_mainLayout->addWidget(m_activeMsgWidget.get(), 5,2,4,2, Qt::AlignTop | Qt::AlignLeft);
        m_activeMsgWidget->show();
        connect(m_activeMsgWidget.get(), SIGNAL(messageAccepted()),
                this, SLOT(onMessageAccepted()) );

        if (TYPE_INDEXES.value(m_targetGroupDisplay->text()) != type){
            m_warningLabel->show();
        }
        else{
            m_warningLabel->hide();
        }
    }
}


void MessageSender::onGroupSelected()
{
    QModelIndex index = m_groupView->selectionModel()->selectedIndexes().first();

    if (m_groupModel.hasChildren(index)){
        return;
    }
    QString group = index.data().toString();
    m_targetGroupDisplay->setText( group );
    int type = TYPE_INDEXES.value(group);
    m_messageTypeBox->setCurrentIndex(type);
    this->onMsgTypeSelected( type );
}


bool MessageSender::initWidgets()
{
    // Target group layot
    m_mainLayout->addWidget(m_targetGroupLabel, 2,2,1,1,
                            Qt::AlignTop | Qt::AlignLeft);
    m_mainLayout->addWidget(m_targetGroupDisplay, 2,3,1,5,
                            Qt::AlignTop | Qt::AlignLeft);

    // Message type layout
    m_mainLayout->addWidget(m_messageTypeLabel, 3,2,1,1,
                            Qt::AlignTop | Qt::AlignLeft);
    m_mainLayout->addWidget(m_messageTypeBox, 3,3,1,1,
                            Qt::AlignTop | Qt::AlignLeft);
    m_mainLayout->addWidget(m_warningLabel, 4,2,1,1,
                            Qt::AlignTop | Qt::AlignLeft);
    //m_warningLabel->hide();
    // Message type selection
    const QStringList TYPES = {tr("---"),
                               tr("SignalMessage"),
                               tr("SignalAckMessage"),
                               tr("ConfRequestMessage"),
                               tr("ConfResponseMessage"),
                               tr("LogMessage"),
                               tr("RequestStateMessage"),
                               tr("SetStateMessage"),
                               tr("SetStateAckMessage"),
                               tr("StateChangedAckMessage"),
                               tr("StateChangedMessage"),
                               tr("StateResponseMessage")};

    foreach (QString s, TYPES) {
        m_messageTypeBox->addItem(s);
    }

    // Message editor
    m_activeMsgWidget = m_msgWidgetPool.reserve(m_messageTypeBox->itemText(0));

    // Group view.
    m_mainLayout->addWidget(m_groupView, 1,1,8,1, Qt::AlignLeft);
    m_groupView->setModel(&m_groupModel);
    m_groupView->setHeaderHidden(true);
    m_groupView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_groupView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_groupView->setMinimumWidth(200);

    connect(m_messageTypeBox, SIGNAL(activated(int)),
             this, SLOT(onMsgTypeSelected(int) ) );

    connect(m_groupView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(onGroupSelected()) );

    return true;
}

} // MessageTool
