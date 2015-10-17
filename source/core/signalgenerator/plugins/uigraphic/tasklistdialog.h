#ifndef TASKLISTDIALOG_H
#define TASKLISTDIALOG_H

#include <QDialog>
#include "modelinterface.h"

namespace Ui {
class TaskListDialog;
}

class TaskListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskListDialog(SignalGenerator::ModelInterface* model,
                            QWidget *parent = 0);

    virtual ~TaskListDialog();


public slots:

    void onUpdateClicked();

private:

    Ui::TaskListDialog *ui;
    SignalGenerator::ModelInterface* model_;
};

#endif // TASKLISTDIALOG_H
