// ------------------------------------------------------------------------------------------------
#include "Misc/Official.hpp"
#include "Base/Vector2.hpp"
#include "Core/Utility.hpp"
#include "Library/IO/Buffer.hpp"
// ------------------------------------------------------------------------------------------------
#include "Core/Entity.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include "Misc/Functions.hpp"
#include "Misc/Player.hpp"
#include "Misc/Vehicle.hpp"
#include "Misc/Weapon.hpp"

// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Entity/Checkpoint.hpp"
#include "Entity/KeyBind.hpp"
#include "Entity/Object.hpp"
#include "Entity/Pickup.hpp"
#include "Entity/Player.hpp"
#include "Entity/Vehicle.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
#include <algorithm>
#include <sstream>
#include <cctype>
#include <ctime>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(RGBTn, _SC("RGB"))
SQMOD_DECL_TYPENAME(EntityRGBTn, _SC("EntityRGB"))
SQMOD_DECL_TYPENAME(RGBATn, _SC("RGBA"))
SQMOD_DECL_TYPENAME(ARGBTn, _SC("ARGB"))
SQMOD_DECL_TYPENAME(VectorTn, _SC("Vector"))
SQMOD_DECL_TYPENAME(EntityVectorTn, _SC("EntityVector"))
SQMOD_DECL_TYPENAME(EntityQuaternionTn, _SC("EntityQuaternion"))
SQMOD_DECL_TYPENAME(BoundsTn, _SC("Bounds"))
SQMOD_DECL_TYPENAME(WastedSettingsTn, _SC("WastedSettings"))

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(CCheckpointTn, _SC("CCheckpoint"))
SQMOD_DECL_TYPENAME(CObjectTn, _SC("CObject"))
SQMOD_DECL_TYPENAME(CPickupTn, _SC("CPickup"))
SQMOD_DECL_TYPENAME(CPlayerTn, _SC("CPlayer"))
SQMOD_DECL_TYPENAME(CVehicleTn, _SC("CVehicle"))

/* ------------------------------------------------------------------------------------------------
 * Entity type enumeration.
*/
struct LgEntityType
{
    enum Type
    {
        Player = 0,
        Vehicle,
        Pickup,
        Object,
        Checkpoint,
        Unknown
    };
};

/* ------------------------------------------------------------------------------------------------
 * Player vector flag enumeration.
*/
struct LgPlayerVectorFlag
{
    enum Type
    {
        Pos = 0,
        Speed,
        Unknown
    };
};

/* ------------------------------------------------------------------------------------------------
 * Vehicle vector flag enumeration.
*/
struct LgVehicleVectorFlag
{
    enum Type
    {
        Pos = 0,
        SpawnPos,
        Angle,
        SpawnAngle,
        Speed,
        RelSpeed,
        TurnSpeed,
        RelTurnSpeed,
        Unknown
    };
};

/* ------------------------------------------------------------------------------------------------
 * Object vector flag enumeration.
*/
struct LgObjectVectorFlag
{
    enum Type
    {
        Pos = 0,
        Rotation,
        Unknown
    };
};

/* ------------------------------------------------------------------------------------------------
 * Vehicle quaternion flag enumeration.
*/
struct LgVehicleQuaternionFlag
{
    enum Type
    {
        Angle = 0,
        SpawnAngle,
        Unknown
    };
};

