#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), SignalGenerator::ViewInterface(),
    ui(new Ui::MainWindow),
    verbose_(false), dbModel_(nullptr)
{
    ui->setupUi(this);
    ui->debugLabel->hide();
    ui->debugList->hide();
    ui->dbView->hide();

    connect(ui->exitBtn, SIGNAL(clicked(bool)), this, SLOT(onExitBtnClicked()) );
    this->show();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete dbModel_;
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


void MainWindow::setTableModel(QSqlTableModel *model)
{
    if (model == nullptr) return;

    delete dbModel_;
    dbModel_ = model;
    ui->dbView->setModel(dbModel_);
    ui->dbView->show();
}


void MainWindow::onExitBtnClicked()
{
    this->close();
}
