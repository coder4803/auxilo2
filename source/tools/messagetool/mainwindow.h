#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>

#include "groupmodel.h"
#include "messagemodel.h"

namespace MessageTool {

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

private slots:
   void connectClicked();

private:
   QLineEdit* m_serverAddress;
   QLineEdit* m_serverPort;
   QPushButton* m_connect;
   QTabWidget* m_tabWidget;

   bool m_connected;
   GroupModel* m_groupModel;
   MessageModel* m_messageModel;
};

} // MessageTool

#endif
