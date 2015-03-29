
/* signalqueue.hh
 * 
 * This header file defines a thread safe priority queue for Signals.
 * 
 * Author: Perttu Paarlahti (perttu.paarlahti@gmail.com)
 * Created: 10-Mar-2015
 * Last modified: 10-Mar-2015
 */

#ifndef SIGNALQUEUE_HH
#define SIGNALQUEUE_HH

#include "signal.hh"
#include <list>
#include <mutex>
#include <condition_variable>


namespace SignalHandler
{

/*!
 * \brief The SignalQueue class
 * A thread-safe priority queue for pending signals.
 */
class SignalQueue
{
public:
    
    /*!
     * \brief SignalQueue Constructor
     * \pre -
     * \post Constructed SignalQueue object is empty.
     */
    SignalQueue();
    
    /*!
     * \brief ~SignalQueue Destructor.
     */
    ~SignalQueue();
    
    //! Copy-construction is forbidden.
    SignalQueue(const SignalQueue&) = delete;
    
    //! Copy-assignment is forbidden.
    SignalQueue& operator=(const SignalQueue&) = delete;
    
    /*!
     * \brief pop removes and returns signal with the highest priority.
     * If there are no signals in queue, the calling thread is put into sleep
     * until there are signals to be popped.
     * \return Signal with highest priority
     * 
     * Complexity: Constant, if there were signals in queue.
     * Concurrency: This method is thread safe.
     */
    Signal pop();
    
    /*!
     * \brief push Adds a new signal into the queue
     * \param s Signal to be added.
     * \pre -
     * \post Signal is added into the queue. Signal's place is determined by its
     *  priority.
     * 
     * Complexity: logarithmic.
     * Concurrency: This method is thread safe.
     */
    void push(const Signal& s);
    
    /*!
     * \brief empty Checks if the queue is empty.
     * \return True, if there are no pending signals in the queue.
     * 
     * Complexity: Constant.
     * Concurrency: This method is thread safe.
     */
    bool empty() const;
    
private:
    std::list<Signal> queue_;
    mutable std::recursive_mutex mx_;
    std::condition_variable_any not_empty_cv_;
};


} // Namespace SignalHandler

#endif // SIGNALQUEUE_HH
