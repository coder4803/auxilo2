
/* scriptlangwrapperfactory.hh
 * 
 * This is the header file for the ScriptLangWrapperFactory class that is
 * responsible for creating ScripLangWrapper subclass objects
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 01-April-2015
 * Last modified: 01-April-2015
 */

#ifndef SCRIPTLANGWRAPPERFACTORY_HH
#define SCRIPTLANGWRAPPERFACTORY_HH

#include "scriptlangwrapper.hh"
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
class ScriptLangWrapperFactory
{
public:
    
    //! Constructor
    ScriptLangWrapperFactory();
    //! Destructor
    ~ScriptLangWrapperFactory();
    
    /*!
     * \brief getInstance create new instance of ScriptLangWrapper.
     * \param langName Language identification string.
     * \return New ScriptLangWrapper subclass object. Ownership is passed to
     *  the caller. If langName does not match any supported languages,
     *  return nullptr.
     * \pre -
     */
    ScriptLangWrapper* getInstance(const QString& langName) const;
    
private:
    
    static QHash<QString, std::shared_ptr<InterpreterPlugin> > plugins_;
    static std::mutex mx_;
    
};

}

#endif // SCRIPTLANGWRAPPERFACTORY_HH
