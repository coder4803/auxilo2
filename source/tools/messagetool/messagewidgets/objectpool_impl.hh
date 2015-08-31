/* objectpool_impl.hh
 * 
 * This is the implementation file for the PPUtils::ObjectPool class template
 * defined in objectpool.hh.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 06-Aug-2015
 */

#ifndef OBJECTPOOL_IMPL_HH
#define OBJECTPOOL_IMPL_HH

#include <cassert>

namespace PPUtils
{

template <class T, class Builder, class Key, class Selector>
ObjectPool<T,Builder,Key,Selector>::ObjectPool(Builder* b, Selector* s) :
    builder_(b), selector_(s), total_size_(0)
{
    assert(b != nullptr);
    assert(s != nullptr);
}


template <class T, class Builder, class Key, class Selector>
ObjectPool<T,Builder,Key,Selector>::
ObjectPool(const Builder& b, const Selector& s) :
    builder_(new Builder(b)), selector_(new Selector(s)), total_size_(0)
{
}


template <class T, class Builder, class Key, class Selector>
ObjectPool<T,Builder,Key,Selector>::~ObjectPool()
{
}


template <class T, class Builder, class Key, class Selector>
PPUtils::ObjectPool<T,Builder,Key,Selector>::
ObjectPool(ObjectPool<T,Builder,Key,Selector>&& other) noexcept
{
    std::swap(this->objects_, other.objects_);
    std::swap(this->builder_, other.builder_);
    std::swap(this->selector_, other.selector_);
    this->total_size_ = other.total_size_;
}


template <class T, class Builder, class Key, class Selector>
ObjectPool<T,Builder,Key,Selector>& 
ObjectPool<T,Builder,Key,Selector>::
operator =(ObjectPool<T,Builder,Key,Selector>&& other) noexcept
{
    if (this != &other)
    {
        std::swap(this->objects_, other.objects_);
        std::swap(this->builder_, other.builder_);
        std::swap(this->selector_, other.selector_);
        this->total_size_ = other.total_size_;
    }
    return *this;
}


template <class T, class Builder, class Key, class Selector>
template <class... Args>
typename std::unique_ptr<T> 
ObjectPool<T,Builder,Key,Selector>::reserve(Args... builder_args)
{
    typename std::unique_ptr<T> rv(nullptr);
    Key k = selector_->operator()(builder_args...);
    auto it = objects_.find(k);
    
    if (it == objects_.end() || it->second.empty()){
       rv.reset(builder_->operator()(builder_args...));
    }
    else {
        rv.swap(it->second.back());
        it->second.pop_back();
        --total_size_;
    }
    return rv;
}


template <class T, class Builder, class Key, class Selector>
void ObjectPool<T,Builder,Key,Selector>::
release(typename std::unique_ptr<T>&& object)
{
    assert(object != nullptr);
    
    Key k = selector_->operator()(object.get());
    auto it = objects_.find(k);
    
    if (it == objects_.end()){
        std::vector<std::unique_ptr<T>> v;
        v.push_back( std::move(object) );
        objects_.insert( std::make_pair(k, ObjectV()) );
        objects_[k] = std::move(v);
    }
    else {
        it->second.push_back( std::move(object) );
    }
    ++total_size_;
}


template <class T, class Builder, class Key, class Selector>
unsigned ObjectPool<T,Builder,Key,Selector>::size() const
{
    return total_size_;
}


template <class T, class Builder, class Key, class Selector>
unsigned ObjectPool<T,Builder,Key,Selector>::size(Key key) const
{
    auto it = objects_.find(key);
    if (it != objects_.end()){
        return it->second.size();
    }
    return 0;
}


template <class T, class Builder, class Key, class Selector>
void ObjectPool<T,Builder,Key,Selector>::clear()
{
    objects_.clear();
    total_size_ = 0;
}


template <class T, class Builder, class Key, class Selector>
const Builder* ObjectPool<T,Builder,Key,Selector>::getBuilder() const
{
    return builder_.get();
}


template <class T, class Builder, class Key, class Selector>
const Selector* ObjectPool<T,Builder,Key,Selector>::getSelector() const
{
    return selector_.get();
}


} // Namespace PPUtils

#endif // OBJECTPOOL_IMPL_HH

