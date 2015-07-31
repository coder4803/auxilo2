#include "scriptrunnerpool.hh"
#include <QDebug>

namespace SignalHandler
{

ScriptRunnerPool::
ScriptRunnerPool(std::shared_ptr<SignalQueue> queue, 
                 const ScriptLibrary* library, 
                 std::shared_ptr<ScriptLangWrapperPool> interpreter_pool) :
    runners_(), queue_(queue), library_(library), 
    wrapper_pool_(interpreter_pool)
{
}


ScriptRunnerPool::~ScriptRunnerPool()
{
}


std::unique_ptr<ScriptRunner> ScriptRunnerPool::reserve()
{
    std::unique_ptr<ScriptRunner> rv(nullptr);
    if (runners_.empty()){
        rv.reset(new ScriptRunner(queue_, library_, wrapper_pool_, nullptr));
    }
    else{
        rv.swap(runners_.back());
        runners_.pop_back();
    }
    return rv;
}


void ScriptRunnerPool::release(std::unique_ptr<ScriptRunner>&& runner)
{
    runners_.push_back(std::move(runner));
}

}
