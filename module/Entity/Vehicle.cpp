// ------------------------------------------------------------------------------------------------
#include "Entity/Vehicle.hpp"
#include "Entity/Player.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Vector2.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"
#include "Core/Areas.hpp"
#include "Core/Tasks.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqVehicle"))

// ------------------------------------------------------------------------------------------------
const int32_t CVehicle::Max = SQMOD_VEHICLE_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CVehicle::SqGetNull(HSQUIRRELVM vm)
{
    sq_pushobject(vm, Core::Get().GetNullVehicle().GetObj());
    return 1;
}

// ------------------------------------------------------------------------------------------------
LightObj & CVehicle::GetNull()
{
    return Core::Get().GetNullVehicle();
}

// ------------------------------------------------------------------------------------------------
CVehicle::CVehicle(int32_t id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_VEHICLE_POOL))
    , m_Tag(fmt::format("{}", id)), m_Data(), m_CircularLocks(0)
{
    /* ... */
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
void CVehicle::SetTag(StackStrF & tag)
{
    if (tag.mLen > 0)
    {
        m_Tag.assign(tag.mPtr, static_cast< size_t >(tag.mLen));
    }
    else
    {
        m_Tag.clear();
    }
}

// ------------------------------------------------------------------------------------------------
CVehicle & CVehicle::ApplyTag(StackStrF & tag)
{
    SetTag(tag);
    return *this;
}

// ------------------------------------------------------------------------------------------------
LightObj & CVehicle::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetData(LightObj & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::Destroy(int32_t header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return Core::Get().DelVehicle(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
LightObj & CVehicle::GetEvents() const
{
    // Validate the managed identifier
    Validate();
    // Return the associated event table
    return Core::Get().GetVehicle(m_ID).mEvents;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::CustomEvent(int32_t header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested action
    Core::Get().EmitVehicleCustom(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::IsStreamedFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsVehicleStreamedForPlayer(m_ID, player.GetID());
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetOption(int32_t option_id) const
{
    // Attempt to obtain the current value of the specified option
    const bool value = _Func->GetVehicleOption(m_ID, static_cast< vcmpVehicleOption >(option_id));
    // Check for errors
    if (_Func->GetLastError() == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid option identifier: {}", option_id);
    }
    // Return the requested value
    return value;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetOption(int32_t option_id, bool toggle)
{
    // Attempt to obtain the current value of the specified option
    const bool value = _Func->GetVehicleOption(m_ID, static_cast< vcmpVehicleOption >(option_id));
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_OPTION))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_OPTION);
        // Now forward the event call
        Core::Get().EmitVehicleOption(m_ID, option_id, value, 0, NullLightObj());
    }
    // Attempt to modify the current value of the specified option
    if (_Func->SetVehicleOption(m_ID, static_cast< vcmpVehicleOption >(option_id),
                                static_cast< uint8_t >(toggle)) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid option identifier: {}", option_id);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetOptionEx(int32_t option_id, bool toggle, int32_t header, LightObj & payload)
{
    // Attempt to obtain the current value of the specified option
    const bool value = _Func->GetVehicleOption(m_ID, static_cast< vcmpVehicleOption >(option_id));
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_OPTION))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_OPTION);
        // Now forward the event call
        Core::Get().EmitVehicleOption(m_ID, option_id, value, header, payload);
    }
    // Attempt to modify the current value of the specified option
    if (_Func->SetVehicleOption(m_ID, static_cast< vcmpVehicleOption >(option_id),
                                static_cast< uint8_t >(toggle)) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid option identifier: {}", option_id);
    }
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetSyncSource() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleSyncSource(m_ID);
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetSyncType() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleSyncType(m_ID);
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetWorld(int32_t world)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const int32_t current = _Func->GetVehicleWorld(m_ID);
    // Don't even bother if it's the same value
    if (current == world)
    {
        return;
    }
    // Avoid infinite recursive event loops
    else if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_WORLD))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_WORLD);
        // Now forward the event call
        Core::Get().EmitVehicleWorld(m_ID, current, world);
    }
    // Avoid property unwind from a recursive call
    _Func->SetVehicleWorld(m_ID, world);
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetModel() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleModel(m_ID);
}

