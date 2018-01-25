#include <QFile>
#include <QDebug>
#include <QDataStream>
#include <QMetaEnum>

#include "script.h"
#include "bytearray/bytearrayclass.h"
#include "parameters/parametersclass.h"

namespace Plugins {

const QString Script::PARAMETER_SCRIPT_FILE("scriptpath");

const QString Script::SLOT_KEY_WORD("slot ");

const QString Script::SLOT_KEY_STARTUP("startup");
const QString Script::SLOT_KEY_CLOSING("closing");
const QString Script::SLOT_KEY_HANDLE_PARAMETERS("handleParameters");
const QString Script::SLOT_KEY_HANDLE_STATE_RESPONSE("handleStateResponse");
const QString Script::SLOT_KEY_HANDLE_SIGNAL("handleSignal");
const QString Script::SLOT_KEY_HANDLE_STATE_CHANGE("handleStateChange");
const QString Script::SLOT_KEY_HANDLE_SIGNAL_ACK("handleSignalAck");
const QString Script::SLOT_KEY_HANDLE_SET_STATE_ACK("handleSetStateAck");
const QString Script::SLOT_KEY_CONNECTED("connected");
const QString Script::SLOT_KEY_DISCONNECTED("disconnected");
const QString Script::SLOT_KEY_DATA_RECEIVED("dataReceived");

const QStringList Script::SLOT_LIST =
        QStringList() << Script::SLOT_KEY_STARTUP
                      << Script::SLOT_KEY_CLOSING
                      << Script::SLOT_KEY_HANDLE_PARAMETERS
                      << Script::SLOT_KEY_HANDLE_STATE_RESPONSE
                      << Script::SLOT_KEY_HANDLE_SIGNAL
                      << Script::SLOT_KEY_HANDLE_STATE_CHANGE
                      << Script::SLOT_KEY_HANDLE_SIGNAL_ACK
                      << Script::SLOT_KEY_HANDLE_SET_STATE_ACK
                      << Script::SLOT_KEY_CONNECTED
                      << Script::SLOT_KEY_DISCONNECTED
                      << Script::SLOT_KEY_DATA_RECEIVED;

// Registers enum values to be available in script
// message: message class name.
// type: enum type name
// name: enum name in script
// totype: function that converts enum to QScriptValue
// fromtype: function that converts enum from QScripValue
#define registerEnumType(message, type, name, totype, fromtype) \
   int indexOf ## name = message::staticMetaObject. \
           indexOfEnumerator(#name); \
   QMetaEnum name ## EnumData = message::staticMetaObject. \
         enumerator(indexOf ## name); \
   QScriptValue name ## Consts = m_engine->newObject(); \
   \
   for (int i = 0; i < name ## EnumData.keyCount(); ++i) { \
      QString valueName = name ## EnumData.valueToKey(i); \
      message::type value = static_cast<message::type>(i); \
      name ## Consts.setProperty(valueName, value); \
   } \
   \
   m_engine->globalObject().setProperty(#name, name ## Consts); \
   qScriptRegisterMetaType(m_engine, totype, fromtype);

Script::Script(Utils::ParameterSet parameters,
               QObject* parent) :
   Protocol(parent),
   m_engine(new QScriptEngine(this)),
   m_coreAPI(new CoreAPI(this)),
   m_commAPI(new CommunicationAPI(this))
{
   try {
      // Script file is read from protocol plugin parameters.
      m_scriptFile = parameters.parameter<QString>(PARAMETER_SCRIPT_FILE);
      qDebug() << "Script file:" << m_scriptFile;

      registerScriptAPI();

      if (!loadScript()) {
         qCritical() << "Failed to load script" << m_scriptFile;
         throw QException();
      }
   } catch (QException& e) {
      qCritical("Failed to initialize protocol.");
      throw e;
   }

   QMetaObject::invokeMethod(this, "runStartup", Qt::QueuedConnection);
}

Script::~Script()
{
   // Run closing slot
   ScriptSlots::const_iterator iter = m_slots.find(SLOT_KEY_CLOSING);
   if (iter != m_slots.constEnd()) {
      iter.value()->call(m_scriptObject, QScriptValueList());
   }
}

void Script::handleParameters(const Utils::ParameterSet& parameters)
{
   ScriptSlots::const_iterator iter = m_slots.find(SLOT_KEY_HANDLE_PARAMETERS);

   if (iter != m_slots.constEnd()) {
      QScriptValueList paramList;
      QVariant var = QVariant::fromValue(parameters);
      QScriptValue parametersVariant = m_engine->toScriptValue(var);
      paramList << parametersVariant;

      iter.value()->call(m_scriptObject, paramList);
   }
}

void Script::handleStateResponse(const QString& stateName,
                                 const QVariant& stateValue,
                                 bool available)
{
   ScriptSlots::const_iterator iter = m_slots.find(
               SLOT_KEY_HANDLE_STATE_RESPONSE);

   if (iter != m_slots.constEnd()) {
      QScriptValueList paramList;
      QScriptValue stateValueVariant = m_engine->newVariant(stateValue);
      paramList << stateName << stateValueVariant << available;

      iter.value()->call(m_scriptObject, paramList);
   }
}

Utils::SignalAckMessage::Result
Script::handleSignal(const QString& name,
                     const QStringList& parameters)
{
   ScriptSlots::const_iterator iter = m_slots.find(SLOT_KEY_HANDLE_SIGNAL);

   if (iter != m_slots.constEnd()) {
      QScriptValue parametersVariant = qScriptValueFromSequence(m_engine, parameters);
      QScriptValueList paramList;
      paramList << name << parametersVariant;

      QScriptValue result = iter.value()->call(m_scriptObject, paramList);
      return static_cast<Utils::SignalAckMessage::Result>(result.toInt32());
   }

   return Utils::SignalAckMessage::SIGNAL_NOT_FOUND;
}

void Script::handleStateChange(const QString& label,
                               const QVariant& value)
{
   ScriptSlots::const_iterator iter = m_slots.find(
               SLOT_KEY_HANDLE_STATE_CHANGE);

   if (iter != m_slots.constEnd()) {
      QScriptValue valueVariant = m_engine->newVariant(value);

      QScriptValueList paramList;
      paramList << label << valueVariant;

      iter.value()->call(m_scriptObject, paramList);
   }
}

void Script::handleSignalAck(Utils::SignalAckMessage::Result result,
                             quint32 ackId)
{
   ScriptSlots::const_iterator iter = m_slots.find(SLOT_KEY_HANDLE_SIGNAL_ACK);

   if (iter != m_slots.constEnd()) {
      QScriptValueList paramList;
      paramList << result << ackId;

      iter.value()->call(m_scriptObject, paramList);
   }
}

void Script::handleSetStateAck(Utils::SetStateAckMessage::Result result,
                               quint32 ackId)
{
   ScriptSlots::const_iterator iter = m_slots.find(
               SLOT_KEY_HANDLE_SET_STATE_ACK);

   if (iter != m_slots.constEnd()) {
      QScriptValueList paramList;
      paramList << result << ackId;

      iter.value()->call(m_scriptObject, paramList);
   }
}

void Script::connected(qint32 connectionId)
{
   ScriptSlots::const_iterator iter = m_slots.find(SLOT_KEY_CONNECTED);

   if (iter != m_slots.constEnd()) {
      QScriptValueList paramList;
      paramList << connectionId;

      iter.value()->call(m_scriptObject, paramList);
   }
}

void Script::disconnected(qint32 connectionId)
{
   ScriptSlots::const_iterator iter = m_slots.find(SLOT_KEY_DISCONNECTED);

   if (iter != m_slots.constEnd()) {
      QScriptValueList paramList;
      paramList << connectionId;

      iter.value()->call(m_scriptObject, paramList);
   }
}

void Script::dataReceived(QByteArray data,
                          qint32 connectionId)
{
   ScriptSlots::const_iterator iter = m_slots.find(SLOT_KEY_DATA_RECEIVED);

   if (iter != m_slots.constEnd()) {
      QScriptValue dataVariant = m_engine->newVariant(data);

      QScriptValueList paramList;
      paramList << dataVariant << connectionId;

      iter.value()->call(m_scriptObject, paramList);
   }
}

void Script::registerScriptAPI()
{
   qDebug() << "Initializing script API...";

   // Register enum types
   registerEnumType(Utils::StateChangedAckMessage,
                    Result, StateChangeResult,
                    toScriptStateChangedAckMessage,
                    fromScriptStateChangedAckMessage);

   registerEnumType(Utils::SignalAckMessage,
                    Result, SignalResult,
                    toScriptSignalAckMessage,
                    fromScriptSignalAckMessage);

   registerEnumType(Utils::LogMessage,
                    LogType, LogType,
                    toScriptLogMessage,
                    fromScriptLogMessage);

   // Register ByteArray for script usage
   ByteArrayClass* baClass = new ByteArrayClass(m_engine);
   m_engine->globalObject().setProperty("ByteArray", baClass->constructor());

   // Register Parameters for script usage
   ParametersClass* psClass = new ParametersClass(m_engine);
   m_engine->globalObject().setProperty("Parameters", psClass->constructor());

   // Register script API
   QScriptValue coreAPI = m_engine->newQObject(m_coreAPI);
   QScriptValue commAPI = m_engine->newQObject(m_commAPI);
   m_engine->globalObject().setProperty("Core", coreAPI);
   m_engine->globalObject().setProperty("Comm", commAPI);

   qDebug() << "Script API initialized.";
}

bool Script::loadScript()
{
   qDebug() << "Loading script...";

   // Load parameter file.
   QFile file(m_scriptFile);

   if (!file.open(QIODevice::ReadOnly)) {
      qCritical() << "Couldn't open" << m_scriptFile;
      return false;
   }

   QString script = file.readAll();
   file.close();

   // Parse slots
   bool done = false;
   int startIndex = 0;
   int index = 0;
   int firstSlotIndex = -1;

   // Add necessary 'additions' that QScript understands the script
   while (!done) {
      startIndex = script.indexOf(SLOT_KEY_WORD, index, Qt::CaseInsensitive);
      if (startIndex > -1) {
         index = startIndex + SLOT_KEY_WORD.size();
         int endIndex = script.indexOf("(", index, Qt::CaseInsensitive);
         if (endIndex > -1) {
            int length = endIndex - index;
            QString slotName = script.mid(index, length);

            // 'Slot names' containing linefeeds
            // are not keywords we are lookin for
            if (slotName.contains("\n")) {
                continue;
            }

            // Do not handle unknown slots!
            if (!SLOT_LIST.contains(slotName)) {
               qCritical() << "Error: Unknown slot:" << slotName;
               return false;
            }

            // 'Slot <function-name>' strings are replaced with
            // '<function-name>: function'
            script.replace(startIndex, length + SLOT_KEY_WORD.size(),
                           slotName + ": function");

            // Each slot must be separated with ','
            if (firstSlotIndex >= 0) {
               script.insert(startIndex, ",");
            } else {
                firstSlotIndex = startIndex;
            }
         } else {
            // End of file reached
            done = true;
         }
      } else {
         // No more slot-keywords found
         done = true;
      }
   }

   // Whole script must be covered with "double-brackets" but let
   // user to define global variables at the beginning of the file.
   script.insert(firstSlotIndex, "({");
   script.append("})");

   // Evaluate script.
   m_scriptObject = m_engine->evaluate(script);
   if (m_engine->hasUncaughtException()) {
      qCritical() << "Error in " << m_scriptFile << "on line"
                  << m_engine->uncaughtExceptionLineNumber();

      QStringList backtrace = m_engine->uncaughtExceptionBacktrace();
      if (!backtrace.isEmpty()) {
         qCritical() << "Backtrace:";
         for (int i = 0; i < backtrace.size(); ++i) {
            qCritical() << "   " << backtrace.at(i);
         }
      }

      return false;
   }

   // Load slots from script.
   foreach (QString slotName, SLOT_LIST) {
      QScriptValue* slot = new QScriptValue();
      *slot = m_scriptObject.property(slotName);
      if (slot->isValid()) {
         qDebug("Found %s from script!", slotName.toLatin1().data());
         m_slots.insert(slotName, slot);
      } else {
         delete slot;
      }
   }

   return true;
}

void Script::runStartup()
{
   ScriptSlots::const_iterator iter = m_slots.find(SLOT_KEY_STARTUP);
   if (iter != m_slots.constEnd()) {
      iter.value()->call(m_scriptObject, QScriptValueList());
   }
}

} // Plugins

QScriptValue toScriptStateChangedAckMessage(QScriptEngine* engine,
                                const Utils::StateChangedAckMessage::Result& s)
{
   Q_UNUSED(engine);
   return QScriptValue(static_cast<int>(s));
}

void fromScriptStateChangedAckMessage(const QScriptValue& value,
                                      Utils::StateChangedAckMessage::Result &s)
{
   s = static_cast<Utils::StateChangedAckMessage::Result>(value.toInt32());
}

QScriptValue toScriptSignalAckMessage(QScriptEngine* engine,
                                      const Utils::SignalAckMessage::Result& s)
{
   Q_UNUSED(engine);
   return QScriptValue(static_cast<int>(s));
}

void fromScriptSignalAckMessage(const QScriptValue& value,
                                Utils::SignalAckMessage::Result& s)
{
   s = static_cast<Utils::SignalAckMessage::Result>(value.toInt32());
}


QScriptValue toScriptLogMessage(QScriptEngine* engine,
                                const Utils::LogMessage::LogType& s)
{
   Q_UNUSED(engine);
   return QScriptValue(static_cast<int>(s));
}

void fromScriptLogMessage(const QScriptValue& value,
                          Utils::LogMessage::LogType &s)
{
   s = static_cast<Utils::LogMessage::LogType>(value.toInt32());
}
