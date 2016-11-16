#ifndef _MISC_VEHICLE_HPP_
#define _MISC_VEHICLE_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name associated with a vehicle model identifier.
*/
CSStr GetAutomobileName(Uint32 id);

/* ------------------------------------------------------------------------------------------------
 * Modify the name associated with a vehicle model identifier.
*/
void SetAutomobileName(Uint32 id, const StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * Convert a vehicle model name to a vehicle model identifier.
*/
Int32 GetAutomobileID(const StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified vehicle model identifier is valid.
*/
bool IsAutomobileValid(Int32 id);

} // Namespace:: SqMod

#endif // _MISC_VEHICLE_HPP_
