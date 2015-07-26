
/* scriptbankbulder.cc
 * 
 * This is the implementation file for classes defined in scriptbankbuider.hh.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 06-Mar-2015
 * Last modified: 31-Mar-2015
 */

#include "scriptbankbuilder.hh"
#include "scriptbank.hh"
#include "configuration.hh"
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <QXmlReader>
#include "confxmlhandler.hh"


namespace SignalHandler
{

ScriptBankInterface*ScriptBankBuilder::create(const Utils::ParameterSet& params)
{
    ScriptBankInterface::ScriptData data;
    
    try{
        // Parse XML-file
        QString conf_path, script_path;
        conf_path = params.parameter<QString>(Conf::CONF_PATH.toLower());
        script_path = params.parameter<QString>(Conf::SCRIPT_PATH.toLower());
        
        QFile f(conf_path);
        if (!f.open(QIODevice::ReadOnly) ){
            throw ScriptBankBuilderError(conf_path + " did not open.");
        }
        QXmlInputSource source(&f);
        ConfXmlHandler handler(&data);
        QXmlSimpleReader reader;
        reader.setContentHandler(&handler);
        reader.setErrorHandler(&handler);
        if (!reader.parse(&source)){
            throw ScriptBankBuilderError(conf_path + "is an invalid xml-file.");
        }
        
        // Read files if needed
        for (auto it = data.begin(); it != data.end(); ++it){
            it.value().script = script_path + it.value().script;
            if (it.value().from_file){
                if (!QFile::exists(it.value().script) ){
                    throw ScriptBankBuilderError(it.value().script + 
                                                 " does not exist.");
                }
            }
            else{
                // Read file to memory
                it.value().script = readScriptFile(it.value().script);
            }
        }
        
    }
    catch (QException& e){
        throw ScriptBankBuilderError(e.what());
    }

    return new ScriptBank(data);
}


// Reads and returns script from given file.
QString ScriptBankBuilder::readScriptFile(const QString& file_name)
{   
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text) ){
        // Could not open the file
        throw ScriptBankBuilderError(file_name + QString(" did not open."));
    }
    
    QString script = file.readAll();
    file.close();
    return script;
}


// ScriptBankBuilderFileError implementation
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ScriptBankBuilderError::
ScriptBankBuilderError(const QString& message):
    msg_(message)
{
}


ScriptBankBuilderError::~ScriptBankBuilderError() noexcept
{
}

const char* ScriptBankBuilderError::what() const noexcept
{
    return "SignalHandler::ScriptBankBuilderError";
}


QString ScriptBankBuilderError::getMessage() const
{
    return msg_;
}


} // Namespace SignalHandler
