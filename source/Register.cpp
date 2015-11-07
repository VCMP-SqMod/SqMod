#include "Register.hpp"
#include "Logger.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
/* Snippet for registering a class in a single statement.

    Sqrat::RootTable(vm).Bind(_SC("CLSNAME"), Sqrat::Class<CLSNAME>(vm, _SC("CLSNAME"))

    );
*/

// ------------------------------------------------------------------------------------------------
bool RegisterAPI(HSQUIRRELVM vm)
{
    if (_Log->cFtl(!Register_AABB(vm), "Unable to register: AABB") || \
        _Log->cFtl(!Register_Circle(vm), "Unable to register: Circle") || \
        _Log->cFtl(!Register_Color3(vm), "Unable to register: Color3") || \
        _Log->cFtl(!Register_Color4(vm), "Unable to register: Color4") || \
        _Log->cFtl(!Register_Quaternion(vm), "Unable to register: Quaternion") || \
        _Log->cFtl(!Register_Sphere(vm), "Unable to register: Sphere") || \
        _Log->cFtl(!Register_Vector2f(vm), "Unable to register: Vector2f") || \
        _Log->cFtl(!Register_Vector2i(vm), "Unable to register: Vector2i") || \
        _Log->cFtl(!Register_Vector2u(vm), "Unable to register: Vector2u") || \
        _Log->cFtl(!Register_Vector3(vm), "Unable to register: Vector3") || \
        _Log->cFtl(!Register_Vector4(vm), "Unable to register: Vector4") || \
        _Log->cFtl(!Register_Base(vm), "Unable to register: Base") || \

        _Log->cFtl(!Register_CBlip(vm), "Unable to register: Blip") || \
        _Log->cFtl(!Register_CCheckpoint(vm), "Unable to register: Checkpoint") || \
        _Log->cFtl(!Register_CKeybind(vm), "Unable to register: Keybind") || \
        _Log->cFtl(!Register_CObject(vm), "Unable to register: Object") || \
        _Log->cFtl(!Register_CPickup(vm), "Unable to register: Pickup") || \
        _Log->cFtl(!Register_CPlayer(vm), "Unable to register: Player") || \
        _Log->cFtl(!Register_CSphere(vm), "Unable to register: Sphere") || \
        _Log->cFtl(!Register_CSprite(vm), "Unable to register: Sprite") || \
        _Log->cFtl(!Register_CTextdraw(vm), "Unable to register: Textdraw") || \
        _Log->cFtl(!Register_CVehicle(vm), "Unable to register: Vehicle") || \
        _Log->cFtl(!Register_Entity(vm), "Unable to register: Entity") || \

        _Log->cFtl(!Register_Datetime(vm), "Unable to register: Datetime") || \
        _Log->cFtl(!Register_FileIO(vm), "Unable to register: FileIO") || \
        _Log->cFtl(!Register_Format(vm), "Unable to register: Format") || \
        _Log->cFtl(!Register_Hash(vm), "Unable to register: Hash") || \
        _Log->cFtl(!Register_IRC(vm), "Unable to register: IRC") || \
        _Log->cFtl(!Register_INI(vm), "Unable to register: INI") || \
        _Log->cFtl(!Register_JSON(vm), "Unable to register: JSON") || \
        _Log->cFtl(!Register_LongInt(vm), "Unable to register: LongInt") || \
        _Log->cFtl(!Register_Math(vm), "Unable to register: Math") || \
        _Log->cFtl(!Register_Numeric(vm), "Unable to register: Numeric") || \
        _Log->cFtl(!Register_SQLite(vm), "Unable to register: SQLite") || \
        _Log->cFtl(!Register_String(vm), "Unable to register: String") || \
        _Log->cFtl(!Register_SysPath(vm), "Unable to register: SysPath") || \
        _Log->cFtl(!Register_System(vm), "Unable to register: System") || \
        _Log->cFtl(!Register_Timer(vm), "Unable to register: Timer") || \
        _Log->cFtl(!Register_Utils(vm), "Unable to register: Utils") || \
        _Log->cFtl(!Register_XML(vm), "Unable to register: XML") || \
        _Log->cFtl(!Register_Library(vm), "Unable to register: Library") || \

        _Log->cFtl(!Register_CAutomobile(vm), "Unable to register: CAutomobile") || \
        _Log->cFtl(!Register_Constants(vm), "Unable to register: Functions") || \
        _Log->cFtl(!Register_Functions(vm), "Unable to register: Functions") || \
        _Log->cFtl(!Register_CModel(vm), "Unable to register: CModel") || \
        _Log->cFtl(!Register_CPlayerImmunity(vm), "Unable to register: CPlayerImmunity") || \
        _Log->cFtl(!Register_CRadio(vm), "Unable to register: CRadio") || \
        _Log->cFtl(!Register_CSkin(vm), "Unable to register: CSkin") || \
        _Log->cFtl(!Register_CSound(vm), "Unable to register: CSound") || \
        _Log->cFtl(!Register_CVehicleImmunity(vm), "Unable to register: CVehicleImmunity") || \
        _Log->cFtl(!Register_CWastedSettings(vm), "Unable to register: CWastedSettings") || \
        _Log->cFtl(!Register_CWeapon(vm), "Unable to register: CWeapon") || \
        _Log->cFtl(!Register_CWorldBounds(vm), "Unable to register: CWorldBounds") || \
        _Log->cFtl(!Register_Misc(vm), "Unable to register: Misc") || \

        _Log->cFtl(!Register_BasicEvent(vm), "Unable to register: BasicEvent") || \
        _Log->cFtl(!Register_GlobalEvent(vm), "Unable to register: GlobalEvent") || \
        _Log->cFtl(!Register_LocalEvent(vm), "Unable to register: LocalEvent") || \
        _Log->cFtl(!Register_Event(vm), "Unable to register: Event") || \

        _Log->cFtl(!Register_Cmd(vm), "Unable to register: Command")
    ) return false;

    return true;
}

} // Namespace:: SqMod
