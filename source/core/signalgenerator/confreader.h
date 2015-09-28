#ifndef CONFREADER_H
#define CONFREADER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QTimer>

#include <mutex>

#include "messagegroup.h"
#include "confxmlhandler.h"

namespace SignalGenerator
{


class ConfReader : public QObject
{
    Q_OBJECT

public:

    explicit ConfReader(QObject *parent = 0);

    virtual ~ConfReader();

    void start();

    void restart();

    void stop();

    bool isStarted() const;

    QList<EventEntity> getConfiguration() const;


signals:

    void configurationReady();


private slots:

    void onConfResponseReceived(QByteArray data);

    void onTimeout();

    void onResponseGroupReady();


private:

    static const int RETRY_INTERVAL_;
    static const QString RESPONSE_GROUP_NAME_;
    static const QString FEATURE_;

    bool start_flag_;
    QTimer timer_;
    QList<EventEntity> configuration_;
    Utils::MessageGroup* response_group_;
    mutable std::mutex mx_;

};

} // Namespace SignalGenerator

#endif // CONFREADER_H
