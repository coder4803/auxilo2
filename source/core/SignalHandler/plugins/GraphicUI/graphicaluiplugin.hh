/* graphicaluiplugin.hh
 * 
 * This header defines a graphical SignalHandler::UiPlugin implementation.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 12-Aug-2015
 */

#ifndef GRAPHICALUIPLUGIN_HH
#define GRAPHICALUIPLUGIN_HH

#include "../../UserInterface/uiplugin.hh"
#include <QObject>

/*!
 * \brief The GraphicalUiPlugin class
 *  This class implements the SignalHandler::UiPlugin plugin interface.
 *  This plugin offers a graphical version of SignalHandler::ViewInterface.
 */
class GraphicalUiPlugin : public QObject, public SignalHandler::UiPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SIGNALHANDLER_UIPLUGIN_IID)
    Q_INTERFACES(SignalHandler::UiPlugin)
    
public:
    
    /*!
     * \brief Constructor
     * \param parent QObject's parent.
     */
    explicit GraphicalUiPlugin(QObject* parent = 0);
    
    //! Destructor.
    virtual ~GraphicalUiPlugin();
    
    // Implements the SignalHandler::UiPlugin interface
    virtual QCoreApplication* createApp(int& argc, char* argv[]);
    virtual SignalHandler::ViewInterface* createView(bool verbose);
};

#endif // GRAPHICALUIPLUGIN_HH
