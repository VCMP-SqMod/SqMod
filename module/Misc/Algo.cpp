// ------------------------------------------------------------------------------------------------
#include "Misc/Algo.hpp"

// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Entity/Checkpoint.hpp"
#include "Entity/KeyBind.hpp"
#include "Entity/Object.hpp"
#include "Entity/Pickup.hpp"
#include "Entity/Player.hpp"
#include "Entity/Vehicle.hpp"

// ------------------------------------------------------------------------------------------------
#define SQMOD_VALID_NAME_STR(t) if (!(t)) { STHROWF("The specified name is invalid"); }

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace Algo {

// ------------------------------------------------------------------------------------------------
constexpr const SQChar * InstSpec< CBlip >::LcName;
constexpr const SQChar * InstSpec< CBlip >::UcName;
constexpr const SQChar * InstSpec< CCheckpoint >::LcName;
constexpr const SQChar * InstSpec< CCheckpoint >::UcName;
constexpr const SQChar * InstSpec< CKeyBind >::LcName;
constexpr const SQChar * InstSpec< CKeyBind >::UcName;
constexpr const SQChar * InstSpec< CObject >::LcName;
constexpr const SQChar * InstSpec< CObject >::UcName;
constexpr const SQChar * InstSpec< CPickup >::LcName;
constexpr const SQChar * InstSpec< CPickup >::UcName;
constexpr const SQChar * InstSpec< CPlayer >::LcName;
constexpr const SQChar * InstSpec< CPlayer >::UcName;
constexpr const SQChar * InstSpec< CVehicle >::LcName;
constexpr const SQChar * InstSpec< CVehicle >::UcName;

/* ------------------------------------------------------------------------------------------------
 * Used to fake a string so a raw buffer can be used with search algorithms.
*/
struct FakeString
{
    // --------------------------------------------------------------------------------------------
    SQChar          mBuffer[SQMOD_PLAYER_TMP_BUFFER]; // Buffer to hold the data.
    std::size_t     mSize; // The size of the data in the buffer.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    FakeString()
        : mBuffer(), mSize(0)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Grab the player name from the server into the buffer.
    */
    void GrabName(int id)
    {
        // Clear any previous string (just in case)
        mBuffer[0] = '\0';
        mSize = 0;
        // Query the server for the name of the managed player
        if (_Func->GetPlayerName(id, mBuffer, sizeof(mBuffer)) == vcmpErrorNone)
        {
            mSize = std::strlen(mBuffer); // Calculate the name length
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the size of the name.
    */
    SQMOD_NODISCARD std::size_t size() const
    {
        return mSize;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the string buffer.
    */
    SQMOD_NODISCARD const SQChar * c_str() const
    {
        return mBuffer;
    }

    /* --------------------------------------------------------------------------------------------
     * Find in buffer contents of another string.
    */
    std::size_t find(const SQChar * s) const
    {
        const char * r = std::strstr(mBuffer, s);
        return (r == nullptr) ? String::npos : (r - mBuffer);
    }

    /* --------------------------------------------------------------------------------------------
     * Compare the buffer contents with another string.
    */
    int compare(const SQChar * s) const
    {
        return std::strcmp(mBuffer, s);
    }

    /* --------------------------------------------------------------------------------------------
     * Compare the buffer contents with another string.
    */
    int compare(std::size_t pos, std::size_t len, const SQChar * s) const
    {
        return std::strncmp(mBuffer + pos, s, len);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Functor to retrieve the player name.
*/
struct PlayerName
{
    // --------------------------------------------------------------------------------------------
    FakeString mStr; // The string where the name is stored.

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    const FakeString & operator () (const typename InstSpec< CPlayer >::Instance & inst)
    {
        mStr.GrabName(inst.mID);
        return mStr;
    }
};

// ------------------------------------------------------------------------------------------------
static const LightObj & Blip_FindBySprID(int32_t spr_id)
{
    // Perform a range check on the specified identifier
    if (spr_id < 0)
    {
        STHROWF("The specified sprite identifier is invalid: {}", spr_id);
    }
    // Obtain the ends of the entity pool
    auto itr = Core::Get().GetBlips().cbegin();
    auto end = Core::Get().GetBlips().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mSprID == spr_id)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a blip matching the specified identifier
    return Core::Get().GetNullBlip();
}

/* ------------------------------------------------------------------------------------------------
 * Collect all players where the name matches or not the specified one.
*/
static inline Array Player_AllWhereNameEquals(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Remember the current stack size
    const StackGuard sg;
    // Allocate an empty array on the stack
    sq_newarray(SqVM(), 0);
    // Process each entity in the pool
    EachEquals(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
               ValidInstFunc< CPlayer >(), PlayerName(),
               AppendElemFunc< CPlayer >(), name, !neg, cs);
    // Return the array at the top of the stack
    return Var< Array >(SqVM(), -1).value;
}

/* ------------------------------------------------------------------------------------------------
 * Collect all players where the name begins or not with the specified string.
*/
static inline Array Player_AllWhereNameBegins(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Remember the current stack size
    const StackGuard sg;
    // Allocate an empty array on the stack
    sq_newarray(SqVM(), 0);
    // Process each entity in the pool
    EachBegins(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                AppendElemFunc< CPlayer >(), name, strlen(name), !neg, cs);
    // Return the array at the top of the stack
    return Var< Array >(SqVM(), -1).value;
}

/* ------------------------------------------------------------------------------------------------
 * Collect all players where the name ends or not with the specified string.
*/
static inline Array Player_AllWhereNameEnds(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Remember the current stack size
    const StackGuard sg;
    // Allocate an empty array on the stack
    sq_newarray(SqVM(), 0);
    // Process each entity in the pool
    EachEnds(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                AppendElemFunc< CPlayer >(), name, strlen(name), !neg, cs);
    // Return the array at the top of the stack
    return Var< Array >(SqVM(), -1).value;
}

/* ------------------------------------------------------------------------------------------------
 * Collect all players where the name contains or not the specified string.
*/
static inline Array Player_AllWhereNameContains(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Remember the current stack size
    const StackGuard sg;
    // Allocate an empty array on the stack
    sq_newarray(SqVM(), 0);
    // Process each entity in the pool
    EachContains(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                AppendElemFunc< CPlayer >(), name, !neg, cs);
    // Return the array at the top of the stack
    return Var< Array >(SqVM(), -1).value;
}

/* ------------------------------------------------------------------------------------------------
 * Collect all players where the name matches or not the specified filter.
*/
static inline Array Player_AllWhereNameMatches(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Remember the current stack size
    const StackGuard sg;
    // Allocate an empty array on the stack
    sq_newarray(SqVM(), 0);
    // Process each entity in the pool
    EachMatches(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                AppendElemFunc< CPlayer >(), name, !neg, cs);
    // Return the array at the top of the stack
    return Var< Array >(SqVM(), -1).value;
}

/* ------------------------------------------------------------------------------------------------
 * Retrieve the first player where the name matches or not the specified one.
*/
static inline LightObj Player_FirstWhereNameEquals(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element receiver
    RecvElemFunc< CPlayer > recv;
    // Process each entity in the pool
    FirstEquals(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< RecvElemFunc< CPlayer > >(recv), name, !neg, cs);
    // Return the received element, if any
    return recv.mObj;
}

/* ------------------------------------------------------------------------------------------------
 * Retrieve the first player where the name begins or not with the specified string.
*/
static inline LightObj Player_FirstWhereNameBegins(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element receiver
    RecvElemFunc< CPlayer > recv;
    // Process each entity in the pool
    FirstBegins(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< RecvElemFunc< CPlayer > >(recv), name, strlen(name), !neg, cs);
    // Return the received element, if any
    return recv.mObj;
}

/* ------------------------------------------------------------------------------------------------
 * Retrieve the first player where the name ends or not with the specified string.
*/
static inline LightObj Player_FirstWhereNameEnds(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element receiver
    RecvElemFunc< CPlayer > recv;
    // Process each entity in the pool
    FirstEnds(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< RecvElemFunc< CPlayer > >(recv), name, strlen(name), !neg, cs);
    // Return the received element, if any
    return recv.mObj;
}

/* ------------------------------------------------------------------------------------------------
 * Retrieve the first player where the name contains or not the specified string.
*/
static inline LightObj Player_FirstWhereNameContains(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element receiver
    RecvElemFunc< CPlayer > recv;
    // Process each entity in the pool
    FirstContains(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< RecvElemFunc< CPlayer > >(recv), name, !neg, cs);
    // Return the received element, if any
    return recv.mObj;
}

/* ------------------------------------------------------------------------------------------------
 * Retrieve the first player where the name matches or not the specified filter.
*/
static inline LightObj Player_FirstWhereNameMatches(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element receiver
    RecvElemFunc< CPlayer > recv;
    // Process each entity in the pool
    FirstMatches(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< RecvElemFunc< CPlayer > >(recv), name, !neg, cs);
    // Return the received element, if any
    return recv.mObj;
}

/* --------------------------------------------------------------------------------------------
 * Process all entities of this type where the name matches or not the specified one.
*/
static inline uint32_t Player_EachWhereNameEquals(bool neg, bool cs, const SQChar * name, Function & func)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element forwarder
    ForwardElemFunc< CPlayer > fwd(func);
    // Process each entity in the pool
    EachEqualsWhile(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< ForwardElemFunc< CPlayer > >(fwd), name, !neg, cs);
    // Return the forward count
    return fwd.mCount;
}

/* --------------------------------------------------------------------------------------------
 * Process all entities of this type where the name matches or not the specified one.
*/
static inline uint32_t Player_EachWhereNameEqualsData(bool neg, bool cs, const SQChar * name, LightObj & data, Function & func)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element forwarder
    ForwardElemDataFunc< CPlayer > fwd(data, func);
    // Process each entity in the pool
    EachEqualsWhile(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< ForwardElemDataFunc< CPlayer > >(fwd), name, !neg, cs);
    // Return the forward count
    return fwd.mCount;
}

/* --------------------------------------------------------------------------------------------
 * Process all entities of this type where the name begins with the specified string.
*/
static inline uint32_t Player_EachWhereNameBegins(bool neg, bool cs, const SQChar * name, Function & func)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element forwarder
    ForwardElemFunc< CPlayer > fwd(func);
    // Process each entity in the pool
    EachBeginsWhile(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< ForwardElemFunc< CPlayer > >(fwd), name, strlen(name), !neg, cs);
    // Return the forward count
    return fwd.mCount;
}

/* --------------------------------------------------------------------------------------------
 * Process all entities of this type where the name begins with the specified string.
*/
static inline uint32_t Player_EachWhereNameBeginsData(bool neg, bool cs, const SQChar * name, LightObj & data, Function & func)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element forwarder
    ForwardElemDataFunc< CPlayer > fwd(data, func);
    // Process each entity in the pool
    EachBeginsWhile(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< ForwardElemDataFunc< CPlayer > >(fwd), name, strlen(name), !neg, cs);
    // Return the forward count
    return fwd.mCount;
}

/* --------------------------------------------------------------------------------------------
 * Process all entities of this type where the name ends or not with the specified string.
*/
static inline uint32_t Player_EachWhereNameEnds(bool neg, bool cs, const SQChar * name, Function & func)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element forwarder
    ForwardElemFunc< CPlayer > fwd(func);
    // Process each entity in the pool
    EachEndsWhile(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< ForwardElemFunc< CPlayer > >(fwd), name, strlen(name), !neg, cs);
    // Return the forward count
    return fwd.mCount;
}

