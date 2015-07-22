#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <QString>

namespace SignalHandler 
{

namespace Conf 
{

    //! Define script name with parameter starting with NAME_PREFIX.
    const QString NAME_PREFIX ("script_name");
    
    //! Define script file path with parameter "<script name>PATH".
    const QString PATH ("_path");
    
    //! Define script language with parameter "<script name>LANG".
    const QString LANG ("_language");
    
    //! Define script priority with parameter "<script name>PRIORITY".
    //! This parameter is optional. In not defined, script has default priority.
    const QString PRIORITY ("_priority");
    
    //! Default priority is used if priority is not defined.
    const unsigned DEFAULT_PRIORITY (1);
    
    //! Define wheather the script is read from file when needed or loaded to 
    //! program memory at configuration. Big or rarely used scripts may be
    //! wiser to read from file when needed. This parameter is optional and
    //! defaults to false.
    const QString FROM_FILE ("read_from_file");
    
    //! Define number of script runners i.e. how many scripts can run 
    //! concurrently. This parameter is optional and defaults to 
    //! SignalHandler::Conf::DEFAULT_WORKERS.
    const QString WORKERS_TAG ("workers");
    
    //! Default number of script runners.
    const unsigned DEFAULT_WORKERS (3);

    
} // Namespace SignalHandler::Conf
} // Namespace SignalHandler


#endif // CONFIGURATION_HH

