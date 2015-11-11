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
        BadArg("streamed_for", "see whether is streamed for player", _SCI32(player));
    }
    else
    {
        BadRef("streamed_for", "see whether is streamed for player");
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
        BadRef("@sync_source", "get synchronization source");
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
        BadRef("@sync_type", "get synchronization type");
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
        BadRef("@world", "get world");
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
        BadRef("@world", "set world");
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
        BadRef("@model", "get model");
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
        BadRef("@model_id", "get model id");
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
        BadRef("occupant", "get slot occupant");
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
        BadRef("occupant_id", "get slot occupant id");
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
        BadRef("respawn", "respawn instance");
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
        BadRef("@immunity", "get immunity flags");
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
        BadRef("@immunity", "set immunity flags");
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
        BadRef("@wrecked", "see whether is wrecked");
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
        BadRef("@position", "get position");
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
        BadRef("@position", "set position");
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
        BadRef("set_position", "set position");
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
        BadRef("set_position", "set position");
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
        BadRef("set_position", "set position");
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
        BadRef("@rotation", "get rotation");
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
        BadRef("@rotation", "set rotation");
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
        BadRef("set_rotation", "set rotation");
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
        BadRef("@rotation_euler", "get euler rotatio");
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
        BadRef("@rotation_euler", "set euler rotation");
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
        BadRef("set_rotation_euler", "set euler rotation");
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
        BadRef("@speed", "get speed");
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
        BadRef("@speed", "set speed");
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
        BadRef("set_speed", "set speed");
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
        BadRef("add_speed", "add to speed");
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
        BadRef("add_speed", "add to speed");
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
        BadRef("@rel_speed", "get relative speed");
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
        BadRef("@rel_speed", "set relative speed");
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
        BadRef("set_rel_speed", "set relative speed");
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
        BadRef("add_rel_speed", "add to relative speed");
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
        BadRef("add_rel_speed", "add to relative speed");
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
        BadRef("@turn_speed", "get turn speed");
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
        BadRef("@turn_speed", "set turn speed");
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
        BadRef("set_turn_speed", "set turn speed");
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
        BadRef("add_turn_speed", "add to turn speed");
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
        BadRef("add_turn_speed", "add to turn speed");
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
        BadRef("@rel_turn_speed", "get relative turn speed");
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
        BadRef("@rel_turn_speed", "set relative turn speed");
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
        BadRef("set_rel_turn_speed", "set relative turn speed");
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
        BadRef("add_rel_turn_speed", "add to relative turn speed");
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
        BadRef("add_rel_turn_speed", "add to relative turn speed");
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
        BadRef("@spawn_position", "get spawn position");
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
        BadRef("@spawn_position", "set spawn position");
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
        BadRef("set_spawn_position", "set spawn position");
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
        BadRef("@spawn_rotation", "get spawn rotation");
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
        BadRef("@spawn_rotation", "set spawn rotation");
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
        BadRef("set_spawn_rotation", "set spawn rotation");
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
        BadRef("@spawn_rotation_euler", "get spawn euler rotation");
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
        BadRef("@spawn_rotation_euler", "set spawn euler rotation");
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
        BadRef("set_spawn_rotation_euler", "set spawn euler rotation");
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
        BadRef("@respawn_timer", "get respawn timer");
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
        BadRef("@respawn_timer", "set respawn timer");
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
        BadRef("@health", "get health");
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
        BadRef("@health", "set health");
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
        BadRef("@primary_color", "get primary color");
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
        BadArg("@primary_color", "set primary color", col);
    }
    else
    {
        BadRef("@primary_color", "set primary color");
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
        BadRef("@secondary_color", "get secondary color");
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
        BadArg("@secondary_color", "set secondary color", col);
    }
    else
    {
        BadRef("@secondary_color", "set secondary color");
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
        BadArg("set_colors", "set vehicle colors", primary, secondary);
    }
    else
    {
        BadRef("set_colors", "set vehicle colors");
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetLocked() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleDoorsLocked(m_ID);
    }
    else
    {
        BadRef("@locked", "see whether is locked");
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
        BadRef("@locked", "set whether is locked");
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
        BadRef("get_part_status", "get part status");
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
        BadRef("set_part_status", "set part status");
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
        BadRef("get_tyre_status", "get tyre status");
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
        BadRef("set_tyre_status", "set tyre status");
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
        BadRef("@damage_data", "get damage data");
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
        BadRef("@damage_data", "get damage data");
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetAlarm() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleAlarm(m_ID);
    }
    else
    {
        BadRef("@alarm", "see whether has alarm");
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
        BadRef("@alarm", "set whether has alarm");
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetLights() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleLights(m_ID);
    }
    else
    {
        BadRef("@lights", "see whether has lights");
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
        BadRef("@lights", "set whether has lights");
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
        BadRef("@radio", "get radio");
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
        BadArg("@radio", "set radio", radio);
    }
    else
    {
        BadRef("@radio", "set radio");
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetRadioLocked() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->IsVehicleRadioLocked(m_ID);
    }
    else
    {
        BadRef("@radio_locked", "see whether radio is locked");
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
        BadRef("@radio_locked", "set whether radio is locked");
    }
}

