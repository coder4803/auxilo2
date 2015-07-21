#include <typeinfo>

#include <QHash>
#include <QVariant>
#include <QException>

#include "message.h"

#ifndef UTILS_CONFRESPONSEMESSAGE_H
#define UTILS_CONFRESPONSEMESSAGE_H

namespace Utils {

/*!
 * \brief The ParameterSet class.
 * This class describes set of configuration parameters.
 */
class ParameterSet
{
public:
   /*!
    * \brief Constructor.
    */
   ParameterSet();

   /*!
    * \brief Constructor.
    * \param featureName Feature name of parameters.
    */
   ParameterSet(QString featureName);

   /*!
    * \brief Sets parameter feature name.
    * \param featureName Parameter feature name.
    */
   void setFeatureName(QString featureName) { m_featureName = featureName; }

   /*!
    * \brief Appends new parameter.
    * \param parameter Name of parameter.
    * \param value Value of parameter.
    */
   bool appendParameter(QString parameter,
                        QString value);

   /*!
    * \brief Appends hash of parameters.
    * Ignores existing parameters.
    * \param parameters Hash of parameters.
    */
   void appendParameters(QHash<QString, QString> parameters);

   /*!
    * \brief Returns feature name of parameter set.
    * \return Feature name of parameter set.
    */
   QString featureName() const { return m_featureName; }

   /*!
    * \brief Returns ParameterSet that contains only parameters that
    * start with given string.
    * \param startsWith String to search from start of parameters.
    * \param returnShortPath If true, startsWith string is removed from
    * the beginning of each parameter name.
    * \return ParameterSet that contains only parameters starting with
    * startsWith string.
    */
   ParameterSet getSection(QString startsWith,
                           bool returnShortPath = true) const;

   /*!
    * \brief Checks if parameter set contains certain parameter.
    * \param name Name of parameter.
    * \return True if set contains the parameter, otherwise false.
    */
   bool contains(QString name) const;

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
    * \brief Returns list of parameter names that start with given string.
    * \param startsWith String to search from start of parameters.
    * \param returnShortPath If true, parameters are returned without
    * startsWith string.
    * \return List of parameter names.
    */
   QStringList parameterList(QString startsWith = QString(),
                             bool returnShortPath = true) const;

   /*!
    * \brief Returns hash of parameters.
    * \return Hash of parameters.
    */
   const QHash<QString, QString>& parameters() const { return m_parameters; }

private:
   QString m_featureName;
   QHash<QString, QString> m_parameters;
};

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
   ConfResponseMessage(const ParameterSet& parameterSet);

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
    * \brief Returns result of configuration request.
    * \return Result of configuration request.
    */
   Result result() const { return m_result; }

   /*!
    * \brief Returns set of parameters in response message.
    * \return Parameters in ParameterSet.
    */
   const ParameterSet& parameteSet() { return m_parameterSet; }

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
   ParameterSet m_parameterSet;
   Result m_result;
};

template <class T>
T ParameterSet::parameter(QString name,
                          T defaultValue) const
{
   name = name.toLower();
   if (!m_parameters.contains(name)) {
      return defaultValue;
   }

   return parameter<T>(name);
}

template <class T>
T ParameterSet::parameter(QString name) const
{
   if (!m_parameters.contains(name)) {
      qCritical("Missing mandatory parameter: %s", name.toLatin1().data());
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
   if (!value.canConvert<T>() || !value.convert((int)QVariant(T()).type())) {
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
