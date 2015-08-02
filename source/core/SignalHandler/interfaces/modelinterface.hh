
/* viewinterface.hh
 * 
 * This header defines the ModelInterface interface - UI callable interface
 * for the business logic.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 06-April-2015
 * Last modified: 06-April-2015
 */

#ifndef MODELINTERFACE_HH
#define MODELINTERFACE_HH


namespace SignalHandler 
{

/*!
 * \brief The ModelInterface class
 *  Abstract interface for SignalHandler business logic to the UI.
 */
class ModelInterface
{
public:
    
    //! Constructor
    ModelInterface() = default;
    
    //! Destructor
    virtual ~ModelInterface() = default;
    
    //! Copy-constructor and -assignment operator are forbidden.
    ModelInterface(const ModelInterface&) = delete;
    ModelInterface& operator=(const ModelInterface&) = delete;
    
    /*!
     * \brief start Tells the model to start/continue working.
     * \pre-
     * \post Model starts working.
     */
    virtual void start() = 0;
    
    /*!
     * \brief stop Tells model to stop working.
     * \pre -
     * \post Model stops working at a proper point. Call start() to continue.
     */
    virtual void stop() = 0;
};

}

#endif // MODELINTERFACE_HH
