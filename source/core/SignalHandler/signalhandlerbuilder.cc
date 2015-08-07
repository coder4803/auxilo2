/* signalhandlerbuilder.cc
 * This is the implementation file for the SignalHandlerBuilder class defined
 * if signalhandlerbuilder.hh.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#include "signalhandlerbuilder.hh"
#include "scriptbank.hh"
#include "scriptbankbuilder.hh"
#include "signalqueue.hh"
#include "communication/configurationreader.hh"
#include "communication/signalreader.hh"
#include "scriptrunner.hh"
#include "scriptrunnerpool.hh"
#include "businesslogic.hh"
#include "configuration.hh"
#include "connection/connection.h"
#include <QDebug>

namespace SignalHandler
{

SignalHandlerBuilder::SignalHandlerBuilder()
{
}


SignalHandlerBuilder::~SignalHandlerBuilder()
{    
}


ModelInterface* SignalHandlerBuilder::create(const QString& serverAddress)
{
    Utils::Connection::setHost(serverAddress);

    // Create empty ScriptBank for initial configuration.
    std::unique_ptr<ConfigurationReader> conf_reader(new ConfigurationReader);
    std::unique_ptr<ScriptBankInterface> bank( ScriptBankBuilder::create() );
    
    // Create other central components
    std::shared_ptr<SignalQueue> queue(new SignalQueue);
    std::unique_ptr<SignalReader> sig_reader(new SignalReader(queue, 
                                                              bank.get()) );
    std::shared_ptr<ScriptLangWrapperPool> pool(new ScriptLangWrapperPool);
    
    std::unique_ptr<ScriptRunnerPool> worker_pool(new ScriptRunnerPool(queue,
                                                                       bank.get(),
                                                                       pool));
    
    return new BusinessLogic(std::move(sig_reader), std::move(conf_reader), 
                             std::move(worker_pool), std::move(bank));
}

} // Namespace SignalHandler
