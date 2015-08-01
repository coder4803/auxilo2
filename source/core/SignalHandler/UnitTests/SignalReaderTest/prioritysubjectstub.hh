#ifndef PRIORITYOBSERVERSTUB_HH
#define PRIORITYOBSERVERSTUB_HH

#include "priorityupdatesubject.hh"
#include "priorityupdateobserver.hh"
#include <set>

class PrioritySubjectStub : public SignalHandler::PriorityUpdateSubject
{
public:
    
    PrioritySubjectStub();
    
    virtual ~PrioritySubjectStub();
    
    void registerClient(SignalHandler::PriorityUpdateObserver* client);
    
    void unregisterClient(SignalHandler::PriorityUpdateObserver* client);
    
    bool hasClient(SignalHandler::PriorityUpdateObserver* client);
    
private:
    
    std::set<SignalHandler::PriorityUpdateObserver*> observers_;
};

#endif // PRIORITYOBSERVERSTUB_HH