// ------------------------------------------------------------------------------------------------
static SQInteger VectorToString(HSQUIRRELVM vm)
{
    try
    {
        Var< LgVector * > v(SqVM(), 1);
        // Validate the instance, just to be sure
        if (!v.value)
        {
            return sq_throwerror(vm, "Invalid Vector instance");
        }
        // Generate the formatted string
        auto s = fmt::format("({}, {}, {})", v.value->x, v.value->y, v.value->z);
        // Push it on the stack
        sq_pushstring(vm, s.c_str(), static_cast< SQInteger >(s.size()));
        // Specify that we have a value to return
        return 1;
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    SQ_UNREACHABLE
}

// ------------------------------------------------------------------------------------------------
static SQInteger QuaternionToString(HSQUIRRELVM vm)
{
    try
    {
        Var< Quaternion * > v(SqVM(), 1);
        // Validate the instance, just to be sure
        if (!v.value)
        {
            return sq_throwerror(vm, "Invalid Quaternion instance");
        }
        // Generate the formatted string
        auto s = fmt::format("({}, {}, {}, {})", v.value->x, v.value->y, v.value->z, v.value->w);
        // Push it on the stack
        sq_pushstring(vm, s.c_str(), static_cast< SQInteger >(s.size()));
        // Specify that we have a value to return
        return 1;
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    SQ_UNREACHABLE
}

// ------------------------------------------------------------------------------------------------
void LgEntityVector::Set()
{
    switch (mType)
    {
        case LgEntityType::Player:
            switch (mFlag)
            {
                case LgPlayerVectorFlag::Pos:
                    _Func->SetPlayerPosition(mID, x, y, z);
                break;
                case LgPlayerVectorFlag::Speed:
                    _Func->SetPlayerSpeed(mID, x, y, z);
                break;
                default: break;
            } break;
        case LgEntityType::Vehicle:
            switch (mFlag)
            {
                case LgVehicleVectorFlag::Pos:
                    _Func->SetVehiclePosition(mID, x, y, z, 0);
                break;
                case LgVehicleVectorFlag::SpawnPos:
                    _Func->SetVehicleSpawnPosition(mID, x, y, z);
                break;
                case LgVehicleVectorFlag::Angle:
                    _Func->SetVehicleRotationEuler(mID, x, y, z);
                break;
                case LgVehicleVectorFlag::SpawnAngle:
                    _Func->SetVehicleSpawnRotationEuler(mID, x, y, z);
                break;
                case LgVehicleVectorFlag::Speed:
                    _Func->SetVehicleSpeed(mID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(false));
                break;
                case LgVehicleVectorFlag::RelSpeed:
                    _Func->SetVehicleSpeed(mID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(true));
                break;
                case LgVehicleVectorFlag::TurnSpeed:
                    _Func->SetVehicleTurnSpeed(mID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(false));
                break;
                case LgVehicleVectorFlag::RelTurnSpeed:
                    _Func->SetVehicleTurnSpeed(mID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(true));
                break;
                default: break;
            } break;
        case LgEntityType::Pickup:
            _Func->SetPickupPosition(mID, x, y, z);
        break;
        case LgEntityType::Object:
            switch (mFlag)
            {
                case LgObjectVectorFlag::Pos:
                    _Func->SetObjectPosition(mID, x, y, z);
                break;
                case LgObjectVectorFlag::Rotation:
                    _Func->RotateObjectToEuler(mID, x, y, z, 0);
                break;
                default: break;
            } break;
        case LgEntityType::Checkpoint:
            _Func->SetCheckPointPosition(mID, x, y, z);
        break;
        default: break;
    }
}

// ------------------------------------------------------------------------------------------------
void LgEntityQuaternion::Set()
{
    switch (mType)
    {
        case LgEntityType::Vehicle:
            switch (mFlag)
            {
                case LgVehicleVectorFlag::Angle:
                    _Func->SetVehicleRotation(mID, x, y, z, w);
                break;
                case LgVehicleVectorFlag::SpawnAngle:
                    _Func->SetVehicleSpawnRotation(mID, x, y, z, w);
                break;
                default: break;
            } break;
        case LgEntityType::Object:
            _Func->RotateObjectTo(mID, x, y, z, w, 0);
        break;
        default: break;
    }
}

// ------------------------------------------------------------------------------------------------
void LgEntityRGB::Set()
{
    if (mType == LgEntityType::Player)
    {
        _Func->SetPlayerColour(mID, GetRGB());
    }
}

// ------------------------------------------------------------------------------------------------
void Register_Official_Entity(HSQUIRRELVM vm);
void Register_Official_Functions(HSQUIRRELVM vm);
void Register_Official_Constants(HSQUIRRELVM vm);
void Register_Official_Stream(HSQUIRRELVM vm);

// ================================================================================================
void Register_Official(HSQUIRRELVM vm)
{
    // See if official support is allowed
    if (!Core::Get().IsOfficial())
    {
        return; // Do nothing
    }
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(VectorTn::Str,
        DerivedClass< LgVector, Vector3 >(vm, VectorTn::Str)
        // Constructors
        .Ctor< Vector3::Value, Vector3::Value, Vector3::Value >()
        // Member Methods
        .Func(_SC("Distance"), &LgVector::GetDistanceTo)
        // Global Member Methods
        .SquirrelFunc(_SC("_tostring"), &VectorToString)
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(EntityVectorTn::Str,
        DerivedClass< LgEntityVector, LgVector >(vm, EntityVectorTn::Str)
        // Constructors
        .Ctor< int, int, int, Vector3::Value, Vector3::Value, Vector3::Value >()
        // Properties
        .Prop(_SC("x"), &LgEntityVector::GetX, &LgEntityVector::SetX)
        .Prop(_SC("y"), &LgEntityVector::GetY, &LgEntityVector::SetY)
        .Prop(_SC("z"), &LgEntityVector::GetZ, &LgEntityVector::SetZ)
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(EntityQuaternionTn::Str,
        DerivedClass< LgEntityQuaternion, Quaternion >(vm, EntityQuaternionTn::Str)
        // Constructors
        .Ctor< int, int, int, Quaternion::Value, Quaternion::Value, Quaternion::Value, Quaternion::Value >()
        // Properties
        .Prop(_SC("x"), &LgEntityQuaternion::GetX, &LgEntityQuaternion::SetX)
        .Prop(_SC("y"), &LgEntityQuaternion::GetY, &LgEntityQuaternion::SetY)
        .Prop(_SC("z"), &LgEntityQuaternion::GetZ, &LgEntityQuaternion::SetZ)
        .Prop(_SC("w"), &LgEntityQuaternion::GetW, &LgEntityQuaternion::SetW)
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(RGBTn::Str,
        DerivedClass< LgRGB, Color3 >(vm, RGBTn::Str)
        // Constructors
        .Ctor< LgRGB::Value, LgRGB::Value, LgRGB::Value >()
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(RGBATn::Str,
        DerivedClass< LgRGBA, Color4 >(vm, RGBATn::Str)
        // Constructors
        .Ctor< LgRGBA::Value, LgRGBA::Value, LgRGBA::Value >()
        .Ctor< LgRGBA::Value, LgRGBA::Value, LgRGBA::Value, LgRGBA::Value >()
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(ARGBTn::Str,
        DerivedClass< LgARGB, Color4 >(vm, ARGBTn::Str)
        // Constructors
        .Ctor< LgARGB::Value, LgARGB::Value, LgARGB::Value >()
        .Ctor< LgARGB::Value, LgARGB::Value, LgARGB::Value, LgARGB::Value >()
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(EntityRGBTn::Str,
        DerivedClass< LgEntityRGB, Color3 >(vm, EntityRGBTn::Str)
        // Constructors
        .Ctor< int, int, int, Color3::Value, Color3::Value, Color3::Value >()
        // Properties
        .Prop(_SC("r"), &LgEntityRGB::GetR, &LgEntityRGB::SetR)
        .Prop(_SC("g"), &LgEntityRGB::GetG, &LgEntityRGB::SetG)
        .Prop(_SC("b"), &LgEntityRGB::GetB, &LgEntityRGB::SetB)
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(BoundsTn::Str,
        Class< LgBounds >(vm, BoundsTn::Str)
        // Constructors
        .Ctor< LgBounds::Value, LgBounds::Value, LgBounds::Value, LgBounds::Value >()
        // Member Variables
        .Var(_SC("MaxX"), &LgBounds::max_x)
        .Var(_SC("MinX"), &LgBounds::min_x)
        .Var(_SC("MaxY"), &LgBounds::max_y)
        .Var(_SC("MinY"), &LgBounds::min_y)
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(WastedSettingsTn::Str,
        Class< LgWastedSettings >(vm, WastedSettingsTn::Str)
        // Constructors
        .Ctor< uint32_t, uint32_t, float, float, const Color3 &, uint32_t, uint32_t >()
        // Member Variables
        .Var(_SC("DeathTime"), &LgWastedSettings::mDeathTime)
        .Var(_SC("FadeTime"), &LgWastedSettings::mFadeTime)
        .Var(_SC("FadeInSpeed"), &LgWastedSettings::mFadeInSpeed)
        .Var(_SC("FadeOutSpeed"), &LgWastedSettings::mFadeOutSpeed)
        .Var(_SC("FadeColour"), &LgWastedSettings::mFadeColour)
        .Var(_SC("CorpseFadeDelay"), &LgWastedSettings::mCorpseFadeDelay)
        .Var(_SC("CorpseFadeTime"), &LgWastedSettings::mCorpseFadeTime)
    );
    // --------------------------------------------------------------------------------------------
    Register_Official_Entity(vm);
    Register_Official_Functions(vm);
    Register_Official_Constants(vm);
    Register_Official_Stream(vm);
}

/* ------------------------------------------------------------------------------------------------
 * Checkpoint entity proxy.
*/
struct LgCheckpoint
{
    int32_t mID;
    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit LgCheckpoint(int32_t id)
        : mID(id)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the a valid entity identifier, otherwise throw an exception.
    */
    SQMOD_NODISCARD int32_t GetIdentifier() const
    {
        // Validate the associated identifier
        if (INVALID_ENTITY(mID))
        {
            STHROWF("Invalid checkpoint reference");
        }
        // Return it
        return mID;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the native implementation of the associated entity, otherwise throw an exception.
    */
    SQMOD_NODISCARD CCheckpoint & Get() const
    {
        // Retrieve the associated instance
        CCheckpoint * p = Core::Get().GetCheckpoint(GetIdentifier()).mInst;
        // Validate the associated instance
        if (!p)
        {
            STHROWF("Invalid checkpoint instance");
        }
        // This is valid so we can return it
        return *p;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve entity instance which is native to the plug-in.
    */
    SQMOD_NODISCARD LightObj & GetNative() const
    {
        return Core::Get().GetCheckpoint(GetIdentifier()).mObj;
    }
    // --------------------------------------------------------------------------------------------
    void SetWorld(int world) const { Get().SetWorld(world); }
    void SetColor(const Color4 & c) const { Get().SetColorEx4(c.r, c.g, c.b, c.a); }
    void SetPos(const Vector3 & pos) const { Get().SetPosition(pos); }
    void SetRadius(float radius) const { Get().SetRadius(radius); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD int GetWorld() const { return Get().GetWorld(); }
    SQMOD_NODISCARD LgARGB GetColor() const { const Color4 c = Get().GetColor(); return LgARGB{c.a, c.r, c.g, c.b}; }
    SQMOD_NODISCARD LgEntityVector GetPos() const
    { return LgEntityVector(mID, LgEntityType::Checkpoint, 0, Get().GetPosition()); }
    SQMOD_NODISCARD float GetRadius() const { return Get().GetRadius(); }
    SQMOD_NODISCARD int GetID() const { return mID; }
    SQMOD_NODISCARD LgPlayer * GetOwner() const
    { const int id = Get().GetOwnerID(); return VALID_ENTITYEX(id, SQMOD_PLAYER_POOL) ? Core::Get().GetPlayer(id).mLgInst : nullptr; }
    // --------------------------------------------------------------------------------------------
    void Delete() const { _Func->DeleteCheckPoint(GetIdentifier()); }
    SQMOD_NODISCARD bool StreamedToPlayer(LgPlayer & player) const;
};

/* ------------------------------------------------------------------------------------------------
 * Object entity proxy.
*/
struct LgObject
{
    int32_t mID;
    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit LgObject(int32_t id)
        : mID(id)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the a valid entity identifier, otherwise throw an exception.
    */
    SQMOD_NODISCARD int32_t GetIdentifier() const
    {
        // Validate the associated identifier
        if (INVALID_ENTITY(mID))
        {
            STHROWF("Invalid object reference");
        }
        // Return it
        return mID;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the native implementation of the associated entity, otherwise throw an exception.
    */
    SQMOD_NODISCARD CObject & Get() const
    {
        // Retrieve the associated instance
        CObject * p = Core::Get().GetObj(GetIdentifier()).mInst;
        // Validate the associated instance
        if (!p)
        {
            STHROWF("Invalid object instance");
        }
        // This is valid so we can return it
        return *p;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve entity instance which is native to the plug-in.
    */
    SQMOD_NODISCARD LightObj & GetNative() const
    {
        return Core::Get().GetObj(GetIdentifier()).mObj;
    }
    // --------------------------------------------------------------------------------------------
    void SetWorld(int world) const { Get().SetWorld(world); }
    void SetPos(const Vector3 & pos) const { Get().SetPosition(pos); }
    void SetReportingShots(bool toggle) const { Get().SetShotReport(toggle); }
    void SetReportingBumps(bool toggle) const { Get().SetTouchedReport(toggle); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD int GetModel() const { return Get().GetModel(); }
    SQMOD_NODISCARD int GetAlpha() const { return Get().GetAlpha(); }
    SQMOD_NODISCARD int GetWorld() const { return Get().GetWorld(); }
    SQMOD_NODISCARD LgEntityVector GetPos() const
    { return LgEntityVector(mID, LgEntityType::Object, LgObjectVectorFlag::Pos, Get().GetPosition()); }
    SQMOD_NODISCARD LgEntityQuaternion GetRotation() const
    { return LgEntityQuaternion(mID, LgEntityType::Object, 0, Get().GetRotation()); }
    SQMOD_NODISCARD LgEntityVector GetRotationEuler() const
    { return LgEntityVector(mID, LgEntityType::Object, LgObjectVectorFlag::Rotation, Get().GetRotationEuler()); }
    SQMOD_NODISCARD int GetID() const { return mID; }
    SQMOD_NODISCARD bool GetReportingShots() const { return Get().GetShotReport(); }
    SQMOD_NODISCARD bool GetReportingBumps() const { return Get().GetTouchedReport(); }
    // --------------------------------------------------------------------------------------------
    void Delete() const { _Func->DeleteObject(GetIdentifier()); }
    void MoveTo(const Vector3 & pos, int time) const { Get().MoveTo(pos, static_cast< uint32_t >(time)); }
    void MoveBy(const Vector3 & offset, int time) const { Get().MoveBy(offset, static_cast< uint32_t >(time)); }
    void RotateTo(const Quaternion & rotation, int time) const { Get().RotateTo(rotation, static_cast< uint32_t >(time)); }
    void RotateBy(const Quaternion & rotOffset, int time) const { Get().RotateBy(rotOffset, static_cast< uint32_t >(time)); }
    void RotateToEuler(const Vector3 & rotation, int time) const { Get().RotateToEuler(rotation, static_cast< uint32_t >(time)); }
    void RotateByEuler(const Vector3 & rotOffset, int time) const { Get().RotateByEuler(rotOffset, static_cast< uint32_t >(time)); }
    void SetAlpha(int alpha, int fadeTime) const { Get().SetAlphaEx(alpha, static_cast< uint32_t >(fadeTime)); }
    SQMOD_NODISCARD bool StreamedToPlayer(LgPlayer & player) const;    
};

/* ------------------------------------------------------------------------------------------------
 * Pickup entity proxy.
*/
struct LgPickup
{
    int32_t mID;
    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit LgPickup(int32_t id)
        : mID(id)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the a valid entity identifier, otherwise throw an exception.
    */
    SQMOD_NODISCARD int32_t GetIdentifier() const
    {
        // Validate the associated identifier
        if (INVALID_ENTITY(mID))
        {
            STHROWF("Invalid pickup reference");
        }
        // Return it
        return mID;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the native implementation of the associated entity, otherwise throw an exception.
    */
    SQMOD_NODISCARD CPickup & Get() const
    {
        // Retrieve the associated instance
        CPickup * p = Core::Get().GetPickup(GetIdentifier()).mInst;
        // Validate the associated instance
        if (!p)
        {
            STHROWF("Invalid pickup instance");
        }
        // This is valid so we can return it
        return *p;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve entity instance which is native to the plug-in.
    */
    SQMOD_NODISCARD LightObj & GetNative() const
    {
        return Core::Get().GetPickup(GetIdentifier()).mObj;
    }
    // --------------------------------------------------------------------------------------------
    void SetWorld(int world) const { Get().SetWorld(world); }
    void SetAlpha(int alpha) const { Get().SetAlpha(alpha); }
    void SetAuto(bool toggle) const { Get().SetAutomatic(toggle); }
    void SetAutoTimer(int timer) const { Get().SetAutoTimer(timer); }
    void SetPos(const Vector3 & pos) const { Get().SetPosition(pos); }
    void SetSingleUse(bool toggle) const { _Func->SetPickupOption(GetIdentifier(), vcmpPickupOptionSingleUse, static_cast< uint8_t >(toggle)); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD int GetWorld() const { return Get().GetWorld(); }
    SQMOD_NODISCARD int GetAlpha() const { return Get().GetAlpha(); }
    SQMOD_NODISCARD bool GetAuto() const { return Get().GetAutomatic(); }
    SQMOD_NODISCARD int GetAutoTimer() const { return Get().GetAutoTimer(); }
    SQMOD_NODISCARD LgEntityVector GetPos() const
    { return LgEntityVector(mID, LgEntityType::Pickup, 0, Get().GetPosition()); }
    SQMOD_NODISCARD int GetModel() const { return Get().GetModel(); }
    SQMOD_NODISCARD int GetQuantity() const { return Get().GetQuantity(); }
    SQMOD_NODISCARD int GetID() const { return mID; }
    SQMOD_NODISCARD bool GetSingleUse() const { return _Func->GetPickupOption(GetIdentifier(), vcmpPickupOptionSingleUse) >= 1; }
    // --------------------------------------------------------------------------------------------
    void Delete() const { _Func->DeletePickup(GetIdentifier()); }
    void Respawn() const { Get().Refresh(); }
    SQMOD_NODISCARD bool StreamedToPlayer(LgPlayer & player) const;
};

/* ------------------------------------------------------------------------------------------------
 * Player entity proxy.
*/
struct LgPlayer
{
    int32_t mID;
    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit LgPlayer(int32_t id)
        : mID(id)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the a valid entity identifier, otherwise throw an exception.
    */
    SQMOD_NODISCARD int32_t GetIdentifier() const
    {
        // Validate the associated identifier
        if (INVALID_ENTITY(mID))
        {
            STHROWF("Invalid player reference");
        }
        // Return it
        return mID;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the native implementation of the associated entity, otherwise throw an exception.
    */
    SQMOD_NODISCARD CPlayer & Get() const
    {
        // Retrieve the associated instance
        CPlayer * p = Core::Get().GetPlayer(GetIdentifier()).mInst;
        // Validate the associated instance
        if (!p)
        {
            STHROWF("Invalid player instance");
        }
        // This is valid so we can return it
        return *p;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve entity instance which is native to the plug-in.
    */
    SQMOD_NODISCARD LightObj & GetNative() const
    {
        return Core::Get().GetPlayer(GetIdentifier()).mObj;
    }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD const SQChar * ToString() const { return INVALID_ENTITY(mID) ? Get().GetName() : _SC(""); }
    // --------------------------------------------------------------------------------------------
    void SetName(StackStrF & name) const { Get().SetName(name); }
    void SetPosition(const Vector3 & pos) const { Get().SetPosition(pos); }
    void SetHealth(float health) const { Get().SetHealth(health); }
    void SetArmour(float armour) const { Get().SetArmor(armour); }
    void SetAdmin(bool toggle) const { Get().SetAdmin(toggle); }
    void SetWorld(int world) const { Get().SetWorld(world); }
    void SetSecWorld(int world) const { Get().SetSecondaryWorld(world); }
    void SetTeam(int team) const { Get().SetTeam(team); }
    void SetSkin(int skin) const { Get().SetSkin(skin); }
    void SetColour(const Color3 & colour) const { Get().SetColor(colour); }
    void SetMoney(int money) const { Get().SetMoney(money); }
    void SetScore(int score) const { Get().SetScore(score); }
    void SetImmunity(uint32_t immunity) const { Get().SetImmunity(immunity); }
    void SetHeading(float heading) const { Get().SetHeading(heading); }
    void SetVehicle(LgVehicle & vehicle) const;
    void SetFrozen(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionControllable, static_cast< uint8_t >(!toggle)); }
    void SetDriveByEnabled(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionDriveBy, static_cast< uint8_t >(toggle)); }
    void SetWhiteScanLines(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionWhiteScanlines, static_cast< uint8_t >(toggle)); }
    void SetGreenScanLines(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionGreenScanlines, static_cast< uint8_t >(toggle)); }
    void SetWidescreen(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionWidescreen, static_cast< uint8_t >(toggle)); }
    void SetOnRadar(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionHasMarker, static_cast< uint8_t >(toggle)); }
    void SetCanAttack(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionCanAttack, static_cast< uint8_t >(toggle)); }
    void SetWeaponSlot(int slot) const { Get().SetWeaponSlot(slot); }
    void ShowMarkers(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionShowMarkers, static_cast< uint8_t >(toggle)); }
    void SetSpectateTarget(LightObj & player) const { player.IsNull() ? Get().SetSpectatorID(-1) : Get().SetSpectator(player.CastI< LgPlayer >()->Get()); }
    void SetMarkerVisible(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionHasMarker, static_cast< uint8_t >(toggle)); }
    void SetCanUseColors(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionChatTagsEnabled, static_cast< uint8_t >(toggle)); }
    void SetDrunkStatus(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionDrunkEffects, static_cast< uint8_t >(toggle)); }
    void SetWantedLevel(int level) const { Get().SetWantedLevel(level); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD LgEntityVector GetPosition() const
    { return LgEntityVector(mID, LgEntityType::Player, LgPlayerVectorFlag::Pos, Get().GetPosition()); }
    SQMOD_NODISCARD int GetClass() const { return Get().GetClass(); }
    SQMOD_NODISCARD bool GetAdmin() const { return Get().GetAdmin(); }
    SQMOD_NODISCARD const SQChar * GetIP() const { return Get().GetIP(); }
    SQMOD_NODISCARD bool GetSpawned() const { return Get().IsSpawned(); }
    SQMOD_NODISCARD int GetWorld() const { return Get().GetWorld(); }
    SQMOD_NODISCARD int GetSecWorld() const { return Get().GetSecondaryWorld(); }
    SQMOD_NODISCARD int GetUniqueWorld() const { return Get().GetUniqueWorld(); }
    SQMOD_NODISCARD int GetState() const { return Get().GetState(); }
    SQMOD_NODISCARD const SQChar * GetName() const { return Get().GetName(); }
    SQMOD_NODISCARD int GetTeam() const { return Get().GetTeam(); }
    SQMOD_NODISCARD int GetSkin() const { return Get().GetSkin(); }
    SQMOD_NODISCARD LgEntityRGB GetColour() const { return LgEntityRGB(mID, LgEntityType::Player, 0, Get().GetColor()); }
    SQMOD_NODISCARD int GetMoney() const { return Get().GetMoney(); }
    SQMOD_NODISCARD int GetScore() const { return Get().GetScore(); }
    SQMOD_NODISCARD int GetPing() const { return Get().GetPing(); }
    SQMOD_NODISCARD float GetHealth() const { return Get().GetHealth(); }
    SQMOD_NODISCARD float GetArmour() const { return Get().GetArmor(); }
    SQMOD_NODISCARD uint32_t GetImmunity() const { return Get().GetImmunity(); }
    SQMOD_NODISCARD float GetHeading() const { return Get().GetHeading(); }
    SQMOD_NODISCARD LgVehicle * GetVehicle() const
    { const int id = _Func->GetPlayerVehicleId(GetIdentifier()); return VALID_ENTITYEX(id, SQMOD_VEHICLE_POOL) ? Core::Get().GetVehicle(id).mLgInst : nullptr; }
    SQMOD_NODISCARD bool GetFrozen() const { return _Func->GetPlayerOption(GetIdentifier(), vcmpPlayerOptionControllable) <= 0; }
    SQMOD_NODISCARD bool GetDriveByEnabled() const { return _Func->GetPlayerOption(GetIdentifier(), vcmpPlayerOptionDriveBy) >= 1; }
    SQMOD_NODISCARD bool GetWhiteScanLines() const { return _Func->GetPlayerOption(GetIdentifier(), vcmpPlayerOptionWhiteScanlines) >= 1; }
    SQMOD_NODISCARD bool GetGreenScanLines() const { return _Func->GetPlayerOption(GetIdentifier(), vcmpPlayerOptionGreenScanlines) >= 1; }
    SQMOD_NODISCARD bool GetWidescreen() const { return _Func->GetPlayerOption(GetIdentifier(), vcmpPlayerOptionWidescreen) >= 1; }
    SQMOD_NODISCARD bool GetOnRadar() const { return _Func->GetPlayerOption(GetIdentifier(), vcmpPlayerOptionHasMarker) >= 1; }
    SQMOD_NODISCARD bool GetCanAttack() const { return _Func->GetPlayerOption(GetIdentifier(), vcmpPlayerOptionCanAttack) >= 1; }
    SQMOD_NODISCARD int GetVehicleSlot() const { return Get().GetVehicleSlot(); }
    SQMOD_NODISCARD int GetVehicleStatus() const { return Get().GetVehicleStatus(); }
    SQMOD_NODISCARD int GetWeaponSlot() const { return Get().GetWeaponSlot(); }
    SQMOD_NODISCARD int GetWeapon() const { return Get().GetWeapon(); }
    SQMOD_NODISCARD int GetWeaponAmmo() const { return Get().GetAmmo(); }
    SQMOD_NODISCARD int GetAlpha() const { return Get().GetAlpha(); }
    SQMOD_NODISCARD int GetID() const { return mID; }
    SQMOD_NODISCARD bool Typing() const { return Get().IsTyping(); }
    SQMOD_NODISCARD bool ShowingMarkers() const { return _Func->GetPlayerOption(GetIdentifier(), vcmpPlayerOptionShowMarkers) >= 1; }
    SQMOD_NODISCARD bool GetCameraLocked() const { return Get().IsCameraLocked(); }
    SQMOD_NODISCARD uint32_t GetKey() const { return Get().GetKey(); }
    SQMOD_NODISCARD bool GetAwayStatus() const { return Get().IsAway(); }
    SQMOD_NODISCARD LgPlayer * GetSpectateTarget() const
    { const int id = _Func->GetPlayerSpectateTarget(GetIdentifier()); return VALID_ENTITYEX(id, SQMOD_PLAYER_POOL) ? Core::Get().GetPlayer(id).mLgInst : nullptr; }
    SQMOD_NODISCARD LgEntityVector GetSpeed() const
    { return LgEntityVector(mID, LgEntityType::Player, LgPlayerVectorFlag::Speed, Get().GetSpeed()); }
    SQMOD_NODISCARD bool GetCanUseColors() const { return _Func->GetPlayerOption(GetIdentifier(), vcmpPlayerOptionChatTagsEnabled) >= 1; }
    SQMOD_NODISCARD bool GetMarkerVisible() const { return _Func->GetPlayerOption(GetIdentifier(), vcmpPlayerOptionHasMarker) >= 1; }
    SQMOD_NODISCARD bool GetDrunkStatus() const { return _Func->GetPlayerOption(GetIdentifier(), vcmpPlayerOptionDrunkEffects) >= 1; }
    SQMOD_NODISCARD double GetFPS() const { return Get().GetFPS(); }
    SQMOD_NODISCARD const SQChar * GetUniqueID() const { return Get().GetUID(); }
    SQMOD_NODISCARD const SQChar * GetUniqueID2() const { return Get().GetUID2(); }
    SQMOD_NODISCARD bool GetPlayerOnFireStatus() const { return Get().IsBurning(); }
    SQMOD_NODISCARD bool GetPlayerCrouchStatus() const { return Get().IsCrouched(); }
    SQMOD_NODISCARD int GetPlayerAction() const { return Get().GetAction(); }
    SQMOD_NODISCARD uint32_t GetPlayerGameKeys() const { return Get().GetGameKeys(); }
    SQMOD_NODISCARD LgVector GetPlayerAimPos() const { return LgVector(Get().GetAimPosition()); }
    SQMOD_NODISCARD LgVector GetPlayerAimDir() const { return LgVector(Get().GetAimDirection()); }
    SQMOD_NODISCARD int GetWantedLevel() const { return Get().GetWantedLevel(); }
    // --------------------------------------------------------------------------------------------
    void PlaySound(int sound) const { Get().PlaySound(sound); }
    void Kick() const { Get().Kick(); }
    void Ban() const { Get().Ban(); }
    void Spawn() const { Get().ForceSpawn(); }
    void GiveMoney(int money) const { Get().GiveMoney(money); }
    void AddSpeed(const Vector3 & speed) const { Get().AddSpeed(speed); }
    void SetSpeed(const Vector3 & speed) const { Get().SetSpeed(speed); }
    void Eject() const { Get().Disembark(); }
    void SetCameraPos(const Vector3 & pos, const Vector3 & look) const { Get().SetCameraPosition(pos, look); }
    void Disarm() const { Get().StripWeapons(); }
    void CompatAnimation(int anim) const { Get().SetAnimation(anim); }
    void Animation(int group, int anim) const { Get().SetAnimationEx(anim, group); }
    SQMOD_NODISCARD LgVehicle * StandingOnVehicle() const
    { const int id = _Func->GetPlayerStandingOnVehicle(GetIdentifier()); return VALID_ENTITYEX(id, SQMOD_VEHICLE_POOL) ? Core::Get().GetVehicle(id).mLgInst : nullptr; }
    SQMOD_NODISCARD LgObject * StandingOnObject() const
    { const int id = _Func->GetPlayerStandingOnObject(GetIdentifier()); return VALID_ENTITYEX(id, SQMOD_OBJECT_POOL) ? Core::Get().GetObj(id).mLgInst : nullptr; }
    void GiveWeapon(int weapon, int ammo) const { Get().GiveWeapon(weapon, ammo); }
    void SetWeapon(int weapon, int ammo) const { Get().SetWeaponEx(weapon, ammo); }
    void RemoveWeapon(int weapon) const { Get().RemoveWeapon(weapon); }
    SQMOD_NODISCARD int GetWeaponAtSlot(int slot) const { return Get().GetWeaponAtSlot(slot); }
    SQMOD_NODISCARD int GetAmmoAtSlot(int slot) const { return Get().GetAmmoAtSlot(slot); }
    void SetAlpha(int alpha, int fadeTime) const { Get().SetAlphaEx(alpha, fadeTime); }
    SQMOD_NODISCARD bool StreamedToPlayer(const LgPlayer & player) const { return Get().IsStreamedFor(player.Get()); }
    void SetVehicleSlot(const LgVehicle & vehicle, int slot) const;
    void Select() const { Get().ForceSelect(); }
    void RestoreCamera() const { Get().RestoreCamera(); }
    void RemoveMarker() const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionHasMarker, 0); }
    void SetMarker(int toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionShowMarkers, static_cast< uint8_t >(toggle)); }
    void SetDrunkLevel(int visuals, int handling) const {
        if (visuals <= 0 && handling <= 0) _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionDrunkEffects, 0);
        else _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionDrunkEffects, 1);
    }
    void RedirectPlayerToServer(StackStrF & ip, uint32_t port, StackStrF & nick, StackStrF & spass, StackStrF & upass) const
    { Get().Redirect(ip, port, nick, spass, upass); }
    void RequestModuleList() const { Get().GetModuleList(); }
};

/* ------------------------------------------------------------------------------------------------
 * Vehicle entity proxy.
*/
struct LgVehicle
{
    int32_t mID;
    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit LgVehicle(int32_t id)
        : mID(id)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the a valid entity identifier, otherwise throw an exception.
    */
    SQMOD_NODISCARD int32_t GetIdentifier() const
    {
        // Validate the associated identifier
        if (INVALID_ENTITY(mID))
        {
            STHROWF("Invalid vehicle reference");
        }
        // Return it
        return mID;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the native implementation of the associated entity, otherwise throw an exception.
    */
    SQMOD_NODISCARD CVehicle & Get() const
    {
        // Retrieve the associated instance
        CVehicle * p = Core::Get().GetVehicle(GetIdentifier()).mInst;
        // Validate the associated instance
        if (!p)
        {
            STHROWF("Invalid vehicle instance");
        }
        // This is valid so we can return it
        return *p;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve entity instance which is native to the plug-in.
    */
    SQMOD_NODISCARD LightObj & GetNative() const
    {
        return Core::Get().GetVehicle(GetIdentifier()).mObj;
    }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD static int32_t GetDriverID(int32_t id) {
        for(uint32_t i = 0, n = _Func->GetMaxPlayers(); i < n; ++i) {
            if(_Func->IsPlayerConnected(static_cast< int32_t >(i))) {
                if(_Func->GetPlayerVehicleId(static_cast< int32_t >(i)) == id &&
                    _Func->GetPlayerInVehicleSlot(static_cast< int32_t >(i)) == 0) {
                    return static_cast< int32_t >(i);
                }
            }
        }
        return -1;
    }
    // --------------------------------------------------------------------------------------------
    void SetWorld(int world) const { Get().SetWorld(world); }
    void SetImmunity(int immunity) const { Get().SetImmunity(immunity); }
    void SetPosition(const Vector3 & pos) const { Get().SetPosition(pos); }
    void SetSpawnPos(const Vector3 & pos) const { Get().SetSpawnPosition(pos); }
    void SetSpawnAngle(const Quaternion & angle) const { Get().SetSpawnRotation(angle); }
    void SetSpawnAngleEuler(const Vector3 & angle) const { Get().SetSpawnRotationEuler(angle); }
    void SetIdleRespawnTimer(uint32_t time) const { Get().SetIdleRespawnTimer(time); }
    void SetHealth(float health) const { Get().SetHealth(health); }
    void SetColour1(int colour1) const { Get().SetPrimaryColor(colour1); }
    void SetColour2(int colour2) const { Get().SetSecondaryColor(colour2); }
    void SetLocked(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionDoorsLocked, static_cast< uint8_t >(toggle)); }
    void SetDamage(uint32_t damage) const { Get().SetDamageData(damage); }
    void SetLightFlags(int32_t flags) const { Get().SetLightsData(flags); }
    void SetAlarm(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionAlarm, static_cast< uint8_t >(toggle)); }
    void SetSiren(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionSiren, static_cast< uint8_t >(toggle)); }
    void SetLights(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionLights, static_cast< uint8_t >(toggle)); }
    void SetRotation(const Quaternion & rotation) const { Get().SetRotation(rotation); }
    void SetEulerRotation(const Vector3 & angle) const { Get().SetRotationEuler(angle); }
    void SetSpeed(const Vector3 & speed) const { Get().SetSpeed(speed); }
    void SetRelativeSpeed(const Vector3 & speed) const { Get().SetRelativeSpeed(speed); }
    void SetTurnSpeed(const Vector3 & speed) const { Get().SetTurnSpeed(speed); }
    void SetRelativeTurnSpeed(const Vector3 & speed) const { Get().SetRelativeTurnSpeed(speed); }
    void SetRadio(int id) const { Get().SetRadio(id); }
    void SetRadioLocked(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionRadioLocked, static_cast< uint8_t >(toggle)); }
    void SetGhost(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionGhost, static_cast< uint8_t >(toggle)); }
    void SetSingleUse(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionSingleUse, static_cast< uint8_t >(toggle)); }
    void SetTaxiLight(bool toggle) const {
        if (toggle == (GetTaxiLight() != 0)) return;
        uint32_t data = _Func->GetVehicleLightsData(GetIdentifier());
        if (toggle) data |= (1 << 8);
        else data &= ~(1 << 8);
        _Func->SetVehicleLightsData(GetIdentifier(), data);
    }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD int GetWorld() const { return Get().GetWorld(); }
    SQMOD_NODISCARD int GetModel() const { return Get().GetModel(); }
    SQMOD_NODISCARD uint32_t GetImmunity() const { return Get().GetImmunity(); }
    SQMOD_NODISCARD LgEntityVector GetPosition() const
    { return LgEntityVector(mID, LgEntityType::Vehicle, LgVehicleVectorFlag::Pos, Get().GetPosition()); }
    SQMOD_NODISCARD LgEntityVector GetSpawnPos() const
    { return LgEntityVector(mID, LgEntityType::Vehicle, LgVehicleVectorFlag::SpawnPos, Get().GetSpawnPosition()); }
    SQMOD_NODISCARD LgEntityQuaternion GetSpawnAngle() const
    { return LgEntityQuaternion(mID, LgEntityType::Vehicle, LgVehicleQuaternionFlag::SpawnAngle, Get().GetSpawnRotation()); }
    SQMOD_NODISCARD LgEntityVector GetSpawnAngleEuler() const
    { return LgEntityVector(mID, LgEntityType::Vehicle, LgVehicleVectorFlag::SpawnAngle, Get().GetSpawnRotationEuler()); }
    SQMOD_NODISCARD uint32_t GetIdleRespawnTimer() const { return Get().GetIdleRespawnTimer(); }
    SQMOD_NODISCARD float GetHealth() const { return Get().GetHealth(); }
    SQMOD_NODISCARD int GetColour1() const { return Get().GetPrimaryColor(); }
    SQMOD_NODISCARD int GetColour2() const { return Get().GetSecondaryColor(); }
    SQMOD_NODISCARD bool GetLocked() const { return _Func->GetVehicleOption(GetIdentifier(), vcmpVehicleOptionDoorsLocked) >= 1; }
    SQMOD_NODISCARD uint32_t GetDamage() const { return Get().GetDamageData(); }
    SQMOD_NODISCARD uint32_t GetLightFlags() const { return static_cast< uint32_t >(Get().GetLightsData()); }
    SQMOD_NODISCARD bool GetAlarm() const { return _Func->GetVehicleOption(GetIdentifier(), vcmpVehicleOptionAlarm) >= 1; }
    SQMOD_NODISCARD bool GetSiren() const { return _Func->GetVehicleOption(GetIdentifier(), vcmpVehicleOptionSiren) >= 1; }
    SQMOD_NODISCARD bool GetLights() const { return _Func->GetVehicleOption(GetIdentifier(), vcmpVehicleOptionLights) >= 1; }
    SQMOD_NODISCARD LgPlayer * GetDriver() const
    { const int id = GetDriverID(GetIdentifier()); return VALID_ENTITYEX(id, SQMOD_PLAYER_POOL) ? Core::Get().GetPlayer(id).mLgInst : nullptr; }
    SQMOD_NODISCARD int GetID() const { return mID; }
    SQMOD_NODISCARD int GetSyncSource() const { return Get().GetSyncSource(); }
    SQMOD_NODISCARD int GetSyncType() const { return Get().GetSyncType(); }
    SQMOD_NODISCARD bool GetWrecked() const { return Get().IsWrecked(); }
    SQMOD_NODISCARD LgEntityQuaternion GetRotation() const
    { return LgEntityQuaternion(mID, LgEntityType::Vehicle, LgVehicleQuaternionFlag::Angle, Get().GetRotation()); }
    SQMOD_NODISCARD LgEntityVector GetEulerRotation() const
    { return LgEntityVector(mID, LgEntityType::Vehicle, LgVehicleVectorFlag::Angle, Get().GetRotationEuler()); }
    SQMOD_NODISCARD LgEntityVector GetSpeed() const
    { return LgEntityVector(mID, LgEntityType::Vehicle, LgVehicleVectorFlag::Speed, Get().GetSpeed()); }
    SQMOD_NODISCARD LgEntityVector GetRelativeSpeed() const
    { return LgEntityVector(mID, LgEntityType::Vehicle, LgVehicleVectorFlag::RelSpeed, Get().GetRelativeSpeed()); }
    SQMOD_NODISCARD LgEntityVector GetTurnSpeed() const
    { return LgEntityVector(mID, LgEntityType::Vehicle, LgVehicleVectorFlag::TurnSpeed, Get().GetTurnSpeed()); }
    SQMOD_NODISCARD LgEntityVector GetRelativeTurnSpeed() const
    { return LgEntityVector(mID, LgEntityType::Vehicle, LgVehicleVectorFlag::RelTurnSpeed, Get().GetRelativeTurnSpeed()); }
    SQMOD_NODISCARD int GetRadio() const { return Get().GetRadio(); }
    SQMOD_NODISCARD bool GetRadioLockStatus() const { return _Func->GetVehicleOption(GetIdentifier(), vcmpVehicleOptionRadioLocked) >= 1; }
    SQMOD_NODISCARD bool GetGhost() const { return _Func->GetVehicleOption(GetIdentifier(), vcmpVehicleOptionGhost) >= 1; }
    SQMOD_NODISCARD LgVector GetTurretRotation() const { const Vector2 v = Get().GetTurretRotation(); return LgVector(v.x, v.y, 0); }
    SQMOD_NODISCARD bool GetSingleUse() const { return _Func->GetVehicleOption(GetIdentifier(), vcmpVehicleOptionSingleUse) >= 1; }
    SQMOD_NODISCARD bool GetTaxiLight() const { return (_Func->GetVehicleLightsData(GetIdentifier()) & (1 << 8)) != 0; }
    // --------------------------------------------------------------------------------------------
    void Delete() const { _Func->DeleteVehicle(GetIdentifier()); }
    void Fix() const { Get().Fix(); }
    void Respawn() const { Get().Respawn(); }
    void Kill() const { Get().Explode(); }
    void SetFlatTyres(bool toggle) const { Get().FlattenTyres(toggle); }
    SQMOD_NODISCARD int GetPartStatus(int part) const { return Get().GetPartStatus(part); }
    void SetPartStatus(int part, int status) const { Get().SetPartStatus(part, status); }
    SQMOD_NODISCARD int GetTyreStatus(int tyre) const { return Get().GetTyreStatus(tyre); }
    void SetTyreStatus(int part, int status) const { Get().SetTyreStatus(part, status); }
    SQMOD_NODISCARD bool GetStreamedForPlayer(LgPlayer & player) const { return Get().IsStreamedFor(player.Get()); }
    SQMOD_NODISCARD LgPlayer * GetOccupant(int slot) const
    { const int id = _Func->GetVehicleOccupant(GetIdentifier(), slot); return VALID_ENTITYEX(id, SQMOD_PLAYER_POOL) ? Core::Get().GetPlayer(id).mLgInst : nullptr; }
    void SetHandlingData(int rule, float value) const { Get().SetHandlingRule(rule, value); }
    SQMOD_NODISCARD double GetHandlingData(int rule) const { return Get().GetHandlingRule(rule); }
    void ResetHandlingData(int rule) const { Get().ResetHandlingRule(rule); }
    void ResetAllHandling() const { Get().ResetHandlings(); }
    SQMOD_NODISCARD bool IsHandlingSet(int rule) const { return Get().ExistsHandlingRule(rule); }
    // --------------------------------------------------------------------------------------------
    void AddVehicleSpeed(const Vector3 & speed) const { Get().SetSpeed(speed); }
    void AddVehicleRelSpeed(const Vector3 & speed) const { Get().SetRelativeSpeed(speed); }
    void AddVehicleTurnSpeed(const Vector3 & speed) const { Get().SetTurnSpeed(speed); }
    void AddVehicleRelTurnSpeed(const Vector3 & speed) const { Get().SetRelativeTurnSpeed(speed); }
};

// ------------------------------------------------------------------------------------------------
inline bool LgCheckpoint::StreamedToPlayer(LgPlayer & player) const { return Get().IsStreamedFor(player.Get()); }
inline bool LgObject::StreamedToPlayer(LgPlayer & player) const { return Get().IsStreamedFor(player.Get()); }   
inline bool LgPickup::StreamedToPlayer(LgPlayer & player) const { return Get().IsStreamedFor(player.Get()); }   
inline void LgPlayer::SetVehicle(LgVehicle & vehicle) const { Get().Embark(vehicle.Get()); }
inline void LgPlayer::SetVehicleSlot(const LgVehicle & vehicle, int slot) const { Get().EmbarkEx(vehicle.Get(), slot, true, false); }

// ------------------------------------------------------------------------------------------------
void LgCheckpointSetID(LgCheckpoint * inst, int32_t id) { assert(inst); if (inst) inst->mID = id; }
void LgObjectSetID(LgObject * inst, int32_t id) { assert(inst); if (inst) inst->mID = id; }
void LgPickupSetID(LgPickup * inst, int32_t id) { assert(inst); if (inst) inst->mID = id; }
void LgPlayerSetID(LgPlayer * inst, int32_t id) { assert(inst); if (inst) inst->mID = id; }
void LgVehicleSetID(LgVehicle * inst, int32_t id) { assert(inst); if (inst) inst->mID = id; }

// ------------------------------------------------------------------------------------------------
LightObj LgCheckpointObj(HSQUIRRELVM vm, int32_t id) { return LightObj(SqTypeIdentity< LgCheckpoint >{}, vm, id); }
LightObj LgObjectObj(HSQUIRRELVM vm, int32_t id) { return LightObj(SqTypeIdentity< LgObject >{}, vm, id); }
LightObj LgPickupObj(HSQUIRRELVM vm, int32_t id) { return LightObj(SqTypeIdentity< LgPickup >{}, vm, id); }
LightObj LgPlayerObj(HSQUIRRELVM vm, int32_t id) { return LightObj(SqTypeIdentity< LgPlayer >{}, vm, id); }
LightObj LgVehicleObj(HSQUIRRELVM vm, int32_t id) { return LightObj(SqTypeIdentity< LgVehicle >{}, vm, id); }

// ================================================================================================
void Register_Official_Entity(HSQUIRRELVM vm)
{
    // --------------------------------------------------------------------------------------------
    Class< LgCheckpoint, NoConstructor< LgCheckpoint > > checkpoint(vm, CCheckpointTn::Str);
    // Read-write properties
    checkpoint
        .Prop(_SC("World"), &LgCheckpoint::GetWorld, &LgCheckpoint::SetWorld)
        .Prop(_SC("Color"), &LgCheckpoint::GetColor, &LgCheckpoint::SetColor)
        .Prop(_SC("Radius"), &LgCheckpoint::GetRadius, &LgCheckpoint::SetRadius)
        .Prop(_SC("Pos"), &LgCheckpoint::GetPos, &LgCheckpoint::SetPos)
    ;
    // Read-only properties
    checkpoint
        .Prop(_SC("Native"), &LgCheckpoint::GetNative)
        .Prop(_SC("ID"), &LgCheckpoint::GetID)
        .Prop(_SC("Owner"), &LgCheckpoint::GetOwner)
    ;
    // Functions
    checkpoint
        .SquirrelFunc(_SC("_typename"), &CCheckpointTn::Fn)
        .Func(_SC("Remove"), &LgCheckpoint::Delete)
        .Func(_SC("StreamedToPlayer"), &LgCheckpoint::StreamedToPlayer)
    ;
    RootTable(vm).Bind(_SC("CCheckpoint"), checkpoint);
    // --------------------------------------------------------------------------------------------
    Class< LgObject, NoConstructor< LgObject > > object(vm, CObjectTn::Str);
    // Read-write properties
    object
        .Prop(_SC("World"), &LgObject::GetWorld, &LgObject::SetWorld)
        .Prop(_SC("Pos"), &LgObject::GetPos, &LgObject::SetPos)
        .Prop(_SC("TrackingShots"), &LgObject::GetReportingShots, &LgObject::SetReportingShots)
        .Prop(_SC("TrackingBumps"), &LgObject::GetReportingBumps, &LgObject::SetReportingBumps)
    ;
    // Read-only properties
    object
        .Prop(_SC("Native"), &LgObject::GetNative)
        .Prop(_SC("Alpha"), &LgObject::GetAlpha)
        .Prop(_SC("Model"), &LgObject::GetModel)
        .Prop(_SC("Rotation"), &LgObject::GetRotation)
        .Prop(_SC("RotationEuler"), &LgObject::GetRotationEuler)
        .Prop(_SC("ID"), &LgObject::GetID)
    ;
    // Functions
    object
        .SquirrelFunc(_SC("_typename"), &CObjectTn::Fn)
        .Func(_SC("Delete"), &LgObject::Delete)
        .Func(_SC("MoveTo"), &LgObject::MoveTo)
        .Func(_SC("MoveBy"), &LgObject::MoveBy)
        .Func(_SC("RotateTo"), &LgObject::RotateTo)
        .Func(_SC("RotateBy"), &LgObject::RotateBy)
        .Func(_SC("RotateToEuler"), &LgObject::RotateToEuler)
        .Func(_SC("RotateByEuler"), &LgObject::RotateByEuler)
        .Func(_SC("SetAlpha"), &LgObject::SetAlpha)
        .Func(_SC("StreamedToPlayer"), &LgObject::StreamedToPlayer)
    ;
    RootTable(vm).Bind(_SC("CObject"), object);
    // --------------------------------------------------------------------------------------------
    Class< LgPickup, NoConstructor< LgPickup > > pickup(vm, CPickupTn::Str);
    // Read-write properties
    pickup
        .Prop(_SC("World"), &LgPickup::GetWorld, &LgPickup::SetWorld)
        .Prop(_SC("Alpha"), &LgPickup::GetAlpha, &LgPickup::SetAlpha)
        .Prop(_SC("Automatic"), &LgPickup::GetAuto, &LgPickup::SetAuto)
        .Prop(_SC("Timer"), &LgPickup::GetAutoTimer, &LgPickup::SetAutoTimer)
        .Prop(_SC("RespawnTime"), &LgPickup::GetAutoTimer, &LgPickup::SetAutoTimer)
        .Prop(_SC("Pos"), &LgPickup::GetPos, &LgPickup::SetPos)
        .Prop(_SC("SingleUse"), &LgPickup::GetSingleUse, &LgPickup::SetSingleUse)
    ;
    // Read-only properties
    pickup
        .Prop(_SC("Native"), &LgPickup::GetNative)
        .Prop(_SC("Model"), &LgPickup::GetModel)
        .Prop(_SC("Quantity"), &LgPickup::GetQuantity)
        .Prop(_SC("ID"), &LgPickup::GetID)
    ;
    // Functions
    pickup
        .SquirrelFunc(_SC("_typename"), &CPickupTn::Fn)
        .Func(_SC("Remove"), &LgPickup::Delete)
        .Func(_SC("Respawn"), &LgPickup::Respawn)
        .Func(_SC("StreamedToPlayer"), &LgPickup::StreamedToPlayer)
    ;
    RootTable(vm).Bind(_SC("CPickup"), pickup);
    // --------------------------------------------------------------------------------------------
    Class< LgPlayer, NoConstructor< LgPlayer > > player(vm, CPlayerTn::Str);
    // Read-write properties
    player
        .Prop(_SC("Admin"), &LgPlayer::GetAdmin, &LgPlayer::SetAdmin)
        .Prop(_SC("Angle"), &LgPlayer::GetHeading, &LgPlayer::SetHeading)
        .Prop(_SC("Armor"), &LgPlayer::GetArmour, &LgPlayer::SetArmour)
        .Prop(_SC("Armour"), &LgPlayer::GetArmour, &LgPlayer::SetArmour)
        .Prop(_SC("CanAttack"), &LgPlayer::GetCanAttack, &LgPlayer::SetCanAttack)
        .Prop(_SC("CanDriveby"), &LgPlayer::GetDriveByEnabled, &LgPlayer::SetDriveByEnabled)
        .Prop(_SC("Cash"), &LgPlayer::GetMoney, &LgPlayer::SetMoney)
        .Prop(_SC("Color"), &LgPlayer::GetColour, &LgPlayer::SetColour)
        .Prop(_SC("Colour"), &LgPlayer::GetColour, &LgPlayer::SetColour)
        .Prop(_SC("DrivebyAbility"), &LgPlayer::GetDriveByEnabled, &LgPlayer::SetDriveByEnabled)
        .Prop(_SC("Frozen"), &LgPlayer::GetFrozen, &LgPlayer::SetFrozen)
        .Prop(_SC("GreenScanlines"), &LgPlayer::GetGreenScanLines, &LgPlayer::SetGreenScanLines)
        .Prop(_SC("HasChatTags"), &LgPlayer::GetCanUseColors, &LgPlayer::SetCanUseColors)
        .Prop(_SC("HasMarker"), &LgPlayer::GetMarkerVisible, &LgPlayer::SetMarkerVisible)
        .Prop(_SC("Heading"), &LgPlayer::GetHeading, &LgPlayer::SetHeading)
        .Prop(_SC("Health"), &LgPlayer::GetHealth, &LgPlayer::SetHealth)
        .Prop(_SC("Immunity"), &LgPlayer::GetImmunity, &LgPlayer::SetImmunity)
        .Prop(_SC("IsAdmin"), &LgPlayer::GetAdmin, &LgPlayer::SetAdmin)
        .Prop(_SC("IsDrunk"), &LgPlayer::GetDrunkStatus, &LgPlayer::SetDrunkStatus)
        .Prop(_SC("IsFrozen"), &LgPlayer::GetFrozen, &LgPlayer::SetFrozen)
        .Prop(_SC("IsOnRadar"), &LgPlayer::GetOnRadar, &LgPlayer::SetOnRadar)
        .Prop(_SC("IsWeaponSyncBlocked"), &LgPlayer::GetCanAttack, &LgPlayer::SetCanAttack)
        .Prop(_SC("Money"), &LgPlayer::GetMoney, &LgPlayer::SetMoney)
        .Prop(_SC("Name"), &LgPlayer::GetName, &LgPlayer::SetName)
        .Prop(_SC("Pos"), &LgPlayer::GetPosition, &LgPlayer::SetPosition)
        .Prop(_SC("Score"), &LgPlayer::GetScore, &LgPlayer::SetScore)
        .Prop(_SC("SecWorld"), &LgPlayer::GetSecWorld, &LgPlayer::SetSecWorld)
        .Prop(_SC("ShowMarkers"), &LgPlayer::ShowingMarkers, &LgPlayer::ShowMarkers)
        .Prop(_SC("Slot"), &LgPlayer::GetWeaponSlot, &LgPlayer::SetWeaponSlot)
        .Prop(_SC("Skin"), &LgPlayer::GetSkin, &LgPlayer::SetSkin)
        .Prop(_SC("SpectateTarget"), &LgPlayer::GetSpectateTarget, &LgPlayer::SetSpectateTarget)
        .Prop(_SC("Speed"), &LgPlayer::GetSpeed, &LgPlayer::SetSpeed )
        .Prop(_SC("Team"), &LgPlayer::GetTeam, &LgPlayer::SetTeam)
        .Prop(_SC("Vehicle"), &LgPlayer::GetVehicle, &LgPlayer::SetVehicle)
        .Prop(_SC("WantedLevel"), &LgPlayer::GetWantedLevel, &LgPlayer::SetWantedLevel)
        .Prop(_SC("WhiteScanlines"), &LgPlayer::GetWhiteScanLines, &LgPlayer::SetWhiteScanLines)
        .Prop(_SC("Widescreen"), &LgPlayer::GetWidescreen, &LgPlayer::SetWidescreen)
        .Prop(_SC("World"), &LgPlayer::GetWorld, &LgPlayer::SetWorld)
    ;
    // Read-only properties
    player
        .Prop(_SC("Native"), &LgPlayer::GetNative)
        .Prop(_SC("Action"), &LgPlayer::GetPlayerAction)
        .Prop(_SC("AimDir"), &LgPlayer::GetPlayerAimDir)
        .Prop(_SC("AimPos"), &LgPlayer::GetPlayerAimPos)
        .Prop(_SC("Alpha"), &LgPlayer::GetAlpha)
        .Prop(_SC("Ammo"), &LgPlayer::GetWeaponAmmo)
        .Prop(_SC("Away"), &LgPlayer::GetAwayStatus)
        .Prop(_SC("CameraLocked"), &LgPlayer::GetCameraLocked)
        .Prop(_SC("Class"), &LgPlayer::GetClass)
        .Prop(_SC("FPS"), &LgPlayer::GetFPS)
        .Prop(_SC("GameKeys"), &LgPlayer::GetPlayerGameKeys)
        .Prop(_SC("ID"), &LgPlayer::GetID)
        .Prop(_SC("IP"), &LgPlayer::GetIP)
        .Prop(_SC("IsCrouching"), &LgPlayer::GetPlayerCrouchStatus)
        .Prop(_SC("IsOnFire"), &LgPlayer::GetPlayerOnFireStatus)
        .Prop(_SC("IsSpawned"), &LgPlayer::GetSpawned)
        .Prop(_SC("Key"), &LgPlayer::GetKey)
        .Prop(_SC("Ping"), &LgPlayer::GetPing)
        .Prop(_SC("Spawned"), &LgPlayer::GetSpawned)
        .Prop(_SC("StandingOnObject"), &LgPlayer::StandingOnObject)
        .Prop(_SC("StandingOnVehicle"), &LgPlayer::StandingOnVehicle)
        .Prop(_SC("State"), &LgPlayer::GetState)
        .Prop(_SC("Typing"), &LgPlayer::Typing)
        .Prop(_SC("UniqueWorld"), &LgPlayer::GetUniqueWorld)
        .Prop(_SC("UniqueID"), &LgPlayer::GetUniqueID)
        .Prop(_SC("UID"), &LgPlayer::GetUniqueID)
        .Prop(_SC("UniqueID2"), &LgPlayer::GetUniqueID2)
        .Prop(_SC("UID2"), &LgPlayer::GetUniqueID2)
        .Prop(_SC("VehicleSlot"), &LgPlayer::GetVehicleSlot)
        .Prop(_SC("VehicleStatus"), &LgPlayer::GetVehicleStatus)
        .Prop(_SC("Weapon"), &LgPlayer::GetWeapon)
    ;
    // Functions
    player
        .SquirrelFunc(_SC("_typename"), &CPlayerTn::Fn)
        .Func(_SC("_tostring"), &LgPlayer::ToString)
        .Func(_SC("AddSpeed"), &LgPlayer::AddSpeed)
        .Func(_SC("Ban"), &LgPlayer::Ban)
        .Func(_SC("Disarm"), &LgPlayer::Disarm)
        .Func(_SC("Eject"), &LgPlayer::Eject)
        .Func(_SC("GetAmmoAtSlot"), &LgPlayer::GetAmmoAtSlot)
        .Func(_SC("GetWeaponAtSlot"), &LgPlayer::GetWeaponAtSlot)
        .Func(_SC("GiveMoney"), &LgPlayer::GiveMoney)
        .Func(_SC("GiveWeapon"), &LgPlayer::GiveWeapon)
        .Func(_SC("Kick"), &LgPlayer::Kick)
        .Func(_SC("PlaySound"), &LgPlayer::PlaySound)
        .Func(_SC("Redirect"), &LgPlayer::RedirectPlayerToServer)
        .Func(_SC("RemoveWeapon"), &LgPlayer::RemoveWeapon)
        .Func(_SC("RemoveMarker"), &LgPlayer::RemoveMarker)
        .Func(_SC("RestoreCamera"), &LgPlayer::RestoreCamera)
        .Func(_SC("Select"), &LgPlayer::Select)
        .Func(_SC("SetAlpha"), &LgPlayer::SetAlpha)
        .Overload(_SC("SetAnim"), &LgPlayer::Animation)
        .Overload(_SC("SetAnim"), &LgPlayer::CompatAnimation)
        .Func(_SC("SetCameraPos"), &LgPlayer::SetCameraPos)
        .Func(_SC("SetDrunkLevel"), &LgPlayer::SetDrunkLevel)
        .Func(_SC("SetMarker"), &LgPlayer::SetMarker)
        .Func(_SC("SetWantedLevel"), &LgPlayer::SetWantedLevel)
        .Func(_SC("SetWeapon"), &LgPlayer::SetWeapon)
        .Func(_SC("Spawn"), &LgPlayer::Spawn)
        .Func(_SC("StreamedToPlayer"), &LgPlayer::StreamedToPlayer)
        .Func(_SC("PutInVehicleSlot"), &LgPlayer::SetVehicleSlot)
        .Func(_SC("RequestModuleList"), &LgPlayer::RequestModuleList)
    ;
    RootTable(vm).Bind(_SC("CPlayer"), player);
    // --------------------------------------------------------------------------------------------
    Class< LgVehicle, NoConstructor< LgVehicle > > vehicle(vm, CVehicleTn::Str);
    // Read-write properties
    vehicle
        .Prop(_SC("Immunity"), &LgVehicle::GetImmunity, &LgVehicle::SetImmunity)
        .Prop(_SC("Pos"), &LgVehicle::GetPosition, &LgVehicle::SetPosition)
        .Prop(_SC("World"), &LgVehicle::GetWorld, &LgVehicle::SetWorld)
        .Prop(_SC("SpawnPos"), &LgVehicle::GetSpawnPos, &LgVehicle::SetSpawnPos)
        .Prop(_SC("EulerSpawnAngle"), &LgVehicle::GetSpawnAngleEuler, &LgVehicle::SetSpawnAngleEuler)
        .Prop(_SC("SpawnAngle"), &LgVehicle::GetSpawnAngle, &LgVehicle::SetSpawnAngle)
        .Prop(_SC("RespawnTimer"), &LgVehicle::GetIdleRespawnTimer, &LgVehicle::SetIdleRespawnTimer)
        .Prop(_SC("Health"), &LgVehicle::GetHealth, &LgVehicle::SetHealth)
        .Prop(_SC("Colour1"), &LgVehicle::GetColour1, &LgVehicle::SetColour1)
        .Prop(_SC("Colour2"), &LgVehicle::GetColour2, &LgVehicle::SetColour2)
        .Prop(_SC("Locked"), &LgVehicle::GetLocked, &LgVehicle::SetLocked)
        .Prop(_SC("LightFlags"), &LgVehicle::GetLightFlags, &LgVehicle::SetLightFlags)
        .Prop(_SC("TaxiLight"), &LgVehicle::GetTaxiLight, &LgVehicle::SetTaxiLight)
        .Prop(_SC("Damage"), &LgVehicle::GetDamage, &LgVehicle::SetDamage)
        .Prop(_SC("Alarm"), &LgVehicle::GetAlarm, &LgVehicle::SetAlarm)
        .Prop(_SC("Siren"), &LgVehicle::GetSiren, &LgVehicle::SetSiren)
        .Prop(_SC("Lights"), &LgVehicle::GetLights, &LgVehicle::SetLights)
        .Prop(_SC("Angle"), &LgVehicle::GetRotation, &LgVehicle::SetRotation)
        .Prop(_SC("Rotation"), &LgVehicle::GetRotation, &LgVehicle::SetRotation)
        .Prop(_SC("EulerAngle"), &LgVehicle::GetEulerRotation, &LgVehicle::SetEulerRotation)
        .Prop(_SC("EulerRotation"), &LgVehicle::GetEulerRotation, &LgVehicle::SetEulerRotation)
        .Prop(_SC("Speed"), &LgVehicle::GetSpeed, &LgVehicle::SetSpeed)
        .Prop(_SC("RelativeSpeed"), &LgVehicle::GetRelativeSpeed, &LgVehicle::SetRelativeSpeed)
        .Prop(_SC("TurnSpeed"), &LgVehicle::GetTurnSpeed, &LgVehicle::SetTurnSpeed)
        .Prop(_SC("RelativeTurnSpeed"), &LgVehicle::GetRelativeTurnSpeed, &LgVehicle::SetRelativeTurnSpeed)
        .Prop(_SC("Radio"), &LgVehicle::GetRadio, &LgVehicle::SetRadio)
        .Prop(_SC("RadioLocked"), &LgVehicle::GetRadioLockStatus, &LgVehicle::SetRadioLocked)
        .Prop(_SC("IsGhost"), &LgVehicle::GetGhost, &LgVehicle::SetGhost)
        .Prop(_SC("SingleUse"), &LgVehicle::GetSingleUse, &LgVehicle::SetSingleUse)
    ;
    // Read-only properties
    vehicle
        .Prop(_SC("Native"), &LgVehicle::GetNative)
        .Prop(_SC("Model"), &LgVehicle::GetModel)
        .Prop(_SC("Driver"), &LgVehicle::GetDriver)
        .Prop(_SC("ID"), &LgVehicle::GetID)
        .Prop(_SC("SyncSource"), &LgVehicle::GetSyncSource)
        .Prop(_SC("SyncType"), &LgVehicle::GetSyncType)
        .Prop(_SC("TurretRotation"), &LgVehicle::GetTurretRotation)
        .Prop(_SC("Wrecked"), &LgVehicle::GetWrecked)
    ;
    // Functions
    vehicle
        .SquirrelFunc(_SC("_typename"), &CVehicleTn::Fn)
        .Func( _SC("Delete"), &LgVehicle::Delete)
        .Func( _SC("Remove"), &LgVehicle::Delete)
        .Func( _SC("Respawn"), &LgVehicle::Respawn)
        .Func( _SC("Kill"), &LgVehicle::Kill)
        .Func( _SC("KillEngine"), &LgVehicle::Kill)
        .Func( _SC("Fix"), &LgVehicle::Fix)
        .Func( _SC("GetPart"), &LgVehicle::GetPartStatus)
        .Func( _SC("SetPart"), &LgVehicle::SetPartStatus)
        .Func( _SC("GetTyre"), &LgVehicle::GetTyreStatus)
        .Func( _SC("SetTyre"), &LgVehicle::SetTyreStatus)
        .Func( _SC("GetTire"), &LgVehicle::GetTyreStatus)
        .Func( _SC("SetTire"), &LgVehicle::SetTyreStatus)
        .Func( _SC("SetFlatTyres"), &LgVehicle::SetFlatTyres)
        .Func( _SC("StreamedForPlayer"), &LgVehicle::GetStreamedForPlayer)
        .Func( _SC("GetOccupant"), &LgVehicle::GetOccupant)
        .Func( _SC("SetHandlingData"), &LgVehicle::SetHandlingData)
        .Func( _SC("GetHandlingData"), &LgVehicle::GetHandlingData)
        .Func( _SC("ResetHandlingData"), &LgVehicle::ResetHandlingData)
        .Func( _SC("ResetAllHandling"), &LgVehicle::ResetAllHandling)
        .Func( _SC("IsHandlingSet"), &LgVehicle::IsHandlingSet)
        .Func( _SC("AddSpeed"), &LgVehicle::AddVehicleSpeed)
        .Func( _SC("AddRelSpeed"), &LgVehicle::AddVehicleRelSpeed)
        .Func( _SC("AddTurnSpeed"), &LgVehicle::AddVehicleTurnSpeed)
        .Func( _SC("AddRelTurnSpeed"), &LgVehicle::AddVehicleRelTurnSpeed)
    ;
    RootTable(vm).Bind(_SC("CVehicle"), vehicle);
}

// ------------------------------------------------------------------------------------------------
static void LgClientMessage(StackStrF & msg, LgPlayer & player, int r, int g, int b)
{ _Func->SendClientMessage(player.GetIdentifier(), Color4(static_cast< uint8_t >(r), static_cast< uint8_t >(g),
                                                          static_cast< uint8_t >(b), 255).GetRGBA(), "%s", msg.mPtr); }
static void LgClientMessageWithAlpha(StackStrF & msg, LgPlayer & player, int r, int g, int b, int a)
{ _Func->SendClientMessage(player.GetIdentifier(), Color4(static_cast< uint8_t >(r), static_cast< uint8_t >(g),
                                                          static_cast< uint8_t >(b), static_cast< uint8_t >(a)).GetRGBA(), "%s", msg.mPtr); }
static void LgClientMessageToAll(StackStrF & msg, int r, int g, int b) {
    const uint32_t c = Color4(static_cast< uint8_t >(r), static_cast< uint8_t >(g),
                              static_cast< uint8_t >(b), 255).GetRGBA();
    ForeachActivePlayer([&](auto & p) { _Func->SendClientMessage(p.mID, c, "%s", msg.mPtr); });
}
static void LgClientMessageToAllWithAlpha(StackStrF & msg, int r, int g, int b, int a) {
    const uint32_t c = Color4(static_cast< uint8_t >(r), static_cast< uint8_t >(g),
                              static_cast< uint8_t >(b), static_cast< uint8_t >(a)).GetRGBA();
    ForeachActivePlayer([&](auto & p) { _Func->SendClientMessage(p.mID, c, "%s", msg.mPtr); });
}
static void LgGameMessage(StackStrF & msg, LgPlayer & player, int type)
{ _Func->SendGameMessage(player.GetIdentifier(), type, msg.mPtr); }
static void LgGameMessageAlternate(StackStrF & msg, LgPlayer & player)
{ { _Func->SendGameMessage(player.GetIdentifier(), 1, msg.mPtr); } }
static void LgGameMessageToAll(StackStrF & msg, int type)
{ _Func->SendGameMessage(-1, type, msg.mPtr); }
static void LgGameMessageToAllAlternate(StackStrF & msg)
{ _Func->SendGameMessage(-1, 1, msg.mPtr); }
// ------------------------------------------------------------------------------------------------
static void LgShutdownServer() { _Func->ShutdownServer(); }
static void LgSetServerName(StackStrF & str) { _Func->SetServerName(str.mPtr); }
static void LgSetMaxPlayers(int newMaxPlayers) { _Func->SetMaxPlayers(static_cast< uint32_t >(newMaxPlayers)); }
static void LgSetServerPassword(StackStrF & str) { _Func->SetServerPassword(str.mPtr); }
static void LgSetGameModeText(StackStrF & str) { _Func->SetGameModeText(str.mPtr); }
static void LgSetTimeRate(int32_t rate) { _Func->SetTimeRate(rate); }
static void LgSetHour(int hour) { _Func->SetHour( hour ); }
static void LgSetMinute(int minute) { _Func->SetMinute(minute); }
static void LgSetTime(int hour, int minute) { LgSetHour(hour); LgSetMinute(minute); }
static void LgSetWeather(int weather) { _Func->SetWeather(weather); }
static void LgSetGravity(float gravity) { _Func->SetGravity(gravity); }
static void LgSetGamespeed(float speed) { _Func->SetGameSpeed(speed); }
static void LgSetWaterLevel(float level) { _Func->SetWaterLevel(level); }
static void LgSetMaxHeight(float height) { _Func->SetMaximumFlightAltitude(height); }
static void LgSetKillDelay(int delay) { _Func->SetKillCommandDelay(delay); }
static void LgSetFallTimer(int delay) { _Func->SetFallTimer(static_cast< uint16_t >(delay)); }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static SQInteger LgGetServerName(HSQUIRRELVM vm) {
    char buffer[128]{'\0'};
    _Func->GetServerName(buffer, sizeof(buffer));
    sq_pushstring(vm, buffer, -1);
    return 1;
}
SQMOD_NODISCARD static SQInteger LgGetServerPassword(HSQUIRRELVM vm) {
    char buffer[128]{'\0'};
    _Func->GetServerPassword(buffer, sizeof(buffer));
    sq_pushstring(vm, buffer, -1);
    return 1;
}
SQMOD_NODISCARD static SQInteger LgGetGameModeText(HSQUIRRELVM vm) {
    char buffer[128]{'\0'};
    _Func->GetGameModeText(buffer, sizeof(buffer));
    sq_pushstring(vm, buffer, -1);
    return 1;
}
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static uint32_t LgGetMaxPlayers() { return _Func->GetMaxPlayers(); }
SQMOD_NODISCARD static uint32_t LgGetTimeRate() { return static_cast< uint32_t >(_Func->GetTimeRate()); }
SQMOD_NODISCARD static int LgGetHour() { return _Func->GetHour(); }
SQMOD_NODISCARD static int LgGetMinute() { return _Func->GetMinute(); }
SQMOD_NODISCARD static int LgGetWeather() { return _Func->GetWeather(); }
SQMOD_NODISCARD static float LgGetGravity() { return _Func->GetGravity(); }
SQMOD_NODISCARD static float LgGetGamespeed() { return _Func->GetGameSpeed(); }
SQMOD_NODISCARD static float LgGetWaterLevel() { return _Func->GetWaterLevel(); }
SQMOD_NODISCARD static float LgGetMaxHeight() { return _Func->GetMaximumFlightAltitude(); }
SQMOD_NODISCARD static int LgGetKillDelay() { return _Func->GetKillCommandDelay(); }
SQMOD_NODISCARD static int LgGetFallTimer() { return _Func->GetFallTimer(); }
// ------------------------------------------------------------------------------------------------
static void LgToggleSyncFrameLimiter(bool toggle) { _Func->SetServerOption(vcmpServerOptionSyncFrameLimiter, static_cast< uint8_t >(toggle)); }
static void LgToggleFrameLimiter(bool toggle) { _Func->SetServerOption(vcmpServerOptionFrameLimiter, static_cast< uint8_t >(toggle)); }
static void LgToggleTaxiBoostJump(bool toggle) { _Func->SetServerOption(vcmpServerOptionTaxiBoostJump, static_cast< uint8_t >(toggle)); }
static void LgToggleDriveOnWater(bool toggle) { _Func->SetServerOption(vcmpServerOptionDriveOnWater, static_cast< uint8_t >(toggle)); }
static void LgToggleFastSwitch(bool toggle) { _Func->SetServerOption(vcmpServerOptionFastSwitch, static_cast< uint8_t >(toggle)); }
static void LgToggleFriendlyFire(bool toggle) { _Func->SetServerOption(vcmpServerOptionFriendlyFire, static_cast< uint8_t >(toggle)); }
static void LgToggleDisableDriveby(bool toggle) { _Func->SetServerOption(vcmpServerOptionDisableDriveBy, static_cast< uint8_t >(toggle)); }
static void LgTogglePerfectHandling(bool toggle) { _Func->SetServerOption(vcmpServerOptionPerfectHandling, static_cast< uint8_t >(toggle)); }
static void LgToggleFlyingCars(bool toggle) { _Func->SetServerOption(vcmpServerOptionFlyingCars, static_cast< uint8_t >(toggle)); }
static void LgToggleJumpSwitch(bool toggle) { _Func->SetServerOption(vcmpServerOptionJumpSwitch, static_cast< uint8_t >(toggle)); }
static void LgToggleShowOnRadar(bool toggle) { _Func->SetServerOption(vcmpServerOptionShowMarkers, static_cast< uint8_t >(toggle)); }
static void LgToggleStuntBike(bool toggle) { _Func->SetServerOption(vcmpServerOptionStuntBike, static_cast< uint8_t >(toggle)); }
static void LgToggleShootInAir(bool toggle) { _Func->SetServerOption(vcmpServerOptionShootInAir, static_cast< uint8_t >(toggle)); }
static void LgToggleShowNametags(bool toggle) { _Func->SetServerOption(vcmpServerOptionShowNameTags, static_cast< uint8_t >(toggle)); }
static void LgToggleJoinMessages(bool toggle) { _Func->SetServerOption(vcmpServerOptionJoinMessages, static_cast< uint8_t >(toggle)); }
static void LgToggleDeathMessages(bool toggle) { _Func->SetServerOption(vcmpServerOptionDeathMessages, static_cast< uint8_t >(toggle)); }
static void LgToggleChatTagDefault(bool toggle) { _Func->SetServerOption(vcmpServerOptionChatTagsEnabled, static_cast< uint8_t >(toggle)); }
static void LgToggleShowOnlyTeamMarkers(bool toggle) { _Func->SetServerOption(vcmpServerOptionOnlyShowTeamMarkers, static_cast< uint8_t >(toggle)); }
static void LgToggleWallglitch(bool toggle) { _Func->SetServerOption(vcmpServerOptionWallGlitch, static_cast< uint8_t >(toggle)); }
static void LgToggleDisableBackfaceCulling(bool toggle) { _Func->SetServerOption(vcmpServerOptionDisableBackfaceCulling, static_cast< uint8_t >(toggle)); }
static void LgToggleDisableHeliBladeDamage(bool toggle) { _Func->SetServerOption(vcmpServerOptionDisableHeliBladeDamage, static_cast< uint8_t >(toggle)); }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static bool LgEnabledSyncFrameLimiter() { return _Func->GetServerOption(vcmpServerOptionSyncFrameLimiter) >= 1; }
SQMOD_NODISCARD static bool LgEnabledFrameLimiter() { return _Func->GetServerOption(vcmpServerOptionFrameLimiter) >= 1; }
SQMOD_NODISCARD static bool LgEnabledTaxiBoostJump() { return _Func->GetServerOption(vcmpServerOptionTaxiBoostJump) >= 1; }
SQMOD_NODISCARD static bool LgEnabledDriveOnWater() { return _Func->GetServerOption(vcmpServerOptionDriveOnWater) >= 1; }
SQMOD_NODISCARD static bool LgEnabledFastSwitch() { return _Func->GetServerOption(vcmpServerOptionFastSwitch) >= 1; }
SQMOD_NODISCARD static bool LgEnabledFriendlyFire() { return _Func->GetServerOption(vcmpServerOptionFriendlyFire) >= 1; }
SQMOD_NODISCARD static bool LgEnabledDisableDriveby() { return _Func->GetServerOption(vcmpServerOptionDisableDriveBy) < 1; }
SQMOD_NODISCARD static bool LgEnabledPerfectHandling() { return _Func->GetServerOption(vcmpServerOptionPerfectHandling) >= 1; }
SQMOD_NODISCARD static bool LgEnabledFlyingCars() { return _Func->GetServerOption(vcmpServerOptionFlyingCars) >= 1; }
SQMOD_NODISCARD static bool LgEnabledJumpSwitch() { return _Func->GetServerOption(vcmpServerOptionJumpSwitch) >= 1; }
SQMOD_NODISCARD static bool LgEnabledShowOnRadar() { return _Func->GetServerOption(vcmpServerOptionShowMarkers) >= 1; }
SQMOD_NODISCARD static bool LgEnabledStuntBike() { return _Func->GetServerOption(vcmpServerOptionStuntBike) >= 1; }
SQMOD_NODISCARD static bool LgEnabledShootInAir() { return _Func->GetServerOption(vcmpServerOptionShootInAir) >= 1; }
SQMOD_NODISCARD static bool LgEnabledShowNametags() { return _Func->GetServerOption(vcmpServerOptionShowNameTags) >= 1; }
SQMOD_NODISCARD static bool LgEnabledJoinMessages() { return _Func->GetServerOption(vcmpServerOptionJoinMessages) >= 1; }
SQMOD_NODISCARD static bool LgEnabledDeathMessages() { return _Func->GetServerOption(vcmpServerOptionDeathMessages) >= 1; }
SQMOD_NODISCARD static bool LgEnabledChatTagDefault() { return _Func->GetServerOption(vcmpServerOptionChatTagsEnabled) >= 1; }
SQMOD_NODISCARD static bool LgEnabledShowOnlyTeamMarkers() { return _Func->GetServerOption(vcmpServerOptionOnlyShowTeamMarkers) >= 1; }
SQMOD_NODISCARD static bool LgEnabledWallglitch() { return _Func->GetServerOption(vcmpServerOptionWallGlitch) >= 1; }
SQMOD_NODISCARD static bool LgEnabledDisableBackfaceCulling() { return _Func->GetServerOption(vcmpServerOptionDisableBackfaceCulling) >= 1; }
SQMOD_NODISCARD static bool LgEnabledDisableHeliBladeDamage() { return _Func->GetServerOption(vcmpServerOptionDisableHeliBladeDamage) >= 1; }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static int LgCreateBlip(int world, const Vector3 & pos, int scale, const Color4 & color, int sprite)
{ return _Func->CreateCoordBlip(-1, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprite); }
static void LgDestroyBlip(int blip) { _Func->DestroyCoordBlip(blip); }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static int LgCreateRadioStream(StackStrF & name, StackStrF & url, bool selectable)
{ return _Func->AddRadioStream(-1, name.mPtr, url.mPtr, static_cast< uint8_t >(selectable)); }
static void LgCreateRadioStreamWithID(int radio, StackStrF & name, StackStrF & url, bool selectable)
{ _Func->AddRadioStream(radio, name.mPtr, url.mPtr, static_cast< uint8_t >(selectable)); }
static void LgDestroyRadioStream(int radio) { _Func->RemoveRadioStream(radio); }
// ------------------------------------------------------------------------------------------------
static void LgCreateExplosion (int world, int type, const Vector3 & pos, int player, bool ground)
{ _Func->CreateExplosion(world, type, pos.x, pos.y, pos.z, player, static_cast< uint8_t >(ground)); }
static void LgCreateExplosionExpanded(int world, int type, float x, float y, float z, int player, bool ground)
{ _Func->CreateExplosion(world, type, x, y, z, player, static_cast< uint8_t >(ground)); }
// ------------------------------------------------------------------------------------------------
static void LgPlayGameSound(int world, int sound, const Vector3 & pos)
{ _Func->PlaySound(world, sound, pos.x, pos.y, pos.z); }
static void LgPlayGameSoundForPlayer(LgPlayer & player, int sound)
{ _Func->PlaySound(player.Get().GetUniqueWorld(), sound, NAN, NAN, NAN); }
static void LgPlayGameSoundForWorld(int world, int sound)
{ _Func->PlaySound(world, sound, NAN, NAN, NAN); }
static void LgPlayGameSoundExpanded(int player, int sound, float x, float y, float z)
{ _Func->PlaySound(player, sound, x, y, z); }
// ------------------------------------------------------------------------------------------------
static void LgSetUseClasses(bool toggle)
{ _Func->SetServerOption(vcmpServerOptionUseClasses, static_cast< uint8_t >(toggle)); }
static bool LgUsingClasses()
{ return _Func->GetServerOption(vcmpServerOptionUseClasses) >= 1; }
static void LgAddClass(int team, const Color3 & col, int skin, const Vector3 & pos, float angle,
                        int wep1, int ammo1, int wep2, int ammo2, int wep3, int ammo3)
{ _Func->AddPlayerClass(team, col.GetRGB(), skin, pos.x, pos.y, pos.z, angle, wep1, ammo1, wep2, ammo2, wep3, ammo3); }
// ------------------------------------------------------------------------------------------------
static void LgSetSpawnPlayerPos(const Vector3 & pos) { _Func->SetSpawnPlayerPosition(pos.x, pos.y, pos.z); }
static void LgSetSpawnCameraPos(const Vector3 & pos) { _Func->SetSpawnCameraPosition(pos.x, pos.y, pos.z); }
static void LgSetSpawnCameraLook(const Vector3 & pos) { _Func->SetSpawnCameraLookAt(pos.x, pos.y, pos.z);  }
static void LgSetSpawnPlayerPosExpanded(float x, float y, float z) { _Func->SetSpawnPlayerPosition(x, y, z); }
static void LgSetSpawnCameraPosExpanded(float x, float y, float z) { _Func->SetSpawnCameraPosition(x, y, z); }
static void LgSetSpawnCameraLookExpanded(float x, float y, float z) { _Func->SetSpawnCameraLookAt(x, y, z);  }
// ------------------------------------------------------------------------------------------------
static void LgBanIP(StackStrF & ip) { _Func->BanIP(const_cast< SQChar * >(ip.mPtr)); }
static void LgUnbanIP(StackStrF & ip) { _Func->UnbanIP(const_cast< SQChar * >(ip.mPtr)); }
SQMOD_NODISCARD static bool LgIsIPBanned(StackStrF & ip) { return _Func->IsIPBanned(const_cast< SQChar * >(ip.mPtr)) >= 1; }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static int LgGetPlayerIDFromName(StackStrF & name) { return _Func->GetPlayerIdFromName(name.mPtr); }
SQMOD_NODISCARD static bool LgIsWorldCompatibleWithPlayer (LgPlayer & player, int world)
{ return _Func->IsPlayerWorldCompatible(player.GetIdentifier(), world) >= 1; }
// ------------------------------------------------------------------------------------------------
static LightObj & LgCreatePickupCompat(int model, const Vector3 & pos)
{ return Core::Get().NewPickup(model, 1, 0, pos.x, pos.y, pos.z, 255, false, SQMOD_CREATE_DEFAULT, NullLightObj()).mLgObj; }
static LightObj & LgCreateVehicleCompat(int model, const Vector3 & pos, float angle, int col1, int col2 )
{ return Core::Get().NewVehicle(model, 1, pos.x, pos.y, pos.z, angle, col1, col2, SQMOD_CREATE_DEFAULT, NullLightObj()).mLgObj; }
// ------------------------------------------------------------------------------------------------
static LightObj & LgCreateVehicle(int model, int world, const Vector3 & pos, float angle, int col1, int col2)
{ return Core::Get().NewVehicle(model, world, pos.x, pos.y, pos.z, angle, col1, col2, SQMOD_CREATE_DEFAULT, NullLightObj()).mLgObj; }
static LightObj & LgCreatePickup(int model, int world, int quantity, const Vector3 & pos, int alpha, bool automatic)
{ return Core::Get().NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic, SQMOD_CREATE_DEFAULT, NullLightObj()).mLgObj; }
static LightObj & LgCreateObject(int model, int world, const Vector3 & pos, int alpha)
{ return Core::Get().NewObject(model, world, pos.x, pos.y, pos.z, alpha, SQMOD_CREATE_DEFAULT, NullLightObj()).mLgObj; }
static LightObj & LgCreateCheckpoint(LightObj & player, int world, bool sphere, const Vector3 & pos, const Color4 & col, float radius) {
    const int32_t id = player.IsNull() ? -1 : player.CastI< LgPlayer >()->GetIdentifier();
    return Core::Get().NewCheckpoint(id, world, sphere, pos.x, pos.y, pos.z, col.r, col.g, col.b, col.a, radius, SQMOD_CREATE_DEFAULT, NullLightObj()).mLgObj;
}
// ------------------------------------------------------------------------------------------------
static LightObj & LgCreateVehicleExpanded(int model, int world, float x, float y, float z, float angle, int col1, int col2)
{ return Core::Get().NewVehicle(model, world, x, y, z, angle, col1, col2, SQMOD_CREATE_DEFAULT, NullLightObj()).mLgObj; }
static LightObj & LgCreatePickupExpanded(int model, int world, int quantity, float x, float y, float z, int alpha, bool automatic)
{ return Core::Get().NewPickup(model, world, quantity, x, y, z, alpha, automatic, SQMOD_CREATE_DEFAULT, NullLightObj()).mLgObj; }
static LightObj & LgCreateObjectExpanded(int model, int world, float x, float y, float z, int alpha)
{ return Core::Get().NewObject(model, world, x, y, z, alpha, SQMOD_CREATE_DEFAULT, NullLightObj()).mLgObj; }
// ------------------------------------------------------------------------------------------------
static LightObj & LgFindPickup(int id) { return VALID_ENTITYEX(id, SQMOD_PICKUP_POOL) ? Core::Get().GetPickup(id).mLgObj : NullLightObj();  }
static LightObj & LgFindObject(int id) { return VALID_ENTITYEX(id, SQMOD_OBJECT_POOL) ? Core::Get().GetObj(id).mLgObj : NullLightObj();  }
static LightObj & LgFindVehicle(int id) { return VALID_ENTITYEX(id, SQMOD_VEHICLE_POOL) ? Core::Get().GetVehicle(id).mLgObj : NullLightObj();  }
static LightObj & LgFindCheckpoint(int id) { return VALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL) ? Core::Get().GetCheckpoint(id).mLgObj : NullLightObj();  }
// ------------------------------------------------------------------------------------------------
static void LgSetWorldBounds(float maxX, float minX, float maxY, float minY) { _Func->SetWorldBounds(maxX, minX, maxY, minY); }
SQMOD_NODISCARD static LgBounds LgGetWorldBounds() {
    LgBounds b;
    _Func->GetWorldBounds(&b.max_x, &b.min_x, &b.max_y, &b.min_y);
    return b;
}
// ------------------------------------------------------------------------------------------------
static void LgSetWastedSettings(uint32_t death_time, uint32_t fade_time, float fade_in_speed, float fade_out_speed, const Color3 & colour, uint32_t corpse_fade_delay, uint32_t corpse_fade_time)
{ _Func->SetWastedSettings(death_time, fade_time, fade_in_speed, fade_out_speed, colour.GetRGBA(), corpse_fade_delay, corpse_fade_time ); }
SQMOD_NODISCARD static LgWastedSettings LgGetWastedSettings() {
    uint32_t colour;
    LgWastedSettings ws;
    _Func->GetWastedSettings(&ws.mDeathTime, &ws.mFadeTime, &ws.mFadeInSpeed, &ws.mFadeOutSpeed, &colour, &ws.mCorpseFadeDelay, &ws.mCorpseFadeTime );
    ws.mFadeColour.SetRGB(colour);
    return ws;
}
// ------------------------------------------------------------------------------------------------
static void LgRawHideMapObject(int model, int x, int y, int z) { _Func->HideMapObject(model, static_cast< int16_t >(x), static_cast< int16_t >(y), static_cast< int16_t >(z)); }
static void LgHideMapObject(int model, float x, float y, float z) { HideMapObjectEx(model, x, y, z); }
static void LgShowMapObject(int model, float x, float y, float z) { ShowMapObjectEx(model, x, y, z); }
static void LgShowAllMapObjects() { _Func->ShowAllMapObjects(); }
static void LgForceAllSelect() { _Func->ForceAllSelect(); }
// ------------------------------------------------------------------------------------------------
static void LgResetAllVehicleHandling() { _Func->ResetAllVehicleHandlings(); }
static bool LgIsHandlingRuleSet(int model, int rule) { return _Func->ExistsHandlingRule(model, rule) >= 1; }
static void LgSetHandlingRule(int model, int rule, float value) { _Func->SetHandlingRule(model, rule, value); }
SQMOD_NODISCARD static double LgGetHandlingRule(int model, int rule) { return _Func->GetHandlingRule(model, rule); }
static void LgResetHandlingRule(int model, int rule) { _Func->ResetHandlingRule(model, rule); }
static void LgResetVehicleHandling(int model) { _Func->ResetHandling(model); }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static bool LgSetWeaponDataValue(int weapon, int field, double value) { return _Func->SetWeaponDataValue(weapon, field, value) == vcmpErrorNone; }
SQMOD_NODISCARD static double LgGetWeaponDataValue(int weapon, int field) { return _Func->GetWeaponDataValue(weapon, field); }
SQMOD_NODISCARD static bool LgResetWeaponDataValue(int weapon, int field) { return _Func->ResetWeaponDataValue(weapon, field) == vcmpErrorNone; }
SQMOD_NODISCARD static bool LgIsWeaponDataModified(int weapon, int field) { return _Func->IsWeaponDataValueModified(weapon, field) == vcmpErrorNone; }
SQMOD_NODISCARD static bool LgResetWeaponData(int weapon) { return _Func->ResetWeaponData(weapon) == vcmpErrorNone; }
static void LgResetAllWeaponData() { return _Func->ResetAllWeaponData(); }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static int LgBindKey(bool down, int key1, int key2, int key3)
{ return Core::Get().NewKeyBind(-1, down, key1, key2, key3, SQMOD_CREATE_DEFAULT, NullLightObj()).mID; }
SQMOD_NODISCARD static bool LgRemoveKeybind(int id) { return _Func->RemoveKeyBind(id) == vcmpErrorNone; }
static void LgRemoveAllKeybinds() { _Func->RemoveAllKeyBinds(); }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static bool LgGetCinematicBorder(LgPlayer & player) { return _Func->GetPlayerOption(player.GetIdentifier(), vcmpPlayerOptionWidescreen) >= 1; }
SQMOD_NODISCARD static bool LgGetGreenScanLines(LgPlayer & player) { return _Func->GetPlayerOption(player.GetIdentifier(), vcmpPlayerOptionGreenScanlines) >= 1; }
SQMOD_NODISCARD static bool LgGetWhiteScanLines(LgPlayer & player) { return _Func->GetPlayerOption(player.GetIdentifier(), vcmpPlayerOptionWhiteScanlines) >= 1; }
static void LgSetCinematicBorder(LgPlayer & player, bool toggle) { _Func->SetPlayerOption(player.GetIdentifier(), vcmpPlayerOptionWidescreen, static_cast< uint8_t >(toggle)); }
static void LgSetGreenScanLines(LgPlayer & player, bool toggle) { _Func->SetPlayerOption(player.GetIdentifier(), vcmpPlayerOptionGreenScanlines, static_cast< uint8_t >(toggle)); }
static void LgSetWhiteScanLines(LgPlayer & player, bool toggle) { _Func->SetPlayerOption(player.GetIdentifier(), vcmpPlayerOptionWhiteScanlines, static_cast< uint8_t >(toggle)); }
// ------------------------------------------------------------------------------------------------
static void LgKickPlayer(LgPlayer & player) { _Func->KickPlayer(player.GetIdentifier()); }
static void LgBanPlayer(LgPlayer & player) { _Func->BanPlayer(player.GetIdentifier()); }
// ------------------------------------------------------------------------------------------------
static void LgMessage(StackStrF & msg) { _Func->SendClientMessage(-1, 0x0b5fa5ff, "%s", msg.mPtr); }
static void LgMessagePlayer(StackStrF & msg, LgPlayer & player) { _Func->SendClientMessage(player.GetIdentifier(), 0x0b5fa5ff, "%s", msg.mPtr); }
static void LgMessageAllExcept(StackStrF & msg, LgPlayer & player) {
    const int32_t p = player.GetIdentifier();
    const SQChar * m = msg.mPtr;
    ForeachConnectedPlayer([=](int32_t id) { if (id != p) _Func->SendClientMessage(id, 0x0b5fa5ff, "%s", m); });
}
static void LgPrivMessage(LgPlayer & player, StackStrF & msg) { _Func->SendClientMessage(player.GetIdentifier(), 0x007f16ff, "** pm >> %s", msg.mPtr); }
static void LgPrivMessageAll(StackStrF & msg) {
    const SQChar * m = msg.mPtr;
    ForeachConnectedPlayer([=](int32_t id) { _Func->SendClientMessage(id, 0x007f16ff, "** pm >> %s", m); });
}
static void LgSendPlayerMessage(LgPlayer & source, LgPlayer & target, StackStrF & msg) {
    _Func->SendClientMessage(target.GetIdentifier(), 0x007f16ff, "** pm from %s >> %s", source.Get().GetName(), msg.mPtr);
}
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static const SQChar * LgGetWeaponName(int id) { return GetWeaponName(static_cast< uint32_t >(id)); }
SQMOD_NODISCARD static const SQChar * LgGetDistrictName(float x, float y) { return GetDistrictNameEx(x, y); }
SQMOD_NODISCARD static const SQChar * LgGetSkinName(int id) { return GetSkinName(static_cast< uint32_t >(id)); }
SQMOD_NODISCARD static int LgGetWeaponID(StackStrF & name) { return GetWeaponID(name); }
SQMOD_NODISCARD static size_t LgSQGetTickCount() {
#ifdef SQMOD_OS_WINDOWS
    return GetTickCount();
#else
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) return 0;
    return static_cast< size_t >(now.tv_sec) * 1000u + now.tv_nsec / 1000000u;
