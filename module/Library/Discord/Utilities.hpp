#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <dpp/dpp.h>

// ------------------------------------------------------------------------------------------------
#include <memory>

// ------------------------------------------------------------------------------------------------
namespace Sqrat {

// // Allow the VM to treat the dpp::snowflake type as a integer.
// template<> struct Var<dpp::snowflake>
// {
//     dpp::snowflake value;
//     Var(HSQUIRRELVM vm, SQInteger idx) {
//         sq_getinteger(vm, idx, reinterpret_cast<SQInteger*>(&static_cast<uint64_t&>(value)));
//     }
//     inline static void push(HSQUIRRELVM vm, const dpp::snowflake& value) noexcept {
//         sq_pushinteger(vm, static_cast<SQInteger>(static_cast<uint64_t>(value)));
//     }
// };

// // Allow the VM to treat the dpp::snowflake type as a integer.
// template<> struct Var<const dpp::snowflake&>
// {
//     dpp::snowflake value;
//     Var(HSQUIRRELVM vm, SQInteger idx) {
//         sq_getinteger(vm, idx, reinterpret_cast<SQInteger*>(&static_cast<uint64_t&>(value)));
//     }
//     inline static void push(HSQUIRRELVM vm, const dpp::snowflake& value) noexcept {
//         sq_pushinteger(vm, static_cast<SQInteger>(static_cast<uint64_t>(value)));
//     }
// };


// ------------------------------------------------------------------------------------------------
// Used to get and push dpp::snowflake instances to and from the stack
template <> struct Var<dpp::snowflake> {
    dpp::snowflake value; ///< The actual value of get operations
    Var(HSQUIRRELVM vm, SQInteger idx) : value(popAsInt< uint64_t, true >(vm, idx).value) { }
    // Push dpp::snowflake instances to the stack as integers
    static void push(HSQUIRRELVM vm, const dpp::snowflake& value) noexcept {
        sq_pushinteger(vm, static_cast< SQInteger >(static_cast< std::uint64_t >(value)));
    }
    static void push(HSQUIRRELVM vm, dpp::snowflake& value) noexcept {
        sq_pushinteger(vm, static_cast< SQInteger >(static_cast< std::uint64_t & >(value)));
    }
};
template <> struct Var<dpp::snowflake&> : public Var<dpp::snowflake> {
    Var(HSQUIRRELVM vm, SQInteger idx) : Var<dpp::snowflake>(vm, idx) { }
};
template <> struct Var<const dpp::snowflake&> : public Var<dpp::snowflake> {
    Var(HSQUIRRELVM vm, SQInteger idx) : Var<dpp::snowflake>(vm, idx) { }
};

}

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Wrapper around a std::vector of DPP values.
*/
template < class T, class W > struct DpVectorProxy
{
    using Ptr = std::unique_ptr< std::vector< T > >;
    using Vec = std::vector< std::pair< LightObj, W * > >;
    /* --------------------------------------------------------------------------------------------
     * Referenced vector instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Cached script objects vector.
    */
    Vec mVec{};
    /* --------------------------------------------------------------------------------------------
     * Whether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpVectorProxy() noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVectorProxy(typename Ptr::pointer ptr, bool owned = false)
        : mPtr(ptr), mVec(), mOwned(owned)
    { if (mPtr) mVec.resize(mPtr->size()); }
    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit DpVectorProxy(const typename Ptr::element_type & o) noexcept
        : DpVectorProxy(new typename Ptr::element_type(o), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    explicit DpVectorProxy(typename Ptr::element_type && o) noexcept
        : DpVectorProxy(new typename Ptr::element_type(std::forward< Ptr::element_type >(o)), true)
    { }
    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    DpVectorProxy(const DpVectorProxy & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DpVectorProxy(DpVectorProxy && o) noexcept = default;
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DpVectorProxy() noexcept { Cleanup(); }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    DpVectorProxy & operator = (const DpVectorProxy & o) = delete;
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DpVectorProxy & operator = (DpVectorProxy && o) noexcept
    {
        if (this != &o) {
            Cleanup();
            // Transfer members values
            mPtr = std::move(o.mPtr);
            mVec = std::move(o.mVec);
            mOwned = o.mOwned;
        }
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Release any referenced resources and default to an empty/invalid state.
    */
    void Cleanup()
    {
        // Invalidate cached instances
        ClearCache();
        // Do we own this to try delete it?
        if (!mOwned && mPtr) {
            // Not our job, simply forget about it
            [[maybe_unused]] auto p = mPtr.release();
        } else mPtr.reset(); // We own this so delete the instance
    }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle.
    */
    void Validate() const { if (!mPtr) STHROWF("Invalid discord vector handle"); }
    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and retrieve a const reference to it.
    */
    SQMOD_NODISCARD typename Ptr::element_type & Valid() const { Validate(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check whether a valid instance is managed.
    */
    SQMOD_NODISCARD bool IsValid() const { return static_cast< bool >(mPtr); }
    /* --------------------------------------------------------------------------------------------
     * Make sure an index is within range and return the container. Container must exist.
    */
    void ValidIdx_(SQInteger i)
    {
        if (static_cast< size_t >(i) >= Valid().size())
        {
            STHROWF("Invalid vector container index({})", i);
        }
    }
    typename Ptr::element_type & ValidIdx(SQInteger i) { ValidIdx_(i); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Make sure an index is within range and return the container. Container must exist.
    */
    void ValidIdx_(SQInteger i) const
    {
        if (static_cast< size_t >(i) >= Valid().size())
        {
            STHROWF("Invalid vector container index({})", i);
        }
    }
    const typename Ptr::element_type & ValidIdx(SQInteger i) const { ValidIdx_(i); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is referenced and is populated, then return it.
    */
    void ValidPop_()
    {
        if (Valid().empty())
        {
            STHROWF("Vector container is empty");
        }
    }
    typename Ptr::element_type & ValidPop() { ValidPop_(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is referenced and is populated, then return it.
    */
    void ValidPop_() const
    {
        if (Valid().empty())
        {
            STHROWF("Vector container is empty");
        }
    }
    const typename Ptr::element_type & ValidPop() const { ValidPop_(); return *mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Check if a container instance is referenced.
    */
    SQMOD_NODISCARD bool IsNull() const { return !mPtr; }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a value from the container.
    */
    SQMOD_NODISCARD LightObj & Get_(SQInteger i)
    {
        // Is the element cached?
        if (mVec[static_cast< size_t >(i)].first.IsNull())
        {
            mVec[static_cast< size_t >(i)] = Obj(&mPtr->at(static_cast< size_t >(i)));
        }
        // Return the object from the cache
        return mVec[static_cast< size_t >(i)].first;
    }
    SQMOD_NODISCARD LightObj & Get(SQInteger i)
    {
        // Was the referenced vector modified?
        if (mVec.size() < Valid().size())
        {
            // Synchronize the size
            mVec.resize(mPtr->size());
            // Synchronize the cache
            CacheSync();
        }
        // Validate index
        ValidIdx_(i);
        // Perform the request
        return Get_(i);
    }
    /* --------------------------------------------------------------------------------------------
     * Modify a value from the container.
    */
    void Set(SQInteger i, const W & v) { ValidIdx(i)[static_cast< size_t >(i)] = v.Valid(); }
    /* --------------------------------------------------------------------------------------------
     * Check if the container has no elements.
    */
    SQMOD_NODISCARD bool Empty() const { return Valid().empty(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements in the container.
    */
    SQMOD_NODISCARD SQInteger Size() const { return static_cast< SQInteger >(Valid().size()); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements that the container has currently allocated space for.
    */
    SQMOD_NODISCARD SQInteger Capacity() const { return static_cast< SQInteger >(Valid().capacity()); }
    /* --------------------------------------------------------------------------------------------
     * Synchronize cache container instances.
    */
    void CacheSync()
    {
        // Invalidate cached instances, if any
        for (size_t i = 0; i < mVec.size(); ++i)
        {
            // Is this element cached?
            if (mVec[i].second != nullptr)
            {
                // Discard previous instance, if any
                [[maybe_unused]] auto _ = mVec[i].second->mPtr.release();
                // Sync to new instance
                mVec[i].second->mPtr.reset(&mPtr->at(i));
            }
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Increase the capacity of the container to a value that's greater or equal to the one specified.
    */
    DpVectorProxy & Reserve(SQInteger n)
    {
        Valid().reserve(ClampL< SQInteger, size_t >(n));
        mVec.reserve(mPtr->size());
        CacheSync();
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Request the removal of unused capacity.
    */
    void Compact() { Valid().shrink_to_fit(); CacheSync(); mVec.shrink_to_fit(); }
    /* --------------------------------------------------------------------------------------------
     * Erase all elements from the cache container.
    */
    void ClearCache()
    {
        // Invalidate cached instances, if any
        for (auto & e : mVec)
        {
            // Is this element cached?
            if (e.second != nullptr)
            {
                // Invalidate the instance
                e.second->Cleanup();
                // Forget about it
                e.second = nullptr;
                // Release script object
                e.first.Release();
            }
        }
        // Clear the cache vector
        mVec.clear();
    }
    /* --------------------------------------------------------------------------------------------
     * Erase all elements from the container.
    */
    void Clear() { Validate(); ClearCache(); mPtr->clear(); }
    /* --------------------------------------------------------------------------------------------
     * Push a value at the back of the container.
    */
    void Push(const W & v)
    {
        Valid().push_back(v.Valid());
        mVec.emplace_back();
        CacheSync();
    }
    /* --------------------------------------------------------------------------------------------
     * Extends the Container by appending all the items in the given container.
    */
    void Extend(DpVectorProxy & v)
    {
        Valid().insert(Valid().end(), v.Valid().begin(), v.Valid().end());
        mVec.resize(mPtr->size());
        CacheSync();
    }
    /* --------------------------------------------------------------------------------------------
     * Pop the last element in the container.
    */
    void Pop()
    {
        Validate();
        // Is this element cached?
        if (mVec.back().second != nullptr)
        {
            // Invalidate the instance
            mVec.back().second->Cleanup();
            mVec.back().first.Release();
        }
        // Safe to remove
        mPtr->pop_back();
        mVec.pop_back();
    }
    /* --------------------------------------------------------------------------------------------
     * Erase the element at a certain position.
    */
    void EraseAt(SQInteger i)
    {
        ValidIdx_(i);
        // Is this element cached?
        if (mVec[static_cast< size_t >(i)].second != nullptr)
        {
            // Invalidate the instance
            mVec[static_cast< size_t >(i)].second->Cleanup();
            mVec[static_cast< size_t >(i)].first.Release();
        }
        // Safe to remove
        mPtr->erase(mPtr->begin() + static_cast< size_t >(i));
        mVec.erase(mVec.begin() + static_cast< size_t >(i));
        // Synchronize cache
        CacheSync();
    }
    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor.
    */
    void Each(Function & fn)
    {
        Validate();
        // Iterate referenced vector elements
        for (size_t i = 0; i < mVec.size(); ++i)
        {
            fn.Execute(Get(static_cast< SQInteger >(i)));
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Iterate values in range through a functor.
    */
    void EachRange(SQInteger p, SQInteger n, Function & fn)
    {
        ValidIdx_(p);
        ValidIdx_(p + n);
        // Iterate specified range
        for (n += p; p < n; ++p)
        {
            fn.Execute(Get(static_cast< SQInteger >(p)));
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor until stopped (i.e false is returned).
    */
    void While(Function & fn)
    {
        Validate();
        // Iterate referenced vector elements
        for (size_t i = 0; i < mVec.size(); ++i)
        {
            auto ret = fn.Eval(Get(static_cast< SQInteger >(i)));
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Iterate values in range through a functor until stopped (i.e false is returned).
    */
    void WhileRange(SQInteger p, SQInteger n, Function & fn)
    {
        ValidIdx_(p);
        ValidIdx_(p + n);
        // Iterate specified range
        for (n += p; p < n; ++p)
        {
            auto ret = fn.Eval(Get(static_cast< SQInteger >(p)));
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a wrapped instance as a script object.
    */
    SQMOD_NODISCARD static std::pair< LightObj, W * > Obj(T * ptr, bool owned = false)
    {
        // Create the wrapper instance for given pointer
        auto wp = std::make_unique< W >(ptr, false);
        // Create script object for wrapper instance
        std::pair< LightObj, W * > p{LightObj{wp.get()}, wp.get()};
        // Release ownership of the wrapper instance
        [[maybe_unused]] auto _ = wp.release();
        // Return the script object and wrapper instance
        return p;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a wrapped instance as a script object.
    */
    SQMOD_NODISCARD static std::pair< LightObj, W * > Obj(const T * ptr, bool owned = false)
    {
        return Obj(const_cast< T * >(ptr), owned);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Register a wrapper vector for the specified types.
*/
template < class T, class W, class U > inline void Register_Discord_VectorProxy(HSQUIRRELVM vm, Table & ns, const SQChar * name)
{
    using Container = DpVectorProxy< T, W >;
    // --------------------------------------------------------------------------------------------
    ns.Bind(name,
        Class< Container, NoConstructor< Container > >(vm, U::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &U::Fn)
        // Properties
        .Prop(_SC("Null"), &Container::IsNull)
        .Prop(_SC("Empty"), &Container::Empty)
        .Prop(_SC("Size"), &Container::Size)
        .Prop(_SC("Capacity"), &Container::Capacity, &Container::Reserve)
        // Member Methods
        .Func(_SC("Get"), &Container::Get)
        .Func(_SC("Set"), &Container::Set)
        .Func(_SC("Reserve"), &Container::Reserve)
        .Func(_SC("Compact"), &Container::Compact)
        .Func(_SC("Clear"), &Container::Clear)
        .Func(_SC("Push"), &Container::Push)
        .Func(_SC("Append"), &Container::Push)
        .Func(_SC("Extend"), &Container::Extend)
        .Func(_SC("Pop"), &Container::Pop)
        .Func(_SC("EraseAt"), &Container::EraseAt)
        .Func(_SC("Each"), &Container::Each)
        .Func(_SC("EachRange"), &Container::EachRange)
        .Func(_SC("While"), &Container::While)
        .Func(_SC("WhileRange"), &Container::WhileRange)
    );
}

/* ------------------------------------------------------------------------------------------------
 * Wrapper around a std::unordered_map of DPP values.
*/
template < class K, class V, class W > struct DpUnorderedMapProxy
{
    using Native = std::unordered_map< K, V >;
    using Binder = std::pair< LightObj, W * >;
    using Script = std::unordered_map< K, Binder >;
    using Ptr = std::unique_ptr< Native >;
    /* --------------------------------------------------------------------------------------------
     * Referenced map instance.
    */
    Ptr mPtr{nullptr};
    /* --------------------------------------------------------------------------------------------
     * Cached script objects map.
    */
    Script mMap{};
    /* --------------------------------------------------------------------------------------------
     * Whether the referenced pointer is owned.
    */
    bool mOwned{false};
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    DpUnorderedMapProxy() noexcept = default;
};

/* ------------------------------------------------------------------------------------------------
 * Register a wrapper unordered map for the specified types.
*/
template < class K, class V, class W, class U > inline void Register_Discord_UnorderedMapProxy(HSQUIRRELVM vm, Table & ns, const SQChar * name)
{
    using Container = DpUnorderedMapProxy< K, V, W >;
    // --------------------------------------------------------------------------------------------
    ns.Bind(name,
        Class< Container, NoConstructor< Container > >(vm, U::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &U::Fn)
        // Properties
        //.Prop(_SC("Null"), &Container::IsNull)
        //.Prop(_SC("Empty"), &Container::Empty)
        //.Prop(_SC("Size"), &Container::Size)
        //.Prop(_SC("Capacity"), &Container::Capacity, &Container::Reserve)
        // Member Methods
        //.Func(_SC("Get"), &Container::Get)
        //.Func(_SC("Set"), &Container::Set)
    );
}

} // Namespace:: SqMod
