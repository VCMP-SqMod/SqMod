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
 * Built-in inventory.
*/
class InventoryManager
{
public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    InventoryManager() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    InventoryManager(const InventoryManager & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    InventoryManager(InventoryManager && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~InventoryManager() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    InventoryManager & operator = (const InventoryManager & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    InventoryManager & operator = (InventoryManager && o) = delete;

private:

};

} // Namespace:: SqMod
