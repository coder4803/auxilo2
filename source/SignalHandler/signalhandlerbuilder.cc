#include "signalhandlerbuilder.hh"
#include "scriptbank.hh"
#include "scriptbankbuilder.hh"
#include "signalqueue.hh"
#include "communication/configurationreader.hh"
#include "communication/signalreader.hh"
#include "scriptrunner.hh"
#include "businesslogic.hh"
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
    qDebug() << "Starting to build system.";
    // Get initial configuration.
    std::unique_ptr<ConfigurationReader> conf_reader(new ConfigurationReader);
    qDebug() << "Waiting for initial configuration...";
    conf_reader->start();
    qDebug() << "Configuration received!";
    Utils::ParameterSet conf = conf_reader->getConfiguration();
    std::unique_ptr<ScriptBankInterface> bank( ScriptBankBuilder::create(conf));
    
    // Create queue, signalreader and scriptrunners.
    std::shared_ptr<SignalQueue> queue(new SignalQueue);
    std::unique_ptr<SignalReader> sig_reader(new SignalReader(queue, 
                                                              bank.get()) );
    std::shared_ptr<ScriptLangWrapperPool> pool(new ScriptLangWrapperPool);
    
    unsigned worker_count = 3;
    if (conf_reader->getConfiguration().contains("workers")){
        worker_count = conf.parameter<unsigned>("workers");
    }
    std::vector<std::unique_ptr<ScriptRunner> > workers;
    for (unsigned i=0; i<worker_count; ++i){
        workers.push_back(std::unique_ptr<ScriptRunner>
                          (new ScriptRunner(queue, bank.get(), pool)) );
    }
    
    return new BusinessLogic(std::move(sig_reader), std::move(conf_reader), 
                             std::move(workers), std::move(bank));
}



} // Namespace SignalHandler
