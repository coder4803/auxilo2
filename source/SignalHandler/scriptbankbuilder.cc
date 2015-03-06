
/* scriptbankbulder.cc
 * 
 * This is the implementation file for classes defined in scriptbankbuider.hh.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 06-Mar-2015
 * Last modified: 06-Mar-2015
 */

#include "scriptbankbuilder.hh"
#include <QStringList>
#include <QFile>


namespace SignalHandler
{

const QChar ScriptBankBuilder::SCRIPT_SEPERATOR = QChar(';');
const QChar ScriptBankBuilder::FIELD_SEPERATOR = QChar(':');


ScriptBank* ScriptBankBuilder::createScriptBank(const QString& conf_msg)
{
    QStringList clauses = conf_msg.split(SCRIPT_SEPERATOR);
    ScriptBank::ScriptData scripts;
    
    try
    {
        for (int i=0; i<clauses.size(); ++i){
            ScriptBankBuilder::getScriptProperties(clauses.at(i), scripts);
        }
    }
    catch (BadMessage&){
        // Attach original message into exception.
        throw BadMessage(conf_msg);
    }
    
    return new ScriptBank(scripts);
}


// Reads individual script's properties from QString and inserts them in 
// Scriptdata
void ScriptBankBuilder::getScriptProperties(const QString& input, 
                                            ScriptBank::ScriptData& scripts)
{
    QStringList fields = input.split(FIELD_SEPERATOR);
    if (fields.size() != 3){
        // too few fields.
        throw BadMessage("");
    }
    
    // Read ScriptID
    bool ok(true);
    unsigned scriptID = fields.at(0).toUInt(&ok);
    if (!ok){
        // Invalid scriptID field
        throw BadMessage("");
    }
    
    // Read priority
    unsigned priority = fields.at(1).toUInt(&ok);
    if (!ok){
        // Invalid priority number
        throw BadMessage("");
    }
    
    // Read script file
    QString script = ScriptBankBuilder::readScriptFile(fields.at(2));
    
    // Check if a script with identical ID already exists
    if (scripts.find(scriptID) != scripts.end()){
        throw BadMessage("");
    }
    
    scripts[scriptID] = ScriptBank::ScriptInfo({script, priority});
}


// Reads and returns script from given file.
QString ScriptBankBuilder::readScriptFile(const QString& file_name)
{
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text) ){
        // Could not open the file
        throw ScriptBankBuilderFileError(file_name);
    }
    
    QString script = file.readAll();
    file.close();
    return script;
}


// ScriptBankBuilderFileError implementation
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ScriptBankBuilderFileError::ScriptBankBuilderFileError(const QString& file_name):
    file_name_(file_name)
{
}


ScriptBankBuilderFileError::~ScriptBankBuilderFileError() noexcept
{
}

const char* ScriptBankBuilderFileError::what() const noexcept
{
    return "SignalHandler::ScriptBankBuilderFileError";
}


QString ScriptBankBuilderFileError::getFileName() const
{
    return file_name_;
}


} // Namespace SignalHandler
