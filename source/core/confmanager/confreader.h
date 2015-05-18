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

   /*!
    * \brief Handles element start tags.
    * \param namespaceURI Unused.
    * \param localName Unused.
    * \param qName Element name.
    * \param attributes Element attributes.
    * \return True on success, false on failure.
    */
   bool startElement(const QString& namespaceURI,
                     const QString& localName,
                     const QString& qName,
                     const QXmlAttributes& attributes);

   /*!
    * \brief Handles element end tags.
    * \param namespaceURI Unused.
    * \param localName Unused.
    * \param qName Element name.
    * \return True on success, false on failure.
    */
   bool endElement(const QString& namespaceURI,
                   const QString& localName,
                   const QString& qName);

   /*!
    * \brief Handles fatal errors.
    * Prints line, column and other information about the error.
    * \param exception Parse exception.
    * \return Returns always false.
    */
   bool fatalError(const QXmlParseException& exception);

   /*!
    * \brief Handles fatal errors.
    * \param exception Parse exception.
    * \return Returns always false.
    */
   QString errorString() const;

   /*!
    * \brief Returns error message. Error message includes error string.
    * \return Error message.
    */
   QString getErrorMsg() const;

private:
   //! Features element name.
   static const QString ELEMENT_FEATURES;

   //! Feature element name.
   static const QString ELEMENT_FEATURE;

   //! Parameter element name.
   static const QString ELEMENT_PARAMETER;

   //! Feature element's name attribute name.
   static const QString ATTRIBUTE_FEATURE_NAME;

   //! Parameter element's name attribute name.
   static const QString ATTRIBUTE_PARAMETER_NAME;

   //! Parameter element's value attribute name.
   static const QString ATTRIBUTE_PARAMETER_VALUE;

   //! Boolean indicator id we are inside features element.
   bool m_parsingFeatures;

   //! Group stack container. Group stack string is inserted to the beginning
   //! of each parameter.
   QStack<QString> m_groupStack;

   //! Feature name under reading.
   QString m_currentFeatureName;

   //! Feature name to read parameters from.
   QString m_featureName;

   //! Reference to parameter container.
   QHash<QString, QString>& m_parameters;

   //! Error message.
   QString m_errorMsg;

   //! Error string.
   QString m_errorStr;
};

} // Core
