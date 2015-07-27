#include "signalhandlerbuilder.hh"
#include "scriptbank.hh"
#include "scriptbankbuilder.hh"
#include "signalqueue.hh"
#include "communication/configurationreader.hh"
#include "communication/signalreader.hh"
#include "scriptrunner.hh"
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


ModelInterface* SignalHandlerBuilder::create()
{
    Utils::Connection::setHost("127.0.0.1");
    
    qDebug() << "Starting to build system.";
    // Get initial configuration.
    std::unique_ptr<ConfigurationReader> conf_reader(new ConfigurationReader);
    qDebug() << "Waiting for initial configuration...";
    conf_reader->start();
    qDebug() << "Configuration received!";
    Utils::ParameterSet conf = conf_reader->getConfiguration();
    std::unique_ptr<ScriptBankInterface> bank( ScriptBankBuilder::create(conf));
    
    // Create queue and signalreader.
    std::shared_ptr<SignalQueue> queue(new SignalQueue);
    std::unique_ptr<SignalReader> sig_reader(new SignalReader(queue, 
                                                              bank.get()) );
    std::shared_ptr<ScriptLangWrapperPool> pool(new ScriptLangWrapperPool);
    
    // Find number of workers.
    unsigned worker_count = Conf::DEFAULT_WORKERS;
    if (conf_reader->getConfiguration().contains(Conf::WORKERS_TAG)){
        worker_count = conf.parameter<unsigned>(Conf::WORKERS_TAG);
        if (worker_count == 0){
            throw std::domain_error("invalid number of workers.");
        }
    }
    
    // Create script runners.
    std::vector<std::unique_ptr<ScriptRunner> > workers;
    for (unsigned i=0; i<worker_count; ++i){
        workers.push_back(std::unique_ptr<ScriptRunner>
                          (new ScriptRunner(queue, bank.get(), pool)) );
    }
    
    return new BusinessLogic(std::move(sig_reader), std::move(conf_reader), 
                             std::move(workers), std::move(bank));
}



} // Namespace SignalHandler
