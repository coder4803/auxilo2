#include "businesslogic.h"
#include "userinterface.h"
#include <QException>

namespace SignalGenerator
{

BusinessLogic::BusinessLogic():
    confReader_(), eventManager_(nullptr)
{
    try{
        eventManager_ = new EventManager;
        UserInterface::getInstance()->setTableModel( eventManager_->getTableModel() );
    }
    catch (std::exception&){
        qFatal("Could not open database.");
        return;
    }

    connect(&confReader_, SIGNAL(configurationReady()),
            this, SLOT(onConfigurationReceived()) );

    confReader_.start();
}


BusinessLogic::~BusinessLogic()
{
    delete eventManager_;
}


void BusinessLogic::onConfigurationReceived()
{
    bool ok = eventManager_->setStaticEvents( confReader_.getConfiguration() );
    if (!ok){
        qCritical() << "Setting new configuration failed.";
        confReader_.restart();
    }
}

} // Namespace SignalGenerator
