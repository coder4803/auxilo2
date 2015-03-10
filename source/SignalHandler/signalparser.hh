
/* signalparser.hh
 * 
 * This header file declares the SignalParser class.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 04-Mar-2015
 * Last Modified: 07-Mar-2015
 */

#ifndef SIGNALPARSER_HH
#define SIGNALPARSER_HH

#include "signal.hh"
#include "priorityupdateobserver.hh"
#include "badmessage.hh"
#include <QString>
#include <exception>
#include <mutex>
#include <QDebug>


namespace SignalHandler
{

class PriorityUpdateSubject;

/*!
 * \brief The SignalParser class
 * Class that converts incoming messages into Signal objects.
 */
class SignalParser : public PriorityUpdateObserver
{   
public:
    
    /*!
     * \brief SignalParser Constructor
     * \param lib Script priority source.
     * \param subject Subjet to be observed.
     * \pre lib != nullptr.
     * \post Object uses lib as script priority source. If subject was not 
     *  null-pointer, subject is observed.
     */
    explicit SignalParser(const ScriptPriorityLibrary* lib,
                          PriorityUpdateSubject* subject = nullptr);
    
    //! Destructor
    virtual ~SignalParser();
    
    //! Copy-construction is forbidden.
    SignalParser(const SignalParser&) = delete;
    
    //! Copy-assignment is forbidden.
    SignalParser& operator = (const SignalParser&) = delete;
    
    // PriorityUpdateObserver methods (see priorityupdateobserver.hh):
    virtual void notifyOnPriorityUpdate(const ScriptPriorityLibrary* new_lib);
    
    /*!
     * \brief parse Converts given message into a Signal object.
     * \param message Message to be converted.
     * \return Converted Signal.
     * \pre -
     * \exception BadMessage is thrown, if message is invalid. If scriptID in
     *  message is unknown, UnknownScript is thrown.
     * 
     * Valid Messages are of form:
     * scriptID[; ...arbitary number of arguments seperated with ';']
     * Where scriptID is a non-negative integer.
     */
    Signal parse(const QString& message);
    
private:
    
    const ScriptPriorityLibrary* lib_;
    PriorityUpdateSubject* subject_;
    std::mutex update_mutex_;
    static const QChar FIELD_SEPERATOR_;
    
    friend class SignalTest;
};


} // Namespace SignalHandler

#endif // SIGNALPARSER_HH
