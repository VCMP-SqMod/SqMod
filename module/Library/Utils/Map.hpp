#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <map>
#include <random>
#include <iterator>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Utility used to transform optimal argument type to stored type.
*/
template < class T > struct SqMapOpt
{
    /* --------------------------------------------------------------------------------------------
     * Optimal argument type.
    */
    using Type = T;

    /* --------------------------------------------------------------------------------------------
     * Container type.
    */
    using Container = std::map< T, LightObj >;

    /* --------------------------------------------------------------------------------------------
     * Convert the optimal type to the stored type. Does nothing special in this case.
    */
    inline static Type & Get(Type & k) { return k; }
    inline static const Type & Get(const Type & k) { return k; }
    // --------------------------------------------------------------------------------------------
    inline static void Put(Container & c, LightObj & v, Type & k)
    {
        c[k] = v;
    }
    inline static void Put(Container & c, LightObj & v, const Type & k)
    {
        c[k] = v;
    }
    // --------------------------------------------------------------------------------------------
    inline static void Put(Container & c, LightObj && v, Type & k)
    {
        c[k] = std::move(v);
    }
    inline static void Put(Container & c, LightObj && v, const Type & k)
    {
        c[k] = std::move(v);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of SqMapOpt for String type.
*/
template < > struct SqMapOpt< String >
{
    /* --------------------------------------------------------------------------------------------
     * Optimal argument type.
    */
    using Type = StackStrF;

    /* --------------------------------------------------------------------------------------------
     * Container type.
    */
    using Container = std::map< String, LightObj >;

    /* --------------------------------------------------------------------------------------------
     * Convert the optimal type to the stored type.
    */
    inline static String Get(Type & k) { return k.ToStr(); }
    inline static String Get(const Type & k) { return k.ToStr(); }
    // --------------------------------------------------------------------------------------------
    inline static void Put(Container & c, LightObj & v, Type & k)
    {
        c[k.ToStr()] = v;
    }
    inline static void Put(Container & c, LightObj & v, const Type & k)
    {
        c[k.ToStr()] = v;
    }
    // --------------------------------------------------------------------------------------------
    inline static void Put(Container & c, LightObj && v, Type & k)
    {
        c[k.ToStr()] = std::move(v);
    }
    inline static void Put(Container & c, LightObj && v, const Type & k)
    {
        c[k.ToStr()] = std::move(v);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Wrapper around a std::map of values. Space efficient table.
*/
template < class T > struct SqMap
{
    /* --------------------------------------------------------------------------------------------
     * Type given via the template parameter.
    */
    using Type = T;

    /* --------------------------------------------------------------------------------------------
     * The typeof container that will be used.
    */
    using Container = std::map< T, LightObj >;

    /* --------------------------------------------------------------------------------------------
     * Type stored in the container.
    */
    using Element = typename Container::value_type;

    /* --------------------------------------------------------------------------------------------
     * Reference to the container.
    */
    using Reference = std::shared_ptr< Container >;

    /* --------------------------------------------------------------------------------------------
     * Type given used to interact with specialized value cases.
    */
    using Opt = SqMapOpt< T >;

    /* --------------------------------------------------------------------------------------------
     * Optimal type to receive a value of this type as function argument. Mainly for strings.
    */
    using OptimalType = typename Opt::Type;

    /* --------------------------------------------------------------------------------------------
     * Same as OptimalType but preferably with a reference qualifier to avoid copies.
    */
    using OptimalArg = typename std::conditional< std::is_same< T, OptimalType >::value, T, OptimalType & >::type;

    /* --------------------------------------------------------------------------------------------
     * Reference to the container instance.
    */
    Reference mC;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    SqMap()
        : mC(std::make_shared< Container >())
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor from reference.
    */
    explicit SqMap(const Reference & v)
        : mC(v)
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor from reference.
    */
    explicit SqMap(Reference && v) noexcept
        : mC(std::move(v))
    {
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SqMap(SqMap &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destroys the Statement.
    */
    ~SqMap() = default;

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    SqMap & operator = (const SqMap &) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment.
    */
    SqMap & operator = (SqMap &&) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is referenced.
    */
    void Validate() const
    {
        if (!mC)
        {
            STHROWF("Invalid map container instance");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is referenced and return it.
    */
    Container & Valid() const { Validate(); return *mC; }

    /* --------------------------------------------------------------------------------------------
     * Make sure a container instance is referenced and is populated, then return it.
    */
    Container & ValidPop() const
    {
        Validate();
        if (mC->empty())
        {
            STHROWF("Vector container is empty");
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
    SQMOD_NODISCARD const LightObj & Get(OptimalArg k) const
    {
        return Valid()[Opt::Get(k)];
    }

    /* --------------------------------------------------------------------------------------------
     * Modify a value from the container.
    */
    void Set(LightObj & v, OptimalArg k)
    {
        Opt::Put(Valid(), std::move(v), k);
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the container has no elements.
    */
    SQMOD_NODISCARD bool Empty() const
    {
        return Valid().empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements in the container.
    */
    SQMOD_NODISCARD SQInteger Size() const
    {
        return static_cast< SQInteger >(Valid().size());
    }

    /* --------------------------------------------------------------------------------------------
     * Erase all elements from the container.
    */
    void Clear()
    {
        return Valid().clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Erase the element with a certain key from the container.
    */
    void Erase(OptimalArg k)
    {
        Valid().erase(Opt::Get(k));
    }
};

} // Namespace:: SqMod
