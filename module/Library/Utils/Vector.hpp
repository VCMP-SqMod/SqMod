#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <iterator>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Utility used to transform optimal argument type to stored type.
*/
template < class T > struct SqVectorOpt
{
    /* --------------------------------------------------------------------------------------------
     * Optimal argument type.
    */
    using Type = T;

    /* --------------------------------------------------------------------------------------------
     * Convert the optimal type to the stored type. Does nothing special in this case.
    */
    inline static Type & Get(Type & v) { return v; }
    inline static const Type & Get(const Type & v) { return v; }
    // --------------------------------------------------------------------------------------------
    inline static void Put(std::vector< T > & c, SQInteger i, Type & v)
    { c[ConvTo< size_t >::From(i)] = v; }
    inline static void Put(std::vector< T > & c, SQInteger i, const Type & v)
    { c[ConvTo< size_t >::From(i)] = v; }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqVectorOpt for String type.
*/
template < > struct SqVectorOpt< String >
{
    /* --------------------------------------------------------------------------------------------
     * Optimal argument type.
    */
    using Type = StackStrF;

    /* --------------------------------------------------------------------------------------------
     * Convert the optimal type to the stored type.
    */
    inline static String Get(Type & v) { return v.ToStr(); }
    inline static String Get(const Type & v) { return v.ToStr(); }
    // --------------------------------------------------------------------------------------------
    inline static void Put(std::vector< String > & c, SQInteger i, Type & v)
    { c[ConvTo< size_t >::From(i)].assign(v.mPtr, v.GetSize()); }
    inline static void Put(std::vector< String > & c, SQInteger i, const Type & v)
    { c[ConvTo< size_t >::From(i)].assign(v.mPtr, v.GetSize()); }
};

/* ------------------------------------------------------------------------------------------------
 * Wrapper around a vector of values. Space efficient array.
*/
template < class T > struct SqVector
{
    /* --------------------------------------------------------------------------------------------
     * Type given via the template parameter.
    */
    using Type = T;

    /* --------------------------------------------------------------------------------------------
     * The typeof vector that will be used.
    */
    using Vector = std::vector< T >;

    /* --------------------------------------------------------------------------------------------
     * Reference to the vector.
    */
    using Reference = std::shared_ptr< Vector >;

    /* --------------------------------------------------------------------------------------------
     * Optimal type to receive a value of this type as function argument. Mainly for strings.
    */
    using OptimalType = typename SqVectorOpt< T >::Type;

    /* --------------------------------------------------------------------------------------------
     * Same as OptimalType but preferably with a reference qualifier to avoid copies.
    */
    using OptimalArg = typename std::conditional< std::is_same< T, OptimalType >::value, T, OptimalType & >::type;

    /* --------------------------------------------------------------------------------------------
     * A vector of bool needs special treatment from the other types.
    */
    using ReturnType = typename std::conditional< std::is_same< T, bool >::value, T, T & >::type;

    /* --------------------------------------------------------------------------------------------
     * Reference to the container instance.
    */
    Reference mC;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    SqVector()
        : mC(std::make_shared< Vector >())
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with initial capacity. No element is created.
    */
    explicit SqVector(SQInteger n)
        : SqVector()
    {
        mC->reserve(static_cast< size_t >(n));
    }

    /* --------------------------------------------------------------------------------------------
     * Construct with initial size. Filled with copies of specified element.
    */
    SqVector(SQInteger n, OptimalArg v)
        : SqVector()
    {
        ResizeEx(n, v);
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SqVector(SqVector &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destroys the Statement.
    */
    ~SqVector() = default;

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    SqVector & operator = (const SqVector &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
    SqVector & operator = (SqVector &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is referenced.
    */
    void Validate() const
    {
        if (!mC)
        {
            STHROWF("Invalid container instance");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is referenced and return it.
    */
    Vector & Valid() const { Validate(); return *mC; }

    /* --------------------------------------------------------------------------------------------
     * Make sure an index is within rance and return the container it. Container must exist.
    */
    Vector & ValidIdx(SQInteger i) const
    {
        if (static_cast< size_t >(i) >= mC->size())
        {
            STHROWF("Invalid container index");
        }
        return *mC;
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is referenced and is populated, then return it.
    */
    Vector & ValidPop() const
    {
        Validate();
        if (mC->empty())
        {
            STHROWF("Container is empty");
        }
        return *mC;
    }

    /* --------------------------------------------------------------------------------------------
     * Check if a container instance is referenced.
    */
    SQMOD_NODISCARD bool IsNull() const
    {
        return static_cast< bool >(mC);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a value from the container.
    */
    SQMOD_NODISCARD typename std::add_const< ReturnType >::type Get(SQInteger i) const { return ValidIdx(i).at(ConvTo< size_t >::From(i)); }

    /* --------------------------------------------------------------------------------------------
     * Modify a value from the container.
    */
    void Set(SQInteger i, OptimalArg v)
    {
        SqVectorOpt< T >::Put(ValidIdx(i), i, v);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first element in the container.
    */
    SQMOD_NODISCARD typename std::add_const< ReturnType >::type  Front() const { return ValidPop().front(); }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last element in the container.
    */
    SQMOD_NODISCARD typename std::add_const< ReturnType >::type  Back() const { return ValidPop().back(); }

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
     * Resize the container to contain a specific amount of elements.
    */
    void Resize(SQInteger n)
    {
        return Valid().resize(ConvTo< size_t >::From(n), T());
    }

    /* --------------------------------------------------------------------------------------------
     * Resize the container to contain a specific amount of elements.
    */
    void ResizeEx(SQInteger n, OptimalArg v)
    {
        return Valid().resize(ConvTo< size_t >::From(n), SqVectorOpt< T >::Get(v));
    }

    /* --------------------------------------------------------------------------------------------
     * Increase the capacity of the vector to a value that's greater or equal to the one specified.
    */
    void Reserve(SQInteger n) { return Valid().reserve(ConvTo< size_t >::From(n)); }

    /* --------------------------------------------------------------------------------------------
     * Request the removal of unused capacity.
    */
    void Compact() { return Valid().shrink_to_fit(); }

    /* --------------------------------------------------------------------------------------------
     * Erase all elements from the container.
    */
    void Clear() { return Valid().clear(); }

    /* --------------------------------------------------------------------------------------------
     * Push a value at the back of the container.
    */
    void Push(OptimalArg v) { return Valid().push_back(SqVectorOpt< T >::Get(v)); }

    /* --------------------------------------------------------------------------------------------
     * Pop the last element in the container.
    */
    void Pop() { ValidPop().pop_back(); }

    /* --------------------------------------------------------------------------------------------
     * Erase the element at a certain position.
    */
    void EraseAt(SQInteger i)
    {
        Validate();
        mC->erase(ValidIdx(i).begin() + static_cast< size_t >(i));
    }

    /* --------------------------------------------------------------------------------------------
     * Erase a certain amount of elements starting from a specific position.
    */
    void EraseFrom(SQInteger i, SQInteger n)
    {
        Validate();
        mC->erase(ValidIdx(i).begin() + static_cast< size_t >(i),
                  ValidIdx(i + n).begin() + static_cast< size_t >(i + n));
    }

    /* --------------------------------------------------------------------------------------------
     * Erase all occurrences of value from the container.
    */
    void EraseValue(OptimalArg v)
    {
        Validate();
        mC->erase(std::remove(mC->begin(), mC->end(), SqVectorOpt< T >::Get(v)), mC->end());
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a specific value starting from a certain position.
    */
    void InsertAt(SQInteger i, OptimalArg v)
    {
        Validate();
        mC->insert(ValidIdx(i).begin() + static_cast< size_t >(i), SqVectorOpt< T >::Get(v));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a certain amount of copies of a value starting from a specific position.
    */
    void Insert(SQInteger i, SQInteger n, OptimalArg v)
    {
        Validate();
        mC->insert(ValidIdx(i).begin() + static_cast< size_t >(i), ConvTo< size_t >::From(n), SqVectorOpt< T >::Get(v));
    }

    /* --------------------------------------------------------------------------------------------
     * Locate the position of a value.
    */
    SQMOD_NODISCARD SQInteger Locate(OptimalArg v) const
    {
        auto itr = std::find(Valid().begin(), Valid().end(), SqVectorOpt< T >::Get(v));
        return itr == mC->end() ? -1 : static_cast< SQInteger >(std::distance(mC->begin(), itr));
    }

    /* --------------------------------------------------------------------------------------------
     * Locate the position of a value starting from an offset.
    */
    SQMOD_NODISCARD SQInteger LocateFrom(SQInteger p, OptimalArg v) const
    {
        Validate();
        auto itr = std::find(ValidIdx(p).begin() + static_cast< size_t >(p),
                             Valid().end(), SqVectorOpt< T >::Get(v));
        return itr == mC->end() ? -1 : static_cast< SQInteger >(std::distance(mC->begin(), itr));
    }

    /* --------------------------------------------------------------------------------------------
     * Count the occurrences of a value in the container.
    */
    SQMOD_NODISCARD SQInteger Count(OptimalArg v) const
    {
        return static_cast< SQInteger >(std::count(Valid().begin(), Valid().end(), SqVectorOpt< T >::Get(v)));
    }

    /* --------------------------------------------------------------------------------------------
     * See if values are the same as another container.
    */
    SQMOD_NODISCARD bool Equal(SqVector & o) const { return Valid() == o.Valid(); }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values.
    */
    void Each(Function & fn) const
    {
        Validate();
        for (const auto & e : *mC)
        {
            fn.Execute(e);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate values in range.
    */
    void EachRange(SQInteger p, SQInteger n, Function & fn) const
    {
        Validate();
        std::for_each(ValidIdx(p).begin() + static_cast< size_t >(p),
                      ValidIdx(p + n).begin() + static_cast< size_t >(p + n),
        [&](const T & e) {
            fn.Execute(e);
        });
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate all values until stopped.
    */
    void While(Function & fn) const
    {
        Validate();
        for (const auto & e : *mC)
        {
            auto ret = fn.Eval(e);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Iterate values in range until stopped.
    */
    void WhileRange(SQInteger p, SQInteger n, Function & fn) const
    {
        Validate();
        auto itr = ValidIdx(p).begin() + static_cast< size_t >(p);
        auto end = ValidIdx(p + n).begin() + static_cast< size_t >(p + n);
        for (; itr != end; ++itr)
        {
            auto ret = fn.Eval(*itr);
            // (null || true) == continue & false == break
            if (!ret.IsNull() || !ret.template Cast< bool >())
            {
                break;
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Create a script array with the elements from the container.
    */
    SQMOD_NODISCARD Array AsArray() const
    {
        Validate();
        // Create a script array
        Array arr(SqVM());
        // Reserve space in advance
        arr.Reserve(static_cast< SQInteger >(mC->size()));
        // Make sure to preserve the stack
        StackGuard sg(SqVM());
        // Populate the array with vector elements
        arr.AppendFromCounted([](HSQUIRRELVM vm, SQInteger i, const Vector & v) -> bool {
            if (static_cast< size_t >(i) < v.size())
            {
                Var< T >::push(vm, v[static_cast< size_t >(i)]);
                // There's a value on the stack
                return true;
            }
            // No more elements
            return false;
        }, *mC);
        // Return the array
        return arr;
    }

    /* --------------------------------------------------------------------------------------------
     * Reverse the order of elements in the container.
    */
    void Reverse()
    {
         std::reverse(Valid().begin(), Valid().end());
    }

    /* --------------------------------------------------------------------------------------------
     * Generate new elements at the back of the container.
    */
    void Generate(LightObj & ctx, Function & fn)
    {
        for (;;)
        {
            auto ret = fn.Eval(ctx);
            // null == break
            if (ret.IsNull())
            {
                break;
            }
            // Extract the value from object
            mC->push_back(ret.Cast< T >());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Generate new elements at the back of the container.
    */
    void GenerateSome(SQInteger n, LightObj & ctx, Function & fn)
    {
        while (n--)
        {
            auto ret = fn.Eval(ctx);
            // Extract the value from object
            mC->push_back(ret.Cast< T >());
        }
    }
    
    /* --------------------------------------------------------------------------------------------
     * Generate new elements at specified position.
    */
    void GenerateFrom(SQInteger p, SQInteger n, LightObj & ctx, Function & fn)
    {
        Validate();
        if (static_cast< size_t >(p) >= mC->size())
        {
            STHROWF("Invalid container index");
        }
        for (auto i = static_cast< size_t >(p); n--; ++i)
        {
            auto ret = fn.Eval(ctx);
            // Extract the value from object and insert it
            mC->insert(mC->begin() + i, ret.Cast< T >());
        }
    }
};

} // Namespace:: SqMod
