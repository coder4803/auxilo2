#include "businesslogic.hh"

namespace SignalHandler
{

BusinessLogic::BusinessLogic(std::unique_ptr<SignalReader>&& signal_reader, 
                             std::unique_ptr<ConfigurationReader>&& conf_reader, 
                             std::vector<std::unique_ptr<ScriptRunner>>&& workers, 
                             std::unique_ptr<ScriptBankInterface>&& library, 
                             QObject* parent) : 
    QObject(parent), ModelInterface(), ScriptUpdateSubject(), 
    PriorityUpdateSubject(), sig_reader_( std::move(signal_reader) ), 
    conf_reader_( std::move(conf_reader) ), workers_( std::move(workers) ), 
    library_( std::move(library) ), script_observers_(), priority_observers_()
{
    sig_reader_->setPrioritySubject(this);
    for (std::unique_ptr<ScriptRunner>& worker : workers_){
        worker->setScriptUpdateSubject(this);
    }
    
    connect(conf_reader.get(), SIGNAL(configurationUpdated()),
            this, SLOT(onConfigurationReceived()), Qt::DirectConnection);
    conf_reader_->start();
}

BusinessLogic::~BusinessLogic()
{
    
}

void BusinessLogic::start()
{
    
}

void BusinessLogic::stop()
{
    
}

void BusinessLogic::registerObserver(ScriptUpdateObserver* obs)
{
    script_observers_.insert(obs);
}

void BusinessLogic::unregisterObserver(ScriptUpdateObserver* obs)
{
    script_observers_.erase(obs);
}


void BusinessLogic::registerClient(PriorityUpdateObserver* client)
{
    priority_observers_.insert(client);
}

void BusinessLogic::unregisterClient(PriorityUpdateObserver* client)
{
    priority_observers_.erase(client);
}

void BusinessLogic::onConfigurationReceived()
{
    // 1. Build new ScriptBank with confMessage.
    // 2. Notify all observers.
    // 3. replace old library with the new one.
}



}

