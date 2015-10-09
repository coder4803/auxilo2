/* uiplugininterface.h
 * This header defines the interface for signalgenerator's UI-plugins.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 7-Oct-2015
 */

#ifndef UIPLUGININTERFACE_H
#define UIPLUGININTERFACE_H

#include <QtPlugin>
#include <QCoreApplication>
#include "viewinterface.h"

// Use this IID in plugin implementation.
#define SIGNALGENERATOR_UIPLUGIN_IID "auxilo2.signalgenerator.uiplugininterface"

namespace SignalGenerator
{

/*!
 * \brief The UiPluginInterface class
 * The interface for UI-plugins.
 */
class UiPluginInterface
{
public:

    /*!
     * \brief Destructor.
     */
    virtual ~UiPluginInterface() {}

    /*!
     * \brief Create suitable instance of QCoreApplication.
     * \return New instance of QCoreApplication or its subclass.
     * \pre Call this method only once.
     * \post Ownership of the app is passed to the caller.
     */
    virtual QCoreApplication* createApp(int& argc, char* argv[]) = 0;

    /*!
     * \brief Create instance of user interface.
     * \return New instance of user interface. The caller takes the ownership.
     * \pre createApp has been called in advance.
     */
    virtual ViewInterface* createView() = 0;
};

} // Namespace SignalGenerator

Q_DECLARE_INTERFACE(SignalGenerator::UiPluginInterface, SIGNALGENERATOR_UIPLUGIN_IID)

#endif // UIPLUGININTERFACE_H

