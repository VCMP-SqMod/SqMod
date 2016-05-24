#ifndef _REGISTER_HPP_
#define _REGISTER_HPP_

// ------------------------------------------------------------------------------------------------
#include <squirrel.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void Register_AABB(HSQUIRRELVM vm);
extern void Register_Circle(HSQUIRRELVM vm);
extern void Register_Color3(HSQUIRRELVM vm);
extern void Register_Color4(HSQUIRRELVM vm);
extern void Register_Quaternion(HSQUIRRELVM vm);
extern void Register_Sphere(HSQUIRRELVM vm);
extern void Register_Vector2(HSQUIRRELVM vm);
extern void Register_Vector2i(HSQUIRRELVM vm);
extern void Register_Vector3(HSQUIRRELVM vm);
extern void Register_Vector4(HSQUIRRELVM vm);
extern void Register_Base(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void Register_CBlip(HSQUIRRELVM vm);
extern void Register_CCheckpoint(HSQUIRRELVM vm);
extern void Register_CForcefield(HSQUIRRELVM vm);
extern void Register_CKeybind(HSQUIRRELVM vm);
extern void Register_CObject(HSQUIRRELVM vm);
extern void Register_CPickup(HSQUIRRELVM vm);
extern void Register_CPlayer(HSQUIRRELVM vm);
extern void Register_CSprite(HSQUIRRELVM vm);
extern void Register_CTextdraw(HSQUIRRELVM vm);
extern void Register_CVehicle(HSQUIRRELVM vm);
extern void Register_Entity(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void Register_Chrono(HSQUIRRELVM vm);
extern void Register_Crypt(HSQUIRRELVM vm);
extern void Register_IO(HSQUIRRELVM vm);
extern void Register_Numeric(HSQUIRRELVM vm);
extern void Register_Math(HSQUIRRELVM vm);
extern void Register_Random(HSQUIRRELVM vm);
extern void Register_String(HSQUIRRELVM vm);
extern void Register_System(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void Register_Constants(HSQUIRRELVM vm);
extern void Register_Log(HSQUIRRELVM vm);
extern void Register_Core(HSQUIRRELVM vm);
extern void Register_Command(HSQUIRRELVM vm);
extern void Register_Routine(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void Register_Misc(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
bool RegisterAPI(HSQUIRRELVM vm)
{
    Register_AABB(vm);
    Register_Circle(vm);
    Register_Color3(vm);
    Register_Color4(vm);
    Register_Quaternion(vm);
    Register_Sphere(vm);
    Register_Vector2(vm);
    Register_Vector2i(vm);
    Register_Vector3(vm);
    Register_Vector4(vm);
    Register_Base(vm);

    Register_CBlip(vm);
    Register_CCheckpoint(vm);
    Register_CKeybind(vm);
    Register_CObject(vm);
    Register_CPickup(vm);
    Register_CPlayer(vm);
    Register_CVehicle(vm);

    Register_Chrono(vm);
    Register_Crypt(vm);
    Register_IO(vm);
    Register_Random(vm);
    Register_Numeric(vm);
    Register_Math(vm);
    Register_String(vm);
    Register_System(vm);

    Register_Constants(vm);
    Register_Log(vm);
    Register_Core(vm);
    Register_Command(vm);
    Register_Routine(vm);

    Register_Misc(vm);

    return true;
}

} // Namespace:: SqMod

#endif // _REGISTER_HPP_
