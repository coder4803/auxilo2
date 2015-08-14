
/* scriptinterpreterfactory.hh
 * 
 * This is the header file for the ScriptInterpreterFactory class that is
 * responsible for creating ScriptInterpreter subclass objects
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 01-April-2015
 * Last modified: 01-April-2015
 */

#ifndef SCRIPTINTERPRETERFACTORY_HH
#define SCRIPTINTERPRETERFACTORY_HH

#include "scriptinterpreter.hh"
#include "interpreterplugin.hh"
#include <QHash>
#include <mutex>
#include <memory>


namespace SignalHandler
{

/*!
 * \brief The ScripLangWrapperFactory class
 *  Factory class for ScripLangWrapper subclasses.
 */
class ScriptInterpreterFactory
{
public:
    
    //! Constructor
    ScriptInterpreterFactory();
    //! Destructor
    ~ScriptInterpreterFactory();
    
    /*!
     * \brief getInstance create new instance of ScriptLangWrapper.
     * \param langName Language identification string.
     * \return New ScriptLangWrapper subclass object. Ownership is passed to
     *  the caller. If langName does not match any supported languages,
     *  return nullptr.
     * \pre -
     */
    ScriptInterpreter* getInstance(const QString& langName) const;
    
private:
    
    static QHash<QString, std::shared_ptr<InterpreterPlugin> > plugins_;
    static std::mutex mx_;
    
};

}

#endif // SCRIPTINTERPRETERFACTORY_HH
