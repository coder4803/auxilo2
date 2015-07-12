#include "configurationreader.hh"
#include "confresponsemessage.h"
#include "confrequestmessage.h"
#include <mutex>
#include <QDebug>

namespace SignalHandler
{

const QString ConfigurationReader::RESPONSE_GROUP_NAME_ ("SignalHandlerConf");
const QString ConfigurationReader::FEATURE_NAME_ ("SignalHandler");
const unsigned ConfigurationReader::RETRY_INTERVAL_ (1000);


ConfigurationReader::ConfigurationReader(QObject* parent) :
    QObject(parent), conf_data_(), responseGroup_(nullptr), mx_(),
    retry_timer_()
{
    Utils::Connection::setHost("127.0.0.1");
}


ConfigurationReader::~ConfigurationReader()
{
    if (responseGroup_ != nullptr)
        disconnect(responseGroup_, SIGNAL(messageReceived(QByteArray,QString)),
                   this, SLOT(onConfMessageReceived(QByteArray)) );
}


void ConfigurationReader::start(const QString& group_name)
{
    Q_ASSERT(!group_name.isEmpty());
    
    responseGroup_ = new Utils::MessageGroup(RESPONSE_GROUP_NAME_,
                                             Utils::MessageGroup::Subscriber,
                                             this);
    
    connect(responseGroup_, SIGNAL(ready()), 
            this, SLOT(onResponseGroupReady()) );
    
    connect(responseGroup_, SIGNAL(messageReceived(QByteArray,QString)),
            this, SLOT(onConfMessageReceived(QByteArray)), 
            Qt::DirectConnection);
}


Utils::ParameterSet ConfigurationReader::getConfiguration() const
{
    std::lock_guard<std::mutex> lock(mx_);
    return conf_data_;
}


void ConfigurationReader::onConfMessageReceived(QByteArray data)
{
    if (retry_timer_.isActive())
        retry_timer_.stop();
    
    std::unique_lock<std::mutex> lock(mx_);
    conf_data_ = Utils::ConfResponseMessage(data).parameteSet();
    lock.unlock();
    emit this->configurationUpdated();
}


void ConfigurationReader::onResponseGroupReady()
{
    Utils::ConfRequestMessage req_msg(RESPONSE_GROUP_NAME_,FEATURE_NAME_,true);
    Utils::MessageGroup::publish(req_msg, Utils::CONF_REQUEST_GROUP);
    
    if (!retry_timer_.isActive()){
        connect(&retry_timer_, SIGNAL(timeout()),
                this, SLOT(onResponseGroupReady()) );
        retry_timer_.start(RETRY_INTERVAL_);
    }
}


} // Namespace SignalHandler
