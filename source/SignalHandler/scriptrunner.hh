#ifndef SCRIPTRUNNER_HH
#define SCRIPTRUNNER_HH

#include "interfaces/scriptupdateobserver.hh"
#include "interfaces/scriptupdatesubject.hh"
#include "interfaces/scriptlibrary.hh"
#include "ScriptLangWrapper/scriptlangwrapperpool.hh"
#include "signalqueue.hh"
#include <mutex>


namespace SignalHandler
{

/*!
 * \brief The ScriptRunner class
 *  This class Runs requested scripts.
 */
class ScriptRunner : public ScriptUpdateObserver
{
public:
    
    ScriptRunner(SignalQueue* queue, 
                 const ScriptLibrary* lib,
                 ScriptUpdateSubject* subject,
                 ScriptLangWrapperPool* pool);
    
    ~ScriptRunner();
    
    void start();
    
    void notifyOnScriptUpdate(const ScriptLibrary* new_lib);
    
    
private:
    
    SignalQueue* queue_;
    const ScriptLibrary* lib_;
    ScriptUpdateSubject* subject_;
    ScriptLangWrapperPool* pool_;
    int runner_id_;
    std::mutex update_mx_;
    
    static int runner_counter_;
    
};

}

#endif // SCRIPTRUNNER_HH
