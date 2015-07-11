#include "scriptrunner.hh"
#include <QDebug>
#include "messagegroup.h"
#include "signalackmessage.h"
#include "exceptions/scriptrunexceptions.hh"
#include "ScriptAPI/scriptapiimplementation.hh"


namespace SignalHandler
{

int ScriptRunner::runner_counter_ = 0;

ScriptRunner::ScriptRunner(std::shared_ptr<SignalQueue> queue, 
                           const ScriptLibrary* lib, 
                           std::shared_ptr<ScriptLangWrapperPool> pool, 
                           ScriptUpdateSubject* subject) :
    
    queue_(queue), lib_(lib), pool_(pool), subject_(subject),
    runner_id_(runner_counter_++), services_(nullptr), update_mx_()
{
    Q_ASSERT(queue != nullptr);
    Q_ASSERT(lib != nullptr);
    
    if (subject_ != nullptr){
        subject_->registerObserver(this);
    }
    services_ = new ScriptApiImplementation(lib_, subject_, 
                                            QString::number(runner_id_));
}


ScriptRunner::~ScriptRunner()
{   
    if (subject_ != nullptr){
        subject_->unregisterObserver(this);
    }
    delete services_;
}


void ScriptRunner::setScriptUpdateSubject(ScriptUpdateSubject* sub)
{
    Q_ASSERT(sub != nullptr);
    
    std::unique_lock<std::mutex> lock(update_mx_);
    if (subject_ != nullptr){
        subject_->unregisterObserver(this);
    }
    subject_ = sub;
    subject_->registerObserver(this);
    lock.unlock();
}


void ScriptRunner::start()
{
    typedef ScriptLangWrapperPool::ScriptLangWrapperPtr ScriptLangWrapperPtr;
    
    while (true)
    {
        Signal s = queue_->pop();
        
        std::unique_lock<std::mutex> lock(update_mx_);
        QString lang = lib_->getLanguage( s.getScriptID() );
        QString script = lib_->getScript( s.getScriptID() );
        lock.unlock();
        
        ScriptLangWrapperPtr interpreter = pool_->reserve(lang);
        if (interpreter == nullptr){
            // Error: Language not supported.
            Utils::SignalAckMessage ack_msg(s.getAckInfo().ackID,
                                            Utils::SignalAckMessage::FAILED);
            Utils::MessageGroup::publish(ack_msg, s.getAckInfo().ackGroup);
            continue;
        }
        
        try
        {
            interpreter->run( script, s.getParameters(), services_);
        }
        catch(InvalidParameters&)
        {
            // AckMessage: Invalid parameters.
            Utils::SignalAckMessage ack_msg(s.getAckInfo().ackID,
                                            Utils::SignalAckMessage::INVALID_PARAMETERS);
            Utils::MessageGroup::publish(ack_msg, s.getAckInfo().ackGroup);
            pool_->release( std::move(interpreter) );
            continue;
        }
        catch(BadScript&){
            // AckMessage: other error.
            Utils::SignalAckMessage ack_msg(s.getAckInfo().ackID,
                                            Utils::SignalAckMessage::FAILED);
            Utils::MessageGroup::publish(ack_msg, s.getAckInfo().ackGroup);
            pool_->release( std::move(interpreter) );
            continue;
        }
        
        pool_->release( std::move(interpreter) );
        
        // Send ackMessage: Success.
        Utils::SignalAckMessage ack_msg(s.getAckInfo().ackID,
                                        Utils::SignalAckMessage::SUCCEEDED);
        Utils::MessageGroup::publish(ack_msg, s.getAckInfo().ackGroup);
    }
}


void ScriptRunner::notifyOnScriptUpdate(const ScriptLibrary* new_lib)
{
    update_mx_.lock();
    lib_ = new_lib;
    update_mx_.unlock();
}



}
