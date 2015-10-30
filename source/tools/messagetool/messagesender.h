#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include <QWidget>
#include <QTableView>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "groupmodel.h"
#include "sendmessagemodel.h"
#include "detailmodel.h"

namespace MessageTool {

/*!
 * \brief The MessageSender class.
 * This class represents the "message send" view.
 */
class MessageSender : public QWidget
{
   Q_OBJECT
public:
   /*!
    * \brief Costructor.
    * \param groupModel Reference to group model.
    * \param parent Parent widget.
    */
   explicit MessageSender(GroupModel& groupModel,
                          QWidget* parent = 0);

private slots:
   //! Adds new message to "send message" model.
   void onNewMessage();

   //! Deletes selected message from "send message" model.
   void onDeleteMessage();

   //! Sends selected message.
   void onSendMessage();

   /*!
    * \brief Handles message selection from "send message" model.
    * \param current Selected row.
    */
   void onMessageSelected(QModelIndex current);

   /*!
    * \brief Handles target group change.
    * \param group Name of new group.
    */
   void onTargetGroupChanged(QString group);

   /*!
    * \brief Handles description change.
    * \param description New description.
    */
   void onDescriptionChanged(QString description);

   /*!
    * \brief Handles detail field change.
    * Copies payload from detai model to "send message" model.
    */
   void onDetailFieldEdited();

private:
   /*!
    * \brief Initializes widgets of the view.
    */
   void initWidgets();

   //! Reference to group model.
   GroupModel& m_groupModel;

   //! sendMessageModel contains messages ready to send.
   SendMessageModel* m_sendMessageModel;

   //! View for "send messages".
   QTableView* m_sendMessageView;

   //! Button for adding a new message.
   QPushButton* m_newMessagePushButton;

   //! Button for deleting a message.
   QPushButton* m_deleteMessagePushButton;

   //! Button for sending selected message.
   QPushButton* m_sendMessagePushButton;

   //! Detail model.
   DetailModel* m_detailModel;

   //! View for message details.
   QTableView* m_detailView;

   //! Drop-down box for target groups.
   QComboBox* m_targetGroupComboBox;

   //! Label for name of message type.
   QLabel* m_messageTypeLabel;

   //! Label for message description.
   QLineEdit* m_descriptionLineEdit;

   //! Button for adding detail fields for selected message.
   QPushButton* m_addRowPushButton;

   //! Selected row.
   int m_selectedRow;

   //! Bool indication if row has just been changed.
   //! This is required only internally.
   bool m_rowChanged;
};

} // MessageTool

#endif
