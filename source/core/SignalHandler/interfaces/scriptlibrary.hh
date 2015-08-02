
/* scriptlibrary.hh
 * 
 * This header file defines abstract interface for quering scripts.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 07-Mar-2015
 * Last modified: 31-Mar-2015
 */

#ifndef SCRIPTLIBRARY_HH
#define SCRIPTLIBRARY_HH

#include "exceptions/unknownscript.hh"
#include <QString>

namespace SignalHandler 
{

/*!
 * \brief The ScriptLibrary class
 * Abstract interface for quering scripts.
 */
class ScriptLibrary
{
public:
    
    //! Constructor
    ScriptLibrary() = default;
    
    //! Destructor
    virtual ~ScriptLibrary() = default;
    
    //! Copy-construction is forbidden.
    ScriptLibrary(const ScriptLibrary&) = delete;
    
    //! Copy-assignment is forbidden.
    ScriptLibrary& operator=(const ScriptLibrary&) = delete;
    
    /*!
     * \brief getScript Get script with given ID-number.
     * \param scriptID Distinct script ID-number.
     * \return Script as a QString object.
     * \exception UnknownScript is thrown, if scriptID doesn't match any script.
     */
    virtual QString getScript(const QString& scriptID) const = 0;
    
    /*!
     * \brief getLanguage Get script language.
     * \param scriptID script identifier.
     * \return Language identifier.
     * \exception UnknownScript is thrown, if scriptID doesn't match any script.
     */
    virtual QString getLanguage(const QString& scriptID) const = 0;
};

} // Namespace SignalHandler

#endif // SCRIPTLIBRARY_HH