// ------------------------------------------------------------------------------------------------
LightObj & CVehicle::GetOccupant(int32_t slot) const
{
    // Validate the managed identifier
    Validate();
    // Attempt to retrieve the requested information
    const int id = _Func->GetVehicleOccupant(m_ID, slot);
    // Was there an issue with the given value?
    if (INVALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
    {
        const vcmpError err = _Func->GetLastError();
        // Identify the type of error and at least log it
        if (err == vcmpErrorArgumentOutOfBounds)
        {
            LogWrn("Out of range slot [%d]", slot);
        }
        else if (err == vcmpErrorNoSuchEntity)
        {
            LogWrn("Unoccupied slot [%d]", id);
        }
        else
        {
            LogWrn("Error while getting occupant at [%d] for [%s]", slot, m_Tag.c_str());
        }
        // Default to a null instance
        return Core::Get().GetNullVehicle();
    }
    // Return the requested information
    return Core::Get().GetPlayer(id).mObj;
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetOccupantID(int32_t slot) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleOccupant(m_ID, slot);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::HasOccupant(int32_t slot) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    const int32_t id = _Func->GetVehicleOccupant(m_ID, slot);
    // Use the server errors to see if there was an occupant
    const vcmpError err = _Func->GetLastError();
    // Return whether there was no error and the returned ID is valid
    return (err == vcmpErrorNone) && VALID_ENTITYEX(id, SQMOD_PLAYER_POOL);
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
uint32_t CVehicle::GetImmunity() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleImmunityFlags(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetImmunity(uint32_t flags)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const uint32_t current = _Func->GetVehicleImmunityFlags(m_ID);
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_IMMUNITY))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_IMMUNITY);
        // Now forward the event call
        Core::Get().EmitVehicleImmunity(m_ID, static_cast< int32_t >(current), static_cast< int32_t >(flags));
    }
    // Avoid property unwind from a recursive call
    _Func->SetVehicleImmunityFlags(m_ID, static_cast< uint32_t >(flags));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::Explode() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->ExplodeVehicle(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::FlattenTyres(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    if(toggle)
    {
        _Func->SetVehicleTyreStatus(m_ID, 0, 0);
        _Func->SetVehicleTyreStatus(m_ID, 1, 0);
        _Func->SetVehicleTyreStatus(m_ID, 2, 0);
        _Func->SetVehicleTyreStatus(m_ID, 3, 0);
    }
    else
    {
        _Func->SetVehicleTyreStatus(m_ID, 0, 1);
        _Func->SetVehicleTyreStatus(m_ID, 1, 1);
        _Func->SetVehicleTyreStatus(m_ID, 2, 1);
        _Func->SetVehicleTyreStatus(m_ID, 3, 1);
    }
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
Vector3 CVehicle::GetPosition() const
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the values
    _Func->GetVehiclePosition(m_ID, &vec.x, &vec.y, &vec.z);
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPosition(const Vector3 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehiclePosition(m_ID, pos.x, pos.y, pos.z, static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionB(const Vector3 & pos, bool empty) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehiclePosition(m_ID, pos.x, pos.y, pos.z, static_cast< uint8_t >(empty));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehiclePosition(m_ID, x, y, z, static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionExB(float x, float y, float z, bool empty) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehiclePosition(m_ID, x, y, z, static_cast< uint8_t >(empty));
}

// ------------------------------------------------------------------------------------------------
Quaternion CVehicle::GetRotation() const
{
    // Validate the managed identifier
    Validate();
    // Create a default quaternion instance
    Quaternion q;
    // Query the server for the values
    _Func->GetVehicleRotation(m_ID, &q.x, &q.y, &q.z, &q.w);
    // Return the requested information
    return q;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotation(const Quaternion & rot) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRotation(m_ID, rot.x, rot.y, rot.z, rot.w);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationEx(float x, float y, float z, float w) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRotation(m_ID, x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
Vector3 CVehicle::GetRotationEuler() const
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the values
    _Func->GetVehicleRotationEuler(m_ID, &vec.x, &vec.y, &vec.z);
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationEuler(const Vector3 & rot) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRotationEuler(m_ID, rot.x, rot.y, rot.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationEulerEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleRotationEuler(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
Vector3 CVehicle::GetSpeed() const
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the values
    _Func->GetVehicleSpeed(m_ID, &vec.x, &vec.y, &vec.z, static_cast< uint8_t >(false));
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, vel.x, vel.y, vel.z,
                           static_cast< uint8_t >(false), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpeedEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, vel.x, vel.y, vel.z,
                           static_cast< uint8_t >(true), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddSpeedEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, x, y, z, static_cast< uint8_t >(true), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
Vector3 CVehicle::GetRelativeSpeed() const
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the values
    _Func->GetVehicleSpeed(m_ID, &vec.x, &vec.y, &vec.z, static_cast< uint8_t >(true));
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelativeSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, vel.x, vel.y, vel.z,
                           static_cast< uint8_t >(false), static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelativeSpeedEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelativeSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, vel.x, vel.y, vel.z,
                           static_cast< uint8_t >(true), static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelativeSpeedEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, x, y, z, static_cast< uint8_t >(true), static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
Vector3 CVehicle::GetTurnSpeed() const
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the values
    _Func->GetVehicleTurnSpeed(m_ID, &vec.x, &vec.y, &vec.z, static_cast< uint8_t >(false));
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTurnSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, vel.x, vel.y, vel.z,
                               static_cast< uint8_t >(false), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTurnSpeedEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, x, y, z, static_cast< uint8_t >(false),
                               static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddTurnSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, vel.x, vel.y, vel.z,
                               static_cast< uint8_t >(true), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddTurnSpeedEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, x, y, z, static_cast< uint8_t >(true),
                               static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
Vector3 CVehicle::GetRelativeTurnSpeed() const
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the values
    _Func->GetVehicleTurnSpeed(m_ID, &vec.x, &vec.y, &vec.z, static_cast< uint8_t >(true));
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelativeTurnSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, vel.x, vel.y, vel.z,
                               static_cast< uint8_t >(false), static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelativeTurnSpeedEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, x, y, z, static_cast< uint8_t >(false),
                               static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelativeTurnSpeed(const Vector3 & vel) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, vel.x, vel.y, vel.z,
                               static_cast< uint8_t >(true), static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelativeTurnSpeedEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, x, y, z, static_cast< uint8_t >(true),
                               static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
Vector3 CVehicle::GetSpawnPosition() const
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the values
    _Func->GetVehicleSpawnPosition(m_ID, &vec.x, &vec.y, &vec.z);
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnPosition(const Vector3 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnPosition(m_ID, pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnPositionEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
Quaternion CVehicle::GetSpawnRotation() const
{
    // Validate the managed identifier
    Validate();
    // Create a default quaternion instance
    Quaternion q;
    // Query the server for the values
    _Func->GetVehicleSpawnRotation(m_ID, &q.x, &q.y, &q.z, &q.w);
    // Return the requested information
    return q;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotation(const Quaternion & rot) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnRotation(m_ID, rot.x, rot.y, rot.z, rot.w);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotationEx(float x, float y, float z, float w) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnRotation(m_ID, x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
Vector3 CVehicle::GetSpawnRotationEuler() const
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the rotation values
    _Func->GetVehicleSpawnRotationEuler(m_ID, &vec.x, &vec.y, &vec.z);
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotationEuler(const Vector3 & rot) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnRotationEuler(m_ID, rot.x, rot.y, rot.z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotationEulerEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleSpawnRotationEuler(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
uint32_t CVehicle::GetIdleRespawnTimer() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleIdleRespawnTimer(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetIdleRespawnTimer(uint32_t millis) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleIdleRespawnTimer(m_ID, millis);
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetHealth() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleHealth(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetHealth(float amount) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleHealth(m_ID, amount);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::Fix() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleHealth(m_ID, 1000);
    _Func->SetVehicleDamageData(m_ID, 0);
    _Func->SetVehicleLightsData(m_ID, _Func->GetVehicleLightsData(m_ID) & 0xFFFFFF00);
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetPrimaryColor() const
{
    // Validate the managed identifier
    Validate();
    // The color value
    int32_t primary = -1, dummy;
    // Query the server for the requested color
    _Func->GetVehicleColour(m_ID, &primary, &dummy);
    // Return the requested information
    return primary;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPrimaryColor(int32_t col) const
{
    // Validate the managed identifier
    Validate();
    // The unchanged color value
    int32_t secondary = -1, dummy;
    // Query the server for the unchanged color
    _Func->GetVehicleColour(m_ID, &dummy, &secondary);
    // Perform the requested operation
    _Func->SetVehicleColour(m_ID, col, secondary);
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetSecondaryColor() const
{
    // Validate the managed identifier
    Validate();
    // The color value
    int32_t secondary = -1, dummy;
    // Query the server for the requested color
    _Func->GetVehicleColour(m_ID, &dummy, &secondary);
    // Return the requested information
    return secondary;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSecondaryColor(int32_t col) const
{
    // Validate the managed identifier
    Validate();
    // The unchanged color value
    int32_t primary = -1, dummy;
    // Query the server for the unchanged color
    _Func->GetVehicleColour(m_ID, &primary, &dummy);
    // Perform the requested operation
    _Func->SetVehicleColour(m_ID, primary, col);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetColors(int32_t primary, int32_t secondary) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetVehicleColour(m_ID, primary, secondary);
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetPartStatus(int32_t part) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehiclePartStatus(m_ID, part);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPartStatus(int32_t part, int32_t status)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const int32_t current = _Func->GetVehiclePartStatus(m_ID, part);
    // Don't even bother if it's the same value
    if (current == status)
    {
        return;
    }
    // Avoid infinite recursive event loops
    else if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_PARTSTATUS))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_PARTSTATUS);
        // Now forward the event call
        Core::Get().EmitVehiclePartStatus(m_ID, part, current, status);
    }
    // Avoid property unwind from a recursive call
    _Func->SetVehiclePartStatus(m_ID, part, status);
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetTyreStatus(int32_t tyre) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleTyreStatus(m_ID, tyre);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTyreStatus(int32_t tyre, int32_t status)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const int32_t current = _Func->GetVehicleTyreStatus(m_ID, tyre);
    // Don't even bother if it's the same value
    if (current == status)
    {
        return;
    }
    // Avoid infinite recursive event loops
    else if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_TYRESTATUS))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_TYRESTATUS);
        // Now forward the event call
        Core::Get().EmitVehicleTyreStatus(m_ID, tyre, current, status);
    }
    // Avoid property unwind from a recursive call
    _Func->SetVehicleTyreStatus(m_ID, tyre, status);
}

// ------------------------------------------------------------------------------------------------
uint32_t CVehicle::GetDamageData() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleDamageData(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetDamageData(uint32_t data)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const uint32_t current = _Func->GetVehicleDamageData(m_ID);
    // Don't even bother if it's the same value
    if (current == data)
    {
        return;
    }
    // Avoid infinite recursive event loops
    else if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_DAMAGEDATA))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_DAMAGEDATA);
        // Now forward the event call
        Core::Get().EmitVehicleDamageData(m_ID, current, data);
    }
    // Avoid property unwind from a recursive call
    _Func->SetVehicleDamageData(m_ID, data);
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetRadio() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetVehicleRadio(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRadio(int32_t radio)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const int32_t current = _Func->GetVehicleRadio(m_ID);
    // Don't even bother if it's the same value
    if (current == radio)
    {
        return;
    }
    // Avoid infinite recursive event loops
    else if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_RADIO))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_RADIO);
        // Now forward the event call
        Core::Get().EmitVehicleRadio(m_ID, current, radio);
    }
    // Avoid property unwind from a recursive call
    _Func->SetVehicleRadio(m_ID, radio);
}

// ------------------------------------------------------------------------------------------------
Vector2 CVehicle::GetTurretRotation() const
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector2 vec;
    // Query the server for the values
    _Func->GetVehicleTurretRotation(m_ID, &vec.x, &vec.y);
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetHorizontalTurretRotation() const
{
    // Validate the managed identifier
    Validate();
    // Where the rotation value is retrieved
    float rot = 0.0f, dummy;
    // Query the server for the turret rotation value
    _Func->GetVehicleTurretRotation(m_ID, &rot, &dummy);
    // Return the requested information
    return rot;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetVerticalTurretRotation() const
{
    // Validate the managed identifier
    Validate();
    // Where the rotation value is retrieved
    float rot = 0.0f, dummy;
    // Query the server for the turret rotation value
    _Func->GetVehicleTurretRotation(m_ID, &dummy, &rot);
    // Return the requested information
    return rot;
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::ExistsHandlingRule(int32_t rule) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->ExistsInstHandlingRule(m_ID, rule);
}

// ------------------------------------------------------------------------------------------------
SQFloat CVehicle::GetHandlingRule(int32_t rule) const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return static_cast< SQFloat >(_Func->GetInstHandlingRule(m_ID, rule));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetHandlingRule(int32_t rule, float data)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const auto current = static_cast< SQFloat >(_Func->GetInstHandlingRule(m_ID, rule));
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_HANDLINGRULE))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_HANDLINGRULE);
        // Now forward the event call
        Core::Get().EmitVehicleHandlingRule(m_ID, rule, current, data);
    }
    // Avoid property unwind from a recursive call
    _Func->SetInstHandlingRule(m_ID, rule, data);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::ResetHandlingRule(int32_t rule)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const auto current = static_cast< SQFloat >(_Func->GetInstHandlingRule(m_ID, rule));
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_HANDLINGRULE))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_HANDLINGRULE);
        // Now forward the event call
        Core::Get().EmitVehicleHandlingRule(m_ID, rule, current, static_cast< SQFloat >(_Func->GetInstHandlingRule(m_ID, rule)));
    }
    // Avoid property unwind from a recursive call
    _Func->ResetInstHandlingRule(m_ID, rule);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::ResetHandlings() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->ResetInstHandling(m_ID);
}

