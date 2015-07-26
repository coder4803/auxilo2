#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <QString>

namespace SignalHandler 
{

namespace Conf 
{
    //! Define configuration file path with parameter "CONF_PATH"
    const QString CONF_PATH ("signalConfigurationFile");

    //! Define script file path with parameter "SCRIPT_PATH".
    const QString SCRIPT_PATH ("scriptFolder");
    
    //! Define number of script runners i.e. how many scripts can run 
    //! concurrently. This parameter is optional and defaults to 
    //! SignalHandler::Conf::DEFAULT_WORKERS.
    const QString WORKERS_TAG ("workers");
    
    //! Default number of script runners.
    const unsigned DEFAULT_WORKERS (5);
    
    const QString SIGNAL_NAME ("name");
    
    const QString SIGNAL_PRIORITY ("priority");
    
    const QString SIGNAL_PATH ("script");
    
    const QString SIGNAL_TO_MEM ("readToMemory");
    
    const QString SIGNAL_LANG ("language");
    
    //! DEFAULT PRIORITY is used, if priority has not been defined.
    const unsigned DEFAULT_PRIORITY (2);

    
} // Namespace SignalHandler::Conf
} // Namespace SignalHandler


#endif // CONFIGURATION_HH

