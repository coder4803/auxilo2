
/* signalreader.hh
 * 
 * This is the implementation file for the SignalReader class that is defined
 * in signalreader.hh.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmai.com
 * Created: 01-April-2015
 * Last modified: 01-April-2015
 */


#include "signalreader.hh"
#include "signalmessage.h"
#include <QDebug>


namespace SignalHandler
{

const char* SignalReader::GROUP_NAME = "SignalHandlerSignals";


SignalReader::SignalReader(SignalQueue* queue, const ScriptPriorityLibrary* lib,
                           PriorityUpdateSubject* subject,
                           QObject* parent):
    QObject(parent), PriorityUpdateObserver(), 
    group_(nullptr), queue_(queue), lib_(lib), subject_(subject), update_mx_()
{
    Q_ASSERT (queue != nullptr);
    Q_ASSERT (lib != nullptr);
    Q_ASSERT (subject != nullptr);
    
    // Register to subject.
    subject_->registerClient(this);
}


void SignalReader::start()
{
    // Only one call is allowed in object lifetime.
    static bool first_time(true);
    Q_ASSERT(firts_time);
    first_time = false;
    
    // Host address
    Utils::Connection::setHost("127.0.0.1");
    // Create message group
    group_ = new Utils::MessageGroup(GROUP_NAME, 
                                     Utils::MessageGroup::Subscriber,
                                     this);
    // Connect signals
    connect(group_, SIGNAL(messageReceived(QByteArray, QString)),
            this, SLOT(onMessageReceived(QByteArray)));
}


SignalReader::~SignalReader()
{
    subject_->unregisterClient(this);
    // group_ is automaticly destroyed as a child QObject.
}


void SignalReader::notifyOnPriorityUpdate(const ScriptPriorityLibrary* new_lib)
{
    // Change used priority library.
    Q_ASSERT(new_lib != nullptr);
    update_mx_.lock();
    lib_ = new_lib;
    update_mx_.unlock();
}


void SignalReader::onMessageReceived(QByteArray data)
{
    Utils::SignalMessage msg(data);
    unsigned priority;
    
    try{
        std::lock_guard<std::mutex> lock(update_mx_);
        priority = lib_->getPriorityOf( msg.signalName() );
    }
    catch (UnknownScript&){
        // Send ackMessage: SIGNAL_NOT_FOUND
        Utils::MessageGroup ack(msg.ackGroup(), Utils::MessageGroup::Publisher);
        auto result = Utils::SignalAckMessage::SIGNAL_NOT_FOUND;
        Utils::SignalAckMessage ack_msg(msg.ackId(), result);
        ack.publish(ack_msg);
        return;
    }

    Signal s(priority, msg.signalName(), msg.parameters());
    queue_->push(s);
}

}
