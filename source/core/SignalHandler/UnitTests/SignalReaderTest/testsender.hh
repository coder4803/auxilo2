#ifndef TESTSENDER_HH
#define TESTSENDER_HH

#include <QObject>
#include <map>
#include "signalmessage.h"
#include "messagegroup.h"
#include <mutex>
#include <condition_variable>


class TestSender : public QObject
{
    Q_OBJECT
    
public:
    
    TestSender(const std::vector<Utils::SignalMessage>& messages);
    
    virtual ~TestSender();
    
    void startSending();
    
Q_SIGNALS:
    
    void SendMessage(QByteArray msg);
    void testFailed(const char* msg);
    
public Q_SLOTS:
    
    void onAckMessageReceived(QByteArray msg);
    
private Q_SLOTS:
    
    void onGroupReady();
    
private:
    
    Utils::MessageGroup* send_group_;
    Utils::MessageGroup* ack_group_;
    std::vector<Utils::SignalMessage> msgs_;
    std::mutex mx_;
    std::condition_variable cv_;
};

#endif // TESTSENDER_HH