// ------------------------------------------------------------------------------------------------
uint32_t CVehicle::GetLightsData() const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the requested data
    return _Func->GetVehicleLightsData(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetLightsData(int32_t data) const
{
    // Validate the managed identifier
    Validate();
    // Apply the requested data
    _Func->SetVehicleLightsData(m_ID, static_cast< uint32_t >(data));
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::Embark(CPlayer & player)
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // If the player embarks in the same vehicle then ignore
    if (_Func->GetPlayerVehicleId(player.GetID()) == m_ID)
    {
        return true; // I guess this is somewhat successful
    }
    // Avoid infinite recursive event loops
    else if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_EMBARK))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_EMBARK);
        // Now forward the event call
        Core::Get().EmitPlayerEmbarking(player.GetID(), m_ID, 0);
    }
    // Perform the requested operation
    return (_Func->PutPlayerInVehicle(player.GetID(), m_ID, 0,
        static_cast< uint8_t >(true), static_cast< uint8_t >(true))
            != vcmpErrorRequestDenied);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::EmbarkEx(CPlayer & player, int32_t slot, bool allocate, bool warp)
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // If the player embarks in the same vehicle then ignore
    if (_Func->GetPlayerVehicleId(player.GetID()) == m_ID)
    {
        return true; // I guess this is somewhat successful
    }
    // Avoid infinite recursive event loops
    else if (!(m_CircularLocks & VEHICLECL_EMIT_VEHICLE_EMBARK))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, VEHICLECL_EMIT_VEHICLE_EMBARK);
        // Now forward the event call
        Core::Get().EmitPlayerEmbarking(player.GetID(), m_ID, 0);
    }
    // Perform the requested operation
    return (_Func->PutPlayerInVehicle(player.GetID(), m_ID, slot,
        static_cast< uint8_t >(allocate), static_cast< uint8_t >(warp)) != vcmpErrorRequestDenied);
}
#if SQMOD_SDK_LEAST(2, 1)
// ------------------------------------------------------------------------------------------------
void CVehicle::SetPlayer3DArrow(CPlayer & target, bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Spectate the given target
    _Func->SetVehicle3DArrowForPlayer(m_ID, target.GetID(), toggle);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetPlayer3DArrow(CPlayer & target) const
{
    // Validate the managed identifier
    Validate();
    // Spectate the given target
    return _Func->GetVehicle3DArrowForPlayer(m_ID, target.GetID());
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPlayer3DArrowID(SQInteger id, bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Spectate the given target
    _Func->SetVehicle3DArrowForPlayer(m_ID, id, toggle);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetPlayer3DArrowID(SQInteger id) const
{
    // Validate the managed identifier
    Validate();
    // Spectate the given target
    return _Func->GetVehicle3DArrowForPlayer(m_ID, id);
}
#endif
// ------------------------------------------------------------------------------------------------
bool CVehicle::GetCollideAreas() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return static_cast< bool >(Core::Get().GetVehicle(m_ID).mFlags & ENF_AREA_TRACK);
}

void CVehicle::SetCollideAreas(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    if (toggle)
    {
        Core::Get().GetVehicle(m_ID).mFlags |= ENF_AREA_TRACK;
    }
    else
    {
        // Obtain the actual entity instance
        auto & inst = Core::Get().GetVehicle(m_ID);
        // Is this option even enabled?
        if (!(inst.mFlags & ENF_AREA_TRACK))
        {
            return; // Not enabled to begin with
        }
        // Disable the option
        inst.mFlags ^= ENF_AREA_TRACK;
        // Clear current areas
        inst.mAreas.clear();
    }
}

void CVehicle::SetAreasCollide(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    if (toggle)
    {
        Core::Get().GetVehicle(m_ID).mFlags |= ENF_AREA_TRACK;
    }
    else
    {
        // Obtain the actual entity instance
        auto & inst = Core::Get().GetVehicle(m_ID);
        // Is this option even enabled?
        if (!(inst.mFlags & ENF_AREA_TRACK))
        {
            return; // Not enabled to begin with
        }
        // Disable the option
        inst.mFlags ^= ENF_AREA_TRACK;
        // Is the vehicle currently in any areas?
        if (inst.mAreas.empty())
        {
            return; // Nothing to test
        }

        Vector3 pos;
        // Obtain the current position of this instance
        _Func->GetVehiclePosition(m_ID, &pos.x, &pos.y, &pos.z);
        // Do a final check to see if the vehicle left any area
        for (auto & ap : inst.mAreas)
        {
            // Is the vehicle still in this area?
            if (!ap.first->TestEx(pos.x, pos.y))
            {
                Core::Get().EmitVehicleLeaveArea(m_ID, ap.second); // Emit the script event
            }
        }
        // Clear current areas
        inst.mAreas.clear();
    }
}

// ------------------------------------------------------------------------------------------------
SQInteger CVehicle::GetTrackPosition() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetVehicle(m_ID).mTrackPosition;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTrackPosition(SQInteger num) const
{
    // Validate the managed identifier
    Validate();
    // Assign the requested information
    Core::Get().GetVehicle(m_ID).mTrackPosition = num;
}

// ------------------------------------------------------------------------------------------------
SQInteger CVehicle::GetTrackRotation() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetVehicle(m_ID).mTrackRotation;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTrackRotation(SQInteger num) const
{
    // Validate the managed identifier
    Validate();
    // Assign the requested information
    Core::Get().GetVehicle(m_ID).mTrackRotation = num;
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetLastPrimaryColor() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetVehicle(m_ID).mLastPrimaryColor;
}

// ------------------------------------------------------------------------------------------------
int32_t CVehicle::GetLastSecondaryColor() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetVehicle(m_ID).mLastSecondaryColor;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetLastHealth() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetVehicle(m_ID).mLastHealth;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetLastPosition() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetVehicle(m_ID).mLastPosition;
}

// ------------------------------------------------------------------------------------------------
const Quaternion & CVehicle::GetLastRotation() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetVehicle(m_ID).mLastRotation;
}

