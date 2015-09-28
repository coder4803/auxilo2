#include "confreader.h"
#include "confrequestmessage.h"
#include "confresponsemessage.h"
#include <QXmlReader>


namespace SignalGenerator
{

const int ConfReader::RETRY_INTERVAL_(2000);
const QString ConfReader::RESPONSE_GROUP_NAME_("signalgeneratorConf");
const QString ConfReader::FEATURE_("signalgenerator");


ConfReader::ConfReader(QObject *parent) :
    QObject(parent),
    start_flag_(false), timer_(), configuration_(),
    response_group_(nullptr), mx_()
{
}


ConfReader::~ConfReader()
{
    std::lock_guard<std::mutex> lock(mx_);
    if (response_group_ != nullptr){
        disconnect(response_group_, SIGNAL(messageReceived(QByteArray,QString)),
                   this, SLOT(onConfResponseReceived(QByteArray)) );
        disconnect(response_group_, SIGNAL(ready()), this, SLOT(onTimeout()) );
    }
}


void ConfReader::start()
{
    std::lock_guard<std::mutex> lock(mx_);
    if (start_flag_) return;

    response_group_ = new Utils::MessageGroup(RESPONSE_GROUP_NAME_,
                                              Utils::MessageGroup::Subscriber,
                                              this);

    connect( response_group_, SIGNAL(ready()),
             this, SLOT(onResponseGroupReady()) );

    connect( response_group_, SIGNAL(messageReceived(QByteArray,QString)),
             this, SLOT(onConfResponseReceived(QByteArray)) );

    start_flag_ = true;
}


void ConfReader::restart()
{
    timer_.start();
}


void ConfReader::stop()
{
    std::lock_guard<std::mutex> lock(mx_);
    start_flag_ = false;
    timer_.stop();
}


bool ConfReader::isStarted() const
{
    std::lock_guard<std::mutex> lock(mx_);
    return start_flag_;
}


QList<EventEntity> ConfReader::getConfiguration() const
{
    std::lock_guard<std::mutex> lock(mx_);
    return configuration_;
}


void ConfReader::onConfResponseReceived(QByteArray data)
{
    this->stop();
    Utils::ConfResponseMessage msg(data);
    QString conf_path;

    try{
        conf_path = msg.parameterSet().parameter<QString>("configurationFile");
    }
    catch (QException&){
        qDebug() << "Invalid configuration received";
        this->restart();
        return;
    }

    // Parse the xml-file.
    QFile f(conf_path);
    if (!f.open(QIODevice::ReadOnly)){
        qDebug() << "Configuration file" << conf_path << "did not open.";
        this->restart();
        return;
    }
    QXmlInputSource source(&f);
    QList<EventEntity> result;
    ConfXmlHandler handler(result);
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);

    if ( !reader.parse(&source) ){
        qDebug() << "Error parsing" << conf_path << ":" << handler.errorString();
        this->restart();
    }
    else{
        mx_.lock();
        configuration_.swap(result);
        mx_.unlock();
        emit configurationReady();
    }
}


void ConfReader::onTimeout()
{
    std::lock_guard<std::mutex> lock(mx_);
    if ( this->isStarted() ){
        // Re-send request
        timer_.start();
    }
}


void ConfReader::onResponseGroupReady()
{
    Utils::ConfRequestMessage msg(RESPONSE_GROUP_NAME_, FEATURE_, true);
    Utils::MessageGroup::publish(msg, Utils::CONF_REQUEST_GROUP);
    timer_.setInterval(RETRY_INTERVAL_);
    connect(&timer_, SIGNAL(timeout()), this, SLOT(onTimeout()) );
    timer_.start();
}


} // Namespace SignalGenerator
