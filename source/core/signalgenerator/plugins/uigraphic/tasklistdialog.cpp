#include "tasklistdialog.h"
#include "ui_tasklistdialog.h"

TaskListDialog::TaskListDialog(SignalGenerator::ModelInterface *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskListDialog),
    model_(model)
{
    ui->setupUi(this);
    ui->taskListView->setModel(model_->getTaskList());

    connect(ui->closeBtn, SIGNAL(clicked(bool)), this, SLOT(close()) );

    connect(ui->updateBtn, SIGNAL(clicked(bool)),
            this, SLOT(onUpdateClicked()));
}


TaskListDialog::~TaskListDialog()
{
    delete ui;
}


void TaskListDialog::onUpdateClicked()
{
    ui->taskListView->setModel(model_->getTaskList());
}