// ------------------------------------------------------------------------------------------------
SQFloat CVehicle::GetDistance() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return static_cast< SQFloat >(Core::Get().GetVehicle(m_ID).mDistance);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetDistance(SQFloat distance) const
{
    // Validate the managed identifier
    Validate();
    // Assign the requested information
    Core::Get().GetVehicle(m_ID).mDistance = static_cast< double >(distance);
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetTrackDistance() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return static_cast< bool >(Core::Get().GetVehicle(m_ID).mFlags & ENF_DIST_TRACK);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTrackDistance(bool toggle) const
{
    // Validate the managed identifier
    Validate();
    // Assign the requested information
    if (toggle)
    {
        Core::Get().GetVehicle(m_ID).mFlags |= ENF_DIST_TRACK;
    }
    else
    {
        Core::Get().GetVehicle(m_ID).mFlags ^= ENF_DIST_TRACK;
    }
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetPositionX() const
{
    // Validate the managed identifier
    Validate();
    // Reserve a temporary float to retrieve the requested component
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehiclePosition(m_ID, &x, &dummy, &dummy);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetPositionY() const
{
    // Validate the managed identifier
    Validate();
    // Reserve a temporary float to retrieve the requested component
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehiclePosition(m_ID, &dummy, &y, &dummy);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetPositionZ() const
{
    // Validate the managed identifier
    Validate();
    // Reserve a temporary float to retrieve the requested component
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehiclePosition(m_ID, &dummy, &dummy, &z);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehiclePosition(m_ID, &dummy, &y, &z);
    // Perform the requested operation
    _Func->SetVehiclePosition(m_ID, x, y, z, static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehiclePosition(m_ID, &x, &dummy, &z);
    // Perform the requested operation
    _Func->SetVehiclePosition(m_ID, x, y, z, static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehiclePosition(m_ID, &x, &y, &dummy);
    // Perform the requested operation
    _Func->SetVehiclePosition(m_ID, x, y, z, static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetRotationX() const
{
    // Validate the managed identifier
    Validate();
    // Reserve a temporary float to retrieve the requested component
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleRotation(m_ID, &x, &dummy, &dummy, &dummy);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetRotationY() const
{
    // Validate the managed identifier
    Validate();
    // Reserve a temporary float to retrieve the requested component
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleRotation(m_ID, &dummy, &y, &dummy, &dummy);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetRotationZ() const
{
    // Validate the managed identifier
    Validate();
    // Reserve a temporary float to retrieve the requested component
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleRotation(m_ID, &dummy, &dummy, &z, &dummy);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetRotationW() const
{
    // Validate the managed identifier
    Validate();
    // Reserve a temporary float to retrieve the requested component
    float w = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleRotation(m_ID, &dummy, &dummy, &dummy, &w);
    // Return the requested information
    return w;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, w, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRotation(m_ID, &dummy, &y, &z, &w);
    // Perform the requested operation
    _Func->SetVehicleRotation(m_ID, x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, w, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRotation(m_ID, &x, &dummy, &z, &w);
    // Perform the requested operation
    _Func->SetVehicleRotation(m_ID, x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, w, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRotation(m_ID, &x, &y, &dummy, &w);
    // Perform the requested operation
    _Func->SetVehicleRotation(m_ID, x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationW(float w) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRotation(m_ID, &x, &y, &z, &dummy);
    // Perform the requested operation
    _Func->SetVehicleRotation(m_ID, x, y, z, w);
}


// ------------------------------------------------------------------------------------------------
float CVehicle::GetEulerRotationX() const
{
    // Validate the managed identifier
    Validate();
    // Reserve a temporary float to retrieve the requested component
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleRotationEuler(m_ID, &x, &dummy, &dummy);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetEulerRotationY() const
{
    // Validate the managed identifier
    Validate();
    // Reserve a temporary float to retrieve the requested component
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleRotationEuler(m_ID, &dummy, &y, &dummy);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetEulerRotationZ() const
{
    // Validate the managed identifier
    Validate();
    // Reserve a temporary float to retrieve the requested component
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleRotationEuler(m_ID, &dummy, &dummy, &z);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetEulerRotationX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRotationEuler(m_ID, &dummy, &y, &z);
    // Perform the requested operation
    _Func->SetVehicleRotationEuler(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetEulerRotationY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRotationEuler(m_ID, &x, &dummy, &z);
    // Perform the requested operation
    _Func->SetVehicleRotationEuler(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetEulerRotationZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleRotationEuler(m_ID, &x, &y, &dummy);
    // Perform the requested operation
    _Func->SetVehicleRotationEuler(m_ID, z, y, z);
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetSpeedX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleSpeed(m_ID, &x, &dummy, &dummy, static_cast< uint8_t >(false));
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetSpeedY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleSpeed(m_ID, &dummy, &y, &dummy, static_cast< uint8_t >(false));
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetSpeedZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleSpeed(m_ID, &dummy, &dummy, &z, static_cast< uint8_t >(false));
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpeedX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleSpeed(m_ID, &dummy, &y, &z, static_cast< uint8_t >(false));
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpeedY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleSpeed(m_ID, &x, &dummy, &z, static_cast< uint8_t >(false));
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpeedZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleSpeed(m_ID, &x, &y, &dummy, static_cast< uint8_t >(false));
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, z, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetRelativeSpeedX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleSpeed(m_ID, &x, &dummy, &dummy, static_cast< uint8_t >(true));
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetRelativeSpeedY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleSpeed(m_ID, &dummy, &y, &dummy, static_cast< uint8_t >(true));
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetRelativeSpeedZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleSpeed(m_ID, &dummy, &dummy, &z, static_cast< uint8_t >(true));
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelativeSpeedX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleSpeed(m_ID, &dummy, &y, &z, static_cast< uint8_t >(true));
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelativeSpeedY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleSpeed(m_ID, &x, &dummy, &z, static_cast< uint8_t >(true));
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelativeSpeedZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleSpeed(m_ID, &x, &y, &dummy, static_cast< uint8_t >(true));
    // Perform the requested operation
    _Func->SetVehicleSpeed(m_ID, z, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetTurnSpeedX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleTurnSpeed(m_ID, &x, &dummy, &dummy, static_cast< uint8_t >(false));
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetTurnSpeedY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleTurnSpeed(m_ID, &dummy, &y, &dummy, static_cast< uint8_t >(false));
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetTurnSpeedZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleTurnSpeed(m_ID, &dummy, &dummy, &z, static_cast< uint8_t >(false));
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTurnSpeedX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleTurnSpeed(m_ID, &dummy, &y, &z, static_cast< uint8_t >(false));
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTurnSpeedY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleTurnSpeed(m_ID, &x, &dummy, &z, static_cast< uint8_t >(false));
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTurnSpeedZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleTurnSpeed(m_ID, &x, &y, &dummy, static_cast< uint8_t >(false));
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, z, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(false));
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetRelativeTurnSpeedX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleTurnSpeed(m_ID, &x, &dummy, &dummy, static_cast< uint8_t >(true));
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetRelativeTurnSpeedY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleTurnSpeed(m_ID, &dummy, &y, &dummy, static_cast< uint8_t >(true));
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CVehicle::GetRelativeTurnSpeedZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous information, if any
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetVehicleTurnSpeed(m_ID, &dummy, &dummy, &z, static_cast< uint8_t >(true));
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelativeTurnSpeedX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleTurnSpeed(m_ID, &dummy, &y, &z, static_cast< uint8_t >(true));
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelativeTurnSpeedY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleTurnSpeed(m_ID, &x, &dummy, &z, static_cast< uint8_t >(true));
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, x, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(true));
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelativeTurnSpeedZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetVehicleTurnSpeed(m_ID, &x, &y, &dummy, static_cast< uint8_t >(true));
    // Perform the requested operation
    _Func->SetVehicleTurnSpeed(m_ID, z, y, z, static_cast< uint8_t >(false), static_cast< uint8_t >(true));
}
#ifdef VCMP_ENABLE_OFFICIAL
// ------------------------------------------------------------------------------------------------
LightObj & CVehicle::GetLegacyObject() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetVehicle(m_ID).mLgObj;
}
#endif

// ------------------------------------------------------------------------------------------------
static LightObj & Vehicle_CreateEx1a(int32_t model, int32_t world, float x, float y, float z, float angle,
                            int32_t primary, int32_t secondary)
{
    return Core::Get().NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullLightObj()).mObj;
}

static LightObj & Vehicle_CreateEx1b(int32_t model, int32_t world, float x, float y, float z, float angle,
                            int32_t primary, int32_t secondary, int32_t header, LightObj & payload)
{
    return Core::Get().NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            header, payload).mObj;
}

// ------------------------------------------------------------------------------------------------
static LightObj & Vehicle_Create1a(int32_t model, int32_t world, const Vector3 & pos, float angle,
                        int32_t primary, int32_t secondary)
{
    return Core::Get().NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullLightObj()).mObj;
}

static LightObj & Vehicle_Create1b(int32_t model, int32_t world, const Vector3 & pos, float angle,
                        int32_t primary, int32_t secondary, int32_t header, LightObj & payload)
{
    return Core::Get().NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            header, payload).mObj;
}

// ================================================================================================
void Register_CVehicle(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(Typename::Str,
        Class< CVehicle, NoConstructor< CVehicle > >(vm, Typename::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &CVehicle::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CVehicle::Max)
        // Core Properties
        .Prop(_SC("On"), &CVehicle::GetEvents)
        .Prop(_SC("ID"), &CVehicle::GetID)
        .Prop(_SC("Tag"), &CVehicle::GetTag, &CVehicle::SetTag)
        .Prop(_SC("Data"), &CVehicle::GetData, &CVehicle::SetData)
#ifdef VCMP_ENABLE_OFFICIAL
        .Prop(_SC("Legacy"), &CVehicle::GetLegacyObject)
#endif
        .Prop(_SC("Active"), &CVehicle::IsActive)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &CVehicle::ApplyTag)
        .Func(_SC("CustomEvent"), &CVehicle::CustomEvent)
        // Core Overloads
        .Overload(_SC("Destroy"), &CVehicle::Destroy0)
        .Overload(_SC("Destroy"), &CVehicle::Destroy1)
        .Overload(_SC("Destroy"), &CVehicle::Destroy)
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
        .Prop(_SC("EulerRot"), &CVehicle::GetRotationEuler, &CVehicle::SetRotationEuler)
        .Prop(_SC("EulerRotation"), &CVehicle::GetRotationEuler, &CVehicle::SetRotationEuler)
        .Prop(_SC("Speed"), &CVehicle::GetSpeed, &CVehicle::SetSpeed)
        .Prop(_SC("RelSpeed"), &CVehicle::GetRelativeSpeed, &CVehicle::SetRelativeSpeed)
        .Prop(_SC("RelativeSpeed"), &CVehicle::GetRelativeSpeed, &CVehicle::SetRelativeSpeed)
        .Prop(_SC("TurnSpeed"), &CVehicle::GetTurnSpeed, &CVehicle::SetTurnSpeed)
        .Prop(_SC("RelTurnSpeed"), &CVehicle::GetRelativeTurnSpeed, &CVehicle::SetRelativeTurnSpeed)
        .Prop(_SC("RelativeTurnSpeed"), &CVehicle::GetRelativeTurnSpeed, &CVehicle::SetRelativeTurnSpeed)
        .Prop(_SC("SpawnPos"), &CVehicle::GetSpawnPosition, &CVehicle::SetSpawnPosition)
        .Prop(_SC("SpawnPosition"), &CVehicle::GetSpawnPosition, &CVehicle::SetSpawnPosition)
        .Prop(_SC("SpawnRot"), &CVehicle::GetSpawnRotation, &CVehicle::SetSpawnRotation)
        .Prop(_SC("SpawnRotation"), &CVehicle::GetSpawnRotation, &CVehicle::SetSpawnRotation)
        .Prop(_SC("SpawnEulerRot"), &CVehicle::GetSpawnRotationEuler, &CVehicle::SetSpawnRotationEuler)
        .Prop(_SC("SpawnEulerRotation"), &CVehicle::GetSpawnRotationEuler, &CVehicle::SetSpawnRotationEuler)
        .Prop(_SC("IdleRespawnTimer"), &CVehicle::GetIdleRespawnTimer, &CVehicle::SetIdleRespawnTimer)
        .Prop(_SC("Health"), &CVehicle::GetHealth, &CVehicle::SetHealth)
        .Prop(_SC("PrimaryColor"), &CVehicle::GetPrimaryColor, &CVehicle::SetPrimaryColor)
        .Prop(_SC("SecondaryColor"), &CVehicle::GetSecondaryColor, &CVehicle::SetSecondaryColor)
        .Prop(_SC("PrimaryColour"), &CVehicle::GetPrimaryColor, &CVehicle::SetPrimaryColor)
        .Prop(_SC("SecondaryColour"), &CVehicle::GetSecondaryColor, &CVehicle::SetSecondaryColor)
        .Prop(_SC("DamageData"), &CVehicle::GetDamageData, &CVehicle::SetDamageData)
        .Prop(_SC("Radio"), &CVehicle::GetRadio, &CVehicle::SetRadio)
        .Prop(_SC("TurretRotation"), &CVehicle::GetTurretRotation)
        .Prop(_SC("HorTurretRotation"), &CVehicle::GetHorizontalTurretRotation)
        .Prop(_SC("HorizontalTurretRotation"), &CVehicle::GetHorizontalTurretRotation)
        .Prop(_SC("VerTurretRotation"), &CVehicle::GetVerticalTurretRotation)
        .Prop(_SC("VerticalTurretRotation"), &CVehicle::GetVerticalTurretRotation)
        .Prop(_SC("LightsData"), &CVehicle::GetLightsData, &CVehicle::SetLightsData)
        .Prop(_SC("CollideAreas"), &CVehicle::GetCollideAreas, &CVehicle::SetCollideAreas)
        .Prop(_SC("TrackPosition"), &CVehicle::GetTrackPosition, &CVehicle::SetTrackPosition)
        .Prop(_SC("TrackRotation"), &CVehicle::GetTrackRotation, &CVehicle::SetTrackRotation)
        .Prop(_SC("LastPrimaryColor"), &CVehicle::GetLastPrimaryColor)
        .Prop(_SC("LastSecondaryColor"), &CVehicle::GetLastSecondaryColor)
        .Prop(_SC("LastPrimaryColour"), &CVehicle::GetLastPrimaryColor)
        .Prop(_SC("LastSecondaryColour"), &CVehicle::GetLastSecondaryColor)
        .Prop(_SC("LastHealth"), &CVehicle::GetLastHealth)
        .Prop(_SC("LastPosition"), &CVehicle::GetLastPosition)
        .Prop(_SC("LastRotation"), &CVehicle::GetLastRotation)
        .Prop(_SC("Distance"), &CVehicle::GetDistance, &CVehicle::SetDistance)
        .Prop(_SC("TrackDistance"), &CVehicle::GetTrackDistance, &CVehicle::SetTrackDistance)
        .Prop(_SC("PosX"), &CVehicle::GetPositionX, &CVehicle::SetPositionX)
        .Prop(_SC("PosY"), &CVehicle::GetPositionY, &CVehicle::SetPositionY)
        .Prop(_SC("PosZ"), &CVehicle::GetPositionZ, &CVehicle::SetPositionZ)
        .Prop(_SC("RotX"), &CVehicle::GetRotationX, &CVehicle::SetRotationX)
        .Prop(_SC("RotY"), &CVehicle::GetRotationY, &CVehicle::SetRotationY)
        .Prop(_SC("RotZ"), &CVehicle::GetRotationZ, &CVehicle::SetRotationZ)
        .Prop(_SC("RotW"), &CVehicle::GetRotationW, &CVehicle::SetRotationW)
        .Prop(_SC("EulerX"), &CVehicle::GetEulerRotationX, &CVehicle::SetEulerRotationX)
        .Prop(_SC("EulerY"), &CVehicle::GetEulerRotationY, &CVehicle::SetEulerRotationY)
        .Prop(_SC("EulerZ"), &CVehicle::GetEulerRotationZ, &CVehicle::SetEulerRotationZ)
        .Prop(_SC("SpeedX"), &CVehicle::GetSpeedX, &CVehicle::SetSpeedX)
        .Prop(_SC("SpeedY"), &CVehicle::GetSpeedY, &CVehicle::SetSpeedY)
        .Prop(_SC("SpeedZ"), &CVehicle::GetSpeedZ, &CVehicle::SetSpeedZ)
        .Prop(_SC("RelSpeedX"), &CVehicle::GetRelativeSpeedX, &CVehicle::SetRelativeSpeedX)
        .Prop(_SC("RelSpeedY"), &CVehicle::GetRelativeSpeedY, &CVehicle::SetRelativeSpeedY)
        .Prop(_SC("RelSpeedZ"), &CVehicle::GetRelativeSpeedZ, &CVehicle::SetRelativeSpeedZ)
        .Prop(_SC("TurnSpeedX"), &CVehicle::GetTurnSpeedX, &CVehicle::SetTurnSpeedX)
        .Prop(_SC("TurnSpeedY"), &CVehicle::GetTurnSpeedY, &CVehicle::SetTurnSpeedY)
        .Prop(_SC("TurnSpeedZ"), &CVehicle::GetTurnSpeedZ, &CVehicle::SetTurnSpeedZ)
        .Prop(_SC("RelTurnSpeedX"), &CVehicle::GetRelativeTurnSpeedX, &CVehicle::SetRelativeTurnSpeedX)
        .Prop(_SC("RelTurnSpeedY"), &CVehicle::GetRelativeTurnSpeedY, &CVehicle::SetRelativeTurnSpeedY)
        .Prop(_SC("RelTurnSpeedZ"), &CVehicle::GetRelativeTurnSpeedZ, &CVehicle::SetRelativeTurnSpeedZ)
        // Member Methods
        .Func(_SC("StreamedFor"), &CVehicle::IsStreamedFor)
        .Func(_SC("GetOption"), &CVehicle::GetOption)
        .Func(_SC("SetOption"), &CVehicle::SetOption)
        .Func(_SC("SetOptionEx"), &CVehicle::SetOptionEx)
        .Func(_SC("Occupant"), &CVehicle::GetOccupant)
        .Func(_SC("OccupantID"), &CVehicle::GetOccupantID)
        .Func(_SC("HasOccupant"), &CVehicle::HasOccupant)
        .Func(_SC("Respawn"), &CVehicle::Respawn)
        .Func(_SC("Explode"), &CVehicle::Explode)
        .Func(_SC("FlattenTyres"), &CVehicle::FlattenTyres)
        .Func(_SC("SetRot"), &CVehicle::SetRotationEx)
        .Func(_SC("SetRotation"), &CVehicle::SetRotationEx)
        .Func(_SC("SetEulerRot"), &CVehicle::SetRotationEulerEx)
        .Func(_SC("SetEulerRotation"), &CVehicle::SetRotationEulerEx)
        .Func(_SC("SetSpeed"), &CVehicle::SetSpeedEx)
        .Func(_SC("SetRelativeSpeed"), &CVehicle::SetRelativeSpeedEx)
        .Func(_SC("SetTurnSpeed"), &CVehicle::SetTurnSpeedEx)
        .Func(_SC("SetRelativeTurnSpeed"), &CVehicle::SetRelativeTurnSpeedEx)
        .Func(_SC("SetSpawnPos"), &CVehicle::SetSpawnPositionEx)
        .Func(_SC("SetSpawnPosition"), &CVehicle::SetSpawnPositionEx)
        .Func(_SC("SetSpawnRot"), &CVehicle::SetSpawnRotationEx)
        .Func(_SC("SetSpawnRotation"), &CVehicle::SetSpawnRotationEx)
        .Func(_SC("SetSpawnEulerRot"), &CVehicle::SetSpawnRotationEulerEx)
        .Func(_SC("SetSpawnEulerRotation"), &CVehicle::SetSpawnRotationEulerEx)
        .Func(_SC("Fix"), &CVehicle::Fix)
        .Func(_SC("SetColors"), &CVehicle::SetColors)
        .Func(_SC("GetPartStatus"), &CVehicle::GetPartStatus)
        .Func(_SC("SetPartStatus"), &CVehicle::SetPartStatus)
        .Func(_SC("GetTyreStatus"), &CVehicle::GetTyreStatus)
        .Func(_SC("SetTyreStatus"), &CVehicle::SetTyreStatus)
        .Func(_SC("ExistsHandlingRule"), &CVehicle::ExistsHandlingRule)
        .Func(_SC("GetHandlingRule"), &CVehicle::GetHandlingRule)
        .Func(_SC("SetHandlingRule"), &CVehicle::SetHandlingRule)
        .Func(_SC("ResetHandlingRule"), &CVehicle::ResetHandlingRule)
        .Func(_SC("ResetHandlings"), &CVehicle::ResetHandlings)
#if SQMOD_SDK_LEAST(2, 1)
        .Func(_SC("SetPlayer3DArrow"), &CVehicle::SetPlayer3DArrow)
        .Func(_SC("GetPlayer3DArrow"), &CVehicle::GetPlayer3DArrow)
        .Func(_SC("SetPlayer3DArrowID"), &CVehicle::SetPlayer3DArrowID)
        .Func(_SC("GetPlayer3DArrowID"), &CVehicle::GetPlayer3DArrowID)
#endif
        .Func(_SC("AreasCollide"), &CVehicle::SetAreasCollide)
        // Member Overloads
        .Overload(_SC("SetPos"), &CVehicle::SetPosition)
        .Overload(_SC("SetPos"), &CVehicle::SetPositionB)
        .Overload(_SC("SetPos"), &CVehicle::SetPositionEx)
        .Overload(_SC("SetPos"), &CVehicle::SetPositionExB)
        .Overload(_SC("SetPosition"), &CVehicle::SetPosition)
        .Overload(_SC("SetPosition"), &CVehicle::SetPositionB)
        .Overload(_SC("SetPosition"), &CVehicle::SetPositionEx)
        .Overload(_SC("SetPosition"), &CVehicle::SetPositionExB)
        .Overload(_SC("AddSpeed"), &CVehicle::AddSpeed)
        .Overload(_SC("AddSpeed"), &CVehicle::AddSpeedEx)
        .Overload(_SC("AddRelativeSpeed"), &CVehicle::AddRelativeSpeed)
        .Overload(_SC("AddRelativeSpeed"), &CVehicle::AddRelativeSpeedEx)
        .Overload(_SC("AddTurnSpeed"), &CVehicle::AddTurnSpeed)
        .Overload(_SC("AddTurnSpeed"), &CVehicle::AddTurnSpeedEx)
        .Overload(_SC("AddRelativeTurnSpeed"), &CVehicle::AddRelativeTurnSpeed)
        .Overload(_SC("AddRelativeTurnSpeed"), &CVehicle::AddRelativeTurnSpeedEx)
        .Overload(_SC("ResetHandling"), &CVehicle::ResetHandlings)
        .Overload(_SC("ResetHandling"), &CVehicle::ResetHandlingRule)
        .Overload(_SC("Embark"), &CVehicle::Embark)
        .Overload(_SC("Embark"), &CVehicle::EmbarkEx)
        // Static Overloads
        .StaticOverload(_SC("CreateEx"), &Vehicle_CreateEx1a)
        .StaticOverload(_SC("CreateEx"), &Vehicle_CreateEx1b)
        .StaticOverload(_SC("Create"), &Vehicle_Create1a)
        .StaticOverload(_SC("Create"), &Vehicle_Create1b)
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("NullInst"), &CVehicle::SqGetNull)
        .SquirrelFunc(_SC("MakeTask"), &Tasks::MakeTask< CVehicle, ENT_VEHICLE >)
        .SquirrelFunc(_SC("DropTask"), &Tasks::DropTask< CVehicle, ENT_VEHICLE >)
        .SquirrelFunc(_SC("DoesTask"), &Tasks::DoesTask< CVehicle, ENT_VEHICLE >)
        .SquirrelFunc(_SC("FindTask"), &Tasks::FindTask< CVehicle, ENT_VEHICLE >)
    );
}

} // Namespace:: SqMod
