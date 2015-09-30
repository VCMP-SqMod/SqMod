#ifndef _REGISTER_HPP_
#define _REGISTER_HPP_

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
using namespace Sqrat;

// ------------------------------------------------------------------------------------------------
bool RegisterAPI(HSQUIRRELVM vm) noexcept;

// ------------------------------------------------------------------------------------------------
bool Register_AABB(HSQUIRRELVM vm);
bool Register_Circle(HSQUIRRELVM vm);
bool Register_Color3(HSQUIRRELVM vm);
bool Register_Color4(HSQUIRRELVM vm);
bool Register_Quaternion(HSQUIRRELVM vm);
bool Register_Sphere(HSQUIRRELVM vm);
bool Register_Vector2f(HSQUIRRELVM vm);
bool Register_Vector2i(HSQUIRRELVM vm);
bool Register_Vector2u(HSQUIRRELVM vm);
bool Register_Vector3(HSQUIRRELVM vm);
bool Register_Vector4(HSQUIRRELVM vm);
bool Register_Base(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
bool Register_CBlip(HSQUIRRELVM vm);
bool Register_CCheckpoint(HSQUIRRELVM vm);
bool Register_CKeybind(HSQUIRRELVM vm);
bool Register_CObject(HSQUIRRELVM vm);
bool Register_CPickup(HSQUIRRELVM vm);
bool Register_CPlayer(HSQUIRRELVM vm);
bool Register_CSphere(HSQUIRRELVM vm);
bool Register_CSprite(HSQUIRRELVM vm);
bool Register_CTextdraw(HSQUIRRELVM vm);
bool Register_CVehicle(HSQUIRRELVM vm);
bool Register_Entity(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
bool Register_CFG(HSQUIRRELVM vm);
bool Register_Datetime(HSQUIRRELVM vm);
bool Register_FileIO(HSQUIRRELVM vm);
bool Register_Format(HSQUIRRELVM vm);
bool Register_INI(HSQUIRRELVM vm);
bool Register_JSON(HSQUIRRELVM vm);
bool Register_LongInt(HSQUIRRELVM vm);
bool Register_Math(HSQUIRRELVM vm);
bool Register_Numeric(HSQUIRRELVM vm);
bool Register_String(HSQUIRRELVM vm);
bool Register_SysPath(HSQUIRRELVM vm);
bool Register_System(HSQUIRRELVM vm);
bool Register_Timer(HSQUIRRELVM vm);
bool Register_Utils(HSQUIRRELVM vm);
bool Register_XML(HSQUIRRELVM vm);
bool Register_Library(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
bool Register_CAutomobile(HSQUIRRELVM vm);
bool Register_Constants(HSQUIRRELVM vm);
bool Register_Functions(HSQUIRRELVM vm);
bool Register_CModel(HSQUIRRELVM vm);
bool Register_CPlayerImmunity(HSQUIRRELVM vm);
bool Register_CRadio(HSQUIRRELVM vm);
bool Register_CSkin(HSQUIRRELVM vm);
bool Register_CSound(HSQUIRRELVM vm);
bool Register_CVehicleImmunity(HSQUIRRELVM vm);
bool Register_CWastedSettings(HSQUIRRELVM vm);
bool Register_CWeapon(HSQUIRRELVM vm);
bool Register_CWorldBounds(HSQUIRRELVM vm);
bool Register_Misc(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
bool Register_QBlip(HSQUIRRELVM vm);
bool Register_QCheckpoint(HSQUIRRELVM vm);
bool Register_QKeybind(HSQUIRRELVM vm);
bool Register_QObject(HSQUIRRELVM vm);
bool Register_QPickup(HSQUIRRELVM vm);
bool Register_QPlayer(HSQUIRRELVM vm);
bool Register_QSphere(HSQUIRRELVM vm);
bool Register_QSprite(HSQUIRRELVM vm);
bool Register_QTextdraw(HSQUIRRELVM vm);
bool Register_QVehicle(HSQUIRRELVM vm);
bool Register_Queue(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
bool Register_SBlip(HSQUIRRELVM vm);
bool Register_SCheckpoint(HSQUIRRELVM vm);
bool Register_SKeybind(HSQUIRRELVM vm);
bool Register_SObject(HSQUIRRELVM vm);
bool Register_SPickup(HSQUIRRELVM vm);
bool Register_SPlayer(HSQUIRRELVM vm);
bool Register_SSphere(HSQUIRRELVM vm);
bool Register_SSprite(HSQUIRRELVM vm);
bool Register_STextdraw(HSQUIRRELVM vm);
bool Register_Selector(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
bool Register_Utility(HSQUIRRELVM vm);

} // Namespace:: SqMod

#endif // _REGISTER_HPP_
