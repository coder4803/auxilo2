
/* scriptupdateobserver.hh
 * 
 * This header file defines abstract interface for observing updates on
 * ScriptLibrary objects.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 07-Mar-2015
 * Last modified: 07-Mar-2015
 */

#ifndef SCRIPTUPDATEOBSERVER_HH
#define SCRIPTUPDATEOBSERVER_HH

#include "scriptlibrary.hh"

namespace SignalHandler
{

/*!
 * \brief The ScriptUpdateObserver class
 * Observer class for updates on ScriptLibrary objects.
 */
class ScriptUpdateObserver
{
    //! Constructor
    ScriptUpdateObserver() = default;
    
    //! Destructor
    virtual ~ScriptUpdateObserver() = default;
    
    //! Copy-construction is forbidden.
    ScriptUpdateObserver(const ScriptUpdateObserver&) = delete;
    
    //! Copy-assignment is forbidden.
    ScriptUpdateObserver& operator=(const ScriptUpdateObserver&) = delete;
    
    /*!
     * \brief notifyOnScriptUpdate Notifies observer on script updates.
     * \param new_lib Updated script library.
     * \pre new_lib is not a null-pointer.
     * \post Observer uses new_lib for quering scripts from this on.
     */
    virtual void notifyOnScriptUpdate(const ScriptLibrary* new_lib) = 0;
};

}

#endif // SCRIPTUPDATEOBSERVER_HH
