/* configurationreader.hh
 * 
 * This header defines the ConfigurationReader class that is responsiple for
 * receiving configuration messages and storeing latest configuration.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#ifndef CONFIGURATIONREADER_HH
#define CONFIGURATIONREADER_HH

#include <QObject>
#include <QTimer>
#include <QString>
#include <mutex>
#include <condition_variable>
#include "messagegroup.h"
#include "confresponsemessage.h"

namespace SignalHandler
{

/*!
 * \brief The ConfigurationReader class
 *  This class sends configuration requests, receives response messages and 
 *  stores the latest configuration.
 */
class ConfigurationReader : public QObject
{
    Q_OBJECT
    
public:

    /*!
     * \brief Constructor
     * \param parent QObject's parent.
     * \pre parent is nullptr or a valid QObject-pointer.
     * \post ConfigurationReader is created and holding an empty configuration.
     *  Connects to the messager server if not already connected.
     */
    ConfigurationReader(QObject* parent = nullptr);
    
    //! Destructor.
    virtual ~ConfigurationReader();
    
    //! Copy-constructor is forbidden.
    ConfigurationReader(const ConfigurationReader&) = delete;
    //! Assignment operator is forbidden.
    ConfigurationReader& operator=(const ConfigurationReader&) = delete;
    
    /*!
     * \brief Start sending ConfRequestMessages. 
     * \param group_name Request group.
     * \pre group_name is a non-empty string.
     * \post The response group is created. Starts sending request messages when
     *  the response group is ready. Function won't return until at liest one 
     *  ConfResponceMessage is received.
     */
    void start(const QString& group_name = Utils::CONF_REQUEST_GROUP);
    
    /*!
     * \brief Start requesting configuration again. This could be used, if 
     *  earlier received configuration was invalid.
     */
    void restart();
    
    /*!
     * \brief Get latest configuration.
     * \return ParameterSet that holds SignalHandler configuration.
     * \pre None.
     */
    Utils::ParameterSet getConfiguration() const;
    
    
Q_SIGNALS:
    
    //! Notify on configuration update.
    void configurationUpdated();
    
    
private Q_SLOTS:
    
    /*!
     * \brief Handle incomming ConfResponseMessages.
     * \param data Message's binary representation.
     * \pre data is a valid representation.
     * \post New configuration is set. configurationUpdated()-signal is emitted.
     */
    void onConfMessageReceived(QByteArray data);
    
    /*!
     * \brief Starts sending ConfRequestMessages when the response group is
     *  ready.
     * \pre -
     * \post Requests are sent every second until a response is received.
     */
    void onResponseGroupReady();
    
    
private:
    
    Utils::ParameterSet conf_data_;
    Utils::MessageGroup* responseGroup_;
    mutable std::mutex mx_;
    QTimer retry_timer_;
    
    static const QString FEATURE_NAME_;
    static const unsigned RETRY_INTERVAL_;
};

} // namespace SignalHandler

#endif // CONFIGURATIONREADER_HH
