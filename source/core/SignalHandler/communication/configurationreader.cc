/* configurationreader.hh
 * 
 * This is the implementation file for the ConfigurationReader class defined in
 * configurationreader.hh.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#include "configurationreader.hh"
#include "confresponsemessage.h"
#include "confrequestmessage.h"
#include "configuration.hh"
#include <mutex>
#include <QDebug>

namespace SignalHandler
{

const QString ConfigurationReader::RESPONSE_GROUP_NAME_ ("signalHandlerConf");
const QString ConfigurationReader::FEATURE_NAME_ (Conf::SIGNALHANDLER_FEATURE_NAME);
const unsigned ConfigurationReader::RETRY_INTERVAL_ (2000);


ConfigurationReader::ConfigurationReader(QObject* parent) :
    QObject(parent), conf_data_(), responseGroup_(nullptr), mx_(),
    retry_timer_()
{
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
    
    // Wait for configuration received.
    qDebug() << "Waiting for response group to be ready.";
    connect(responseGroup_, SIGNAL(ready()), 
            this, SLOT(onResponseGroupReady()),
            Qt::DirectConnection);
    
    
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
    Utils::ConfResponseMessage msg(data);
    if (msg.result() == Utils::ConfResponseMessage::Result::INVALID_PARAMETERS){
        qDebug() << "Invalid conf parameters";
    }
    else if (msg.result() == Utils::ConfResponseMessage::Result::NO_PARAMETERS){
        qDebug() << "No conf parameters";
    }
    conf_data_ = Utils::ConfResponseMessage(data).parameterSet();
    lock.unlock();
    emit this->configurationUpdated();
}


void ConfigurationReader::onResponseGroupReady()
{
    Utils::ConfRequestMessage req_msg(RESPONSE_GROUP_NAME_,FEATURE_NAME_,true);
    Utils::MessageGroup::publish(req_msg, Utils::CONF_REQUEST_GROUP);
    qDebug() << "ConfRequest sent.";
    if (!retry_timer_.isActive()){
        connect(&retry_timer_, SIGNAL(timeout()),
                this, SLOT(onResponseGroupReady()) );
        retry_timer_.start(RETRY_INTERVAL_);
    }
}


} // Namespace SignalHandler

