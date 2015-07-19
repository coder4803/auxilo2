
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
#include <QStringList>
#include <QFile>
#include <QDebug>


namespace SignalHandler
{

const QString ScriptBankBuilder::SCRIPT_NAME_PREFIX("scriptname");
const QString ScriptBankBuilder::PRIORITY_POSTFIX("_priority");
const QString ScriptBankBuilder::PATH_POSTFIX("_path");
const QString ScriptBankBuilder::LANG_POSTFIX("_language");

ScriptBankInterface*ScriptBankBuilder::create(const Utils::ParameterSet& params)
{
    ScriptBankInterface::ScriptData data;
    Utils::ParameterSet nameset = params.getSection(SCRIPT_NAME_PREFIX);
    QHash<QString, QString> names = nameset.parameters();
    
    try
    {
        foreach (QString name, names.values()) {
            QString script_file = params.parameter<QString>(name+PATH_POSTFIX);
            QString script = ScriptBankBuilder::readScriptFile(script_file);
            unsigned priority=params.parameter<unsigned>(name+PRIORITY_POSTFIX);
            QString lang=params.parameter<QString>(name+QString(LANG_POSTFIX));
            
            ScriptBankInterface::ScriptInfo info;
            info.script = script;
            info.priority = priority;
            info.language = lang;
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
