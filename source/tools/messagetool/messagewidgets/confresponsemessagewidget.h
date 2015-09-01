#ifndef CONFRESPONSEMESSAGEWIDGET_H
#define CONFRESPONSEMESSAGEWIDGET_H

#include "messagewidget.h"
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

namespace MessageTool
{

class ConfResponseMessageWidget : public MessageWidget
{
    Q_OBJECT

public:

    ConfResponseMessageWidget(QWidget* parent = 0);

    virtual ~ConfResponseMessageWidget();

    virtual Utils::Message* getMessage() const;

    virtual QString errorString() const;


private slots:

    void onAcceptButtonClicked();
    void onClearButtonClicked();
    void onAddParameterClicked();
    void onRemoveParameterClicked();

private:

    QLabel* featureLabel_;
    QLineEdit* featureEdit_;
    QTableWidget* parameterTable_;
    QPushButton* addParameterButton_;
    QPushButton* removeParameterButton_;
    QPushButton* acceptButton_;
    QPushButton* clearButton_;
    QGridLayout* mainLayout_;

    void initWidgets();
};

}

#endif // CONFRESPONSEMESSAGEWIDGET_H
