#ifndef SCRIPTAPIQOBJECTWRAPPER_HH
#define SCRIPTAPIQOBJECTWRAPPER_HH

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include "../ScriptAPI/scriptapi.hh"


namespace SignalHandler
{

/*!
 * \brief The ScriptApiQObjectWrapper class
 * This class wraps the ScriptAPI-object inside QObject making it available
 * for QtScript environment.
 */
class ScriptApiQObjectWrapper : public QObject
{
    Q_OBJECT
    
public:
    
    /*!
     * \brief Constructor.
     * \param api Wrapped ScriptAPI object. Ownership is not transfered.
     * \param engine QScriptEngine, where where wrapper is used.
     * \param parent QObject's parent.
     * \pre api != nullptr, engine != nullptr.
     * \post Invokes of this object's slots are redirected to api.
     */
    ScriptApiQObjectWrapper(ScriptAPI* api, QScriptEngine* engine, 
                            QObject* parent = 0);
    
    /*!
     * \brief Destructor.
     */
    virtual ~ScriptApiQObjectWrapper();
    
public slots:
    
    // Following slots behave as respective calls to wrapped ScriptAPI object.
    QDateTime dateTimeNow() const;
    QScriptValue getStateOf(const QString& stateName);
    QScriptValue getStates(const QStringList& states);
    int setState(const QString& stateName, const QVariant& value);    
    int sendSignal(const QString& signalName, const QStringList& args);
    
    
private:
    ScriptAPI* api_;
    QScriptEngine* eng_;
};

} // Namespace SignalHandler

// Conversions from custom type to QScriptValue

/*!
 * \brief Converts Utils::StateResponseMessage::State into QScriptValue.
 * \param engine engine, where conversion will be declared.
 * \param state Value to be converted.
 * \pre engine != nullptr.
 * \return QScriptValue constructed from state.
 */
QScriptValue toScriptValue(QScriptEngine* engine,
                           const Utils::StateResponseMessage::State& state);

/*!
 * \brief Converts QScriptValue into Utils::StateResponseMessage::State.
 * \param obj Value to be converted.
 * \param state conversion result.
 * \pre None
 * \post Conversion result is stored into state.
 */
void fromScriptValue(const QScriptValue& obj,
                     Utils::StateResponseMessage::State& state);


#endif // SCRIPTAPIQOBJECTWRAPPER_HH
