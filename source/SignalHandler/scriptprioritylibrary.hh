
/* scriptprioritylibrary.hh
 * 
 * This header file defines abstract interface for checking script priorities.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 06-Mar-2015
 * Last modified: 06-Mar-2015
 */

#ifndef SCRIPTPRIORITYLIBRARY_HH
#define SCRIPTPRIORITYLIBRARY_HH

#include "unknownscript.hh"


namespace SignalHandler
{

/*!
 * \brief The ScriptPriorityLibrary class
 * Abstract interface for checking script priorities.
 */
class ScriptPriorityLibrary
{
public:
    
    //! Constructor
    ScriptPriorityLibrary() = default;
    
    //! Destructor
    virtual ~ScriptPriorityLibrary() = default;
    
    //! Copy-construction is forbidden.
    ScriptPriorityLibrary(const ScriptPriorityLibrary&) = delete;
    
    //! Copy-assignment is forbidden.
    ScriptPriorityLibrary& operator=(const ScriptPriorityLibrary&) = delete;
    
    /*!
     * \brief getPriorityOf Get priority of the script with given ID.
     * \param scriptID Distinct script ID-number.
     * \return Script's priority number (0 = highest).
     * \exception UnknownScript is thrown, if given ID doesn't match any script.
     */
    virtual unsigned int getPriorityOf(unsigned int scriptID) const = 0;
};

} // Namespace SignalHandler

#endif // SCRIPTPRIORITYLIBRARY_HH
