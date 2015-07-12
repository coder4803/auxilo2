#include "signalhandlerbuilder.hh"
#include "scriptbank.hh"
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
    std::unique_ptr<ScriptBankInterface> bank(new ScriptBank);
    std::shared_ptr<SignalQueue> queue(new SignalQueue);
    std::unique_ptr<ConfigurationReader> conf_reader(new ConfigurationReader);
    std::unique_ptr<SignalReader> sig_reader(new SignalReader(queue, 
                                                              bank.get()) );
    std::shared_ptr<ScriptLangWrapperPool> pool(new ScriptLangWrapperPool);
    
    std::vector<std::unique_ptr<ScriptRunner> > workers;
    for (unsigned i=0; i<3; ++i){
        workers.push_back(std::unique_ptr<ScriptRunner>
                          (new ScriptRunner(queue, bank.get(), pool)) );
    }
    
    return new BusinessLogic(std::move(sig_reader), std::move(conf_reader), 
                             std::move(workers), std::move(bank));
}



} // Namespace SignalHandler
