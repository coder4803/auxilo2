#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>

#include "signalackmessage.h"
#include "confresponsemessage.h"
#include "protocols.h"
#include "coreapi.h"
#include "communicationapi.h"

#ifndef SCRIPT_H
#define SCRIPT_H

namespace Plugins {

/*!
 * \brief The Script class.
 * This class describes plugin for running scripts as protocol plugin.
 */
class Script : public Protocol
{
   Q_OBJECT

   friend class CommunicationAPI;
public:
   /*!
    * \brief Constructor.
    * \param parameters Protocol parameters.
    * \param parent Parent object.
    */
   explicit Script(Utils::ParameterSet parameters,
                   QObject* parent = NULL);

   //! Destructor.
   ~Script();

   /*!
    * \brief Calls the corresponding function from the script.
    * \param parameters Unused.
    */
   void handleParameters(const Utils::ParameterSet& parameters);

   /*!
    * \brief Calls the corresponding function from the script.
    * \param stateName Unused.
    * \param stateValue Unused.
    * \param available Unused.
    */
   void handleStateResponse(const QString& stateName,
                            const QVariant& stateValue,
                            bool available);

   /*!
    * \brief Calls the corresponding function from the script.
    * \param name Unused.
    * \param parameters Unused.
    * \return Result of handling signal.
    */
   Utils::SignalAckMessage::Result handleSignal(const QString& name,
                                                const QStringList& parameters);

   /*!
    * \brief Calls the corresponding function from the script.
    * \param label Device's state name label.
    * \param value New state value.
    */
   void handleStateChange(const QString& label,
                          const QVariant& value);

   /*!
    * \brief Calls the corresponding function from the script.
    * \param result Result of handling signal.
    * \param ackId Acknowledge id.
    */
   void handleSignalAck(Utils::SignalAckMessage::Result result,
                        quint32 ackId);

   /*!
    * \brief Calls the corresponding function from the script.
    * \param result Result of settign state.
    * \param ackId Acknowledge id.
    */
   void handleSetStateAck(Utils::SetStateAckMessage::Result result,
                          quint32 ackId);

   /*!
    * \brief Calls the corresponding function from the script.
    * \param connectionId Connection id.
    */
   void connected(qint32 connectionId);

   /*!
    * \brief Calls the corresponding function from the script.
    * \param connectionId Connection id.
    */
   void disconnected(qint32 connectionId);

   /*!
    * \brief Calls the corresponding function from the script.
    * \param data Received data.
    * \param connectionId Connection id.
    */
   void dataReceived(QByteArray data,
                     qint32 connectionId);

private:
   typedef QHash<QString, QScriptValue*> ScriptSlots;

   //! Constant for script file parameter name.
   static const QString PARAMETER_SCRIPT_FILE;

   //! Constant for slot key word.
   static const QString SLOT_KEY_WORD;

   //! Slot constants
   static const QString SLOT_KEY_STARTUP;
   static const QString SLOT_KEY_CLOSING;
   static const QString SLOT_KEY_HANDLE_PARAMETERS;
   static const QString SLOT_KEY_HANDLE_STATE_RESPONSE;
   static const QString SLOT_KEY_HANDLE_SIGNAL;
   static const QString SLOT_KEY_HANDLE_STATE_CHANGE;
   static const QString SLOT_KEY_HANDLE_SIGNAL_ACK;
   static const QString SLOT_KEY_HANDLE_SET_STATE_ACK;
   static const QString SLOT_KEY_CONNECTED;
   static const QString SLOT_KEY_DISCONNECTED;
   static const QString SLOT_KEY_DATA_RECEIVED;

   //! List of supported script slot functions.
   static const QStringList SLOT_LIST;

   //! Script file path.
   QString m_scriptFile;

   //! Script engine.
   QScriptEngine* m_engine;

   //! Script engine object.
   QScriptValue m_scriptObject;

   //! Instance of Core API class.
   CoreAPI* m_coreAPI;

   //! Instance of Communication API class.
   CommunicationAPI* m_commAPI;

   //! List of found script slot functions.
   ScriptSlots m_slots;

   /*!
    * \brief Registers script API.
    */
   void registerScriptAPI();

   /*!
    * \brief Loads the script.
    * \return True on success, false on failure.
    */
   bool loadScript();

   /*!
    * \brief Runs startup slot.
    */
   Q_INVOKABLE void runStartup();
};

} // Plugins

// Meta type declaration and value converters for state change ack result type.
Q_DECLARE_METATYPE(Utils::StateChangedAckMessage::Result)
QScriptValue toScriptStateChangedAckMessage(QScriptEngine *engine,
                               const Utils::StateChangedAckMessage::Result &s);
void fromScriptStateChangedAckMessage(const QScriptValue &obj,
                                      Utils::StateChangedAckMessage::Result &s);

// Meta type declaration and value converters for signal ack result type.
Q_DECLARE_METATYPE(Utils::SignalAckMessage::Result)
QScriptValue toScriptSignalAckMessage(QScriptEngine *engine,
                                      const Utils::SignalAckMessage::Result &s);
void fromScriptSignalAckMessage(const QScriptValue &obj,
                                Utils::SignalAckMessage::Result &s);

// Meta type declaration and value converters for log message type.
Q_DECLARE_METATYPE(Utils::LogMessage::LogType)
QScriptValue toScriptLogMessage(QScriptEngine *engine,
                                const Utils::LogMessage::LogType &s);
void fromScriptLogMessage(const QScriptValue &obj,
                          Utils::LogMessage::LogType &s);

Q_DECLARE_METATYPE(Utils::ParameterSet)

DEFINE_PROTOCOL_PLUGIN(Script)

#endif
