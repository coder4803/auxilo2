/* scriptapiadapter.hh
 * 
 * This header defines the ScriptApiAdapter class that wraps ScriptAPI
 * into a QObject so that it could be used in a QtScript program.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#ifndef SCRIPTAPIADAPTER_HH
#define SCRIPTAPIADAPTER_HH

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include "scriptapi.hh"


namespace QtScriptPlugin
{

/*!
 * \brief The ScriptApiQObjectWrapper class
 * This class wraps the ScriptAPI-object inside QObject making it available
 * for QtScript environment.
 */
class ScriptApiAdapter : public QObject
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
    ScriptApiAdapter(SignalHandler::ScriptAPI* api, 
                            QScriptEngine* engine, 
                            QObject* parent = 0);
    
    /*!
     * \brief Destructor.
     */
    virtual ~ScriptApiAdapter();
    
public slots:
    
    // Following slots behave as respective calls to wrapped ScriptAPI object.
    QDateTime dateTimeNow() const;
    QScriptValue getStateOf(const QString& stateName);
    QScriptValue getStates(const QStringList& states);
    int setState(const QString& stateName, const QVariant& value);    
    int sendSignal(const QString& signalName, const QStringList& args);
    QString getSender() const;
    void sendEvent(const QString& signal, const QDateTime& timestamp, quint32 interval_sec, quint32 repeat);
    
    
private:
    SignalHandler::ScriptAPI* api_;
    QScriptEngine* eng_;
};

} // Namespace QtScriptPlugin

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




#endif // SCRIPTAPIADAPTER_HH
