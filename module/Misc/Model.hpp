#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name associated with a model identifier.
*/
SQMOD_NODISCARD const SQChar * GetModelName(int32_t id);

/* ------------------------------------------------------------------------------------------------
 * Modify the name associated with a model identifier.
*/
void SetModelName(int32_t id, StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * See whether the given model identifier is used a weapon model.
*/
SQMOD_NODISCARD bool IsModelWeapon(int32_t id);

/* ------------------------------------------------------------------------------------------------
 * See whether the given model identifier is an actual weapon model.
*/
SQMOD_NODISCARD bool IsModelActuallyWeapon(int32_t id);

} // Namespace:: SqMod
