#include "scriptapiimplementation.hh"
#include "requeststatemessage.h"
#include "setstatemessage.h"
#include "ScriptLangWrapper/scriptlangwrapperfactory.hh"
#include "exceptions/scriptrunexceptions.hh"
#include <ctime>
#include <memory>
#include <chrono>

namespace SignalHandler 
{

ScriptApiImplementation::ScriptApiImplementation(const ScriptLibrary* lib, 
                                                 ScriptUpdateSubject* subject, 
                                                 const QString& namingSuffix):
    ScriptAPI(), ScriptUpdateObserver(), lib_(lib), subject_(subject),
    reqGroup_(nullptr), ackGroup_(nullptr),
    reqGroupName_(QString("SignalHandlerStateReq") + namingSuffix),
    ackGroupName_(QString("SignalHandlerStateAck") + namingSuffix),
    pendingReq_(), pendingAck_(0, Utils::StateChangedAckMessage::FAILED),
    updateMx_(), waitMx_(), cv_()
{
    subject->registerObserver(this);
    
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
    subject_->unregisterObserver(this);
    delete reqGroup_;
    delete ackGroup_;
}


ScriptAPI::DateTime ScriptApiImplementation::dateTimeNow() const
{
    std::time_t rawTime;
    std::time(&rawTime);
    std::tm* dt = localtime(&rawTime);
    
    return ScriptAPI::DateTime(dt->tm_mday, dt->tm_mon, dt->tm_year,
                               dt->tm_hour, dt->tm_min, dt->tm_sec);
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
        rv.insert(std::make_pair(it.key(), *it.value()));
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
    
    // Check if successed. StateChangeAckMessage has an insufficient interface.
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
    catch(InvalidParameters&){
        return -1;
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
    catch(...) {}
    
    return -1;
}


void ScriptApiImplementation::notifyOnScriptUpdate(const ScriptLibrary* new_lib)
{
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
    pendingAck_ = Utils::StateChangedAckMessage(data);
    waitMx_.unlock();
    cv_.notify_one();
}


}
