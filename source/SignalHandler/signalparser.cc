
/* signalparser.cc
 * 
 * This is the implementation file for the SignalParser class and its 
 * exception class BadMessage.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 04-Mar-2015
 * Last Modified: 07-Mar-2015
 */


#include "signalparser.hh"
#include "priorityupdatesubject.hh"
#include <vector>
#include <QStringList>
#include <QDebug>

namespace SignalHandler 
{


SignalParser::SignalParser(const ScriptPriorityLibrary* lib, 
                           PriorityUpdateSubject* subject):
    PriorityUpdateObserver(), lib_(lib), subject_(subject), update_mutex_()
{
    Q_ASSERT (lib_ != nullptr);
    
    if (subject_ != nullptr){
        subject_->registerClient(this);
    }
}


SignalParser::~SignalParser()
{
    if (subject_ != nullptr){
        subject_->unregisterClient(this);
    }
}


void SignalParser::notifyOnPriorityUpdate(const ScriptPriorityLibrary* new_lib)
{
    Q_ASSERT(new_lib != nullptr);
    
    update_mutex_.lock();
    lib_ = new_lib;
    update_mutex_.unlock();
}


Signal SignalParser::parse(const QString& message)
{
    QStringList fields = message.split(FIELD_SEPERATOR_);
    if (fields.isEmpty()){
        // too few fields.
        throw BadMessage(message);
    }    
    
    // Get scriptID
    bool ok(true);
    unsigned int scriptID = fields.at(0).toUInt(&ok);
    if (!ok){
        // scriptID field is not an unsigned integer.
        throw BadMessage(message);
    }
    
    // Check priority
    std::unique_lock<std::mutex> lock(update_mutex_); //Exception safe lock.
    unsigned int priority = lib_->getPriorityOf(scriptID);
    lock.unlock();
    
    // Get additional parameters
    std::vector<QString> parameters;
    for (int i = 1; i < fields.size(); ++i){
        parameters.push_back( fields.at(i) );
    }
    
    return Signal(priority, scriptID, parameters);
}


const QChar SignalParser::FIELD_SEPERATOR_ = QChar(';');

} // Namespace SignalHandler
