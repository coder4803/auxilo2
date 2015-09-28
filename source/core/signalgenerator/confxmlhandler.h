#ifndef CONFXMLHANDLER_H
#define CONFXMLHANDLER_H

#include <QXmlDefaultHandler>
#include <QDateTime>
#include <QString>
#include <QList>

namespace SignalGenerator
{

struct EventEntity
{
    QString signal;
    QDateTime timestamp;
    QString interval;
    unsigned int repeat;
};


class ConfXmlHandler : public QXmlDefaultHandler
{
public:

    ConfXmlHandler(QList<EventEntity>& result);

    virtual ~ConfXmlHandler();

    virtual bool startElement(const QString &namespaceURI,
                              const QString &localName,
                              const QString &qName,
                              const QXmlAttributes &atts);

    virtual bool endElement(const QString &namespaceURI,
                            const QString &localName,
                            const QString &qName);

    virtual bool fatalError(const QXmlParseException &exception);

    virtual QString errorString() const;


private:

    bool parseEvent(const QXmlAttributes &atts);
    static bool isValidInterval(const QString& interval);

    QList<EventEntity>& result_;
    QString errorString_;
    bool parsingStarted_;
    bool readingEntity_;
    bool endFlag_;
};

} // Namespace SignalGenerator

#endif // CONFXMLHANDLER_H
