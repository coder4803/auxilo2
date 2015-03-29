#include "signalqueue.hh"


namespace SignalHandler 
{


SignalQueue::SignalQueue(): queue_(), mx_(), not_empty_cv_()
{
}


Signal SignalQueue::pop()
{
    std::unique_lock<std::recursive_mutex> lock(mx_);
    
    // Wait until there are signals to be popped.
    not_empty_cv_.wait(lock, [this](){return !this->empty();} );
    
    Signal s = queue_.top();
    queue_.pop();
    lock.unlock();
    return s;
}


void SignalQueue::push(const Signal& s)
{
    std::unique_lock<std::recursive_mutex> lock(mx_);
    queue_.push(s);
    lock.unlock();
    // Notify one consumer.
    not_empty_cv_.notify_one();
}


bool SignalQueue::empty() const
{
    std::lock_guard<std::recursive_mutex> lock(mx_);
    return queue_.empty();
}


SignalQueue::~SignalQueue()
{    
}


}

