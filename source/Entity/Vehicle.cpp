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
const Int32 CVehicle::Max = SQMOD_VEHICLE_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CVehicle::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqVehicle");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
CVehicle::CVehicle(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_VEHICLE_POOL))
    , m_Tag(ToStrF("%d", id))
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

// ------------------------------------------------------------------------------------------------
const String & CVehicle::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CVehicle::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
Object & CVehicle::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetData(Object & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::Destroy(Int32 header, Object & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return _Core->DelVehicle(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::BindEvent(Int32 evid, Object & env, Function & func) const
{
    // Validate the managed identifier
    Validate();
    // Obtain the function instance called for this event
    Function & event = _Core->GetVehicleEvent(m_ID, evid);
    // Is the specified callback function null?
    if (func.IsNull())
    {
        event.Release(); // Then release the current callback
    }
    // Assign the specified environment and function
    else
    {
        event = Function(env.GetVM(), env, func.GetFunc());
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::IsStreamedFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        SqThrowF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsVehicleStreamedForPlayer(m_ID, player.GetID());
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::GetSyncSource() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleSyncSource(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::GetSyncType() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleSyncType(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetWorld(Int32 world) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleWorld(m_ID, world);
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::GetModel() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleModel(m_ID);
}

// ------------------------------------------------------------------------------------------------
Object & CVehicle::GetOccupant(Int32 slot) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetPlayer(_Func->GetVehicleOccupant(m_ID, slot)).mObj;
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::GetOccupantID(Int32 slot) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleOccupant(m_ID, slot);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::Respawn() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RespawnVehicle(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::GetImmunity() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleImmunityFlags(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetImmunity(Int32 flags) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleImmunityFlags(m_ID, flags);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::IsWrecked() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsVehicleWrecked(m_ID);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetPosition() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.Clear();
    // Query the server for the position values
    _Func->GetVehiclePos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPosition(const Vector3 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehiclePos(m_ID, pos.x, pos.y, pos.z, false);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionEx(const Vector3 & pos, bool empty) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehiclePos(m_ID, pos.x, pos.y, pos.z, empty);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehiclePos(m_ID, x, y, z, false);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionEx(Float32 x, Float32 y, Float32 z, bool empty) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehiclePos(m_ID, x, y, z, empty);
}

// ------------------------------------------------------------------------------------------------
const Quaternion & CVehicle::GetRotation() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Quaternion.Clear();
    // Query the server for the rotation values
    _Func->GetVehicleRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, &s_Quaternion.z, &s_Quaternion.w);
    // Return the requested information
    return s_Quaternion;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotation(const Quaternion & rot) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRot(m_ID, rot.x, rot.y, rot.z, rot.w);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationEx(Float32 x, Float32 y, Float32 z, Float32 w) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRot(m_ID, x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetRotationEuler() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Vector3.Clear();
    // Query the server for the rotation values
    _Func->GetVehicleRotEuler(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationEuler(const Vector3 & rot) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRotEuler(m_ID, rot.x, rot.y, rot.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationEulerEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRotEuler(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetSpeed() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous speed information, if any
    s_Vector3.Clear();
    // Query the server for the speed values
    _Func->GetVehicleSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, vel.x, vel.y, vel.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->AddVehicleSpeed(m_ID, vel.x, vel.y, vel.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->AddVehicleSpeed(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetRelSpeed() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous relative speed information, if any
    s_Vector3.Clear();
    // Query the server for the relative speed values
    _Func->GetVehicleRelSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRelSpeed(m_ID, vel.x, vel.y, vel.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRelSpeed(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->AddVehicleRelSpeed(m_ID, vel.x, vel.y, vel.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->AddVehicleRelSpeed(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetTurnSpeed() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous turn speed information, if any
    s_Vector3.Clear();
    // Query the server for the turn speed values
    _Func->GetVehicleTurnSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTurnSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, vel.x, vel.y, vel.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTurnSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddTurnSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->AddVehicleTurnSpeed(m_ID, vel.x, vel.y, vel.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddTurnSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->AddVehicleTurnSpeed(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetRelTurnSpeed() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous relative turn speed information, if any
    s_Vector3.Clear();
    // Query the server for the relative turn speed values
    _Func->GetVehicleRelTurnSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelTurnSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRelTurnSpeed(m_ID, vel.x, vel.y, vel.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelTurnSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRelTurnSpeed(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelTurnSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->AddVehicleRelTurnSpeed(m_ID, vel.x, vel.y, vel.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelTurnSpeedEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->AddVehicleRelTurnSpeed(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
const Vector4 & CVehicle::GetSpawnPosition() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous spawn position information, if any
    s_Vector4.Clear();
    // Query the server for the spawn position values
    _Func->GetVehicleSpawnPos(m_ID, &s_Vector4.x, &s_Vector4.y, &s_Vector4.z, &s_Vector4.w);
    // Return the requested information
    return s_Vector4;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnPosition(const Vector4 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnPos(m_ID, pos.x, pos.y, pos.z, pos.w);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnPositionEx(Float32 x, Float32 y, Float32 z, Float32 w) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnPos(m_ID, x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
const Quaternion & CVehicle::GetSpawnRotation() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous spawn rotation information, if any
    s_Quaternion.Clear();
    // Query the server for the spawn rotation values
    _Func->GetVehicleSpawnRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, &s_Quaternion.z, &s_Quaternion.w);
    // Return the requested information
    return s_Quaternion;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotation(const Quaternion & rot) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnRot(m_ID, rot.x, rot.y, rot.z, rot.w);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotationEx(Float32 x, Float32 y, Float32 z, Float32 w) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnRot(m_ID, x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetSpawnRotationEuler() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Vector3.Clear();
    // Query the server for the rotation values
    _Func->GetVehicleSpawnRotEuler(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotationEuler(const Vector3 & rot) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnRotEuler(m_ID, rot.x, rot.y, rot.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotationEulerEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnRotEuler(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
Uint32 CVehicle::GetRespawnTimer() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleIdleRespawnTimer(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRespawnTimer(Uint32 timer) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleIdleRespawnTimer(m_ID, timer);
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetHealth() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleHealth(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetHealth(Float32 amount) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleHealth(m_ID, amount);
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::GetPrimaryColor() const
{
    // Validate the managed identifier
    Validate();
    // The color value
    Int32 primary = -1;
    // Query the server for the requested color
    _Func->GetVehicleColour(m_ID, &primary, NULL);
    // Return the requested information
    return primary;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPrimaryColor(Int32 col) const
{
    // Validate the managed identifier
    Validate();
    // The unchanged color value
    Int32 secondary;
    // Query the server for the unchanged color
    _Func->GetVehicleColour(m_ID, NULL, &secondary);
    // Perform the requested operation
    _Func->SetVehicleColour(m_ID, col, secondary);
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::GetSecondaryColor() const
{
    // Validate the managed identifier
    Validate();
    // The color value
    Int32 secondary = -1;
    // Query the server for the requested color
    _Func->GetVehicleColour(m_ID, NULL, &secondary);
    // Return the requested information
    return secondary;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSecondaryColor(Int32 col) const
{
    // Validate the managed identifier
    Validate();
    // The unchanged color value
    Int32 primary;
    // Query the server for the unchanged color
    _Func->GetVehicleColour(m_ID, &primary, NULL);
    // Perform the requested operation
    _Func->SetVehicleColour(m_ID, primary, col);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetColors(Int32 primary, Int32 secondary) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleColour(m_ID, primary, secondary);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetLocked() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleDoorsLocked(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetLocked(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleDoorsLocked(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::GetPartStatus(Int32 part) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehiclePartStatus(m_ID, part);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPartStatus(Int32 part, Int32 status) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehiclePartStatus(m_ID, part, status);
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::GetTyreStatus(Int32 tyre) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleTyreStatus(m_ID, tyre);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTyreStatus(Int32 tyre, Int32 status) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleTyreStatus(m_ID, tyre, status);
}

// ------------------------------------------------------------------------------------------------
Uint32 CVehicle::GetDamageData() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleDamageData(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetDamageData(Uint32 data) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleDamageData(m_ID, data);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetAlarm() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleAlarm(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetAlarm(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleAlarm(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetLights() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleLights(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetLights(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleLights(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
Int32 CVehicle::GetRadio() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleRadio(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRadio(Int32 radio) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRadio(m_ID, radio);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetRadioLocked() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsVehicleRadioLocked(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRadioLocked(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRadioLocked(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetGhostState() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleGhostState(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetGhostState(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleGhostState(m_ID, toggle);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::ResetHandling() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->ResetInstHandling(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::ResetHandling(Int32 rule) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->ResetInstHandlingRule(m_ID, rule);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::ExistsHandling(Int32 rule) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->ExistsInstHandlingRule(m_ID, rule);
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetHandlingData(Int32 rule) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetInstHandlingRule(m_ID, rule);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetHandlingData(Int32 rule, Float32 data) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetInstHandlingRule(m_ID, rule, data);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::Embark(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        SqThrowF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->PutPlayerInVehicle(player.GetID(), m_ID, 0, true, true);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::Embark(CPlayer & player, Int32 slot, bool allocate, bool warp) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        SqThrowF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->PutPlayerInVehicle(player.GetID(), m_ID, slot, allocate, warp);
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetPosX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.x = 0;
    // Query the server for the requested component value
    _Func->GetVehiclePos(m_ID, &s_Vector3.x, NULL, NULL);
    // Return the requested information
    return s_Vector3.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetPosY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.y = 0;
    // Query the server for the requested component value
    _Func->GetVehiclePos(m_ID, NULL, &s_Vector3.y, NULL);
    // Return the requested information
    return s_Vector3.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetPosZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.z = 0;
    // Query the server for the requested component value
    _Func->GetVehiclePos(m_ID, NULL, NULL, &s_Vector3.z);
    // Return the requested information
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPosX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetVehiclePos(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetVehiclePos(m_ID, x, s_Vector3.y, s_Vector3.z, false);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPosY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetVehiclePos(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetVehiclePos(m_ID, s_Vector3.x, y, s_Vector3.z, false);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPosZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetVehiclePos(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
    // Perform the requested operation
    _Func->SetVehiclePos(m_ID, s_Vector3.z, s_Vector3.y, z, false);
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetRotX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Quaternion.x = 0;
    // Query the server for the requested component value
    _Func->GetVehicleRot(m_ID, &s_Quaternion.x, NULL, NULL, NULL);
    // Return the requested information
    return s_Quaternion.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetRotY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Quaternion.y = 0;
    // Query the server for the requested component value
    _Func->GetVehicleRot(m_ID, NULL, &s_Quaternion.y, NULL, NULL);
    // Return the requested information
    return s_Quaternion.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetRotZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Quaternion.z = 0;
    // Query the server for the requested component value
    _Func->GetVehicleRot(m_ID, NULL, NULL, &s_Quaternion.z, NULL);
    // Return the requested information
    return s_Quaternion.z;
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetRotW() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Quaternion.w = 0;
    // Query the server for the requested component value
    _Func->GetVehicleRot(m_ID, NULL, NULL, NULL, &s_Quaternion.w);
    // Return the requested information
    return s_Quaternion.w;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRot(m_ID, NULL, &s_Quaternion.y, &s_Quaternion.z, &s_Quaternion.w);
    // Perform the requested operation
    _Func->SetVehicleRot(m_ID, x, s_Quaternion.y, s_Quaternion.z, s_Quaternion.w);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRot(m_ID, &s_Quaternion.x, NULL, &s_Quaternion.z, &s_Quaternion.w);
    // Perform the requested operation
    _Func->SetVehicleRot(m_ID, s_Quaternion.x, y, s_Quaternion.z, s_Quaternion.w);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, NULL, &s_Quaternion.w);
    // Perform the requested operation
    _Func->SetVehicleRot(m_ID, s_Quaternion.x, s_Quaternion.y, z, s_Quaternion.w);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotW(Float32 w) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, &s_Quaternion.z, NULL);
    // Perform the requested operation
    _Func->SetVehicleRot(m_ID, s_Quaternion.x, s_Quaternion.y, s_Quaternion.z, w);
}


// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetERotX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Vector3.x = 0;
    // Query the server for the requested component value
    _Func->GetVehicleRotEuler(m_ID, &s_Vector3.x, NULL, NULL);
    // Return the requested information
    return s_Vector3.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetERotY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Vector3.y = 0;
    // Query the server for the requested component value
    _Func->GetVehicleRotEuler(m_ID, NULL, &s_Vector3.y, NULL);
    // Return the requested information
    return s_Vector3.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CVehicle::GetERotZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous rotation information, if any
    s_Vector3.z = 0;
    // Query the server for the requested component value
    _Func->GetVehicleRotEuler(m_ID, NULL, NULL, &s_Vector3.z);
    // Return the requested information
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetERotX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRotEuler(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetVehicleRotEuler(m_ID, x, s_Vector3.y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetERotY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRotEuler(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetVehicleRotEuler(m_ID, s_Vector3.x, y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetERotZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRotEuler(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
    // Perform the requested operation
    _Func->SetVehicleRotEuler(m_ID, s_Vector3.z, s_Vector3.y, z);
}

// ------------------------------------------------------------------------------------------------
static Object & Vehicle_CreateEx(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z, Float32 angle,
                            Int32 primary, Int32 secondary)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Vehicle_CreateEx(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z, Float32 angle,
                            Int32 primary, Int32 secondary, Int32 header, Object & payload)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Vehicle_Create(Int32 model, Int32 world, const Vector3 & pos, Float32 angle,
                        Int32 primary, Int32 secondary)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Vehicle_Create(Int32 model, Int32 world, const Vector3 & pos, Float32 angle,
                        Int32 primary, Int32 secondary, Int32 header, Object & payload)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static const Object & Vehicle_FindByID(Int32 id)
{
    // Perform a range check on the specified identifier
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        SqThrowF("The specified vehicle identifier is invalid: %d", id);
    }
    // Obtain the ends of the entity pool
    Core::Vehicles::const_iterator itr = _Core->GetVehicles().cbegin();
    Core::Vehicles::const_iterator end = _Core->GetVehicles().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mID == id)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a vehicle matching the specified identifier
    return NullObject();
}

static const Object & Vehicle_FindByTag(CSStr tag)
{
    // Perform a validity check on the specified tag
    if (!tag || *tag == '\0')
    {
        SqThrowF("The specified vehicle tag is invalid: null/empty");
    }
    // Obtain the ends of the entity pool
    Core::Vehicles::const_iterator itr = _Core->GetVehicles().cbegin();
    Core::Vehicles::const_iterator end = _Core->GetVehicles().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does this entity even exist and does the tag match the specified one?
        if (itr->mInst != nullptr && itr->mInst->GetTag().compare(tag) == 0)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a vehicle matching the specified tag
    return NullObject();
}

// ================================================================================================
void Register_CVehicle(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqVehicle"),
        Class< CVehicle, NoConstructor< CVehicle > >(vm, _SC("SqVehicle"))
        // Metamethods
        .Func(_SC("_cmp"), &CVehicle::Cmp)
        .SquirrelFunc(_SC("_typename"), &CVehicle::Typename)
        .Func(_SC("_tostring"), &CVehicle::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CVehicle::Max)
        // Core Properties
        .Prop(_SC("ID"), &CVehicle::GetID)
        .Prop(_SC("Tag"), &CVehicle::GetTag, &CVehicle::SetTag)
        .Prop(_SC("Data"), &CVehicle::GetData, &CVehicle::SetData)
        .Prop(_SC("Active"), &CVehicle::IsActive)
        // Core Methods
        .Func(_SC("Bind"), &CVehicle::BindEvent)
        // Core Overloads
        .Overload< bool (CVehicle::*)(void) >(_SC("Destroy"), &CVehicle::Destroy)
        .Overload< bool (CVehicle::*)(Int32) >(_SC("Destroy"), &CVehicle::Destroy)
        .Overload< bool (CVehicle::*)(Int32, Object &) >(_SC("Destroy"), &CVehicle::Destroy)
        // Properties
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
        // Member Methods
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
        // Member Overloads
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
        // Static Functions
        .StaticFunc(_SC("FindByID"), &Vehicle_FindByID)
        .StaticFunc(_SC("FindByTag"), &Vehicle_FindByTag)
        // Static Overloads
        .StaticOverload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Float32, Int32, Int32) >
            (_SC("CreateEx"), &Vehicle_CreateEx)
        .StaticOverload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Float32, Int32, Int32, Int32, Object &) >
            (_SC("CreateEx"), &Vehicle_CreateEx)
        .StaticOverload< Object & (*)(Int32, Int32, const Vector3 &, Float32, Int32, Int32) >
            (_SC("Create"), &Vehicle_Create)
        .StaticOverload< Object & (*)(Int32, Int32, const Vector3 &, Float32, Int32, Int32, Int32, Object &) >
            (_SC("Create"), &Vehicle_Create)
    );
}

} // Namespace:: SqMod