#endif
}
// ------------------------------------------------------------------------------------------------
extern bool GetReloadStatus();
extern void SetReloadStatus(bool toggle);
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static float LgDistanceFromPoint (float x1, float y1, float x2, float y2) { return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); }
static void LgReloadScripts() { SetReloadStatus(true); }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static int LgGetVehicleModelFromName(StackStrF & name) { return GetAutomobileID(name); }
SQMOD_NODISCARD static String & LgGetVehicleNameFromModel(int model) { return GetAutomobileName(
            static_cast< uint32_t >(model)); }
static void LgLoadVCMPModule(StackStrF & name) { OutputError("LoadModule() cannot be used by scripts. This functionality is not allowed."); }
SQMOD_NODISCARD static bool LgIsNum(StackStrF & s) {
    if (!s.mLen) return false;
    char * p = nullptr;
    strtol(s.mPtr, &p, 10);
    return (p - s.mPtr) > 0;
}
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static SQInteger LgGetVehicleCount() {
    int count = 0, i = 0;
    while (i < SQMOD_VEHICLE_POOL) {
        if (_Func->CheckEntityExists(vcmpEntityPoolVehicle, i++) >= 1) ++ count;
    }
    return count;
}
SQMOD_NODISCARD SQInteger LgGetPickupCount() {
    int count = 0, i = 0;
    while (i < SQMOD_VEHICLE_POOL) {
        if (_Func->CheckEntityExists(vcmpEntityPoolPickup, i++) >= 1) ++ count;
    }
    return count;
}
SQMOD_NODISCARD SQInteger LgGetObjectCount() {
    int count = 0, i = 0;
    while (i < SQMOD_VEHICLE_POOL) {
        if (_Func->CheckEntityExists(vcmpEntityPoolObject, i++) >= 1) ++ count;
    }
    return count;
}
SQMOD_NODISCARD SQInteger LgGetPlayers() { return ForeachConnectedPlayerCount([](int32_t) { return true; }); }
// ------------------------------------------------------------------------------------------------
static void LgSetVehiclesForcedRespawnHeight(SQFloat height) { _Func->SetVehiclesForcedRespawnHeight(static_cast< float >(height)); }
SQMOD_NODISCARD static SQFloat LgGetVehiclesForcedRespawnHeight() { return _Func->GetVehiclesForcedRespawnHeight(); }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static SQInteger LgFindPlayer(HSQUIRRELVM vm) {
    if (sq_gettop(vm) >= 2) {
        if (sq_gettype(vm, 2) == OT_INTEGER) {
            SQInteger id;
            sq_getinteger(vm, 2, &id);
            if (VALID_ENTITYEX(id, SQMOD_PLAYER_POOL)) {
                Var< LightObj >::push(vm, Core::Get().GetPlayer(static_cast< int32_t >(id)).mLgObj);
            } else {
                sq_pushnull(vm);
                return 1;
            }
        } else if (sq_gettype(vm, 2 ) == OT_STRING) {
            StackStrF val(vm, 2);
            if (SQ_FAILED(val.Proc(true))) { return val.mRes; }
            String name(val.mPtr, static_cast< size_t >(val.mLen));
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c){ return std::tolower(c); });
            char name_buf[SQMOD_NAMELENGTH];
            const int32_t id = ForeachConnectedPlayerUntil([&](int32_t id) -> bool {
                _Func->GetPlayerName(id, name_buf, 64);
                std::transform(name_buf, name_buf + strlen(name_buf), name_buf, [](unsigned char c){ return std::tolower(c); });
                return name.compare(name_buf) == 0; // NOLINT(readability-string-compare)
            });
            if (VALID_ENTITYEX(id, SQMOD_PLAYER_POOL)) Var< LightObj >::push(vm, Core::Get().GetPlayer(id).mLgObj);
            else sq_pushnull(vm);
        } else {
            return sq_throwerror(vm, "Unexpected argument in FindPlayer: must be integer or string");
        }
    } else {
        return sq_throwerror(vm, "Unexpected number of parameters for FindPlayer [integer or string]");
    }
    return 1;
}
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static int LgGetSkinID(StackStrF & name) { return GetSkinID(name); }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static bool LgInternal_InPoly(float fX, float fY, size_t points_n, const Vector2 * points)
{
    // http://sidvind.com/wiki/Point-in-polygon:_Jordan_Curve_Theorem
    // The points creating the polygon
    float x1, x2;
    // How many times the ray crosses a line segment
    int crossings = 0;
    // Iterate through each line
    for (unsigned int i = 0; i < points_n; i++)
    {
        // This is done to ensure that we get the same result when
        // the line goes from left to right and right to left.
        if (points[i].x < points[(i + 1) % points_n].x) {
            x1 = points[i].x;
            x2 = points[(i + 1) % points_n].x;
        } else {
            x1 = points[(i + 1) % points_n].x;
            x2 = points[i].x;
        }
        // First check if the ray is able to cross the line
        if (fX > x1 && fX <= x2 && (fY < points[i].y || fY <= points[(i + 1) % points_n].y)) {
            static const float eps = 0.000001f;
            // Calculate the equation of the line
            float dx = points[(i + 1) % points_n].x - points[i].x;
            float dy = points[(i + 1) % points_n].y - points[i].y;
            float k;
            if (fabsf(dx) < eps) {
                k = static_cast< float >(0xffffffff);
            } else {
                k = dy / dx;
            }
            float m = points[i].y - k * points[i].x;
            // Find if the ray crosses the line
            float y2 = k * fX + m;
            if (fY <= y2) {
                crossings++;
            }
        }
    }
    return (crossings % 2 == 1);
}
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static SQInteger LgInPolyProcStack(HSQUIRRELVM vm) {
    SQFloat x = PopStackFloat(vm, 2), y = PopStackFloat(vm, 3);
    SQInteger top = sq_gettop(vm);
    std::vector< Vector2 > points;
    points.reserve(static_cast<unsigned long long int>((top - 1) / 2));
    for (SQInteger idx = 4; idx < top;) {
        points.emplace_back();
        points.back().x = SQMOD_DOUBLE_OR(static_cast< Vector2::Value >(PopStackFloat(vm, idx++)), PopStackFloat(vm, idx++));
        points.back().y = SQMOD_DOUBLE_OR(static_cast< Vector2::Value >(PopStackFloat(vm, idx++)), PopStackFloat(vm, idx++));
    }
    sq_pushbool(vm, static_cast< SQBool >(LgInternal_InPoly(SQMOD_DOUBLE_OR(static_cast< float >(x), x),
                                                            SQMOD_DOUBLE_OR(static_cast< float >(y), y), points.size(), points.data())));
    return 1;
}
SQMOD_NODISCARD static SQInteger LgInPolyProcString(HSQUIRRELVM vm) {
    SQFloat x = PopStackFloat(vm, 2), y = PopStackFloat(vm, 3);
    StackStrF sq_str(vm, 4);
    if (SQ_FAILED(sq_str.Proc(true))) return sq_str.mRes; // Propagate the error
    std::vector< Vector2 > points;
    points.reserve(static_cast< size_t >(sq_str.mLen) / 8 + 1);
    std::stringstream ss(String(sq_str.mPtr, static_cast< size_t >(sq_str.mLen)));
    for (char c; !ss.eof();) {
        points.emplace_back();
        ss >> points.back().x >> c;
        if (!ss.eof())
            ss >> points.back().y >> c;
        else break;
    }
    sq_pushbool(vm, static_cast< SQBool >(LgInternal_InPoly(SQMOD_DOUBLE_OR(static_cast< float >(x), x),
                                                            SQMOD_DOUBLE_OR(static_cast< float >(y), y), points.size(), points.data())));
    return 1;
}
SQMOD_NODISCARD static SQInteger LgInPolyProcArray(HSQUIRRELVM vm) {
    SQFloat x = PopStackFloat(vm, 2), y = PopStackFloat(vm, 3);
    Array arr(LightObj(4, vm));
    std::vector< Vector2 > points;
    points.reserve(static_cast<unsigned long long int>(arr.Length() / 2 + 1));
    arr.Foreach([&](HSQUIRRELVM vm, SQInteger i) -> SQRESULT {
        if ((i & 1) == 0) {
            points.emplace_back();
            points.back().x = SQMOD_DOUBLE_OR(static_cast< Vector2::Value >(PopStackFloat(vm, -1)), PopStackFloat(vm, -1));
        } else {
            points.back().y = SQMOD_DOUBLE_OR(static_cast< Vector2::Value >(PopStackFloat(vm, -1)), PopStackFloat(vm, -1));
        }
        return SQ_OK;
    });
    sq_pushbool(vm, static_cast< SQBool >(LgInternal_InPoly(SQMOD_DOUBLE_OR(static_cast< float >(x), x),
                                                            SQMOD_DOUBLE_OR(static_cast< float >(y), y), points.size(), points.data())));
    return 1;
}
SQMOD_NODISCARD static SQInteger LgInPoly(HSQUIRRELVM vm) {
    try {
        const SQInteger top = sq_gettop(vm);
        if(top >= 9 && (top - 1) % 2 == 0) {
            return LgInPolyProcStack(vm);
        } else if (top >= 4) {
            if (sq_gettype(vm, 4) == OT_STRING) {
                return LgInPolyProcString(vm);
            } else if (top == 4 && sq_gettype(vm, 4) == OT_ARRAY) {
                return LgInPolyProcArray(vm);
            }
        }
    } catch (const std::exception & e) {
        return sq_throwerror(vm, e.what());
    }
    return sq_throwerror(vm, "Unexpected number or types of parameters for InPoly");
}
// ------------------------------------------------------------------------------------------------
// These functions are for compatibility, but will be deprecated
SQMOD_NODISCARD static SQInteger LgSetAmmuWeapon(HSQUIRRELVM SQ_UNUSED_ARG(vm)) { OutputError("SetAmmuWeapon does not exist in 0.4. Ammunations must be scripted."); return 0; }
SQMOD_NODISCARD static SQInteger LgIsAmmuWeaponEnabled(HSQUIRRELVM vm) { OutputError("IsAmmuWeaponEnabled does not exist in 0.4. Ammunations must be scripted."); sq_pushbool(vm,
                                                                                                                                                                              static_cast<SQBool>(false));  return 1; }
