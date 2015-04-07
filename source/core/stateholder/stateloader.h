#include <QXmlDefaultHandler>
#include <QHash>

#include "states/state.h"

namespace Core {

class StateLoader : public QXmlDefaultHandler
{
public:
   StateLoader(QHash<QString, State*>& states,
               QString configPath);
   ~StateLoader();

   bool startElement(const QString& namespaceURI,
                     const QString& localName,
                     const QString& qName,
                     const QXmlAttributes& attributes);

   bool endElement(const QString& namespaceURI,
                   const QString& localName,
                   const QString& qName);

   bool characters(const QString& ch);

   bool fatalError(const QXmlParseException& exception);
   QString errorString() const;
   QString getErrorMsg() const;

private:
   static const quint32 DEFAULT_UPDATE_INTERVAL;

   bool m_readingConfiguration;
   bool m_parsingDevice;
   State* m_state;
   QString m_data;

   QString m_deviceName;
   QString m_deviceLabel;
   quint32 m_deviceUpdateInterval;

   QHash<QString, State*>& m_states;
   QString m_configPath;

   QString m_errorMsg;
   QString m_errorStr;

   template <class T>
   T readMadatoryAttribute(const QXmlAttributes& attributes,
                           const QString& name);

   template <class T>
   T readOptionalAttribute(const QXmlAttributes& attributes,
                           const QString& name,
                           const T& defaultValue);

   State* createState(QString type,
                      QString name,
                      bool persisted);
};

} // Core
