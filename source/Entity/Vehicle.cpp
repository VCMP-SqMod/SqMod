#include "Entity/Vehicle.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Vector4.hpp"
#include "Misc/Automobile.hpp"
#include "Core.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CAutomobile  CVehicle::s_Automobile;

// ------------------------------------------------------------------------------------------------
Vector3      CVehicle::s_Vector3;
Vector4      CVehicle::s_Vector4;
Quaternion   CVehicle::s_Quaternion;

// ------------------------------------------------------------------------------------------------
CVehicle::CVehicle(const Reference< CVehicle > & o)
    : Reference(o)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::IsStreamedFor(const Reference< CPlayer > & player) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->IsVehicleStreamedForPlayer(m_ID, player);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <see if vehicle is streamed for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <see if vehicle is streamed for player> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CVehicle::GetSyncSource() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleSyncSource(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle synchronization source> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CVehicle::GetSyncType() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleSyncType(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle synchronization type> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CVehicle::GetWorld() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleWorld(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle world> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetWorld(SQInt32 world) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleWorld(m_ID, world);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle world> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const CAutomobile & CVehicle::GetModel() const
{
    // Clear any previous model
    s_Automobile.SetID(SQMOD_UNKNOWN);
    // Attempt to retrieve the model
    if (VALID_ENTITY(m_ID))
    {
        s_Automobile.SetID(_Func->GetVehicleModel(m_ID));
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle model> using an invalid reference: %d"), m_ID);
    }
    // Return the model that could be retrieved
    return s_Automobile;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CVehicle::GetModelID() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleModel(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle model id> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
Reference< CPlayer > CVehicle::GetOccupant(SQInt32 slot) const
{
    if (VALID_ENTITY(m_ID))
    {
        return Reference< CPlayer >(_Func->GetVehicleOccupant(m_ID, slot));
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle slot occupant> using an invalid reference: %d"), m_ID);
    }

    return Reference< CPlayer >();
}

// ------------------------------------------------------------------------------------------------
SQInt32 CVehicle::GetOccupantID(SQInt32 slot) const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleOccupant(m_ID, slot);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle slot occupant id> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::Respawn() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RespawnVehicle(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <respawn vehicle> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CVehicle::GetImmunity() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleImmunityFlags(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle immunity flags> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetImmunity(SQInt32 flags) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleImmunityFlags(m_ID, flags);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle immunity flags> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::IsWrecked() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsVehicleWrecked(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <see if vehicle is wrecked> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetPosition() const
{
    // Clear any previous position
    s_Vector3.Clear();
    // Attempt to retrieve the position
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetVehiclePos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle position> using an invalid reference: %d"), m_ID);
    }
    // Return the position that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPosition(const Vector3 & pos) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehiclePos(m_ID, pos.x, pos.y, pos.z, false);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionEx(const Vector3 & pos, bool empty) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehiclePos(m_ID, pos.x, pos.y, pos.z, empty);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehiclePos(m_ID, x, y, z, false);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPositionEx(SQFloat x, SQFloat y, SQFloat z, bool empty) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehiclePos(m_ID, x, y, z, empty);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Quaternion & CVehicle::GetRotation() const
{
    // Clear any previous rotation
    s_Quaternion.Clear();
    // Attempt to retrieve the rotation
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetVehicleRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, &s_Quaternion.z, &s_Quaternion.w);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle rotation> using an invalid reference: %d"), m_ID);
    }
    // Return the rotation that could be retrieved
    return s_Quaternion;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotation(const Quaternion & rot) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleRot(m_ID, rot.x, rot.y, rot.z, rot.w);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle rotation> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleRot(m_ID, x, y, z, w);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle rotation> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetRotationEuler() const
{
    // Clear any previous rotation
    s_Vector3.Clear();
    // Attempt to retrieve the rotation
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetVehicleRotEuler(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle euler rotation> using an invalid reference: %d"), m_ID);
    }
    // Return the rotation that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationEuler(const Vector3 & rot) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleRotEuler(m_ID, rot.x, rot.y, rot.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle euler rotation> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRotationEulerEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleRotEuler(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle euler rotation> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetSpeed() const
{
    // Clear any previous speed
    s_Vector3.Clear();
    // Attempt to retrieve the speed
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetVehicleSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle speed> using an invalid reference: %d"), m_ID);
    }
    // Return the speed that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpeed(const Vector3 & vel) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleSpeed(m_ID, vel.x, vel.y, vel.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpeedEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleSpeed(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddSpeed(const Vector3 & vel) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->AddVehicleSpeed(m_ID, vel.x, vel.y, vel.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <add vehicle speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddSpeedEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->AddVehicleSpeed(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <add vehicle speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetRelSpeed() const
{
    // Clear any previous speed
    s_Vector3.Clear();
    // Attempt to retrieve the speed
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetVehicleRelSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle relative speed> using an invalid reference: %d"), m_ID);
    }
    // Return the speed that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelSpeed(const Vector3 & vel) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleRelSpeed(m_ID, vel.x, vel.y, vel.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle relative speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelSpeedEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleRelSpeed(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle relative speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelSpeed(const Vector3 & vel) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->AddVehicleRelSpeed(m_ID, vel.x, vel.y, vel.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <add vehicle relative speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelSpeedEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->AddVehicleRelSpeed(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <add vehicle relative speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetTurnSpeed() const
{
    // Clear any previous speed
    s_Vector3.Clear();
    // Attempt to retrieve the speed
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetVehicleTurnSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle turn speed> using an invalid reference: %d"), m_ID);
    }
    // Return the speed that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTurnSpeed(const Vector3 & vel) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleTurnSpeed(m_ID, vel.x, vel.y, vel.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle turn speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTurnSpeedEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleTurnSpeed(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle turn speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddTurnSpeed(const Vector3 & vel) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->AddVehicleTurnSpeed(m_ID, vel.x, vel.y, vel.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <add vehicle turn speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddTurnSpeedEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->AddVehicleTurnSpeed(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <add vehicle turn speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetRelTurnSpeed() const
{
    // Clear any previous speed
    s_Vector3.Clear();
    // Attempt to retrieve the speed
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetVehicleRelTurnSpeed(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle relative turn speed> using an invalid reference: %d"), m_ID);
    }
    // Return the speed that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelTurnSpeed(const Vector3 & vel) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleRelTurnSpeed(m_ID, vel.x, vel.y, vel.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle relative turn speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRelTurnSpeedEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleRelTurnSpeed(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle relative turn speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelTurnSpeed(const Vector3 & vel) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->AddVehicleRelTurnSpeed(m_ID, vel.x, vel.y, vel.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <add vehicle relative turn speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::AddRelTurnSpeedEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->AddVehicleRelTurnSpeed(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <add vehicle relative turn speed> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Vector4 & CVehicle::GetSpawnPosition() const
{
    // Clear any previous position
    s_Vector4.Clear();
    // Attempt to retrieve the position
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetVehicleSpawnPos(m_ID, &s_Vector4.x, &s_Vector4.y, &s_Vector4.z, &s_Vector4.w);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle spawn position> using an invalid reference: %d"), m_ID);
    }
    // Return the position that could be retrieved
    return s_Vector4;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnPosition(const Vector4 & pos) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleSpawnPos(m_ID, pos.x, pos.y, pos.z, pos.w);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle spawn position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnPositionEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleSpawnPos(m_ID, x, y, z, w);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle spawn position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Quaternion & CVehicle::GetSpawnRotation() const
{
    // Clear any previous rotation
    s_Quaternion.Clear();
    // Attempt to retrieve the rotation
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetVehicleSpawnRot(m_ID, &s_Quaternion.x, &s_Quaternion.y, &s_Quaternion.z, &s_Quaternion.w);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle spawn rotation> using an invalid reference: %d"), m_ID);
    }
    // Return the rotation that could be retrieved
    return s_Quaternion;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotation(const Quaternion & rot) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleSpawnRot(m_ID, rot.x, rot.y, rot.z, rot.w);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle spawn rotation> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotationEx(SQFloat x, SQFloat y, SQFloat z, SQFloat w) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleSpawnRot(m_ID, x, y, z, w);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle spawn rotation> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CVehicle::GetSpawnRotationEuler() const
{
    // Clear any previous rotation
    s_Vector3.Clear();
    // Attempt to retrieve the rotation
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetVehicleSpawnRotEuler(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle euler spawn rotation> using an invalid reference: %d"), m_ID);
    }
    // Return the rotation that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotationEuler(const Vector3 & rot) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleSpawnRotEuler(m_ID, rot.x, rot.y, rot.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle euler spawn rotation> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSpawnRotationEulerEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleSpawnRotEuler(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle euler spawn rotation> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQUint32 CVehicle::GetRespawnTimer() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleIdleRespawnTimer(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle respawn timer> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_BLANK;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRespawnTimer(SQUint32 timer) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleIdleRespawnTimer(m_ID, timer);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle respawn timer> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQFloat CVehicle::GetHealth() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleHealth(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle health> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetHealth(SQFloat amount) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleHealth(m_ID, amount);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle health> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CVehicle::GetPrimaryColor() const
{
    if (VALID_ENTITY(m_ID))
    {
        SQInt32 primary;
        _Func->GetVehicleColour(m_ID, &primary, NULL);
        return primary;
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle primary color> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPrimaryColor(SQInt32 col) const
{
    if (VALID_ENTITY(m_ID) && VALID_VEHCOL(col))
    {
        SQInt32 secondary;
        _Func->GetVehicleColour(m_ID, NULL, &secondary);
        _Func->SetVehicleColour(m_ID, col, secondary);
    }
    else if (!VALID_VEHCOL(col))
    {
        LogWrn(_SC("Attempting to <set vehicle primary color> using an invalid argument: %d"), col);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle primary color> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CVehicle::GetSecondaryColor() const
{
    if (VALID_ENTITY(m_ID))
    {
        SQInt32 secondary;
        _Func->GetVehicleColour(m_ID, NULL, &secondary);
        return secondary;
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle secondary color> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetSecondaryColor(SQInt32 col) const
{
    if (VALID_ENTITY(m_ID) && VALID_VEHCOL(col))
    {
        SQInt32 primary;
        _Func->GetVehicleColour(m_ID, &primary, NULL);
        _Func->SetVehicleColour(m_ID, primary, col);
    }
    else if (!VALID_VEHCOL(col))
    {
        LogWrn(_SC("Attempting to <set vehicle secondary color> using an invalid argument: %d"), col);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle secondary color> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetColors(SQInt32 primary, SQInt32 secondary) const
{
    if (VALID_ENTITY(m_ID) && VALID_VEHCOL(primary) && VALID_VEHCOL(secondary))
    {
        _Func->SetVehicleColour(m_ID, primary, secondary);
    }
    else if (!VALID_VEHCOL(primary) || !VALID_VEHCOL(secondary))
    {
        LogWrn(_SC("Attempting to <set vehicle primary color> using an invalid argument: %d, %d"), primary, secondary);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle primary color> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::IsLocked() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleDoorsLocked(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <see if vehicle is locked> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetLocked(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleDoorsLocked(m_ID, toggle);
    }
    else
    {
        LogWrn(_SC("Attempting to <set whether vehicle is locked> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CVehicle::GetPartStatus(SQInt32 part) const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehiclePartStatus(m_ID, part);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle part status> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetPartStatus(SQInt32 part, SQInt32 status) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehiclePartStatus(m_ID, part, status);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle part status> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CVehicle::GetTyreStatus(SQInt32 tyre) const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleTyreStatus(m_ID, tyre);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle tyre status> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetTyreStatus(SQInt32 tyre, SQInt32 status) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleTyreStatus(m_ID, tyre, status);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle tyre status> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQUint32 CVehicle::GetDamageData() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleDamageData(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle damage data> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_BLANK;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetDamageData(SQUint32 data) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleDamageData(m_ID, data);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle damage data> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::HasAlarm() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleAlarm(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <see if vehicle has alarm> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetAlarm(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleAlarm(m_ID, toggle);
    }
    else
    {
        LogWrn(_SC("Attempting to <set whether vehicle has alarm> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::HasLights() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleLights(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <see if vehicle has lights> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetLights(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleLights(m_ID, toggle);
    }
    else
    {
        LogWrn(_SC("Attempting to <set whether vehicle has lights> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CVehicle::GetRadio() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleRadio(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle radio> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRadio(SQInt32 radio) const
{
    if (VALID_ENTITY(m_ID) && VALID_ENTITY(radio))
    {
        _Func->SetVehicleRadio(m_ID, radio);
    }
    else if (INVALID_ENTITY(radio))
    {
        LogWrn(_SC("Attempting to <set vehicle radio> using an invalid argument: %d"), radio);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle radio> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::IsRadioLocked() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsVehicleRadioLocked(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <see if vehicle has radio locked> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetRadioLocked(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleRadioLocked(m_ID, toggle);
    }
    else
    {
        LogWrn(_SC("Attempting to <set whether vehicle has radio locked> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::IsGhostState() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleGhostState(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <see if vehicle is in ghost state> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetGhostState(bool toggle) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetVehicleGhostState(m_ID, toggle);
    }
    else
    {
        LogWrn(_SC("Attempting to <set whether vehicle is in ghost state> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::ResetHandling() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->ResetInstHandling(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <reset all vehicle handling rules> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::ResetHandling(SQInt32 rule) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->ResetInstHandlingRule(m_ID, rule);
    }
    else
    {
        LogWrn(_SC("Attempting to <reset vehicle handling rules> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::ExistsHandling(SQInt32 rule) const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->ExistsInstHandlingRule(m_ID, rule);
    }
    else
    {
        LogWrn(_SC("Attempting to <see if vehicle handling rule exists> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQFloat CVehicle::GetHandlingData(SQInt32 rule) const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetInstHandlingRule(m_ID, rule);
    }
    else
    {
        LogWrn(_SC("Attempting to <get vehicle handling data> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CVehicle::SetHandlingData(SQInt32 rule, SQFloat data) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetInstHandlingRule(m_ID, rule, data);
    }
    else
    {
        LogWrn(_SC("Attempting to <set vehicle handling data> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::Embark(const Reference< CPlayer > & player) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->PutPlayerInVehicle(player, m_ID, 0, true, true);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <embark player in vehicle> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <embark player in vehicle> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CVehicle::Embark(const Reference< CPlayer > & player, SQInt32 slot, bool allocate, bool warp) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->PutPlayerInVehicle(player, m_ID, slot, allocate, warp);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <embark player in vehicle> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <embark player in vehicle> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
Reference< CVehicle > CreateBaseVehicle_PEF(SQInt32 model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CVehicle > CreateBaseVehicle_PEF(SQInt32 model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CVehicle > CreateBaseVehicle_PCF(SQInt32 model, SQInt32 world, const Vector3 & pos,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CVehicle > CreateBaseVehicle_PCF(SQInt32 model, SQInt32 world, const Vector3 & pos,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CVehicle > CreateBaseVehicle_EF(const CAutomobile & model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CVehicle > CreateBaseVehicle_EF(const CAutomobile & model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CVehicle > CreateBaseVehicle_CF(const CAutomobile & model, SQInt32 world,
                        const Vector3 & pos, SQFloat angle,
                        SQInt32 primary, SQInt32 secondary)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CVehicle > CreateBaseVehicle_CF(const CAutomobile & model, SQInt32 world,
                        const Vector3 & pos, SQFloat angle, SQInt32 primary, SQInt32 secondary,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CVehicle CreateVehicle_PEF(SQInt32 model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CVehicle CreateVehicle_PEF(SQInt32 model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CVehicle CreateVehicle_PCF(SQInt32 model, SQInt32 world, const Vector3 & pos,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CVehicle CreateVehicle_PCF(SQInt32 model, SQInt32 world, const Vector3 & pos,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CVehicle CreateVehicle_EF(const CAutomobile & model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CVehicle CreateVehicle_EF(const CAutomobile & model, SQInt32 world,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQFloat angle, SQInt32 primary, SQInt32 secondary,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewVehicle(model, world, x, y, z, angle, primary, secondary,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CVehicle CreateVehicle_CF(const CAutomobile & model, SQInt32 world,
                        const Vector3 & pos, SQFloat angle,
                        SQInt32 primary, SQInt32 secondary)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CVehicle CreateVehicle_CF(const CAutomobile & model, SQInt32 world,
                        const Vector3 & pos, SQFloat angle, SQInt32 primary, SQInt32 secondary,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewVehicle(model, world, pos.x, pos.y, pos.z, angle, primary, secondary,
                            header, payload);
}

// ================================================================================================
bool Register_CVehicle(HSQUIRRELVM vm)
{
    // Attempt to register the base reference type before the actual implementation
    if (!Register_Reference< CVehicle >(vm, _SC("BaseVehicle")))
    {
        LogFtl("Unable to register the base class <BaseVehicle> for <CVehicle> type");
        // Registration failed
        return false;
    }
    // Typedef the base reference type for simplicity
    typedef Reference< CVehicle > RefType;
    // Output debugging information
    LogDbg("Beginning registration of <CVehicle> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CVehicle"), Sqrat::DerivedClass< CVehicle, RefType >(vm, _SC("CVehicle"))
        .Ctor()
        .Ctor< SQInt32 >()
    );
    // Output debugging information
    LogDbg("Registration of <CVehicle> type was successful");
    // Output debugging information
    LogDbg("Beginning registration of <Vehicle functions> type");
    // Register global functions related to this entity type
    Sqrat::RootTable(vm)
    /* Create BaseVehicle [P]rimitive [E]xtended [F]Full */
    .Overload< RefType (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32) >
        (_SC("CreateBaseVehicle_PEF"), &CreateBaseVehicle_PEF)
    .Overload< RefType (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseVehicle_PEF"), &CreateBaseVehicle_PEF)
    /* Create BaseVehicle [P]rimitive [C]ompact [F]ull */
    .Overload< RefType (*)(SQInt32, SQInt32, const Vector3 &, SQFloat, SQInt32, SQInt32) >
        (_SC("CreateBaseVehicle_PCF"), &CreateBaseVehicle_PCF)
    .Overload< RefType (*)(SQInt32, SQInt32, const Vector3 &, SQFloat, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseVehicle_PCF"), &CreateBaseVehicle_PCF)
    /* Create BaseVehicle [E]xtended [F]Full */
    .Overload< RefType (*)(const CAutomobile &, SQInt32, SQFloat, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32) >
        (_SC("CreateBaseVehicle_EF"), &CreateBaseVehicle_EF)
    .Overload< RefType (*)(const CAutomobile &, SQInt32, SQFloat, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseVehicle_EF"), &CreateBaseVehicle_EF)
    /* Create BaseVehicle [C]ompact [F]ull */
    .Overload< RefType (*)(const CAutomobile &, SQInt32, const Vector3 &, SQFloat, SQInt32, SQInt32) >
        (_SC("CreateBaseVehicle_CF"), &CreateBaseVehicle_CF)
    .Overload< RefType (*)(const CAutomobile &, SQInt32, const Vector3 &, SQFloat, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseVehicle_CF"), &CreateBaseVehicle_CF)
    /* Create CVehicle [P]rimitive [E]xtended [F]Full */
    .Overload< CVehicle (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32) >
        (_SC("CreateVehicle_PEF"), &CreateVehicle_PEF)
    .Overload< CVehicle (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateVehicle_PEF"), &CreateVehicle_PEF)
    /* Create CVehicle [P]rimitive [C]ompact [F]ull */
    .Overload< CVehicle (*)(SQInt32, SQInt32, const Vector3 &, SQFloat, SQInt32, SQInt32) >
        (_SC("CreateVehicle_PCF"), &CreateVehicle_PCF)
    .Overload< CVehicle (*)(SQInt32, SQInt32, const Vector3 &, SQFloat, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateVehicle_PCF"), &CreateVehicle_PCF)
    /* Create CVehicle [E]xtended [F]Full */
    .Overload< CVehicle (*)(const CAutomobile &, SQInt32, SQFloat, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32) >
        (_SC("CreateVehicle_EF"), &CreateVehicle_EF)
    .Overload< CVehicle (*)(const CAutomobile &, SQInt32, SQFloat, SQFloat, SQFloat, SQFloat, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateVehicle_EF"), &CreateVehicle_EF)
    /* Create CVehicle [C]ompact [F]ull */
    .Overload< CVehicle (*)(const CAutomobile &, SQInt32, const Vector3 &, SQFloat, SQInt32, SQInt32) >
        (_SC("CreateVehicle_CF"), &CreateVehicle_CF)
    .Overload< CVehicle (*)(const CAutomobile &, SQInt32, const Vector3 &, SQFloat, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateVehicle_CF"), &CreateVehicle_CF);
    // Output debugging information
    LogDbg("Registration of <Vehicle functions> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
