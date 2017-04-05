#include <QXmlDefaultHandler>
#include <QHash>

#include "states/state.h"

namespace Core {

/*!
 * \brief The StateLoader class.
 * This class takes care of loading state holder configurations.
 */
class StateLoader : public QXmlDefaultHandler
{
public:
   /*!
    * \brief Constructor.
    * \param states Reference to state container.
    */
   StateLoader(QHash<QString, State*>& states);

   //! Destructor.
   ~StateLoader();

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
    * \brief Catches text between element tags for further handling.
    * \param ch Text between element tags.
    * \return Returns always true.
    */
   bool characters(const QString& ch);

   /*!
    * \brief Handles fatal errors.
    * \param exception Parse exception.
    * \return Returns always false.
    */
   bool fatalError(const QXmlParseException& exception);

   /*!
    * \brief Returns error string.
    * \return Error string.
    */
   QString errorString() const;

   /*!
    * \brief Returns error message. Error message includes error string.
    * \return Error message.
    */
   QString getErrorMsg() const;

private:
   //! Default value for state update interval.
   static const quint32 DEFAULT_UPDATE_INTERVAL;

   //! Constant for stateholder element name.
   static const QString ELEMENT_STATE;

   //! Constant for state element name.
   static const QString ELEMENT_STATEHOLDER;

   //! Constant for signal element name.
   static const QString ELEMENT_SIGNAL;

   //! Constant for device element name.
   static const QString ELEMENT_DEVICE;

   //! Boolean indicator if we are reading configurations.
   bool m_readingConfiguration;

   //! State under reading.
   State* m_state;

   //! String between element tags.
   QString m_data;

   //! Device update interval holder (how often device is tried to inform about
   //! changed state until succeeded).
   quint32 m_deviceUpdateInterval;

   //! Reference to state container.
   QHash<QString, State*>& m_states;

   //! Error message.
   QString m_errorMsg;

   //! Error string.
   QString m_errorStr;

   /*!
    * \brief Reads mandatory attribute value.
    * \param attributes Reference to all attributes.
    * \param name Name of attribute to read.
    * \return Value of attribute.
    */
   template <class T>
   T readMandatoryAttribute(const QXmlAttributes& attributes,
                           const QString& name);

   /*!
    * \brief Reads optional attribute value.
    * Returns default value if attribute doesn't exist.
    * \param attributes Reference to all attributes.
    * \param name Name of attribute to read.
    * \return Value of attribute.
    */
   template <class T>
   T readOptionalAttribute(const QXmlAttributes& attributes,
                           const QString& name,
                           const T& defaultValue);

   /*!
    * \brief Creates new instance of state.
    * \param type State type.
    * \param name State name.
    * \param persisted Should state value be persisted or not.
    * \return Pointer to state.
    */
   State* createState(QString type,
                      QString name,
                      bool persisted);
};

} // Core
