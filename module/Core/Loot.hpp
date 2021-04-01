#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"
#include "Base/Vector3.hpp"
#include "Base/Quaternion.hpp"

// ------------------------------------------------------------------------------------------------
#include <array>
#include <vector>
#include <random>
#include <chrono>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Defines the type of loot and is responsible for creating and destroying the associated item.
*/
struct LootFactory
{
    // --------------------------------------------------------------------------------------------
    SQInteger   mID{-1}; // User defined item identifier.
    SQInteger   mType{-1}; // User defined item type.
    SQInteger   mClass{-1}; // User defined item class.

    // --------------------------------------------------------------------------------------------
    String      mTag; // User tag associated with this instance.
    LightObj    mData; // User data associated with this instance.

    // --------------------------------------------------------------------------------------------
    Function    mCreate{}; // Callback to create the loot item.
    Function    mDelete{}; // Callback to delete the loot item.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LootFactory() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    LootFactory(const LootFactory & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    LootFactory(LootFactory && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~LootFactory() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    LootFactory & operator = (const LootFactory & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    LootFactory & operator = (LootFactory && o) noexcept = default;
};

/* ------------------------------------------------------------------------------------------------
 * Defines an entry inside a loot cluster where a single loot item can be spawned.
*/
struct LootSpawn
{
    Vector3     mPos; // Spawn position

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LootSpawn() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    LootSpawn(const LootSpawn & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    LootSpawn(LootSpawn && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~LootSpawn() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    LootSpawn & operator = (const LootSpawn & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    LootSpawn & operator = (LootSpawn && o) noexcept = default;
};

/* ------------------------------------------------------------------------------------------------
 * Defines a cluster of loot spawns with little space in between where associated items can be spawned.
*/
class LootCluster
{
    // --------------------------------------------------------------------------------------------
    LightObj    mData; // User data associated with this instance.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LootCluster() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    LootCluster(const LootCluster & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    LootCluster(LootCluster && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~LootCluster() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    LootCluster & operator = (const LootCluster & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    LootCluster & operator = (LootCluster && o) noexcept = default;
};

/* ------------------------------------------------------------------------------------------------
 * Defines a region of loot clusters where various attributes can be influenced globally.
*/
class LootRegion
{
    // --------------------------------------------------------------------------------------------
    String      mTag; // User tag associated with this instance.
    LightObj    mData; // User data associated with this instance.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LootRegion() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    LootRegion(const LootRegion & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    LootRegion(LootRegion && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~LootRegion() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    LootRegion & operator = (const LootRegion & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    LootRegion & operator = (LootRegion && o) noexcept = default;
};

/* ------------------------------------------------------------------------------------------------
 * Loot distribution utility.
*/
class LootManager
{
public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LootManager() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    LootManager(const LootManager & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    LootManager(LootManager && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~LootManager() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    LootManager & operator = (const LootManager & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    LootManager & operator = (LootManager && o) = delete;

private:

};

} // Namespace:: SqMod
