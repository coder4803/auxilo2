/* qtscriptwrapper.hh
 * 
 * This header defines the QtScriptWrapper class that is responsible for 
 * executeing QScript code.
 * 
 * Author: Perttu Paarlahti:    perttu.paarlahti@gmail.com
 * Last Modified: 27-May-2015
 */

#ifndef QTSCRIPTWRAPPER_HH
#define QTSCRIPTWRAPPER_HH

#include "scriptlangwrapper.hh"
#include "../exceptions/scriptrunexceptions.hh"
#include <QtScript/QScriptEngine>

namespace SignalHandler 
{

/*!
 * \brief The QtScriptWrapper class
 * This class is responsible for executeing QtScript code.
 */
class QtScriptWrapper : public ScriptLangWrapper
{
public:
    
    //! QtScript langueage ID
    static const char* LANG_NAME;
    
    /*!
     * \brief Constructor
     * \pre -
     * \post New QtScriptWrapper is ready to be used.
     */
    QtScriptWrapper();
    
    /*!
     * \brief Destructor.
     */
    virtual ~QtScriptWrapper();
    
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
     * \exception BadScript if script was invalid to be executed.
     */
    virtual int run(const QString& script, const QStringList& args,
                    ScriptAPI* services);
    
    /*!
     * \brief return QtScript language ID.
     * \return QtScriptWrapper::LANG_NAME.
     */
    virtual QString getLangID() const;
    
};

} // Namespace SignalHandler.

#endif // QTSCRIPTWRAPPER_HH
