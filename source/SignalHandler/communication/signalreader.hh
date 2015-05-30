
/* signalreader.hh
 * 
 * This is the header file for the SignalReader class that receives
 * SignalMessages from RabbidMQ server.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmai.com
 * Created: 01-April-2015
 * Last modified: 01-April-2015
 */


#ifndef SIGNALREADER_HH
#define SIGNALREADER_HH

#include <QObject>
#include <mutex>
#include "messagegroup.h"
#include "interfaces/scriptprioritylibrary.hh"
#include "interfaces/priorityupdateobserver.hh"
#include "interfaces/priorityupdatesubject.hh"
#include "signalqueue.hh"


namespace SignalHandler
{

/*!
 * \brief The SignalReader class
 *  This class is responsible for receiving signal messages from RabbidQM.
 *  Incomming signals are pushed into siqnalqueue waiting to be handled.
 */
class SignalReader : public QObject, public PriorityUpdateObserver
{
    Q_OBJECT

public:
    
    /*!
     * \brief SignalReader Constructor
     * \param queue SignalQueue.
     * \param lib Script priority source.
     * \param subject Observable PriorityUpdateSubject.
     * \param parent QObject's parent.
     * \pre queue != nullptr, lib != nullptr and subject != nullptr.
     * \post SignalReader does not start reading messages until start() is
     *  called.
     */
    SignalReader(SignalQueue* queue, const ScriptPriorityLibrary* lib,
                 PriorityUpdateSubject* subject,
                 QObject* parent = nullptr);
    
    //! Destructor.
    ~SignalReader();
    
    //! Copy- and move-constructors and -assignment operators are forbidden.
    SignalReader(const SignalReader&) = delete;
    SignalReader(SignalReader&&) = delete;
    SignalReader& operator=(const SignalReader&) = delete;
    SignalReader& operator=(SignalReader&&) = delete;
    
    /*!
     * \brief start
     *  Start reading messages.
     * \pre This method can be called only once.
     * \post Incomming messages are handled and pushed to SignalQueue.
     */
    void start();
    
    //! Implements the PriorityUpdateObserver interface.
    void notifyOnPriorityUpdate(const ScriptPriorityLibrary* new_lib);
    
    
private Q_SLOTS:
    
    // Handles incomming messages.
    void onMessageReceived(QByteArray data);
    
private:
    
    Utils::MessageGroup* group_;
    SignalQueue* queue_;
    const ScriptPriorityLibrary* lib_;
    PriorityUpdateSubject* subject_;
    std::mutex update_mx_;
};

} // Namespace SignalHandler

#endif // SIGNALREADER_HH