/* --------------------------------------------------------------------------------------------
 * Process all entities of this type where the name ends or not with the specified string.
*/
static inline uint32_t Player_EachWhereNameEndsData(bool neg, bool cs, const SQChar * name, LightObj & data, Function & func)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element forwarder
    ForwardElemDataFunc< CPlayer > fwd(data, func);
    // Process each entity in the pool
    EachEndsWhile(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< ForwardElemDataFunc< CPlayer > >(fwd), name, strlen(name), !neg, cs);
    // Return the forward count
    return fwd.mCount;
}

/* --------------------------------------------------------------------------------------------
 * Process all entities of this type where the name contains the specified string.
*/
static inline uint32_t Player_EachWhereNameContains(bool neg, bool cs, const SQChar * name, Function & func)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element forwarder
    ForwardElemFunc< CPlayer > fwd(func);
    // Process each entity in the pool
    EachContainsWhile(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< ForwardElemFunc< CPlayer > >(fwd), name, !neg, cs);
    // Return the forward count
    return fwd.mCount;
}

/* --------------------------------------------------------------------------------------------
 * Process all entities of this type where the name contains the specified string.
*/
static inline uint32_t Player_EachWhereNameContainsData(bool neg, bool cs, const SQChar * name, LightObj & data, Function & func)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element forwarder
    ForwardElemDataFunc< CPlayer > fwd(data, func);
    // Process each entity in the pool
    EachContainsWhile(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< ForwardElemDataFunc< CPlayer > >(fwd), name, !neg, cs);
    // Return the forward count
    return fwd.mCount;
}

