
/* scriptinterpreterpool.hh
 * 
 * This header defines the ScriptInterpreterPool class, the concurrent object 
 * pool for ScriptInterpreter subclass objects.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 01-Apri-2015
 * Last modified: 01-April-2015
 */

#ifndef SCRIPTINTERPRETERPOOL_HH
#define SCRIPTINTERPRETERPOOL_HH

#include "scriptinterpreter.hh"
#include <memory>
#include <map>
#include <mutex>
#include <QString>


namespace SignalHandler {


/*!
 * \brief The ScriptLangWrapperPool class
 *  Concurrent object pool for ScriptLangWrapper objects.
 */
class ScriptInterpreterPool
{
public:
    
    typedef std::unique_ptr<ScriptInterpreter> InterpreterPtr;
    
    //! Constructor
    ScriptInterpreterPool();
    //! Destructor
    ~ScriptInterpreterPool();
    
    //! Copy- and move-constructor are forbidden.
    ScriptInterpreterPool(const ScriptInterpreterPool&) = delete;
    ScriptInterpreterPool(ScriptInterpreterPool&&) = delete;
    
    //! Copy- and move-assignment operators are forbidden.
    ScriptInterpreterPool& operator=(const ScriptInterpreterPool&) = delete;
    ScriptInterpreterPool& operator=(ScriptInterpreterPool&&) = delete;
    
    /*!
     * \brief reserve retrive one interpreter.
     * \param langName Script language name.
     * \return Appropriate ScriptLangWrapper or nullptr, if language is not
     *  supported.
     * \pre -
     * \post No-one else can reserve thae same wrapper.
     * 
     * Thread safety: This method is thread safe.
     */
    InterpreterPtr reserve(const QString& langName);
    
    /*!
     * \brief release Return wrapper back to the pool.
     * \param wrapper Wrapper to be released.
     * \pre wrapper != nullptr.
     * \post wrapper is ready to be reserved again.
     * 
     * Thread safety: This method is thread safe.
     */
    void release(InterpreterPtr&& wrapper);
    
    
private:
    
    std::multimap<QString, InterpreterPtr> data_;
    std::mutex mx_;
};


} // Namespace SignalHandler

#endif // SCRIPTINTERPRETERPOOL_HH
