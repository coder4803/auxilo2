
/* scriptbank.hh
 * 
 * This header file defines the ScriptBank class that holds all suported
 * Python scripts, their ID and priority. This file also defines needed
 * exception classes.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 05-Mar-2015
 * Last Modified: 05-Mar-2015
 */


#ifndef SCRIPTBANK_HH
#define SCRIPTBANK_HH

#include <map>
#include <QString>
#include <exception>


namespace SignalHandler
{


/*!
 * \brief The ScriptBank class
 * Holds information about supported scripts. Once created, objects of this
 * class are immutabe, and therefore thread safe.
 */
class ScriptBank
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
    
    /*!
     * \brief getScript Retrieves script-string corresponding given ID-number.
     * \param scriptID ID-number.
     * \return Corresponding script as a QString.
     * \pre -
     * \exception UnknownScript is thrown, if scriptID is unknown.
     */
    QString getScript(unsigned int scriptID) const;
    
    /*!
     * \brief getPriorityOf Retrieves priority of given script.
     * \param scriptID Script's ID-number
     * \return Script's priority number.
     * \pre -
     * \exception UnknownScript is thrown, if scriptID is unknown.
     */
    unsigned int getPriorityOf(unsigned int scriptID) const;
    
    /*!
     * \brief at Get all script properties at once.
     * \param scriptID Script's ID number.
     * \return ScriptInfo object holding script's properties.
     * \pre -
     * \exception UnknownScript is thrown, if scriptID is unknown.
     */
    ScriptInfo at(unsigned int scriptID) const;
    
    /*!
     * \brief operator [] Get all script properties at once.
     * \param scriptID Script's ID-number.
     * \return ScriptInfo object holding script's properties.
     * \pre scriptID is known.
     */
    ScriptInfo operator[](unsigned int scriptID) const;
    
    
private:
    ScriptData scripts_;
};



/*!
 * \brief The UnknownScript class
 * Exception class to signal attempt to retrieve script with unknown ID.
 * Class implements std::exception interface.
 */
class UnknownScript : std::exception
{
public:
    
    /*!
     * \brief UnknownScript Constructor.
     * \param scriptID Unknown ID that caused exception.
     * \pre -
     */
    UnknownScript(unsigned int scriptID);
    
    //! Destructor
    virtual ~UnknownScript() noexcept;
    
    /*!
     * \brief what Reimplements std::exception interface.
     * \return Exception class name.
     * \pre -
     */
    virtual const char* what() const noexcept;
    
    /*!
     * \brief getUnknownID Get unknown script ID-number.
     * \return ID-number that caused the exception.
     * \pre -
     */
    unsigned int getUnknownID() const noexcept;
    
    
private:
    unsigned int scriptID_;
};

} // Namespace SignalHandler

#endif // SCRIPTBANK_HH
