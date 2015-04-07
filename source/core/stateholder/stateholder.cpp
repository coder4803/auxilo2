#include <QXmlSimpleReader>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QException>

#include "messagegroup.h"
#include "setstatemessage.h"
#include "setstateackmessage.h"
#include "requeststatemessage.h"
#include "stateresponsemessage.h"
#include "statechangedackmessage.h"

#include "states/state.h"
#include "stateloader.h"
#include "stateholder.h"

namespace Core {

const quint32 StateHolder::STATE_CHECK_INTERVAL(1000); // ms
const QString StateHolder::SET_STATE_GROUP("setState");
const QString StateHolder::REQUEST_STATE_GROUP("requestState");

const QString StateHolder::DATABASE_CONNECTION_NAME("stateholder");

StateHolder::StateHolder(QObject* parent) :
   QObject(parent),
   m_configPath("../parameters/stateconfig.xml"),
   m_setStateGroup(NULL),
   m_stateRequestGroup(NULL),
   m_stateChangedAckGroup(NULL)
{
   if (!openPersistentDatabase()) {
      throw QException();
   }

   if (!loadStateConfigurations()) {
      throw QException();
   }

   Utils::Connection::setHost("127.0.0.1");

   // Initialize setState message group
   m_setStateGroup = new Utils::MessageGroup(SET_STATE_GROUP,
                                            Utils::MessageGroup::Subscriber,
                                            this);
   connect(m_setStateGroup, SIGNAL(messageReceived(QByteArray, QString)),
           this, SLOT(handleSetStateMessage(QByteArray)));

   // Initialize stateRequest message group
   m_stateRequestGroup = new Utils::MessageGroup(REQUEST_STATE_GROUP,
                                                Utils::MessageGroup::Subscriber,
                                                this);
   connect(m_stateRequestGroup, SIGNAL(messageReceived(QByteArray, QString)),
           this, SLOT(handleRequestStateMessage(QByteArray)));

   // Initialize stateChangedAck message group
   m_stateChangedAckGroup = new Utils::MessageGroup(State::STATE_CHANGED_ACK_GROUP,
                                                   Utils::MessageGroup::Subscriber,
                                                   this);
   connect(m_stateChangedAckGroup, SIGNAL(messageReceived(QByteArray, QString)),
           this, SLOT(handleStateChangedAckMessage(QByteArray)));

   // Initialize timer that triggers state updates
   connect(&m_updateTimer, SIGNAL(timeout()),
           this, SLOT(updateStates()));
   m_updateTimer.start(STATE_CHECK_INTERVAL);

   qCritical("\nStateholder is running...");
}

StateHolder::~StateHolder()
{
   QSqlDatabase db = QSqlDatabase::database(State::DATABASE_NAME);
   db.close();

   foreach (State* state, m_states) {
      delete state;
   }

   m_states.clear();
}

void StateHolder::handleSetStateMessage(QByteArray payload)
{
   qDebug("handling set state message");

   Utils::SetStateMessage message(payload);

   if (m_states.contains(message.name())) {
      Utils::SetStateAckMessage::Result result = Utils::SetStateAckMessage::FAILED;
      if (m_states.value(message.name())->setState(message.value())) {
         result = Utils::SetStateAckMessage::SUCCEEDED;
      }

      if (!message.ackGroup().isEmpty()) {
         Utils::MessageGroup::publish(message.createAckMessage(result),
                                     message.ackGroup());
      }
   }
}

void StateHolder::handleRequestStateMessage(QByteArray payload)
{
   Utils::RequestStateMessage request(payload);
   Utils::StateResponseMessage response;

   foreach (QString name, request.states()) {
      bool available = false;
      QVariant value = QVariant();
      bool devicesAreUpToData = false;

      if (m_states.contains(name)) {
         available = true;
         value = m_states.value(name)->getState();
         devicesAreUpToData = m_states.value(name)->isUpToDate();
      }

      Utils::StateResponseMessage::State state(available, value,
                                               devicesAreUpToData);
      response.appendState(name, state);
   }

   Utils::MessageGroup::publish(response, request.responseGroup());
}

void StateHolder::handleStateChangedAckMessage(QByteArray payload)
{
   // Todo: Performance can be improved by having quick access table
   // (QHash that links ackId to device).

   Utils::StateChangedAckMessage message(payload);

   foreach (State* state, m_states) {
      if (state->isWaitingForAck(message.ackId())) {
         return;
      }
   }
}

void StateHolder::updateStates()
{
   foreach (State* state, m_states) {
      state->update();
   }
}

bool StateHolder::loadStateConfigurations()
{
   QFile file(m_configPath);
   if (!file.exists()) {
      qCritical("Couldn't find %s.", m_configPath.toLatin1().data());
      return false;
   }

   QXmlInputSource source(&file);
   QXmlSimpleReader xmlReader;
   StateLoader stateLoader(m_states, m_configPath);

   xmlReader.setContentHandler(&stateLoader);
   xmlReader.setErrorHandler(&stateLoader);
   if (!xmlReader.parse(source)) {
      qCritical("Error while reading state configurations: %s",
                stateLoader.getErrorMsg().toLatin1().data());
      return false;
   }

   qCritical("... OK.");

   return true;
}

bool StateHolder::openPersistentDatabase()
{
   qCritical("Opening persistent database...");

   // Create and open database connection
   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName(State::DATABASE_NAME);

   if (!db.open()) {
      qCritical("Failed to open persistent database (%s): %s",
                State::DATABASE_NAME.toLatin1().data(),
                db.lastError().text().toLatin1().data());
      return false;
   }

   // Create persistent table if it doesn't exist
   QSqlQuery query("CREATE TABLE IF NOT EXISTS states ("
                   "id     INTEGER  PRIMARY KEY NOT NULL,"
                   "name   TEXT                 NOT NULL,"
                   "value  BLOB                 NOT NULL"
                   ");", db);

   if (!query.exec()) {
      qCritical("Failed to create value table: %s.",
                db.lastError().text().toLatin1().data());
      return false;
   }

   qCritical("... OK.");

   return true;
}

} // Core
