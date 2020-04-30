#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>
#include <vector>
#include <functional>

// ------------------------------------------------------------------------------------------------
#define SQMOD_VALID_TAG_STR(t) if (!(t)) { STHROWF("The specified tag is invalid"); }

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace Algo {

/* ------------------------------------------------------------------------------------------------
 * Returns a pointer to the first occurrence of 'needle' in 'haystack'.
*/
inline CSStr sqmod_stristr(CSStr haystack, CSStr needle)
{
    for (const auto chr = static_cast< const SQChar >(std::tolower(*needle)); *haystack != '\0'; ++haystack)
    {
        if (static_cast< SQChar >(std::tolower(*haystack)) != chr)
        {
            continue;
        }

        for (CSStr itr = haystack, str = needle;; ++itr, ++str)
        {
            if (*str == '\0')
            {
                return haystack;
            }
            else if (*itr == '\0')
            {
                return nullptr;
            }
            else if (std::tolower(*itr) != std::tolower(*str))
            {
                break;
            }
        }
    }

    return nullptr;
}

/* ------------------------------------------------------------------------------------------------
 * Find the specified string in another string.
*/
inline bool FindStr(CSStr str1, CSStr str2, bool cs)
{
    if (!str1 || !str2 || (*str1 == '\0' && *str2 == '\0'))
    {
        return false;
    }
    else if (cs)
    {
        return (strstr(str1, str2) != nullptr);
    }

    return (sqmod_stristr(str1, str2) != nullptr);
}

/* ------------------------------------------------------------------------------------------------
 * Compare the specified strings.
*/
inline Int32 CompareStr(CSStr lhs, CSStr rhs, bool cs)
{
    if (!lhs)
    {
        if (!rhs)
        {
            return 0;
        }

        return -1;
    }

    if (!rhs)
    {
        return 1;
    }

    return cs ? strcmp(lhs, rhs) : sqmod_stricmp(lhs, rhs);
}

/* ------------------------------------------------------------------------------------------------
 * Compare a portion from the specified strings.
*/
inline Int32 CompareStr(CSStr lhs, CSStr rhs, Uint32 len, bool cs)
{
    if (!lhs)
    {
        if (!rhs)
        {
            return 0;
        }

        return -1;
    }

    if (!rhs)
    {
        return 1;
    }

    return cs ? strncmp(lhs, rhs, len) : sqmod_strnicmp(lhs, rhs, len);
}

/* ------------------------------------------------------------------------------------------------
 * Compare a portion from the specified strings starting from a certain position.
*/
inline Int32 CompareStr(CSStr lhs, CSStr rhs, Uint32 pos, Uint32 len, bool cs)
{
    if (!lhs)
    {
        if (!rhs)
        {
            return 0;
        }

        return -1;
    }

    if (!rhs)
    {
        return 1;
    }

    while (pos)
    {
        if (*lhs == '\0')
        {
            return -1;
        }

        ++lhs, --pos;
    }

    return cs ? strncmp(lhs, rhs, len) : sqmod_strnicmp(lhs, rhs, len);
}

/* ------------------------------------------------------------------------------------------------
 * Compare the specified strings.
*/
inline bool ApplyStrFilter(CSStr name, CSStr filter, bool cs)
{
    return cs ? NameFilterCheck(filter, name) : NameFilterCheckInsensitive(filter, name);
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range that the inspector deems worthy.
*/
template < typename Iterator, typename Inspector, typename Collector >
void Collect(Iterator first, Iterator last, Inspector inspect, Collector collect)
{
    for (; first != last; ++first)
    {
        if (inspect(*first))
        {
            collect(*first);
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range that the inspector deems worthy.
*/
template < typename Iterator, typename Inspector, typename Collector >
void CollectWhile(Iterator first, Iterator last, Inspector inspect, Collector collect)
{
    for (; first != last; ++first)
    {
        if (inspect(*first))
        {
            if (!collect(*first))
            {
                break;
            }
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range where the string matches or not the specified one.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Collector >
void EachEquals(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && (CompareStr(retrieve(*first).c_str(), str, cs) == 0) == neg)
        {
            collect(*first);
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range where the string matches or not the specified one.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Collector >
void EachEqualsWhile(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && (CompareStr(retrieve(*first).c_str(), str, cs) == 0) == neg)
        {
            if (!collect(*first))
            {
                break;
            }
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range where the string begins or not with the specified
 * string.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Collector >
void EachBegins(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, std::size_t len, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (!inspect(*first))
        {
            continue;
        }
        // Retrieve the string
        const auto & s = retrieve(*first);
        // Compare the string
        if (s.size() >= len)
        {
            if ((CompareStr(s.c_str(), str, static_cast< Uint32 >(len), cs) == 0) == neg)
            {
                collect(*first);
            }
        }
        else if (!neg)
        {
            collect(*first);
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range where the string begins or not with the specified
 * string.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Collector >
void EachBeginsWhile(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, std::size_t len, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (!inspect(*first))
        {
            continue;
        }
        // Retrieve the string
        const auto & s = retrieve(*first);
        // Compare the string
        if (s.size() >= len)
        {
            if ((CompareStr(s.c_str(), str, static_cast< Uint32 >(len), cs) == 0) == neg)
            {
                if (!collect(*first))
                {
                    break;
                }
            }
        }
        else if (!neg)
        {
            if (!collect(*first))
            {
                break;
            }
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range where the string ends or not with the specified
 * string.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Collector >
void EachEnds(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, std::size_t len, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (!inspect(*first))
        {
            continue;
        }
        // Retrieve the string
        const auto & s = retrieve(*first);
        // Compare the tag
        if (s.size() >= len)
        {
            if ((CompareStr(s.c_str(), str, static_cast< Uint32 >(s.size() - len), static_cast< Uint32 >(len), cs) == 0) == neg)
            {
                collect(*first);
            }
        }
        else if (!neg)
        {
            collect(*first);
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range where the string ends or not with the specified
 * string.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Collector >
void EachEndsWhile(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, std::size_t len, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (!inspect(*first))
        {
            continue;
        }
        // Retrieve the string
        const auto & s = retrieve(*first);
        // Compare the tag
        if (s.size() >= len)
        {
            if ((CompareStr(s.c_str(), str, static_cast< Uint32 >(s.size() - len), static_cast< Uint32 >(len), cs) == 0) == neg)
            {
                if (!collect(*first))
                {
                    break;
                }
            }
        }
        else if (!neg)
        {
            if (!collect(*first))
            {
                break;
            }
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range where the string contains or not the specified
 * string.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Collector >
void EachContains(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && FindStr(retrieve(*first).c_str(), str, cs) == neg)
        {
            collect(*first);
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range where the string contains or not the specified
 * string.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Collector >
void EachContainsWhile(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && FindStr(retrieve(*first).c_str(), str, cs) == neg)
        {
            if (!collect(*first))
            {
                break;
            }
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range where the string matches or not the specified
 * filter.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Collector >
void EachMatches(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && ApplyStrFilter(retrieve(*first).c_str(), str, cs) == neg)
        {
            collect(*first);
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Collect all elements within the specified range where the string matches or not the specified
 * filter.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Collector >
void EachMatchesWhile(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && ApplyStrFilter(retrieve(*first).c_str(), str, cs) == neg)
        {
            if (!collect(*first))
            {
                break;
            }
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Find the first element within the specified range where the string matches or not the specified one.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Receiver >
void FirstEquals(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Receiver receive,
                        CSStr str, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && (CompareStr(retrieve(*first).c_str(), str, cs) == 0) == neg)
        {
            receive(*first);
            break;
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Find the first element within the specified range where the string begins or not with the specified
 * string.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Receiver >
void FirstBegins(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Receiver receive,
                        CSStr str, std::size_t len, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (!inspect(*first))
        {
            continue;
        }
        // Retrieve the string
        const auto & s = retrieve(*first);
        // Compare the string
        if (s.size() >= len)
        {
            if ((CompareStr(s.c_str(), str, static_cast< Uint32 >(len), cs) == 0) == neg)
            {
                receive(*first);
                break;
            }
        }
        else if (!neg)
        {
            receive(*first);
            break;
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Find the first element within the specified range where the string ends or not with the specified
 * string.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Receiver >
void FirstEnds(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Receiver receive,
                        CSStr str, std::size_t len, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (!inspect(*first))
        {
            continue;
        }
        // Retrieve the string
        const auto & s = retrieve(*first);
        // Compare the string
        if (s.size() >= len)
        {
            if ((CompareStr(s.c_str(), str, static_cast< Uint32 >(s.size() - len), static_cast< Uint32 >(len), cs) == 0) == neg)
            {
                receive(*first);
                break;
            }
        }
        else if (!neg)
        {
            receive(*first);
            break;
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Find the first element within the specified range where the string contains or not the specified
 * string.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Receiver >
void FirstContains(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Receiver receive,
                        CSStr str, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && FindStr(retrieve(*first).c_str(), str, cs) == neg)
        {
            receive(*first);
            break;
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Find the first element within the specified range where the string matches or not the specified
 * filter.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Receiver >
void FirstMatches(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Receiver receive,
                        CSStr str, bool neg, bool cs)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && ApplyStrFilter(retrieve(*first).c_str(), str, cs) == neg)
        {
            receive(*first);
            break;
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Used to work with entity instances in a template fashion.
*/
template < typename T >  struct InstSpec;

/* ------------------------------------------------------------------------------------------------
 * Specialization for the Blip entity type.
*/
template <> struct InstSpec< CBlip >
{
    // --------------------------------------------------------------------------------------------
    typedef Core::Blips Instances; // Container to store instances of this entity type.
    typedef Core::Blips::value_type Instance; // Type that manages this type of entity instance.

    // --------------------------------------------------------------------------------------------
    static constexpr int Max = SQMOD_BLIP_POOL; // Maximum identifier for this entity type.

    // --------------------------------------------------------------------------------------------
    static constexpr CSStr LcName = "blip"; // Lowercase name of this entity type.
    static constexpr CSStr UcName = "Blip"; // Uppercase name of this entity type.

    /* --------------------------------------------------------------------------------------------
     * Iterator to the beginning of the instance container.
    */
    static inline Instances::const_iterator CBegin()
    {
        return Core::Get().GetBlips().cbegin();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterator to the ending of the instance container.
    */
    static inline Instances::const_iterator CEnd()
    {
        return Core::Get().GetBlips().cend();
    }

    /* --------------------------------------------------------------------------------------------
     * Reference to the NULL instance.
    */
    static inline LightObj & Null()
    {
        return Core::Get().GetNullBlip();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for the Checkpoint entity type.
*/
template <> struct InstSpec< CCheckpoint >
{
    // --------------------------------------------------------------------------------------------
    typedef Core::Checkpoints Instances; // Container to store instances of this entity type.
    typedef Core::Checkpoints::value_type Instance; // Type that manages this type of entity instance.

    // --------------------------------------------------------------------------------------------
    static constexpr int Max = SQMOD_CHECKPOINT_POOL; // Maximum identifier for this entity type.

    // --------------------------------------------------------------------------------------------
    static constexpr CSStr LcName = "checkpoint"; // Lowercase name of this entity type.
    static constexpr CSStr UcName = "Checkpoint"; // Uppercase name of this entity type.

    /* --------------------------------------------------------------------------------------------
     * Iterator to the beginning of the instance container.
    */
    static inline Instances::const_iterator CBegin()
    {
        return Core::Get().GetCheckpoints().cbegin();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterator to the ending of the instance container.
    */
    static inline Instances::const_iterator CEnd()
    {
        return Core::Get().GetCheckpoints().cend();
    }

    /* --------------------------------------------------------------------------------------------
     * Reference to the NULL instance.
    */
    static inline LightObj & Null()
    {
        return Core::Get().GetNullCheckpoint();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for the Keybind entity type.
*/
template <> struct InstSpec< CKeybind >
{
    // --------------------------------------------------------------------------------------------
    typedef Core::Keybinds Instances; // Container to store instances of this entity type.
    typedef Core::Keybinds::value_type Instance; // Type that manages this type of entity instance.

    // --------------------------------------------------------------------------------------------
    static constexpr int Max = SQMOD_KEYBIND_POOL; // Maximum identifier for this entity type.

    // --------------------------------------------------------------------------------------------
    static constexpr CSStr LcName = "keybind"; // Lowercase name of this entity type.
    static constexpr CSStr UcName = "Keybind"; // Uppercase name of this entity type.

    /* --------------------------------------------------------------------------------------------
     * Iterator to the beginning of the instance container.
    */
    static inline Instances::const_iterator CBegin()
    {
        return Core::Get().GetKeybinds().cbegin();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterator to the ending of the instance container.
    */
    static inline Instances::const_iterator CEnd()
    {
        return Core::Get().GetKeybinds().cend();
    }

    /* --------------------------------------------------------------------------------------------
     * Reference to the NULL instance.
    */
    static inline LightObj & Null()
    {
        return Core::Get().GetNullKeybind();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for the Object entity type.
*/
template <> struct InstSpec< CObject >
{
    // --------------------------------------------------------------------------------------------
    typedef Core::Objects Instances; // Container to store instances of this entity type.
    typedef Core::Objects::value_type Instance; // Type that manages this type of entity instance.

    // --------------------------------------------------------------------------------------------
    static constexpr int Max = SQMOD_OBJECT_POOL; // Maximum identifier for this entity type.

    // --------------------------------------------------------------------------------------------
    static constexpr CSStr LcName = "object"; // Lowercase name of this entity type.
    static constexpr CSStr UcName = "Object"; // Uppercase name of this entity type.

    /* --------------------------------------------------------------------------------------------
     * Iterator to the beginning of the instance container.
    */
    static inline Instances::const_iterator CBegin()
    {
        return Core::Get().GetObjs().cbegin();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterator to the ending of the instance container.
    */
    static inline Instances::const_iterator CEnd()
    {
        return Core::Get().GetObjs().cend();
    }

    /* --------------------------------------------------------------------------------------------
     * Reference to the NULL instance.
    */
    static inline LightObj & Null()
    {
        return Core::Get().GetNullObject();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for the Pickup entity type.
*/
template <> struct InstSpec< CPickup >
{
    // --------------------------------------------------------------------------------------------
    typedef Core::Pickups Instances; // Container to store instances of this entity type.
    typedef Core::Pickups::value_type Instance; // Type that manages this type of entity instance.

    // --------------------------------------------------------------------------------------------
    static constexpr int Max = SQMOD_PICKUP_POOL; // Maximum identifier for this entity type.

    // --------------------------------------------------------------------------------------------
    static constexpr CSStr LcName = "pickup"; // Lowercase name of this entity type.
    static constexpr CSStr UcName = "Pickup"; // Uppercase name of this entity type.

    /* --------------------------------------------------------------------------------------------
     * Iterator to the beginning of the instance container.
    */
    static inline Instances::const_iterator CBegin()
    {
        return Core::Get().GetPickups().cbegin();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterator to the ending of the instance container.
    */
    static inline Instances::const_iterator CEnd()
    {
        return Core::Get().GetPickups().cend();
    }

    /* --------------------------------------------------------------------------------------------
     * Reference to the NULL instance.
    */
    static inline LightObj & Null()
    {
        return Core::Get().GetNullPickup();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for the Player entity type.
*/
template <> struct InstSpec< CPlayer >
{
    // --------------------------------------------------------------------------------------------
    typedef Core::Players Instances; // Container to store instances of this entity type.
    typedef Core::Players::value_type Instance; // Type that manages this type of entity instance.

    // --------------------------------------------------------------------------------------------
    static constexpr int Max = SQMOD_PLAYER_POOL; // Maximum identifier for this entity type.

    // --------------------------------------------------------------------------------------------
    static constexpr CSStr LcName = "player"; // Lowercase name of this entity type.
    static constexpr CSStr UcName = "Player"; // Uppercase name of this entity type.

    /* --------------------------------------------------------------------------------------------
     * Iterator to the beginning of the instance container.
    */
    static inline Instances::const_iterator CBegin()
    {
        return Core::Get().GetPlayers().cbegin();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterator to the ending of the instance container.
    */
    static inline Instances::const_iterator CEnd()
    {
        return Core::Get().GetPlayers().cend();
    }

    /* --------------------------------------------------------------------------------------------
     * Reference to the NULL instance.
    */
    static inline LightObj & Null()
    {
        return Core::Get().GetNullPlayer();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for the Vehicle entity type.
*/
template <> struct InstSpec< CVehicle >
{
    // --------------------------------------------------------------------------------------------
    typedef Core::Vehicles Instances; // Container to store instances of this entity type.
    typedef Core::Vehicles::value_type Instance; // Type that manages this type of entity instance.

    // --------------------------------------------------------------------------------------------
    static constexpr int Max = SQMOD_VEHICLE_POOL; // Maximum identifier for this entity type.

    // --------------------------------------------------------------------------------------------
    static constexpr CSStr LcName = "vehicle"; // Lowercase name of this entity type.
    static constexpr CSStr UcName = "Vehicle"; // Uppercase name of this entity type.

    /* --------------------------------------------------------------------------------------------
     * Iterator to the beginning of the instance container.
    */
    static inline Instances::const_iterator CBegin()
    {
        return Core::Get().GetVehicles().cbegin();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterator to the ending of the instance container.
    */
    static inline Instances::const_iterator CEnd()
    {
        return Core::Get().GetVehicles().cend();
    }

    /* --------------------------------------------------------------------------------------------
     * Reference to the NULL instance.
    */
    static inline LightObj & Null()
    {
        return Core::Get().GetNullVehicle();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor to check if an instance is valid.
*/
template < typename T > struct ValidInstFunc
{
    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    bool operator () (const typename InstSpec< T >::Instance & inst) const
    {
        return VALID_ENTITY(inst.mID);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor to retrieve the entity tag.
*/
template < typename T > struct InstTagFunc
{
    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    const String & operator () (const typename InstSpec< T >::Instance & inst) const
    {
        return inst.mInst->GetTag();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor to append the entity instance object at the back of an array.
*/
template < typename T > struct AppendElemFunc
{
    // --------------------------------------------------------------------------------------------
    const SQInteger mIdx; // Array index after pushing one object on the stack.
    HSQUIRRELVM     mVM; // The VM to use when pushing the object.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit AppendElemFunc(SQInteger idx = -2, HSQUIRRELVM vm = SqVM())
        : mIdx(idx), mVM(vm)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    void operator () (const typename InstSpec< T >::Instance & inst) const
    {
        // Push the script object on the stack
        sq_pushobject(mVM, inst.mObj.GetObj());
        // Append the object at the back of the array
        if (SQ_FAILED(sq_arrayappend(mVM, mIdx)))
        {
            STHROWF("Unable to append %s instance to the list", InstSpec< T >::LcName);
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor to retrieve the entity tag.
*/
template < typename T > struct RecvElemFunc
{
    // --------------------------------------------------------------------------------------------
    LightObj  mObj; // Reference to the received object.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    RecvElemFunc()
        : mObj(InstSpec< T >::Null())
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    void operator () (const typename InstSpec< T >::Instance & inst)
    {
        mObj = inst.mObj;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor to forward the the received element to a callback.
*/
template < typename T > struct ForwardElemFunc
{
public:

    // --------------------------------------------------------------------------------------------
    LightObj    mEnv; // The script callback environment object.
    LightObj    mFunc; // The script callback object to receive the element.
    Uint32      mCount; // The number of elements forwarded by this functor.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    ForwardElemFunc(Function & func)
        : mEnv(func.GetEnv()), mFunc(func.GetFunc()), mCount(0)
    {
        if (mFunc.IsNull())
        {
            STHROWF("Invalid script callback");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    bool operator () (const typename InstSpec< T >::Instance & inst)
    {
        HSQUIRRELVM vm = SqVM();
        // Push the callback object
        sq_pushobject(vm, mFunc.mObj);
        // Push the callback environment object
        sq_pushobject(vm, mEnv.mObj);
        // Push the element instance on the stack
        sq_pushobject(vm, inst.mObj.mObj);
        // Make the function call and store the result
        SQRESULT res = sq_call(vm, 2, static_cast< SQBool >(true), static_cast< SQBool >(ErrorHandling::IsEnabled()));
        // Make sure the callback object and return value are popped from the stack
        const SqPopGuard pg(vm, 2);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return false; // Stop the iteration process
        }
        // Is the returned value not null?
        else if (sq_gettype(vm, -1) != OT_NULL)
        {
            // Default to continue
            SQBool ret = SQTrue;
            // Obtain the returned value as a boolean
            sq_tobool(vm, -1, &ret);
            // Should we proceed to the next element or stop here?
            if (ret == SQFalse)
            {
                return false; // Stop the iteration process
            }
        }
        // Only successful forwards are counted!
        ++mCount;
        // Proceed to the next element, if any
        return true;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit cast to the count value.
    */
    operator Uint32 () const // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        return mCount;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor to forward the the received element and specific payload to a callback.
*/
template < typename T > struct ForwardElemDataFunc
{
public:

    // --------------------------------------------------------------------------------------------
    LightObj    mEnv; // The script callback environment object.
    LightObj    mFunc; // The script callback object to receive the element.
    LightObj    mData; // The script payload to accompany the element.
    Uint32      mCount; // The number of elements forwarded by this functor.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    ForwardElemDataFunc(LightObj & data, Function & func)
        : mEnv(func.GetEnv()), mFunc(func.GetFunc()), mData(data), mCount(0)
    {
        if (mFunc.IsNull())
        {
            STHROWF("Invalid script callback");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    bool operator () (const typename InstSpec< T >::Instance & inst)
    {
        HSQUIRRELVM vm = SqVM();
        // Push the callback object
        sq_pushobject(vm, mFunc.mObj);
        // Push the callback environment object
        sq_pushobject(vm, mEnv.mObj);
        // Push the element instance on the stack
        sq_pushobject(vm, inst.mObj.mObj);
        // Push the iteration context on the stack
        sq_pushobject(vm, mData.mObj);
        // Make the function call and store the result
        SQRESULT res = sq_call(vm, 3, static_cast< SQBool >(true), static_cast< SQBool >(ErrorHandling::IsEnabled()));
        // Make sure the callback object and return value are popped from the stack
        const SqPopGuard pg(vm, 2);
        // Validate the result
        if (SQ_FAILED(res))
        {
            return false; // Stop the iteration process
        }
        // Is the returned value not null?
        else if (sq_gettype(vm, -1) != OT_NULL)
        {
            // Default to continue
            SQBool ret = SQTrue;
            // Obtain the returned value as a boolean
            sq_tobool(vm, -1, &ret);
            // Should we proceed to the next element or stop here?
            if (ret == SQFalse)
            {
                return false; // Stop the iteration process
            }
        }
        // Only successful forwards are counted!
        ++mCount;
        // Proceed to the next element, if any
        return true;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit cast to the count value.
    */
    operator Uint32 () const // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        return mCount;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor to count the the received elements.
*/
template < typename T > struct CountElemFunc
{
public:

    // --------------------------------------------------------------------------------------------
    Uint32      mCount; // The number of elements received by this functor.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    CountElemFunc()
        : mCount(0)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    void operator () (const typename InstSpec< T >::Instance & /*inst*/)
    {
        ++mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit cast to the count value.
    */
    operator Uint32 () const // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        return mCount;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Shared utilities for entities in order to avoid duplicate code.
*/
template < typename T > struct Entity
{
public:

    // --------------------------------------------------------------------------------------------
    typedef T Type; // The type of entity that we're working with.
    typedef InstSpec< T > Inst; // The type of entity instance to work with.

    // --------------------------------------------------------------------------------------------
    typedef ValidInstFunc< T >          ValidInst;
    typedef InstTagFunc< T >            InstTag;
    typedef AppendElemFunc< T >         AppendElem;
    typedef RecvElemFunc< T >           RecvElem;
    typedef ForwardElemFunc< T >        ForwardElem;
    typedef ForwardElemDataFunc< T >    ForwardElemData;
    typedef CountElemFunc< T >          CountElem;

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Entity() = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Entity(const Entity & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Entity(Entity && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Entity() = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Entity & operator = (const Entity & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Entity & operator = (Entity && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Find the entity that matches the specified identifier.
    */
    static inline const LightObj & FindByID(Int32 id)
    {
        // Perform a range check on the specified identifier
        if (INVALID_ENTITYEX(id, Inst::Max))
        {
            STHROWF("The specified %s identifier is invalid: %d", Inst::LcName, id);
        }
        // Obtain the ends of the entity pool
        typename Inst::Instances::const_iterator itr = Inst::CBegin();
        typename Inst::Instances::const_iterator end = Inst::CEnd();
        // Process each entity in the pool
        for (; itr != end; ++itr)
        {
            // Does the identifier match the specified one?
            if (itr->mID == id)
            {
                return itr->mObj; // Stop searching and return this entity
            }
        }
        // Unable to locate a player matching the specified identifier
        return Inst::Null();
    }

    /* --------------------------------------------------------------------------------------------
     * Find all active entities of this type.
    */
    static inline Array AllActive()
    {
        const StackGuard sg;
        // Allocate an empty array on the stack
        sq_newarray(SqVM(), 0);
        // Process each entity in the pool
        Collect(Inst::CBegin(), Inst::CEnd(), ValidInst(), AppendElem());
        // Return the array at the top of the stack
        return Var< Array >(SqVM(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Collect all entities of this type where the tag matches or not the specified one.
    */
    static inline Array AllWhereTagEquals(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Remember the current stack size
        const StackGuard sg;
        // Allocate an empty array on the stack
        sq_newarray(SqVM(), 0);
        // Process each entity in the pool
        EachEquals(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), AppendElem(), tag, !neg, cs);
        // Return the array at the top of the stack
        return Var< Array >(SqVM(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Collect all entities of this type where the tag begins or not with the specified string.
    */
    static inline Array AllWhereTagBegins(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Remember the current stack size
        const StackGuard sg;
        // Allocate an empty array on the stack
        sq_newarray(SqVM(), 0);
        // Process each entity in the pool
        EachBegins(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), AppendElem(), tag, strlen(tag), !neg, cs);
        // Return the array at the top of the stack
        return Var< Array >(SqVM(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Collect all entities of this type where the tag ends or not with the specified string.
    */
    static inline Array AllWhereTagEnds(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Remember the current stack size
        const StackGuard sg;
        // Allocate an empty array on the stack
        sq_newarray(SqVM(), 0);
        // Process each entity in the pool
        EachEnds(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), AppendElem(), tag, strlen(tag), !neg, cs);
        // Return the array at the top of the stack
        return Var< Array >(SqVM(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Collect all entities of this type where the tag contains or not the specified string.
    */
    static inline Array AllWhereTagContains(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Remember the current stack size
        const StackGuard sg;
        // Allocate an empty array on the stack
        sq_newarray(SqVM(), 0);
        // Process each entity in the pool
        EachContains(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), AppendElem(), tag, !neg, cs);
        // Return the array at the top of the stack
        return Var< Array >(SqVM(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Collect all entities of this type where the tag matches or not the specified filter.
    */
    static inline Array AllWhereTagMatches(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Remember the current stack size
        const StackGuard sg;
        // Allocate an empty array on the stack
        sq_newarray(SqVM(), 0);
        // Process each entity in the pool
        EachMatches(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), AppendElem(), tag, !neg, cs);
        // Return the array at the top of the stack
        return Var< Array >(SqVM(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first entity of this type where the tag matches or not the specified one.
    */
    static inline LightObj FirstWhereTagEquals(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element receiver
        RecvElem recv;
        // Process each entity in the pool
        FirstEquals(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< RecvElem >(recv), tag, !neg, cs);
        // Return the received element, if any
        return recv.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first entity of this type where the tag begins or not with the specified string.
    */
    static inline LightObj FirstWhereTagBegins(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element receiver
        RecvElem recv;
        // Process each entity in the pool
        FirstBegins(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< RecvElem >(recv), tag, strlen(tag), !neg, cs);
        // Return the received element, if any
        return recv.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first entity of this type where the tag ends or not with the specified string.
    */
    static inline LightObj FirstWhereTagEnds(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element receiver
        RecvElem recv;
        // Process each entity in the pool
        FirstEnds(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< RecvElem >(recv), tag, strlen(tag), !neg, cs);
        // Return the received element, if any
        return recv.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first entity of this type where the tag contains or not the specified string.
    */
    static inline LightObj FirstWhereTagContains(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element receiver
        RecvElem recv;
        // Process each entity in the pool
        FirstContains(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< RecvElem >(recv), tag, !neg, cs);
        // Return the received element, if any
        return recv.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first entity of this type where the tag matches or not the specified filter.
    */
    static inline LightObj FirstWhereTagMatches(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element receiver
        RecvElem recv;
        // Process each entity in the pool
        FirstMatches(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< RecvElem >(recv), tag, !neg, cs);
        // Return the received element, if any
        return recv.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all active entities of this type.
    */
    static inline Uint32 EachActive(Function & func)
    {
        // Create a new element forwarder
        ForwardElem fwd(func);
        // Process each entity in the pool
        CollectWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(),
                        std::reference_wrapper< ForwardElem >(fwd));
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all active entities of this type.
    */
    static inline Uint32 EachActiveData(LightObj & data, Function & func)
    {
        // Create a new element forwarder
        ForwardElemData fwd(data, func);
        // Process each entity in the pool
        CollectWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(),
                        std::reference_wrapper< ForwardElemData >(fwd));
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the tag matches or not the specified one.
    */
    static inline Uint32 EachWhereTagEquals(bool neg, bool cs, CSStr tag, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElem fwd(func);
        // Process each entity in the pool
        EachEqualsWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< ForwardElem >(fwd), tag, !neg, cs);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the tag matches or not the specified one.
    */
    static inline Uint32 EachWhereTagEqualsData(bool neg, bool cs, CSStr tag, LightObj & data, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElemData fwd(data, func);
        // Process each entity in the pool
        EachEqualsWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< ForwardElemData >(fwd), tag, !neg, cs);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the tag begins with the specified string.
    */
    static inline Uint32 EachWhereTagBegins(bool neg, bool cs, CSStr tag, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElem fwd(func);
        // Process each entity in the pool
        EachBeginsWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< ForwardElem >(fwd), tag, strlen(tag), !neg, cs);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the tag begins with the specified string.
    */
    static inline Uint32 EachWhereTagBeginsData(bool neg, bool cs, CSStr tag, LightObj & data, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElemData fwd(data, func);
        // Process each entity in the pool
        EachBeginsWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< ForwardElemData >(fwd), tag, strlen(tag), !neg, cs);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the tag ends or not with the specified string.
    */
    static inline Uint32 EachWhereTagEnds(bool neg, bool cs, CSStr tag, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElem fwd(func);
        // Process each entity in the pool
        EachEndsWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< ForwardElem >(fwd), tag, strlen(tag), !neg, cs);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the tag ends or not with the specified string.
    */
    static inline Uint32 EachWhereTagEndsData(bool neg, bool cs, CSStr tag, LightObj & data, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElemData fwd(data, func);
        // Process each entity in the pool
        EachEndsWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< ForwardElemData >(fwd), tag, strlen(tag), !neg, cs);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the tag contains the specified string.
    */
    static inline Uint32 EachWhereTagContains(bool neg, bool cs, CSStr tag, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElem fwd(func);
        // Process each entity in the pool
        EachContainsWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< ForwardElem >(fwd), tag, !neg, cs);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the tag contains the specified string.
    */
    static inline Uint32 EachWhereTagContainsData(bool neg, bool cs, CSStr tag, LightObj & data, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElemData fwd(data, func);
        // Process each entity in the pool
        EachContainsWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< ForwardElemData >(fwd), tag, !neg, cs);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the tag match the specified filter.
    */
    static inline Uint32 EachWhereTagMatches(bool neg, bool cs, CSStr tag, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElem fwd(func);
        // Process each entity in the pool
        EachMatchesWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< ForwardElem >(fwd), tag, !neg, cs);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the tag match the specified filter.
    */
    static inline Uint32 EachWhereTagMatchesData(bool neg, bool cs, CSStr tag, LightObj & data, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElemData fwd(data, func);
        // Process each entity in the pool
        EachMatchesWhile(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< ForwardElemData >(fwd), tag, !neg, cs);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Count all active entities of this type.
    */
    static inline Uint32 CountActive()
    {
        // Create a new element counter
        CountElem cnt;
        // Process each entity in the pool
        Collect(Inst::CBegin(), Inst::CEnd(), ValidInst(),
                        std::reference_wrapper< CountElem >(cnt));
        // Return the count
        return cnt;
    }

    /* --------------------------------------------------------------------------------------------
     * Count all entities of this type where the tag matches or not the specified one.
    */
    static inline Uint32 CountWhereTagEquals(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element counter
        CountElem cnt;
        // Process each entity in the pool
        EachEquals(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< CountElem >(cnt), tag, !neg, cs);
        // Return the count
        return cnt;
    }

    /* --------------------------------------------------------------------------------------------
     * Count all entities of this type where the tag begins with the specified string.
    */
    static inline Uint32 CountWhereTagBegins(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element counter
        CountElem cnt;
        // Process each entity in the pool
        EachBegins(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< CountElem >(cnt), tag, strlen(tag), !neg, cs);
        // Return the count
        return cnt;
    }

    /* --------------------------------------------------------------------------------------------
     * Count all entities of this type where the tag ends or not with the specified string.
    */
    static inline Uint32 CountWhereTagEnds(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element counter
        CountElem cnt;
        // Process each entity in the pool
        EachEnds(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< CountElem >(cnt), tag, strlen(tag), !neg, cs);
        // Return the count
        return cnt;
    }

    /* --------------------------------------------------------------------------------------------
     * Count all entities of this type where the tag contains the specified string.
    */
    static inline Uint32 CountWhereTagContains(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element counter
        CountElem cnt;
        // Process each entity in the pool
        EachContains(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                        std::reference_wrapper< CountElem >(cnt), tag, !neg, cs);
        // Return the count
        return cnt;
    }

    /* --------------------------------------------------------------------------------------------
     * Count all entities of this type where the tag matches the specified filter.
    */
    static inline CountElem CountWhereTagMatches(bool neg, bool cs, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element counter
        CountElem cnt;
        // Process each entity in the pool
        EachMatches(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(),
                    std::reference_wrapper< CountElem >(cnt), tag, !neg, static_cast< bool >(cs));
        // Return the count
        return cnt;
    }
};

} // Namespace:: Algo
} // Namespace:: SqMod
