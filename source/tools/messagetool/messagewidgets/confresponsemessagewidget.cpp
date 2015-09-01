#include "confresponsemessagewidget.h"
#include "confresponsemessage.h"
#include <QDebug>

namespace MessageTool
{

ConfResponseMessageWidget::ConfResponseMessageWidget(QWidget *parent) :
    MessageWidget(parent),
    featureLabel_( new QLabel(tr("feature name:"), this) ),
    featureEdit_( new QLineEdit(this) ),
    parameterTable_( new QTableWidget(0, 2, this) ),
    addParameterButton_( new QPushButton(tr("Add parameter"), this) ),
    removeParameterButton_( new QPushButton(tr("Remove parameter"), this) ),
    acceptButton_( new QPushButton(tr("Accept"), this) ),
    clearButton_( new QPushButton(tr("Clear"), this) ),
    mainLayout_( new QGridLayout(this) )
{
    this->initWidgets();
}

ConfResponseMessageWidget::~ConfResponseMessageWidget()
{

}


Utils::Message *ConfResponseMessageWidget::getMessage() const
{
    Utils::ParameterSet params(featureEdit_->text());
    if (params.featureName().isEmpty()){
        return nullptr;
    }

    for (int i=0; i<parameterTable_->rowCount(); ++i){
        QString name = parameterTable_->item(i,0)->text();
        if (name.isEmpty()) {
            return nullptr;
        }
        QString val = parameterTable_->item(i,1)->text();
        params.appendParameter(name, val);
    }

    return new Utils::ConfResponseMessage(params);
}


QString ConfResponseMessageWidget::errorString() const
{
    QString msg;
    if (featureEdit_->text().isEmpty()){
        msg = tr("Feature name is missing.");
    }
    else {
        for (int i=0; i<parameterTable_->rowCount(); ++i){
            if (parameterTable_->item(i,0)->text().isEmpty()){
                msg = tr("Parameter name cannot be an empty string.");
            }
        }
    }
    return msg;
}


void ConfResponseMessageWidget::onAcceptButtonClicked()
{
    emit this->messageAccepted();
}


void ConfResponseMessageWidget::onClearButtonClicked()
{
    featureEdit_->clear();
    parameterTable_->clearContents();
    while (parameterTable_->rowCount() > 0){
        parameterTable_->removeRow(0);
    }
}


void ConfResponseMessageWidget::onAddParameterClicked()
{
    parameterTable_->insertRow( parameterTable_->rowCount() );
}


void ConfResponseMessageWidget::onRemoveParameterClicked()
{
    qDebug() << "Remove row";
    parameterTable_->removeRow( parameterTable_->rowCount()-1 );
}


void ConfResponseMessageWidget::initWidgets()
{
    // Feature
    mainLayout_->addWidget(featureLabel_, 0,0,1,1, Qt::AlignTop|Qt::AlignLeft);
    mainLayout_->addWidget(featureEdit_, 0,1,1,1, Qt::AlignTop|Qt::AlignLeft);

    // Parameters
    mainLayout_->addWidget(parameterTable_, 1,0,4,2, Qt::AlignTop|Qt::AlignLeft);
    mainLayout_->addWidget(addParameterButton_, 1,2,1,1);
    mainLayout_->addWidget(removeParameterButton_, 2,2,1,1);
    parameterTable_->setHorizontalHeaderLabels( {tr("Parameter name"), tr("Value")} );

    // Buttons
    mainLayout_->addWidget(acceptButton_, 6,0,1,1);
    mainLayout_->addWidget(clearButton_, 6,1,1,1);
    this->setLayout(mainLayout_);

    connect(acceptButton_, SIGNAL(clicked(bool)),
            this, SLOT(onAcceptButtonClicked()) );

    connect(clearButton_, SIGNAL(clicked(bool)),
            this, SLOT(onClearButtonClicked()) );

    connect(addParameterButton_, SIGNAL(clicked(bool)),
            this, SLOT(onAddParameterClicked()) );

    connect(removeParameterButton_, SIGNAL(clicked(bool)),
            this, SLOT(onRemoveParameterClicked()) );
}

}
