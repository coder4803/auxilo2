/* scriptapiimplementation.hh
 * 
 * This header defines the ScriptApiImplementation class: the concrete 
 * implementation for the ScriptAPI interface.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#ifndef SCRIPTAPIIMPLEMENTATION_HH
#define SCRIPTAPIIMPLEMENTATION_HH

#include <QObject>
#include "scriptapi.hh"
#include "interfaces/scriptlibrary.hh"
#include "interfaces/scriptupdateobserver.hh"
#include "interfaces/scriptupdatesubject.hh"
#include "messagegroup.h"
#include "stateresponsemessage.h"
#include "setstateackmessage.h"
#include <mutex>
#include <condition_variable>


namespace SignalHandler 
{

/*!
 * \brief The ScriptApiImplementation class
 *  Implements the ScriptAPI interface.
 */
class ScriptApiImplementation : 
        public QObject, public ScriptAPI, public ScriptUpdateObserver
{
    Q_OBJECT
    
public:
    
    /*!
     * \brief Constructor.
     * \param lib Script library.
     * \param subject Script library update subject.
     * \param namingSuffix Suffix to be added at the end of message groups.
     * \pre lib != nullptr, namingSuffix is unique.
     */
    ScriptApiImplementation(const ScriptLibrary* lib,
                            ScriptUpdateSubject* subject,
                            const QString& namingSuffix);
    
    //! Destructor.
    virtual ~ScriptApiImplementation();
    
    //! Move- and copy-constructors and assignment operators are forbidden.
    ScriptApiImplementation(const ScriptApiImplementation&) = delete;
    ScriptApiImplementation(ScriptApiImplementation&&) = delete;
    ScriptApiImplementation& operator=(const ScriptApiImplementation&) = delete;
    ScriptApiImplementation& operator=(ScriptApiImplementation&&) = delete;
    
    /*!
     * \brief Set (new) ScriptUpdateSubject.
     * \param sub New subject.
     * \pre sub != nullptr.
     * \post This observer is unregistered to previous subject, if there is one.
     *  This observer is registered to new subject.
     */
    void setSubject(ScriptUpdateSubject* sub);
    
    void setSender(const QString& name);
    
    // ScriptApi interface implementations. See scriptapi.hh for documentation.
    
    virtual QDateTime dateTimeNow() const;
    
    virtual Utils::StateResponseMessage::State 
    getStateOf(const QString& stateName);
    
    virtual ScriptAPI::StateMap getStates(const QStringList& states);
    
    virtual int setState(const QString& stateName, const QVariant& value);
    
    virtual int sendSignal(const QString& signalName, const QStringList& args,
                           const QString& target = QString() );
    
    virtual QString getSender() const;

    virtual void sendEvent(const QString &signal,
                           const QDateTime &timestamp,
                           quint32 interval_sec,
                           quint32 repeat);
    
    // Implements the ScriptUpdateObserver interface.
    virtual void notifyOnScriptUpdate(const ScriptLibrary* new_lib);
    
    
private Q_SLOTS:
    
    void onStateReqReturned(QByteArray data);
    void onStateChangeAck(QByteArray data);
  
    
private:
    
    const ScriptLibrary* lib_;
    ScriptUpdateSubject* subject_;
    Utils::MessageGroup* reqGroup_;
    Utils::MessageGroup* ackGroup_;
    QString reqGroupName_;
    QString ackGroupName_;
    Utils::StateResponseMessage pendingReq_;
    Utils::SetStateAckMessage pendingAck_;
    QString senderName_;
    
    std::mutex updateMx_;
    std::mutex waitMx_;
    std::condition_variable cv_;
};

}

#endif // SCRIPTAPIIMPLEMENTATION_HH
