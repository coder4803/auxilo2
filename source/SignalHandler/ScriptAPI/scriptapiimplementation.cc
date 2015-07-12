#include "scriptapiimplementation.hh"
#include "requeststatemessage.h"
#include "setstatemessage.h"
#include "ScriptLangWrapper/scriptlangwrapperfactory.hh"
#include "exceptions/scriptrunexceptions.hh"
#include <ctime>
#include <memory>
#include <chrono>
#include <QDebug>

namespace SignalHandler 
{

ScriptApiImplementation::ScriptApiImplementation(const ScriptLibrary* lib, 
                                                 ScriptUpdateSubject* subject, 
                                                 const QString& namingSuffix):
    ScriptAPI(), ScriptUpdateObserver(), lib_(lib), subject_(subject),
    reqGroup_(nullptr), ackGroup_(nullptr),
    reqGroupName_(QString("SignalHandlerStateReq") + namingSuffix),
    ackGroupName_(QString("SignalHandlerStateAck") + namingSuffix),
    pendingReq_(), pendingAck_(0, Utils::SetStateAckMessage::FAILED),
    updateMx_(), waitMx_(), cv_()
{
    Q_ASSERT(lib != nullptr);
    
    if (subject_ != nullptr){
        subject->registerObserver(this);
    }
    Utils::Connection::setHost("127.0.0.1");
    
    // Create Group for receiving states.
    reqGroup_ = new Utils::MessageGroup(reqGroupName_, 
                                        Utils::MessageGroup::Subscriber,
                                        this);
    connect(reqGroup_, SIGNAL(messageReceived(QByteArray,QString)),
            this, SLOT(onStateReqReturned(QByteArray)) );
    
    // Create Group for receiving state change acknowledgements.
    ackGroup_ = new Utils::MessageGroup(ackGroupName_,
                                        Utils::MessageGroup::Subscriber,
                                        this);
    connect(ackGroup_, SIGNAL(messageReceived(QByteArray,QString)),
            this, SLOT(onStateChangeAck(QByteArray)) );
}


ScriptApiImplementation::~ScriptApiImplementation()
{
    if (subject_ != nullptr){
        subject_->unregisterObserver(this);
    }
    delete reqGroup_;
    delete ackGroup_;
}


void ScriptApiImplementation::setSubject(ScriptUpdateSubject* sub)
{
    std::lock_guard<std::mutex> lock(updateMx_);
    if (subject_ != nullptr){
        subject_->unregisterObserver(this);
    }
    subject_ = sub;
    subject_->registerObserver(this);
}


QDateTime ScriptApiImplementation::dateTimeNow() const
{
    return QDateTime::currentDateTime();
}


Utils::StateResponseMessage::State 
ScriptApiImplementation::getStateOf(const QString& stateName)
{
    Utils::RequestStateMessage msg(reqGroupName_, stateName);
    Utils::MessageGroup::publish(msg, "StateHolder???");
    
    // Wait for response...
    std::unique_lock<std::mutex> lock(waitMx_);
    while ( !pendingReq_.contains(stateName) ){
        if (cv_.wait_for(lock, std::chrono::seconds(1)) == 
                std::cv_status::timeout){
            // Timeout
            return Utils::StateResponseMessage::State(false);
        }
    }
    
    return pendingReq_.state(stateName);
}


ScriptAPI::StateMap 
ScriptApiImplementation::getStates(const QStringList& states)
{
    Utils::RequestStateMessage msg(reqGroupName_, states);
    Utils::MessageGroup::publish(msg, "StateHolder???");
    
    // Wait for response...
    std::unique_lock<std::mutex> lock(waitMx_);  
    while (true)
    {
        if (cv_.wait_for(lock, std::chrono::seconds(1)) == 
                std::cv_status::timeout){
            // Timeout
            return ScriptAPI::StateMap();
        }
        // Check that all requested states are in response message.
        for (int i=0; i<states.size(); ++i){
            if ( !pendingReq_.contains(states.at(i)) ){
                continue;
            }
        }
        break;
    }
    
    // Convert to ScriptApi::StateMap.
    QHash<QString,Utils::StateResponseMessage::State*> res=pendingReq_.states();
    ScriptAPI::StateMap rv;
    for (auto it=res.begin(); it!=res.end(); ++it){
        rv.insert(it.key(), *it.value());
    }
    
    lock.unlock();
    return rv;
}


int ScriptApiImplementation::setState(const QString& stateName, 
                                      const QVariant& value)
{
    Utils::SetStateMessage msg(stateName, value, ackGroupName_);
    Utils::MessageGroup::publish(msg, "StateHolder???");
    
    // Wait for response...
    std::unique_lock<std::mutex> lock(waitMx_);
    while (pendingAck_.ackId() != msg.ackId() ){
        if (cv_.wait_for(lock, std::chrono::seconds(1)) == 
                std::cv_status::timeout){
            // Timeout
            return -1;
        }
    }
    
    // Check if succeeded
    /*
    if (pendingAck_.result() != Utils::SetStateAckMessage::SUCCEEDED){
        return 1;
    }
    */
    return 0;
}

int ScriptApiImplementation::sendSignal(const QString& signalName, 
                                        const QStringList& args)
{
    // Get script info
    QString lang, script;
    try {
        std::unique_lock<std::mutex> lock(updateMx_);
        lang = lib_->getLanguage(signalName);
        script = lib_->getScript(signalName);
        lock.unlock();
    }
    catch(BadScript&){
        return -1;
    }
    
    // Run script.
    std::unique_ptr<ScriptLangWrapper> interpreter;
    interpreter.reset( ScriptLangWrapperFactory().getInstance(lang) );
    if (interpreter == nullptr){
        // Language not supported
        return -1;
    }
    
    try{
        return interpreter->run(script, args, this);
    }
    catch(BadScript&){
        return -1;
    }
    
    return 0;
}


void ScriptApiImplementation::notifyOnScriptUpdate(const ScriptLibrary* new_lib)
{
    Q_ASSERT(new_lib != nullptr);
    updateMx_.lock();
    lib_ = new_lib;
    updateMx_.unlock();
}


void ScriptApiImplementation::onStateReqReturned(const QByteArray& data)
{
    waitMx_.lock();
    pendingReq_ = Utils::StateResponseMessage(data);
    waitMx_.unlock();
    cv_.notify_one();
}


void ScriptApiImplementation::onStateChangeAck(const QByteArray& data)
{
    waitMx_.lock();
    pendingAck_ = Utils::SetStateAckMessage(data);
    waitMx_.unlock();
    cv_.notify_one();
}


}
