
/* scriptbankbuilder.hh
 * 
 * This header file defines the ScriptBankBuilder class, whitch is a bulder
 * class for the ScriptBank class. This file also defines necesseary exception
 * classes.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 06-Mar-2015
 * Last modified: 26-July-2015
 */

#ifndef SCRIPTBANKBUILDER_HH
#define SCRIPTBANKBUILDER_HH

#include "interfaces/scriptbankinterface.hh"
#include "confresponsemessage.h"
#include <exception>
#include <QString>
#include <QHash>


namespace SignalHandler
{


/*!
 * \brief The ScriptBankBuilder class
 * Builder class for the ScriptBank class. This class is responsible for
 * constrcting ScriptBank objects based on ParameterSet.
 */
class ScriptBankBuilder
{
public:
    
    //! Default constructor
    ScriptBankBuilder() = default;
    
    //! Default destrucor
    ~ScriptBankBuilder() = default;
    
    //! Default copy-constructor.
    ScriptBankBuilder(const ScriptBankBuilder&) = default;
    
    //! Default copy-assignment operator.
    ScriptBankBuilder& operator=(const ScriptBankBuilder&) = default;
    
    /*!
     * \brief Creates new instance of ScriptBankInterface implementation.
     * \param params Configuration parameters from ConfManager.
     * \return new instance of ScriptBankInterface implementation. Ownership is
     *  passed to the caller.
     * \pre None.
     * \exception ScriptBankBuilderError, if params is not a valid configuration
     *  or some other error occurs.
     */
    static ScriptBankInterface* create(const Utils::ParameterSet& params);
    
    
private:
    
    static QString readScriptFile(const QString& file_name);
};





/*!
 * \brief The ScriptBankBuilderError class
 * Exception class to signal that ScriptBank construction failed.
 */
class ScriptBankBuilderError : std::exception
{
public:
    
    /*!
     * \brief Constructor
     * \param clarifying message.
     */
    ScriptBankBuilderError(const QString& message);
    
    //! Destructor
    virtual ~ScriptBankBuilderError() noexcept;
    
    /*!
     * \brief what Reimplements the std::exception-interface.
     * \return Exceprion class name.
     */
    virtual const char* what() const noexcept;
    
    /*!
     * \brief Get the clarifying error message.
     */
    QString getMessage() const;
    
private:
    QString msg_;
};


} // Namespace SignalHandler

#endif // SCRIPTBANKBUILDER_HH
