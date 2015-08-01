#include "prioritysubjectstub.hh"


PrioritySubjectStub::PrioritySubjectStub() : 
    PriorityUpdateSubject(), observers_()
{
}


PrioritySubjectStub::~PrioritySubjectStub()
{
}


void PrioritySubjectStub::
registerClient(SignalHandler::PriorityUpdateObserver* client)
{
    observers_.insert(client);
}


void PrioritySubjectStub::
unregisterClient(SignalHandler::PriorityUpdateObserver* client)
{
    observers_.erase(client);
}


bool PrioritySubjectStub::
hasClient(SignalHandler::PriorityUpdateObserver* client)
{
    return observers_.find(client) != observers_.end();
}


