#include <typeinfo>

#include <QHash>
#include <QVariant>
#include <QException>

#include "message.h"

#ifndef UTILS_CONFRESPONSEMESSAGE_H
#define UTILS_CONFRESPONSEMESSAGE_H

namespace Utils {

/*!
 * \brief The ConfResponseMessage class
 * This class can be used to create and parse configuration response messages.
 */
class ConfResponseMessage : public Message
{
public:
   enum Result {
      NO_ERROR,          //!< No errors.
      NO_PARAMETERS,     //!< Erro occured.
      INVALID_PARAMETERS //!< Invalid parameters.
   };

   /*!
    * \brief Constructor.
    */
   ConfResponseMessage();

   /*!
    * \brief Constructor.
    * This constructor is used to create message from received binary data.
    * \param payload Received binary data.
    */
   ConfResponseMessage(const QByteArray& payload);

   /*!
    * \brief Sets result of request.
    * \param result Result of request.
    */
   void setResult(Result result) { m_result = result; }

   /*!
    * \brief Appends new parameter to response.
    * \param parameter Name of parameter.
    * \param value Value of parameter.
    */
   bool appendParameter(QString parameter,
                        QString value);

   /*!
    * \brief Appends hash of parameters.
    * Ignores existing parameters
    * \param parameters Hash of parameters.
    */
   void appendParameters(QHash<QString, QString> parameters);

   /*!
    * \brief Returns result of configuration request.
    * \return Result of configuration request.
    */
   Result result() const { return m_result; }

   /*!
    * \brief Returns list of parameter names that start with startsWith string.
    * \param startsWith String to search from start of parameters.
    * \param returnShortPath If true, parameters are returned without
    * startsWith string.
    * \return List of parameter names.
    */
   QStringList parameterList(QString startsWith = QString(),
                             bool returnShortPath = true) const;

   /*!
    * \brief Checks if list contains certain parameter.
    * \param name Name of parameter.
    * \return True if response contains the parameter, otherwise false.
    */
   bool contains(QString name) const { return m_parameters.contains(name); }

   /*!
    * \brief Returns value of parameter converted to desired type.
    * \param name Name of parameter.
    * \return Value of parameter converted to desired type.
    * \throw QException Parameter doesn't exist or couldn't be converted to
    * desired type.
    */
   template <class T> T parameter(QString name) const;

   /*!
    * \brief Returns value of parameter converted to desired type.
    * If parameter is not found it will return defaultValue.
    * \param name Name of parameter.
    * \return Value of parameter converted to desired type.
    * \throw QException Parameter couldn't be converted to desired type.
    */
   template <class T> T parameter(QString name,
                                  T defaultValue) const;

   /*!
    * \brief Returns message in binary format.
    * \return Message in binary format.
    */
   QByteArray data() const;

   /*!
    * \brief Returns message in string format.
    * This is useful when debugging.
    * \return Message in string format.
    */
   QString string() const;

private:
   QHash<QString, QString> m_parameters;
   Result m_result;
};

template <class T>
T ConfResponseMessage::parameter(QString name,
                                 T defaultValue) const
{
   if (!m_parameters.contains(name)) {
      return defaultValue;
   }

   return parameter<T>(name);
}

template <class T>
T ConfResponseMessage::parameter(QString name) const
{
   if (!m_parameters.contains(name)) {
      throw QException();
   }

   QVariant value = m_parameters.value(name);

   // Handle hexadecimal markings
   if (typeid(T) != typeid(QString)) {
      if (value.toString().startsWith("0x")) {
         bool ok = false;
         int dec = value.toString().toUInt(&ok, 16);
         if (ok) {
            value = QString::number(dec);
         } else {
            qCritical("Warning: value starts with 0x but "
                      "can't be interpret as hex value");
         }
      }
   }

   // Check if value can be converted
   if (!value.canConvert<T>()) {
      qCritical("Value can't be converted to desired type");
      throw QException();
   }

   // Convert value
   T returnValue = value.value<T>();
   qDebug("%s", QString("%1: %2").arg(name)
          .arg(returnValue).toLatin1().data());

   return returnValue;
}

} // Utils

#endif
