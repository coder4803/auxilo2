#include <QException>

#include "stateloader.h"
#include "states/booleanstate.h"
#include "states/integerstate.h"

namespace Core {

const quint32 StateLoader::DEFAULT_UPDATE_INTERVAL = 5;

const QString StateLoader::ELEMENT_STATE("state");
const QString StateLoader::ELEMENT_STATEHOLDER("stateholder");

StateLoader::StateLoader(QHash<QString, State*>& states) :
   m_readingConfiguration(false),
   m_state(NULL),
   m_deviceUpdateInterval(0),
   m_states(states)
{
}

StateLoader::~StateLoader()
{
}

bool StateLoader::startElement(const QString& namespaceURI,
                               const QString& localName,
                               const QString& qName,
                               const QXmlAttributes& attributes)
{
   Q_UNUSED(namespaceURI);
   Q_UNUSED(localName);

   // Everything has to be declared between <stateholder> and </stateholder>
   if (qName.toLower() == ELEMENT_STATEHOLDER) {
      m_readingConfiguration = true;
      m_state = NULL;
      return true;
   }

   if (!m_readingConfiguration) {
      m_errorStr = QString("Unknown start tag outside stateholder element.");
      return false;
   }

   // Parse state start tag
   if (qName.toLower() == ELEMENT_STATE) {
      if (m_state) {
         m_errorStr = QString("state can't be defined inside another state.");
         return false;
      }

      QString type;
      QString name;
      bool persisted = false;
      try {
         type = readMandatoryAttribute<QString>(attributes, "type");
         name = readMandatoryAttribute<QString>(attributes, "name");
         persisted = readOptionalAttribute<bool>(attributes, "persisted", false);
      } catch (QException& e) {
         return false;
      }

      // Check if state name is already in use
      if (m_states.contains(name)) {
         m_errorStr = QString("dublicate state name: %1").arg(name);
         return false;
      }

      // Create new state object based on type. Options are set later.
      m_state = createState(type, name, persisted);
      if (m_state == NULL) {
         m_errorStr = QString("Invalid state type: %1").arg(type);
         return false;
      }
   } else if (m_state) {
      // Here we parse elements between state start and end tags.
      if (qName.toLower() == "device") {
         try {
            QString name = readMandatoryAttribute<QString>(attributes, "name");
            QString label = readOptionalAttribute<QString>(attributes, "label",
                                                           m_state->name());
            quint32 updateInterval = readOptionalAttribute<quint32>(attributes,
                                     "updateInterval", DEFAULT_UPDATE_INTERVAL);

            m_state->addDevice(name, label, updateInterval);
         } catch (QException& e) {
            return false;
         }
      } else if (qName.toLower() == "signal") {
         try {
            QString name = readMandatoryAttribute<QString>(attributes, "name");
            m_state->addSignal(name);
         } catch (QException& e) {
            return false;
         }
      }
   } else {
      m_errorStr = QString("Unrecognized element: %1").arg(qName);
      return false;
   }

   return true;
}

bool StateLoader::endElement(const QString& namespaceURI,
                             const QString& localName,
                             const QString& qName)
{
   Q_UNUSED(namespaceURI);
   Q_UNUSED(localName);

   if (qName.toLower() == ELEMENT_STATEHOLDER) {
      m_readingConfiguration = false;
      return true;
   }

   if (!m_readingConfiguration) {
      m_errorStr = QString("Unknown end tag outside stateholder element.");
      return false;
   }

   // Handle state end element
   if (qName.toLower() == ELEMENT_STATE) {
      if (!m_state->verifyOptions()) {
         m_errorStr = QString("error while verifying options");
         return false;
      }

      m_states.insert(m_state->name(), m_state);
      m_state = NULL;
   } else {
      if (!m_state) {
         // Not inside state element
         m_errorStr = QString("Unknown start tag inside stateholder.");
         return false;
      }

      // Handle device end element
      if (qName.toLower() != "device" &&
          qName.toLower() != "signal")
      {
         if (!m_state->setOption(qName, m_data)) {
            m_errorStr = QString("invalid option: %1 (value: %2)")
                                .arg(qName).arg(m_data);
            return false;
         }
      }
   }

   return true;
}

bool StateLoader::characters(const QString& ch)
{
   m_data = ch;
   return true;
}

bool StateLoader::fatalError(const QXmlParseException& exception)
{
   m_errorMsg = QString("Error while loading states: line %1, column %2 (%3)")
                        .arg(exception.lineNumber())
                        .arg(exception.columnNumber())
                        .arg(m_errorStr);
   qCritical(m_errorMsg.toLatin1().data());
   return false;
}

QString StateLoader::errorString() const
{
   return m_errorStr;
}

QString StateLoader::getErrorMsg() const
{
   return m_errorMsg  + " (" + m_errorStr + ").";
}

template <class T>
T StateLoader::readMandatoryAttribute(const QXmlAttributes& attributes,
                                      const QString& name)
{
   // Check if attribute is missing.
   if (attributes.index(name) == -1) {
      m_errorStr = QString("missing attribute: %1").arg(name);
      throw QException();
   }

   // Check if value is invalid
   QVariant tmp(attributes.value(name));
   if (!tmp.canConvert<T>()) {
      m_errorStr = QString("invalid value value for %1 (%2)")
                          .arg(name).arg(tmp.toString());
      throw QException();
   }

   return tmp.value<T>();
}

template <class T>
T StateLoader::readOptionalAttribute(const QXmlAttributes& attributes,
                                     const QString& name,
                                     const T& defaultValue)
{
   // Check if attribute is missing.
   if (attributes.index(name) == -1) {
      return defaultValue;
   }

   // Check if value is invalid
   QVariant tmp(attributes.value(name));
   if (!tmp.canConvert<T>()) {
      m_errorStr = QString("invalid value value for %1 (%2)")
                          .arg(name).arg(tmp.toString());
      throw QException();
   }

   return tmp.value<T>();
}

State* StateLoader::createState(QString type,
                                QString name,
                                bool persisted)
{
   State* state = NULL;

   try {
      if (type.toLower() == "boolean") {
         state = new BooleanState(name, persisted);
      } else if (type.toLower() == "integer") {
         state = new IntegerState(name, persisted);
      }
   } catch (QException& e) {
      return NULL;
   }

   return state;
}

} // Core
