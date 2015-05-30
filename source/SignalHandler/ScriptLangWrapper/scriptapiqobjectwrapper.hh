#ifndef SCRIPTAPIQOBJECTWRAPPER_HH
#define SCRIPTAPIQOBJECTWRAPPER_HH

#include <QObject>
#include "ScriptAPI/scriptapi.hh"

namespace SignalHandler
{

/*!
 * \brief The ScriptApiQObjectWrapper class
 * This class wraps the ScriptAPI-object inside QObject making it available
 * for QtScript environment.
 */
class ScriptApiQObjectWrapper : public QObject
{
public:
    
    /*!
     * \brief Constructor.
     * \param api Wrapped ScriptAPI object. Ownership is not transfered.
     * \pre api != nullptr
     * \post Invokeing this objects slots are redirected to api.
     */
    ScriptApiQObjectWrapper(ScriptAPI* api, QObject* parent = 0);
    
    /*!
     * \brief Destructor.
     */
    virtual ~ScriptApiQObjectWrapper();
    
public slots:
    
    // Following slots behave as respective calls to wrapped ScriptAPI object.
    ScriptAPI::DateTime dateTimeNow() const;
    Utils::StateResponseMessage::State getStateOf(const QString& stateName);
    ScriptAPI::StateMap getStates(const QStringList& states);
    int setState(const QString& stateName, const QVariant& value);    
    int sendSignal(const QString& signalName, const QStringList& args);
    
    
private:
    ScriptAPI* api_;
};


}

#endif // SCRIPTAPIQOBJECTWRAPPER_HH
