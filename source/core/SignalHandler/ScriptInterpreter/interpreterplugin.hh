/* interpreterplugin.hh
 * 
 * This header file defines the InterpreterPlugin interface for script 
 * interpreter plugins.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 12-Aug-2015
 */

#ifndef INTERPRETERPLUGIN_HH
#define INTERPRETERPLUGIN_HH

//! InterpreterPlugin unique identifier.
#define SIGNALHANDLER_INTERPRETERPLUGIN_IID "auxilo2.signalhandler.InterpreterPlugin"

#include <QtPlugin>
#include "scriptinterpreter.hh"

namespace SignalHandler
{

/*!
 * \brief The InterpreterPlugin class
 *  Plugin interface for the script interpreters.
 */
class InterpreterPlugin
{
public:
    
    //! Destructor.
    virtual ~InterpreterPlugin() = default;
    
    /*!
     * \brief Create script interpreter.
     * \return New instance of ScriptLangWrapper (interpreter). Caller takes
     *  the ownership.
     */
    virtual ScriptInterpreter* createInterpreter() = 0;
    
    /*!
     * \brief Get language id (name).
     * \return Name of script language that interpreter is able to run.
     */
    virtual QString getLangId() const = 0;
};

} // Namespace SignalHandler

Q_DECLARE_INTERFACE (SignalHandler::InterpreterPlugin, 
                     SIGNALHANDLER_INTERPRETERPLUGIN_IID)

#endif // INTERPRETERPLUGIN_HH

