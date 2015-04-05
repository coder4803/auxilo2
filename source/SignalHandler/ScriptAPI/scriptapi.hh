#ifndef SCRIPTAPI_HH
#define SCRIPTAPI_HH

#include <QString>
#include <QStringList>
#include <QVariant>
#include <map>
#include "stateresponsemessage.h"


namespace SignalHandler
{

/*!
 * \brief The ScriptAPI class
 *  Abstract interface for services that SignalHandler offers to the scripts.
 */
class ScriptAPI
{
public:
    
    typedef std::map<QString, Utils::StateResponseMessage::State> StateMap;
    
    /*!
     * \brief The DateTime struct
     * Current date and time.
     */
    struct DateTime
    {
        const unsigned day;
        const unsigned month;
        const unsigned year;
        const unsigned hour;
        const unsigned minutes;
        const unsigned seconds;
        
        //! DateTime Constructor.
        DateTime(unsigned d, unsigned m, unsigned y, 
                 unsigned h, unsigned min, unsigned s) :
            day(d), month(m), year(y), hour(h), minutes(min), seconds(s) {}
    };
    
    //! Constructor
    ScriptAPI() = default;
    
    //! Destructor
    virtual ~ScriptAPI() = default;
    
    //! Copy-construction is forbidden.
    ScriptAPI(const ScriptAPI&) = delete;
    //! Copy-assignment is forbidden.
    ScriptAPI& operator=(const ScriptAPI&) = delete;
    
    /*!
     * \brief dateTimeNow Get current date and time.
     * \return DateTime object representing current time.
     */
    virtual DateTime dateTimeNow() const = 0;
    
    /*!
     * \brief getStateOf Ask a state.
     * \param stateName Name of state to be asked.
     * \return State object representing requested state.
     */
    virtual Utils::StateResponseMessage::State
    getStateOf(const QString& stateName) = 0;
    
    /*!
     * \brief getStates Ask multible states.
     * \param states List of state names.
     * \return Name-State pairs in map.
     */
    virtual StateMap getStates(const QStringList& states) = 0;
    
    /*!
     * \brief setState Request a state change.
     * \param stateName Name of state to be changed.
     * \param value New value for the state.
     * \return 0 if state was changed successfully.
     */
    virtual int setState(const QString& stateName, const QVariant& value) = 0;
    
    /*!
     * \brief sendSignal request another signal to be handled.
     * \param signalName Signal identification.
     * \param args Signal arguments.
     * \return 0 if signal was handled successfully.
     */
    virtual int sendSignal(const QString& signalName, 
                           const QStringList& args) = 0;
};

}


#endif // SCRIPTAPI_HH
