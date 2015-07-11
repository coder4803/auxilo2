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
};

} // Namespace SignalHandler

#endif // SIGNALHANDLERBUILDER_HH
