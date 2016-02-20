// ------------------------------------------------------------------------------------------------
#include "Entity/Vehicle.hpp"
#include "Entity/Player.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Vector4.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Vector3      CVehicle::s_Vector3;
Vector4      CVehicle::s_Vector4;
Quaternion   CVehicle::s_Quaternion;

// ------------------------------------------------------------------------------------------------
SQChar CVehicle::s_StrID[SQMOD_VEHICLE_POOL][8];

// ------------------------------------------------------------------------------------------------
const Int32 CVehicle::Max = SQMOD_VEHICLE_POOL;

// ------------------------------------------------------------------------------------------------
CVehicle::CVehicle(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_VEHICLE_POOL))
    , m_Tag(VALID_ENTITY(m_ID) ? s_StrID[m_ID] : _SC("-1"))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CVehicle::~CVehicle()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::Cmp(const CVehicle & o) const
{
    if (m_ID == o.m_ID)
        return 0;
    else if (m_ID > o.m_ID)
        return 1;
    else
        return -1;
}

CSStr CVehicle::ToString() const
{
    return VALID_ENTITYEX(m_ID, SQMOD_VEHICLE_POOL) ? s_StrID[m_ID] : _SC("-1");
}

// ------------------------------------------------------------------------------------------------
CSStr CVehicle::GetTag() const
{
    return m_Tag.c_str();
}

void CVehicle::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

Object & CVehicle::GetData()
{
    if (Validate())
        return m_Data;
    return NullObject();
}

void CVehicle::SetData(Object & data)
{
    if (Validate())
        m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::Destroy(Int32 header, Object & payload)
{
    return _Core->DelVehicle(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::BindEvent(Int32 evid, Object & env, Function & func) const
{
    if (!Validate())
        return false;

    Function & event = _Core->GetVehicleEvent(m_ID, evid);

    if (func.IsNull())
        event.Release();
    else
        event = Function(env.GetVM(), env, func.GetFunc());

    return true;
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::IsStreamedFor(CPlayer & player) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        return _Func->IsVehicleStreamedForPlayer(m_ID, player.GetID());
    return false;
}

Int32 CVehicle::GetSyncSource() const
{
    if (Validate())
        return _Func->GetVehicleSyncSource(m_ID);
    return -1;
}

Int32 CVehicle::GetSyncType() const
{
    if (Validate())
        return _Func->GetVehicleSyncType(m_ID);
    return -1;
}

Int32 CVehicle::GetWorld() const
{
    if (Validate())
        return _Func->GetVehicleWorld(m_ID);
    return -1;
}

void CVehicle::SetWorld(Int32 world) const
{
    if (Validate())
        _Func->SetVehicleWorld(m_ID, world);
}

Int32 CVehicle::GetModel() const
{
    if (Validate())
        return _Func->GetVehicleModel(m_ID);
    return -1;
}

Object & CVehicle::GetOccupant(Int32 slot) const
{
    if (Validate())
        return _Core->GetPlayer(_Func->GetVehicleOccupant(m_ID, slot)).mObj;
    return NullObject();
}

Int32 CVehicle::GetOccupantID(Int32 slot) const
{
    if (Validate())
        return _Func->GetVehicleOccupant(m_ID, slot);
    return -1;
}

void CVehicle::Respawn() const
{
    if (Validate())
        _Func->RespawnVehicle(m_ID);
}

Int32 CVehicle::GetImmunity() const
{
    if (Validate())
        return _Func->GetVehicleImmunityFlags(m_ID);
    return 0;
}

void CVehicle::SetImmunity(Int32 flags) const
{
    if (Validate())
        _Func->SetVehicleImmunityFlags(m_ID, flags);
}

bool CVehicle::IsWrecked() const
{
    if (Validate())
        return _Func->IsVehicleWrecked(m_ID);
    return false;
}

const Vector3 & CVehicle::GetPosition() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetVehiclePos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CVehicle::SetPosition(const Vector3 & pos) const
{
    if (Validate())
        _Func->SetVehiclePos(m_ID, pos.x, pos.y, pos.z, false);
}

void CVehicle::SetPositionEx(const Vector3 & pos, bool empty) const
{
    if (Validate())
        _Func->SetVehiclePos(m_ID, pos.x, pos.y, pos.z, empty);
}

void CVehicle::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetVehiclePos(m_ID, x, y, z, false);
}

void CVehicle::SetPositionEx(Float32 x, Float32 y, Float32 z, bool empty) const
{
    if (Validate())
        _Func->SetVehiclePos(m_ID, x, y, z, empty);
}

const Quaternion & CVehicle::GetRotation() const
{
    s_Quaternion.Clear();
    if (Validate())
        _Func->GetVehicleRot(m_ID, &s_Quaternion.x, &s_Quaternion.y,
                                    &s_Quaternion.z, &s_Quaternion.w);
    return s_Quaternion;
}

void CVehicle::SetRotation(const Quaternion & rot) const
{
    if (Validate())
        _Func->SetVehicleRot(m_ID, rot.x, rot.y, rot.z, rot.w);
}

void CVehicle::SetRotationEx(Float32 x, Float32 y, Float32 z, Float32 w) const
{
    if (Validate())
        _Func->SetVehicleRot(m_ID, x, y, z, w);
}

const Vector3 & CVehicle::GetRotationEuler() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetVehicleRotEuler(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CVehicle::SetRotationEuler(const Vector3 & rot) const
{
    if (Validate())
        _Func->SetVehicleRotEuler(m_ID, rot.x, rot.y, rot.z);
}

void CVehicle::SetRotationEulerEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetVehicleRotEuler(m_ID, x, y, z);
}

const Vector3 & CVehicle::GetSpeed() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetVehicleSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CVehicle::SetSpeed(const Vector3 & vel) const
{
    if (Validate())
        _Func->SetVehicleSpeed(m_ID, vel.x, vel.y, vel.z);
}

void CVehicle::SetSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetVehicleSpeed(m_ID, x, y, z);
}

void CVehicle::AddSpeed(const Vector3 & vel) const
{
    if (Validate())
        _Func->AddVehicleSpeed(m_ID, vel.x, vel.y, vel.z);
}

void CVehicle::AddSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->AddVehicleSpeed(m_ID, x, y, z);
}

