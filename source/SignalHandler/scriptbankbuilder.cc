
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
#include <QDir>
#include <QDebug>


namespace SignalHandler
{

const QChar ScriptBankBuilder::SCRIPT_SEPERATOR = QChar(';');
const QChar ScriptBankBuilder::FIELD_SEPERATOR = QChar(':');


ScriptBankInterface* ScriptBankBuilder::createScriptBank(const QString& conf_msg)
{
    QStringList clauses = conf_msg.split(SCRIPT_SEPERATOR);
    ScriptBankInterface::ScriptData scripts;
    
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
void ScriptBankBuilder::
getScriptProperties(const QString& input, 
                    ScriptBankInterface::ScriptData& scripts)
{
    QStringList fields = input.split(FIELD_SEPERATOR);
    if (fields.size() != 3){
        // too few fields.
        throw BadMessage("");
    }
    
    // Read ScriptID
    QString scriptID = fields.at(0);
    if (scriptID.size() == 0){
        throw BadMessage("");
    }
    
    // Read priority
    bool ok(false);
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
    
    scripts[scriptID] = ScriptBank::ScriptInfo({script, QString(), priority});
}


// Reads and returns script from given file.
QString ScriptBankBuilder::readScriptFile(const QString& file_name)
{
    /*
    QStringList seperated = file_name.split('/');
    QString path = QDir::currentPath();
    for (int i=0; i<seperated.size(); ++i){
        path.append("/");
        path.append(seperated.at(i));
    }
    */
    
    QFile file(file_name);
    qDebug() << "Does exist?" << file.exists() << file.fileName();
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

ScriptBankBuilderFileError::
ScriptBankBuilderFileError(const QString& file_name):
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
