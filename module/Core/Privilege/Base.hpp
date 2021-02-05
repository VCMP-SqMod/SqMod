#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"
#include "Core/VecMap.hpp"

// ------------------------------------------------------------------------------------------------
#include <memory>
#include <utility>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
struct SqPvUnit;
struct SqPvClass;
struct SqPvEntry;

// ------------------------------------------------------------------------------------------------
struct PvUnit;
struct PvClass;
struct PvEntry;
struct PvManager;

// ------------------------------------------------------------------------------------------------
typedef VecMap< SQInteger, SQInteger > PvStatusList;

/* ------------------------------------------------------------------------------------------------
 * Used to represent unique identity for entries, units and classes.
*/
struct PvIdentity
{
    /* --------------------------------------------------------------------------------------------
     * Unique identifier. This must be unique for amongst similar identities.
    */
    SQInteger mID{0};

    /* --------------------------------------------------------------------------------------------
     * Name hash. Optional cached value used when searching by name.
    */
    size_t mHash{0};

    /* -------------------------------------------------------------------------------------------
     * Default constructor.
    */
    constexpr PvIdentity() noexcept = default;

    /* -------------------------------------------------------------------------------------------
     * Identifier constructor.
    */
    constexpr explicit PvIdentity(SQInteger id) noexcept
        : mID(id), mHash(0)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Identifier and hash constructor.
    */
    constexpr explicit PvIdentity(SQInteger id, size_t h) noexcept
        : mID(id), mHash(h)
    {
    }

    /* -------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    PvIdentity(const PvIdentity & o) noexcept = default;

    /* -------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    PvIdentity(PvIdentity && o) noexcept = default;

    /* -------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~PvIdentity() = default;

    /* -------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    PvIdentity & operator = (const PvIdentity & o) noexcept = default;

    /* -------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    PvIdentity & operator = (PvIdentity && o) noexcept = default;
};

/* ------------------------------------------------------------------------------------------------
 * Used as for VecMap to know when two elements are considered equal (the same).
*/
struct PvIdPred
{
    /* --------------------------------------------------------------------------------------------
     * Function call operator. Takes two identities and compares the `mID` member.
    */
    bool operator() (const PvIdentity & a, const PvIdentity & b) const noexcept
    {
        return (a.mID == b.mID);
    }
};

} // Namespace:: SqMod
