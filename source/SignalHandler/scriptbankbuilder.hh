
/* scriptbankbuilder.hh
 * 
 * This header file defines the ScriptBankBuilder class, whitch is a bulder
 * class for the ScriptBank class. This file also defines necesseary exception
 * classes.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 06-Mar-2015
 * Last modified: 31-Mar-2015
 */

#ifndef SCRIPTBANKBUILDER_HH
#define SCRIPTBANKBUILDER_HH

#include "interfaces/scriptbankinterface.hh"
#include "exceptions/badmessage.hh"
#include "confresponsemessage.h"
#include <exception>
#include <QString>
#include <QHash>


namespace SignalHandler
{


/*!
 * \brief The ScriptBankBuilder class
 * Builder class for the ScriptBank class. This class is responsible for
 * constrcting ScriptBank objects based on configuration messages.
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
    
    static ScriptBankInterface* create(const Utils::ParameterSet& params);
    
    
private:
    
    static const QString SCRIPT_NAME_PREFIX_;
    
    static QString readScriptFile(const QString& file_name);
};



/*!
 * \brief The ScriptBankBuilderFileError class
 * Exception class to signal that ScriptBank construction failed, because
 * at liest one script file could not be opened.
 */
class ScriptBankBuilderFileError : std::exception
{
public:
    
    /*!
     * \brief ScriptBankBuilderFileError Constructor
     * \param file_name Name of file that failed to open.
     */
    ScriptBankBuilderFileError(const QString& file_name);
    
    //! Destructor
    virtual ~ScriptBankBuilderFileError() noexcept;
    
    /*!
     * \brief what Reimplements the std::exception-interface.
     * \return Exceprion class name.
     */
    virtual const char* what() const noexcept;
    
    /*!
     * \brief getFileName Gets unopenable file name.
     * \return Name of script file that failed to open.
     */
    QString getFileName() const;
    
private:
    QString file_name_;
};


} // Namespace SignalHandler

#endif // SCRIPTBANKBUILDER_HH
