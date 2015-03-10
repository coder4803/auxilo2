
/* signal.hh
 * This header file declares the Signal class that represents incoming messages
 * in the form that SignalHandler understands them.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 04-Mar-2015
 * Last Modified: 04-Mar-2015
 */


#ifndef SIGNAL_HH
#define SIGNAL_HH

#include <vector>
#include <QString>


namespace SignalHandler
{

/*!
 * \brief The Signal class
 * Holds incoming message's task priority, identification for associated script
 * and arguments to be passed to that script.
 */
class Signal
{
public:
    
    /*!
     * \brief Signal Constructor
     * 
     * \param priority Task priority
     * \param scriptID Associated script ID.
     * \param parameters Arguments to be passed to the associated script.
     * 
     * \pre -
     * \post New Signal is constructed with given values.
     */
    Signal(unsigned int priority, 
           unsigned int scriptID, 
           std::vector<QString> parameters);
    
    //! Destructor
    ~Signal();
    
    //! Default copy-constructor
    Signal(const Signal&) = default;
    
    //! Default copy-assignment operator.
    Signal& operator=(const Signal&) = default;
    
    /*!
     * \brief Gets Signal's priority.
     * \return Priority-number.
     */
    unsigned int getPriority() const;
    
    /*!
     * \brief getScriptID Gets associated script ID.
     * \return Associated script's distinct ID-number.
     */
    unsigned int getScriptID() const;
    
    /*!
     * \brief getParameters Gets script arguments as QStrings.
     * \return vector of arguments that will be passed to the associated script.
     */
    std::vector<QString> getParameters() const;
    
    /*!
     * \brief operator < Comparison operator defines relative priority order.
     * This is equivalent to lhs.getPriority() > rhs.getPriority();
     * 
     * \param rhs Righ-hand-side value.
     * \return True, if this has lower priority than rhs. Else false.
     */
    bool operator<(const Signal& rhs) const;
    
    
private:
    unsigned int priority_;
    unsigned int scriptID_;
    std::vector<QString> parameters_;
};


} // Namespace SignalHandler

#endif // SIGNAL_HH
