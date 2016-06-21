#ifndef _BASE_ALGO_HPP_
#define _BASE_ALGO_HPP_

// ------------------------------------------------------------------------------------------------
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>
#include <vector>

// ------------------------------------------------------------------------------------------------
#define SQMOD_VALID_TAG_STR(t) if (!t) { STHROWF("The specified tag is invalid"); }

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace Algo {

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
 * Collect all elements within the specified range where the string matches or not the specified one.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Collector >
void EachEquals(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, bool neg)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && (retrieve(*first).compare(str) == 0) == neg)
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
void EachBegins(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, std::size_t len, bool neg)
{
    for (; first != last; ++first)
    {
        if (!inspect(*first))
        {
            continue;
        }
        // Retrieve the tag
        const String & tag = retrieve(*first);
        // Compare the tag
        if (tag.size() > len && (tag.compare(0, len, str) == 0) == neg)
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
void EachEnds(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, std::size_t len, bool neg)
{
    for (; first != last; ++first)
    {
        if (!inspect(*first))
        {
            continue;
        }
        // Retrieve the tag
        const String & tag = retrieve(*first);
        // Compare the tag
        if (tag.size() > len && (tag.compare(tag.size() - len, len, str) == 0) == neg)
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
void EachContains(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Collector collect,
                        CSStr str, bool neg)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && (retrieve(*first).find(str) != String::npos) == neg)
        {
            collect(*first);
        }
    }
}

/* ------------------------------------------------------------------------------------------------
 * Find the first element within the specified range where the string matches or not the specified one.
*/
template < typename Iterator, typename Inspector, typename Retriever, typename Receiver >
void FirstEquals(Iterator first, Iterator last,
                        Inspector inspect, Retriever retrieve, Receiver receive,
                        CSStr str, bool neg)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && (retrieve(*first).compare(str) == 0) == neg)
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
                        CSStr str, std::size_t len, bool neg)
{
    for (; first != last; ++first)
    {
        if (!inspect(*first))
        {
            continue;
        }
        // Retrieve the tag
        const String & tag = retrieve(*first);
        // Compare the tag
        if (tag.size() > len && (tag.compare(0, len, str) == 0) == neg)
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
                        CSStr str, std::size_t len, bool neg)
{
    for (; first != last; ++first)
    {
        if (!inspect(*first))
        {
            continue;
        }
        // Retrieve the tag
        const String & tag = retrieve(*first);
        // Compare the tag
        if (tag.size() > len && (tag.compare(tag.size() - len, len, str) == 0) == neg)
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
                        CSStr str, bool neg)
{
    for (; first != last; ++first)
    {
        if (inspect(*first) && (retrieve(*first).find(str) != String::npos) == neg)
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
        return Core::Get().GetObjects().cbegin();
    }

    /* --------------------------------------------------------------------------------------------
     * Iterator to the ending of the instance container.
    */
    static inline Instances::const_iterator CEnd()
    {
        return Core::Get().GetObjects().cend();
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
    AppendElemFunc(SQInteger idx = -2, HSQUIRRELVM vm = DefaultVM::Get())
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
        sq_pushobject(mVM, inst.mObj.GetObject());
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
    Object  mObj; // Reference to the received object.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    RecvElemFunc()
        : mObj()
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

    /* --------------------------------------------------------------------------------------------
     * Implicit cast to the managed object.
    */
    operator Object ()
    {
        return mObj;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor to forward the the received element to a callback.
*/
template < typename T > struct ForwardElemFunc
{
public:

    // --------------------------------------------------------------------------------------------
    Function    mFunc; // The script callback to forward the element.
    Uint32      mCount; // The number of elements forwarded by this functor.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    ForwardElemFunc(Object & env, Function & func)
        : mFunc(env.GetVM(), env, func.GetFunc()), mCount(0)
    {
        if (mFunc.IsNull())
        {
            STHROWF("Invalid script callback");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    void operator () (const typename InstSpec< T >::Instance & inst)
    {
        mFunc.Execute(inst.mObj);
        ++mCount; // Only successful forwards are counted!
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
    typedef ValidInstFunc< T >      ValidInst;
    typedef InstTagFunc< T >        InstTag;
    typedef AppendElemFunc< T >     AppendElem;
    typedef RecvElemFunc< T >       RecvElem;
    typedef ForwardElemFunc< T >    ForwardElem;

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
    static inline const Object & FindByID(Int32 id)
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
        return NullObject();
    }

    /* --------------------------------------------------------------------------------------------
     * Find all active entities of this type.
    */
    static inline Array AllActive()
    {
        const StackGuard sg;
        // Allocate an empty array on the stack
        sq_newarray(DefaultVM::Get(), 0);
        // Process each entity in the pool
        Collect(Inst::CBegin(), Inst::CEnd(), ValidInst(), AppendElem());
        // Return the array at the top of the stack
        return Var< Array >(DefaultVM::Get(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Collect all entities of this type where the string match or not the specified one.
    */
    static inline Array AllWhereTagEquals(bool neg, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Remember the current stack size
        const StackGuard sg;
        // Allocate an empty array on the stack
        sq_newarray(DefaultVM::Get(), 0);
        // Process each entity in the pool
        EachEquals(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), AppendElem(), tag, neg);
        // Return the array at the top of the stack
        return Var< Array >(DefaultVM::Get(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Collect all entities of this type where the string begins or not with the specified string.
    */
    static inline Array AllWhereTagBegins(bool neg, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Remember the current stack size
        const StackGuard sg;
        // Allocate an empty array on the stack
        sq_newarray(DefaultVM::Get(), 0);
        // Process each entity in the pool
        EachBegins(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), AppendElem(), tag, strlen(tag), neg);
        // Return the array at the top of the stack
        return Var< Array >(DefaultVM::Get(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Collect all entities of this type where the string ends or not with the specified string.
    */
    static inline Array AllWhereTagEnds(bool neg, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Remember the current stack size
        const StackGuard sg;
        // Allocate an empty array on the stack
        sq_newarray(DefaultVM::Get(), 0);
        // Process each entity in the pool
        EachEnds(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), AppendElem(), tag, strlen(tag), neg);
        // Return the array at the top of the stack
        return Var< Array >(DefaultVM::Get(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Collect all entities of this type where the string contains or not the specified string.
    */
    static inline Array AllWhereTagContains(bool neg, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Remember the current stack size
        const StackGuard sg;
        // Allocate an empty array on the stack
        sq_newarray(DefaultVM::Get(), 0);
        // Process each entity in the pool
        EachContains(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), AppendElem(), tag, neg);
        // Return the array at the top of the stack
        return Var< Array >(DefaultVM::Get(), -1).value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first entity of this type where the tag matches or not the specified one.
    */
    static inline Object FirstWhereTagEquals(bool neg, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element receiver
        RecvElem recv;
        // Process each entity in the pool
        FirstEquals(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), recv, tag, neg);
        // Return the received element, if any
        return recv.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first entity of this type where the tag begins or not with the specified string.
    */
    static inline Object FirstWhereTagBegins(bool neg, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element receiver
        RecvElem recv;
        // Process each entity in the pool
        FirstBegins(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), recv, tag, strlen(tag), neg);
        // Return the received element, if any
        return recv.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first entity of this type where the tag ends or not with the specified string.
    */
    static inline Object FirstWhereTagEnds(bool neg, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element receiver
        RecvElem recv;
        // Process each entity in the pool
        FirstEnds(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), recv, tag, strlen(tag), neg);
        // Return the received element, if any
        return recv.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first entity of this type where the tag contains or not the specified string.
    */
    static inline Object FirstWhereTagContains(bool neg, CSStr tag)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element receiver
        RecvElem recv;
        // Process each entity in the pool
        FirstContains(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), recv, tag, neg);
        // Return the received element, if any
        return recv.mObj;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all active entities of this type.
    */
    static inline Uint32 EachActive(Object & env, Function & func)
    {
        // Create a new element forwarder
        ForwardElem fwd(env, func);
        // Process each entity in the pool
        Collect(Inst::CBegin(), Inst::CEnd(), ValidInst(), fwd);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the string matches or not the specified one.
    */
    static inline Uint32 EachWhereTagEquals(bool neg, CSStr tag, Object & env, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElem fwd(env, func);
        // Process each entity in the pool
        EachEquals(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), fwd, tag, neg);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the string begins with the specified string.
    */
    static inline Uint32 EachWhereTagBegins(bool neg, CSStr tag, Object & env, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElem fwd(env, func);
        // Process each entity in the pool
        EachBegins(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), fwd, tag, strlen(tag), neg);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the string ends or not with the specified string.
    */
    static inline Uint32 EachWhereTagEnds(bool neg, CSStr tag, Object & env, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElem fwd(env, func);
        // Process each entity in the pool
        EachEnds(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), fwd, tag, strlen(tag), neg);
        // Return the forward count
        return fwd.mCount;
    }

    /* --------------------------------------------------------------------------------------------
     * Process all entities of this type where the string contains the specified string.
    */
    static inline Uint32 EachWhereTagContains(bool neg, CSStr tag, Object & env, Function & func)
    {
        SQMOD_VALID_TAG_STR(tag)
        // Create a new element forwarder
        ForwardElem fwd(env, func);
        // Process each entity in the pool
        EachContains(Inst::CBegin(), Inst::CEnd(), ValidInst(), InstTag(), fwd, tag, neg);
        // Return the forward count
        return fwd.mCount;
    }
};

} // Namespace:: Algo
} // Namespace:: SqMod

#endif // _BASE_ALGO_HPP_