SQMOD_NODISCARD static SQInteger LgSetAmmuWeaponEnabled(HSQUIRRELVM SQ_UNUSED_ARG(vm)) { OutputError("SetAmmuWeaponEnabled does not exist in 0.4. Ammunations must be scripted."); return 0; }
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static uint32_t LgGetTime() {
    OutputError("GetTime is deprecated and may be removed in the future.");
    OutputMessage("Please use Squirrel's time() function instead.");
    return 0;
}
SQMOD_NODISCARD static const SQChar * LgGetFullTime() {
    OutputError("GetFullTime is deprecated and may be removed in the future.");
    OutputMessage("Please use Squirrel's date() function instead.");
    static char date[96];
    std::time_t rt;
    std::time(&rt);
    std::tm * ti = std::localtime(&rt);
    std::strftime(date, 96, "%A, %B %d, %Y, %H:%M:%S %Z", ti);
    return date;
}
// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static bool LgGetFallEnabled() { OutputError("GetFallEnabled has no effect."); return false; }
static void LgSetFallEnabled(bool) { OutputError("SetFallEnabled has no effect."); }
SQMOD_NODISCARD static bool LgGetDeathmatchScoreboard() { OutputError("GetDeathmatchScoreboard has no effect. Use scripts to implement it."); return false; }
static void LgSetDeathmatchScoreboard(bool) { OutputError("GetDeathmatchScoreboard has no effect. Use scripts to implement it."); }
SQMOD_NODISCARD static bool LgGetWeaponSync(int) { OutputError("GetWeaponSync does not exist in 0.4. Rely on the server's anti-hack system and callbacks instead."); return false; }
static void LgSetWeaponSync(int, bool) { OutputError("GetWeaponSync does not exist in 0.4. Rely on the server's anti-hack system and callbacks instead."); }
SQMOD_NODISCARD static bool LgGetWeatherLock() { OutputError("GetWeatherLock has no effect."); return false; }
static void LgSetWeatherLock(bool) { OutputError("SetWeatherLock has no effect."); }

