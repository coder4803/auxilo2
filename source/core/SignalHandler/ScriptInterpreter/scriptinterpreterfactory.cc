
/* scriptinterpreterfactory.cc
 * 
 * This is the implementation file for the ScriptInterpreterFactory class 
 * defined in scriptinterpreterfactory.hh
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 01-April-2015
 * Last modified: 01-April-2015
 */

#include "scriptinterpreterfactory.hh"
#include "configuration.hh"
#include <QPluginLoader>
#include <QDebug>



namespace SignalHandler 
{

QHash<QString, std::shared_ptr<InterpreterPlugin> >
ScriptInterpreterFactory::plugins_;

std::mutex ScriptInterpreterFactory::mx_;


ScriptInterpreterFactory::ScriptInterpreterFactory()
{   
}


ScriptInterpreterFactory::~ScriptInterpreterFactory()
{
}


ScriptInterpreter*
ScriptInterpreterFactory::getInstance(const QString& langName) const
{   
    std::unique_lock<std::mutex> lock(mx_);
    if (plugins_.contains(langName)){
        // Proper plugin has been loaded.
        InterpreterPlugin* p = plugins_.value(langName).get();
        lock.unlock();
        return p->createInterpreter();
    }
    
    // Proper plugin has not been loaded, try to load it.
    QString fileName = Conf::PLUGIN_PATH + langName + "interpreter";
    QPluginLoader loader(fileName);
    if (!loader.load()){
        qWarning() << "Failed to load interpreter plugin: " << langName;
        return nullptr;
    }
    
    std::shared_ptr<InterpreterPlugin> p(nullptr);
    p.reset(qobject_cast<InterpreterPlugin*>( loader.instance() ) );
    if (p == nullptr){
        qWarning() << "Failed to cast interpreter plugin: " << langName;
        return nullptr;
    }
    
    plugins_.insert( langName, p );
    lock.unlock();
    return p->createInterpreter();
}


}
