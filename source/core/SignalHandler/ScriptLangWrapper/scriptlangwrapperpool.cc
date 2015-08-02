
/* scriptlangwrapperpool.cc
 * 
 * This is the implementation file for the ScriptLangWrapperPool class that is 
 * defined in scriptlangwrapper.hh.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 01-Apri-2015
 * Last modified: 01-April-2015
 */

#include "scriptlangwrapperpool.hh"
#include "scriptlangwrapperfactory.hh"


namespace SignalHandler 
{


ScriptLangWrapperPool::ScriptLangWrapperPool(): data_(), mx_()
{
}


ScriptLangWrapperPool::~ScriptLangWrapperPool()
{
}


ScriptLangWrapperPool::ScriptLangWrapperPtr 
ScriptLangWrapperPool::reserve(const QString& langName)
{
    std::unique_lock<std::mutex> lock(mx_);
    
    auto iter = data_.find(langName);
    if (iter == data_.end()){
        // No available wrappers of requested type - create new instance.
        lock.unlock();
        ScriptLangWrapperFactory f;
        return ScriptLangWrapperPtr( f.getInstance(langName) );
    }
    
    // Remove and return one wrapper
    ScriptLangWrapperPtr rval(nullptr);
    rval.swap(iter->second);
    data_.erase(iter);
    lock.unlock();
    return rval;
}


void ScriptLangWrapperPool::
release(ScriptLangWrapperPool::ScriptLangWrapperPtr&& wrapper)
{
    // Move wrapper into the container.
    std::unique_lock<std::mutex> lock(mx_);
    data_.insert(std::make_pair(wrapper->getLangID(), std::move(wrapper)) );
    lock.unlock();
}

}
