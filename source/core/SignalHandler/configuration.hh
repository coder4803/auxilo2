/* configuration.hh
 * This header contains constants used in SignalHandler configuration.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 26-July-2015
 */

#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <QString>

namespace SignalHandler 
{

namespace Conf 
{
    //! Feature name in coreconfig-file.
    const QString SIGNALHANDLER_FEATURE_NAME ("signalHandler");

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
    
    //! Name of signal's name property (mandatory)
    const QString SIGNAL_NAME ("name");
    
    //! Name of signal script's language property (mandatory)
    const QString SIGNAL_LANG ("language");
    
    //! Name of signal's file path property (mandatory).
    const QString SIGNAL_PATH ("script");
    
    //! Name of signal's priority property 
    //! (optional, defaults to DEFAULT PRIORITY).
    const QString SIGNAL_PRIORITY ("priority");
    
    
    //! Name of signal's read-to-memory -property (optional, defaults to true).
    const QString SIGNAL_TO_MEM ("readToMemory");
    
    //! DEFAULT PRIORITY is used, if priority has not been defined.
    const unsigned DEFAULT_PRIORITY (2);
    
    //! Signalhandler plugin library path.
    const QString PLUGIN_PATH ("../lib/signalhandler/");

    
} // Namespace SignalHandler::Conf
} // Namespace SignalHandler


#endif // CONFIGURATION_HH

