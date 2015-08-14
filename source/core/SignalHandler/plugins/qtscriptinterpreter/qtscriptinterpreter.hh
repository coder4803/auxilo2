/* qtscriptinterpreter.hh
 * 
 * This header defines the QtScriptInterpreter class that is responsible for 
 * executeing QScript code.
 * 
 * Author: Perttu Paarlahti:    perttu.paarlahti@gmail.com
 * Last Modified: 27-May-2015
 */

#ifndef QTSCRIPTINTERPRETER_HH
#define QTSCRIPTINTERPRETER_HH

#include "scriptinterpreter.hh"
#include "scriptrunexceptions.hh"
#include "scriptapi.hh"
#include <QtScript/QScriptEngine>

namespace QtScriptPlugin 
{


/*!
 * \brief The QtScriptWrapper class
 * This class is responsible for executeing QtScript code.
 */
class QtScriptInterpreter : public SignalHandler::ScriptInterpreter
{
public:
    
    /*!
     * \brief Constructor
     * \pre -
     * \post New QtScriptWrapper is ready to be used.
     */
    QtScriptInterpreter();
    
    /*!
     * \brief Destructor.
     */
    virtual ~QtScriptInterpreter();
    
    /*!
     * \brief Runs the given script using the given arguments and ScriptAPI.
     * \param script Script to be run.
     * \param args Script arguments.
     * \param services ScriptAPI object.
     * \return Script return value.
     * \pre services != nullptr.
     * \post Script is executed.
     * \post Exception guarantee: Strong guarantee.
     * \exception InvalidParameters if given arguments were invalid.
     * \exception BadScript if script was invalid.
     */
    virtual int run(const QString& script, const QStringList& args,
                    SignalHandler::ScriptAPI* services);
    
    /*!
     * \brief return QtScript language ID.
     * \return QtScriptWrapper::LANG_NAME.
     */
    virtual QString getLangID() const;   
};

} // Namespace QtScriptPlugin

#endif // QTSCRIPTINTERPRETER_HH
