
/* scriptlangwrapper.hh
 * 
 * This header defines the ScriptLangWrapper interface for script language 
 * interpreters.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 01-April-2015
 * Last modified: 01-April-2015
 */

#ifndef SCRIPTLANGWRAPPER_HH
#define SCRIPTLANGWRAPPER_HH

#include <QString>
#include <QStringList>
#include "../ScriptAPI/scriptapi.hh"


namespace SignalHandler
{

/*!
 * \brief The ScriptLangWrapper class
 *  Abstract base for supported script language interpreters.
 */
class ScriptLangWrapper
{
public:
    
    //! Constructor
    ScriptLangWrapper() = default;
    //! Destructor
    virtual ~ScriptLangWrapper() = default;
    //! Copy-constructor is forbidden.
    ScriptLangWrapper(const ScriptLangWrapper&) = delete;
    //! Copy-assignment operator is forbidden.
    ScriptLangWrapper& operator=(const ScriptLangWrapper&) = delete;
    
    /*!
     * \brief run Run given script with given arguments.
     * \param script Script to be executed.
     * \param args Arguments to be passed to the script.
     * \return On successful run return 0.
     * \pre -
     * \post After executing script, the interpreter returns back to its
     *  initial state, and it is ready to run another script.
     */
    virtual int run(const QString& script, 
                    const QStringList& args,
                    ScriptAPI* services) = 0;
    
    /*!
     * \brief getLangID Return string identifying wrapped language.
     * \return Language identification string.
     */
    virtual QString getLangID() const = 0;
};

} // Namespace SignalHandler

#endif // SCRIPTLANGWRAPPER_HH
