#ifndef CONFIGURATIONREADER_HH
#define CONFIGURATIONREADER_HH

#include <QObject>
#include <QTimer>
#include <QString>
#include <mutex>
#include "messagegroup.h"
#include "confresponsemessage.h"

namespace SignalHandler
{

class ConfigurationReader : public QObject
{
    Q_OBJECT
    
public:

    /*!
     * \brief Constructor
     * \param parent QObject's parent.
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
     */
    void start(const QString& group_name = Utils::CONF_REQUEST_GROUP);
    
    /*!
     * \brief Get latest configuration.
     * \return ParameterSet that holds SignalHandler configuration.
     */
    Utils::ParameterSet getConfiguration() const;
    
    
Q_SIGNALS:
    
    //! Notify on configuration update.
    void configurationUpdated();
    
    
private Q_SLOTS:
    
    void onConfMessageReceived(QByteArray data);
    
    void onResponseGroupReady();
    
    
private:
    
    Utils::ParameterSet conf_data_;
    Utils::MessageGroup* responseGroup_;
    mutable std::mutex mx_;
    QTimer retry_timer_;
    
    static const QString RESPONSE_GROUP_NAME_;
    static const QString FEATURE_NAME_;
    static const unsigned RETRY_INTERVAL_;
};

} // namespace SignalHandler

#endif // CONFIGURATIONREADER_HH
