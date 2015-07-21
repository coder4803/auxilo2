#include "businesslogic.hh"
#include "scriptbankbuilder.hh"
#include <QDebug>

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
    library_( std::move(library) ), script_observers_(), 
    priority_observers_(), threads_(workers.size()), mx_()
{
    sig_reader_->setPrioritySubject(this);
    for (std::unique_ptr<ScriptRunner>& worker : workers_){
        worker->setScriptUpdateSubject(this);
    }
    
    connect(conf_reader.get(), SIGNAL(configurationUpdated()),
            this, SLOT(onConfigurationReceived()), Qt::DirectConnection);
}

BusinessLogic::~BusinessLogic()
{
    
}

void BusinessLogic::start()
{
    qDebug() << "Starting...";
    
    for (unsigned i=0; i<threads_.size(); ++i){
        threads_[i] = std::move(std::thread(&ScriptRunner::start, 
                                            workers_[i].get()) );
        threads_[i].detach();
    }
    
    qDebug() << "Workers are running.";
}

void BusinessLogic::stop()
{
    qDebug() << "Stopping...";
    sig_reader_->stop();
}

void BusinessLogic::registerObserver(ScriptUpdateObserver* obs)
{
    std::lock_guard<std::mutex> lock(mx_);
    script_observers_.insert(obs);
    obs->notifyOnScriptUpdate(library_.get());
}

void BusinessLogic::unregisterObserver(ScriptUpdateObserver* obs)
{
    std::lock_guard<std::mutex> lock(mx_);
    script_observers_.erase(obs);
}


void BusinessLogic::registerClient(PriorityUpdateObserver* client)
{
    std::lock_guard<std::mutex> lock(mx_);
    priority_observers_.insert(client);
    client->notifyOnPriorityUpdate(library_.get());
}

void BusinessLogic::unregisterClient(PriorityUpdateObserver* client)
{
    std::lock_guard<std::mutex> lock(mx_);
    priority_observers_.erase(client);
}

void BusinessLogic::onConfigurationReceived()
{
    qDebug() << "Setting new configuration...";
    
    // 1. Build new ScriptBank with confMessage.
    std::unique_ptr<ScriptBankInterface> new_bank;
    try{
        new_bank.reset(ScriptBankBuilder::create(conf_reader_->getConfiguration()));
    }
    catch(ScriptBankBuilderError& e){
        qDebug() << "Configuration failed!";
        qDebug() << "Error with configuration:" << e.getMessage();
        return;
    }
    
    // 2. Notify all observers.
    std::unique_lock<std::mutex> lock(mx_);
    for (PriorityUpdateObserver* obs : priority_observers_){
        obs->notifyOnPriorityUpdate( new_bank.get() );
    }
    for (ScriptUpdateObserver* obs : script_observers_){
        obs->notifyOnScriptUpdate( new_bank.get() );
    }
    
    // 3. replace old library with the new one.
    new_bank.swap(library_);
    lock.unlock();
    
    qDebug() << "New configuration has been set.";
    if (!sig_reader_->isStarted()){
        sig_reader_->start();
        qDebug() << "Ready to receive signals.";
    }
    
}



}

