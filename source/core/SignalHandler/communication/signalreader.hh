
/* signalreader.hh
 * 
 * This is the header file for the SignalReader class that receives
 * SignalMessages from RabbidMQ server.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmai.com
 * Created: 01-April-2015
 * Last modified: 19-June-2015
 */


#ifndef SIGNALREADER_HH
#define SIGNALREADER_HH

#include <QObject>
#include <mutex>
#include <memory>
#include "messagegroup.h"
#include "interfaces/scriptprioritylibrary.hh"
#include "interfaces/priorityupdateobserver.hh"
#include "interfaces/priorityupdatesubject.hh"
#include "signalqueue.hh"


namespace SignalHandler
{

/*!
 * \brief The SignalReader class.
 *  This class is responsible for receiving signal messages from message app.
 *  Incomming signals are pushed into siqnalqueue waiting to be handled.
 */
class SignalReader : public QObject, public PriorityUpdateObserver
{
    Q_OBJECT

public:
    
    /*!
     * \brief Constructor
     * \param queue SignalQueue.
     * \param lib Script priority source.
     * \param subject Observable PriorityUpdateSubject.
     * \param parent QObject's parent.
     * \pre queue != nullptr, lib != nullptr.
     * \post New object is registered to subject, if subject is not nullptr.
     *  SignalReader does not start reading messages until start() is called.
     */
    SignalReader(std::shared_ptr<SignalQueue> queue, 
                 const ScriptPriorityLibrary* lib,
                 PriorityUpdateSubject* subject = nullptr,
                 QObject* parent = nullptr);
    
    /*!
     * \brief Destructor.
     * \post Object is unregistered to its subject (if there was one).
     */
    virtual ~SignalReader();
    
    //! Copy- and move-constructors and -assignment operators are forbidden.
    SignalReader(const SignalReader&) = delete;
    SignalReader(SignalReader&&) = delete;
    SignalReader& operator=(const SignalReader&) = delete;
    SignalReader& operator=(SignalReader&&) = delete;
    
    /*!
     * \brief Set (new) priority update subject.
     * \param sub new subject.
     * \pre sub != nullptr.
     * \post If there was a non-null subject, this object is unregistered from
     *  it. This object is registered to new subject.
     */
    void setPrioritySubject(PriorityUpdateSubject* sub);
    
    /*!
     * \brief Start reading messages.
     * \param group_name Name of the signal message group.
     * \pre This method can be called only once for an object.
     *  Group name is not empty.
     * \post Incomming messages are handled and pushed to SignalQueue.
     */
    void start(const QString& group_name = Utils::SIGNAL_HANDLER_GROUP);
    
    /*!
     * \brief Check if start has been called.
     * \return True, if start() has been called.
     */
    bool isStarted() const;
    
    /*!
     * \brief Stop receiving signals. Signal group is destroyed. SignalReader
     *  can be restarted by calling start().
     */
    void stop();
    
    //! Implements the PriorityUpdateObserver interface.
    void notifyOnPriorityUpdate(const ScriptPriorityLibrary* new_lib);
    
    
private Q_SLOTS:
    
    /*!
     * \brief Handle incoming SignalMessage
     * \param data SignalMessage as a QByteArray.
     * \pre data is a binary representation of a legit SignalMessage.
     * \post SignalMessage is converted into a Signal and added to the queue.
     */
    void onMessageReceived(QByteArray data);
    
private:
    
    Utils::MessageGroup* group_;
    std::shared_ptr<SignalQueue> queue_;
    const ScriptPriorityLibrary* lib_;
    PriorityUpdateSubject* subject_;
    mutable std::mutex update_mx_;
};

} // Namespace SignalHandler

#endif // SIGNALREADER_HH
