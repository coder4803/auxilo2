/* confreader.h
 * This header defines the SignalGenerator::ConfReader class that is
 * responsiple for requesting and receiving configuration.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 28-Sep-2015
 */

#ifndef CONFREADER_H
#define CONFREADER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QTimer>

#include <mutex>

#include "messagegroup.h"
#include "confxmlhandler.h"
#include "evententity.h"

namespace SignalGenerator
{

/*!
 * \brief The ConfReader class
 * Class that is responsiple for sending ConfRequestMessages and
 * receiving and processing ConfResponseMessages.
 */
class ConfReader : public QObject
{
    Q_OBJECT

public:

    /*!
     * \brief Constructor
     * \param parent QObject's parent.
     */
    explicit ConfReader(QObject *parent = 0);

    /*!
     * \brief Destructor.
     */
    virtual ~ConfReader();

    /*!
     * \brief Prepare response group and start sending configuration requests.
     * \pre Call this method only once.
     * \post ConfReader sends requests every 2 seconds until a valid response
     *  is received.
     */
    void start();

    /*!
     * \brief Restart sending requests.
     * \pre start has been called before.
     * \post ConfReader sends requests every 2 seconds until a valid response
     *  is received.
     */
    void restart();

    /*!
     * \brief Stop sending requests.
     * \pre None.
     * \post ConfReader no longer sends requests.
     */
    void stop();

    /*!
     * \brief Check if ConfReader is actively sending requests.
     * \return True, if it is.
     */
    bool isStarted() const;

    /*!
     * \brief Get latest valid configuration.
     * \return All static EventEntities in a list.
     */
    QList<EventEntity> getConfiguration() const;


signals:

    /*!
     * \brief Notify that a valid ConfResponseMessage has been received and
     *  processed.
     */
    void configurationReady();


private slots:

    /*!
     * \brief process received ConfResponseMessage.
     * \param data ConfResponseMessage in binary form.
     */
    void onConfResponseReceived(QByteArray data);

    /*!
     * \brief Action on retry timer timeout.
     */
    void onTimeout();

    /*!
     * \brief Start sending requests when response group is ready.
     */
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
