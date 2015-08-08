/* signalhandlerbuilder.hh
 * 
 * This header defines the builder class for SignalHandler model.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 */

#ifndef SIGNALHANDLERBUILDER_HH
#define SIGNALHANDLERBUILDER_HH

#include "interfaces/modelinterface.hh"
#include "message.h"
#include <QObject>
#include <mutex>
#include <condition_variable>
#include <deque>


namespace SignalHandler
{

/*!
 * \brief The SignalHandlerBuilder class
 *  This is the builder class for the SignalHandler ModelInterface implemetation.
 */
class SignalHandlerBuilder : public QObject
{
    Q_OBJECT
    
public:
    
    /*!
     * \brief Constructor
     */
    SignalHandlerBuilder();
    
    /*!
     * \brief Destructor
     */
    ~SignalHandlerBuilder();
    
    /*!
     * \brief Creates the SignalHandler model.
     * \return New instance of model implementation. Ownership is passed to the
     *  caller.
     */
    ModelInterface* create(const QString& serverAddress);
};

} // Namespace SignalHandler

#endif // SIGNALHANDLERBUILDER_HH
