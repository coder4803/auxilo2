
/* scriptbankbuilder.hh
 * 
 * This header file defines the ScriptBankBuilder class, whitch is a bulder
 * class for the ScriptBank class. This file also defines necesseary exception
 * classes.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 06-Mar-2015
 * Last modified: 06-Mar-2015
 */

#ifndef SCRIPTBANKBUILDER_HH
#define SCRIPTBANKBUILDER_HH

#include "scriptbank.hh"
#include "signalparser.hh" // BadMessage-exception.
#include <exception>
#include <QString>


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
    
    //! Character in configuration message to seperate different scripts.
    static const QChar SCRIPT_SEPERATOR;
    
    //! Character in configuration message to seperate script properties.
    static const QChar FIELD_SEPERATOR;
    
    //! Default constructor
    ScriptBankBuilder() = default;
    
    //! Default destrucor
    ~ScriptBankBuilder() = default;
    
    //! Default copy-constructor.
    ScriptBankBuilder(const ScriptBankBuilder&) = default;
    
    //! Default copy-assignment operator.
    ScriptBankBuilder& operator=(const ScriptBankBuilder&) = default;
    
    /*!
     * \brief createScriptBank Creates new instance of ScriptBank.
     * 
     * \param conf_msg The configuration message.
     * 
     * \return New ScriptBank object that is constructed after configuration. 
     *  The caller takes ownership over the new object.
     * 
     * \pre -
     * 
     * \exception BadMessage is thrown, if conf_msg is invalid.
     *  If any of sctipt files could not be opened, 
     *  ScriptBankBuilderFileError is thrown.
     * 
     * Valid configuration messages are formated as follows:
     * 
     * ScriptID:ScriptPriority:ScriptFilePath
     * <... All script definitions as declared above, seperated with ';' ...>
     * 
     * where ScriptID and ScriptPriority are unsigned integers.
     */
    static ScriptBank* createScriptBank(const QString& conf_msg);
    
    
private:
    
    static void getScriptProperties(const QString& input, 
                                    ScriptBank::ScriptData& scripts);
    
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