/* --------------------------------------------------------------------------------------------
 * Process all entities of this type where the name matches the specified filter.
*/
static inline uint32_t Player_EachWhereNameMatches(bool neg, bool cs, const SQChar * name, Function & func)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element forwarder
    ForwardElemFunc< CPlayer > fwd(func);
    // Process each entity in the pool
    EachMatchesWhile(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< ForwardElemFunc< CPlayer > >(fwd), name, !neg, cs);
    // Return the forward count
    return fwd.mCount;
}

/* --------------------------------------------------------------------------------------------
 * Process all entities of this type where the name matches the specified filter.
*/
static inline uint32_t Player_EachWhereNameMatchesData(bool neg, bool cs, const SQChar * name, LightObj & data, Function & func)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element forwarder
    ForwardElemDataFunc< CPlayer > fwd(data, func);
    // Process each entity in the pool
    EachMatchesWhile(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< ForwardElemDataFunc< CPlayer > >(fwd), name, !neg, cs);
    // Return the forward count
    return fwd.mCount;
}

/* --------------------------------------------------------------------------------------------
 * Count all entities of this type where the name matches or not the specified one.
*/
static inline CountElemFunc <CPlayer> Player_CountWhereNameEquals(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element counter
    CountElemFunc< CPlayer > cnt;
    // Process each entity in the pool
    EachEquals(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< CountElemFunc< CPlayer > >(cnt), name, !neg, cs);
    // Return the count
    return cnt;
}

