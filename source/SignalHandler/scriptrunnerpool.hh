#ifndef SCRIPTRUNNERPOOL_HH
#define SCRIPTRUNNERPOOL_HH

#include "scriptrunner.hh"
#include <memory>
#include <vector>

namespace SignalHandler
{

class ScriptRunnerPool
{
public:

    ScriptRunnerPool(std::shared_ptr<SignalQueue> queue,
                     const ScriptLibrary* library,
                     std::shared_ptr<ScriptLangWrapperPool> interpreter_pool);
    
    virtual ~ScriptRunnerPool();
    
    std::unique_ptr<ScriptRunner> reserve();
    
    void release(std::unique_ptr<ScriptRunner>&& runner);
    
    
private:
    
    std::vector<std::unique_ptr<ScriptRunner> > runners_;
    std::shared_ptr<SignalQueue> queue_;
    const ScriptLibrary* library_;
    std::shared_ptr<ScriptLangWrapperPool> wrapper_pool_;
};


}

#endif // SCRIPTRUNNERPOOL_HH
