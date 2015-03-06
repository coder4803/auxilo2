
/* scriptbank.hh
 * 
 * This header file defines the ScriptBank class that holds all suported
 * Python scripts, their ID and priority. This file also defines needed
 * exception classes.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 05-Mar-2015
 * Last Modified: 07-Mar-2015
 */


#ifndef SCRIPTBANK_HH
#define SCRIPTBANK_HH

#include <map>
#include <QString>
#include "scriptlibrary.hh"
#include "scriptprioritylibrary.hh"

namespace SignalHandler
{

/*!
 * \brief The ScriptBank class
 * Holds information about supported scripts. Once created, objects of this
 * class are immutabe, and therefore thread safe.
 */
class ScriptBank : public ScriptLibrary, public ScriptPriorityLibrary
{
public:
    
    /*!
     * \brief The ScriptInfo struct
     * Auxiliary data structure to hold individual script's properties.
     */
    struct ScriptInfo
    {
        QString script;         // Python script as a QString.
        unsigned int priority;  // Script priority number (0 == highest).
    };
    
    //! Convenience type defifinition.
    typedef std::map<unsigned int, ScriptInfo> ScriptData;
    
    
    /*!
     * \brief ScriptBank Constructor
     * \param scripts Script ID numbers as keys, other properties as values.
     * \pre -
     * \post ScriptBank object holds all given scripts and their properties.
     */
    ScriptBank(const ScriptData& scripts);
    
    //! Destructor.
    ~ScriptBank();
    
    //! Copy-construction is forbidden.
    ScriptBank(const ScriptBank&) = delete;
    
    //! Copy-assignment is forbidden.
    ScriptBank& operator=(const ScriptBank&) = delete;
    
    /*!
     * \brief isValidID Checks if scriptID is known.
     * \param scriptID Tested ID-number.
     * \return True, if scriptID is known, else returns false.
     */
    bool isValidID(unsigned int scriptID) const;
    
    // ScriptLibrary methods (see documentation in scriptlibrary.hh):
    virtual QString getScript(unsigned int scriptID) const;
    
    // ScriptPriorityLibrary methods 
    // (see documentation in scriptprioritylibrary.hh):
    virtual unsigned int getPriorityOf(unsigned int scriptID) const;
    
    
private:
    ScriptData scripts_;
};

} // Namespace SignalHandler

#endif // SCRIPTBANK_HH
