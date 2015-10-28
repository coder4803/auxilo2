#include <QException>
#include <QXmlSimpleReader>
#include <QSet>
#include <QMetaEnum>

#include "groupmodel.h"
#include "devicenameloader.h"
#include "messagegroup.h"
#include "globals.h"

namespace MessageTool {

GroupModel::GroupModel(const QString& deviceFileName,
                       QObject* parent) :
   QStandardItemModel(parent)
{
   QSet<QString> interfaceNames;
   QSet<QString> deviceNames;
   if (!loadDeviceNames(deviceFileName, interfaceNames, deviceNames)) {
      throw QException();
   }

   generateGroupTree(interfaceNames, deviceNames);
}

QStringList GroupModel::getGroups() const
{
   return m_messageTypeTable.keys();
}

Globals::MessageType GroupModel::getMessageTypeByGroup(const QString& group) const
{
   return m_messageTypeTable.value(group);
}

QString GroupModel::getMessageTypeNameByGroup(const QString& group) const
{
   return Globals::staticMetaObject.
         enumerator(Globals::ENUMINDEX_MESSAGE_TYPE).
         valueToKey(getMessageTypeByGroup(group));
}

void GroupModel::handleMessage(QByteArray payload, QString group)
{
   Globals::MessageType messageType = m_messageTypeTable.value(group);
   emit messageReceived(messageType, payload, group);
}

bool GroupModel::loadDeviceNames(const QString& deviceFileName,
                                 QSet<QString>& interfaces,
                                 QSet<QString>& devices)
{
   qCritical("Loading device names...");

   QFile file(deviceFileName);
   if (!file.exists()) {
      qCritical("Couldn't find %s.", deviceFileName.toLatin1().data());
      return false;
   }

   // Parse configuration file.
   QXmlInputSource source(&file);
   QXmlSimpleReader xmlReader;
   DeviceNameLoader deviceNameLoader(interfaces, devices);

   xmlReader.setContentHandler(&deviceNameLoader);
   xmlReader.setErrorHandler(&deviceNameLoader);
   if (!xmlReader.parse(source)) {
      qCritical("Error while reading device names (%s): %s",
                deviceFileName.toLatin1().data(),
                deviceNameLoader.getErrorMsg().toLatin1().data());
      return false;
   }

   qCritical("... OK.");

   return true;
}

QStandardItem* GroupModel::newGroup(Globals::MessageType messageType,
                                    QString group)
{
   Utils::MessageGroup* messageGroup =
         new Utils::MessageGroup(group, Utils::MessageGroup::Both, this);

   connect(messageGroup, SIGNAL(messageReceived(QByteArray,QString)),
           this, SLOT(handleMessage(QByteArray,QString)));

   m_messageTypeTable.insert(group, messageType);
   return new QStandardItem(group);
}

void GroupModel::generateGroupTree(QSet<QString>& interfaceNames,
                                   QSet<QString>& deviceNames)
{
   QStandardItem* rootNode = this->invisibleRootItem();

   // State holder
   QStandardItem* stateHolder = new QStandardItem("State holder");
   stateHolder->appendRow(newGroup(Globals::ConfResponseMessage,
                                   Utils::SH_CONF_RESPONSE_GROUP));
   stateHolder->appendRow(newGroup(Globals::SetStateMessage,
                                   Utils::SET_STATE_GROUP));
   stateHolder->appendRow(newGroup(Globals::RequestStateMessage,
                                   Utils::REQUEST_STATE_GROUP));
   stateHolder->appendRow(newGroup(Globals::StateChangedAckMessage,
                                   Utils::SH_STATE_CHANGED_ACK_GROUP));
   rootNode->appendRow(stateHolder);

   // Confmanager
   QStandardItem* confmanager = new QStandardItem("Confmanager");
   confmanager->appendRow(newGroup(Globals::ConfRequestMessage,
                                   Utils::CONF_REQUEST_GROUP));
   rootNode->appendRow(confmanager);

   // Signal handler
   QStandardItem* signalHandler = new QStandardItem("Signal handler");
   signalHandler->appendRow(newGroup(Globals::SignalMessage,
                                     Utils::SIGNAL_HANDLER_GROUP));
   rootNode->appendRow(signalHandler);

   // Logger
   QStandardItem* logger = new QStandardItem("Logger");
   logger->appendRow(newGroup(Globals::LogMessage,
                              Utils::LOG_MESSAGE_GROUP));

   rootNode->appendRow(logger);

   // Add interfaces
   foreach (QString name, interfaceNames) {
      QStandardItem* interface = new QStandardItem(name);
      interface->appendRow(newGroup(Globals::ConfResponseMessage,
                                    Utils::INTERFACE_CONF_RESPONSE_GROUP));

      rootNode->appendRow(interface);
   }

   // Add devices
   foreach (QString name, deviceNames) {
      QStandardItem* device = new QStandardItem(name);
      device->appendRow(newGroup(Globals::SignalMessage,
                                 name + Utils::SIGNAL_POST_FIX));
      device->appendRow(newGroup(Globals::StateChangedMessage,
                                 name + Utils::STATE_CHANGED_POST_FIX));
      device->appendRow(newGroup(Globals::ConfResponseMessage,
                                 name + Utils::CONF_RESPONSE_POST_FIX));
      device->appendRow(newGroup(Globals::StateResponseMessage,
                                 name + Utils::STATE_RESPONSE_POST_FIX));
      device->appendRow(newGroup(Globals::SignalAckMessage,
                                 name + Utils::SIGNAL_ACK_POST_FIX));
      device->appendRow(newGroup(Globals::SetStateAckMessage,
                                 name + Utils::SET_STATE_ACK_POST_FIX));

      rootNode->appendRow(device);
   }
}

} // MessageTool
