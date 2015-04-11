
/* scriptlangwrapperfactory.cc
 * 
 * This is the implementation file for the ScriptLangWrapperFactory class 
 * defined in scriptlangwrapperfactory.hh
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 01-April-2015
 * Last modified: 01-April-2015
 */

// Add headers of concrete wrapper classes here
// --------------------------------------------

// --------------------------------------------

#include "scriptlangwrapperfactory.hh"
#include <QDebug>

namespace SignalHandler 
{


ScriptLangWrapper*
ScriptLangWrapperFactory::getInstance(const QString& langName) const
{
    Q_UNUSED(langName);
    /*   
    if (langName == "Python"){
        return new PythonWrapper;
    }
    else if (langName == "QtScript"){
        return new QtScriptWrapper;
    }
    */
    
    // Add else-if -branches for new wrapper classes here
    
    // Language is not supported.
    return nullptr;
    
}



}
