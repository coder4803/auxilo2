
/* viewinterface.hh
 * 
 * This header defines the ViewInterface interface - base for different
 * user interfaces.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 06-April-2015
 * Last modified: 06-April-2015
 */

#ifndef VIEWINTERFACE_HH
#define VIEWINTERFACE_HH

#include <QString>

namespace SignalHandler 
{

/*!
 * \brief The ViewInterface class
 *  Abstract interface for the user interface.
 */
class ViewInterface
{
public:
    
    //! Constructor
    ViewInterface() = default;
    
    //! Destructor
    virtual ~ViewInterface() = default;
    
    //! Copy- and move-consturctors and -assignment operators are forbidden.
    ViewInterface(const ViewInterface&) = delete;
    ViewInterface(ViewInterface&&) = delete;
    ViewInterface& operator=(const ViewInterface&) = delete;
    ViewInterface& operator=(ViewInterface&&) = delete;
    
    /*!
     * \brief Displays given message on the UI.
     * \param msg Message to be shown.
     * \pre None
     * \post Message is displayed no matter if 'verbose' -application parameter
     *  is set or not.
     *  Concurrency: This method is thread safe.
     */
    virtual void critical(const QString& msg) = 0;
    
    /*!
     * \brief Display debug message on the UI.
     * \param msg Message to be shown.
     * \pre None.
     * \post Message is shown only if 'verbose' application parameter is set.
     */
    virtual void debug(const QString& msg) = 0;
    
    /*!
     * \brief Display warning message.
     * \param msg Message.
     * \pre None.
     * \post Message is shown only if 'verbose' application parameter is set.
     */
    virtual void warning(const QString& msg) = 0;
    
    /*!
     * \brief Display a fatal error message and abort program.
     * \param msg Fatal error message.
     * \pre None.
     * \post Fatal error message is displayed and program is aborted.
     */
    virtual void fatal(const QString& msg) = 0;
};

}


#endif // VIEWINTERFACE_HH
