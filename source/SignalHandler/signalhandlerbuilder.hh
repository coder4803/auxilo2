#ifndef SIGNALHANDLERBUILDER_HH
#define SIGNALHANDLERBUILDER_HH

#include "interfaces/modelinterface.hh"
#include "message.h"
#include <QObject>
#include <mutex>
#include <condition_variable>
#include <deque>


namespace SignalHandler
{

class SignalHandlerBuilder : public QObject
{
    Q_OBJECT
    
public:
    
    SignalHandlerBuilder();
    
    ~SignalHandlerBuilder();
    
    ModelInterface* create();
    
public slots:
    
    void onMessageReceived(const QByteArray& data);
    
private:
    
    std::mutex mx_;
    std::condition_variable cv_;
    std::deque<Utils::Message> received_;
};

} // Namespace SignalHandler

#endif // SIGNALHANDLERBUILDER_HH
