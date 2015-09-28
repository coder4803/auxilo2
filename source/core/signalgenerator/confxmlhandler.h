/* confxmlhandler.h
 * This header defines the SignalGenerator::ConfXmlHandler class that is
 * responsiple for parsing signal generator configuration files.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 28-Sep-2015
 */

#ifndef CONFXMLHANDLER_H
#define CONFXMLHANDLER_H

#include <QXmlDefaultHandler>
#include <QString>
#include <QList>

#include "evententity.h"


namespace SignalGenerator
{




/*!
 * \brief The ConfXmlHandler class
 *  Implements the QXmlContentHandler and the QXmlErrorHandler interfaces
 *  for signal generator configuration files.
 */
class ConfXmlHandler : public QXmlDefaultHandler
{
public:

    /*!
     * \brief Constructor
     * \param result Reference to the target entity list.
     */
    ConfXmlHandler(QList<EventEntity>& result);

    /*!
     * \brief Destructor.
     */
    virtual ~ConfXmlHandler();

    /*!
     * \brief Implements the QXmlContentHandler interface.
     * \param namespaceURI Unused
     * \param localName Unused
     * \param qName Starting element's name.
     * \param atts Element attributes.
     * \return False, if an error was detected.
     */
    virtual bool startElement(const QString &namespaceURI,
                              const QString &localName,
                              const QString &qName,
                              const QXmlAttributes &atts);

    /*!
     * \brief Implements the QXmlContentHandler interface.
     * \param namespaceURI Unused
     * \param localName Unused
     * \param qName Ending element's name.
     * \return False, if an error was detected.
     */
    virtual bool endElement(const QString &namespaceURI,
                            const QString &localName,
                            const QString &qName);

    /*!
     * \brief Implements the QXmlErrorHandler interface.
     * \param exception Error details.
     * \return False.
     */
    virtual bool fatalError(const QXmlParseException &exception);

    /*!
     * \brief Implements the QXmlErrorHandler interface.
     * \return Error message describing latest error.
     */
    virtual QString errorString() const;


private:

    bool parseEvent(const QXmlAttributes &atts);

    QList<EventEntity>& result_;
    QString errorString_;
    bool parsingStarted_;
    bool readingEntity_;
    bool endFlag_;
};

} // Namespace SignalGenerator

#endif // CONFXMLHANDLER_H
