
/* scriptbank.hh
 * 
 * This header file defines the ScriptBank class that holds all suported
 * Python scripts, their ID and priority. This file also defines needed
 * exception classes.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 05-Mar-2015
 * Last Modified: 31-Mar-2015
 */


#ifndef SCRIPTBANK_HH
#define SCRIPTBANK_HH

#include <map>
#include <QString>
#include "interfaces/scriptbankinterface.hh"

namespace SignalHandler
{

/*!
 * \brief The ScriptBank class
 * Holds information about supported scripts. Once created, objects of this
 * class are immutabe, and therefore thread safe.
 */
class ScriptBank : public ScriptBankInterface
{
public:
    
    /*!
     * \brief ScriptBank Constructor
     * \param scripts Script ID numbers as keys, other properties as values.
     * \pre -
     * \post ScriptBank object holds all given scripts and their properties.
     */
    explicit ScriptBank(const ScriptData& scripts = ScriptData());
    
    //! Destructor.
    ~ScriptBank();
    
    //! Copy-construction is forbidden.
    ScriptBank(const ScriptBank&) = delete;
    
    //! Copy-assignment is forbidden.
    ScriptBank& operator=(const ScriptBank&) = delete;
    
    // ScriptLibrary methods (see documentation in scriptlibrary.hh):
    virtual QString getScript(const QString& scriptID) const;
    virtual QString getLanguage(const QString& scriptID) const;
    
    // ScriptPriorityLibrary methods 
    // (see documentation in scriptprioritylibrary.hh):
    virtual unsigned int getPriorityOf(const QString& scriptID) const;
    
    
private:
    ScriptData scripts_;
};

} // Namespace SignalHandler

#endif // SCRIPTBANK_HH
