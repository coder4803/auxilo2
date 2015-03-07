
/* priorityupdatesubject.hh
 * 
 * This header file defines abstract interface for subjects of updates on
 * ScriptPriorityLibrary objects.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 07-Mar-2015
 * Last modified: 07-Mar-2015
 */

#ifndef PRIORITYUPDATESUBJECT_HH
#define PRIORITYUPDATESUBJECT_HH

#include "priorityupdateobserver.hh"

namespace SignalHandler
{

/*!
 * \brief The PriorityUpdateSubject class
 * Abstract interface for subjects of updates on script priorities.
 */
class PriorityUpdateSubject
{
public:
    
    //! Constructor
    PriorityUpdateSubject() = default;
    
    //! Destructor
    virtual ~PriorityUpdateSubject() = default;
    
    //! Copy-construction is forbidden.
    PriorityUpdateSubject(const PriorityUpdateSubject&) = delete;
    
    //! Copy-assignment is forbidden.
    PriorityUpdateSubject& operator=(const PriorityUpdateSubject&) = delete;
    
    /*!
     * \brief registerClient Registeres new observer on this subject.
     * \param client Observer to be registered.
     * \pre client is not a null-pointer. If client registers itself, it has to
     * unregister before being destroyed.
     * \post Observer is notified every time script priorities are changed.
     */
    virtual void registerClient(PriorityUpdateObserver* client) = 0;
    
    /*!
     * \brief unregisterClient Unregisters previously registered observer.
     * \param client Observer to be unregistered.
     * \pre Client has been registered earlier.
     * \post Client is no more notified on script priority updates.
     */
    virtual void unregisterClient(PriorityUpdateObserver* client) = 0;
};

}
#endif // PRIORITYUPDATESUBJECT_HH
