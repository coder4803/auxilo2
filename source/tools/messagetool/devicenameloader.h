#ifndef DEVICENAMELOADER_H
#define DEVICENAMELOADER_H

#include <QXmlDefaultHandler>
#include <QStringList>
#include <QSet>

namespace MessageTool {

/*!
 * \brief The DeviceNameLoader class
 * This class takes care of reading device names from deviceconfig.xml
 */
class DeviceNameLoader : public QXmlDefaultHandler
{
public:
   /*!
    * \brief Constructor.
    * \param interfaces Reference to interface list.
    * \param groups Reference to group list.
    */
   explicit DeviceNameLoader(QSet<QString>& interfaces,
                             QSet<QString>& devices);

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
   //! List of core component feature names.
   static const QStringList CORE_APPLICATIONS;

   //! Features element name.
   static const QString ELEMENT_FEATURES;

   //! Feature element name.
   static const QString ELEMENT_FEATURE;

   //! Feature element's name attribute name.
   static const QString ATTRIBUTE_FEATURE_NAME;

   //! Boolean indicator id we are inside features element.
   bool m_parsingFeatures;

   //! Current feature name.
   QString m_currentFeatureName;

   //! Depth of nested element.
   quint16 m_elementDepth;

   //! Reference to interface list
   QSet<QString>& m_interfaces;

   //! Reference to device list.
   QSet<QString>& m_devices;

   //! Error message.
   QString m_errorMsg;

   //! Error string.
   QString m_errorStr;
};

} // MessageTool

#endif
