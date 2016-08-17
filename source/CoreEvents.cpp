// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"
#include "Library/Utils/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void Core::EmitCustomEvent(Int32 group, Int32 header, Object & payload)
{
    Emit(mOnCustomEvent, group, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitBlipCreated(Int32 blip, Int32 header, Object & payload)
{
    Emit(mOnBlipCreated, m_Blips.at(blip).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointCreated(Int32 checkpoint, Int32 header, Object & payload)
{
    Emit(mOnCheckpointCreated, m_Checkpoints.at(checkpoint).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitKeybindCreated(Int32 keybind, Int32 header, Object & payload)
{
    Emit(mOnKeybindCreated, m_Keybinds.at(keybind).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectCreated(Int32 object, Int32 header, Object & payload)
{
    Emit(mOnObjectCreated, m_Objects.at(object).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupCreated(Int32 pickup, Int32 header, Object & payload)
{
    Emit(mOnPickupCreated, m_Pickups.at(pickup).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCreated(Int32 player, Int32 header, Object & payload)
{
    Emit(mOnPlayerCreated, m_Players.at(player).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleCreated(Int32 vehicle, Int32 header, Object & payload)
{
    Emit(mOnVehicleCreated, m_Vehicles.at(vehicle).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitBlipDestroyed(Int32 blip, Int32 header, Object & payload)
{
    BlipInst & _blip = m_Blips.at(blip);
    Emit(_blip.mOnDestroyed, header, payload);
    Emit(mOnBlipDestroyed, _blip.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointDestroyed(Int32 checkpoint, Int32 header, Object & payload)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint);
    Emit(_checkpoint.mOnDestroyed, header, payload);
    Emit(mOnCheckpointDestroyed, _checkpoint.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitKeybindDestroyed(Int32 keybind, Int32 header, Object & payload)
{
    KeybindInst & _keybind = m_Keybinds.at(keybind);
    Emit(_keybind.mOnDestroyed, header, payload);
    Emit(mOnKeybindDestroyed, _keybind.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectDestroyed(Int32 object, Int32 header, Object & payload)
{
    ObjectInst & _object = m_Objects.at(object);
    Emit(_object.mOnDestroyed, header, payload);
    Emit(mOnObjectDestroyed, _object.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupDestroyed(Int32 pickup, Int32 header, Object & payload)
{
    PickupInst & _pickup = m_Pickups.at(pickup);
    Emit(_pickup.mOnDestroyed, header, payload);
    Emit(mOnPickupDestroyed, _pickup.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerDestroyed(Int32 player, Int32 header, Object & payload)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnDestroyed, header, payload);
    Emit(mOnPlayerDestroyed, _player.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleDestroyed(Int32 vehicle, Int32 header, Object & payload)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    Emit(_vehicle.mOnDestroyed, header, payload);
    Emit(mOnVehicleDestroyed, _vehicle.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitBlipCustom(Int32 blip, Int32 header, Object & payload)
{
    BlipInst & _blip = m_Blips.at(blip);
    Emit(_blip.mOnCustom, header, payload);
    Emit(mOnBlipCustom, _blip.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointCustom(Int32 checkpoint, Int32 header, Object & payload)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint);
    Emit(_checkpoint.mOnCustom, header, payload);
    Emit(mOnCheckpointCustom, _checkpoint.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitKeybindCustom(Int32 keybind, Int32 header, Object & payload)
{
    KeybindInst & _keybind = m_Keybinds.at(keybind);
    Emit(_keybind.mOnCustom, header, payload);
    Emit(mOnKeybindCustom, _keybind.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectCustom(Int32 object, Int32 header, Object & payload)
{
    ObjectInst & _object = m_Objects.at(object);
    Emit(_object.mOnCustom, header, payload);
    Emit(mOnObjectCustom, _object.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupCustom(Int32 pickup, Int32 header, Object & payload)
{
    PickupInst & _pickup = m_Pickups.at(pickup);
    Emit(_pickup.mOnCustom, header, payload);
    Emit(mOnPickupCustom, _pickup.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCustom(Int32 player, Int32 header, Object & payload)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnCustom, header, payload);
    Emit(mOnPlayerCustom, _player.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleCustom(Int32 vehicle, Int32 header, Object & payload)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    Emit(_vehicle.mOnCustom, header, payload);
    Emit(mOnVehicleCustom, _vehicle.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerStartup()
{
    Emit(mOnServerStartup);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerShutdown()
{
    Emit(mOnServerShutdown);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerFrame(Float32 elapsed_time)
{
    Emit(mOnServerFrame, elapsed_time);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPluginCommand(Uint32 /*command_identifier*/, CCStr /*message*/)
{
    // Ignored for now...
}

// ------------------------------------------------------------------------------------------------
void Core::EmitIncomingConnection(CStr player_name, size_t name_buffer_size, CCStr user_password, CCStr ip_address)
{
    // Save the buffer information so that we can write to it from the string
    m_IncomingNameBuffer = player_name;
    m_IncomingNameCapacity = name_buffer_size;
    // Attempt to forward the event to the script callback
    try
    {
        Emit(mOnIncomingConnection, player_name, name_buffer_size, user_password, ip_address);
    }
    catch (...)
    {
        // We catch the exception so we can release the assigned buffer
    }
    // Release any stored buffer information
    m_IncomingNameBuffer = nullptr;
    m_IncomingNameCapacity = 0;
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerRequestClass(Int32 player_id, Int32 offset)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnRequestClass, offset);
    Emit(mOnPlayerRequestClass, _player.mObj, offset);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerRequestSpawn(Int32 player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnRequestSpawn);
    Emit(mOnPlayerRequestSpawn, _player.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerSpawn(Int32 player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnSpawn);
    Emit(mOnPlayerSpawn, _player.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerWasted(Int32 player_id, Int32 reason)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnWasted, reason);
    Emit(mOnPlayerWasted, _player.mObj, reason);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerKilled(Int32 player_id, Int32 killer_id, Int32 reason, vcmpBodyPart body_part, bool team_kill)
{
    PlayerInst & _player = m_Players.at(player_id);
    PlayerInst & _killer = m_Players.at(killer_id);
    Emit(_player.mOnKilled, _killer.mObj, reason, static_cast< Int32 >(body_part), team_kill);
    Emit(mOnPlayerKilled, _player.mObj, _killer.mObj, reason, static_cast< Int32 >(body_part), team_kill);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerEmbarking(Int32 player_id, Int32 vehicle_id, Int32 slot_index)
{
    PlayerInst & _player = m_Players.at(player_id);
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    Emit(_player.mOnEmbarking, _vehicle.mObj, slot_index);
    Emit(_vehicle.mOnEmbarking, _player.mObj, slot_index);
    Emit(mOnPlayerEmbarking, _player.mObj, _vehicle.mObj, slot_index);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerEmbarked(Int32 player_id, Int32 vehicle_id, Int32 slot_index)
{
    PlayerInst & _player = m_Players.at(player_id);
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    Emit(_player.mOnEmbarked, _vehicle.mObj, slot_index);
    Emit(_vehicle.mOnEmbarked, _player.mObj, slot_index);
    Emit(mOnPlayerEmbarked, _player.mObj, _vehicle.mObj, slot_index);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerDisembark(Int32 player_id, Int32 vehicle_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    Emit(_player.mOnDisembark, _vehicle.mObj);
    Emit(_vehicle.mOnDisembark, _player.mObj);
    Emit(mOnPlayerDisembark, _player.mObj, _vehicle.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerRename(Int32 player_id, CCStr old_name, CCStr new_name)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnRename, old_name, new_name);
    Emit(mOnPlayerRename, _player.mObj, old_name, new_name);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerState(Int32 player_id, Int32 old_state, Int32 new_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnState, old_state, new_state);
    Emit(mOnPlayerState, _player.mObj, old_state, new_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateNone(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnStateNone, old_state);
    Emit(mOnStateNone, _player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateNormal(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnStateNormal, old_state);
    Emit(mOnStateNormal, _player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateAim(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnStateAim, old_state);
    Emit(mOnStateAim, _player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateDriver(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnStateDriver, old_state);
    Emit(mOnStateDriver, _player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStatePassenger(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnStatePassenger, old_state);
    Emit(mOnStatePassenger, _player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateEnterDriver(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnStateEnterDriver, old_state);
    Emit(mOnStateEnterDriver, _player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateEnterPassenger(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnStateEnterPassenger, old_state);
    Emit(mOnStateEnterPassenger, _player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateExit(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnStateExit, old_state);
    Emit(mOnStateExit, _player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateUnspawned(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnStateUnspawned, old_state);
    Emit(mOnStateUnspawned, _player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerAction(Int32 player_id, Int32 old_action, Int32 new_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnAction, old_action, new_action);
    Emit(mOnPlayerAction, _player.mObj, old_action, new_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionNone(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionNone, old_action);
    Emit(mOnActionNone, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionNormal(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionNormal, old_action);
    Emit(mOnActionNormal, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionAiming(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionAiming, old_action);
    Emit(mOnActionAiming, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionShooting(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionShooting, old_action);
    Emit(mOnActionShooting, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionJumping(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionJumping, old_action);
    Emit(mOnActionJumping, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionLieDown(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionLieDown, old_action);
    Emit(mOnActionLieDown, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionGettingUp(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionGettingUp, old_action);
    Emit(mOnActionGettingUp, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionJumpVehicle(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionJumpVehicle, old_action);
    Emit(mOnActionJumpVehicle, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionDriving(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionDriving, old_action);
    Emit(mOnActionDriving, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionDying(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionDying, old_action);
    Emit(mOnActionDying, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionWasted(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionWasted, old_action);
    Emit(mOnActionWasted, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionEmbarking(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionEmbarking, old_action);
    Emit(mOnActionEmbarking, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionDisembarking(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnActionDisembarking, old_action);
    Emit(mOnActionDisembarking, _player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerBurning(Int32 player_id, bool is_on_fire)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnBurning, is_on_fire);
    Emit(mOnPlayerBurning, _player.mObj, is_on_fire);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCrouching(Int32 player_id, bool is_crouching)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnCrouching, is_crouching);
    Emit(mOnPlayerCrouching, _player.mObj, is_crouching);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerGameKeys(Int32 player_id, Uint32 old_keys, Uint32 new_keys)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnGameKeys, old_keys, new_keys);
    Emit(mOnPlayerGameKeys, _player.mObj, old_keys, new_keys);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerStartTyping(Int32 player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnStartTyping);
    Emit(mOnPlayerStartTyping, _player.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerStopTyping(Int32 player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnStopTyping);
    Emit(mOnPlayerStopTyping, _player.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerAway(Int32 player_id, bool is_away)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnAway, is_away);
    Emit(mOnPlayerAway, _player.mObj, is_away);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerMessage(Int32 player_id, CCStr message)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnMessage, message);
    Emit(mOnPlayerMessage, _player.mObj, message);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCommand(Int32 player_id, CCStr message)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnCommand, message);
    Emit(mOnPlayerCommand, _player.mObj, message);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerPrivateMessage(Int32 player_id, Int32 target_player_id, CCStr message)
{
    PlayerInst & _player = m_Players.at(player_id);
    PlayerInst & _receiver = m_Players.at(target_player_id);
    Emit(_player.mOnMessage,_receiver.mObj,  message);
    Emit(mOnPlayerPrivateMessage, _player.mObj, _receiver.mObj, message);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerKeyPress(Int32 player_id, Int32 bind_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    KeybindInst & _keybind = m_Keybinds.at(bind_id);
    Emit(_player.mOnKeyPress, _keybind.mObj);
    Emit(_keybind.mOnKeyPress, _player.mObj);
    Emit(mOnPlayerKeyPress, _player.mObj, _keybind.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerKeyRelease(Int32 player_id, Int32 bind_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    KeybindInst & _keybind = m_Keybinds.at(bind_id);
    Emit(_keybind.mOnKeyRelease, _player.mObj);
    Emit(_player.mOnKeyRelease, _keybind.mObj);
    Emit(mOnPlayerKeyRelease, _player.mObj, _keybind.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerSpectate(Int32 player_id, Int32 target_player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    PlayerInst & _target = m_Players.at(target_player_id);
    Emit(_player.mOnSpectate, _target.mObj);
    Emit(mOnPlayerSpectate, _player.mObj, _target.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCrashreport(Int32 player_id, CCStr report)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnCrashreport, report);
    Emit(mOnPlayerCrashreport, _player.mObj, report);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleExplode(Int32 vehicle_id)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    Emit(_vehicle.mOnExplode);
    Emit(mOnVehicleExplode, _vehicle.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleRespawn(Int32 vehicle_id)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    Emit(_vehicle.mOnRespawn);
    Emit(mOnVehicleRespawn, _vehicle.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectShot(Int32 object_id, Int32 player_id, Int32 weapon_id)
{
    ObjectInst & _object = m_Objects.at(object_id);
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_object.mOnShot, _player.mObj, weapon_id);
    Emit(_player.mOnObjectShot, _object.mObj, weapon_id);
    Emit(mOnObjectShot, _player.mObj, _object.mObj, weapon_id);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectTouched(Int32 object_id, Int32 player_id)
{
    ObjectInst & _object = m_Objects.at(object_id);
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_object.mOnTouched, _player.mObj);
    Emit(_player.mOnObjectTouched, _object.mObj);
    Emit(mOnObjectTouched, _player.mObj, _object.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupClaimed(Int32 pickup_id, Int32 player_id)
{
    PickupInst & _pickup = m_Pickups.at(pickup_id);
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_pickup.mOnClaimed, _player.mObj);
    Emit(_player.mOnPickupClaimed, _pickup.mObj);
    Emit(mOnPickupClaimed, _player.mObj, _pickup.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupCollected(Int32 pickup_id, Int32 player_id)
{
    PickupInst & _pickup = m_Pickups.at(pickup_id);
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_pickup.mOnCollected, _player.mObj);
    Emit(_player.mOnPickupCollected, _pickup.mObj);
    Emit(mOnPickupCollected, _player.mObj, _pickup.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupRespawn(Int32 pickup_id)
{
    PickupInst & _pickup = m_Pickups.at(pickup_id);
    Emit(_pickup.mOnRespawn);
    Emit(mOnPickupRespawn, _pickup.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointEntered(Int32 checkpoint_id, Int32 player_id)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint_id);
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_checkpoint.mOnEntered, _player.mObj);
    Emit(_player.mOnCheckpointEntered, _checkpoint.mObj);
    Emit(mOnCheckpointEntered, _player.mObj, _checkpoint.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointExited(Int32 checkpoint_id, Int32 player_id)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint_id);
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_checkpoint.mOnExited, _player.mObj);
    Emit(_player.mOnCheckpointExited, _checkpoint.mObj);
    Emit(mOnCheckpointExited, _player.mObj, _checkpoint.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerHealth(Int32 player_id, Float32 old_health, Float32 new_health)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnHealth, old_health, new_health);
    Emit(mOnPlayerHealth, _player.mObj, old_health, new_health);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerArmour(Int32 player_id, Float32 old_armour, Float32 new_armour)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnArmour, old_armour, new_armour);
    Emit(mOnPlayerArmour, _player.mObj, old_armour, new_armour);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerWeapon(Int32 player_id, Int32 old_weapon, Int32 new_weapon)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnWeapon, old_weapon, new_weapon);
    Emit(mOnPlayerWeapon, _player.mObj, old_weapon, new_weapon);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerHeading(Int32 player_id, Float32 old_heading, Float32 new_heading)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnHeading, old_heading, new_heading);
    Emit(mOnPlayerHeading, _player.mObj, old_heading, new_heading);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerPosition(Int32 player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnPosition, _player.mTrackPositionHeader, _player.mTrackPositionPayload);
    Emit(mOnPlayerPosition, _player.mObj, _player.mTrackPositionHeader, _player.mTrackPositionPayload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerOption(Int32 player_id, Int32 option_id, bool value,
                                Int32 header, Object & payload)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnOption, option_id, value, header, payload);
    Emit(mOnPlayerOption, _player.mObj, option_id, value, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerAdmin(Int32 player_id, bool old_admin, bool new_admin)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnAdmin, old_admin, new_admin);
    Emit(mOnPlayerAdmin, _player.mObj, old_admin, new_admin);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerWorld(Int32 player_id, Int32 old_world, Int32 new_world, bool secondary)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnWorld, old_world, new_world, secondary);
    Emit(mOnPlayerWorld, _player.mObj, old_world, new_world, secondary);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerTeam(Int32 player_id, Int32 old_team, Int32 new_team)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnTeam, old_team, new_team);
    Emit(mOnPlayerTeam, _player.mObj, old_team, new_team);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerSkin(Int32 player_id, Int32 old_skin, Int32 new_skin)
{
    PlayerInst & _player = m_Players.at(player_id);
    Emit(_player.mOnSkin, old_skin, new_skin);
    Emit(mOnPlayerSkin, _player.mObj, old_skin, new_skin);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleColour(Int32 vehicle_id, Int32 changed)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    Emit(_vehicle.mOnColour, changed);
    Emit(mOnVehicleColour, _vehicle.mObj, changed);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleHealth(Int32 vehicle_id, Float32 old_health, Float32 new_health)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    Emit(_vehicle.mOnHealth, old_health, new_health);
    Emit(mOnVehicleHealth, _vehicle.mObj, old_health, new_health);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehiclePosition(Int32 vehicle_id)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    Emit(_vehicle.mOnPosition);
    Emit(mOnVehiclePosition, _vehicle.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleRotation(Int32 vehicle_id)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    Emit(_vehicle.mOnRotation);
    Emit(mOnVehicleRotation, _vehicle.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleOption(Int32 vehicle_id, Int32 option_id, bool value,
                                Int32 header, Object & payload)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    Emit(_vehicle.mOnOption, option_id, value, header, payload);
    Emit(mOnVehicleOption, _vehicle.mObj, option_id, value, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerOption(Int32 option, bool value, Int32 header, Object & payload)
{
    if (m_CircularLocks & CCL_EMIT_SERVER_OPTION)
    {
        return; // Already inside this event!
    }
    // Prevent further calls to this event
    BitGuardU32 bg(m_CircularLocks, CCL_EMIT_SERVER_OPTION);
    // Now forward the event call
    Emit(mOnServerOption, option, value, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitScriptReload(Int32 header, Object & payload)
{
    Emit(mOnScriptReload, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitScriptLoaded()
{
    Emit(mOnScriptLoaded);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitEntityPool(vcmpEntityPool entity_type, Int32 entity_id, bool is_deleted)
{
    // See what type of change happened in the entity pool
    switch (entity_type)
    {
        case vcmpEntityPoolVehicle:
            // Do we even have this vehicle that we're trying to delete?
            if (is_deleted && VALID_ENTITY(m_Vehicles[entity_id].mID))
            {
                DeallocVehicle(entity_id, false, SQMOD_DESTROY_POOL, NullObject());
            }
            // Do we already have this vehicle that we're trying to create?
            else if (INVALID_ENTITY(m_Vehicles[entity_id].mID))
            {
                AllocVehicle(entity_id, false, SQMOD_CREATE_POOL, NullObject());
            }
        break;
        case vcmpEntityPoolObject:
            // Do we even have this object that we're trying to delete?
            if (is_deleted && VALID_ENTITY(m_Objects[entity_id].mID))
            {
                DeallocObject(entity_id, false, SQMOD_DESTROY_POOL, NullObject());
            }
            // Do we already have this object that we're trying to create?
            else if (INVALID_ENTITY(m_Objects[entity_id].mID))
            {
                AllocObject(entity_id, false, SQMOD_CREATE_POOL, NullObject());
            }
        break;
        case vcmpEntityPoolPickup:
            // Do we even have this pickup that we're trying to delete?
            if (is_deleted && VALID_ENTITY(m_Pickups[entity_id].mID))
            {
                DeallocPickup(entity_id, false, SQMOD_DESTROY_POOL, NullObject());
            }
            // Do we already have this pickup that we're trying to create?
            else if (INVALID_ENTITY(m_Pickups[entity_id].mID))
            {
                AllocPickup(entity_id, false, SQMOD_CREATE_POOL, NullObject());
            }
        break;
        case vcmpEntityPoolRadio:
            // @TODO Implement...
        break;
        case vcmpEntityPoolCheckPoint:
            // Do we even have this checkpoint that we're trying to delete?
            if (is_deleted && VALID_ENTITY(m_Checkpoints[entity_id].mID))
            {
                DeallocCheckpoint(entity_id, false, SQMOD_DESTROY_POOL, NullObject());
            }
            // Do we already have this checkpoint that we're trying to create?
            else if (INVALID_ENTITY(m_Checkpoints[entity_id].mID))
            {
                AllocCheckpoint(entity_id, false, SQMOD_CREATE_POOL, NullObject());
            }
        break;
        case vcmpEntityPoolBlip:
            // Do we even have this blip that we're trying to delete?
            if (is_deleted && VALID_ENTITY(m_Blips[entity_id].mID))
            {
                DeallocBlip(entity_id, false, SQMOD_DESTROY_POOL, NullObject());
            }
            // Do we already have this blip that we're trying to create?
            else if (INVALID_ENTITY(m_Blips[entity_id].mID))
            {
                // Make sure that the specified entity identifier is valid
                if (INVALID_ENTITYEX(entity_id, SQMOD_BLIP_POOL))
                {
                    STHROWF("Cannot allocate blip with invalid identifier: %d", entity_id);
                }
                // Retrieve the specified entity instance
                BlipInst & inst = m_Blips[entity_id];
                // Make sure that the instance isn't already allocated
                if (VALID_ENTITY(inst.mID))
                {
                    STHROWF("Cannot allocate blip that already exists: %d", entity_id);
                }
                // Information about the blip entity
                Int32 world, scale, sprid;
                Uint32 color;
                Float32 x, y, z;
                // Get the blip information from the server
                _Func->GetCoordBlipInfo(entity_id, &world, &x, &y, &z, &scale, &color, &sprid);
                // Assign the obtain information
                inst.mWorld = world;
                inst.mScale = scale;
                inst.mSprID = sprid;
                inst.mColor.SetRGBA(color);
                inst.mPosition.SetVector3Ex(x, y, z);
                // Now we can try to allocate the instance after we have all the information
                AllocBlip(entity_id, false, SQMOD_CREATE_POOL, NullObject());
            }
        break;
        default:
            LogErr("Unknown change in the entity pool of type: %d", entity_type);
    }
    // Finally, forward the event to the script
    Emit(mOnEntityPool, static_cast< Int32 >(entity_type), entity_id, is_deleted);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerUpdate(Int32 player_id, vcmpPlayerUpdate update_type)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(player_id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Cannot update player with invalid identifier: %d", player_id);
    }
    // Retrieve the associated tracking instance
    PlayerInst & inst = m_Players[player_id];

    // Obtain the current heading of this instance
    Float32 heading = _Func->GetPlayerHeading(player_id);
    // Did the heading change since the last tracked value?
    if (!EpsEq(heading, inst.mLastHeading))
    {
        // Trigger the event specific to this change
        if (inst.mTrackHeading != 0)
        {
            // Should we decrease the tracked position changes?
            if (inst.mTrackHeading)
            {
                --inst.mTrackHeading;
            }
            // Now emit the event
            EmitPlayerHeading(player_id, inst.mLastHeading, heading);
        }
        // Update the tracked value
        inst.mLastHeading = heading;
    }

    Vector3 pos;
    // Obtain the current position of this instance
    _Func->GetPlayerPosition(player_id, &pos.x, &pos.y, &pos.z);
    // Did the position change since the last tracked value?
    if (pos != inst.mLastPosition)
    {
        // Trigger the event specific to this change
        if (inst.mTrackPosition != 0)
        {
            // Should we decrease the tracked position changes?
            if (inst.mTrackPosition)
            {
                --inst.mTrackPosition;
            }
            // Now emit the event
            EmitPlayerPosition(player_id);
        }
        // Update the tracked value
        inst.mLastPosition = pos;
    }

    // Obtain the current health of this instance
    Float32 health = _Func->GetPlayerHealth(player_id);
    // Did the health change since the last tracked value?
    if (!EpsEq(health, inst.mLastHealth))
    {
        // Trigger the event specific to this change
        EmitPlayerHealth(player_id, inst.mLastHealth, health);
        // Update the tracked value
        inst.mLastHealth = health;
    }

    // Obtain the current armor of this instance
    Float32 armour = _Func->GetPlayerArmour(player_id);
    // Did the armor change since the last tracked value?
    if (!EpsEq(armour, inst.mLastArmour))
    {
        // Trigger the event specific to this change
        EmitPlayerArmour(player_id, inst.mLastArmour, armour);
        // Update the tracked value
        inst.mLastArmour = armour;
    }

    // Obtain the current weapon of this instance
    Int32 wep = _Func->GetPlayerWeapon(player_id);
    // Did the weapon change since the last tracked value?
    if (wep != inst.mLastWeapon)
    {
        // Trigger the event specific to this change
        EmitPlayerWeapon(player_id, inst.mLastWeapon, wep);
        // Update the tracked value
        inst.mLastWeapon = wep;
    }

    // Finally, forward the call to the update callback
    Emit(inst.mOnUpdate, static_cast< Int32 >(update_type));
    Emit(mOnPlayerUpdate, inst.mObj, static_cast< Int32 >(update_type));
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleUpdate(Int32 vehicle_id, vcmpVehicleUpdate update_type)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(vehicle_id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Cannot update vehicle with invalid identifier: %d", vehicle_id);
    }
    // Retrieve the associated instance
    VehicleInst & inst = m_Vehicles[vehicle_id];
    // Identify the update type
    switch (update_type)
    {
        case vcmpVehicleUpdatePosition:
        {
            // Trigger the event specific to this change
            if (inst.mTrackPosition != 0)
            {
                // Should we decrease the tracked position changes?
                if (inst.mTrackPosition)
                {
                    --inst.mTrackPosition;
                }
                // Now emit the event
                EmitVehiclePosition(vehicle_id);
            }
            // Update the tracked value
            _Func->GetVehiclePosition(vehicle_id, &inst.mLastPosition.x,
                                        &inst.mLastPosition.y, &inst.mLastPosition.z);
        } break;
        case vcmpVehicleUpdateHealth:
        {
            // Obtain the current health of this instance
            Float32 health = _Func->GetVehicleHealth(vehicle_id);
            // Trigger the event specific to this change
            EmitVehicleHealth(vehicle_id, inst.mLastHealth, health);
            // Update the tracked value
            inst.mLastHealth = health;
        } break;
        case vcmpVehicleUpdateColour:
        {
            Int32 primary, secondary;
            // Obtain the current colours of this instance
            _Func->GetVehicleColour(vehicle_id, &primary, &secondary);
            // Which colours changed
            Int32 changed = 0;
            // Did the primary colour changed?
            if (primary != inst.mLastPrimaryColour)
            {
                changed |= (1<<0);
            }
            // Did the secondary colour changed?
            if (primary != inst.mLastSecondaryColour)
            {
                changed |= (1<<1);
            }
            // Trigger the event specific to this change
            EmitVehicleColour(vehicle_id, changed);
            // Update the tracked value
            inst.mLastPrimaryColour = primary;
            inst.mLastSecondaryColour = secondary;
        } break;
        case vcmpVehicleUpdateRotation:
        {
            // Trigger the event specific to this change
            if (inst.mTrackRotation != 0)
            {
                // Should we decrease the tracked rotation changes?
                if (inst.mTrackRotation)
                {
                    --inst.mTrackRotation;
                }
                // Now emit the event
                EmitVehicleRotation(vehicle_id);
            }
            // Obtain the current rotation of this instance
            _Func->GetVehicleRotation(vehicle_id, &inst.mLastRotation.x, &inst.mLastRotation.y,
                                                    &inst.mLastRotation.z, &inst.mLastRotation.w);
        } break;
        default:
        {
            // Finally, forward the call to the update callback
            Emit(inst.mOnUpdate, static_cast< Int32 >(update_type));
            Emit(mOnVehicleUpdate, inst.mObj, static_cast< Int32 >(update_type));
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::EmitClientScriptData(Int32 player_id, const uint8_t * data, size_t size)
{
    PlayerInst & _player = m_Players.at(player_id);
    // Don't even bother if there's no one listening
    if (_player.mOnClientScriptData.IsNull() && mOnClientScriptData.IsNull())
    {
        return;
    }
    // Allocate a buffer with the received size
    Buffer b(size);
    // Replicate the data to the allocated buffer
    b.Write(0, reinterpret_cast< Buffer::ConstPtr >(data), size);
    // Prepare an object for the obtained buffer
    Object o;
    // Attempt to create the requested buffer
    try
    {
        // Remember the current stack size
        const StackGuard sg;
        // Create a protected instance of a buffer wrapper
        AutoDelete< SqBuffer > ad(new SqBuffer(std::move(b)));
        // Transform the pointer into a script object
        PushVar< SqBuffer * >(DefaultVM::Get(), ad.Get());
        // The script took over the instance now
        ad.Release();
        // Get the object from the stack and store it
        o = Var< Object >(DefaultVM::Get(), -1).value;
    }
    catch (const std::exception & e)
    {
        STHROWF("%s", e.what()); // Re-package
    }
    // Make sure the buffer cannot be null at this point
    if (o.IsNull())
    {
        STHROWF("Unable to transform script data into buffer");
    }
    // Forward the event call
    Emit(_player.mOnClientScriptData, o, size);
    Emit(mOnClientScriptData, _player.mObj, o, size);
}

} // Namespace:: SqMod
