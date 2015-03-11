
/* scriptbankinterface.hh
 * 
 * This header file defines the abstract ScriptBankInterface, which combines
 * two other abstract interfaces: ScriptLibrary and ScriptPriorityLibrary.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 07-Mar-2015
 * Last modified: 07-Mar-2015 
 */

#ifndef SCRIPTBANKINTERFACE_HH
#define SCRIPTBANKINTERFACE_HH

#include "scriptlibrary.hh"
#include "scriptprioritylibrary.hh"
#include <map>

namespace SignalHandler
{

/*!
 * \brief The ScriptBankInterface class
 * Abstract interface that combines ScriptLibrary- and ScriptPriorityLibrary-
 * interfaces.
 */
class ScriptBankInterface : public ScriptLibrary, public ScriptPriorityLibrary
{
public:
    
    /*!
     * \brief The ScriptInfo struct
     * Auxiliary data structure to hold individual script's properties.
     */
    struct ScriptInfo
    {
        QString script;
        unsigned priority;
    };
    
    //! Convenience type defifinition.
    typedef std::map<unsigned int, ScriptInfo> ScriptData;
    
    //! Constructor
    ScriptBankInterface(): ScriptLibrary(), ScriptPriorityLibrary() {}
    
    //! Destructor
    virtual ~ScriptBankInterface() = default;
    
    //! Copy-construction is forbidden.
    ScriptBankInterface(const ScriptBankInterface&) = delete;
    
    //! Copy-assignment is forbidden.
    ScriptBankInterface& operator=(const ScriptBankInterface&) = delete;
    
    // This class does not implement any of its base class methods.
};

        
}
#endif // SCRIPTBANKINTERFACE_HH
