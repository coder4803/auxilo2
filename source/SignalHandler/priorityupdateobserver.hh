
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
 * Observer interface for updates on ScriptPriorityLibrary objects.
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
     * \brief notyfyOnPriorityUpdate Notifies observer on priority updates.
     * \param new_lib Updated script priority library.
     * \pre new_lib is not a null-pointer.
     * \post Observer uses new_lib for quering script priorities from this on.
     */
    virtual void notyfyOnPriorityUpdate(const ScriptPriorityLibrary* new_lib)=0;
};

} // Namespace SignalHandler

#endif // PRIORITYUPDATEOBSERVER_HH
