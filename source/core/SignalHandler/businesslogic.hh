/* businesslogic.hh
 * 
 * This header file defines the SignalHandler::BusinessLogic class that works
 * as a hub class binding central SignalHandler components togeather.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

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
#include "communication/signalreader.hh"
#include "communication/configurationreader.hh"
#include "scriptrunnerpool.hh"
#include "scriptrunner.hh"


namespace SignalHandler
{

/*!
 * \brief The BusinessLogic class
 *  This is a 'hub class' binding central objects togeather. Object of this 
 *  class controls configuration updates and program lifetime.
 */
class BusinessLogic : 
        public QObject, 
        public SignalHandler::ModelInterface,
        public SignalHandler::ScriptUpdateSubject,
        public SignalHandler::PriorityUpdateSubject
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor
     * \param signal_reader Receives signal messages.
     * \param conf_reader Receives configuration messages.
     * \param worker_pool Script runner pool.
     * \param library Initial script bank.
     * \param parent QObject's parent.
     */
    BusinessLogic(std::unique_ptr<SignalReader>&& signal_reader,
                  std::unique_ptr<ConfigurationReader>&& conf_reader,
                  std::unique_ptr<ScriptRunnerPool>&& worker_pool,
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
    
    /*!
     * \brief Reacts on new configuration message.
     * \post tries to set new configuration. On success, old Script bank is
     *  replaced and all observers are notified. On failure SignalHandler falls
     *  back using previous script bank.
     */
    void onConfigurationReceived();
    
    
private:
    
    std::unique_ptr<SignalReader> sig_reader_;
    std::unique_ptr<ConfigurationReader> conf_reader_;
    std::unique_ptr<ScriptRunnerPool> worker_pool;
    std::unique_ptr<ScriptBankInterface> library_;
    std::unordered_set<ScriptUpdateObserver*> script_observers_;
    std::unordered_set<PriorityUpdateObserver*> priority_observers_;
    std::vector<std::thread> threads_;
    std::vector<std::unique_ptr<ScriptRunner>> workers_;
    std::mutex mx_;
};

} // Namespace SignalHandler

#endif // BUSINESSLOGIC_HH