// ================================================================================================
void Register_Official_Functions(HSQUIRRELVM vm)
{
    RootTable(vm)
        .Overload(_SC("ClientMessage"), LgClientMessage)
        .Overload(_SC("ClientMessage"), LgClientMessageWithAlpha)
        .Overload(_SC("ClientMessageToAll"), LgClientMessageToAll)
        .Overload(_SC("ClientMessageToAll"), LgClientMessageToAllWithAlpha)
        .Overload(_SC("Announce"), LgGameMessage)
        .Overload(_SC("Announce"), LgGameMessageAlternate)
        .Overload(_SC("AnnounceAll"), LgGameMessageToAll)
        .Overload(_SC("AnnounceAll"), LgGameMessageToAllAlternate)

        .Func(_SC("ShutdownServer"), LgShutdownServer)
        .Func(_SC("SetServerName"), LgSetServerName)
        .Func(_SC("SetMaxPlayers"), LgSetMaxPlayers)
        .Func(_SC("SetPassword"), LgSetServerPassword)
        .Func(_SC("SetGameModeName"), LgSetGameModeText)
        .Func(_SC("SetTimeRate"), LgSetTimeRate)
        .Func(_SC("SetHour"), LgSetHour)
        .Func(_SC("SetMinute"), LgSetMinute)
        .Func(_SC("SetTime"), LgSetTime)
        .Func(_SC("SetWeather"), LgSetWeather)
        .Func(_SC("SetGravity"), LgSetGravity)
        .Func(_SC("SetGamespeed"), LgSetGamespeed)
        .Func(_SC("SetWaterLevel"), LgSetWaterLevel)
        .Func(_SC("SetMaxHeight"), LgSetMaxHeight)
        .Func(_SC("SetFallTimer"), LgSetFallTimer)

        .SquirrelFunc(_SC("GetServerName"), LgGetServerName)
        .Func(_SC("GetMaxPlayers"), LgGetMaxPlayers)
        .SquirrelFunc(_SC("GetPassword"), LgGetServerPassword)
        .SquirrelFunc(_SC("GetGameModeName"), LgGetGameModeText)
        .Func(_SC("GetTimeRate"), LgGetTimeRate)
        .Func(_SC("GetHour"), LgGetHour)
        .Func(_SC("GetMinute"), LgGetMinute)
        .Func(_SC("GetWeather"), LgGetWeather)
        .Func(_SC("GetGravity"), LgGetGravity)
        .Func(_SC("GetGamespeed"), LgGetGamespeed)
        .Func(_SC("GetWaterLevel"), LgGetWaterLevel)
        .Func(_SC("GetMaxHeight"), LgGetMaxHeight)
        .Func(_SC("GetFallTimer"), LgGetFallTimer)

        .Func(_SC("SetSyncFrameLimiter"), LgToggleSyncFrameLimiter)
        .Func(_SC("SetFrameLimiter"), LgToggleFrameLimiter)
        .Func(_SC("SetTaxiBoostJump"), LgToggleTaxiBoostJump)
        .Func(_SC("SetDriveOnWater"), LgToggleDriveOnWater)
        .Func(_SC("SetFastSwitch"), LgToggleFastSwitch)
        .Func(_SC("SetFriendlyFire"), LgToggleFriendlyFire)
        .Func(_SC("SetDrivebyEnabled"), LgToggleDisableDriveby)
        .Func(_SC("SetPerfectHandling"), LgTogglePerfectHandling)
        .Func(_SC("SetFlyingCars"), LgToggleFlyingCars)
        .Func(_SC("SetJumpSwitch"), LgToggleJumpSwitch)
        .Func(_SC("SetShowOnRadar"), LgToggleShowOnRadar)
        .Func(_SC("SetShowOnlyTeamMarkers"), LgToggleShowOnlyTeamMarkers)
        .Func(_SC("SetStuntBike"), LgToggleStuntBike)
        .Func(_SC("SetShootInAir"), LgToggleShootInAir)
        .Func(_SC("SetShowNametags"), LgToggleShowNametags)
        .Func(_SC("SetJoinMessages"), LgToggleJoinMessages)
        .Func(_SC("SetDeathMessages"), LgToggleDeathMessages)
        .Func(_SC("SetChatTagDefault"), LgToggleChatTagDefault)
        .Func(_SC("SetWallglitch"), LgToggleWallglitch)
        .Func(_SC("SetBackfaceCullingDisabled"), LgToggleDisableBackfaceCulling)
        .Func(_SC("SetHeliBladeDamageDisabled"), LgToggleDisableHeliBladeDamage)

        .Func(_SC("GetSyncFrameLimiter"), LgEnabledSyncFrameLimiter)
        .Func(_SC("GetFrameLimiter"), LgEnabledFrameLimiter)
        .Func(_SC("GetTaxiBoostJump"), LgEnabledTaxiBoostJump)
        .Func(_SC("GetDriveOnWater"), LgEnabledDriveOnWater)
        .Func(_SC("GetFastSwitch"), LgEnabledFastSwitch)
        .Func(_SC("GetFriendlyFire"), LgEnabledFriendlyFire)
        .Func(_SC("GetDrivebyEnabled"), LgEnabledDisableDriveby)
        .Func(_SC("GetPerfectHandling"), LgEnabledPerfectHandling)
        .Func(_SC("GetFlyingCars"), LgEnabledFlyingCars)
        .Func(_SC("GetJumpSwitch"), LgEnabledJumpSwitch)
        .Func(_SC("GetShowOnRadar"), LgEnabledShowOnRadar)
        .Func(_SC("GetShowOnlyTeamMarkers"), LgEnabledShowOnlyTeamMarkers)
        .Func(_SC("GetStuntBike"), LgEnabledStuntBike)
        .Func(_SC("GetShootInAir"), LgEnabledShootInAir)
        .Func(_SC("GetShowNametags"), LgEnabledShowNametags)
        .Func(_SC("GetJoinMessages"), LgEnabledJoinMessages)
        .Func(_SC("GetDeathMessages"), LgEnabledDeathMessages)
        .Func(_SC("GetChatTagDefault"), LgEnabledChatTagDefault)
        .Func(_SC("GetWallglitch"), LgEnabledWallglitch)
        .Func(_SC("GetBackfaceCullingDisabled"), LgEnabledDisableBackfaceCulling)
        .Func(_SC("GetHeliBladeDamageDisabled"), LgEnabledDisableHeliBladeDamage)

        .Overload(_SC("CreateRadioStream"), LgCreateRadioStreamWithID)
        .Overload(_SC("CreateRadioStream"), LgCreateRadioStream)
        .Func(_SC("DestroyRadioStream"), LgDestroyRadioStream)

        .Func(_SC("CreateMarker"), LgCreateBlip)
        .Func(_SC("DestroyMarker"), LgDestroyBlip)

        .Overload(_SC("CreateExplosion"), LgCreateExplosion)
        .Overload(_SC("CreateExplosion"), LgCreateExplosionExpanded)

        .Overload(_SC("PlaySound"), LgPlayGameSound)
        .Overload(_SC("PlaySound"), LgPlayGameSoundExpanded)
        .Func(_SC("PlaySoundForPlayer"), LgPlayGameSoundForPlayer)
        .Func(_SC("PlaySoundForWorld"), LgPlayGameSoundForWorld)

        .Func(_SC("SetUseClasses"), LgSetUseClasses)
        .Func(_SC("UsingClasses"), LgUsingClasses)
        .Func(_SC("AddClass"), LgAddClass)

        .Overload(_SC("SetSpawnPlayerPos"), LgSetSpawnPlayerPos)
        .Overload(_SC("SetSpawnCameraPos"), LgSetSpawnCameraPos)
        .Overload(_SC("SetSpawnCameraLook"), LgSetSpawnCameraLook)

        .Overload(_SC("SetSpawnPlayerPos"), LgSetSpawnPlayerPosExpanded)
        .Overload(_SC("SetSpawnCameraPos"), LgSetSpawnCameraPosExpanded)
        .Overload(_SC("SetSpawnCameraLook"), LgSetSpawnCameraLookExpanded)

        .Func(_SC("BanIP"), LgBanIP)
        .Func(_SC("UnbanIP"), LgUnbanIP)
        .Func(_SC("IsIPBanned"), LgIsIPBanned)

        .Func(_SC("GetPlayerIDFromName"), LgGetPlayerIDFromName)
        .Func(_SC("IsWorldCompatibleWithPlayer"), LgIsWorldCompatibleWithPlayer)

        .Func(_SC("CreateCheckpoint"), LgCreateCheckpoint)

        .Overload(_SC("CreateVehicle"), LgCreateVehicleCompat)
        .Overload(_SC("CreateVehicle"), LgCreateVehicle)
        .Overload(_SC("CreateVehicle"), LgCreateVehicleExpanded)

        .Overload(_SC("CreatePickup"), LgCreatePickupCompat)
        .Overload(_SC("CreatePickup"), LgCreatePickup)
        .Overload(_SC("CreatePickup"), LgCreatePickupExpanded)

        .Overload(_SC("CreateObject"), LgCreateObject)
        .Overload(_SC("CreateObject"), LgCreateObjectExpanded)

        .Func(_SC("FindPickup"), LgFindPickup)
        .Func(_SC("FindObject"), LgFindObject)
        .Func(_SC("FindVehicle"), LgFindVehicle)
        .Func(_SC("FindCheckpoint"), LgFindCheckpoint)

        .Func(_SC("SetWorldBounds"), LgSetWorldBounds)
        .Func(_SC("GetWorldBounds"), LgGetWorldBounds)

        .Func(_SC("SetWastedSettings"), LgSetWastedSettings)
        .Func(_SC("GetWastedSettings"), LgGetWastedSettings)
        .Func(_SC("SetKillDelay"), LgSetKillDelay)
        .Func(_SC("GetKillDelay"), LgGetKillDelay)

        .Func(_SC("RawHideMapObject"), LgRawHideMapObject)
        .Func(_SC("HideMapObject"), LgHideMapObject)
        .Func(_SC("ShowMapObject"), LgShowMapObject)
        .Func(_SC("ShowAllMapObjects"), LgShowAllMapObjects)

        .Func(_SC("ForceAllSelect"), LgForceAllSelect)

        .Func(_SC("ResetAllVehicleHandling"), LgResetAllVehicleHandling)
        .Func(_SC("IsHandlingRuleSet"), LgIsHandlingRuleSet)
        .Func(_SC("SetHandlingRule"), LgSetHandlingRule)
        .Func(_SC("GetHandlingRule"), LgGetHandlingRule)
        .Func(_SC("ResetHandlingRule"), LgResetHandlingRule)
        .Func(_SC("ResetVehicleHandling"), LgResetVehicleHandling)

        // Compatibility functions
        .Func(_SC("GetCinematicBorder"), LgGetCinematicBorder)
        .Func(_SC("GetGreenScanLines"), LgGetGreenScanLines)
        .Func(_SC("GetWhiteScanLines"), LgGetWhiteScanLines)
        .Func(_SC("SetCinematicBorder"), LgSetCinematicBorder)
        .Func(_SC("SetGreenScanLines"), LgSetGreenScanLines)
        .Func(_SC("SetWhiteScanLines"), LgSetWhiteScanLines)
        .Func(_SC("KickPlayer"), LgKickPlayer)
        .Func(_SC("BanPlayer"), LgBanPlayer)
        .Func(_SC("Message"), LgMessage)
        .Func(_SC("MessagePlayer"), LgMessagePlayer)
        .Func(_SC("MessageAllExcept"), LgMessageAllExcept)
        .Func(_SC("PrivMessage"), LgPrivMessage)
        .Func(_SC("PrivMessageAll"), LgPrivMessageAll)
        .Func(_SC("SendPlayerMessage"), LgSendPlayerMessage)

        .Func(_SC("GetWeaponName"), LgGetWeaponName)
        .Func(_SC("GetDistrictName"), LgGetDistrictName)
        .Func(_SC("GetSkinName"), LgGetSkinName)
        .Func(_SC("GetWeaponID"), LgGetWeaponID)
        .Func(_SC("GetSkinID"), LgGetSkinID)
        .Func(_SC("GetTickCount"), LgSQGetTickCount)

        .Func(_SC("SetWeaponDataValue"), LgSetWeaponDataValue)
        .Func(_SC("GetWeaponDataValue"), LgGetWeaponDataValue)
        .Func(_SC("ResetWeaponDataValue"), LgResetWeaponDataValue)
        .Func(_SC("IsWeaponDataModified"), LgIsWeaponDataModified)
        .Func(_SC("ResetWeaponData"), LgResetWeaponData)
        .Func(_SC("ResetAllWeaponData"), LgResetAllWeaponData)

        .Func(_SC("DistanceFromPoint"), LgDistanceFromPoint)
        .Func(_SC("ReloadScripts"), LgReloadScripts)

        .Func(_SC("GetVehicleModelFromName"), LgGetVehicleModelFromName)
        .Func(_SC("GetVehicleNameFromModel"), LgGetVehicleNameFromModel)
        .Func(_SC("IsNum"), LgIsNum)

        .Func(_SC("GetVehicleCount"), LgGetVehicleCount)
        .Func(_SC("GetPickupCount"), LgGetPickupCount)
        .Func(_SC("GetObjectCount"), LgGetObjectCount)
        .Func(_SC("GetPlayers"), LgGetPlayers)

        .Func(_SC("GetFallEnabled"), LgGetFallEnabled)
        .Func(_SC("SetFallEnabled"), LgSetFallEnabled)
        .Func(_SC("GetDeathmatchScoreboard"), LgGetDeathmatchScoreboard)
        .Func(_SC("SetDeathmatchScoreboard"), LgSetDeathmatchScoreboard)
        .Func(_SC("GetWeaponSync"), LgGetWeaponSync)
        .Func(_SC("SetWeaponSync"), LgSetWeaponSync)
        .Func(_SC("SetWeatherLock"), LgSetWeatherLock)
        .Func(_SC("GetWeatherLock"), LgGetWeatherLock)

        .Func(_SC("GetTime"), LgGetTime)
        .Func(_SC("GetFullTime"), LgGetFullTime)
        .Func(_SC("LoadModule"), LgLoadVCMPModule)

        .Func(_SC("GetVehiclesForcedRespawnHeight"), LgGetVehiclesForcedRespawnHeight)
        .Func(_SC("SetVehiclesForcedRespawnHeight"), LgSetVehiclesForcedRespawnHeight)

        .SquirrelFunc(_SC("FindPlayer"), LgFindPlayer)
        .SquirrelFunc(_SC("InPoly"), LgInPoly)

        .SquirrelFunc(_SC("SetAmmuWeapon"), LgSetAmmuWeapon)
        .SquirrelFunc(_SC("IsAmmuWeaponEnabled"), LgIsAmmuWeaponEnabled)
        .SquirrelFunc(_SC("SetAmmuWeaponEnabled"), LgSetAmmuWeaponEnabled)

        .Func(_SC("BindKey"), LgBindKey)
        .Func(_SC("UnbindKey"), LgRemoveKeybind)
        .Func(_SC("UnbindAll"), LgRemoveAllKeybinds)
    ;
}

