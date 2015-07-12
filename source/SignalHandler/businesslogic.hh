#ifndef BUSINESSLOGIC_HH
#define BUSINESSLOGIC_HH

#include <QObject>
#include <unordered_set>
#include <memory>
#include <thread>
#include <mutex>
#include "interfaces/modelinterface.hh"
#include "interfaces/priorityupdateobserver.hh"
#include "interfaces/scriptupdateobserver.hh"
#include "interfaces/scriptbankinterface.hh"
#include "interfaces/scriptupdatesubject.hh"
#include "interfaces/priorityupdatesubject.hh"
#include "interfaces/viewinterface.hh"
#include "communication/signalreader.hh"
#include "communication/configurationreader.hh"
#include "scriptrunner.hh"


namespace SignalHandler
{

class BusinessLogic : 
        public QObject, 
        public SignalHandler::ModelInterface,
        public SignalHandler::ScriptUpdateSubject,
        public SignalHandler::PriorityUpdateSubject
{
    Q_OBJECT

public:

    BusinessLogic(std::unique_ptr<SignalReader>&& signal_reader,
                  std::unique_ptr<ConfigurationReader>&& conf_reader,
                  std::vector<std::unique_ptr<ScriptRunner> >&& workers,
                  std::unique_ptr<ScriptBankInterface>&& library,
                  QObject* parent = nullptr);
    
    //! Destructor
    virtual ~BusinessLogic();
    
    //! Copy-constructor is forbidden.
    BusinessLogic(const BusinessLogic&) = delete;
    //! Copy-assignment is forbidden.
    BusinessLogic& operator = (const BusinessLogic&) = delete;
    
    // Implements ModelInterface
    virtual void start();
    virtual void stop();
    
    // Implements the ScriptUpdateSubject interface.
    virtual void registerObserver(ScriptUpdateObserver* obs);
    virtual void unregisterObserver(ScriptUpdateObserver* obs);
    
    // Implements the PriorityUpdateSubjects interface.
    virtual void registerClient(PriorityUpdateObserver* client);
    virtual void unregisterClient(PriorityUpdateObserver* client);
    
    
private Q_SLOTS:
    
    void onConfigurationReceived();
    
    
private:
    
    std::unique_ptr<SignalReader> sig_reader_;
    std::unique_ptr<ConfigurationReader> conf_reader_;
    std::vector<std::unique_ptr<ScriptRunner> > workers_;
    std::unique_ptr<ScriptBankInterface> library_;
    std::unordered_set<ScriptUpdateObserver*> script_observers_;
    std::unordered_set<PriorityUpdateObserver*> priority_observers_;
    std::vector<std::thread> threads_;
    std::mutex mx_;
};

} // Namespace SignalHandler

#endif // BUSINESSLOGIC_HH
