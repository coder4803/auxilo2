
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


namespace SignalHandler
{

ScriptBankInterface*ScriptBankBuilder::create(const Utils::ParameterSet& params)
{
    ScriptBankInterface::ScriptData data;
    Utils::ParameterSet nameset = params.getSection(Conf::NAME_PREFIX);
    QHash<QString, QString> names = nameset.parameters();
    
    try
    {
        foreach (QString name, names.values()) {
            ScriptBankInterface::ScriptInfo info;
            
            // Read file (unless script is a from-file-script.
            QString script_file = params.parameter<QString>(name+Conf::PATH);
            if (!params.contains(name+Conf::FROM_FILE) || 
                                 !params.parameter<bool>(name+Conf::FROM_FILE))
            {
                // read script to memory
                info.script = ScriptBankBuilder::readScriptFile(script_file);
                info.from_file = false;
            }
            else{
                if (!QFile::exists(script_file) ){
                    throw ScriptBankBuilderError(script_file+" does not exist");
                }
                info.script = script_file;
                info.from_file = true;
            }
            
            // Get sctipt priority
            if (!params.contains(name + Conf::PRIORITY)){
                info.priority = Conf::DEFAULT_PRIORITY;
            }
            else {
                info.priority = params.parameter<unsigned>(name+Conf::PRIORITY);
            }
            
            info.language = params.parameter<QString>(name+QString(Conf::LANG));
            data.insert(name, info);
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