// ------------------------------------------------------------------------------------------------
static const EnumElement g_LgConstants[] = {
    {_SC("WEP_FIST"), 0},
    {_SC("WEP_BRASSKNUCKLES"), 1},
    {_SC("WEP_SCREWDRIVER"), 2},
    {_SC("WEP_GOLFCLUB"), 3},
    {_SC("WEP_NIGHTSTICK"), 4},
    {_SC("WEP_KNIFE"), 5},
    {_SC("WEP_BASEBALLBAT"), 6},
    {_SC("WEP_HAMMER"), 7},
    {_SC("WEP_MEATCLEAVER"), 8},
    {_SC("WEP_MACHETE"), 9},
    {_SC("WEP_KATANA"), 10},
    {_SC("WEP_CHAINSAW"), 11},
    {_SC("WEP_GRENADE"), 12},
    {_SC("WEP_REMOTE"), 13},
    {_SC("WEP_TEARGAS"), 14},
    {_SC("WEP_MOLOTOV"), 15},
    {_SC("WEP_ROCKET"), 16},
    {_SC("WEP_COLT"), 17},
    {_SC("WEP_COLT45"), 17},
    {_SC("WEP_PYTHON"), 18},
    {_SC("WEP_SHOTGUN"), 19},
    {_SC("WEP_SPAZ"), 20},
    {_SC("WEP_SPAS12"), 20},
    {_SC("WEP_STUBBY"), 21},
    {_SC("WEP_TEC9"), 22},
    {_SC("WEP_UZI"), 23},
    {_SC("WEP_INGRAM"), 24},
    {_SC("WEP_MP5"), 25},
    {_SC("WEP_M4"), 26},
    {_SC("WEP_RUGER"), 27},
    {_SC("WEP_SNIPER"), 28},
    {_SC("WEP_LASERSCOPE"), 29},
    {_SC("WEP_ROCKETLAUNCHER"), 30},
    {_SC("WEP_RPG"), 30},
    {_SC("WEP_FLAMETHROWER"), 31},
    {_SC("WEP_M60"), 32},
    {_SC("WEP_MINIGUN"), 33},

    {_SC("WEP_BOMB"), 34},
    {_SC("WEP_HELICANNON"), 35},
    {_SC("WEP_CAMERA"), 36},
    {_SC("WEP_VEHICLE"), 39},
    {_SC("WEP_DRIVEBY"), 42},
    {_SC("WEP_DROWNED"), 43},
    {_SC("WEP_FALL"), 44},
    {_SC("WEP_EXPLOSION"), 51},
    {_SC("WEP_SUICIDE"), 70},

    {_SC("WEATHER_MOSTLYCLEAR"), 0},
    {_SC("WEATHER_OVERCAST"), 1},
    {_SC("WEATHER_THUNDERSTORM"), 2},
    {_SC("WEATHER_STORM"), 2},
    {_SC("WEATHER_STORMY"), 2},
    {_SC("WEATHER_FOGGY"), 3},
    {_SC("WEATHER_FOG"), 3},
    {_SC("WEATHER_CLEAR"), 4},
    {_SC("WEATHER_SUNNY"), 4},
    {_SC("WEATHER_RAIN"), 5},
    {_SC("WEATHER_RAINY"), 5},
    {_SC("WEATHER_DARKCLOUDY"), 6},
    {_SC("WEATHER_LIGHTCLOUDY"), 7},
    {_SC("WEATHER_OVERCASTCLOUDY"), 8},
    {_SC("WEATHER_BLACKCLOUDS"), 9},

    {_SC("VEH_LANDSTALKER"), 130},
    {_SC("VEH_IDAHO"), 131},
    {_SC("VEH_STINGER"), 132},
    {_SC("VEH_LINERUNNER"), 133},
    {_SC("VEH_PERENNIAL"), 134},
    {_SC("VEH_SENTINEL"), 135},
    {_SC("VEH_RIO"), 136},
    {_SC("VEH_FIRETRUCK"), 137},
    {_SC("VEH_TRASHMASTER"), 138},
    {_SC("VEH_STRETCH"), 139},
    {_SC("VEH_MANANA"), 140},
    {_SC("VEH_INFERNUS"), 141},
    {_SC("VEH_VOODOO"), 142},
    {_SC("VEH_PONY"), 143},
    {_SC("VEH_MULE"), 144},
    {_SC("VEH_CHEETAH"), 145},
    {_SC("VEH_AMBULANCE"), 146},
    {_SC("VEH_FBIWASHINGTON"), 147},
    {_SC("VEH_MOONBEAM"), 148},
    {_SC("VEH_ESPERANTO"), 149},
    {_SC("VEH_TAXI"), 150},
    {_SC("VEH_WASHINGTON"), 151},
    {_SC("VEH_BOBCAT"), 152},
    {_SC("VEH_MRWHOOPEE"), 153},
    {_SC("VEH_BFINJECTION"), 154},
    {_SC("VEH_HUNTER"), 155},
    {_SC("VEH_POLICE"), 156},
    {_SC("VEH_ENFORCER"), 157},
    {_SC("VEH_SECURICAR"), 158},
    {_SC("VEH_BANSHEE"), 159},
    {_SC("VEH_PREDATOR"), 160},
    {_SC("VEH_BUS"), 161},
    {_SC("VEH_RHINO"), 162},
    {_SC("VEH_BARRACKSOL"), 163},
    {_SC("VEH_BARRACKS"), 163},
    {_SC("VEH_CUBANHERMES"), 164},
    {_SC("VEH_HELICOPTER"), 165},
    {_SC("VEH_ANGEL"), 166},
    {_SC("VEH_COACH"), 167},
    {_SC("VEH_CABBIE"), 168},
    {_SC("VEH_STALLION"), 169},
    {_SC("VEH_RUMPO"), 170},
    {_SC("VEH_RCBANDIT"), 171},
    {_SC("VEH_HEARSE"), 172},
    {_SC("VEH_PACKER"), 173},
    {_SC("VEH_SENTINELXS"), 174},
    {_SC("VEH_ADMIRAL"), 175},
    {_SC("VEH_SQUALO"), 176},
    {_SC("VEH_SEASPARROW"), 177},
    {_SC("VEH_PIZZABOY"), 178},
    {_SC("VEH_GANGBURRITO"), 179},
    {_SC("VEH_AIRTRAIN"), 180},
    {_SC("VEH_DEADDODO"), 181},
    {_SC("VEH_SPEEDER"), 182},
    {_SC("VEH_REEFER"), 183},
    {_SC("VEH_TROPIC"), 184},
    {_SC("VEH_FLATBED"), 185},
    {_SC("VEH_YANKEE"), 186},
    {_SC("VEH_CADDY"), 187},
    {_SC("VEH_ZEBRA"), 188},
    {_SC("VEH_ZEBRACAB"), 188},
    {_SC("VEH_TOPFUN"), 189},
    {_SC("VEH_SKIMMER"), 190},
    {_SC("VEH_PCJ600"), 191},
    {_SC("VEH_PCJ"), 191},
    {_SC("VEH_FAGGIO"), 192},
    {_SC("VEH_FREEWAY"), 193},
    {_SC("VEH_RCBARON"), 194},
    {_SC("VEH_RCRAIDER"), 195},
    {_SC("VEH_GLENDALE"), 196},
    {_SC("VEH_OCEANIC"), 197},
    {_SC("VEH_SANCHEZ"), 198},
    {_SC("VEH_SPARROW"), 199},
    {_SC("VEH_PATRIOT"), 200},
    {_SC("VEH_LOVEFIST"), 201},
    {_SC("VEH_COASTGUARD"), 202},
    {_SC("VEH_DINGHY"), 203},
    {_SC("VEH_HERMES"), 204},
    {_SC("VEH_SABRE"), 205},
    {_SC("VEH_SABRETURBO"), 206},
    {_SC("VEH_PHOENIX"), 207},
    {_SC("VEH_WALTON"), 208},
    {_SC("VEH_REGINA"), 209},
    {_SC("VEH_COMET"), 210},
    {_SC("VEH_DELUXO"), 211},
    {_SC("VEH_BURRITO"), 212},
    {_SC("VEH_SPANDEX"), 213},
    {_SC("VEH_SPANDEXPRESS"), 213},
    {_SC("VEH_MARQUIS"), 214},
    {_SC("VEH_BAGGAGE"), 215},
    {_SC("VEH_BAGGAGEHANDLER"), 215},
    {_SC("VEH_KAUFMAN"), 216},
    {_SC("VEH_KAUFMANCAB"), 216},
    {_SC("VEH_MAVERICK"), 217},
    {_SC("VEH_VCNMAVERICK"), 218},
    {_SC("VEH_RANCHER"), 219},
    {_SC("VEH_FBIRANCHER"), 220},
    {_SC("VEH_VIRGO"), 221},
    {_SC("VEH_GREENWOOD"), 222},
    {_SC("VEH_CUBANJETMAX"), 223},
    {_SC("VEH_HOTRING1"), 224},
    {_SC("VEH_HOTRINGRACER1"), 224},
    {_SC("VEH_SANDKING"), 225},
    {_SC("VEH_BLISTA"), 226},
    {_SC("VEH_BLISTAC"), 226},
    {_SC("VEH_BLISTACOMPACT"), 226},
    {_SC("VEH_COMPACT"), 226},
    {_SC("VEH_POLICEMAV"), 227},
    {_SC("VEH_POLICEMAVERICK"), 227},
    {_SC("VEH_BOXVILLE"), 228},
    {_SC("VEH_BENSON"), 229},
    {_SC("VEH_MESA"), 230},
    {_SC("VEH_MESAGRANDE"), 230},
    {_SC("VEH_RCGOBLIN"), 231},
    {_SC("VEH_HOTRING2"), 232},
    {_SC("VEH_HOTRINGRACER2"), 232},
    {_SC("VEH_HOTRING3"), 233},
    {_SC("VEH_HOTRINGRACER3"), 233},
    {_SC("VEH_BLOODRING1"), 234},
    {_SC("VEH_BLOODRINGBANGER1"), 234},
    {_SC("VEH_BLOODRING2"), 235},
    {_SC("VEH_BLOODRINGBANGER2"), 235},
    {_SC("VEH_VICECHEE"), 236},
    {_SC("VEH_POLICECHEETAH"), 236},
    {_SC("VEH_FBICHEETAH"), 236},
    {_SC("VEH_CHEETAH2"), 236},

    {_SC("UPDATE_PLAYER_ONFOOT"), 0},
    {_SC("UPDATE_PLAYER_FOOT"), 0},
    {_SC("UPDATE_PLAYER_AIM"), 1},
    {_SC("UPDATE_PLAYER_DRIVER"), 2},
    {_SC("UPDATE_PLAYER_PASSENGER"), 3},

    {_SC("UPDATE_VEHICLE_DRIVER"), 0},
    {_SC("UPDATE_VEHICLE_OTHER"), 1},

    {_SC("BODYPART_BODY"), 0},
    {_SC("BODYPART_TORSO"), 1},
    {_SC("BODYPART_LEFTARM"), 2},
    {_SC("BODYPART_LARM"), 2},
    {_SC("BODYPART_RIGHTARM"), 3},
    {_SC("BODYPART_RARM"), 3},
    {_SC("BODYPART_LEFTLEG"), 4},
    {_SC("BODYPART_LLEG"), 4},
    {_SC("BODYPART_RIGHTLEG"), 5},
    {_SC("BODYPART_RLEG"), 5},
    {_SC("BODYPART_HEAD"), 6},

    {_SC("PARTREASON_QUIT"), 1},
    {_SC("PARTREASON_DISCONNECTED"), 1},
    {_SC("PARTREASON_TIMEOUT"), 0},
    {_SC("PARTREASON_KICKEDBANNED"), 2},
    {_SC("PARTREASON_KICKED"), 2},
    {_SC("PARTREASON_BANNED"), 2},
    {_SC("PARTREASON_CRASHED"), 3},

    {_SC("KEY_ONFOOT_FORWARD"), 32768},
    {_SC("KEY_ONFOOT_BACKWARD"), 16384},
    {_SC("KEY_ONFOOT_LEFT"), 8192},
    {_SC("KEY_ONFOOT_RIGHT"), 4096},
    {_SC("KEY_ONFOOT_JUMP"), 2176},
    {_SC("KEY_ONFOOT_SPRINT"), 1024},
    {_SC("KEY_ONFOOT_FIRE"), 576},
    {_SC("KEY_ONFOOT_CROUCH"), 288},
    {_SC("KEY_ONFOOT_PUNCH"), 64},
    {_SC("KEY_ONFOOT_NEXTWEP"), 2},
    {_SC("KEY_ONFOOT_PREVWEP"), 4},
    {_SC("KEY_ONFOOT_AIM"), 1},

    {_SC("KEY_INCAR_LEFT"), 8192},
    {_SC("KEY_INCAR_RIGHT"), 4096},
    {_SC("KEY_INCAR_BACKWARD"), 2176},
    {_SC("KEY_INCAR_FORWARD"), 1024},
    {_SC("KEY_INCAR_HORN"), 288},
    {_SC("KEY_INCAR_LEANUP"), 16},
    {_SC("KEY_INCAR_LEANDOWN"), 8},
    {_SC("KEY_INCAR_LOOKLEFT"), 4},
    {_SC("KEY_INCAR_LOOKRIGHT"), 2},
    // 0.4-squirrel version 3.99 (up to 4.00 on final release)
    {_SC("SQUIRREL_VERSION"), 399}
};

