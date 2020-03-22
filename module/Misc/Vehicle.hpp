#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the name associated with a vehicle model identifier.
*/
String GetAutomobileName(Uint32 id);

/* ------------------------------------------------------------------------------------------------
 * Modify the name associated with a vehicle model identifier.
*/
void SetAutomobileName(Uint32 id, StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * Convert a vehicle model name to a vehicle model identifier.
*/
Int32 GetAutomobileID(StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * See whether the specified vehicle model identifier is valid.
*/
bool IsAutomobileValid(Int32 id);

} // Namespace:: SqMod
