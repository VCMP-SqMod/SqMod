#ifndef _MISC_VEHICLE_HPP_
#define _MISC_VEHICLE_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CCStr GetAutomobileName(Uint32 id);
void SetAutomobileName(Uint32 id, CCStr name);
Int32 GetAutomobileID(CCStr name);
bool IsAutomobileValid(Int32 id);

} // Namespace:: SqMod

#endif // _MISC_VEHICLE_HPP_
