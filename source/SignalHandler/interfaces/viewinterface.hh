
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

#include <string>

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
     * \brief showMessage Displays given message on the UI.
     * \param msg Message to be shown.
     * \pre -
     * \post Message is displayed.
     *  Concurrency: This method is thread safe.
     */
    virtual void showMessage(const std::string& msg) = 0;
};

}


#endif // VIEWINTERFACE_HH
