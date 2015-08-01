#ifndef PRIORITYLIBRARYSTUB_HH
#define PRIORITYLIBRARYSTUB_HH

#include "scriptprioritylibrary.hh"
#include <map>

class PriorityLibraryStub : public SignalHandler::ScriptPriorityLibrary
{
public:
    
    PriorityLibraryStub();
    
    virtual ~PriorityLibraryStub();
    
    virtual unsigned getPriorityOf(const QString &scriptID) const;
    
    void addScript(const QString& name, unsigned priority);
    
    void clear();
    
private:
    
    std::map<QString, unsigned> data_;
};

#endif // PRIORITYLIBRARYSTUB_HH
