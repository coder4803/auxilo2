#ifndef MESSAGEVIEWER_H
#define MESSAGEVIEWER_H

#include <QWidget>
#include <QTreeView>
#include <QTableView>
#include <QAbstractTableModel>
#include <QStandardItemModel>

#include "messagefilterproxymodel.h"
#include "detailmodel.h"

namespace MessageTool {

/*!
 * \brief The MessageViewer class.
 * This class represents "message view" view.
 */
class MessageViewer : public QWidget
{
   Q_OBJECT
public:
   /*!
    * \brief Constructor.
    * \param groupModel Reference to group model.
    * \param messageModel Reference to message model.
    * \param parent Parent QWidget.
    */
   explicit MessageViewer(QStandardItemModel& groupModel,
                          QStandardItemModel& messageModel,
                          QWidget* parent = NULL);

private slots:
   /*!
    * \brief Handles message selection. Copies it to detail model.
    * \param current Current message.
    */
   void onMessageSelected(QModelIndex current);

   /*!
    * \brief Handles group selection change.
    */
   void onGroupSelectionChanged();

private:
   /*!
    * \brief Initializes view's widgets.
    */
   void initWidgets();

   //! Reference to group model.
   QStandardItemModel& m_groupModel;

   //! View for group model.
   QTreeView* m_groupView;

   //! Reference to message model.
   QStandardItemModel& m_messageModel;

   //! Filter for message model.
   MessageFilterProxyModel* m_messageProxyModel;

   //! View for message model.
   QTableView* m_messageView;

   //! Detail model for messages.
   DetailModel* m_detailModel;

   //! View for detail model.
   QTableView* m_detailView;
};

} // MessageTool

#endif