/* --------------------------------------------------------------------------------------------
 * Count all entities of this type where the name begins with the specified string.
*/
static inline uint32_t Player_CountWhereNameBegins(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element counter
    CountElemFunc< CPlayer > cnt;
    // Process each entity in the pool
    EachBegins(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< CountElemFunc< CPlayer > >(cnt), name, strlen(name), !neg, cs);
    // Return the count
    return cnt;
}

/* --------------------------------------------------------------------------------------------
 * Count all entities of this type where the name ends or not with the specified string.
*/
static inline uint32_t Player_CountWhereNameEnds(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element counter
    CountElemFunc< CPlayer > cnt;
    // Process each entity in the pool
    EachEnds(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< CountElemFunc< CPlayer > >(cnt), name, strlen(name), !neg, cs);
    // Return the count
    return cnt;
}

/* --------------------------------------------------------------------------------------------
 * Count all entities of this type where the name contains the specified string.
*/
static inline uint32_t Player_CountWhereNameContains(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element counter
    CountElemFunc< CPlayer > cnt;
    // Process each entity in the pool
    EachContains(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< CountElemFunc< CPlayer > >(cnt), name, !neg, cs);
    // Return the count
    return cnt;
}

/* --------------------------------------------------------------------------------------------
 * Count all entities of this type where the name matches the specified filter.
*/
static inline uint32_t Player_CountWhereNameMatches(bool neg, bool cs, const SQChar * name)
{
    SQMOD_VALID_NAME_STR(name)
    // Create a new element counter
    CountElemFunc< CPlayer > cnt;
    // Process each entity in the pool
    EachMatches(InstSpec< CPlayer >::CBegin(), InstSpec< CPlayer >::CEnd(),
                ValidInstFunc< CPlayer >(), PlayerName(),
                std::reference_wrapper< CountElemFunc< CPlayer > >(cnt), name, !neg, cs);
    // Return the count
    return cnt;
}

