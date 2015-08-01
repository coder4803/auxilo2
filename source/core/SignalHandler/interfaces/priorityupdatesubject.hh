
/* priorityupdatesubject.hh
 * 
 * This header file defines abstract interface for subjects of script priority
 * library updates.
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
 * Abstract interface for subjects of script priority library updates.
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
     * \brief registerObserver Registers a new observer on subject.
     * \param client Observer to be registered.
     * \pre client is not a null-pointer. If observer registers itself, it has
     *  to unregister before being destroyed.
     * \post Observer is notified every time script library is changed.
     */
    virtual void registerClient(PriorityUpdateObserver* client) = 0;
    
    /*!
     * \brief unregisterClient Unregisters an observer.
     * \param client Observer to be unregistered.
     * \pre Observer has been registered to this subject earlier.
     * \post Unregistered observer is no more notified on script updates. 
     */
    virtual void unregisterClient(PriorityUpdateObserver* client) = 0;
};

} // Namespace SignalHandler


#endif // PRIORITYUPDATESUBJECT_HH
