// ------------------------------------------------------------------------------------------------
#include "Misc/Official.hpp"
#include "Base/Vector2.hpp"
#include "Core/Utility.hpp"
// ------------------------------------------------------------------------------------------------
#include "Core/Entity.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Entity/Checkpoint.hpp"
#include "Entity/KeyBind.hpp"
#include "Entity/Object.hpp"
#include "Entity/Pickup.hpp"
#include "Entity/Player.hpp"
#include "Entity/Vehicle.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(RGBTn, _SC("RGB"))
SQMOD_DECL_TYPENAME(EntityRGBTn, _SC("LgEntityRGB"))
SQMOD_DECL_TYPENAME(RGBATn, _SC("RGBA"))
SQMOD_DECL_TYPENAME(ARGBTn, _SC("ARGB"))
SQMOD_DECL_TYPENAME(VectorTn, _SC("Vector"))
SQMOD_DECL_TYPENAME(QuaternionTn, _SC("Quaternion"))
SQMOD_DECL_TYPENAME(EntityVectorTn, _SC("LgEntityVector"))
SQMOD_DECL_TYPENAME(EntityQuaternionTn, _SC("LgEntityQuaternion"))
SQMOD_DECL_TYPENAME(BoundsTn, _SC("Bounds"))
SQMOD_DECL_TYPENAME(WastedSettingsTn, _SC("WastedSettings"))

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(CCheckpointTn, _SC("CCheckpoint_INTERNAL"))
SQMOD_DECL_TYPENAME(CObjectTn, _SC("CObject_INTERNAL"))
SQMOD_DECL_TYPENAME(CPickupTn, _SC("CPickup_INTERNAL"))
SQMOD_DECL_TYPENAME(CPlayerTn, _SC("CPlayer_INTERNAL"))
SQMOD_DECL_TYPENAME(CVehicleTn, _SC("CVehicle_INTERNAL"))

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
        Var< LgQuaternion * > v(SqVM(), 1);
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
                    _Func->SetVehicleSpeed(mID, x, y, z, false, false);
                break;
                case LgVehicleVectorFlag::RelSpeed:
                    _Func->SetVehicleSpeed(mID, x, y, z, false, true);
                break;
                case LgVehicleVectorFlag::TurnSpeed:
                    _Func->SetVehicleTurnSpeed(mID, x, y, z, false, false);
                break;
                case LgVehicleVectorFlag::RelTurnSpeed:
                    _Func->SetVehicleTurnSpeed(mID, x, y, z, false, true);
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
    RootTable(vm).Bind(QuaternionTn::Str,
        DerivedClass< LgQuaternion, Quaternion >(vm, QuaternionTn::Str)
        // Constructors
        .Ctor< Quaternion::Value, Quaternion::Value, Quaternion::Value >()
        // Global Member Methods
        .SquirrelFunc(_SC("_tostring"), &QuaternionToString)
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(EntityQuaternionTn::Str,
        DerivedClass< LgEntityQuaternion, LgQuaternion >(vm, EntityQuaternionTn::Str)
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
    );
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(ARGBTn::Str,
        Class< LgARGB >(vm, ARGBTn::Str)
        // Constructors
        .Ctor< LgARGB::Value, LgARGB::Value, LgARGB::Value, LgARGB::Value >()
        // Member Variables
        .Var(_SC("a"), &LgARGB::a)
        .Var(_SC("r"), &LgARGB::r)
        .Var(_SC("g"), &LgARGB::g)
        .Var(_SC("b"), &LgARGB::b)
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
        .Ctor< uint32_t, uint32_t, float, float, LgRGB, uint32_t, uint32_t >()
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
    void SetColor(const LgARGB & c) const { Get().SetColorEx4(c.r, c.g, c.b, c.a); }
    void SetPos(const Vector3 & pos) const { Get().SetPosition(pos); }
    void SetRadius(float radius) const { Get().SetRadius(radius); }
    // --------------------------------------------------------------------------------------------
    SQMOD_NODISCARD int GetWorld() const { return Get().GetWorld(); }
    SQMOD_NODISCARD LgARGB GetColor() const { const Color4 c = Get().GetColor(); return LgARGB(c.a, c.r, c.g, c.b); }
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
    void MoveTo(const Vector3 & pos, int time) const { Get().MoveTo(pos, time); }
    void MoveBy(const Vector3 & offset, int time) const { Get().MoveBy(offset, time); }
    void RotateTo(const Quaternion & rotation, int time) const { Get().RotateTo(rotation, time); }
    void RotateBy(const Quaternion & rotOffset, int time) const { Get().RotateBy(rotOffset, time); }
    void RotateToEuler(const Vector3 & rotation, int time) const { Get().RotateToEuler(rotation, time); }
    void RotateByEuler(const Vector3 & rotOffset, int time) const { Get().RotateByEuler(rotOffset, time); }
    void SetAlpha(int alpha, int fadeTime) const { Get().SetAlphaEx(alpha, fadeTime); }
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
    void SetSingleUse(bool toggle) const { _Func->SetPickupOption(GetIdentifier(), vcmpPickupOptionSingleUse, toggle); }
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
    void SetColour(const LgRGB & colour) const { Get().SetColor(colour); }
    void SetMoney(int money) const { Get().SetMoney(money); }
    void SetScore(int score) const { Get().SetScore(score); }
    void SetImmunity(uint32_t immunity) const { Get().SetImmunity(immunity); }
    void SetHeading(float heading) const { Get().SetHeading(heading); }
    void SetVehicle(LgVehicle & vehicle) const;
    void SetFrozen(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionControllable, !toggle); }
    void SetDriveByEnabled(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionDriveBy, toggle); }
    void SetWhiteScanLines(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionWhiteScanlines, toggle); }
    void SetGreenScanLines(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionGreenScanlines, toggle); }
    void SetWidescreen(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionWidescreen, toggle); }
    void SetOnRadar(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionHasMarker, toggle); }
    void SetCanAttack(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionCanAttack, toggle); }
    void SetWeaponSlot(int slot) const { Get().SetWeaponSlot(slot); }
    void ShowMarkers(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionShowMarkers, toggle); }
    void SetSpectateTarget(LgPlayer & player) const { Get().SetSpectator(player.Get()); }
    void SetMarkerVisible(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionHasMarker, toggle); }
    void SetCanUseColors(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionChatTagsEnabled, toggle); }
    void SetDrunkStatus(bool toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionDrunkEffects, toggle); }
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
    SQMOD_NODISCARD int32_t GetImmunity() const { return Get().GetImmunity(); }
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
    SQMOD_NODISCARD int GetKey() const { return Get().GetKey(); }
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
    SQMOD_NODISCARD int GetPlayerGameKeys() const { return Get().GetGameKeys(); }
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
    void SetMarker(int toggle) const { _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionShowMarkers, toggle); }
    void SetDrunkLevel(int visuals, int handling) const {
        if (visuals <= 0 && handling <= 0) _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionDrunkEffects, 0);
        else _Func->SetPlayerOption(GetIdentifier(), vcmpPlayerOptionDrunkEffects, 1);
    }
    void RedirectPlayerToServer(StackStrF & ip, unsigned int port, StackStrF & nick, StackStrF & spass, StackStrF & upass) const
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
        for(int i = 0, n = _Func->GetMaxPlayers(); i < n; ++i) {
            if(_Func->IsPlayerConnected(i)) {
                if(_Func->GetPlayerVehicleId(i) == id && _Func->GetPlayerInVehicleSlot(i) == 0) return i;
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
    void SetLocked(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionDoorsLocked, toggle); }
    void SetDamage(uint32_t damage) const { Get().SetDamageData(damage); }
    void SetLightFlags(uint32_t flags) const { Get().SetLightsData(flags); }
    void SetAlarm(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionAlarm, toggle ); }
    void SetSiren(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionSiren, toggle); }
    void SetLights(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionLights, toggle ); }
    void SetRotation(const Quaternion & rotation) const { Get().SetRotation(rotation); }
    void SetEulerRotation(const Vector3 & angle) const { Get().SetRotationEuler(angle); }
    void SetSpeed(const Vector3 & speed) const { Get().SetSpeed(speed); }
    void SetRelativeSpeed(const Vector3 & speed) const { Get().SetRelativeSpeed(speed); }
    void SetTurnSpeed(const Vector3 & speed) const { Get().SetTurnSpeed(speed); }
    void SetRelativeTurnSpeed(const Vector3 & speed) const { Get().SetRelativeTurnSpeed(speed); }
    void SetRadio(int id) const { Get().SetRadio(id); }
    void SetRadioLocked(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionRadioLocked, toggle); }
    void SetGhost(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionGhost, toggle); }
    void SetSingleUse(bool toggle) const { _Func->SetVehicleOption(GetIdentifier(), vcmpVehicleOptionSingleUse, toggle); }
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
    SQMOD_NODISCARD int GetImmunity() const { return Get().GetImmunity(); }
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
    SQMOD_NODISCARD uint32_t GetLightFlags() const { return Get().GetLightsData(); }
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
    Class< LgCheckpoint, NoConstructor< LgCheckpoint > > checkpoint(vm, "CCheckpoint_INTERNAL");
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
    Class< LgObject, NoConstructor< LgObject > > object(vm, "CObject_INTERNAL");
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
    Class< LgPickup, NoConstructor< LgPickup > > pickup(vm, "CPickup_INTERNAL");
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
    Class< LgPlayer, NoConstructor< LgPlayer > > player(vm, "CPlayer_INTERNAL");
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
    Class< LgVehicle, NoConstructor< LgVehicle > > vehicle(vm, "CVehicle_INTERNAL");
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

} // Namespace:: SqMod
