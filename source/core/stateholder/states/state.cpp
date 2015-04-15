#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QException>

#include "statechangedmessage.h"

#include "state.h"

namespace Core {

const QString State::STATE_CHANGED_ACK_GROUP("statechangedack");
const QString State::DATABASE_NAME("states");

State::State(QString name,
             QVariant defaultValue,
             bool persisted,
             QObject* parent) :
   QObject(parent),
   m_name(name),
   m_value(defaultValue),
   m_persisted(persisted)
{
   if (m_persisted) {
      if (!readPersistedValue()) {
         throw QException();
      }
   }
}

State::~State()
{
   foreach (Device* device, m_devices) {
      delete device;
   }
   m_devices.clear();
}

void State::addDevice(const QString name,
                      const QString label,
                      quint32 updateInterval)
{
   Device* device = new Device(label, QVariant(), updateInterval);
   m_devices.insert(name, device);
}

bool State::setOption(const QString name,
                      const QString value)
{
   Q_UNUSED(value);

   qCritical("Unknown option: %s", name.toLatin1().data());

   return false;
}

bool State::verifyOptions()
{
   return true;
}

void State::update(bool force)
{
   if (isUpToDate() && !force) {
      return;
   }

   foreach (Device* device, m_devices) {
      if (device->value == m_value) {
         continue;
      }

      ++device->intervalCounter;
      if (device->intervalCounter >= device->updateInterval || force) {
         QString deviceGroup = "d_" + m_devices.key(device);
         Utils::StateChangedMessage message(device->label, m_value,
                                            STATE_CHANGED_ACK_GROUP);

         Utils::MessageGroup::publish(message, deviceGroup);
         device->intervalCounter = 0;
         device->waitingForAckId = message.ackId();
      }
   }
}

bool State::isUpToDate() const
{
   foreach (Device* device, m_devices) {
      if (device->value != m_value) {
         return false;
      }
   }

   return true;
}

bool State::isWaitingForAck(quint32 ackId)
{
   if (isUpToDate() || ackId == 0) {
      return false;
   }

   foreach (Device* device, m_devices) {
      if (device->waitingForAckId == ackId) {
         device->value = m_value;
         device->waitingForAckId = 0;
         return true;
      }
   }

   return false;
}

void State::setDefaultValue(QVariant value)
{
   if (!m_persisted) {
      m_value = value;
   }
}

void State::setValue(QVariant value)
{
   if (m_value == value) {
      return;
   }

   // Update value immediately to all devices
   m_value = value;
   update(true);

   // Persisted value is written to database if it changes
   if (m_persisted) {
      QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);

      // Check if value exists in database
      QSqlQuery query(db);
      query.prepare("SELECT id FROM states WHERE name = :name;");
      query.bindValue(":name", m_name);
      if (!query.exec()) {
         qCritical("Failed to perform SELECT query: %s.",
                   query.lastError().text().toLatin1().data());
         return;
      }

      // isSelect() must be called to get first() work
      if (!query.isSelect()) {
         return;
      }

      // If state exists in database
      if (query.first()) {
         // Update value to database
         bool ok = false;
         quint32 id = query.value("id").toUInt(&ok);
         if (!ok) {
            qCritical("Failed to convert id to integer.");
            return;
         }

         query.prepare("UPDATE states SET value = :value WHERE id = :id;");
         query.bindValue(":value", value);
         query.bindValue(":id", id);

         if (!query.exec()) {
            qCritical("Failed to update value to database.");
         }
      } else {
         // State doesn't exist in database so new row has to be added
         query.prepare("INSERT INTO states (name, value) "
                       "VALUES (:name, :value);");
         query.bindValue(":name", m_name);
         query.bindValue(":value", value);
         if (!query.exec()) {
            qCritical("Failed to write value to database.");
         }
      }
   }
}

bool State::readPersistedValue()
{
   QSqlDatabase db = QSqlDatabase::database(DATABASE_NAME);

   // Read value from database
   QSqlQuery query(db);
   query.prepare("SELECT value FROM states WHERE name = :name;");
   query.bindValue(":name", m_name);
   if (!query.exec()) {
      qCritical("Failed to load value from database: %s.",
                query.lastError().text().toLatin1().data());
      return false;
   }

   // If value exists in database
   query.isSelect();
   if (query.isSelect() && query.first()) {
      m_value = query.value("value");
   }

   return true;
}

} // Core