// ================================================================================================
void Register(HSQUIRRELVM vm)
{
    Table collect_ns(vm);

    collect_ns.Bind(_SC("Blip"), Table(vm)
        .Func(_SC("Active"), &Entity< CBlip >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CBlip >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CBlip >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CBlip >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CBlip >::AllWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CBlip >::AllWhereTagMatches)
    );

    collect_ns.Bind(_SC("Checkpoint"), Table(vm)
        .Func(_SC("Active"), &Entity< CCheckpoint >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CCheckpoint >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CCheckpoint >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CCheckpoint >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CCheckpoint >::AllWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CCheckpoint >::AllWhereTagMatches)
    );

    collect_ns.Bind(_SC("KeyBind"), Table(vm)
        .Func(_SC("Active"), &Entity< CKeyBind >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CKeyBind >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CKeyBind >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CKeyBind >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CKeyBind >::AllWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CKeyBind >::AllWhereTagMatches)
    );

    collect_ns.Bind(_SC("Object"), Table(vm)
        .Func(_SC("Active"), &Entity< CObject >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CObject >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CObject >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CObject >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CObject >::AllWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CObject >::AllWhereTagMatches)
    );

    collect_ns.Bind(_SC("Pickup"), Table(vm)
        .Func(_SC("Active"), &Entity< CPickup >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CPickup >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPickup >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPickup >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPickup >::AllWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CPickup >::AllWhereTagMatches)
    );

    collect_ns.Bind(_SC("Player"), Table(vm)
        .Func(_SC("Active"), &Entity< CPlayer >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CPlayer >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPlayer >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPlayer >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPlayer >::AllWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CPlayer >::AllWhereTagMatches)
        .Func(_SC("NameEquals"), &Player_AllWhereNameEquals)
        .Func(_SC("NameBegins"), &Player_AllWhereNameBegins)
        .Func(_SC("NameEnds"), &Player_AllWhereNameEnds)
        .Func(_SC("NameContains"), &Player_AllWhereNameContains)
        .Func(_SC("NameMatches"), &Player_AllWhereNameMatches)
    );

    collect_ns.Bind(_SC("Vehicle"), Table(vm)
        .Func(_SC("Active"), &Entity< CVehicle >::AllActive)
        .Func(_SC("TagEquals"), &Entity< CVehicle >::AllWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CVehicle >::AllWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CVehicle >::AllWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CVehicle >::AllWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CVehicle >::AllWhereTagMatches)
    );

    RootTable(vm).Bind(_SC("SqCollect"), collect_ns);

    Table find_ns(vm);

    find_ns.Bind(_SC("Blip"), Table(vm)
        .Func(_SC("WithID"), &Entity< CBlip >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CBlip >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CBlip >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CBlip >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CBlip >::FirstWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CBlip >::FirstWhereTagMatches)
        .Func(_SC("WithSprID"), &Blip_FindBySprID)
    );

    find_ns.Bind(_SC("Checkpoint"), Table(vm)
        .Func(_SC("WithID"), &Entity< CCheckpoint >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CCheckpoint >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CCheckpoint >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CCheckpoint >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CCheckpoint >::FirstWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CCheckpoint >::FirstWhereTagMatches)
    );

    find_ns.Bind(_SC("KeyBind"), Table(vm)
        .Func(_SC("WithID"), &Entity< CKeyBind >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CKeyBind >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CKeyBind >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CKeyBind >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CKeyBind >::FirstWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CKeyBind >::FirstWhereTagMatches)
    );

    find_ns.Bind(_SC("Object"), Table(vm)
        .Func(_SC("WithID"), &Entity< CObject >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CObject >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CObject >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CObject >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CObject >::FirstWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CObject >::FirstWhereTagMatches)
    );

    find_ns.Bind(_SC("Pickup"), Table(vm)
        .Func(_SC("WithID"), &Entity< CPickup >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CPickup >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPickup >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPickup >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPickup >::FirstWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CPickup >::FirstWhereTagMatches)
    );

    find_ns.Bind(_SC("Player"), Table(vm)
        .Func(_SC("WithID"), &Entity< CPlayer >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CPlayer >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPlayer >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPlayer >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPlayer >::FirstWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CPlayer >::FirstWhereTagMatches)
        .Func(_SC("NameEquals"), &Player_FirstWhereNameEquals)
        .Func(_SC("NameBegins"), &Player_FirstWhereNameBegins)
        .Func(_SC("NameEnds"), &Player_FirstWhereNameEnds)
        .Func(_SC("NameContains"), &Player_FirstWhereNameContains)
        .Func(_SC("NameMatches"), &Player_FirstWhereNameMatches)
    );

    find_ns.Bind(_SC("Vehicle"), Table(vm)
        .Func(_SC("WithID"), &Entity< CVehicle >::FindByID)
        .Func(_SC("TagEquals"), &Entity< CVehicle >::FirstWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CVehicle >::FirstWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CVehicle >::FirstWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CVehicle >::FirstWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CVehicle >::FirstWhereTagMatches)
    );

    RootTable(vm).Bind(_SC("SqFind"), find_ns);

    Table each_ns(vm);

    each_ns.Bind(_SC("Blip"), Table(vm)
        .Func(_SC("Active"), &Entity< CBlip >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CBlip >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CBlip >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CBlip >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CBlip >::EachWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CBlip >::EachWhereTagMatches)
    );

    each_ns.Bind(_SC("Checkpoint"), Table(vm)
        .Func(_SC("Active"), &Entity< CCheckpoint >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CCheckpoint >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CCheckpoint >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CCheckpoint >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CCheckpoint >::EachWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CCheckpoint >::EachWhereTagMatches)
    );

    each_ns.Bind(_SC("KeyBind"), Table(vm)
        .Func(_SC("Active"), &Entity< CKeyBind >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CKeyBind >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CKeyBind >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CKeyBind >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CKeyBind >::EachWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CKeyBind >::EachWhereTagMatches)
    );

    each_ns.Bind(_SC("Object"), Table(vm)
        .Func(_SC("Active"), &Entity< CObject >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CObject >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CObject >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CObject >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CObject >::EachWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CObject >::EachWhereTagMatches)
    );

    each_ns.Bind(_SC("Pickup"), Table(vm)
        .Func(_SC("Active"), &Entity< CPickup >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CPickup >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPickup >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPickup >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPickup >::EachWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CPickup >::EachWhereTagMatches)
    );

    each_ns.Bind(_SC("Player"), Table(vm)
        .Func(_SC("Active"), &Entity< CPlayer >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CPlayer >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPlayer >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPlayer >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPlayer >::EachWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CPlayer >::EachWhereTagMatches)
        .Func(_SC("NameEquals"), &Player_EachWhereNameEquals)
        .Func(_SC("NameBegins"), &Player_EachWhereNameBegins)
        .Func(_SC("NameEnds"), &Player_EachWhereNameEnds)
        .Func(_SC("NameContains"), &Player_EachWhereNameContains)
        .Func(_SC("NameMatches"), &Player_EachWhereNameMatches)
    );

    each_ns.Bind(_SC("Vehicle"), Table(vm)
        .Func(_SC("Active"), &Entity< CVehicle >::EachActive)
        .Func(_SC("TagEquals"), &Entity< CVehicle >::EachWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CVehicle >::EachWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CVehicle >::EachWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CVehicle >::EachWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CVehicle >::EachWhereTagMatches)
    );

    RootTable(vm).Bind(_SC("SqForeach"), each_ns);

    Table exeach_ns(vm);

    exeach_ns.Bind(_SC("Blip"), Table(vm)
        .Func(_SC("Active"), &Entity< CBlip >::EachActiveData)
        .Func(_SC("TagEquals"), &Entity< CBlip >::EachWhereTagEqualsData)
        .Func(_SC("TagBegins"), &Entity< CBlip >::EachWhereTagBeginsData)
        .Func(_SC("TagEnds"), &Entity< CBlip >::EachWhereTagEndsData)
        .Func(_SC("TagContains"), &Entity< CBlip >::EachWhereTagContainsData)
        .Func(_SC("TagMatches"), &Entity< CBlip >::EachWhereTagMatchesData)
    );

    exeach_ns.Bind(_SC("Checkpoint"), Table(vm)
        .Func(_SC("Active"), &Entity< CCheckpoint >::EachActiveData)
        .Func(_SC("TagEquals"), &Entity< CCheckpoint >::EachWhereTagEqualsData)
        .Func(_SC("TagBegins"), &Entity< CCheckpoint >::EachWhereTagBeginsData)
        .Func(_SC("TagEnds"), &Entity< CCheckpoint >::EachWhereTagEndsData)
        .Func(_SC("TagContains"), &Entity< CCheckpoint >::EachWhereTagContainsData)
        .Func(_SC("TagMatches"), &Entity< CCheckpoint >::EachWhereTagMatchesData)
    );

    exeach_ns.Bind(_SC("KeyBind"), Table(vm)
        .Func(_SC("Active"), &Entity< CKeyBind >::EachActiveData)
        .Func(_SC("TagEquals"), &Entity< CKeyBind >::EachWhereTagEqualsData)
        .Func(_SC("TagBegins"), &Entity< CKeyBind >::EachWhereTagBeginsData)
        .Func(_SC("TagEnds"), &Entity< CKeyBind >::EachWhereTagEndsData)
        .Func(_SC("TagContains"), &Entity< CKeyBind >::EachWhereTagContainsData)
        .Func(_SC("TagMatches"), &Entity< CKeyBind >::EachWhereTagMatchesData)
    );

    exeach_ns.Bind(_SC("Object"), Table(vm)
        .Func(_SC("Active"), &Entity< CObject >::EachActiveData)
        .Func(_SC("TagEquals"), &Entity< CObject >::EachWhereTagEqualsData)
        .Func(_SC("TagBegins"), &Entity< CObject >::EachWhereTagBeginsData)
        .Func(_SC("TagEnds"), &Entity< CObject >::EachWhereTagEndsData)
        .Func(_SC("TagContains"), &Entity< CObject >::EachWhereTagContainsData)
        .Func(_SC("TagMatches"), &Entity< CObject >::EachWhereTagMatchesData)
    );

    exeach_ns.Bind(_SC("Pickup"), Table(vm)
        .Func(_SC("Active"), &Entity< CPickup >::EachActiveData)
        .Func(_SC("TagEquals"), &Entity< CPickup >::EachWhereTagEqualsData)
        .Func(_SC("TagBegins"), &Entity< CPickup >::EachWhereTagBeginsData)
        .Func(_SC("TagEnds"), &Entity< CPickup >::EachWhereTagEndsData)
        .Func(_SC("TagContains"), &Entity< CPickup >::EachWhereTagContainsData)
        .Func(_SC("TagMatches"), &Entity< CPickup >::EachWhereTagMatchesData)
    );

    exeach_ns.Bind(_SC("Player"), Table(vm)
        .Func(_SC("Active"), &Entity< CPlayer >::EachActiveData)
        .Func(_SC("TagEquals"), &Entity< CPlayer >::EachWhereTagEqualsData)
        .Func(_SC("TagBegins"), &Entity< CPlayer >::EachWhereTagBeginsData)
        .Func(_SC("TagEnds"), &Entity< CPlayer >::EachWhereTagEndsData)
        .Func(_SC("TagContains"), &Entity< CPlayer >::EachWhereTagContainsData)
        .Func(_SC("TagMatches"), &Entity< CPlayer >::EachWhereTagMatchesData)
        .Func(_SC("NameEquals"), &Player_EachWhereNameEqualsData)
        .Func(_SC("NameBegins"), &Player_EachWhereNameBeginsData)
        .Func(_SC("NameEnds"), &Player_EachWhereNameEndsData)
        .Func(_SC("NameContains"), &Player_EachWhereNameContainsData)
        .Func(_SC("NameMatches"), &Player_EachWhereNameMatchesData)
    );

    exeach_ns.Bind(_SC("Vehicle"), Table(vm)
        .Func(_SC("Active"), &Entity< CVehicle >::EachActiveData)
        .Func(_SC("TagEquals"), &Entity< CVehicle >::EachWhereTagEqualsData)
        .Func(_SC("TagBegins"), &Entity< CVehicle >::EachWhereTagBeginsData)
        .Func(_SC("TagEnds"), &Entity< CVehicle >::EachWhereTagEndsData)
        .Func(_SC("TagContains"), &Entity< CVehicle >::EachWhereTagContainsData)
        .Func(_SC("TagMatches"), &Entity< CVehicle >::EachWhereTagMatchesData)
    );

    RootTable(vm).Bind(_SC("SqForeachEx"), exeach_ns);

    Table count_ns(vm);

    count_ns.Bind(_SC("Blip"), Table(vm)
        .Func(_SC("Active"), &Entity< CBlip >::CountActive)
        .Func(_SC("TagEquals"), &Entity< CBlip >::CountWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CBlip >::CountWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CBlip >::CountWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CBlip >::CountWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CBlip >::CountWhereTagMatches)
    );

    count_ns.Bind(_SC("Checkpoint"), Table(vm)
        .Func(_SC("Active"), &Entity< CCheckpoint >::CountActive)
        .Func(_SC("TagEquals"), &Entity< CCheckpoint >::CountWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CCheckpoint >::CountWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CCheckpoint >::CountWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CCheckpoint >::CountWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CCheckpoint >::CountWhereTagMatches)
    );

    count_ns.Bind(_SC("KeyBind"), Table(vm)
        .Func(_SC("Active"), &Entity< CKeyBind >::CountActive)
        .Func(_SC("TagEquals"), &Entity< CKeyBind >::CountWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CKeyBind >::CountWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CKeyBind >::CountWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CKeyBind >::CountWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CKeyBind >::CountWhereTagMatches)
    );

    count_ns.Bind(_SC("Object"), Table(vm)
        .Func(_SC("Active"), &Entity< CObject >::CountActive)
        .Func(_SC("TagEquals"), &Entity< CObject >::CountWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CObject >::CountWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CObject >::CountWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CObject >::CountWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CObject >::CountWhereTagMatches)
    );

    count_ns.Bind(_SC("Pickup"), Table(vm)
        .Func(_SC("Active"), &Entity< CPickup >::CountActive)
        .Func(_SC("TagEquals"), &Entity< CPickup >::CountWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPickup >::CountWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPickup >::CountWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPickup >::CountWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CPickup >::CountWhereTagMatches)
    );

    count_ns.Bind(_SC("Player"), Table(vm)
        .Func(_SC("Active"), &Entity< CPlayer >::CountActive)
        .Func(_SC("TagEquals"), &Entity< CPlayer >::CountWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CPlayer >::CountWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CPlayer >::CountWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CPlayer >::CountWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CPlayer >::CountWhereTagMatches)
        .Func(_SC("NameEquals"), &Player_CountWhereNameEquals)
        .Func(_SC("NameBegins"), &Player_CountWhereNameBegins)
        .Func(_SC("NameEnds"), &Player_CountWhereNameEnds)
        .Func(_SC("NameContains"), &Player_CountWhereNameContains)
        .Func(_SC("NameMatches"), &Player_CountWhereNameMatches)
    );

    count_ns.Bind(_SC("Vehicle"), Table(vm)
        .Func(_SC("Active"), &Entity< CVehicle >::CountActive)
        .Func(_SC("TagEquals"), &Entity< CVehicle >::CountWhereTagEquals)
        .Func(_SC("TagBegins"), &Entity< CVehicle >::CountWhereTagBegins)
        .Func(_SC("TagEnds"), &Entity< CVehicle >::CountWhereTagEnds)
        .Func(_SC("TagContains"), &Entity< CVehicle >::CountWhereTagContains)
        .Func(_SC("TagMatches"), &Entity< CVehicle >::CountWhereTagMatches)
    );

    RootTable(vm).Bind(_SC("SqCount"), count_ns);
}

} // Namespace:: Algo

// ================================================================================================
void Register_Algo(HSQUIRRELVM vm)
{
    Algo::Register(vm);
}

} // Namespace:: SqMod
