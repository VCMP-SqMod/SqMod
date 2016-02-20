#ifndef _MISC_WEAPON_HPP_
#define _MISC_WEAPON_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CCStr GetWeaponName(Uint32 id);
void SetWeaponName(Uint32 id, CCStr name);
Int32 GetWeaponID(CCStr name);
bool IsWeaponValid(Int32 id);
Int32 WeaponToModel(Int32 id);

} // Namespace:: SqMod

#endif // _MISC_WEAPON_HPP_
