#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the slot associated with a weapon identifier.
*/
SQMOD_NODISCARD uint32_t GetWeaponSlot(uint32_t id);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name associated with a weapon identifier.
*/
SQMOD_NODISCARD const SQChar * GetWeaponName(uint32_t id);

/* ------------------------------------------------------------------------------------------------
 * Modify the name associated with a weapon identifier.
*/
void SetWeaponName(uint32_t id, StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the total number of identifiers in the pool of custom weapon names.
*/
SQMOD_NODISCARD uint32_t GetCustomWeaponNamePoolSize();

/* ------------------------------------------------------------------------------------------------
 * Clear all identifiersand associated names from the pool of custom weapon names.
*/
void ClearCustomWeaponNamePool();

/* ------------------------------------------------------------------------------------------------
 * Convert a weapon name to a weapon identifier.
*/
SQMOD_NODISCARD int32_t GetWeaponID(StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified weapon identifier is valid.
*/
bool IsWeaponValid(int32_t id);

/* ------------------------------------------------------------------------------------------------
 * Convert the given weapon identifier to it's associated model identifier.
*/
SQMOD_NODISCARD int32_t WeaponToModel(int32_t id);

/* ------------------------------------------------------------------------------------------------
 * See whether the given weapon identifier cannot be used by another player to inflict damage.
*/
SQMOD_NODISCARD bool IsWeaponNatural(int32_t id);

} // Namespace:: SqMod
