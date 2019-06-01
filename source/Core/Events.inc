// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void Core::EmitCustomEvent(Int32 group, Int32 header, LightObj & payload)
{
    (*mOnCustomEvent.first)(group, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitBlipCreated(Int32 blip, Int32 header, LightObj & payload)
{
    (*mOnBlipCreated.first)(m_Blips.at(blip).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointCreated(Int32 checkpoint, Int32 header, LightObj & payload)
{
    (*mOnCheckpointCreated.first)(m_Checkpoints.at(checkpoint).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitKeybindCreated(Int32 keybind, Int32 header, LightObj & payload)
{
    (*mOnKeybindCreated.first)(m_Keybinds.at(keybind).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectCreated(Int32 object, Int32 header, LightObj & payload)
{
    (*mOnObjectCreated.first)(m_Objects.at(object).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupCreated(Int32 pickup, Int32 header, LightObj & payload)
{
    (*mOnPickupCreated.first)(m_Pickups.at(pickup).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCreated(Int32 player, Int32 header, LightObj & payload)
{
    (*mOnPlayerCreated.first)(m_Players.at(player).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleCreated(Int32 vehicle, Int32 header, LightObj & payload)
{
    (*mOnVehicleCreated.first)(m_Vehicles.at(vehicle).mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitBlipDestroyed(Int32 blip, Int32 header, LightObj & payload)
{
    BlipInst & _blip = m_Blips.at(blip);
    (*_blip.mOnDestroyed.first)(header, payload);
    (*mOnBlipDestroyed.first)(_blip.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointDestroyed(Int32 checkpoint, Int32 header, LightObj & payload)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint);
    (*_checkpoint.mOnDestroyed.first)(header, payload);
    (*mOnCheckpointDestroyed.first)(_checkpoint.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitKeybindDestroyed(Int32 keybind, Int32 header, LightObj & payload)
{
    KeybindInst & _keybind = m_Keybinds.at(keybind);
    (*_keybind.mOnDestroyed.first)(header, payload);
    (*mOnKeybindDestroyed.first)(_keybind.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectDestroyed(Int32 object, Int32 header, LightObj & payload)
{
    ObjectInst & _object = m_Objects.at(object);
    (*_object.mOnDestroyed.first)(header, payload);
    (*mOnObjectDestroyed.first)(_object.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupDestroyed(Int32 pickup, Int32 header, LightObj & payload)
{
    PickupInst & _pickup = m_Pickups.at(pickup);
    (*_pickup.mOnDestroyed.first)(header, payload);
    (*mOnPickupDestroyed.first)(_pickup.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerDestroyed(Int32 player, Int32 header, LightObj & payload)
{
    PlayerInst & _player = m_Players.at(player);
    (*_player.mOnDestroyed.first)(header, payload);
    (*mOnPlayerDestroyed.first)(_player.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleDestroyed(Int32 vehicle, Int32 header, LightObj & payload)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    (*_vehicle.mOnDestroyed.first)(header, payload);
    (*mOnVehicleDestroyed.first)(_vehicle.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitBlipCustom(Int32 blip, Int32 header, LightObj & payload)
{
    BlipInst & _blip = m_Blips.at(blip);
    (*_blip.mOnCustom.first)(header, payload);
    (*mOnBlipCustom.first)(_blip.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointCustom(Int32 checkpoint, Int32 header, LightObj & payload)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint);
    (*_checkpoint.mOnCustom.first)(header, payload);
    (*mOnCheckpointCustom.first)(_checkpoint.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitKeybindCustom(Int32 keybind, Int32 header, LightObj & payload)
{
    KeybindInst & _keybind = m_Keybinds.at(keybind);
    (*_keybind.mOnCustom.first)(header, payload);
    (*mOnKeybindCustom.first)(_keybind.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectCustom(Int32 object, Int32 header, LightObj & payload)
{
    ObjectInst & _object = m_Objects.at(object);
    (*_object.mOnCustom.first)(header, payload);
    (*mOnObjectCustom.first)(_object.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupCustom(Int32 pickup, Int32 header, LightObj & payload)
{
    PickupInst & _pickup = m_Pickups.at(pickup);
    (*_pickup.mOnCustom.first)(header, payload);
    (*mOnPickupCustom.first)(_pickup.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCustom(Int32 player, Int32 header, LightObj & payload)
{
    PlayerInst & _player = m_Players.at(player);
    (*_player.mOnCustom.first)(header, payload);
    (*mOnPlayerCustom.first)(_player.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleCustom(Int32 vehicle, Int32 header, LightObj & payload)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    (*_vehicle.mOnCustom.first)(header, payload);
    (*mOnVehicleCustom.first)(_vehicle.mObj, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerStartup()
{
    (*mOnServerStartup.first)();
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerShutdown()
{
    (*mOnServerShutdown.first)();
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerFrame(Float32 elapsed_time)
{
    (*mOnServerFrame.first)(elapsed_time);
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
        (*mOnIncomingConnection.first)(LightObj(player_name, -1), name_buffer_size,
                                        LightObj(user_password, -1), LightObj(ip_address, -1));
    }
    catch (...)
    {
        // Release any stored buffer information
        m_IncomingNameBuffer = nullptr;
        m_IncomingNameCapacity = 0;
        // We catched the exception so we can release the assigned buffer
        throw; // re-throw it
    }
    // Release any stored buffer information
    m_IncomingNameBuffer = nullptr;
    m_IncomingNameCapacity = 0;
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerRequestClass(Int32 player_id, Int32 offset)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnRequestClass.first)(offset);
    (*mOnPlayerRequestClass.first)(_player.mObj, offset);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerRequestSpawn(Int32 player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnRequestSpawn.first)();
    (*mOnPlayerRequestSpawn.first)(_player.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerSpawn(Int32 player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnSpawn.first)();
    (*mOnPlayerSpawn.first)(_player.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerWasted(Int32 player_id, Int32 reason)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnWasted.first)(reason);
    (*mOnPlayerWasted.first)(_player.mObj, reason);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerKilled(Int32 player_id, Int32 killer_id, Int32 reason, vcmpBodyPart body_part, bool team_kill)
{
    PlayerInst & _player = m_Players.at(player_id);
    PlayerInst & _killer = m_Players.at(killer_id);
    (*_player.mOnKilled.first)(_killer.mObj, reason, static_cast< Int32 >(body_part), team_kill);
    (*mOnPlayerKilled.first)(_player.mObj, _killer.mObj, reason, static_cast< Int32 >(body_part), team_kill);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerEmbarking(Int32 player_id, Int32 vehicle_id, Int32 slot_index)
{
    PlayerInst & _player = m_Players.at(player_id);
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_player.mOnEmbarking.first)(_vehicle.mObj, slot_index);
    (*_vehicle.mOnEmbarking.first)(_player.mObj, slot_index);
    (*mOnPlayerEmbarking.first)(_player.mObj, _vehicle.mObj, slot_index);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerEmbarked(Int32 player_id, Int32 vehicle_id, Int32 slot_index)
{
    PlayerInst & _player = m_Players.at(player_id);
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_player.mOnEmbarked.first)(_vehicle.mObj, slot_index);
    (*_vehicle.mOnEmbarked.first)(_player.mObj, slot_index);
    (*mOnPlayerEmbarked.first)(_player.mObj, _vehicle.mObj, slot_index);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerDisembark(Int32 player_id, Int32 vehicle_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_player.mOnDisembark.first)(_vehicle.mObj);
    (*_vehicle.mOnDisembark.first)(_player.mObj);
    (*mOnPlayerDisembark.first)(_player.mObj, _vehicle.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerRename(Int32 player_id, CCStr old_name, CCStr new_name)
{
    PlayerInst & _player = m_Players.at(player_id);
    LightObj oname(old_name, -1), nname(new_name, -1);
    (*_player.mOnRename.first)(oname, nname);
    (*mOnPlayerRename.first)(_player.mObj, oname, nname);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerState(Int32 player_id, Int32 old_state, Int32 new_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnState.first)(old_state, new_state);
    (*mOnPlayerState.first)(_player.mObj, old_state, new_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateNone(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnStateNone.first)(old_state);
    (*mOnStateNone.first)(_player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateNormal(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnStateNormal.first)(old_state);
    (*mOnStateNormal.first)(_player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateAim(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnStateAim.first)(old_state);
    (*mOnStateAim.first)(_player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateDriver(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnStateDriver.first)(old_state);
    (*mOnStateDriver.first)(_player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStatePassenger(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnStatePassenger.first)(old_state);
    (*mOnStatePassenger.first)(_player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateEnterDriver(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnStateEnterDriver.first)(old_state);
    (*mOnStateEnterDriver.first)(_player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateEnterPassenger(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnStateEnterPassenger.first)(old_state);
    (*mOnStateEnterPassenger.first)(_player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateExit(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnStateExit.first)(old_state);
    (*mOnStateExit.first)(_player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateUnspawned(Int32 player_id, Int32 old_state)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnStateUnspawned.first)(old_state);
    (*mOnStateUnspawned.first)(_player.mObj, old_state);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerAction(Int32 player_id, Int32 old_action, Int32 new_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnAction.first)(old_action, new_action);
    (*mOnPlayerAction.first)(_player.mObj, old_action, new_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionNone(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionNone.first)(old_action);
    (*mOnActionNone.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionNormal(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionNormal.first)(old_action);
    (*mOnActionNormal.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionAiming(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionAiming.first)(old_action);
    (*mOnActionAiming.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionShooting(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionShooting.first)(old_action);
    (*mOnActionShooting.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionJumping(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionJumping.first)(old_action);
    (*mOnActionJumping.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionLieDown(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionLieDown.first)(old_action);
    (*mOnActionLieDown.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionGettingUp(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionGettingUp.first)(old_action);
    (*mOnActionGettingUp.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionJumpVehicle(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionJumpVehicle.first)(old_action);
    (*mOnActionJumpVehicle.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionDriving(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionDriving.first)(old_action);
    (*mOnActionDriving.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionDying(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionDying.first)(old_action);
    (*mOnActionDying.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionWasted(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionWasted.first)(old_action);
    (*mOnActionWasted.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionEmbarking(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionEmbarking.first)(old_action);
    (*mOnActionEmbarking.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionDisembarking(Int32 player_id, Int32 old_action)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnActionDisembarking.first)(old_action);
    (*mOnActionDisembarking.first)(_player.mObj, old_action);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerBurning(Int32 player_id, bool is_on_fire)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnBurning.first)(is_on_fire);
    (*mOnPlayerBurning.first)(_player.mObj, is_on_fire);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCrouching(Int32 player_id, bool is_crouching)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnCrouching.first)(is_crouching);
    (*mOnPlayerCrouching.first)(_player.mObj, is_crouching);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerGameKeys(Int32 player_id, Uint32 old_keys, Uint32 new_keys)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnGameKeys.first)(old_keys, new_keys);
    (*mOnPlayerGameKeys.first)(_player.mObj, old_keys, new_keys);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerStartTyping(Int32 player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnStartTyping.first)();
    (*mOnPlayerStartTyping.first)(_player.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerStopTyping(Int32 player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnStopTyping.first)();
    (*mOnPlayerStopTyping.first)(_player.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerAway(Int32 player_id, bool is_away)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnAway.first)(is_away);
    (*mOnPlayerAway.first)(_player.mObj, is_away);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerMessage(Int32 player_id, CCStr message)
{
    PlayerInst & _player = m_Players.at(player_id);
    LightObj msg(message, -1);
    (*_player.mOnMessage.first)(msg);
    (*mOnPlayerMessage.first)(_player.mObj, msg);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCommand(Int32 player_id, CCStr message)
{
    PlayerInst & _player = m_Players.at(player_id);
    LightObj msg(message, -1);
    (*_player.mOnCommand.first)(msg);
    (*mOnPlayerCommand.first)(_player.mObj, msg);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerPrivateMessage(Int32 player_id, Int32 target_player_id, CCStr message)
{
    PlayerInst & _player = m_Players.at(player_id);
    PlayerInst & _receiver = m_Players.at(target_player_id);
    LightObj msg(message, -1);
    (*_player.mOnMessage.first)(_receiver.mObj,  msg);
    (*mOnPlayerPrivateMessage.first)(_player.mObj, _receiver.mObj, msg);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerKeyPress(Int32 player_id, Int32 bind_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    KeybindInst & _keybind = m_Keybinds.at(bind_id);
    (*_player.mOnKeyPress.first)(_keybind.mObj);
    (*_keybind.mOnKeyPress.first)(_player.mObj);
    (*mOnPlayerKeyPress.first)(_player.mObj, _keybind.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerKeyRelease(Int32 player_id, Int32 bind_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    KeybindInst & _keybind = m_Keybinds.at(bind_id);
    (*_keybind.mOnKeyRelease.first)(_player.mObj);
    (*_player.mOnKeyRelease.first)(_keybind.mObj);
    (*mOnPlayerKeyRelease.first)(_player.mObj, _keybind.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerSpectate(Int32 player_id, Int32 target_player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    PlayerInst & _target = m_Players.at(target_player_id);
    (*_player.mOnSpectate.first)(_target.mObj);
    (*mOnPlayerSpectate.first)(_player.mObj, _target.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerUnspectate(Int32 player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnUnspectate.first)();
    (*mOnPlayerUnspectate.first)(_player.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCrashreport(Int32 player_id, CCStr report)
{
    PlayerInst & _player = m_Players.at(player_id);
    LightObj rep(report, -1);
    (*_player.mOnCrashreport.first)(rep);
    (*mOnPlayerCrashreport.first)(_player.mObj, rep);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerModuleList(Int32 player_id, CCStr list)
{
    PlayerInst & _player = m_Players.at(player_id);
    LightObj rep(list, -1);
    (*_player.mOnModuleList.first)(rep);
    (*mOnPlayerModuleList.first)(_player.mObj, rep);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleExplode(Int32 vehicle_id)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnExplode.first)();
    (*mOnVehicleExplode.first)(_vehicle.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleRespawn(Int32 vehicle_id)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnRespawn.first)();
    (*mOnVehicleRespawn.first)(_vehicle.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectShot(Int32 object_id, Int32 player_id, Int32 weapon_id)
{
    ObjectInst & _object = m_Objects.at(object_id);
    PlayerInst & _player = m_Players.at(player_id);
    (*_object.mOnShot.first)(_player.mObj, weapon_id);
    (*_player.mOnObjectShot.first)(_object.mObj, weapon_id);
    (*mOnObjectShot.first)(_player.mObj, _object.mObj, weapon_id);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectTouched(Int32 object_id, Int32 player_id)
{
    ObjectInst & _object = m_Objects.at(object_id);
    PlayerInst & _player = m_Players.at(player_id);
    (*_object.mOnTouched.first)(_player.mObj);
    (*_player.mOnObjectTouched.first)(_object.mObj);
    (*mOnObjectTouched.first)(_player.mObj, _object.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupClaimed(Int32 pickup_id, Int32 player_id)
{
    PickupInst & _pickup = m_Pickups.at(pickup_id);
    PlayerInst & _player = m_Players.at(player_id);
    (*_pickup.mOnClaimed.first)(_player.mObj);
    (*_player.mOnPickupClaimed.first)(_pickup.mObj);
    (*mOnPickupClaimed.first)(_player.mObj, _pickup.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupCollected(Int32 pickup_id, Int32 player_id)
{
    PickupInst & _pickup = m_Pickups.at(pickup_id);
    PlayerInst & _player = m_Players.at(player_id);
    (*_pickup.mOnCollected.first)(_player.mObj);
    (*_player.mOnPickupCollected.first)(_pickup.mObj);
    (*mOnPickupCollected.first)(_player.mObj, _pickup.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupRespawn(Int32 pickup_id)
{
    PickupInst & _pickup = m_Pickups.at(pickup_id);
    (*_pickup.mOnRespawn.first)();
    (*mOnPickupRespawn.first)(_pickup.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointEntered(Int32 checkpoint_id, Int32 player_id)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint_id);
    PlayerInst & _player = m_Players.at(player_id);
    (*_checkpoint.mOnEntered.first)(_player.mObj);
    (*_player.mOnCheckpointEntered.first)(_checkpoint.mObj);
    (*mOnCheckpointEntered.first)(_player.mObj, _checkpoint.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointExited(Int32 checkpoint_id, Int32 player_id)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint_id);
    PlayerInst & _player = m_Players.at(player_id);
    (*_checkpoint.mOnExited.first)(_player.mObj);
    (*_player.mOnCheckpointExited.first)(_checkpoint.mObj);
    (*mOnCheckpointExited.first)(_player.mObj, _checkpoint.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointWorld(Int32 checkpoint_id, Int32 old_world, Int32 new_world)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint_id);
    (*_checkpoint.mOnWorld.first)(old_world, new_world);
    (*mOnCheckpointWorld.first)(_checkpoint.mObj, old_world, new_world);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointRadius(Int32 checkpoint_id, Float32 old_radius, Float32 new_radius)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint_id);
    (*_checkpoint.mOnRadius.first)(old_radius, new_radius);
    (*mOnCheckpointRadius.first)(_checkpoint.mObj, old_radius, new_radius);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectWorld(Int32 object_id, Int32 old_world, Int32 new_world)
{
    ObjectInst & _object = m_Objects.at(object_id);
    (*_object.mOnWorld.first)(old_world, new_world);
    (*mOnObjectWorld.first)(_object.mObj, old_world, new_world);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectAlpha(Int32 object_id, Int32 old_alpha, Int32 new_alpha, Int32 time)
{
    ObjectInst & _object = m_Objects.at(object_id);
    (*_object.mOnAlpha.first)(old_alpha, new_alpha, time);
    (*mOnObjectAlpha.first)(_object.mObj, old_alpha, new_alpha, time);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupWorld(Int32 pickup_id, Int32 old_world, Int32 new_world)
{
    PickupInst & _pickup = m_Pickups.at(pickup_id);
    (*_pickup.mOnWorld.first)(old_world, new_world);
    (*mOnPickupWorld.first)(_pickup.mObj, old_world, new_world);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupAlpha(Int32 pickup_id, Int32 old_alpha, Int32 new_alpha)
{
    PickupInst & _pickup = m_Pickups.at(pickup_id);
    (*_pickup.mOnAlpha.first)(old_alpha, new_alpha);
    (*mOnPickupAlpha.first)(_pickup.mObj, old_alpha, new_alpha);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupAutomatic(Int32 pickup_id, bool old_status, bool new_status)
{
    PickupInst & _pickup = m_Pickups.at(pickup_id);
    (*_pickup.mOnAutomatic.first)(old_status, new_status);
    (*mOnPickupAutomatic.first)(_pickup.mObj, old_status, new_status);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupAutoTimer(Int32 pickup_id, Int32 old_timer, Int32 new_timer)
{
    PickupInst & _pickup = m_Pickups.at(pickup_id);
    (*_pickup.mOnAutoTimer.first)(old_timer, new_timer);
    (*mOnPickupAutoTimer.first)(_pickup.mObj, old_timer, new_timer);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupOption(Int32 pickup_id, Int32 option_id, bool value,
                                Int32 header, LightObj & payload)
{
    PickupInst & _pickup = m_Pickups.at(pickup_id);
    (*_pickup.mOnOption.first)(option_id, value, header, payload);
    (*mOnPickupOption.first)(_pickup.mObj, option_id, value, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectReport(Int32 object_id, bool old_status, bool new_status, bool touched)
{
    ObjectInst & _object = m_Objects.at(object_id);
    (*_object.mOnReport.first)(old_status, new_status, touched);
    (*mOnObjectReport.first)(_object.mObj, old_status, new_status, touched);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerHealth(Int32 player_id, Float32 old_health, Float32 new_health)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnHealth.first)(old_health, new_health);
    (*mOnPlayerHealth.first)(_player.mObj, old_health, new_health);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerArmour(Int32 player_id, Float32 old_armour, Float32 new_armour)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnArmour.first)(old_armour, new_armour);
    (*mOnPlayerArmour.first)(_player.mObj, old_armour, new_armour);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerWeapon(Int32 player_id, Int32 old_weapon, Int32 new_weapon)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnWeapon.first)(old_weapon, new_weapon);
    (*mOnPlayerWeapon.first)(_player.mObj, old_weapon, new_weapon);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerHeading(Int32 player_id, Float32 old_heading, Float32 new_heading)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnHeading.first)(old_heading, new_heading);
    (*mOnPlayerHeading.first)(_player.mObj, old_heading, new_heading);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerPosition(Int32 player_id)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnPosition.first)(_player.mTrackPositionHeader, _player.mTrackPositionPayload);
    (*mOnPlayerPosition.first)(_player.mObj, _player.mTrackPositionHeader, _player.mTrackPositionPayload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerOption(Int32 player_id, Int32 option_id, bool value,
                                Int32 header, LightObj & payload)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnOption.first)(option_id, value, header, payload);
    (*mOnPlayerOption.first)(_player.mObj, option_id, value, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerAdmin(Int32 player_id, bool old_status, bool new_status)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnAdmin.first)(old_status, new_status);
    (*mOnPlayerAdmin.first)(_player.mObj, old_status, new_status);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerWorld(Int32 player_id, Int32 old_world, Int32 new_world, bool secondary)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnWorld.first)(old_world, new_world, secondary);
    (*mOnPlayerWorld.first)(_player.mObj, old_world, new_world, secondary);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerTeam(Int32 player_id, Int32 old_team, Int32 new_team)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnTeam.first)(old_team, new_team);
    (*mOnPlayerTeam.first)(_player.mObj, old_team, new_team);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerSkin(Int32 player_id, Int32 old_skin, Int32 new_skin)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnSkin.first)(old_skin, new_skin);
    (*mOnPlayerSkin.first)(_player.mObj, old_skin, new_skin);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerMoney(Int32 player_id, Int32 old_money, Int32 new_money)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnMoney.first)(old_money, new_money);
    (*mOnPlayerMoney.first)(_player.mObj, old_money, new_money);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerScore(Int32 player_id, Int32 old_score, Int32 new_score)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnScore.first)(old_score, new_score);
    (*mOnPlayerScore.first)(_player.mObj, old_score, new_score);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerWantedLevel(Int32 player_id, Int32 old_level, Int32 new_level)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnWantedLevel.first)(old_level, new_level);
    (*mOnPlayerWantedLevel.first)(_player.mObj, old_level, new_level);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerImmunity(Int32 player_id, Int32 old_immunity, Int32 new_immunity)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnImmunity.first)(old_immunity, new_immunity);
    (*mOnPlayerImmunity.first)(_player.mObj, old_immunity, new_immunity);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerAlpha(Int32 player_id, Int32 old_alpha, Int32 new_alpha, Int32 fade)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnAlpha.first)(old_alpha, new_alpha, fade);
    (*mOnPlayerAlpha.first)(_player.mObj, old_alpha, new_alpha, fade);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerEnterArea(Int32 player_id, LightObj & area_obj)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnEnterArea.first)(area_obj);
    (*mOnPlayerEnterArea.first)(_player.mObj, area_obj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerLeaveArea(Int32 player_id, LightObj & area_obj)
{
    PlayerInst & _player = m_Players.at(player_id);
    (*_player.mOnLeaveArea.first)(area_obj);
    (*mOnPlayerLeaveArea.first)(_player.mObj, area_obj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleColor(Int32 vehicle_id, Int32 changed)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnColor.first)(changed);
    (*mOnVehicleColor.first)(_vehicle.mObj, changed);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleHealth(Int32 vehicle_id, Float32 old_health, Float32 new_health)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnHealth.first)(old_health, new_health);
    (*mOnVehicleHealth.first)(_vehicle.mObj, old_health, new_health);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehiclePosition(Int32 vehicle_id)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnPosition.first)();
    (*mOnVehiclePosition.first)(_vehicle.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleRotation(Int32 vehicle_id)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnRotation.first)();
    (*mOnVehicleRotation.first)(_vehicle.mObj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleOption(Int32 vehicle_id, Int32 option_id, bool value,
                                Int32 header, LightObj & payload)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnOption.first)(option_id, value, header, payload);
    (*mOnVehicleOption.first)(_vehicle.mObj, option_id, value, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleWorld(Int32 vehicle_id, Int32 old_world, Int32 new_world)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnWorld.first)(old_world, new_world);
    (*mOnVehicleWorld.first)(_vehicle.mObj, old_world, new_world);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleImmunity(Int32 vehicle_id, Int32 old_immunity, Int32 new_immunity)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnImmunity.first)(old_immunity, new_immunity);
    (*mOnVehicleImmunity.first)(_vehicle.mObj, old_immunity, new_immunity);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehiclePartStatus(Int32 vehicle_id, Int32 part, Int32 old_status, Int32 new_status)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnPartStatus.first)(part, old_status, new_status);
    (*mOnVehiclePartStatus.first)(_vehicle.mObj, part, old_status, new_status);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleTyreStatus(Int32 vehicle_id, Int32 tyre, Int32 old_status, Int32 new_status)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnTyreStatus.first)(tyre, old_status, new_status);
    (*mOnVehicleTyreStatus.first)(_vehicle.mObj, tyre, old_status, new_status);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleDamageData(Int32 vehicle_id, Uint32 old_data, Uint32 new_data)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnDamageData.first)(old_data, new_data);
    (*mOnVehicleDamageData.first)(_vehicle.mObj, old_data, new_data);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleRadio(Int32 vehicle_id, Int32 old_radio, Int32 new_radio)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnRadio.first)(old_radio, new_radio);
    (*mOnVehicleRadio.first)(_vehicle.mObj, old_radio, new_radio);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleHandlingRule(Int32 vehicle_id, Int32 rule, Float32 old_data, Float32 new_data)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnHandlingRule.first)(rule, old_data, new_data);
    (*mOnVehicleHandlingRule.first)(_vehicle.mObj, rule, old_data, new_data);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleEnterArea(Int32 vehicle_id, LightObj & area_obj)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnEnterArea.first)(area_obj);
    (*mOnVehicleEnterArea.first)(_vehicle.mObj, area_obj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleLeaveArea(Int32 vehicle_id, LightObj & area_obj)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle_id);
    (*_vehicle.mOnLeaveArea.first)(area_obj);
    (*mOnVehicleLeaveArea.first)(_vehicle.mObj, area_obj);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerOption(Int32 option, bool value, Int32 header, LightObj & payload)
{
    if (m_CircularLocks & CCL_EMIT_SERVER_OPTION)
    {
        return; // Already inside this event!
    }
    // Prevent further calls to this event
    BitGuardU32 bg(m_CircularLocks, CCL_EMIT_SERVER_OPTION);
    // Now forward the event call
    (*mOnServerOption.first)(option, value, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitScriptReload(Int32 header, LightObj & payload)
{
    (*mOnScriptReload.first)(header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitScriptLoaded()
{
    (*mOnScriptLoaded.first)();
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
                DeallocVehicle(entity_id, false, SQMOD_DESTROY_POOL, NullLightObj());
            }
            // Do we already have this vehicle that we're trying to create?
            else if (INVALID_ENTITY(m_Vehicles[entity_id].mID))
            {
                AllocVehicle(entity_id, false, SQMOD_CREATE_POOL, NullLightObj());
            }
        break;
        case vcmpEntityPoolObject:
            // Do we even have this object that we're trying to delete?
            if (is_deleted && VALID_ENTITY(m_Objects[entity_id].mID))
            {
                DeallocObject(entity_id, false, SQMOD_DESTROY_POOL, NullLightObj());
            }
            // Do we already have this object that we're trying to create?
            else if (INVALID_ENTITY(m_Objects[entity_id].mID))
            {
                AllocObject(entity_id, false, SQMOD_CREATE_POOL, NullLightObj());
            }
        break;
        case vcmpEntityPoolPickup:
            // Do we even have this pickup that we're trying to delete?
            if (is_deleted && VALID_ENTITY(m_Pickups[entity_id].mID))
            {
                DeallocPickup(entity_id, false, SQMOD_DESTROY_POOL, NullLightObj());
            }
            // Do we already have this pickup that we're trying to create?
            else if (INVALID_ENTITY(m_Pickups[entity_id].mID))
            {
                AllocPickup(entity_id, false, SQMOD_CREATE_POOL, NullLightObj());
            }
        break;
        case vcmpEntityPoolRadio:
            // @TODO Implement...
        break;
        case vcmpEntityPoolCheckPoint:
            // Do we even have this checkpoint that we're trying to delete?
            if (is_deleted && VALID_ENTITY(m_Checkpoints[entity_id].mID))
            {
                DeallocCheckpoint(entity_id, false, SQMOD_DESTROY_POOL, NullLightObj());
            }
            // Do we already have this checkpoint that we're trying to create?
            else if (INVALID_ENTITY(m_Checkpoints[entity_id].mID))
            {
                AllocCheckpoint(entity_id, false, SQMOD_CREATE_POOL, NullLightObj());
            }
        break;
        case vcmpEntityPoolBlip:
            // Do we even have this blip that we're trying to delete?
            if (is_deleted && VALID_ENTITY(m_Blips[entity_id].mID))
            {
                DeallocBlip(entity_id, false, SQMOD_DESTROY_POOL, NullLightObj());
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
                AllocBlip(entity_id, false, SQMOD_CREATE_POOL, NullLightObj());
            }
        break;
        default:
            LogErr("Unknown change in the entity pool of type: %d", entity_type);
    }
    // Finally, forward the event to the script
    (*mOnEntityPool.first)(static_cast< Int32 >(entity_type), entity_id, is_deleted);
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
        // Should we check for area collision
        if (inst.mFlags & ENF_AREA_TRACK)
        {
            // Eliminate existing areas first, if the player is not in them anymore
            inst.mAreas.erase(std::remove_if(inst.mAreas.begin(), inst.mAreas.end(),
                [this, pos, player_id](AreaList::reference ap) -> bool {
                    // Is this player still in this area?
                    if (!ap.first->TestEx(pos.x, pos.y))
                    {
                        // Emit the script event
                        this->EmitPlayerLeaveArea(player_id, ap.second);
                        // Remove this area
                        return true;
                    }
                    // Still in this area
                    return false;
            }), inst.mAreas.end());
            // See if the player entered any new areas
            AreaManager::Get().TestPoint([this, &inst, player_id](AreaList::reference ap) -> void {
                // Was the player in this area before?
                if (std::find_if(inst.mAreas.begin(), inst.mAreas.end(),
                    [a = ap.first](AreaList::reference ap) -> bool {
                        return (a == ap.first);
                    }) == inst.mAreas.end())
                {
                    // The player just entered this area so emit the event
                    this->EmitPlayerEnterArea(player_id, ap.second);
                    // Now store this area so we know when the player leaves
                    inst.mAreas.emplace_back(ap);
                }
                // The player was in this area before so ignore it
            }, pos.x, pos.y);
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
    (*inst.mOnUpdate.first)(static_cast< Int32 >(update_type));
    (*mOnPlayerUpdate.first)(inst.mObj, static_cast< Int32 >(update_type));
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointStream(int32_t player_id, int32_t entity_id, bool is_deleted)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(entity_id);
    PlayerInst & _client = m_Players.at(player_id);
    (*_checkpoint.mOnStream.first)(_client.mObj, is_deleted);
    (*_client.mOnEntityStream.first)(_checkpoint.mObj, static_cast< Int32 >(vcmpEntityPoolCheckPoint), is_deleted);
    (*mOnCheckpointStream.first)(_client.mObj, _checkpoint.mObj, is_deleted);
    (*mOnEntityStream.first)(_client.mObj, _checkpoint.mObj, static_cast< Int32 >(vcmpEntityPoolCheckPoint), is_deleted);
}
// ------------------------------------------------------------------------------------------------
void Core::EmitObjectStream(int32_t player_id, int32_t entity_id, bool is_deleted)
{
    ObjectInst & _object = m_Objects.at(entity_id);
    PlayerInst & _client = m_Players.at(player_id);
    (*_object.mOnStream.first)(_client.mObj, is_deleted);
    (*_client.mOnEntityStream.first)(_object.mObj, static_cast< Int32 >(vcmpEntityPoolObject), is_deleted);
    (*mOnObjectStream.first)(_client.mObj, _object.mObj, is_deleted);
    (*mOnEntityStream.first)(_client.mObj, _object.mObj, static_cast< Int32 >(vcmpEntityPoolObject), is_deleted);
}
// ------------------------------------------------------------------------------------------------
void Core::EmitPickupStream(int32_t player_id, int32_t entity_id, bool is_deleted)
{
    PickupInst & _pickup = m_Pickups.at(entity_id);
    PlayerInst & _client = m_Players.at(player_id);
    (*_pickup.mOnStream.first)(_client.mObj, is_deleted);
    (*_client.mOnEntityStream.first)(_pickup.mObj, static_cast< Int32 >(vcmpEntityPoolPickup), is_deleted);
    (*mOnPickupStream.first)(_client.mObj, _pickup.mObj, is_deleted);
    (*mOnEntityStream.first)(_client.mObj, _pickup.mObj, static_cast< Int32 >(vcmpEntityPoolPickup), is_deleted);
}
// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleStream(int32_t player_id, int32_t entity_id, bool is_deleted)
{
    VehicleInst & _vehicle = m_Vehicles.at(entity_id);
    PlayerInst & _client = m_Players.at(player_id);
    (*_vehicle.mOnStream.first)(_client.mObj, is_deleted);
    (*_client.mOnEntityStream.first)(_vehicle.mObj, static_cast< Int32 >(vcmpEntityPoolVehicle), is_deleted);
    (*mOnVehicleStream.first)(_client.mObj, _vehicle.mObj, is_deleted);
    (*mOnEntityStream.first)(_client.mObj, _vehicle.mObj, static_cast< Int32 >(vcmpEntityPoolVehicle), is_deleted);
}
// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerStream(int32_t player_id, int32_t entity_id, bool is_deleted)
{
    PlayerInst & _player = m_Players.at(entity_id);
    PlayerInst & _client = m_Players.at(player_id);
    (*_player.mOnStream.first)(_client.mObj, is_deleted);
    (*_client.mOnEntityStream.first)(_player.mObj, static_cast< Int32 >(vcmpEntityPoolPlayer), is_deleted);
    (*mOnPlayerStream.first)(_client.mObj, _player.mObj, is_deleted);
    (*mOnEntityStream.first)(_client.mObj, _player.mObj, static_cast< Int32 >(vcmpEntityPoolPlayer), is_deleted);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitEntityStreaming(int32_t player_id, int32_t entity_id, vcmpEntityPool entity_type, bool is_deleted)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(player_id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Cannot notify player with invalid identifier about streaming: %d", player_id);
    }
    // See what type of entity changed
    switch (entity_type)
    {
        case vcmpEntityPoolVehicle: {
            // Make sure that the specified entity identifier is valid
            if (INVALID_ENTITYEX(entity_id, SQMOD_VEHICLE_POOL))
            {
                STHROWF("Cannot stream vehicle with invalid identifier: %d", entity_id);
            }
            // Forward the event to the dedicated handler
            EmitVehicleStream(player_id, entity_id, is_deleted);
        } break;
        case vcmpEntityPoolObject: {
            // Make sure that the specified entity identifier is valid
            if (INVALID_ENTITYEX(entity_id, SQMOD_OBJECT_POOL))
            {
                STHROWF("Cannot stream object with invalid identifier: %d", entity_id);
            }
            // Forward the event to the dedicated handler
            EmitObjectStream(player_id, entity_id, is_deleted);
        } break;
        case vcmpEntityPoolPickup: {
            // Make sure that the specified entity identifier is valid
            if (INVALID_ENTITYEX(entity_id, SQMOD_PICKUP_POOL))
            {
                STHROWF("Cannot stream pickup with invalid identifier: %d", entity_id);
            }
            // Forward the event to the dedicated handler
            EmitPickupStream(player_id, entity_id, is_deleted);
        } break;
        case vcmpEntityPoolPlayer: {
            // Make sure that the specified entity identifier is valid
            if (INVALID_ENTITYEX(entity_id, SQMOD_PLAYER_POOL))
            {
                STHROWF("Cannot stream player with invalid identifier: %d", entity_id);
            }
            // Forward the event to the dedicated handler
            EmitPlayerStream(player_id, entity_id, is_deleted);
        } break;
        case vcmpEntityPoolCheckPoint: {
            // Make sure that the specified entity identifier is valid
            if (INVALID_ENTITYEX(entity_id, SQMOD_CHECKPOINT_POOL))
            {
                STHROWF("Cannot stream checkpoint with invalid identifier: %d", entity_id);
            }
            // Forward the event to the dedicated handler
            EmitCheckpointStream(player_id, entity_id, is_deleted);
        } break;
        default:
            LogErr("Unknown change in entity streaming: client %d > type %d > entity %d", player_id, entity_type, entity_id);
    }
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
            // New vehicle position
            Vector3 pos;
            // Retrieve the current vehicle position
            _Func->GetVehiclePosition(vehicle_id, &pos.x, &pos.y, &pos.z);
            // Should we check for area collision
            if (inst.mFlags & ENF_AREA_TRACK)
            {
                // Eliminate existing areas first, if the vehicle is not in them anymore
                inst.mAreas.erase(std::remove_if(inst.mAreas.begin(), inst.mAreas.end(),
                    [this, pos, vehicle_id](AreaList::reference ap) -> bool {
                        // Is this vehicle still in this area?
                        if (!ap.first->TestEx(pos.x, pos.y))
                        {
                            // Emit the script event
                            this->EmitVehicleLeaveArea(vehicle_id, ap.second);
                            // Remove this area
                            return true;
                        }
                        // Still in this area
                        return false;
                }), inst.mAreas.end());
                // See if the vehicle entered any new areas
                AreaManager::Get().TestPoint([this, &inst, vehicle_id](AreaList::reference ap) -> void {
                    // Was the vehicle in this area before?
                    if (std::find_if(inst.mAreas.begin(), inst.mAreas.end(),
                        [a = ap.first](AreaList::reference ap) -> bool {
                            return (a == ap.first);
                        }) == inst.mAreas.end())
                    {
                        // The vehicle just entered this area so emit the event
                        this->EmitVehicleEnterArea(vehicle_id, ap.second);
                        // Now store this area so we know when the vehicle leaves
                        inst.mAreas.emplace_back(ap);
                    }
                    // The vehicle was in this area before so ignore it
                }, pos.x, pos.y);
            }
            // Update the tracked value
            inst.mLastPosition = pos;
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
            // Obtain the current colors of this instance
            _Func->GetVehicleColour(vehicle_id, &primary, &secondary);
            // Which colors changed
            Int32 changed = 0;
            // Did the primary color changed?
            if (primary != inst.mLastPrimaryColor)
            {
                changed |= (1<<0);
            }
            // Did the secondary color changed?
            if (primary != inst.mLastSecondaryColor)
            {
                changed |= (1<<1);
            }
            // Trigger the event specific to this change
            EmitVehicleColor(vehicle_id, changed);
            // Update the tracked value
            inst.mLastPrimaryColor = primary;
            inst.mLastSecondaryColor = secondary;
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
            (*inst.mOnUpdate.first)(static_cast< Int32 >(update_type));
            (*mOnVehicleUpdate.first)(inst.mObj, static_cast< Int32 >(update_type));
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::EmitClientScriptData(Int32 player_id, const uint8_t * data, size_t size)
{
    PlayerInst & _player = m_Players.at(player_id);
    // Don't even bother if there's no one listening
    if (_player.mOnClientScriptData.first->IsEmpty() && mOnClientScriptData.first->IsEmpty())
    {
        return;
    }
    // Allocate a buffer with the received size
    Buffer b(size);
    // Replicate the data to the allocated buffer
    b.Write(0, reinterpret_cast< Buffer::ConstPtr >(data), size);
    // Prepare an object for the obtained buffer
    LightObj o;
    // Attempt to create the requested buffer
    try
    {
        // Remember the current stack size
        const StackGuard sg;
        // Create a protected instance of a buffer wrapper
        DeleteGuard< SqBuffer > ad(new SqBuffer(std::move(b)));
        // Transform the pointer into a script object
        PushVar< SqBuffer * >(DefaultVM::Get(), ad.Get());
        // The script took over the instance now
        ad.Release();
        // Get the object from the stack and store it
        o = Var< LightObj >(DefaultVM::Get(), -1).value;
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
    (*_player.mOnClientScriptData.first)(o, size);
    (*mOnClientScriptData.first)(_player.mObj, o, size);
}

} // Namespace:: SqMod
