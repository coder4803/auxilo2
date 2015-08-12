/* graphicalui.cc
 * 
 * This is the implementation file for the GraphicalUi class defined in 
 * graphicalui.hh.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#include "graphicalui.hh"
#include "ui_graphicalui.h"
#include <QMessageBox>
#include <QDateTime>

GraphicalUI::GraphicalUI(bool verbose, QWidget *parent) :
    QMainWindow(parent), SignalHandler::ViewInterface(),
    ui(new Ui::GraphicalUI), verbose_(verbose),
    outputMx_(), debugMx_(), fatalMx_()
{
    ui->setupUi(this);
    if (!verbose_){
        ui->debugLabel->hide();
        ui->debugList->hide();
    }
    
    QObject::connect(ui->quitBtn, SIGNAL(clicked(bool)),
                     this, SLOT(onQuitBtnClicked()) );
    
    this->show();
}

GraphicalUI::~GraphicalUI()
{
    delete ui;
}

void GraphicalUI::critical(const QString& msg)
{
    QString time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
    std::lock_guard<std::mutex> lock(outputMx_);
    ui->outputList->addItem("(" + time + "): " + msg);
}

void GraphicalUI::debug(const QString& msg)
{
    if (verbose_){
        QString time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        std::lock_guard<std::mutex> lock(debugMx_);
        ui->debugList->addItem(QString("Debug: ") + " (" + time + "): " + msg);
    }
}

void GraphicalUI::warning(const QString& msg)
{
    if (verbose_){
        QString time = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
        std::lock_guard<std::mutex> lock(debugMx_);
        ui->debugList->addItem(QString("Debug: ") + " (" + time + "): " + msg);
    }
}

void GraphicalUI::fatal(const QString& msg)
{
    QMessageBox box(QMessageBox::Critical, "Fatal Error",
                    msg, QMessageBox::Ok, this);
    std::unique_lock<std::mutex> lock(fatalMx_);
    box.exec();
    lock.unlock();
    QApplication::exit(-1);
    this->close();
}

void GraphicalUI::onQuitBtnClicked()
{
    this->close();
}