// ================================================================================================
void Register_Official_Constants(HSQUIRRELVM vm)
{
    ConstTable t(vm);
    for (const auto & c : g_LgConstants) {
        switch (c.Type) {
            case SQET_STRING:   t.Const(c.Name, c.mSTR); break;
            case SQET_INT:      t.Const(c.Name, c.mINT); break;
            case SQET_REAL:     t.Const(c.Name, c.mREAL); break;
            case SQET_BOOL:     t.Const(c.Name, c.mBOOL); break;
            default:            SQTHROW(t.GetVM(), "Unknown constant value type");
        }
    }
}

// ------------------------------------------------------------------------------------------------
struct LgStream {
    static constexpr size_t MAX_SIZE = 4096;
    // --------------------------------------------------------------------------------------------
    static void ClearOutput() {
        m_OutputStreamPosition = m_OutputStreamEnd = 0;
        m_OutputStreamError = false;
    }
    static void ClearInput() {
        m_InputStreamSize = m_InputStreamPosition = 0;
        m_InputStreamError = false;
    }
    // --------------------------------------------------------------------------------------------
    static void StartWrite() { ClearOutput(); }
    static void SetWritePosition(size_t position) {
        if (position < m_OutputStreamEnd) m_OutputStreamPosition = position;
        else m_OutputStreamPosition = m_OutputStreamEnd;
    }
    static size_t GetWritePosition() { return m_OutputStreamPosition; }
    static size_t GetWriteSize() { return m_OutputStreamEnd; }
    static bool HasWriteError() { return m_OutputStreamError; }
    static void WriteByte(uint8_t value) { Write(&value, sizeof(value)); }
    static void WriteInt(int32_t value) { Write(&value, sizeof(value)); }
    static void WriteFloat(float value) { Write(&value, sizeof(value)); }
    static void WriteString(StackStrF & value) {
        uint16_t length = ConvTo< uint16_t >::From(value.mLen);
        if (CanWrite(sizeof(length))) {
            if (!CanWrite(length)) {
                length = static_cast< uint16_t >(MAX_SIZE - m_OutputStreamPosition);
                m_OutputStreamError = true;
            }
            auto lengthBE = static_cast< uint16_t >(((length >> 8u) & 0xFFu) | ((length & 0xFFu) << 8u));
            Write(&lengthBE, sizeof(lengthBE));
            Write(value.mPtr, length);
        }
    }
    static void SendStream(LightObj & target) {
        int32_t id;
        if (target.IsNull()) id = -1;
        else if (target.GetType() == OT_INTEGER || target.GetType() == OT_FLOAT) id = target.Cast< int32_t >();
        else if (static_cast< AbstractStaticClassData * >(target.GetTypeTag()) == StaticClassTypeTag< LgPlayer >::Get()) {
            id = target.CastI< LgPlayer >()->GetIdentifier();
        } else STHROWF("Invalid target type");
        if (id >= SQMOD_PLAYER_POOL) STHROWF("Invalid player ID");
        _Func->SendClientScriptData(id, m_OutputStreamData, m_OutputStreamEnd);
        ClearOutput();
    }
    // --------------------------------------------------------------------------------------------
    static void LoadInput(const void * data, size_t size) {
        ClearInput();
        m_InputStreamSize = (size < sizeof(m_InputStreamData)) ? size : sizeof(m_InputStreamData);
        memcpy(m_InputStreamData, data, m_InputStreamSize);
    }
    static void SetReadPosition(size_t position) {
        if (position <= m_InputStreamPosition) m_InputStreamPosition = position;
        else m_InputStreamPosition = m_InputStreamSize;
    }
    static size_t GetReadPosition() { return m_InputStreamPosition; }
    static size_t GetReadSize() { return m_InputStreamSize; }
    static bool HasReadError() { return m_InputStreamError; }
    static uint8_t ReadByte() {
        if ((m_InputStreamPosition + sizeof(uint8_t)) <= m_InputStreamSize) {
            return m_InputStreamData[m_InputStreamPosition++];
        } else m_InputStreamError = true;
        return 0;
    }
    static int32_t ReadInt() {
        if ((m_InputStreamPosition + sizeof(int32_t)) <= m_InputStreamSize) {
            int32_t result;
            memcpy(&result, &m_InputStreamData[m_InputStreamPosition], sizeof(result));
            m_InputStreamPosition += sizeof(int32_t);
            return result;
        } else m_InputStreamError = true;
        return 0;
    }
    static float ReadFloat() {
        if ((m_InputStreamPosition + sizeof(float)) <= m_InputStreamSize) {
            float result;
            memcpy(&result, &m_InputStreamData[m_InputStreamPosition], sizeof(result));
            m_InputStreamPosition += sizeof(float);
            return result;
        } else m_InputStreamError = true;
        return 0.0f;
    }
    static LightObj ReadString() {
        uint16_t length = ReadBEInt16();
        if ((m_InputStreamPosition + length) > m_InputStreamSize) {
            length = static_cast< uint16_t >(m_InputStreamSize - m_InputStreamPosition);
            m_InputStreamError = true;
        }
        length = static_cast< uint16_t >(length < (MAX_SIZE - 1) ? length : (MAX_SIZE - 1));
        memcpy(m_Buffer, &m_InputStreamData[m_InputStreamPosition], length);
        m_Buffer[length] = '\0';
        m_InputStreamPosition += length;
        return LightObj(m_Buffer, static_cast< SQInteger >(length));
    }
    // --------------------------------------------------------------------------------------------
    static LightObj CloneInputToBuffer() {
        return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(), Buffer(m_Buffer,
                static_cast< Buffer::SzType >(m_InputStreamSize),
                static_cast< Buffer::SzType >(m_InputStreamPosition)));
    }
    static LightObj CloneOutputToBuffer() {
        return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(), Buffer(m_Buffer,
                static_cast< Buffer::SzType >(m_OutputStreamEnd),
                static_cast< Buffer::SzType >(m_OutputStreamPosition)));
    }
