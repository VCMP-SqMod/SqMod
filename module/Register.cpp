#ifndef _REGISTER_HPP_
#define _REGISTER_HPP_

// ------------------------------------------------------------------------------------------------
#include <squirrelex.h>

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
extern void Register_Algo(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void Register_CBlip(HSQUIRRELVM vm);
extern void Register_CCheckpoint(HSQUIRRELVM vm);
extern void Register_CKeyBind(HSQUIRRELVM vm);
extern void Register_CObject(HSQUIRRELVM vm);
extern void Register_CPickup(HSQUIRRELVM vm);
extern void Register_CPlayer(HSQUIRRELVM vm);
extern void Register_CVehicle(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void Register_Chrono(HSQUIRRELVM vm);
extern void Register_CURL(HSQUIRRELVM vm);
extern void Register_Format(HSQUIRRELVM vm);
extern void Register_IO(HSQUIRRELVM vm);
extern void Register_JSON(HSQUIRRELVM vm);
extern void Register_MMDB(HSQUIRRELVM vm);
extern void Register_Net(HSQUIRRELVM vm);
extern void Register_Numeric(HSQUIRRELVM vm);
extern void Register_String(HSQUIRRELVM vm);
extern void Register_System(HSQUIRRELVM vm);
extern void Register_UTF8(HSQUIRRELVM vm);
extern void Register_Utils(HSQUIRRELVM vm);
extern void Register_XML(HSQUIRRELVM vm);
extern void Register_ZMQ(HSQUIRRELVM vm);
#ifdef SQMOD_POCO_HAS_SQLITE
    extern void Register_SQLite(HSQUIRRELVM vm);
#endif
#ifdef SQMOD_POCO_HAS_MYSQL
    extern void Register_MySQL(HSQUIRRELVM vm);
#endif

// ------------------------------------------------------------------------------------------------
extern void Register_POCO(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void Register_Constants(HSQUIRRELVM vm);
extern void Register_Log(HSQUIRRELVM vm);
extern void Register_Core(HSQUIRRELVM vm);
extern void Register_Command(HSQUIRRELVM vm);
extern void Register_Inventory(HSQUIRRELVM vm);
extern void Register_Loot(HSQUIRRELVM vm);
extern void Register_Privilege(HSQUIRRELVM vm);
extern void Register_Routine(HSQUIRRELVM vm);
extern void Register_Tasks(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void Register_Misc(HSQUIRRELVM vm);
extern void Register_Areas(HSQUIRRELVM vm);
extern void Register_Signal(HSQUIRRELVM vm);
#ifdef VCMP_ENABLE_OFFICIAL
    extern void Register_Official(HSQUIRRELVM vm);
#endif

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
    Register_Algo(vm);

    Register_CBlip(vm);
    Register_CCheckpoint(vm);
    Register_CKeyBind(vm);
    Register_CObject(vm);
    Register_CPickup(vm);
    Register_CPlayer(vm);
    Register_CVehicle(vm);

    Register_Chrono(vm);
    Register_CURL(vm);
    Register_Format(vm);
    Register_IO(vm);
    Register_JSON(vm);
    Register_MMDB(vm);
    Register_Net(vm);
    Register_Numeric(vm);
    Register_String(vm);
    Register_System(vm);
    Register_UTF8(vm);
    Register_Utils(vm);
    Register_XML(vm);
    Register_ZMQ(vm);
#ifdef SQMOD_POCO_HAS_SQLITE
    Register_SQLite(vm);
#endif
#ifdef SQMOD_POCO_HAS_MYSQL
    Register_MySQL(vm);
#endif

    Register_POCO(vm);

    Register_Constants(vm);
    Register_Log(vm);
    Register_Core(vm);
    Register_Command(vm);
    Register_Inventory(vm);
    Register_Loot(vm);
    Register_Privilege(vm);
    Register_Routine(vm);
    Register_Tasks(vm);

    Register_Misc(vm);
    Register_Areas(vm);
    Register_Signal(vm);
#ifdef VCMP_ENABLE_OFFICIAL
    Register_Official(vm);
#endif

    return true;
}

} // Namespace:: SqMod

#endif // _REGISTER_HPP_