const Vector3 & CVehicle::GetRelSpeed() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetVehicleRelSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CVehicle::SetRelSpeed(const Vector3 & vel) const
{
    if (Validate())
        _Func->SetVehicleRelSpeed(m_ID, vel.x, vel.y, vel.z);
}

void CVehicle::SetRelSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetVehicleRelSpeed(m_ID, x, y, z);
}

void CVehicle::AddRelSpeed(const Vector3 & vel) const
{
    if (Validate())
        _Func->AddVehicleRelSpeed(m_ID, vel.x, vel.y, vel.z);
}

void CVehicle::AddRelSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->AddVehicleRelSpeed(m_ID, x, y, z);
}

const Vector3 & CVehicle::GetTurnSpeed() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetVehicleTurnSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CVehicle::SetTurnSpeed(const Vector3 & vel) const
{
    if (Validate())
        _Func->SetVehicleTurnSpeed(m_ID, vel.x, vel.y, vel.z);
}

void CVehicle::SetTurnSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetVehicleTurnSpeed(m_ID, x, y, z);
}

void CVehicle::AddTurnSpeed(const Vector3 & vel) const
{
    if (Validate())
        _Func->AddVehicleTurnSpeed(m_ID, vel.x, vel.y, vel.z);
}

void CVehicle::AddTurnSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->AddVehicleTurnSpeed(m_ID, x, y, z);
}

const Vector3 & CVehicle::GetRelTurnSpeed() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetVehicleRelTurnSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);

    return s_Vector3;
}

void CVehicle::SetRelTurnSpeed(const Vector3 & vel) const
{
    if (Validate())
        _Func->SetVehicleRelTurnSpeed(m_ID, vel.x, vel.y, vel.z);
}

void CVehicle::SetRelTurnSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetVehicleRelTurnSpeed(m_ID, x, y, z);
}

void CVehicle::AddRelTurnSpeed(const Vector3 & vel) const
{
    if (Validate())
        _Func->AddVehicleRelTurnSpeed(m_ID, vel.x, vel.y, vel.z);
}

void CVehicle::AddRelTurnSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->AddVehicleRelTurnSpeed(m_ID, x, y, z);
}

