/* eventreceiver.cpp
 *
 * This is the implementation file for the EventReceiver class defined in
 * eventreceiver.h.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 14-Nov-2015
 */

#include "eventreceiver.h"
#include <QDebug>

namespace SignalGenerator
{

EventReceiver::EventReceiver(EventManager *manager, QObject *parent) :
    QObject(parent), manager_(manager), group_(nullptr)
{
    Q_ASSERT(manager_ != nullptr);
}


EventReceiver::~EventReceiver()
{
    this->stop();
}


void EventReceiver::start()
{
    Q_ASSERT( !this->isStarted() );

    group_ = new Utils::MessageGroup(Utils::SIGNAL_GENERATOR_GROUP,
                                     Utils::MessageGroup::Subscriber, this);

    connect(group_, SIGNAL(messageReceived(QByteArray,QString)),
            this, SLOT(onEventMessageReceived(QByteArray)) );

    qDebug() << "Ready to receive dynamic events.";
}


bool EventReceiver::isStarted() const
{
    return group_ != nullptr;
}


void EventReceiver::stop()
{
    if (group_ == nullptr) return;

    disconnect(group_, SIGNAL(messageReceived(QByteArray,QString)),
               this, SLOT(onEventMessageReceived(QByteArray)) );

    delete group_;
    group_ = nullptr;
}


void EventReceiver::onEventMessageReceived(QByteArray data)
{
    qDebug() << "Message received!";
    Utils::EventMessage msg(data);

    EventEntity e;
    e.signal = msg.signalName();
    e.timestamp = msg.timestamp();
    e.interval = msg.interval()==0 ? "" : QString::number(msg.interval())+" second";
    e.repeat = msg.repeat();

    manager_->addDynamicEvent(e);
}

} // Namespace SignalGenerator
