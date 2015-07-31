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
    
    ScriptRunner(std::shared_ptr<SignalQueue> queue, 
                 const ScriptLibrary* lib,
                 std::shared_ptr<ScriptLangWrapperPool> pool,
                 ScriptUpdateSubject* subject = nullptr);
    
    ~ScriptRunner();
    
    void setScriptUpdateSubject(ScriptUpdateSubject* sub);
    
    void start();
    
    void stop();
    
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
