#ifndef _MISC_PLAYER_HPP_
#define _MISC_PLAYER_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name associated with a skin model identifier.
*/
CCStr GetSkinName(Uint32 id);

/* ------------------------------------------------------------------------------------------------
 * Modify the name associated with a skin model identifier.
*/
void SetSkinName(Uint32 id, const StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * Convert a vehicle model name to a skin model identifier.
*/
Int32 GetSkinID(const StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified skin model identifier is valid.
*/
bool IsSkinValid(Int32 id);

} // Namespace:: SqMod

#endif // _MISC_PLAYER_HPP_
