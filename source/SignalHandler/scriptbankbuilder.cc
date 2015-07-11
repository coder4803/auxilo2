
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

const QString ScriptBankBuilder::SCRIPT_NAME_PREFIX_("ScriptName");

ScriptBankInterface*ScriptBankBuilder::create(const Utils::ParameterSet& params)
{
    ScriptBankInterface::ScriptData data;
    Utils::ParameterSet nameset = params.getSection(SCRIPT_NAME_PREFIX_);
    QHash<QString, QString> names = nameset.parameters();
    
    foreach (QString name, names.values()) {
       QString script_file = params.parameter<QString>(name+QString("_path"));
       QString script = ScriptBankBuilder::readScriptFile(script_file);
       unsigned priority = params.parameter<unsigned>(name+QString("_priority"));
       QString lang = params.parameter<QString>(name+QString("_language"));
       
       ScriptBankInterface::ScriptInfo info;
       info.script = script;
       info.priority = priority;
       info.language = lang;
       data.insert(name, info);
    }
    
    return new ScriptBank(data);
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