// ------------------------------------------------------------------------------------------------
bool CVehicle::GetGhostState() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetVehicleGhostState(m_ID);
    }
    else
    {
        BadRef("@ghost_state", "see whether is in ghost state");
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
        BadRef("@ghost_state", "set whether is in ghost state");
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
        BadRef("reset_handling", "reset all handling rules");
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
        BadRef("reset_handling", "reset a handling rule");
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
        BadRef("exists_handling", "see whether handling rule exists");
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
        BadRef("get_handling_data", "get handling data");
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
        BadRef("set_handling_data", "set handling data");
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
        BadArg("embark", "embark player", _SCI32(player));
    }
    else
    {
        BadRef("embark", "embark player");
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
        BadArg("embark", "embark player", _SCI32(player));
    }
    else
    {
        BadRef("embark", "embark player");
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
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        /* Properties */
        .Prop(_SC("sync_source"), &CVehicle::GetSyncSource)
        .Prop(_SC("sync_type"), &CVehicle::GetSyncType)
        .Prop(_SC("world"), &CVehicle::GetWorld, &CVehicle::SetWorld)
        .Prop(_SC("model"), &CVehicle::GetModel)
        .Prop(_SC("model_id"), &CVehicle::GetModelID)
        .Prop(_SC("immunity"), &CVehicle::GetImmunity, &CVehicle::SetImmunity)
        .Prop(_SC("wrecked"), &CVehicle::IsWrecked)
        .Prop(_SC("pos"), &CVehicle::GetPosition, &CVehicle::SetPosition)
        .Prop(_SC("position"), &CVehicle::GetPosition, &CVehicle::SetPosition)
        .Prop(_SC("rot"), &CVehicle::GetRotation, &CVehicle::SetRotation)
        .Prop(_SC("rotation"), &CVehicle::GetRotation, &CVehicle::SetRotation)
        .Prop(_SC("erot"), &CVehicle::GetRotationEuler, &CVehicle::SetRotationEuler)
        .Prop(_SC("rot_euler"), &CVehicle::GetRotationEuler, &CVehicle::SetRotationEuler)
        .Prop(_SC("rotation_euler"), &CVehicle::GetRotationEuler, &CVehicle::SetRotationEuler)
        .Prop(_SC("speed"), &CVehicle::GetSpeed, &CVehicle::SetSpeed)
        .Prop(_SC("rel_speed"), &CVehicle::GetRelSpeed, &CVehicle::SetRelSpeed)
        .Prop(_SC("turn_speed"), &CVehicle::GetTurnSpeed, &CVehicle::SetTurnSpeed)
        .Prop(_SC("rel_turn_speed"), &CVehicle::GetRelTurnSpeed, &CVehicle::SetRelTurnSpeed)
        .Prop(_SC("spawn_pos"), &CVehicle::GetSpawnPosition, &CVehicle::SetSpawnPosition)
        .Prop(_SC("spawn_position"), &CVehicle::GetSpawnPosition, &CVehicle::SetSpawnPosition)
        .Prop(_SC("spawn_rot"), &CVehicle::GetSpawnRotation, &CVehicle::SetSpawnRotation)
        .Prop(_SC("spawn_rotation"), &CVehicle::GetSpawnRotation, &CVehicle::SetSpawnRotation)
        .Prop(_SC("spawn_erot"), &CVehicle::GetSpawnRotationEuler, &CVehicle::SetSpawnRotationEuler)
        .Prop(_SC("spawn_rot_euler"), &CVehicle::GetSpawnRotationEuler, &CVehicle::SetSpawnRotationEuler)
        .Prop(_SC("spawn_rotation_euler"), &CVehicle::GetSpawnRotationEuler, &CVehicle::SetSpawnRotationEuler)
        .Prop(_SC("respawn_timer"), &CVehicle::GetRespawnTimer, &CVehicle::SetRespawnTimer)
        .Prop(_SC("health"), &CVehicle::GetHealth, &CVehicle::SetHealth)
        .Prop(_SC("primary_color"), &CVehicle::GetPrimaryColor, &CVehicle::SetPrimaryColor)
        .Prop(_SC("secondary_color"), &CVehicle::GetSecondaryColor, &CVehicle::SetSecondaryColor)
        .Prop(_SC("locked"), &CVehicle::GetLocked, &CVehicle::SetLocked)
        .Prop(_SC("damage_data"), &CVehicle::GetDamageData, &CVehicle::SetDamageData)
        .Prop(_SC("alarm"), &CVehicle::GetAlarm, &CVehicle::SetAlarm)
        .Prop(_SC("lights"), &CVehicle::GetLights, &CVehicle::SetLights)
        .Prop(_SC("radio"), &CVehicle::GetRadio, &CVehicle::SetRadio)
        .Prop(_SC("radio_locked"), &CVehicle::GetRadioLocked, &CVehicle::SetRadioLocked)
        .Prop(_SC("ghost"), &CVehicle::GetGhostState, &CVehicle::SetGhostState)
        .Prop(_SC("ghost_state"), &CVehicle::GetGhostState, &CVehicle::SetGhostState)
        /* Functions */
        .Func(_SC("streamed_for"), &CVehicle::IsStreamedFor)
        .Func(_SC("occupant"), &CVehicle::GetOccupant)
        .Func(_SC("occupant_id"), &CVehicle::GetOccupantID)
        .Func(_SC("respawn"), &CVehicle::Respawn)
        .Func(_SC("set_rot"), &CVehicle::SetRotationEx)
        .Func(_SC("set_rotation"), &CVehicle::SetRotationEx)
        .Func(_SC("set_erot"), &CVehicle::SetRotationEulerEx)
        .Func(_SC("set_rot_euler"), &CVehicle::SetRotationEulerEx)
        .Func(_SC("set_rotation_euler"), &CVehicle::SetRotationEulerEx)
        .Func(_SC("set_speed"), &CVehicle::SetSpeedEx)
        .Func(_SC("set_rel_speed"), &CVehicle::SetRelSpeedEx)
        .Func(_SC("set_turn_speed"), &CVehicle::SetTurnSpeedEx)
        .Func(_SC("set_rel_turn_speed"), &CVehicle::SetRelTurnSpeedEx)
        .Func(_SC("set_spawn_pos"), &CVehicle::SetSpawnPositionEx)
        .Func(_SC("set_spawn_position"), &CVehicle::SetSpawnPositionEx)
        .Func(_SC("set_spawn_rot"), &CVehicle::SetSpawnRotationEx)
        .Func(_SC("set_spawn_rotation"), &CVehicle::SetSpawnRotationEx)
        .Func(_SC("set_spawn_erot"), &CVehicle::SetSpawnRotationEulerEx)
        .Func(_SC("set_spawn_rot_euler"), &CVehicle::SetSpawnRotationEulerEx)
        .Func(_SC("set_spawn_rotation_euler"), &CVehicle::SetSpawnRotationEulerEx)
        .Func(_SC("set_colors"), &CVehicle::SetColors)
        .Func(_SC("get_part_status"), &CVehicle::GetPartStatus)
        .Func(_SC("set_part_status"), &CVehicle::SetPartStatus)
        .Func(_SC("get_tyre_status"), &CVehicle::GetTyreStatus)
        .Func(_SC("set_tyre_status"), &CVehicle::SetTyreStatus)
        .Func(_SC("exists_handling"), &CVehicle::ExistsHandling)
        .Func(_SC("get_handling_data"), &CVehicle::GetHandlingData)
        .Func(_SC("set_handling_data"), &CVehicle::SetHandlingData)
        /* Overloads */
        .Overload< void (CVehicle::*)(const Vector3 &, bool) const >
            (_SC("set_pos"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("set_pos"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(SQFloat, SQFloat, SQFloat, bool) const >
            (_SC("set_pos"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(const Vector3 &, bool) const >
            (_SC("set_position"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("set_position"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(SQFloat, SQFloat, SQFloat, bool) const >
            (_SC("set_position"), &CVehicle::SetPositionEx)
        .Overload< void (CVehicle::*)(const Vector3 &) const >
            (_SC("add_speed"), &CVehicle::AddSpeed)
        .Overload< void (CVehicle::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("add_speed"), &CVehicle::AddSpeedEx)
        .Overload< void (CVehicle::*)(const Vector3 &) const >
            (_SC("add_rel_speed"), &CVehicle::AddRelSpeed)
        .Overload< void (CVehicle::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("add_rel_speed"), &CVehicle::AddRelSpeedEx)
        .Overload< void (CVehicle::*)(const Vector3 &) const >
            (_SC("add_turn_speed"), &CVehicle::AddTurnSpeed)
        .Overload< void (CVehicle::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("add_turn_speed"), &CVehicle::AddTurnSpeedEx)
        .Overload< void (CVehicle::*)(const Vector3 &) const >
            (_SC("add_rel_turn_speed"), &CVehicle::AddRelTurnSpeed)
        .Overload< void (CVehicle::*)(SQFloat, SQFloat, SQFloat) const >
            (_SC("add_rel_turn_speed"), &CVehicle::AddRelTurnSpeedEx)
        .Overload< void (CVehicle::*)(void) const >
            (_SC("reset_handling"), &CVehicle::ResetHandling)
        .Overload< void (CVehicle::*)(SQInt32) const >
            (_SC("reset_handling"), &CVehicle::ResetHandling)
        .Overload< void (CVehicle::*)(const Reference< CPlayer > &) const >
            (_SC("embark"), &CVehicle::Embark)
        .Overload< void (CVehicle::*)(const Reference< CPlayer > &, SQInt32, bool, bool) const >
            (_SC("embark"), &CVehicle::Embark)
    );
    // Output debugging information
    LogDbg("Registration of <CVehicle> type was successful");
    // Output debugging information
    LogDbg("Beginning registration of <Vehicle> functions");
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
    LogDbg("Registration of <Vehicle> functions was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
