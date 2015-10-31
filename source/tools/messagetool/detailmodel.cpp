#include <QComboBox>
#include <QMetaEnum>

#include "detailmodel.h"

#include "confrequestmessage.h"
#include "confresponsemessage.h"
#include "signalmessage.h"
#include "signalackmessage.h"
#include "logmessage.h"
#include "requeststatemessage.h"
#include "stateresponsemessage.h"
#include "setstatemessage.h"
#include "setstateackmessage.h"
#include "statechangedmessage.h"
#include "statechangedackmessage.h"
#include "eventmessage.h"

namespace MessageTool {

DetailModel::DetailModel(QObject* parent) :
   QStandardItemModel(parent),
   m_messageType(Globals::SignalMessage),
   m_ductile(false)
{
   setColumnCount(NUMBER_OF_COLUMNS);
   setHorizontalHeaderItem(COLUMN_NAME, new QStandardItem("Name"));
   setHorizontalHeaderItem(COLUMN_VALUE, new QStandardItem("Value"));
}

DetailModel::~DetailModel()
{
   clearContent();
}

void DetailModel::setDetailData(Globals::MessageType messageType,
                                const QByteArray& data)
{
   m_messageType = messageType;

   clearContent();

   switch (m_messageType) {
   case Globals::ConfRequestMessage:
      parseConfRequestMessage(data);
      break;
   case Globals::ConfResponseMessage:
      parseConfResponseMessage(data);
      break;
   case Globals::SignalMessage:
      parseSignalMessage(data);
      break;
   case Globals::SignalAckMessage:
      parseSignalAckMessage(data);
      break;
   case Globals::LogMessage:
      parseLogMessage(data);
      break;
   case Globals::RequestStateMessage:
      parseRequestStateMessage(data);
      break;
   case Globals::StateResponseMessage:
      parseStateResponseMessage(data);
      break;
   case Globals::SetStateMessage:
      parseSetStateMessage(data);
      break;
   case Globals::SetStateAckMessage:
      parseSetStateAckMessage(data);
      break;
   case Globals::StateChangedMessage:
      parseStateChangedMessage(data);
      break;
   case Globals::StateChangedAckMessage:
      parseStateChangedAckMessage(data);
      break;
   case Globals::EventMessage:
      parseEventMessage(data);
      break;
   default:
      break;
   }
}

void DetailModel::clearContent()
{
   removeRows(0, rowCount());
}

void DetailModel::enableEditing(bool ductile)
{
   m_ductile = ductile;
}

QStringList DetailModel::getOptions(int row)
{
   return index(row, COLUMN_VALUE).data(Qt::UserRole).toString().split(",");
}

QString DetailModel::getAddButtonText()
{
   switch (m_messageType) {
   case Globals::ConfResponseMessage:
      return QString("Add parameter");
   case Globals::StateResponseMessage:
      return QString("Add state");
   default:
      return QString();
   }
}

QByteArray DetailModel::getMessageData()
{
   switch (m_messageType) {
   case Globals::ConfRequestMessage:
      return createConfRequestMessage();
   case Globals::ConfResponseMessage:
      return createConfResponseMessage();
   case Globals::SignalMessage:
      return createSignalMessage();
   case Globals::SignalAckMessage:
      return createSignalAckMessage();
   case Globals::LogMessage:
      return createLogMessage();
   case Globals::RequestStateMessage:
      return createRequestStateMessage();
   case Globals::StateResponseMessage:
      return createStateResponseMessage();
   case Globals::SetStateMessage:
      return createSetStateMessage();
   case Globals::SetStateAckMessage:
      return createSetStateAckMessage();
   case Globals::StateChangedMessage:
      return createStateChangedMessage();
   case Globals::StateChangedAckMessage:
      return createStateChangedAckMessage();
   case Globals::EventMessage:
      return createEventMessage();
   default:
      return QByteArray();
   }
}

void DetailModel::addEmptyRow()
{
   newRow(QString(), QString(), QStringList(), true);
}

void DetailModel::newRow(QString name,
                         QVariant value,
                         QStringList options,
                         bool ductile)
{
   QStandardItem* nameItem = new QStandardItem(name);
   nameItem->setData(ductile, Qt::UserRole);

   QStandardItem* valueItem = new QStandardItem(value.toString());
   valueItem->setData(options.join(","), Qt::UserRole);

   QList<QStandardItem*> newRow;
   newRow << nameItem;
   newRow << valueItem;

   appendRow(newRow);
}

void DetailModel::parseConfRequestMessage(const QByteArray& data)
{
   Utils::ConfRequestMessage message(data);

   QStringList options = QStringList() << "false" << "true";

   newRow("Feature name", message.featureName());
   newRow("Core feature", message.isCoreFeature() ? "true" : "false", options);
   newRow("Response group", message.responseGroup());
}

void DetailModel::parseConfResponseMessage(const QByteArray& data)
{
   Utils::ConfResponseMessage message(data);

   QStringList resultOptions;
   int indexOfResult = message.staticMetaObject.indexOfEnumerator("Result");
   QMetaEnum resultEnumData = message.staticMetaObject.
         enumerator(indexOfResult);
   for (int i = 0; i < resultEnumData.keyCount(); ++i) {
      resultOptions << resultEnumData.valueToKey(i);
   }

   newRow("Result", resultEnumData.valueToKey(message.result()), resultOptions);
   newRow("Feature name", message.parameterSet().featureName());

   QHash<QString, QString> parameters = message.parameterSet().parameters();
   foreach (QString name, parameters.keys()) {
      newRow(name, parameters.value(name), QStringList(), true);
   }
}

void DetailModel::parseSignalMessage(const QByteArray& data)
{
   Utils::SignalMessage message(data);

   newRow("Name", message.signalName());
   newRow("Sender", message.senderName());

   if (!message.ackGroup().isEmpty() || m_ductile) {
      newRow("AckGroup", message.ackGroup());

      if (!m_ductile) {
         newRow("AckId", message.ackId());
      }
   }

   newRow(QString("Parameters"), message.parameters().join(", "));
}

void DetailModel::parseSignalAckMessage(const QByteArray& data)
{
   Utils::SignalAckMessage message(data);

   QStringList resultOptions;
   int indexOfResult = message.staticMetaObject.indexOfEnumerator("Result");
   QMetaEnum resultEnumData = message.staticMetaObject.
         enumerator(indexOfResult);
   for (int i = 0; i < resultEnumData.keyCount(); ++i) {
      resultOptions << resultEnumData.valueToKey(i);
   }

   newRow("Result", resultEnumData.valueToKey(message.result()), resultOptions);
   newRow("AckId", message.ackId());
}

void DetailModel::parseLogMessage(const QByteArray& data)
{
   Utils::LogMessage message(data);

   QStringList typeOptions;
   int indexOfType = message.staticMetaObject.indexOfEnumerator("LogType");
   QMetaEnum typeEnumData = message.staticMetaObject.enumerator(indexOfType);
   for (int i = 0; i < typeEnumData.keyCount(); ++i) {
      typeOptions << typeEnumData.valueToKey(i);
   }

   newRow("Message", message.message());
   newRow("Type", typeEnumData.valueToKey(message.type()), typeOptions);
   newRow("Sender", message.sender());
}

void DetailModel::parseRequestStateMessage(const QByteArray& data)
{
   Utils::RequestStateMessage message(data);

   newRow("States", message.states().join(", "));
   newRow("Response group", message.responseGroup());
}

void DetailModel::parseStateResponseMessage(const QByteArray& data)
{
   Utils::StateResponseMessage message(data);

   QHash<QString, Utils::StateResponseMessage::State*>
         states = message.states();

   foreach (QString name, states.keys()) {
      Utils::StateResponseMessage::State* state = states.value(name);

      if (!state->available) {
         newRow(name, "<not available>", QStringList(), true);
      } else {
         if (state->devicesAreUpToDate) {
            newRow(name, state->value.toString(), QStringList(), true);
         } else {
            newRow(name, state->value.toString() + " <OutOfDate>",
                   QStringList(), true);
         }
      }
   }
}

void DetailModel::parseSetStateMessage(const QByteArray& data)
{
   Utils::SetStateMessage message(data);

   newRow("Name", message.name());
   newRow("Value", message.value());

   if (!message.ackGroup().isEmpty() || m_ductile) {
      newRow("AckGroup", message.ackGroup());

      if (!m_ductile) {
         newRow("AckId", message.ackId());
      }
   }
}

void DetailModel::parseSetStateAckMessage(const QByteArray& data)
{
   Utils::SetStateAckMessage message(data);

   QStringList resultOptions;
   int indexOfResult = message.staticMetaObject.indexOfEnumerator("Result");
   QMetaEnum resultEnumData = message.staticMetaObject.
         enumerator(indexOfResult);
   for (int i = 0; i < resultEnumData.keyCount(); ++i) {
      resultOptions << resultEnumData.valueToKey(i);
   }

   newRow("Result", resultEnumData.valueToKey(message.result()), resultOptions);
   newRow("AckId", message.ackId());
}

void DetailModel::parseStateChangedMessage(const QByteArray& data)
{
   Utils::StateChangedMessage message(data);

   newRow("Label", message.label());
   newRow("Value", message.value());

   if (!message.ackGroup().isEmpty() || m_ductile) {
      newRow("AckGroup", message.ackGroup());

      if (!m_ductile) {
         newRow("AckId", message.ackId());
      }
   }
}

void DetailModel::parseStateChangedAckMessage(const QByteArray& data)
{
   Utils::StateChangedAckMessage message(data);

   QStringList resultOptions;
   int indexOfResult = message.staticMetaObject.indexOfEnumerator("Result");
   QMetaEnum resultEnumData = message.staticMetaObject.
         enumerator(indexOfResult);
   for (int i = 0; i < resultEnumData.keyCount(); ++i) {
      resultOptions << resultEnumData.valueToKey(i);
   }

   newRow("Result", resultEnumData.valueToKey(message.result()), resultOptions);
   newRow("AckId", message.ackId());
}

void DetailModel::parseEventMessage(const QByteArray& data)
{
   Utils::EventMessage message(data);

   QString signalName = message.signalName();
   QDateTime timestamp = message.timestamp();
   quint32 interval = message.interval();
   quint32 repeat = message.repeat();

   newRow("Signal name", signalName);
   newRow("Timestamp", timestamp.toString("dd:MM:yyyy hh:mm:ss"));
   newRow("Interval", interval);
   newRow("Repeat", repeat);
}

QByteArray DetailModel::createConfRequestMessage()
{
   QString featureName = item(0, COLUMN_VALUE)->data(Qt::DisplayRole).toString();
   bool coreFeature = item(1, COLUMN_VALUE)->data(Qt::DisplayRole).toBool();
   QString responseGroup = item(2, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   Utils::ConfRequestMessage message(responseGroup, featureName, coreFeature);
   return message.data();
}

QByteArray DetailModel::createConfResponseMessage()
{
   QString resultAsString = item(0, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   int indexOfResult = Utils::ConfResponseMessage::staticMetaObject.
         indexOfEnumerator("Result");
   QMetaEnum resultEnumData = Utils::ConfResponseMessage::staticMetaObject.
         enumerator(indexOfResult);

   Utils::ConfResponseMessage::Result result;
   result = static_cast<Utils::ConfResponseMessage::Result>(
            resultEnumData.keyToValue(resultAsString.toLatin1().data()));

   QString featureName = item(1, COLUMN_VALUE)->data(Qt::DisplayRole).toString();
   Utils::ParameterSet parameterSet(featureName);

   for (int i = 2; i < rowCount(); ++i) {
      QString parameter = item(i, COLUMN_NAME)->data(Qt::DisplayRole).toString();
      QString value = item(i, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

      if (!parameter.isEmpty() && !parameter.isEmpty()) {
         parameterSet.appendParameter(parameter, value);
      }
   }

   Utils::ConfResponseMessage message(parameterSet);
   message.setResult(result);
   return message.data();
}

QByteArray DetailModel::createSignalMessage()
{
   QString signalName = item(0, COLUMN_VALUE)->data(Qt::DisplayRole).toString();
   QString senderName = item(1, COLUMN_VALUE)->data(Qt::DisplayRole).toString();
   QString ackGroup = item(2, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   QString parameters = item(3, COLUMN_VALUE)->data(Qt::DisplayRole).toString();
   QStringList listOfParameters = parameters.split(",");

   Utils::SignalMessage message(signalName, senderName, listOfParameters,
                                ackGroup);
   return message.data();
}

QByteArray DetailModel::createSignalAckMessage()
{
   QString resultAsString = item(0, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   int indexOfResult = Utils::SignalAckMessage::staticMetaObject.
         indexOfEnumerator("Result");
   QMetaEnum resultEnumData = Utils::SignalAckMessage::staticMetaObject.
         enumerator(indexOfResult);

   Utils::SignalAckMessage::Result result;
   result = static_cast<Utils::SignalAckMessage::Result>(
            resultEnumData.keyToValue(resultAsString.toLatin1().data()));

   bool ok = false;
   int ackId = item(1, COLUMN_VALUE)->data(Qt::DisplayRole).toInt(&ok);
   if (!ok) {
      qCritical("Invalid ackId: %s", item(1, COLUMN_VALUE)->
                data(Qt::DisplayRole).toString().toLatin1().data());
      return QByteArray();
   }

   Utils::SignalAckMessage message(ackId, result);
   return message.data();
}

QByteArray DetailModel::createLogMessage()
{
   QString messageText = item(0, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   QString logTypeAsString = item(1, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   int indexOfLogType = Utils::LogMessage::staticMetaObject.
         indexOfEnumerator("LogType");
   QMetaEnum typeEnumData = Utils::LogMessage::staticMetaObject.
         enumerator(indexOfLogType);

   Utils::LogMessage::LogType logType;
   logType = static_cast<Utils::LogMessage::LogType>(
            typeEnumData.keyToValue(logTypeAsString.toLatin1().data()));

   QString sender = item(2, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   Utils::LogMessage message(messageText, logType, sender);
   return message.data();
}

QByteArray DetailModel::createRequestStateMessage()
{
   QStringList states = item(0, COLUMN_VALUE)->data(Qt::DisplayRole).toString().split(",");
   QString responseGroup = item(1, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   Utils::RequestStateMessage message(responseGroup, states);
   return message.data();
}

QByteArray DetailModel::createStateResponseMessage()
{
   Utils::StateResponseMessage message;

   for (int i = 0; i < rowCount(); ++i) {
      QString parameterName = item(i, COLUMN_NAME)->data(Qt::DisplayRole).toString();
      QString parameterValue = item(i, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

      Utils::StateResponseMessage::State state;

      if (parameterValue != QString("<not available>")) {
         state.available = true;
      }

      if (state.available) {
         if (!parameterValue.endsWith(" <OutOfDate>")) {
            state.devicesAreUpToDate = true;
         } else {
            parameterValue.chop(12);
         }

         state.value = parameterValue;
      }

      message.appendState(parameterName, state);
   }

   return message.data();
}

QByteArray DetailModel::createSetStateMessage()
{
   QString name = item(0, COLUMN_VALUE)->data(Qt::DisplayRole).toString();
   QString value = item(1, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   QString ackGroup = item(2, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   Utils::SetStateMessage message(name, value, ackGroup);
   return message.data();
}

QByteArray DetailModel::createSetStateAckMessage()
{
   QString resultAsString = item(0, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   int indexOfResult = Utils::SetStateAckMessage::staticMetaObject.
         indexOfEnumerator("Result");
   QMetaEnum resultEnumData = Utils::SetStateAckMessage::staticMetaObject.
         enumerator(indexOfResult);

   Utils::SetStateAckMessage::Result result;
   result = static_cast<Utils::SetStateAckMessage::Result>(
            resultEnumData.keyToValue(resultAsString.toLatin1().data()));

   bool ok = false;
   int ackId = item(1, COLUMN_VALUE)->data(Qt::DisplayRole).toInt(&ok);
   if (!ok) {
      qCritical("Invalid ackId: %s", item(1, COLUMN_VALUE)->
                data(Qt::DisplayRole).toString().toLatin1().data());
      return QByteArray();
   }

   Utils::SetStateAckMessage message(ackId, result);
   return message.data();
}

QByteArray DetailModel::createStateChangedMessage()
{
   QString label = item(0, COLUMN_VALUE)->data(Qt::DisplayRole).toString();
   QString value = item(1, COLUMN_VALUE)->data(Qt::DisplayRole).toString();
   QString ackGroup = item(2, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   Utils::StateChangedMessage message(label, value, ackGroup);
   return message.data();
}

QByteArray DetailModel::createStateChangedAckMessage()
{
   QString resultAsString = item(0, COLUMN_VALUE)->data(Qt::DisplayRole).toString();

   int indexOfResult = Utils::StateChangedAckMessage::staticMetaObject.
         indexOfEnumerator("Result");
   QMetaEnum resultEnumData = Utils::StateChangedAckMessage::staticMetaObject.
         enumerator(indexOfResult);

   Utils::StateChangedAckMessage::Result result;
   result = static_cast<Utils::StateChangedAckMessage::Result>(
            resultEnumData.keyToValue(resultAsString.toLatin1().data()));

   bool ok = false;
   int ackId = item(1, COLUMN_VALUE)->data(Qt::DisplayRole).toInt(&ok);
   if (!ok) {
      qCritical("Invalid ackId: %s", item(1, COLUMN_VALUE)->
                data(Qt::DisplayRole).toString().toLatin1().data());
      return QByteArray();
   }

   Utils::StateChangedAckMessage message(ackId, result);
   return message.data();
}

QByteArray DetailModel::createEventMessage()
{
   bool ok = false;

   QString signalName = item(0, COLUMN_VALUE)->
         data(Qt::DisplayRole).toString();

   QDateTime timestamp = QDateTime::fromString(item(1, COLUMN_VALUE)->
                                               data(Qt::DisplayRole).toString(),
                                               "dd:MM:yyyy hh:mm:ss");

   quint32 interval = item(2, COLUMN_VALUE)->data(Qt::DisplayRole).toInt(&ok);
   if (!ok) {
      qCritical("Invalid interval: %s", item(2, COLUMN_VALUE)->
                data(Qt::DisplayRole).toString().toLatin1().data());
      return QByteArray();
   }

   quint32 repeat = item(3, COLUMN_VALUE)->data(Qt::DisplayRole).toInt(&ok);
   if (!ok) {
      qCritical("Invalid repeat: %s", item(3, COLUMN_VALUE)->
                data(Qt::DisplayRole).toString().toLatin1().data());
      return QByteArray();
   }

   Utils::EventMessage message(signalName, timestamp, interval, repeat);
   return message.data();
}

} // MessageTool
