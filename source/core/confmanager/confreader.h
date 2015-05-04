#include <QXmlDefaultHandler>
#include <QHash>
#include <QStack>

namespace Core {

/*!
 * \brief The ConfReader class
 * This class takes care of reading configuration from xml file.
 */
class ConfReader : public QXmlDefaultHandler
{
public:
   /*!
    * \brief Constructor.
    * \param featureName Feature name to read
    * \param parameters Reference to hash that is filled with parameters.
    */
   ConfReader(QString featureName,
              QHash<QString, QString>& parameters);

   bool startElement(const QString& namespaceURI,
                     const QString& localName,
                     const QString& qName,
                     const QXmlAttributes& attributes);

   bool endElement(const QString& namespaceURI,
                   const QString& localName,
                   const QString& qName);

   bool fatalError(const QXmlParseException& exception);
   QString errorString() const;
   QString getErrorMsg() const;

private:
   static const QString ELEMENT_FEATURES;
   static const QString ELEMENT_FEATURE;
   static const QString ELEMENT_GROUP;
   static const QString ELEMENT_PARAMETER;
   static const QString ATTRIBUTE_FEATURE_NAME;
   static const QString ATTRIBUTE_GROUP_NAME;
   static const QString ATTRIBUTE_PARAMETER_NAME;
   static const QString ATTRIBUTE_PARAMETER_VALUE;

   bool m_parsingFeatures;

   QStack<QString> m_groupStack;
   QString m_currentFeatureName;

   QString m_featureName;
   QHash<QString, QString>& m_parameters;

   QString m_errorMsg;
   QString m_errorStr;
};

} // Core
