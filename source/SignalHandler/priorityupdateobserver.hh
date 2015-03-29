
/* priorityupdateobserver.hh
 * 
 * This header file defines abstract interface for observing updates on
 * ScriptPriorityLibrary objects.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 07-Mar-2015
 * Last modified: 07-Mar-2015
 */

#ifndef PRIORITYUPDATEOBSERVER_HH
#define PRIORITYUPDATEOBSERVER_HH

#include "scriptprioritylibrary.hh"

namespace SignalHandler
{

/*!
 * \brief The PriorityUpdateObserver class
 * Observer class for updates on ScriptPriorityLibrary objects.
 */
class PriorityUpdateObserver
{
public:
    
    //! Constructor
    PriorityUpdateObserver() = default;
    
    //! Destructor
    virtual ~PriorityUpdateObserver() = default;
    
    //! Copy-construction is forbidden.
    PriorityUpdateObserver(const PriorityUpdateObserver&) = delete;
    
    //! Copy-assignment is forbidden.
    PriorityUpdateObserver& operator=(const PriorityUpdateObserver&) = delete;
    
    /*!
     * \brief notify Notifies observer on script priority updates.
     * \param new_lib Updated script priority library.
     * \pre new_lib is not a null-pointer.
     * \post Observer uses new_lib for quering script priorities from this on.
     */
    virtual void notifyOnPriorityUpdate(const ScriptPriorityLibrary* new_lib)=0;
};

}

#endif // PRIORITYUPDATEOBSERVER_HH
