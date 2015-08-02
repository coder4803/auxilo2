#include "testsender.hh"
#include <QDebug>

TestSender::TestSender(const std::vector<Utils::SignalMessage>& messages):
    QObject(), send_group_(nullptr), ack_group_(nullptr), msgs_(messages),
    mx_(), cv_()
{
}

TestSender::~TestSender()
{
    delete send_group_;
    delete ack_group_;
}


void TestSender::startSending()
{   
    std::unique_lock<std::mutex> lock(mx_);
    send_group_ = new Utils::MessageGroup("test_group", 
                                          Utils::MessageGroup::Publisher);
    ack_group_ = new Utils::MessageGroup("test_ack_group", 
                                         Utils::MessageGroup::Subscriber);
    
    connect(ack_group_, SIGNAL(messageReceived(QByteArray,QString)), 
            this, SLOT(onAckMessageReceived(QByteArray)) );
    connect(send_group_, SIGNAL(ready()), this, SLOT(onGroupReady()) );
    
    // Wait until group is ready.
    if (cv_.wait_for(lock, std::chrono::seconds(5)) == std::cv_status::timeout ){
        emit testFailed("Message group does not respond.");
        return;
    }
    
    // Send messages.
    for (auto it=msgs_.begin(); it != msgs_.end(); ++it){
        if (!send_group_->publish(*it) ){
            emit testFailed("Message group not ready!");
            return;
        }
    }
}

void TestSender::onAckMessageReceived(QByteArray msg)
{
    Q_UNUSED(msg);
    emit testFailed("Unknown script!");
}


void TestSender::onGroupReady()
{
    mx_.lock();
    cv_.notify_one();
    mx_.unlock();
}

