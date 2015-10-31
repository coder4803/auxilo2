#include <QObject>
#include <QVariant>
#include <QTimer>

#include "messagegroup.h"

#ifndef STATE_H
#define STATE_H

namespace Core {

/*!
 * \brief The State class.
 * This is abstract base class for all state types. It takes care of everything
 * that is common for all state types.
 */
class State : public QObject
{
   Q_OBJECT
public:
   //! Persisted value data base name.
   static const QString DATABASE_NAME;

   /*!
    * \brief Constructor.
    * \param name State name.
    * \param defaultValue State default value.
    * \param persisted Shoud value be persisted or not.
    * \param parent Parent object.
    */
   State(QString name,
         QVariant defaultValue,
         bool persisted,
         QObject* parent = NULL);

   //! Destructor.
   virtual ~State();

   /*!
    * \brief Links device to this state.
    * \param name Device name.
    * \param label Devices label for this state.
    * \param updateInterval How often state is tried to update to device
    * (until succeeded).
    */
   void addDevice(const QString& name,
                  const QString& label,
                  quint32 updateInterval);

   /*!
    * \brief Links signal to the state change.
    * \param name Name of the signal.
    */
   void addSignal(const QString& name);

   /*!
    * \brief Sets an option. Each state has its own options.
    * \param name Name of option.
    * \param value Value of option.
    * \return True if option is valid, otherwise false.
    */
   virtual bool setOption(const QString& name,
                          const QString& value);

   /*!
    * \brief Verifys options.
    * \return True on success, otherwise false.
    */
   virtual bool verifyOptions();

   /*!
    * \brief Updates values to all devices if necessary.
    * \param force If true, value will be updated immediately to those devices
    * that are out-of-date.
    */
   void update(bool force = false);

   /*!
    * \brief Checks if values are up-to-date.
    * \return True if all devices are up-to-date.
    */
   virtual bool isUpToDate() const;

   /*!
    * \brief Checks if state is waiting for specific acknowledge message.
    * \param ackId Id of acknowledge id.
    * \return True if waiting, otherwise false.
    */
   bool isWaitingForAck(quint32 ackId);

   /*!
    * \brief Set state value.
    * \param value State value.
    * \return True on success, false on failure.
    */
   virtual bool setState(const QVariant& value) = 0;

   /*!
    * \brief Returns state value.
    * \return State value.
    */
   virtual QVariant getState() const = 0;

   /*!
    * \brief Returns list of linked lists.
    * \return List of linked lists.
    */
   QStringList getSignals() const { return m_signals; }

   /*!
    * \brief Returns state value.
    * \return State value.
    */
   QString name() const { return m_name; }

   /*!
    * \brief Checks if state is linked to certain device with certain label.
    * \param device Name of device.
    * \param label Name of lable.
    * \return True if state is linked, otherwise false.
    */
   bool containsDeviceWithLabel(QString device,
                                QString label);

protected:
   /*!
    * \brief Sets state's default value.
    * \param value Default value.
    */
   void setDefaultValue(QVariant value);

   /*!
    * \brief Sets state value. This is called by child states.
    * \param value State value.
    */
   void setValue(QVariant value);

   /*!
    * \brief Returns state value. This is called by child states.
    * \return State value.
    */
   QVariant value() const { return m_value; }

private:
   /*!
    * \brief The Device struct.
    * This struct describes one device from state holder point of view.
    */
   struct Device
   {
      /*!
       * \brief Constructor.
       * \param Label Devices name for state.
       * \param Value State value on device.
       * \param UpdateInterval How often value is tried to update
       * (until succeeded).
       */
      Device(QString Label,
             QVariant Value,
             quint16 UpdateInterval = 0) :
         label(Label),
         value(Value),
         updateInterval(UpdateInterval),
         intervalCounter(0),
         waitingForAckId(0)
      {}

      //! Device name for state.
      QString label;

      //! State value on device.
      QVariant value;

      //! How often value is tried to update (until succeeded).
      quint16 updateInterval;

      //! Interval counter (increases one per update interval).
      quint16 intervalCounter;

      //! Acknowledge id that is waited from the remote device.
      quint16 waitingForAckId;
   };

   //! State name.
   QString m_name;

   //! State value.
   QVariant m_value;

   //! Boolean indicator if state is persisted or not.
   bool m_persisted;

   //! Container for linked devices.
   QHash<QString, Device*> m_devices;

   //! Container for linked signals.
   QStringList m_signals;

   /*!
    * \brief Reads persited value from data base.
    * \return True on success, false on failure.
    */
   bool readPersistedValue();
};

} // Core

#endif
