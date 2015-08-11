/* uiplugin.hh
 * 
 * This header defines the UiPlugin plugin interface for custom user interfaces.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 11-Aug-2015
 */

#ifndef UIPLUGIN_HH
#define UIPLUGIN_HH

//! SignalHandler::UiPlugin unique identifier.
#define SIGNALHANDLER_UIPLUGIN_IID "auxilo2.signalhandler.UiPlugin"

#include <QtPlugin>
#include <QCoreApplication>
#include "viewinterface.hh"

namespace SignalHandler 
{

/*!
 * \brief The UiPlugin class
 *  The plugin interface for custom user interfaces.
 */
class UiPlugin
{
public:
    
    //! Constructor
    virtual UiPlugin() = default;
    
    //! Destructor
    virtual ~UiPlugin() = default;
    
    /*!
     * \brief Creates a suitable instance of QCoreApplication.
     * \param argc Program argument count
     * \param argv Program argument vector
     * \return Suitable instance of QCoreApplication. The caller takes the 
     *  ownership of application object.
     * \pre argc > 0, argv != nullptr. Call this method only once in the program
     *  lifetime.
     * \post Appropriate version of QCoreApplication is created.
     */
    virtual QCoreApplication* createApp(int argc, char* argv[]) = 0;
    
    /*!
     * \brief Create instance of ViewInterface.
     * \param verbose Are debug and warning messages displayed or not?
     * \return New instance of ViewInterface. Ownership is passed to the caller.
     * \pre Appropriate instace of QCoreApplication has been created (createApp
     *  has been called).
     * \post ViewInterface instance is created.
     */
    virtual ViewInterface* createView(bool verbose = false) = 0;
};

} // Namespace SignalHandler

Q_DECLARE_INTERFACE (SignalHandler::UiPlugin, SIGNALHANDLER_UIPLUGIN_IID)


#endif // UIPLUGIN_HH

