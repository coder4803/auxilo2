#include <QStringList>

#include "message.h"

#ifndef UTILS_SIGNALMESSAGE_H
#define UTILS_SIGNALMESSAGE_H

namespace Utils {

/*!
 * \brief The SignalMessage class
 * This class can be used to create and parse Signal messages.
 */
class SignalMessage : public Message
{
public:
   /*!
    * \brief Constructor to create message from scratch.
    * \param signalName Signal's name.
    * \param parameters List of parameters.
    * \param parent Parent object.
    */
   SignalMessage(QString signalName = QString(),
                 QStringList parameters = QStringList(),
                 QObject* parent = NULL);

   /*!
    * \brief Constructor to create message from binary data.
    * \param data Binary data.
    * \param parent Parent object.
    */
   SignalMessage(QByteArray data,
                 QObject* parent = NULL);

   //! Destructor.
   ~SignalMessage();

   /*!
    * \brief Set signal name.
    * \param name Signal's name.
    */
   void setSignalName(QString name);

   /*!
    * \brief Append parameter to parameter list.
    * \param value Parameter's value.
    */
   void appendParameter(QString value);

   /*!
    * \brief Returns signal name.
    * \return Signal name.
    */
   QString signalName() const;

   /*!
    * \brief Return number of parameters.
    * \return Number of parameters.
    */
   quint32 numberOfParameters() const;

   /*!
    * \brief Return value of specified parameter.
    * \param index Parameter index on list.
    * \return Parameter value.
    * \pre index is less than number of parameters.
    */
   QString parameter(quint8 index) const;

   /*!
    * \brief Return list of parameter values.
    * \return List of parameter values.
    */
   QStringList parameters() const;

   /*!
    * \brief Returns message in binary format.
    * \return Message in binary format.
    */
   QByteArray data() const;

   /*!
    * \brief Returns message in string format.
    * This is usefull when debugging.
    * \return Message in string format.
    */
   QString string() const;

private:
   QString m_signalName;
   QStringList m_parameters;
};

} // Utils

#endif
