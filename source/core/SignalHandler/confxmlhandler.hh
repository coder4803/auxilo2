/* confxmlhandler.hh
 * This header file defines the SignalHandler::ConfXmlHandler class, that
 * implements the QXmlContentHandler and QXmlErrorHandler interfaces.
 * The class is needed to read signalhandler's own configuration file.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 26-June-2015
 */

#ifndef CONFXMLHANDLER_HH
#define CONFXMLHANDLER_HH

#include <QXmlDefaultHandler>
#include "interfaces/scriptbankinterface.hh"

namespace SignalHandler 
{


/*!
 * \brief The ConfXmlHandler class
 *  This class is responsible for handling SignalHandler's configuration 
 *  xml-file's content and errors.
 */
class ConfXmlHandler : public QXmlDefaultHandler
{
public:
    
    /*!
     * \brief Constructor
     * \param result Script data read from xml file is inserted here. Ownership
     *  is not passed.
     * \pre result != nullptr. Do not destroy result before all reading is done.
     * \post This object is ready to parse Xml-files. Information read from
     *  the file is saved into result.
     */
    ConfXmlHandler(ScriptBankInterface::ScriptData* result);
    
    /*!
     * \brief Destructor.
     */
    virtual ~ConfXmlHandler();
    
    /*!
     * \brief Reimplements QXmlContentHandler::startElement. The Xml-reader
     *  calls this method when it encounters a new element.
     * \param namespaceURI is unused in this class.
     * \param localName is unused in this class.
     * \param qName element's qualifying name.
     * \param atts element's attributes.
     * \return true, if no error occured while reading element.
     */
    virtual bool startElement(const QString &namespaceURI, 
                              const QString &localName,
                              const QString &qName, 
                              const QXmlAttributes &atts);
    
    
    /*!
     * \brief Reimplements QXmlContentHandler::endElement. The xml-reader
     *  calls this method when it encounters end of an element.
     * \param namespaceURI Unused in this class.
     * \param localName Unused in this class.
     * \param qName Ending element's qualifying name.
     * \return True if no error has occured. 
     */
    virtual bool endElement(const QString &namespaceURI, 
                            const QString &localName, 
                            const QString &qName);
    
    /*!
     * \brief Reimplements QXmlErrorHandler::fatalError. The xml-reader
     *  calls this method when it encounters a fatal error.
     * \param exception Exception idicateing occured error.
     * \return Always false.
     */
    virtual bool fatalError(const QXmlParseException &exception);
    
    /*!
     * \brief Reimplements QXmlErrorHandler::errorString.
     * \return Clarifying error message or an empty string, if no error has 
     *  occured.
     */
    virtual QString errorString() const;
    
private:
    
    ScriptBankInterface::ScriptData* result_; // Destination for parsed data.
    QString errorString_;   // Error message from latest error.
    bool parsingStarted_;   // signals-element has started -flag.
    bool readingSignal_;    // signal-element is under processing -flag.
    bool signalsDefined_;   // signals-element has ended -flag.
    
    
    bool readSignal(const QXmlAttributes &atts);
    bool readName(const QXmlAttributes &atts, int i);
};

}

#endif // CONFXMLHANDLER_HH
