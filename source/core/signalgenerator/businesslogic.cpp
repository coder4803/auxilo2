/* businesslogic.cpp
 *
 * This is the implementation file for the BusinessLogic class that is defined
 * in businesslogic.h.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 13-Nov-2015
 */

#include "businesslogic.h"
#include "userinterface.h"
#include <QException>

namespace SignalGenerator
{

BusinessLogic::BusinessLogic(bool clearEvents, QObject *parent):
    QObject(parent), ModelInterface(),
    confReader_(), eventManager_(), receiver_(&eventManager_)
{
    if (!eventManager_.isValid()){
        qFatal("Could not open database.");
        return;
    }
    if (clearEvents){
        eventManager_.clearAll();
    }
    UserInterface::getInstance()->setModel(this);

    connect(&confReader_, SIGNAL(configurationReady()),
            this, SLOT(onConfigurationReceived()) );

    confReader_.start();
}


BusinessLogic::~BusinessLogic()
{
}


QSqlTableModel *BusinessLogic::getEventTable()
{
    return eventManager_.getTableModel();
}


QSqlQueryModel *BusinessLogic::getTaskList()
{
    return eventManager_.getTaskList();
}


void BusinessLogic::onConfigurationReceived()
{
    qDebug() << "Setting new conf...";
    bool ok = eventManager_.setStaticEvents( confReader_.getConfiguration() );
    if (!ok){
        qCritical() << "Setting new configuration failed.";
        confReader_.restart();
    }
    else if (!receiver_.isStarted()){
        qDebug() << "New conf set!";
        receiver_.start();
    }
}

} // Namespace SignalGenerator
