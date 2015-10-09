#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "viewinterface.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public SignalGenerator::ViewInterface
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);

    virtual ~MainWindow();

    // Implements the SignalGenerator::ViewInterface.
    virtual void debug(const QString &msg);
    virtual void warning(const QString &msg);
    virtual void critical(const QString &msg);
    virtual void fatal(const QString &msg);
    virtual void setVerbose(bool value);
    virtual void setTableModel(QSqlTableModel *model);


private slots:

    void onExitBtnClicked();


private:

    Ui::MainWindow *ui;
    bool verbose_;
    QSqlTableModel* dbModel_;
};

#endif // MAINWINDOW_H
