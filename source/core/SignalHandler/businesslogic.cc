/* businesslogic.cc
 * 
 * This is the implementation file for the BusinessLogic class defined in
 * businesslogic.cc.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#include "businesslogic.hh"
#include "scriptbankbuilder.hh"
#include "configuration.hh"
#include <QDebug>

namespace SignalHandler
{

BusinessLogic::BusinessLogic(std::unique_ptr<SignalReader>&& signal_reader, 
                             std::unique_ptr<ConfigurationReader>&& conf_reader, 
                             std::unique_ptr<ScriptRunnerPool>&& worker_pool, 
                             std::unique_ptr<ScriptBankInterface>&& library, 
                             QObject* parent) : 
    QObject(parent), ModelInterface(), ScriptUpdateSubject(), 
    PriorityUpdateSubject(), sig_reader_( std::move(signal_reader) ), 
    conf_reader_( std::move(conf_reader) ), worker_pool(std::move(worker_pool)), 
    library_( std::move(library) ), script_observers_(), 
    priority_observers_(), threads_(), workers_(), mx_()
{
    // Request initial configuration.
    sig_reader_->setPrioritySubject(this);
    connect(conf_reader_.get(), SIGNAL(configurationUpdated()),
            this, SLOT(onConfigurationReceived()), Qt::DirectConnection);
    conf_reader_->start();
}


BusinessLogic::~BusinessLogic()
{
    // Finish joinable threads.
    for (unsigned i=0; i<workers_.size(); ++i){
        workers_[i]->stop();
    }
    for (unsigned i=0; i<threads_.size(); ++i){
        threads_[i].join();
    }
    threads_.clear();
}


void BusinessLogic::start()
{
    qDebug() << "Starting...";
    
    threads_.resize( workers_.size() );
    
    for (unsigned i=0; i<workers_.size(); ++i){
        threads_[i] = std::move(std::thread(&ScriptRunner::start, 
                                            workers_[i].get()) );
    }
}


void BusinessLogic::stop()
{
    // Stop receiving signals and finish joinable threads.
    sig_reader_->stop();
    for (unsigned i=0; i<workers_.size(); ++i){
        workers_[i]->stop();
    }
    for (unsigned i=0; i<threads_.size(); ++i){
        threads_[i].join();
    }
    threads_.clear();
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
    Utils::ParameterSet conf = conf_reader_->getConfiguration();
    std::unique_ptr<ScriptBankInterface> new_bank;
    try{
        new_bank.reset(ScriptBankBuilder::create(conf));
    }
    catch(ScriptBankBuilderError& e){
        qDebug() << "Configuration failed!";
        qDebug() << "Error with configuration:" << e.getMessage();
        conf_reader_->restart();
        return;
    }
    
    // 2. Check worker count. Destroy or create more if neccessary.
    unsigned worker_count = Conf::DEFAULT_WORKERS;
    if (conf.contains(Conf::WORKERS_TAG)){
        worker_count = conf.parameter<unsigned>(Conf::WORKERS_TAG);
    }
    while (worker_count < workers_.size()){
        // Destroy extra workers.
        workers_.back()->stop();
        threads_.back().join();
        threads_.pop_back();
        workers_.pop_back();
    }
    while (worker_count > workers_.size()){
        // Create more workers.
        workers_.push_back( std::move( worker_pool->reserve() ) );
        workers_.back()->setScriptUpdateSubject(this);
        workers_.back()->notifyOnScriptUpdate(library_.get());
        threads_.push_back( std::thread(&ScriptRunner::start,
                                        workers_.back().get() ) );
    }
    
    // 3. Notify all observers.
    std::unique_lock<std::mutex> lock(mx_);
    for (PriorityUpdateObserver* obs : priority_observers_){
        obs->notifyOnPriorityUpdate( new_bank.get() );
    }
    for (ScriptUpdateObserver* obs : script_observers_){
        obs->notifyOnScriptUpdate( new_bank.get() );
    }
    
    // 4. replace old library with the new one.
    new_bank.swap(library_);
    lock.unlock();
    
    qDebug() << "New configuration has been set.";
    if (!sig_reader_->isStarted()){
        sig_reader_->start();
    }
    
}

} // Namespace SignalHandler

