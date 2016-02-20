#ifndef _MISC_MODEL_HPP_
#define _MISC_MODEL_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CCStr GetModelName(Int32 id);
void SetModelName(Int32 id, CCStr name);
bool IsModelWeapon(Int32 id);
bool IsModelActuallyWeapon(Int32 id);
bool IsWeaponNatural(Int32 id);

} // Namespace:: SqMod

#endif // _MISC_MODEL_HPP_
