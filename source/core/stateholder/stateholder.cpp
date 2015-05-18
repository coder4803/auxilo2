#include <QXmlSimpleReader>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QCoreApplication>

#include "messagegroup.h"
#include "confrequestmessage.h"
#include "confresponsemessage.h"
#include "setstatemessage.h"
#include "setstateackmessage.h"
#include "requeststatemessage.h"
#include "stateresponsemessage.h"
#include "statechangedackmessage.h"

#include "states/state.h"
#include "stateloader.h"
#include "stateholder.h"

namespace Core {

const QString StateHolder::PARAMETER_CONFPATH("confPath");
const QString StateHolder::DEFAULT_CONF_FILE("../parameters/stateconfig.xml");

const quint32 StateHolder::STATE_CHECK_INTERVAL(1000); // ms

const quint32 StateHolder::CONF_REQUEST_INTERVAL(2000); // ms
const QString StateHolder::CONF_RESPONSE_GROUP("stateHolderConf");
const QString StateHolder::FEATURE_NAME("stateHolder");

const QString StateHolder::DATABASE_CONNECTION_NAME("stateholder");

StateHolder::StateHolder(QString serverAddress,
                         QObject* parent) :
   QObject(parent),
   m_configPath(DEFAULT_CONF_FILE),
   m_confResponseGroup(NULL),
   m_setStateGroup(NULL),
   m_stateRequestGroup(NULL),
   m_stateChangedAckGroup(NULL)
{
   Utils::Connection::setHost(serverAddress);

   // Initialize confResponse message group
   m_confResponseGroup = new Utils::MessageGroup(CONF_RESPONSE_GROUP,
                                                Utils::MessageGroup::Subscriber,
                                                this);
   connect(m_confResponseGroup, SIGNAL(messageReceived(QByteArray, QString)),
           this, SLOT(handleConfResponseMessage(QByteArray)));

   // Request parameters when response group is ready
   connect(m_confResponseGroup, SIGNAL(ready()),
           this, SLOT(requestParameters()));
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

void StateHolder::handleConfResponseMessage(QByteArray payload)
{
   Utils::ConfResponseMessage response(payload);

   // Get state configuration file path.
   const Utils::ParameterSet& parameterSet = response.parameteSet();

   try {
      m_configPath = parameterSet.parameter(PARAMETER_CONFPATH,
                                            DEFAULT_CONF_FILE);
   } catch (QException& e) {
      qCritical("Invalid configuration path, quitting...");
      QCoreApplication::quit();
      return;
   }

   qCritical("Configuration file: %s", m_configPath.toLatin1().data());

   // Stop requesting parameters
   m_confRequestTimer.stop();

   // Start serving
   if(!initStateHolder()) {
      qCritical("Initialization failed, quitting program.");
      emit QCoreApplication::exit(EXIT_FAILURE);
   }
}

void StateHolder::handleSetStateMessage(QByteArray payload)
{
   Utils::SetStateMessage message(payload);

   // If state exists
   if (m_states.contains(message.name())) {
      // Set state value
      Utils::SetStateAckMessage::Result result = Utils::SetStateAckMessage::FAILED;
      if (m_states.value(message.name())->setState(message.value())) {
         result = Utils::SetStateAckMessage::SUCCEEDED;
      }

      // Send ack message if required
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

   // Handle each state.
   foreach (QString name, request.states()) {
      bool available = false;
      QVariant value = QVariant();
      bool devicesAreUpToData = false;

      // If state exists.
      if (m_states.contains(name)) {
         available = true;
         value = m_states.value(name)->getState();
         devicesAreUpToData = m_states.value(name)->isUpToDate();
      }

      // Create and send state response.
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

void StateHolder::requestParameters()
{
   // Request parameters from confmanager
   Utils::ConfRequestMessage request(CONF_RESPONSE_GROUP, FEATURE_NAME, true);
   Utils::MessageGroup::publish(request, Utils::CONF_REQUEST_GROUP);

   // Request parameters until received
   if (!m_confRequestTimer.isActive()) {
      connect(&m_confRequestTimer, SIGNAL(timeout()),
              this, SLOT(requestParameters()));
      m_confRequestTimer.start(CONF_REQUEST_INTERVAL);
   }

   qCritical("Waiting for parameters...");
}

void StateHolder::updateStates()
{
   foreach (State* state, m_states) {
      state->update();
   }
}

bool StateHolder::loadStateConfigurations()
{
   qCritical("Loading state configurations...");

   QFile file(m_configPath);
   if (!file.exists()) {
      qCritical("Couldn't find %s.", m_configPath.toLatin1().data());
      return false;
   }

   // Parse configuratino file.
   QXmlInputSource source(&file);
   QXmlSimpleReader xmlReader;
   StateLoader stateLoader(m_states);

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

bool StateHolder::initStateHolder()
{
   if (!openPersistentDatabase()) {
      return false;
   }

   if (!loadStateConfigurations()) {
      return false;
   }

   // Initialize setState message group
   m_setStateGroup = new Utils::MessageGroup(Utils::SET_STATE_GROUP,
                                             Utils::MessageGroup::Subscriber,
                                             this);
   connect(m_setStateGroup, SIGNAL(messageReceived(QByteArray, QString)),
           this, SLOT(handleSetStateMessage(QByteArray)));

   // Initialize stateRequest message group
   m_stateRequestGroup = new Utils::MessageGroup(Utils::REQUEST_STATE_GROUP,
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

   return true;
}

} // Core
