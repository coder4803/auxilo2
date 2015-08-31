/* objectpool.hh
 * 
 * This header defines the ObjectPool class template.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 06-Aug-2015
 */

#ifndef OBJECTPOOL_HH
#define OBJECTPOOL_HH

#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>

namespace PPUtils
{

/*!
 * \brief The ObjectPool class template is a generic object pool.
 * 
 * Type arguments:
 * 
 * T:
 * Stored object type. There is no restrictions for this type.
 * 
 * Builder:
 * A callable class that is responsiple for constructing T-objects when
 * neccessary. Builder must have at liest one function operator that takes 
 * arbitary number of arguments and returns pointer to a dynamically allocated
 * T-object ('...' matches argument types given to reserve-method):
 * T* operator()(...)
 * 
 * Key:
 * Type that is used to segmentate stored objects. If such segmantation is not
 * needed, use PPUtils::UniformObjectPool instead. Key must supported by 
 * std::hash.
 * 
 * Selector:
 * A callable class that figures out object segment (Key) out of released object
 * or builder arguments given to reserve-method. Therefore it must have at liest
 * two function operators:
 * Key operator()(const T*)
 * Key operator()(...) ['...' matches argumet types given to the reserve-method]
 * 
 * Note: This class is not thread safe. Use instanses of this class from one
 * thread only. If concurrent access is needed, use ConcurrentObjectPool instead. 
 */
template <class T, class Builder, class Key, class Selector>
class ObjectPool
{
public:
    
    /*!
     * \brief Constructor. Constructs ObjectPool that uses b and s as builder 
     *  and selector.
     * \param b Builder. ObjectPool takes the ownership.
     * \param s Selector. ObjectPool takes the ownership.
     * \pre b != nullptr, s != nullptr.
     * \post Empty ObjectPool is created. Its Builder is b and Selector is s.
     */
    ObjectPool(Builder* b, Selector* s);
    
    /*!
     * \brief Constructor Constructs ObjectPool with copy of p and s.
     *  (Builder and Selector must be copy-constructible. If default arguments 
     *  are used, Builder and Selector must be default-constructible.)
     * \param b Builder.
     * \param s Selector.
     * \pre None.
     * \post Empty ObjectPool is created. Its Builder is copy of b and Selector
     *  copy of s.
     */
    explicit ObjectPool(const Builder& b = Builder(),
                        const Selector& s = Selector() );
    
    /*!
     * \brief Destructor.
     */
    ~ObjectPool();
    
    /*!
     * \brief Copy-constructor is forbidden.
     */
    ObjectPool(const ObjectPool&) = delete;
    
    /*!
     * \brief Move-constructor.
     * \param other Moved ObjectPool.
     * \pre None
     * \post New objectpool is created and it has the same state as \p other
     *  used to have. \p other is left into an unspecified state.
     */
    ObjectPool(ObjectPool&& other) noexcept;
    
    /*!
     * \brief Copy-assignment operator is forbidden.
     */
    ObjectPool& operator = (const ObjectPool&) = delete;
    
    /*!
     * \brief Move-assignment operator.
     * \param other Moved ObjectPool.
     * \return reference to this object.
     * \pre None.
     * \post this object has the same state as other used to have. Other is left
     *  into an unspecified state. 
     */
    ObjectPool& operator = (ObjectPool&& other) noexcept;
    
    /*!
     * \brief Exclusively reserve a stored object.
     * \param builder_args Arguments that must match one function operator id 
     *  Builder class and one function operator in Selector class.
     * \return Unique pointer to T-object. This may be same object as stored
     *  earlier with release-method or created with Builder.
     * \pre None.
     * \post If there was an available object of desired type (determined by
     *  Selector and args) the existing object is returned. Else Builder creates
     *  new object using args, and this object is returned.
     */
    template <class... Args>
    typename std::unique_ptr<T> reserve(Args... builder_args);
    
    /*!
     * \brief Release an object to be reused.
     * \param object Released object.
     * \pre object != nullptr.
     * \post Object is stored in its current state. Make sure that released 
     *  object is in a valid re-usable state. (or alternatively make all objects 
     *  valid after reserving them.)
     */
    void release(typename std::unique_ptr<T>&& object);
    
    /*!
     * \brief Return total number of stored objects.
     */
    unsigned size() const;
    
    /*!
     * \brief Return number of stored objects matching key.
     * \param key Segment key.
     * \pre None.
     */
    unsigned size(Key key) const;
    
    /*!
     * \brief Destroy all currently stored objects.
     * \pre None.
     * \post ObjecPool is empty.
     */
    void clear();
    
    /*!
     * \brief Return constant pointer to the used Builder object.
     * \pre None.
     */
    const Builder* getBuilder() const;
    
    /*!
     * \brief Return constant pointer ro the used Selector object.
     * \pre None.
     */
    const Selector* getSelector() const;
    
    
private:
    
    typedef typename std::unique_ptr<T> T_uptr;
    typedef typename std::vector<T_uptr> ObjectV;
    std::unordered_map<Key, ObjectV> objects_;
    std::unique_ptr<Builder> builder_;
    std::unique_ptr<Selector> selector_;
    unsigned total_size_;
};

} // Namespace PPUtils

// Include implementation
#include "objectpool_impl.hh"

#endif // OBJECTPOOL_HH

