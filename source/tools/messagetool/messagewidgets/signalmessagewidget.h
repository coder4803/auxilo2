#ifndef SIGNALMESSAGEWIDGET_H
#define SIGNALMESSAGEWIDGET_H

#include "messagewidget.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QStringList>

namespace MessageTool
{

/*!
 * \brief The SignalMessageWidget class
 *  A widget for inserting input information for a SignalMessage.
 */
class SignalMessageWidget : public MessageWidget
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor
     * \param parent QWidget's parent.
     */
    SignalMessageWidget(QWidget* parent = 0);

    /*!
     * \brief ~Destructor.
     */
    virtual ~SignalMessageWidget();

    // Implements the MessageWidget interface.
    virtual Utils::Message* getMessage() const;
    virtual QString errorString() const;

private slots:

    /*!
     * \brief Action on the click on AcceptButton.
     */
    void onAcceptButtonClicked();

    /*!
     * \brief Action on the click on clearButton.
     */
    void onClearButtonClicked();

private:

    QGridLayout* mainLayout_;
    QLabel* signalNameLabel_;
    QLineEdit* signalNameEdit_;
    QLabel* parametersLabel_;
    QLineEdit* parametersEdit_;
    QLabel* ackGroupLabel_;
    QLineEdit* ackGroupEdit_;
    QLabel* senderLabel_;
    QLineEdit* senderEdit_;
    QPushButton* acceptButton_;
    QPushButton* clearButton_;

    /*!
     * \brief Set child widgets in the layout.
     */
    void createLayout();

    /*!
     * \brief Check if input information is valid.
     * \return True, if information is valid.
     */
    bool isValidSignal() const;
};


}

#endif // SIGNALMESSAGEWIDGET_H
