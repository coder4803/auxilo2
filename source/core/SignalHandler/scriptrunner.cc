/* scriptrunner.cc
 * 
 * This is the implementation file for the ScriptRunner class defined in
 * scriptrunner.hh.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#include "scriptrunner.hh"
#include <QDebug>
#include "messagegroup.h"
#include "signalackmessage.h"
#include "ScriptInterpreter/scriptrunexceptions.hh"
#include "ScriptInterpreter/scriptapiimplementation.hh"


namespace SignalHandler
{

int ScriptRunner::runner_counter_ = 0;

ScriptRunner::ScriptRunner(std::shared_ptr<SignalQueue> queue, 
                           const ScriptLibrary* lib, 
                           std::shared_ptr<ScriptInterpreterPool> pool, 
                           ScriptUpdateSubject* subject) :
    
    queue_(queue), lib_(lib), pool_(pool), subject_(subject),
    runner_id_(runner_counter_++), services_(nullptr), update_mx_(),
    end_flag_(false)
{
    Q_ASSERT(queue != nullptr);
    Q_ASSERT(lib != nullptr);
    Q_ASSERT(pool != nullptr);
    
    if (subject_ != nullptr){
        subject_->registerObserver(this);
    
        services_ = new ScriptApiImplementation(lib_, subject_, 
                                                QString::number(runner_id_));
    }
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
    
    if (subject_ != nullptr){
        subject_->unregisterObserver(this);
        delete services_;
    }
    subject_ = sub;
    subject_->registerObserver(this);
    services_ = new ScriptApiImplementation(lib_, subject_, 
                                            QString::number(runner_id_));
}


void ScriptRunner::start()
{
    Q_ASSERT (subject_ != nullptr);
    
    typedef ScriptInterpreterPool::InterpreterPtr ScriptLangWrapperPtr;
    end_flag_.store(false);
    qDebug() << "Worker" << runner_id_ << "Started";
    
    while (!end_flag_.load())
    {
        Signal s;
        if ( !queue_->try_pop(s, std::chrono::milliseconds(1000) ) ){
            continue;
        }
        
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
        
        // Set sender to ScriptApi
        QString sender = s.getAckInfo().senderName;
        dynamic_cast<ScriptApiImplementation*>(services_)->setSender(sender);
        // Run the script.
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


void ScriptRunner::stop()
{
    end_flag_.store(true);
}


void ScriptRunner::notifyOnScriptUpdate(const ScriptLibrary* new_lib)
{
    update_mx_.lock();
    lib_ = new_lib;
    update_mx_.unlock();
}

} // Namespace SignalHandler.
