
/* scriptlangwrapperpool.hh
 * 
 * This header defines the ScriptLangWrapperPool class, the concurrent object 
 * pool for ScriptLangWrapper subclass objects.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 01-Apri-2015
 * Last modified: 01-April-2015
 */

#ifndef SCRIPTLANGWRAPPERPOOL_HH
#define SCRIPTLANGWRAPPERPOOL_HH

#include "scriptlangwrapper.hh"
#include <memory>
#include <map>
#include <mutex>
#include <QString>


namespace SignalHandler {


/*!
 * \brief The ScriptLangWrapperPool class
 *  Concurrent object pool for ScriptLangWrapper objects.
 */
class ScriptLangWrapperPool
{
public:
    
    typedef std::unique_ptr<ScriptLangWrapper> ScriptLangWrapperPtr;
    
    //! Constructor
    ScriptLangWrapperPool();
    //! Destructor
    ~ScriptLangWrapperPool();
    
    //! Copy- and move-constructor are forbidden.
    ScriptLangWrapperPool(const ScriptLangWrapperPool&) = delete;
    ScriptLangWrapperPool(ScriptLangWrapperPool&&) = delete;
    
    //! Copy- and move-assignment operators are forbidden.
    ScriptLangWrapperPool& operator=(const ScriptLangWrapperPool&) = delete;
    ScriptLangWrapperPool& operator=(ScriptLangWrapperPool&&) = delete;
    
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
    ScriptLangWrapperPtr reserve(const QString& langName);
    
    /*!
     * \brief release Return wrapper back to the pool.
     * \param wrapper Wrapper to be released.
     * \pre wrapper != nullptr.
     * \post wrapper is ready to be reserved again.
     * 
     * Thread safety: This method is thread safe.
     */
    void release(ScriptLangWrapperPtr&& wrapper);
    
    
private:
    
    std::multimap<QString, ScriptLangWrapperPtr> data_;
    std::mutex mx_;
};


} // Namespace SignalHandler

#endif // SCRIPTLANGWRAPPERPOOL_HH
