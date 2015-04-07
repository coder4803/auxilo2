#include <QObject>
#include <QVariant>
#include <QTimer>

#include "messagegroup.h"

#ifndef STATE_H
#define STATE_H

namespace Core {

class State : public QObject
{
   Q_OBJECT
public:
   static const QString STATE_CHANGED_ACK_GROUP;
   static const QString DATABASE_NAME;

   State(QString name,
         QVariant defaultValue,
         bool persisted,
         QObject* parent = NULL);

   virtual ~State();

   void addDevice(const QString name,
                  const QString label,
                  quint32 updateInterval);

   virtual bool setOption(const QString name,
                          const QString value);

   virtual bool verifyOptions();

   virtual void update(bool force = false);
   virtual bool isUpToDate() const;
   bool isWaitingForAck(quint32 ackId);

   virtual bool setState(const QVariant& value) = 0;
   virtual QVariant getState() const = 0;

   QString name() { return m_name; }

protected:
   void setDefaultValue(QVariant value);

   void setValue(QVariant value);
   QVariant value() const { return m_value; }

private:
   struct Device
   {
      Device(QString Label,
             QVariant Value,
             quint16 UpdateInterval = 0) :
         label(Label),
         value(Value),
         updateInterval(UpdateInterval),
         intervalCounter(0),
         waitingForAckId(0)
      {}

      QString label;
      QVariant value;
      quint16 updateInterval;
      quint16 intervalCounter;
      quint16 waitingForAckId;
   };

   QString m_name;
   QVariant m_value;
   bool m_persisted;

   QHash<QString, Device*> m_devices;

   bool readPersistedValue();
};

} // Core

#endif