private:
    // --------------------------------------------------------------------------------------------
    static bool CanWrite(size_t size) { return (size <= (MAX_SIZE - m_OutputStreamPosition)); }
    static void Write(const void * value, size_t size) {
        if (CanWrite(size)) {
            memcpy(&m_OutputStreamData[m_OutputStreamPosition], value, size);
            m_OutputStreamPosition += size;
            if (m_OutputStreamPosition > m_OutputStreamEnd) m_OutputStreamEnd = m_OutputStreamPosition;
        } else m_OutputStreamError = true;
    }
    // --------------------------------------------------------------------------------------------
    static uint16_t ReadBEInt16() {
        if ((m_InputStreamPosition + sizeof(uint16_t)) <= m_InputStreamSize) {
            uint16_t result;
            memcpy(&result, &m_InputStreamData[m_InputStreamPosition], sizeof(result));
            m_InputStreamPosition += sizeof(uint16_t);
            return static_cast< uint16_t >(((result >> 8u) & 0xFFu) | ((result & 0xFFu) << 8u));
        } m_InputStreamError = true;
        return 0;
    }
    // --------------------------------------------------------------------------------------------
    static uint8_t  m_InputStreamData[MAX_SIZE];
    static size_t   m_InputStreamSize;
    static size_t   m_InputStreamPosition;
    static bool     m_InputStreamError;
    // --------------------------------------------------------------------------------------------
    static uint8_t  m_OutputStreamData[MAX_SIZE];
    static size_t   m_OutputStreamPosition;
    static size_t   m_OutputStreamEnd;
    static bool     m_OutputStreamError;
    // --------------------------------------------------------------------------------------------
    static SQChar   m_Buffer[MAX_SIZE];
};
// ------------------------------------------------------------------------------------------------
uint8_t LgStream::m_InputStreamData[LgStream::MAX_SIZE]{};
size_t  LgStream::m_InputStreamSize{0};
size_t  LgStream::m_InputStreamPosition{0};
bool    LgStream::m_InputStreamError{false};
// ------------------------------------------------------------------------------------------------
uint8_t LgStream::m_OutputStreamData[LgStream::MAX_SIZE]{};
size_t  LgStream::m_OutputStreamEnd{0};
size_t  LgStream::m_OutputStreamPosition{0};
bool    LgStream::m_OutputStreamError{false};
// ------------------------------------------------------------------------------------------------
SQChar LgStream::m_Buffer[LgStream::MAX_SIZE];

// ------------------------------------------------------------------------------------------------
void LgStreamLoadInput(const void * data, size_t size) { LgStream::LoadInput(data, size); }

// ================================================================================================
void Register_Official_Stream(HSQUIRRELVM vm)
{
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(_SC("Stream"),
        Class< LgStream, NoConstructor< LgStream > >(vm, "Stream")
            .StaticFunc(_SC("StartWrite"), &LgStream::StartWrite)
            .StaticFunc(_SC("SetWritePosition"), &LgStream::SetWritePosition)
            .StaticFunc(_SC("GetWritePosition"), &LgStream::GetWritePosition)
            .StaticFunc(_SC("GetWriteSize"), &LgStream::GetWriteSize)
            .StaticFunc(_SC("HasWriteError"), &LgStream::HasWriteError)
            .StaticFunc(_SC("WriteByte"), &LgStream::WriteByte)
            .StaticFunc(_SC("WriteInt"), &LgStream::WriteInt)
            .StaticFunc(_SC("WriteFloat"), &LgStream::WriteFloat)
            .StaticFunc(_SC("WriteString"), &LgStream::WriteString)
            .StaticFunc(_SC("SendStream"), &LgStream::SendStream)
            .StaticFunc(_SC("SetReadPosition"), &LgStream::SetReadPosition)
            .StaticFunc(_SC("GetReadPosition"), &LgStream::GetReadPosition)
            .StaticFunc(_SC("GetReadSize"), &LgStream::GetReadSize)
            .StaticFunc(_SC("HasReadError"), &LgStream::HasReadError)
            .StaticFunc(_SC("ReadByte"), &LgStream::ReadByte)
            .StaticFunc(_SC("ReadInt"), &LgStream::ReadInt)
            .StaticFunc(_SC("ReadFloat"), &LgStream::ReadFloat)
            .StaticFunc(_SC("ReadString"), &LgStream::ReadString)
            .StaticFunc(_SC("CloneInputToBuffer"), &LgStream::CloneInputToBuffer)
            .StaticFunc(_SC("CloneOutputToBuffer"), &LgStream::CloneOutputToBuffer)
    );
}

} // Namespace:: SqMod
