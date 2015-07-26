/* confxmlhandler.hh
 * This is the implementation file for the SignalHandler::ConfXmlHandler class
 * defined in confxmlhandler.hh.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 26-June-2015
 */

#include "confxmlhandler.hh"
#include "configuration.hh"
#include <QDebug>

namespace SignalHandler 
{


ConfXmlHandler::ConfXmlHandler(ScriptBankInterface::ScriptData* result):
    QXmlDefaultHandler(), result_(result), 
    errorString_(), parsingStarted_(false), readingSignal_(false), 
    signalsDefined_(false)
{
    Q_ASSERT (result_ != nullptr);
}

ConfXmlHandler::~ConfXmlHandler()
{
}

bool ConfXmlHandler::startElement(const QString& namespaceURI, 
                                  const QString& localName, 
                                  const QString& qName, 
                                  const QXmlAttributes& atts)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(localName);

    if (signalsDefined_){
        errorString_ = "All elements must be defined inside signals-element.";
        return false;
    }
    else if (qName == "signals" && !parsingStarted_){
        parsingStarted_ = true;
        return true;
    }
    else if (!parsingStarted_){
        errorString_ = "Signal definitions must be inside 'signals' tag";
        return false;
    }
    else if (qName == "signal"){
        if (readingSignal_){
            errorString_ = "Error: signals cannot be nested";
            return false;
        }
        readingSignal_ = true;
        return this->readSignal(atts);
    }
    else{
        errorString_ = "Unknown element: " + qName;
        return false;
    }
}

bool ConfXmlHandler::endElement(const QString& namespaceURI, 
                                const QString& localName, 
                                const QString& qName)
{
    Q_UNUSED(namespaceURI);
    Q_UNUSED(localName);
    
    if (!parsingStarted_){
        errorString_ = "Unknown end-tag: " + qName;
        return false;
    }
    else if (qName == "signal"){
        readingSignal_ = false;
        return true;
    }
    else if (qName == "signals"){
        signalsDefined_ = true;
        return true;
    }
    else{
        return false;
        errorString_ = "Unknown end-tag";
    }
}


bool ConfXmlHandler::fatalError(const QXmlParseException& exception)
{
    errorString_= QString("Error while loading script: line %1, column %2")
            .arg(exception.lineNumber())
            .arg(exception.columnNumber());
    return false;
}

QString ConfXmlHandler::errorString() const
{
    return errorString_;
}


bool ConfXmlHandler::readSignal(const QXmlAttributes& atts)
{
    bool name_defined(false), priority_defined(false), path_defined(false),
            lang_defined(false), toMemory_defined(false);
    ScriptBankInterface::ScriptInfo info;
    QString script_name;
    
    for (int i=0; i<atts.length(); ++i){
        QString att_name = atts.qName(i);
        
        // Read and qualify script name.
        if (att_name == Conf::SIGNAL_NAME){
            script_name = atts.value(i);
            if (script_name.isEmpty()){
                errorString_ = "Script name cannot be an empty string.";
                return false;
            }
            else if (result_->find(script_name) != result_->end()){
                errorString_ = "Script name must be unique.";
                return false;
            }
            name_defined = true;
        }
        
        // Read script file path
        else if (att_name == Conf::SIGNAL_PATH){
            info.script = atts.value(i);
            if (info.script.isEmpty()){
                errorString_ = "Script path cannot be an empty string";
                return false;
            }
            path_defined = true;
        }
        
        // Read language
        else if (att_name == Conf::SIGNAL_LANG){
            info.language = atts.value(i);
            if (info.language.isEmpty()){
                errorString_ = "Script language cannot be an empty string.";
                return false;
            }
            lang_defined = true;
        }
        
        // Read priority
        else if (att_name == Conf::SIGNAL_PRIORITY){
            bool ok(false);
            info.priority = atts.value(i).toUInt(&ok);
            if (!ok){
                errorString_ = "Script priority must be an unsigned integer.";
                return false;
            }
            priority_defined = true;
        }
        
        else if (att_name == Conf::SIGNAL_TO_MEM){
            if (atts.value(i) == "true"){
                info.from_file = false;
            }
            else if (atts.value(i) == "false"){
                info.from_file = true;
            }
            else {
                errorString_ = "readToMemory has to be a boolean value.";
                return false;
            }
            toMemory_defined = true;
        }
        else {
            errorString_ = "Unknown attribute: " + att_name;
            return false;
        }
    }
    
    // Verify and add signal.
    if (!name_defined || !path_defined || !lang_defined){
        errorString_ = "Missing a mandatory attribute.";
        return false;
    }
    if (!priority_defined){
        info.priority = Conf::DEFAULT_PRIORITY;
    }
    if (!toMemory_defined){
        info.from_file = false;
    }
    
    result_->insert(script_name, info);
    return true;
}


}
