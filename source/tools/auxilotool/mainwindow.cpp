#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initView();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onEditEvents()
{

}


void MainWindow::onEditSignals()
{

}


void MainWindow::onUnimplemented()
{
    QMessageBox box(QMessageBox::Warning,
                    tr("Feature not implemented"),
                    tr("This feature has not been implemented yet."),
                    QMessageBox::Ok, this);
    box.exec();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    // Check if there are unimplemented changes etc...
    qDebug() << "Quit!";
    event->setAccepted(true);
}


void MainWindow::initView()
{
    // Different ways to exit.
    connect(ui->quitBtn, SIGNAL(clicked(bool)), this, SLOT(close()) );
    connect(ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(close()) );
    ui->actionQuit->setShortcuts({QKeySequence::Quit, QKeySequence::Close});

    // Unimplemented features:
    connect(ui->editEventsBtn, SIGNAL(clicked(bool)), this, SLOT(onUnimplemented()) );
    connect(ui->actionEditEvents, SIGNAL(triggered(bool)), this, SLOT(onUnimplemented()) );

    connect(ui->editSignalsBtn, SIGNAL(clicked(bool)), this, SLOT(onUnimplemented()) );
    connect(ui->actionEditSignals, SIGNAL(triggered(bool)), this, SLOT(onUnimplemented()) );
}
