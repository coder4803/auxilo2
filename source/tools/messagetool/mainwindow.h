#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>

#include "groupmodel.h"
#include "messagemodel.h"

namespace MessageTool {

/*!
 * \brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   //! Constructor.
   explicit MainWindow(QWidget *parent = 0);

   //! Destructor.
   ~MainWindow();

private slots:
   /*!
    * \brief Handles connect button click.
    */
   void connectClicked();

private:
   /*!
    * \brief Initializes connect related widgets.
    */
   void initBasicWidgets();

   /*!
    * \brief Initializes view widgets.
    * \return True on success, false on failure.
    */
   bool initViews();

   //! Edit field for server address.
   QLineEdit* m_serverAddress;

   //! Edit field for server port.
   QLineEdit* m_serverPort;

   //! Connect button.
   QPushButton* m_connect;

   //! Tab widget for different views.
   QTabWidget* m_tabWidget;

   //! Bool indicator for connection state.
   bool m_connected;

   //! Model for group list.
   GroupModel* m_groupModel;

   //! Model for messages.
   MessageModel* m_messageModel;
};

} // MessageTool

#endif
