#include <QException>

#include "confreader.h"

namespace Core {

const QString ConfReader::ELEMENT_FEATURES("features");
const QString ConfReader::ELEMENT_FEATURE("feature");
const QString ConfReader::ELEMENT_PARAMETER("parameter");
const QString ConfReader::ATTRIBUTE_FEATURE_NAME("name");
const QString ConfReader::ATTRIBUTE_PARAMETER_NAME("name");
const QString ConfReader::ATTRIBUTE_PARAMETER_VALUE("value");

ConfReader::ConfReader(QString featureName,
                       QHash<QString, QString>& parameters) :
   m_parsingFeatures(false),
   m_featureName(featureName),
   m_parameters(parameters)
{
   m_parameters.clear();
}

bool ConfReader::startElement(const QString& namespaceURI,
                               const QString& localName,
                               const QString& qName,
                               const QXmlAttributes& attributes)
{
   Q_UNUSED(namespaceURI);
   Q_UNUSED(localName);

   // Everything has to be declared inside features element
   if (qName.toLower() == ELEMENT_FEATURES) {
      m_parsingFeatures = true;
      return true;
   }

   if (!m_parsingFeatures) {
      m_errorStr = "All elements must be defined inside features element.";
      return false;
   }

   if (qName.toLower() == ELEMENT_FEATURE) {
      if (attributes.index(ATTRIBUTE_FEATURE_NAME) == -1) {
         m_errorStr = "Missing attritube 'name'.";
         return false;
      }

      m_currentFeatureName = attributes.value(ATTRIBUTE_FEATURE_NAME);
      return true;
   }

   if (m_currentFeatureName.isEmpty()) {
      m_errorStr = "features element may contain only feature elements";
      return false;
   }

   if (m_currentFeatureName != m_featureName) {
      return true;
   }

   if (qName.toLower() == ELEMENT_PARAMETER) {
      if (attributes.index(ATTRIBUTE_PARAMETER_NAME) == -1) {
         m_errorStr = "Missing attritube 'name'.";
         return false;
      }

      QString name = attributes.value(ATTRIBUTE_PARAMETER_NAME);

      if (attributes.index(ATTRIBUTE_PARAMETER_VALUE) == -1) {
         m_errorStr = "Missing attritube 'value'.";
         return false;
      }

      QString value = attributes.value(ATTRIBUTE_PARAMETER_VALUE);

      QString parameterName;
      for (int i = 0; i < m_groupStack.size(); ++i) {
         parameterName += m_groupStack.at(i) + ".";
      }
      parameterName += name;

      m_parameters.insert(parameterName, value);

      return true;
   }

   // Elemen is group. Add to group stack.
   m_groupStack.push(qName);

   return true;
}

bool ConfReader::endElement(const QString& namespaceURI,
                             const QString& localName,
                             const QString& qName)
{
   Q_UNUSED(namespaceURI);
   Q_UNUSED(localName);

   if (qName.toLower() == ELEMENT_FEATURES) {
      m_parsingFeatures = false;
      return true;
   }

   if (!m_parsingFeatures) {
      m_errorStr = QString("Unknown end tag outside features element: %1.")
                           .arg(qName);
      return false;
   }

   if (qName.toLower() == ELEMENT_FEATURE) {
      m_currentFeatureName.clear();
      return true;
   }

   if (m_currentFeatureName.isEmpty()) {
      m_errorStr = QString("Unknown end tag outside feature element: %1.")
                           .arg(qName);
      return false;
   }

   if (m_currentFeatureName != m_featureName) {
      return true;
   }

   if (qName.toLower() == ELEMENT_PARAMETER) {
      return true;
   }

   // Elemen is group. Remove from stack.
   m_groupStack.pop();
   return true;
}

bool ConfReader::fatalError(const QXmlParseException& exception)
{
   m_errorMsg = QString("Error while loading parameters: line %1, column %2 (%3)")
                        .arg(exception.lineNumber())
                        .arg(exception.columnNumber())
                        .arg(m_errorStr);
   qCritical(m_errorMsg.toLatin1().data());
   return false;
}

QString ConfReader::errorString() const
{
   return m_errorStr;
}

QString ConfReader::getErrorMsg() const
{
   return m_errorMsg;
}

} // Core
