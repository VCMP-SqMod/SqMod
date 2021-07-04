#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <random>
#include <iterator>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace Sqrat {

/* ------------------------------------------------------------------------------------------------
 * Helper type used to retrieve the hash of a value instead of the value itself.
*/
struct SqKeyHash
{
    SQHash mH{};
    constexpr SqKeyHash() noexcept = default;
    constexpr explicit SqKeyHash(SQHash h) noexcept : mH(h) { }
    constexpr SqKeyHash(const SqKeyHash &) noexcept = default;
    constexpr SqKeyHash & operator = (const SqKeyHash &) noexcept = default;
    constexpr operator SQHash () const noexcept { return mH; } //NOLINT (explicit)
};

/* ------------------------------------------------------------------------------------------------
 * Allows the binding system to know how to interact with SqKeyHash type.
*/
template < > struct Var< SqKeyHash > {

    SqKeyHash value;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Var(HSQUIRRELVM vm, SQInteger idx) noexcept : value(sq_gethash(vm, idx)) { }

    /* --------------------------------------------------------------------------------------------
     * Push the associated object on the stack.
    */
    static void push(HSQUIRRELVM vm, const SqKeyHash& value)
    {
        sq_pushinteger(vm, static_cast< SQInteger >(value));
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for SqKeyHash reference.
*/
template < > struct Var< SqKeyHash & > : Var< SqKeyHash >
{
    Var(HSQUIRRELVM vm, SQInteger idx) noexcept : Var< SqKeyHash >(vm, idx) { }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for constant SqKeyHash reference.
*/
template < > struct Var< const SqKeyHash & > : Var< SqKeyHash >
{
    Var(HSQUIRRELVM vm, SQInteger idx) noexcept : Var< SqKeyHash >(vm, idx) { }
};

} // Namespace:: Sqrat

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Wrapper around a std::vector of std::pair values. Efficient contiguous associative container.
*/
struct SqDictionary
{
    /* --------------------------------------------------------------------------------------------
     * Type stored in the container.
    */
    using Element = std::pair< SQHash, LightObj >;

    /* --------------------------------------------------------------------------------------------
     * The typeof container that will be used.
    */
    using Container = std::vector< Element >;

    /* --------------------------------------------------------------------------------------------
     * Container instance.
    */
    Container mC{};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    SqDictionary() = default;

    /* --------------------------------------------------------------------------------------------
     * Construct with initial capacity. No element is created.
    */
    explicit SqDictionary(SQInteger n)
        : SqDictionary()
    {
        mC.reserve(static_cast< size_t >(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SqDictionary(const SqDictionary &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SqDictionary(SqDictionary &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destroys the Statement.
    */
    ~SqDictionary() = default;

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    SqDictionary & operator = (const SqDictionary &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
    SqDictionary & operator = (SqDictionary &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a value from the container.
    */
    SQMOD_NODISCARD LightObj & Get(SqKeyHash k)
    {
        for (auto & e : mC)
        {
            if (e.first == k.mH) return e.second;
        }
        // See if we can get the specified value as a string
        StackStrF val(SqVM(), 2);
        // Include the value in the error if we can
        if (SQ_SUCCEEDED(val.Proc(false)))
        {
            STHROWF("No element found for ({}) key", val.ToStr());
        }
        else
        {
            STHROWF("No element found for specified key");
        }
        // This should not be reached
        SQ_UNREACHABLE
    }

    /* --------------------------------------------------------------------------------------------
     * Modify a value from the container.
    */
    void Set(SqKeyHash k, LightObj & v)
    {
        for (auto & e : mC)
        {
            if (e.first == k.mH)
            {
                e.second = std::move(v);
                return; // We updated existing element
            }
        }
        // Create the element now
        mC.emplace_back(k.mH, std::move(v));
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the container has no elements.
    */
    SQMOD_NODISCARD bool Empty() const
    {
        return mC.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements in the container.
    */
    SQMOD_NODISCARD SQInteger Size() const
    {
        return static_cast< SQInteger >(mC.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements that the container has currently allocated space for.
    */
    SQMOD_NODISCARD SQInteger Capacity() const
    {
        return static_cast< SQInteger >(mC.capacity());
    }

    /* --------------------------------------------------------------------------------------------
     * Increase the capacity of the container to a value that's greater or equal to the one specified.
    */
    SqDictionary & Reserve(SQInteger n)
    {
        mC.reserve(ClampL< SQInteger, size_t >(n));
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Request the removal of unused capacity.
    */
    void Compact()
    {
        mC.shrink_to_fit();
    }

    /* --------------------------------------------------------------------------------------------
     * Erase all elements from the container.
    */
    void Clear()
    {
        mC.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Erase the element with the specified key.
    */
    bool Erase(SqKeyHash k)
    {
        auto itr = std::find_if(mC.cbegin(), mC.cend(),
                            [&](Container::const_reference e) -> bool { return e.first == k.mH; });
        if (itr != mC.end())
        {
            mC.erase(itr);
            return true;
        }
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether an element exists.
    */
    bool Contains(SqKeyHash k) const
    {
        for (auto & e : mC)
        {
            if (e.first == k.mH) return true;
        }
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor.
    */
    void Each(Function & fn) const
    {
        for (const auto & e : mC)
        {
            fn.Execute(static_cast< SQInteger >(e.first), e.second);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor.
    */
    void EachWith(LightObj & ctx, Function & fn) const
    {
        for (const auto & e : mC)
        {
            fn.Execute(ctx, static_cast< SQInteger >(e.first), e.second);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor until stopped (i.e false is returned).
    */
    void While(Function & fn) const
    {
        for (const auto & e : mC)
        {
            auto ret = fn.Eval(static_cast< SQInteger >(e.first), e.second);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values through a functor until stopped (i.e false is returned).
    */
    void WhileWith(LightObj & ctx, Function & fn) const
    {
        for (const auto & e : mC)
        {
            auto ret = fn.Eval(ctx, static_cast< SQInteger >(e.first), e.second);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }
};

} // Namespace:: SqMod
