#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), SignalGenerator::ViewInterface(),
    ui(new Ui::MainWindow),
    verbose_(false), model_(nullptr)
{
    ui->setupUi(this);
    ui->debugLabel->hide();
    ui->debugList->hide();
    ui->dbView->hide();

    ui->dbView->setEditTriggers(QTableView::NoEditTriggers);

    connect(ui->exitBtn, SIGNAL(clicked(bool)), this, SLOT(onExitBtnClicked()) );

    connect(ui->showTaskListBtn, SIGNAL(clicked(bool)),
            this, SLOT(onShowTaskListBtnClicked()) );

    this->show();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::debug(const QString &msg)
{
    if (verbose_){
        QString timestamp = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
        QString output = timestamp + " Debug: " + msg;
        ui->debugList->insertItem(0, output);
    }
}


void MainWindow::warning(const QString &msg)
{
    if (verbose_){
        QString timestamp = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
        QString output = timestamp + " Warning: " + msg;
        ui->debugList->insertItem(0, output);
    }
}


void MainWindow::critical(const QString &msg)
{
    QString timestamp = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm:ss");
    QString output = timestamp + " : " + msg;
    ui->outputList->insertItem(0, output);
}


void MainWindow::fatal(const QString &msg)
{
    QMessageBox box(QMessageBox::Critical,
                    "Fatal error", msg,
                    QMessageBox::Ok, this);
    box.exec();
    this->close();
}


void MainWindow::setVerbose(bool value)
{
    verbose_ = value;
    if (verbose_){
        ui->debugLabel->show();
        ui->debugList->show();
    }
}

void MainWindow::setModel(SignalGenerator::ModelInterface *model)
{
    Q_ASSERT(model != nullptr);
    model_ = model;
    ui->dbView->setModel(model_->getEventTable());
    ui->dbView->show();
}


void MainWindow::onExitBtnClicked()
{
    this->close();
}


void MainWindow::onShowTaskListBtnClicked()
{
    QMessageBox box(QMessageBox::Information,
                    "Task List",
                    "Task List not implemented yet.",
                    QMessageBox::Ok, this);
    box.exec();
}

