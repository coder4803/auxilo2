
/* scriptupdatesubject.hh
 * 
 * This header file defines abstract interface for subjects of script library
 * updates.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 07-Mar-2015
 * Last modified: 07-Mar-2015
 */

#ifndef SCRIPTUPDATESUBJECT_HH
#define SCRIPTUPDATESUBJECT_HH

#include "scriptupdateobserver.hh"


namespace SignalHandler
{

/*!
 * \brief The ScriptUpdateSubject class
 * Abstract interface for subjects of script library updates.
 */
class ScriptUpdateSubject
{
public:
    
    //! Constructor
    ScriptUpdateSubject() = default;
    
    //! Destructor
    virtual ~ScriptUpdateSubject() = default;
    
    //! Copy-construction is forbidden.
    ScriptUpdateSubject(const ScriptUpdateSubject&) = delete;
    
    //! Copy-assignment is forbidden.
    ScriptUpdateSubject& operator=(const ScriptUpdateSubject&) = delete;
    
    /*!
     * \brief registerObserver Registers a new observer on subject.
     * \param obs Observer to be registered.
     * \pre obs is not a null-pointer. If observer registers itself, it has
     *  to unregister before being destroyed.
     * \post Observer is notified every time script library is changed.
     */
    virtual void registerObserver(ScriptUpdateObserver* obs) = 0;
    
    /*!
     * \brief unregisterObserver Unregisters an observer.
     * \param obs Observer to be unregistered.
     * \pre Observer has been registered to this subject earlier.
     * \post Unregistered observer is no more notified on script updates. 
     */
    virtual void unregisterObserver(ScriptUpdateObserver* obs) = 0;
};

} // Namespace SignalHandler


#endif // SCRIPTUPDATESUBJECT_HH
