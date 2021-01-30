#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name associated with a skin model identifier.
*/
SQMOD_NODISCARD const char * GetSkinName(uint32_t id);

/* ------------------------------------------------------------------------------------------------
 * Modify the name associated with a skin model identifier.
*/
void SetSkinName(uint32_t id, StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * Convert a vehicle model name to a skin model identifier.
*/
SQMOD_NODISCARD int32_t GetSkinID(StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified skin model identifier is valid.
*/
SQMOD_NODISCARD bool IsSkinValid(int32_t id);

} // Namespace:: SqMod
