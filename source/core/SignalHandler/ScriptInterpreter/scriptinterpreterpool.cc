
/* scriptinterpreterpool.cc
 * 
 * This is the implementation file for the ScriptInterpreterPool class that is 
 * defined in scriptinterpreterpool.hh.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 01-Apri-2015
 * Last modified: 01-April-2015
 */

#include "scriptinterpreterpool.hh"
#include "scriptinterpreterfactory.hh"


namespace SignalHandler 
{


ScriptInterpreterPool::ScriptInterpreterPool(): data_(), mx_()
{
}


ScriptInterpreterPool::~ScriptInterpreterPool()
{
}


ScriptInterpreterPool::InterpreterPtr 
ScriptInterpreterPool::reserve(const QString& langName)
{
    std::unique_lock<std::mutex> lock(mx_);
    
    auto iter = data_.find(langName);
    if (iter == data_.end()){
        // No available wrappers of requested type - create new instance.
        lock.unlock();
        ScriptInterpreterFactory f;
        return InterpreterPtr( f.getInstance(langName) );
    }
    
    // Remove and return one wrapper
    InterpreterPtr rval(nullptr);
    rval.swap(iter->second);
    data_.erase(iter);
    lock.unlock();
    return rval;
}


void ScriptInterpreterPool::
release(ScriptInterpreterPool::InterpreterPtr&& wrapper)
{
    // Move wrapper into the container.
    std::unique_lock<std::mutex> lock(mx_);
    data_.insert(std::make_pair(wrapper->getLangID(), std::move(wrapper)) );
    lock.unlock();
}

}
