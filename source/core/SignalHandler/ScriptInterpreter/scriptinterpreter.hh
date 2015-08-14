
/* scriptinterpreter.hh
 * 
 * This header defines the ScriptInterpreter interface for script language 
 * interpreters.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 01-April-2015
 * Last modified: 01-April-2015
 */

#ifndef SCRIPTINTERPRETER_HH
#define SCRIPTINTERPRETER_HH

#include <QString>
#include <QStringList>
#include "scriptapi.hh"


namespace SignalHandler
{

/*!
 * \brief The ScriptLangWrapper class
 *  Abstract base for supported script language interpreters.
 */
class ScriptInterpreter
{
public:
    
    //! Constructor
    ScriptInterpreter() = default;
    //! Destructor
    virtual ~ScriptInterpreter() = default;
    //! Copy-constructor is forbidden.
    ScriptInterpreter(const ScriptInterpreter&) = delete;
    //! Copy-assignment operator is forbidden.
    ScriptInterpreter& operator=(const ScriptInterpreter&) = delete;
    
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

#endif // SCRIPTINTERPRETER_HH
