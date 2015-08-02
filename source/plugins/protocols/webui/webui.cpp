#include "webui.h"

namespace Plugins {

WebUI::WebUI(Utils::ParameterSet parameters,
                   QObject* parent) :
   Protocol(parent)
{
   Q_UNUSED(parameters)
}

WebUI::~WebUI()
{
}

void WebUI::handleParameters(const Utils::ParameterSet& parameters)
{
   Q_UNUSED(parameters)
}

void WebUI::handleStateResponse(const QString& stateName,
                                const QVariant& stateValue,
                                bool available)
{
   QString message;

   if (available) {
      message = QString("$val:%1=%2\n").arg(stateName)
            .arg(stateValue.toString());
   } else {
      message = QString("$err:State not available\n");
   }

   foreach (User* user, m_users) {
      if (user->registeredStates.contains(stateName) ||
          user->requestedStates.contains(stateName)) {
         sendData(message.toLatin1().data(), m_users.key(user));
         user->requestedStates.remove(stateName);
      }
   }
}

Utils::SignalAckMessage::Result
WebUI::handleSignal(const QString& name,
                    const QStringList& parameters)
{
   Q_UNUSED(name)
   Q_UNUSED(parameters)
   return Utils::SignalAckMessage::SIGNAL_NOT_FOUND;
}

void WebUI::handleStateChange(const QString& label,
                              const QVariant& value)
{
   handleStateResponse(label, value, true);
   emit acknowledgeStateChange(Utils::StateChangedAckMessage::SUCCEEDED,
                               value);
}

void WebUI::handleSignalAck(Utils::SignalAckMessage::Result result,
                            quint32 ackId)
{
   Q_UNUSED(result)
   Q_UNUSED(ackId)
}

void WebUI::handleSetStateAck(Utils::SetStateAckMessage::Result result,
                              quint32 ackId)
{
   Q_UNUSED(result)
   Q_UNUSED(ackId)
}

void WebUI::connected(qint32 connectionId)
{
   if (!m_users.keys().contains(connectionId)) {
      m_users.insert(connectionId, new User);
   }
}

void WebUI::disconnected(qint32 connectionId)
{
   if (m_users.keys().contains(connectionId)) {
      delete m_users.value(connectionId);
      m_users.remove(connectionId);
   }
}

void WebUI::dataReceived(QByteArray data,
                         qint32 connectionId)
{
   if (!m_users.keys().contains(connectionId)) {
      qCritical("Unknown connectionId: %i", connectionId);
      return;
   }

   User* user = m_users.value(connectionId);
   if (!user) {
      return;
   }

   user->buffer += data;
   while (!user->buffer.isEmpty()) {
      // Find start of message.
      int start = user->buffer.indexOf("$");
      if (start == -1) {
         user->buffer.clear();
         return;
      }

      // Find end of message.
      int end = user->buffer.indexOf("\n", start);
      if (end == -1) {
         return;
      }

      // Get message payload (data between start and end characters).
      QByteArray message = user->buffer.mid(start + 1, end - start - 1);
      user->buffer = user->buffer.mid(end + 1);

      // Separate command and data.
      int commandIndex = message.indexOf(":");
      if (commandIndex == -1) {
         sendErrorMessage("Invalid message", connectionId);
         return;
      }

      QString command = message.mid(0, commandIndex);
      QString data = message.mid(commandIndex + 1);

      // Parse data.
      if (command == "reg") {
         parseReg(data, user);
      } else if (command == "get") {
         parseGet(data, user);
      } else if (command == "set") {
         parseSet(data);
      } else if (command == "err") {
         parseErr(data);
      } else {
         sendErrorMessage("Invalid message", connectionId);
         qCritical("Invalid message: %s", data.toLatin1().data());
      }
   }
}

void WebUI::parseReg(QString data,
                     User* user)
{
   user->registeredStates = data.split(";").toSet();
   user->requestedStates.unite(user->registeredStates);
   requestStates(user->registeredStates);
}

void WebUI::parseGet(QString data,
                     User* user)
{
   user->requestedStates.unite(data.split(";").toSet());
   requestStates(user->requestedStates);
}

void WebUI::parseSet(QString data)
{
   QSet<QString> states = data.split(";").toSet();

   foreach (QString state, states) {
      QStringList parts = state.split("=");
      QString name = parts.first().trimmed();
      QString rawValue = parts.last().trimmed().toLower();

      QVariant value = rawValue;
      if (rawValue == "true" || rawValue == "false") {
         value = QVariant(parts.last().trimmed()).toBool();
      }

      emit setStateValue(name, value, true);
   }
}

void WebUI::parseErr(QString data)
{
   sendLog(data, Utils::LogMessage::WARNING);
}

void WebUI::requestStates(QSet<QString> states)
{
   foreach (QString state, states) {
      emit requestStateValue(state);
   }
}

void WebUI::sendErrorMessage(QString message,
                             int connectionId)
{
   QString data = QString("$err:%1\n").arg(message);
   sendData(QByteArray(data.toLatin1().data()), connectionId);
}

} // Plugins