const Vector4 & CVehicle::GetSpawnPosition() const
{
    s_Vector4.Clear();
    if (Validate())
        _Func->GetVehicleSpawnPos(m_ID, &s_Vector4.x, &s_Vector4.y, &s_Vector4.z, &s_Vector4.w);
    return s_Vector4;
}

void CVehicle::SetSpawnPosition(const Vector4 & pos) const
{
    if (Validate())
        _Func->SetVehicleSpawnPos(m_ID, pos.x, pos.y, pos.z, pos.w);
}

void CVehicle::SetSpawnPositionEx(Float32 x, Float32 y, Float32 z, Float32 w) const
{
    if (Validate())
        _Func->SetVehicleSpawnPos(m_ID, x, y, z, w);
}

const Quaternion & CVehicle::GetSpawnRotation() const
{
    s_Quaternion.Clear();
    if (Validate())
        _Func->GetVehicleSpawnRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, &s_Quaternion.z, &s_Quaternion.w);
    return s_Quaternion;
}

void CVehicle::SetSpawnRotation(const Quaternion & rot) const
{
    if (Validate())
        _Func->SetVehicleSpawnRot(m_ID, rot.x, rot.y, rot.z, rot.w);
}

void CVehicle::SetSpawnRotationEx(Float32 x, Float32 y, Float32 z, Float32 w) const
{
    if (Validate())
        _Func->SetVehicleSpawnRot(m_ID, x, y, z, w);
}

const Vector3 & CVehicle::GetSpawnRotationEuler() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetVehicleSpawnRotEuler(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CVehicle::SetSpawnRotationEuler(const Vector3 & rot) const
{
    if (Validate())
        _Func->SetVehicleSpawnRotEuler(m_ID, rot.x, rot.y, rot.z);
}

void CVehicle::SetSpawnRotationEulerEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetVehicleSpawnRotEuler(m_ID, x, y, z);
}

Uint32 CVehicle::GetRespawnTimer() const
{
    if (Validate())
        return _Func->GetVehicleIdleRespawnTimer(m_ID);
    return 0;
}

void CVehicle::SetRespawnTimer(Uint32 timer) const
{
    if (Validate())
        _Func->SetVehicleIdleRespawnTimer(m_ID, timer);
}

Float32 CVehicle::GetHealth() const
{
    if (Validate())
        return _Func->GetVehicleHealth(m_ID);
    return 0;
}

void CVehicle::SetHealth(Float32 amount) const
{
    if (Validate())
        _Func->SetVehicleHealth(m_ID, amount);
}

Int32 CVehicle::GetPrimaryColor() const
{
    Int32 primary = -1;
    if (Validate())
        _Func->GetVehicleColour(m_ID, &primary, NULL);
    return primary;
}

void CVehicle::SetPrimaryColor(Int32 col) const
{
    if (!Validate())
        return;
    Int32 secondary;
    _Func->GetVehicleColour(m_ID, NULL, &secondary);
    _Func->SetVehicleColour(m_ID, col, secondary);
}

Int32 CVehicle::GetSecondaryColor() const
{
    Int32 secondary = -1;
    if (Validate())
        _Func->GetVehicleColour(m_ID, NULL, &secondary);
    return secondary;
}

void CVehicle::SetSecondaryColor(Int32 col) const
{
    if (!Validate())
        return;
    Int32 primary;
    _Func->GetVehicleColour(m_ID, &primary, NULL);
    _Func->SetVehicleColour(m_ID, primary, col);
}

void CVehicle::SetColors(Int32 primary, Int32 secondary) const
{
    if (Validate())
        _Func->SetVehicleColour(m_ID, primary, secondary);
}

bool CVehicle::GetLocked() const
{
    if (Validate())
        return _Func->GetVehicleDoorsLocked(m_ID);
    return false;
}

void CVehicle::SetLocked(bool toggle) const
{
    if (Validate())
        _Func->SetVehicleDoorsLocked(m_ID, toggle);
}

Int32 CVehicle::GetPartStatus(Int32 part) const
{
    if (Validate())
        return _Func->GetVehiclePartStatus(m_ID, part);
    return -1;
}

void CVehicle::SetPartStatus(Int32 part, Int32 status) const
{
    if (Validate())
        _Func->SetVehiclePartStatus(m_ID, part, status);
}

