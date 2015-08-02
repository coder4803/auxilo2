/* scriptrunner.hh
 * 
 * This header defines the ScriptRunner class that is responsiple to execute
 * scripts determined by received signals.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#ifndef SCRIPTRUNNER_HH
#define SCRIPTRUNNER_HH

#include "interfaces/scriptupdateobserver.hh"
#include "interfaces/scriptupdatesubject.hh"
#include "interfaces/scriptlibrary.hh"
#include "ScriptLangWrapper/scriptlangwrapperpool.hh"
#include "signalqueue.hh"
#include "ScriptAPI/scriptapi.hh"
#include <mutex>
#include <atomic>
#include <memory>


namespace SignalHandler
{

/*!
 * \brief The ScriptRunner class
 *  This class Runs requested scripts.
 */
class ScriptRunner : public ScriptUpdateObserver
{
public:
    
    /*!
     * \brief Constructor
     * \param queue SignalQueue, where the signals are fetched from.
     * \param lib Script library.
     * \param pool Script interpreter source.
     * \param subject ScriptUpdateSubject.
     * \pre queue != nullptr, lib != nullptr, pool != nullptr.
     */
    ScriptRunner(std::shared_ptr<SignalQueue> queue, 
                 const ScriptLibrary* lib,
                 std::shared_ptr<ScriptLangWrapperPool> pool,
                 ScriptUpdateSubject* subject = nullptr);
    
    /*!
     * \brief Destructor
     */
    ~ScriptRunner();
    
    /*!
     * \brief Sets the priority update subject to observe.
     * \param sub New subject.
     * \pre sub != nullptr.
     * \post ScriptRunner observes the new subject. Previous subject is 
     *  unregistered to.
     */
    void setScriptUpdateSubject(ScriptUpdateSubject* sub);
    
    /*!
     * \brief Start running scripts.
     * \pre Use different object for each thread. Subject has been set.
     * \post ScriptRunner reads scripts from the queue and runs them until
     *  stop() is called. This method is supposed to launch in its own thread.
     */
    void start();
    
    /*!
     * \brief Tell ScriptRunner to stop.
     * \pre None.
     * \post ScriptRunner stops at appropriate point.
     */
    void stop();
    
    // Implements the ScriptUpdateObserver interface.
    void notifyOnScriptUpdate(const ScriptLibrary* new_lib);
    
    
private:
    
    std::shared_ptr<SignalQueue> queue_;
    const ScriptLibrary* lib_;
    std::shared_ptr<ScriptLangWrapperPool> pool_;
    ScriptUpdateSubject* subject_;
    int runner_id_;
    ScriptAPI* services_;
    std::mutex update_mx_;
    std::atomic<bool> end_flag_;
    
    static int runner_counter_;
    
};

}

#endif // SCRIPTRUNNER_HH
