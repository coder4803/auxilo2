#include "devicenameloader.h"

namespace MessageTool {

const QString DeviceNameLoader::ELEMENT_FEATURES("features");
const QString DeviceNameLoader::ELEMENT_FEATURE("feature");
const QString DeviceNameLoader::ATTRIBUTE_FEATURE_NAME("name");

const QStringList DeviceNameLoader::CORE_APPLICATIONS =
      QStringList() << "stateholder"
                    << "signalhandler"
                    << "signalgenerator"
                    << "logger";

DeviceNameLoader::DeviceNameLoader(QSet<QString>& interfaces,
                                   QSet<QString>& devices) :
   m_parsingFeatures(false),
   m_elementDepth(0),
   m_interfaces(interfaces),
   m_devices(devices)
{
}

bool DeviceNameLoader::startElement(const QString& namespaceURI,
                                    const QString& localName,
                                    const QString& qName,
                                    const QXmlAttributes& attributes)
{
   Q_UNUSED(namespaceURI);
   Q_UNUSED(localName);

   // Everything has to be declared inside features element.
   if (qName.toLower() == ELEMENT_FEATURES) {
      m_parsingFeatures = true;
      return true;
   }

   // Other elements can exist only inside features-element.
   if (!m_parsingFeatures) {
      m_errorStr = "All elements must be defined inside features element.";
      return false;
   }

   // Parse feature start element
   if (qName.toLower() == ELEMENT_FEATURE) {
      if (attributes.index(ATTRIBUTE_FEATURE_NAME) == -1) {
         m_errorStr = "Missing attritube 'name'.";
         return false;
      }

      m_currentFeatureName = attributes.value(ATTRIBUTE_FEATURE_NAME);
      return true;
   }

   // Other elements can exist only inside feature-element.
   if (m_currentFeatureName.isEmpty()) {
      m_errorStr = "features element may contain only feature elements";
      return false;
   }

   // Element opens, increase depth.
   ++m_elementDepth;

   if (CORE_APPLICATIONS.contains(m_currentFeatureName.toLower())) {
      return true;
   }

   if (m_elementDepth == 1) {
      // Device names are listed into device list.
      m_devices.insert(qName);

      // Interface names are listed into interface list
      m_interfaces.insert(m_currentFeatureName);
   }

   return true;
}

bool DeviceNameLoader::endElement(const QString& namespaceURI,
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

   // Parse feature element.
   if (qName.toLower() == ELEMENT_FEATURE) {
      m_currentFeatureName.clear();
      return true;
   }

   // If not parsing feature element.
   if (m_currentFeatureName.isEmpty()) {
      m_errorStr = QString("Unknown end tag outside feature element: %1.")
                           .arg(qName);
      return false;
   }

   // Element closes, decrease depth.
   --m_elementDepth;

   return true;
}

bool DeviceNameLoader::fatalError(const QXmlParseException& exception)
{
   m_errorMsg = QString("Error while loading device names: "
                        "line %1, column %2 (%3)")
                        .arg(exception.lineNumber())
                        .arg(exception.columnNumber())
                        .arg(m_errorStr);
   qCritical(m_errorMsg.toLatin1().data());
   return false;
}

QString DeviceNameLoader::errorString() const
{
   return m_errorStr;
}

QString DeviceNameLoader::getErrorMsg() const
{
   return m_errorMsg;
}

} // MessageTool