Int32 CVehicle::GetTyreStatus(Int32 tyre) const
{
    if (Validate())
        return _Func->GetVehicleTyreStatus(m_ID, tyre);
    return -1;
}

void CVehicle::SetTyreStatus(Int32 tyre, Int32 status) const
{
    if (Validate())
        _Func->SetVehicleTyreStatus(m_ID, tyre, status);
}

Uint32 CVehicle::GetDamageData() const
{
    if (Validate())
        return _Func->GetVehicleDamageData(m_ID);
    return 0;
}

void CVehicle::SetDamageData(Uint32 data) const
{
    if (Validate())
        _Func->SetVehicleDamageData(m_ID, data);
}

bool CVehicle::GetAlarm() const
{
    if (Validate())
        return _Func->GetVehicleAlarm(m_ID);
    return false;
}

void CVehicle::SetAlarm(bool toggle) const
{
    if (Validate())
        _Func->SetVehicleAlarm(m_ID, toggle);
}

bool CVehicle::GetLights() const
{
    if (Validate())
        return _Func->GetVehicleLights(m_ID);
    return false;
}

void CVehicle::SetLights(bool toggle) const
{
    if (Validate())
        _Func->SetVehicleLights(m_ID, toggle);
}

Int32 CVehicle::GetRadio() const
{
    if (Validate())
        return _Func->GetVehicleRadio(m_ID);
    return -1;
}

void CVehicle::SetRadio(Int32 radio) const
{
    if (Validate())
        _Func->SetVehicleRadio(m_ID, radio);
}

bool CVehicle::GetRadioLocked() const
{
    if (Validate())
        return _Func->IsVehicleRadioLocked(m_ID);
    return false;
}

void CVehicle::SetRadioLocked(bool toggle) const
{
    if (Validate())
        _Func->SetVehicleRadioLocked(m_ID, toggle);
}

bool CVehicle::GetGhostState() const
{
    if (Validate())
        return _Func->GetVehicleGhostState(m_ID);
    return false;
}

void CVehicle::SetGhostState(bool toggle) const
{
    if (Validate())
        _Func->SetVehicleGhostState(m_ID, toggle);
}

void CVehicle::ResetHandling() const
{
    if (Validate())
        _Func->ResetInstHandling(m_ID);
}

void CVehicle::ResetHandling(Int32 rule) const
{
    if (Validate())
        _Func->ResetInstHandlingRule(m_ID, rule);
}

bool CVehicle::ExistsHandling(Int32 rule) const
{
    if (Validate())
        return _Func->ExistsInstHandlingRule(m_ID, rule);
    return false;
}

Float32 CVehicle::GetHandlingData(Int32 rule) const
{
    if (Validate())
        return _Func->GetInstHandlingRule(m_ID, rule);
    return 0;
}

void CVehicle::SetHandlingData(Int32 rule, Float32 data) const
{
    if (Validate())
        _Func->SetInstHandlingRule(m_ID, rule, data);
}

void CVehicle::Embark(CPlayer & player) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->PutPlayerInVehicle(player.GetID(), m_ID, 0, true, true);
}

void CVehicle::Embark(CPlayer & player, Int32 slot, bool allocate, bool warp) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->PutPlayerInVehicle(player.GetID(), m_ID, slot, allocate, warp);
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetPosX() const
{
    s_Vector3.x = 0;
    if (Validate())
        _Func->GetVehiclePos(m_ID, &s_Vector3.x, NULL, NULL);
    return s_Vector3.x;
}

Float32 CVehicle::GetPosY() const
{
    s_Vector3.y = 0;
    if (Validate())
        _Func->GetVehiclePos(m_ID, NULL, &s_Vector3.y, NULL);
    return s_Vector3.y;
}

