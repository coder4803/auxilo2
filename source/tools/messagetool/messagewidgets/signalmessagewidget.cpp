#include "signalmessagewidget.h"
#include "signalmessage.h"


namespace MessageTool
{

SignalMessageWidget::SignalMessageWidget(QWidget *parent) :
    MessageWidget(parent),
    mainLayout_(new QGridLayout(this)),
    signalNameLabel_(new QLabel(tr("Signal name:"), this)),
    signalNameEdit_(new QLineEdit(this)),
    parametersLabel_(new QLabel(tr("Parameters (seperate with ','):"), this)),
    parametersEdit_(new QLineEdit(this)),
    ackGroupLabel_(new QLabel(tr("Acknowledgement group:"), this)),
    ackGroupEdit_(new QLineEdit(this)),
    senderLabel_(new QLabel(tr("Sender:"), this)),
    senderEdit_(new QLineEdit(this)),
    acceptButton_(new QPushButton(tr("Accept"), this)),
    clearButton_(new QPushButton(tr("Clear"), this))
{
    this->createLayout();

    connect(clearButton_, SIGNAL(clicked(bool)),
            this, SLOT(onClearButtonClicked()) );

    connect(acceptButton_, SIGNAL(clicked(bool)),
            this, SLOT(onAcceptButtonClicked()) );
}


SignalMessageWidget::~SignalMessageWidget()
{
}


Utils::Message *SignalMessageWidget::getMessage() const
{
    if ( !isValidSignal() ){
        return 0;
    }

    return new Utils::SignalMessage(signalNameEdit_->text(),
                                    senderEdit_->text(),
                                    parametersEdit_->text().split(','),
                                    ackGroupEdit_->text() );
}


QString SignalMessageWidget::errorString() const
{
    QString msg;
    if ( signalNameEdit_->text().isEmpty() ){
        msg = tr("Signal name is missing.");
    }
    else if ( senderEdit_->text().isEmpty() ){
        msg = tr("Sender name is missing.");
    }
    return msg;
}


void SignalMessageWidget::onAcceptButtonClicked()
{
    emit this->messageAccepted();
}


void SignalMessageWidget::onClearButtonClicked()
{
    signalNameEdit_->clear();
    parametersEdit_->clear();
    ackGroupEdit_->clear();
    senderEdit_->clear();
}


void SignalMessageWidget::createLayout()
{
    mainLayout_->addWidget(signalNameLabel_, 0,0,1,1, Qt::AlignLeft);
    mainLayout_->addWidget(signalNameEdit_, 0,1,1,1, Qt::AlignLeft);

    mainLayout_->addWidget(parametersLabel_, 1,0,1,1, Qt::AlignLeft);
    mainLayout_->addWidget(parametersEdit_, 1,1,1,1, Qt::AlignLeft);

    mainLayout_->addWidget(ackGroupLabel_, 2,0,1,1, Qt::AlignLeft);
    mainLayout_->addWidget(ackGroupEdit_, 2,1,1,1, Qt::AlignLeft);

    mainLayout_->addWidget(senderLabel_, 3,0,1,1, Qt::AlignLeft);
    mainLayout_->addWidget(senderEdit_, 3,1,1,1, Qt::AlignLeft);

    mainLayout_->addWidget(acceptButton_, 4,0,2,1, Qt::AlignLeft);
    mainLayout_->addWidget(clearButton_, 4,1,1,1, Qt::AlignLeft);
}


bool SignalMessageWidget::isValidSignal() const
{
    if ( signalNameEdit_->text().isEmpty() ){
        return false;
    }
    else if ( senderEdit_->text().isEmpty() ){
        return false;
    }
    return true;
}

}
