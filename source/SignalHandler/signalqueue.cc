#include "signalqueue.hh"
#include <algorithm>


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
    
    Signal s = queue_.front();
    queue_.pop_front();
    lock.unlock();
    return s;
}


void SignalQueue::push(const Signal& s)
{
    mx_.lock();
    
    auto it = std::upper_bound(queue_.begin(), queue_.end(), s,
                               [](Signal s1, Signal s2){return s2 < s1;} );
    queue_.insert(it, s);
    
    mx_.unlock();
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

