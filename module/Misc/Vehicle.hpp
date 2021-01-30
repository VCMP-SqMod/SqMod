#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name associated with a vehicle model identifier.
*/
SQMOD_NODISCARD String GetAutomobileName(uint32_t id);

/* ------------------------------------------------------------------------------------------------
 * Modify the name associated with a vehicle model identifier.
*/
void SetAutomobileName(uint32_t id, StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * Convert a vehicle model name to a vehicle model identifier.
*/
SQMOD_NODISCARD int32_t GetAutomobileID(StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified vehicle model identifier is valid.
*/
SQMOD_NODISCARD bool IsAutomobileValid(int32_t id);

} // Namespace:: SqMod
