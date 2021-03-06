/* confxmlhandler.cpp
 * This is the implementation file for the SignalGenerator::ConfXmlHandler
 * class defined in confxmlhandler.h.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 28-Sep-2015
 */

#include "confxmlhandler.h"
#include <QDebug>

namespace SignalGenerator
{

ConfXmlHandler::ConfXmlHandler(QList<EventEntity> &result):
    QXmlDefaultHandler(), result_(result), errorString_(),
    parsingStarted_(false),
    readingEntity_(false),
    endFlag_(false)
{
}


ConfXmlHandler::~ConfXmlHandler()
{
}


bool ConfXmlHandler::startElement(const QString &namespaceURI,
                                  const QString &localName,
                                  const QString &qName,
                                  const QXmlAttributes &atts)
{
    Q_UNUSED(namespaceURI); Q_UNUSED(localName);

    if (endFlag_){
        errorString_ = "Elements defined outside the main element";
        return false;
    }

    // Event
    else if (qName == "event"){
        if (!parsingStarted_){
            errorString_ = "All 'event'-elements must be defined inside 'signalGenerator'.";
            return false;
        }
        else if (readingEntity_){
            errorString_ = "Nested 'event'-elements.";
            return false;
        }
        else {
            readingEntity_ = true;
            return this->parseEvent(atts);
        }
    }

    // Main element (signalGenerator).
    else if (qName == "signalGenerator"){
        if (parsingStarted_){
            errorString_ = "Nested 'signalGenerator'-elements.";
            return false;
        }
        parsingStarted_ = true;
    }

    // Unknown.
    else{
        errorString_ = QString("Unknown element '") + qName + QString("'.");
        return false;
    }
    return true;
}


bool ConfXmlHandler::endElement(const QString &namespaceURI,
                                const QString &localName,
                                const QString &qName)
{
    Q_UNUSED(namespaceURI); Q_UNUSED(localName);

    if (qName == "event"){
        readingEntity_ = false;
        return true;
    }
    else if (qName == "signalGenerator"){
        endFlag_ = true;
        return true;
    }
    return false;
}


bool ConfXmlHandler::fatalError(const QXmlParseException &exception)
{
    errorString_ = QString("Error at line %1, column%2. (%3)")
            .arg(exception.lineNumber())
            .arg(exception.columnNumber())
            .arg(exception.message());

    return false;
}


QString ConfXmlHandler::errorString() const
{
    return errorString_;
}


bool ConfXmlHandler::parseEvent(const QXmlAttributes &atts)
{
    bool signalDefined(false), timestampDefined(false);
    EventEntity e;

    for (int i=0; i<atts.length(); ++i){
        QString qName = atts.qName(i);

        // Signal
        if (qName == "signal"){
            e.signal = atts.value(i);
            signalDefined = true;
        }

        // Timestamp
        else if (qName == "timestamp"){
            e.timestamp = EventEntity::parseDateTime( atts.value(i) );
            if ( !e.timestamp.isValid() ){
                errorString_ = QString("Invalid timestamp: ") + atts.value(i);
                return false;
            }
            timestampDefined = true;
        }

        // Interval
        else if (qName == "interval"){
            e.interval = atts.value(i);
            if ( !EventEntity::isValidInterval(e.interval) ){
                errorString_ = QString("Invalid interval: ") + e.interval;
                return false;
            }
        }

        // Repeat
        else if (qName == "repeat"){
            bool ok(false);
            e.repeat = atts.value(i).toUInt(&ok);
            if (!ok){
                errorString_ = QString("Invalid repetition: ") + atts.value(i);
                return false;
            }
        }

        // Unknown.
        else{
            errorString_ = QString("Unknown event attribute: ") + qName;
            return false;
        }
    }

    // Check mandatory attributes
    if (!signalDefined){
        errorString_ = "Event missing mandatory attributes (signal).";
        return false;
    }
    else if (!timestampDefined){
        errorString_ = "Event missing mandatory attributes (timestamp).";
        return false;
    }
    else if (e.interval.isEmpty() && e.repeat != 0){
        errorString_ = "No timeinterval defined for repeatable event.";
        return false;
    }
    if (result_.contains(e)){
        errorString_ = "All events must be unique.";
        return false;
    }
    result_.append(e);
    return true;
}

} // Namespace SignalGenerator
