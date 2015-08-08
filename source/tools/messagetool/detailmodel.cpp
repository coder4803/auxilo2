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

namespace MessageTool {

DetailModel::DetailModel(QObject* parent) :
   QStandardItemModel(parent)
{
   setColumnCount(2);
   setHorizontalHeaderItem(0, new QStandardItem("Name"));
   setHorizontalHeaderItem(1, new QStandardItem("Value"));
}

void DetailModel::setDetailData(Globals::MessageType messageType,
                                const QByteArray& data)
{
   clearContent();

   switch (messageType) {
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
   default:
      break;
   }
}

void DetailModel::newRow(QString name, QVariant value)
{
   QStandardItem* nameItem = new QStandardItem(name);
   QStandardItem* valueItem = new QStandardItem(value.toString());

   QList<QStandardItem*> newRow;
   newRow << nameItem;
   newRow << valueItem;

   appendRow(newRow);
}

void DetailModel::clearContent()
{
   removeRows(0, rowCount());
}

void DetailModel::parseConfRequestMessage(const QByteArray& data)
{
   Utils::ConfRequestMessage message(data);

   newRow("Feature name", message.featureName());
   newRow("Core feature", message.isCoreFeature() ? "true" : "false");
   newRow("Response group", message.responseGroup());
}

void DetailModel::parseConfResponseMessage(const QByteArray& data)
{
   Utils::ConfResponseMessage message(data);

   newRow("Result", (int)message.result());

   QHash<QString, QString> parameters = message.parameterSet().parameters();
   foreach (QString name, parameters.keys()) {
      newRow(name, parameters.value(name));
   }
}

void DetailModel::parseSignalMessage(const QByteArray& data)
{
   Utils::SignalMessage message(data);

   newRow("Name", message.signalName());

   for (int i = 0; i < message.parameters().size(); ++i) {
      newRow(QString("Parameter %1").arg(i + 1), message.parameters().at(i));
   }

   if (!message.ackGroup().isEmpty()) {
      newRow("AckGroup", message.ackGroup());
      newRow("AckId", message.ackId());
   }
}

void DetailModel::parseSignalAckMessage(const QByteArray& data)
{
   Utils::SignalAckMessage message(data);

   newRow("Result", (int)message.result());
   newRow("AckId", message.ackId());
}

void DetailModel::parseLogMessage(const QByteArray& data)
{
   Utils::LogMessage message(data);

   newRow("Message", message.message());
   newRow("Type", (int)message.type());
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

   QHash<QString, Utils::StateResponseMessage::State*> states = message.states();
   foreach (QString name, states.keys()) {
      Utils::StateResponseMessage::State* state = states.value(name);
      newRow("State", name);
      newRow("   Available", state->available ? "yes" : "no");
      if (state->available) {
         newRow("   Value", state->value.toString());
         newRow("   UpToDate", state->devicesAreUpToDate ? "yes" : "no");
      }
   }
}

void DetailModel::parseSetStateMessage(const QByteArray& data)
{
   Utils::SetStateMessage message(data);

   newRow("Name", message.name());
   newRow("Value", message.value());

   if (!message.ackGroup().isEmpty()) {
      newRow("AckGroup", message.ackGroup());
      newRow("AckId", message.ackId());
   }
}

void DetailModel::parseSetStateAckMessage(const QByteArray& data)
{
   Utils::SetStateAckMessage message(data);

   newRow("Result", (int)message.result());
   newRow("AckId", message.ackId());
}

void DetailModel::parseStateChangedMessage(const QByteArray& data)
{
   Utils::StateChangedMessage message(data);

   newRow("Label", message.label());
   newRow("Value", message.value());

   if (!message.ackGroup().isEmpty()) {
      newRow("AckGroup", message.ackGroup());
      newRow("AckId", message.ackId());
   }
}

void DetailModel::parseStateChangedAckMessage(const QByteArray& data)
{
   Utils::StateChangedAckMessage message(data);

   newRow("Result", (int)message.result());
   newRow("AckId", message.ackId());
}

} // MessageTool
