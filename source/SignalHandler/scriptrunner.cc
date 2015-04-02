#include "scriptrunner.hh"
#include <QDebug>
#include "messagegroup.h"
#include "signalackmessage.h"
#include "exceptions/scriptrunexceptions.hh"


namespace SignalHandler
{

int ScriptRunner::runner_counter_ = 0;

ScriptRunner::ScriptRunner(SignalQueue* queue, 
                           const ScriptLibrary* lib, 
                           ScriptUpdateSubject* subject, 
                           ScriptLangWrapperPool* pool) :
    
    queue_(queue), lib_(lib), subject_(subject), pool_(pool),
    runner_id_(runner_counter_++)
{
    Q_ASSERT(queue != nullptr);
    Q_ASSERT(lib != nullptr);
    Q_ASSERT(subject != nullptr);
    
    subject_->registerObserver(this);
}


ScriptRunner::~ScriptRunner()
{   
    subject_->unregisterObserver(this);
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
            interpreter->run( script, s.getParameters() );
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