Float32 CVehicle::GetPosZ() const
{
    s_Vector3.z = 0;
    if (Validate())
        _Func->GetVehiclePos(m_ID, NULL, NULL, &s_Vector3.z);
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPosX(Float32 x) const
{
    if (Validate())
    {
        _Func->GetVehiclePos(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
        _Func->SetVehiclePos(m_ID, x, s_Vector3.y, s_Vector3.z, false);
    }
}

void CVehicle::SetPosY(Float32 y) const
{
    if (Validate())
    {
        _Func->GetVehiclePos(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
        _Func->SetVehiclePos(m_ID, s_Vector3.x, y, s_Vector3.z, false);
    }
}

void CVehicle::SetPosZ(Float32 z) const
{
    if (Validate())
    {
        _Func->GetVehiclePos(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
        _Func->SetVehiclePos(m_ID, s_Vector3.z, s_Vector3.y, z, false);
    }
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetRotX() const
{
    s_Quaternion.x = 0;
    if (Validate())
        _Func->GetVehicleRot(m_ID, &s_Quaternion.x, NULL, NULL, NULL);
    return s_Quaternion.x;
}

Float32 CVehicle::GetRotY() const
{
    s_Quaternion.y = 0;
    if (Validate())
        _Func->GetVehicleRot(m_ID, NULL, &s_Quaternion.y, NULL, NULL);
    return s_Quaternion.y;
}

Float32 CVehicle::GetRotZ() const
{
    s_Quaternion.z = 0;
    if (Validate())
        _Func->GetVehicleRot(m_ID, NULL, NULL, &s_Quaternion.z, NULL);
    return s_Quaternion.z;
}

Float32 CVehicle::GetRotW() const
{
    s_Quaternion.w = 0;
    if (Validate())
        _Func->GetVehicleRot(m_ID, NULL, NULL, NULL, &s_Quaternion.w);
    return s_Quaternion.w;
}

void CVehicle::SetRotX(Float32 x) const
{
    if (Validate())
    {
        _Func->GetVehicleRot(m_ID, NULL, &s_Quaternion.y, &s_Quaternion.z, &s_Quaternion.w);
        _Func->SetVehicleRot(m_ID, x, s_Quaternion.y, s_Quaternion.z, s_Quaternion.w);
    }
}

void CVehicle::SetRotY(Float32 y) const
{
    if (Validate())
    {
        _Func->GetVehicleRot(m_ID, &s_Quaternion.x, NULL, &s_Quaternion.z, &s_Quaternion.w);
        _Func->SetVehicleRot(m_ID, s_Quaternion.x, y, s_Quaternion.z, s_Quaternion.w);
    }
}

void CVehicle::SetRotZ(Float32 z) const
{
    if (Validate())
    {
        _Func->GetVehicleRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, NULL, &s_Quaternion.w);
        _Func->SetVehicleRot(m_ID, s_Quaternion.x, s_Quaternion.y, z, s_Quaternion.w);
    }
}

void CVehicle::SetRotW(Float32 w) const
{
    if (Validate())
    {
        _Func->GetVehicleRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, &s_Quaternion.z, NULL);
        _Func->SetVehicleRot(m_ID, s_Quaternion.x, s_Quaternion.y, s_Quaternion.z, w);
    }
}


// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetERotX() const
{
    s_Vector3.x = 0;
    if (Validate())
        _Func->GetVehicleRotEuler(m_ID, &s_Vector3.x, NULL, NULL);
    return s_Vector3.x;
}

Float32 CVehicle::GetERotY() const
{
    s_Vector3.y = 0;
    if (Validate())
        _Func->GetVehicleRotEuler(m_ID, NULL, &s_Vector3.y, NULL);
    return s_Vector3.y;
}

Float32 CVehicle::GetERotZ() const
{
    s_Vector3.z = 0;
    if (Validate())
        _Func->GetVehicleRotEuler(m_ID, NULL, NULL, &s_Vector3.z);
    return s_Vector3.z;
}

void CVehicle::SetERotX(Float32 x) const
{
    if (Validate())
    {
        _Func->GetVehicleRotEuler(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
        _Func->SetVehicleRotEuler(m_ID, x, s_Vector3.y, s_Vector3.z);
    }
}

void CVehicle::SetERotY(Float32 y) const
{
    if (Validate())
    {
        _Func->GetVehicleRotEuler(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
        _Func->SetVehicleRotEuler(m_ID, s_Vector3.x, y, s_Vector3.z);
    }
}

void CVehicle::SetERotZ(Float32 z) const
{
    if (Validate())
    {
        _Func->GetVehicleRotEuler(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
        _Func->SetVehicleRotEuler(m_ID, s_Vector3.z, s_Vector3.y, z);
    }
}

// ------------------------------------------------------------------------------------------------
static Object & CreateVehicleEx(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z, Float32 angle,
                            Int32 primary, Int32 secondary)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateVehicleEx(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z, Float32 angle,
                            Int32 primary, Int32 secondary, Int32 header, Object & payload)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateVehicle(Int32 model, Int32 world, const Vector3 & pos, Float32 angle,
                        Int32 primary, Int32 secondary)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateVehicle(Int32 model, Int32 world, const Vector3 & pos, Float32 angle,
                        Int32 primary, Int32 secondary, Int32 header, Object & payload)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            header, payload);
}

// ================================================================================================
void Register_CVehicle(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqVehicle"),
        Class< CVehicle, NoConstructor< CVehicle > >(vm, _SC("SqVehicle"))
        /* Metamethods */
        .Func(_SC("_cmp"), &CVehicle::Cmp)
        .Func(_SC("_tostring"), &CVehicle::ToString)
        /* Core Properties */
        .Prop(_SC("ID"), &CVehicle::GetID)
        .Prop(_SC("Tag"), &CVehicle::GetTag, &CVehicle::SetTag)
        .Prop(_SC("Data"), &CVehicle::GetData, &CVehicle::SetData)
        .Prop(_SC("MaxID"), &CVehicle::GetMaxID)
        .Prop(_SC("Active"), &CVehicle::IsActive)
        /* Core Functions */
        .Func(_SC("Bind"), &CVehicle::BindEvent)
        /* Core Overloads */
        .Overload< bool (CVehicle::*)(void) >(_SC("Destroy"), &CVehicle::Destroy)
        .Overload< bool (CVehicle::*)(Int32) >(_SC("Destroy"), &CVehicle::Destroy)
        .Overload< bool (CVehicle::*)(Int32, Object &) >(_SC("Destroy"), &CVehicle::Destroy)
        /* Properties */
        .Prop(_SC("SyncSource"), &CVehicle::GetSyncSource)
        .Prop(_SC("SyncType"), &CVehicle::GetSyncType)
        .Prop(_SC("World"), &CVehicle::GetWorld, &CVehicle::SetWorld)
        .Prop(_SC("Model"), &CVehicle::GetModel)
        .Prop(_SC("Immunity"), &CVehicle::GetImmunity, &CVehicle::SetImmunity)
        .Prop(_SC("Wrecked"), &CVehicle::IsWrecked)
        .Prop(_SC("Pos"), &CVehicle::GetPosition, &CVehicle::SetPosition)
        .Prop(_SC("Position"), &CVehicle::GetPosition, &CVehicle::SetPosition)
        .Prop(_SC("Rot"), &CVehicle::GetRotation, &CVehicle::SetRotation)
        .Prop(_SC("Rotation"), &CVehicle::GetRotation, &CVehicle::SetRotation)
        .Prop(_SC("ERot"), &CVehicle::GetRotationEuler, &CVehicle::SetRotationEuler)
        .Prop(_SC("RotEuler"), &CVehicle::GetRotationEuler, &CVehicle::SetRotationEuler)
        .Prop(_SC("RotationEuler"), &CVehicle::GetRotationEuler, &CVehicle::SetRotationEuler)
        .Prop(_SC("Speed"), &CVehicle::GetSpeed, &CVehicle::SetSpeed)
        .Prop(_SC("RelSpeed"), &CVehicle::GetRelSpeed, &CVehicle::SetRelSpeed)
        .Prop(_SC("RelativeSpeed"), &CVehicle::GetRelSpeed, &CVehicle::SetRelSpeed)
        .Prop(_SC("TurnSpeed"), &CVehicle::GetTurnSpeed, &CVehicle::SetTurnSpeed)
        .Prop(_SC("RelTurnSpeed"), &CVehicle::GetRelTurnSpeed, &CVehicle::SetRelTurnSpeed)
        .Prop(_SC("RelativeTurnSpeed"), &CVehicle::GetRelTurnSpeed, &CVehicle::SetRelTurnSpeed)
        .Prop(_SC("SpawnPos"), &CVehicle::GetSpawnPosition, &CVehicle::SetSpawnPosition)
        .Prop(_SC("SpawnPosition"), &CVehicle::GetSpawnPosition, &CVehicle::SetSpawnPosition)
        .Prop(_SC("SpawnRot"), &CVehicle::GetSpawnRotation, &CVehicle::SetSpawnRotation)
        .Prop(_SC("SpawnRotation"), &CVehicle::GetSpawnRotation, &CVehicle::SetSpawnRotation)
        .Prop(_SC("SpawnERot"), &CVehicle::GetSpawnRotationEuler, &CVehicle::SetSpawnRotationEuler)
        .Prop(_SC("SpawnRotEuler"), &CVehicle::GetSpawnRotationEuler, &CVehicle::SetSpawnRotationEuler)
        .Prop(_SC("SpawnRotationEuler"), &CVehicle::GetSpawnRotationEuler, &CVehicle::SetSpawnRotationEuler)
        .Prop(_SC("RespawnTimer"), &CVehicle::GetRespawnTimer, &CVehicle::SetRespawnTimer)
        .Prop(_SC("Health"), &CVehicle::GetHealth, &CVehicle::SetHealth)
        .Prop(_SC("PrimaryColor"), &CVehicle::GetPrimaryColor, &CVehicle::SetPrimaryColor)
        .Prop(_SC("SecondaryColor"), &CVehicle::GetSecondaryColor, &CVehicle::SetSecondaryColor)
        .Prop(_SC("Locked"), &CVehicle::GetLocked, &CVehicle::SetLocked)
        .Prop(_SC("DamageData"), &CVehicle::GetDamageData, &CVehicle::SetDamageData)
        .Prop(_SC("Alarm"), &CVehicle::GetAlarm, &CVehicle::SetAlarm)
        .Prop(_SC("Lights"), &CVehicle::GetLights, &CVehicle::SetLights)
        .Prop(_SC("Radio"), &CVehicle::GetRadio, &CVehicle::SetRadio)
        .Prop(_SC("RadioLocked"), &CVehicle::GetRadioLocked, &CVehicle::SetRadioLocked)
        .Prop(_SC("Ghost"), &CVehicle::GetGhostState, &CVehicle::SetGhostState)
        .Prop(_SC("GhostState"), &CVehicle::GetGhostState, &CVehicle::SetGhostState)
        .Prop(_SC("X"), &CVehicle::GetPosX, &CVehicle::SetPosX)
        .Prop(_SC("Y"), &CVehicle::GetPosY, &CVehicle::SetPosY)
        .Prop(_SC("Z"), &CVehicle::GetPosZ, &CVehicle::SetPosZ)
        .Prop(_SC("RX"), &CVehicle::GetRotX, &CVehicle::SetRotX)
        .Prop(_SC("RY"), &CVehicle::GetRotY, &CVehicle::SetRotY)
        .Prop(_SC("RZ"), &CVehicle::GetRotZ, &CVehicle::SetRotZ)
        .Prop(_SC("RW"), &CVehicle::GetRotW, &CVehicle::SetRotW)
        .Prop(_SC("EX"), &CVehicle::GetERotX, &CVehicle::SetERotX)
        .Prop(_SC("EY"), &CVehicle::GetERotY, &CVehicle::SetERotY)
        .Prop(_SC("EZ"), &CVehicle::GetERotZ, &CVehicle::SetERotZ)
        /* Functions */
        .Func(_SC("StreamedFor"), &CVehicle::IsStreamedFor)
        .Func(_SC("Occupant"), &CVehicle::GetOccupant)
        .Func(_SC("OccupantID"), &CVehicle::GetOccupantID)
        .Func(_SC("Respawn"), &CVehicle::Respawn)
        .Func(_SC("SetRot"), &CVehicle::SetRotationEx)
        .Func(_SC("SetRotation"), &CVehicle::SetRotationEx)
        .Func(_SC("SetERot"), &CVehicle::SetRotationEulerEx)
        .Func(_SC("SetRotEuler"), &CVehicle::SetRotationEulerEx)
        .Func(_SC("SetRotationEuler"), &CVehicle::SetRotationEulerEx)
        .Func(_SC("SetSpeed"), &CVehicle::SetSpeedEx)
        .Func(_SC("SetRelativeSpeed"), &CVehicle::SetRelSpeedEx)
        .Func(_SC("SetTurnSpeed"), &CVehicle::SetTurnSpeedEx)
        .Func(_SC("SetRelativeTurnSpeed"), &CVehicle::SetRelTurnSpeedEx)
        .Func(_SC("SetSpawnPos"), &CVehicle::SetSpawnPositionEx)
        .Func(_SC("SetSpawnPosition"), &CVehicle::SetSpawnPositionEx)
        .Func(_SC("SetSpawnRot"), &CVehicle::SetSpawnRotationEx)
        .Func(_SC("SetSpawnRotation"), &CVehicle::SetSpawnRotationEx)
        .Func(_SC("SetSpawnERot"), &CVehicle::SetSpawnRotationEulerEx)
        .Func(_SC("SetSpawnRotEuler"), &CVehicle::SetSpawnRotationEulerEx)
        .Func(_SC("SetSpawnRotationEuler"), &CVehicle::SetSpawnRotationEulerEx)
        .Func(_SC("SetColors"), &CVehicle::SetColors)
        .Func(_SC("GetPartStatus"), &CVehicle::GetPartStatus)
        .Func(_SC("SetPartStatus"), &CVehicle::SetPartStatus)
        .Func(_SC("GetTyreStatus"), &CVehicle::GetTyreStatus)
        .Func(_SC("SetTyreStatus"), &CVehicle::SetTyreStatus)
        .Func(_SC("ExistsHandling"), &CVehicle::ExistsHandling)
        .Func(_SC("GetHandlingData"), &CVehicle::GetHandlingData)
        .Func(_SC("SetHandlingData"), &CVehicle::SetHandlingData)
        /* Overloads */
        .Overload< void (CVehicle::*)(const Vector3 &, bool) const >
            (_SC("SetPos"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(Float32, Float32, Float32) const >
            (_SC("SetPos"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(Float32, Float32, Float32, bool) const >
            (_SC("SetPos"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(const Vector3 &, bool) const >
            (_SC("SetPosition"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(Float32, Float32, Float32) const >
            (_SC("SetPosition"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(Float32, Float32, Float32, bool) const >
            (_SC("SetPosition"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(const Vector3 &) const >
            (_SC("AddSpeed"), &CVehicle::AddSpeed)
        .Overload< void (CVehicle::*)(Float32, Float32, Float32) const >
            (_SC("AddSpeed"), &CVehicle::AddSpeedEx)
        .Overload< void (CVehicle::*)(const Vector3 &) const >
            (_SC("AddRelSpeed"), &CVehicle::AddRelSpeed)
        .Overload< void (CVehicle::*)(Float32, Float32, Float32) const >
            (_SC("AddRelSpeed"), &CVehicle::AddRelSpeedEx)
        .Overload< void (CVehicle::*)(const Vector3 &) const >
            (_SC("AddTurnSpeed"), &CVehicle::AddTurnSpeed)
        .Overload< void (CVehicle::*)(Float32, Float32, Float32) const >
            (_SC("AddTurnSpeed"), &CVehicle::AddTurnSpeedEx)
        .Overload< void (CVehicle::*)(const Vector3 &) const >
            (_SC("AddRelTurnSpeed"), &CVehicle::AddRelTurnSpeed)
        .Overload< void (CVehicle::*)(Float32, Float32, Float32) const >
            (_SC("AddRelTurnSpeed"), &CVehicle::AddRelTurnSpeedEx)
        .Overload< void (CVehicle::*)(void) const >
            (_SC("ResetHandling"), &CVehicle::ResetHandling)
        .Overload< void (CVehicle::*)(Int32) const >
            (_SC("ResetHandling"), &CVehicle::ResetHandling)
        .Overload< void (CVehicle::*)(CPlayer &) const >
            (_SC("Embark"), &CVehicle::Embark)
        .Overload< void (CVehicle::*)(CPlayer &, Int32, bool, bool) const >
            (_SC("Embark"), &CVehicle::Embark)
    );

    RootTable(vm)
    .Overload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Float32, Int32, Int32) >
        (_SC("CreateVehicleEx"), &CreateVehicleEx)
    .Overload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Float32, Int32, Int32, Int32, Object &) >
        (_SC("CreateVehicleEx"), &CreateVehicleEx)
    .Overload< Object & (*)(Int32, Int32, const Vector3 &, Float32, Int32, Int32) >
        (_SC("CreateVehicle"), &CreateVehicle)
    .Overload< Object & (*)(Int32, Int32, const Vector3 &, Float32, Int32, Int32, Int32, Object &) >
        (_SC("CreateVehicle"), &CreateVehicle);
}

} // Namespace:: SqMod