// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
#define NULL_SQOBJ_(o) (o.IsNull() ? "NULL" : "OBJ")

// ------------------------------------------------------------------------------------------------
#ifdef VCMP_ENABLE_OFFICIAL
// ------------------------------------------------------------------------------------------------
// Invoke a script function from the root table with no return value
template < class... Args > static void ExecuteLegacyEvent(HSQUIRRELVM vm, const SQChar * name, Args &&... args)
{
    StackGuard sqsg(vm);
    // Push the root table on the stack
    sq_pushroottable(vm);
    // Grab the function from the table
    Function fn(vm, sq_gettop(vm), name);
    // Was there a callback with that name?
    if (fn.IsNull())
    {
        return; // Nothing to invoke
    }
    // Forward the call to the function
    fn.Execute(std::forward< Args >(args)...);
}
// ------------------------------------------------------------------------------------------------
// Invoke a script function from the root table with a return value
template < class... Args > static LightObj EvaluateLegacyEvent(HSQUIRRELVM vm, const SQChar * name, Args &&... args)
{
    StackGuard sqsg(vm);
    // Push the root table on the stack
    sq_pushroottable(vm);
    // Grab the function from the table
    Function fn(vm, sq_gettop(vm), name);
    // Was there a callback with that name?
    if (fn.IsNull())
    {
        return LightObj{}; // Nothing to invoke
    }
    // Forward the call to the function
    return fn.Eval(std::forward< Args >(args)...);
}
// ------------------------------------------------------------------------------------------------
static int32_t g_LastHour = 0;
static int32_t g_LastMinute = 0;
#endif

// ------------------------------------------------------------------------------------------------
void Core::EmitCustomEvent(int32_t group, int32_t header, LightObj & payload) const
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::CustomEvent(%d, %d, %s)", group, header, NULL_SQOBJ_(payload))
    (*mOnCustomEvent.first)(group, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::CustomEvent")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitBlipCreated(int32_t blip, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::BlipCreated(%d, %d, %s)", blip, header, NULL_SQOBJ_(payload))
    (*mOnBlipCreated.first)(m_Blips.at(static_cast< size_t >(blip)).mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::BlipCreated")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointCreated(int32_t checkpoint, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::CheckpointCreated(%d, %d, %s)", checkpoint, header, NULL_SQOBJ_(payload))
    (*mOnCheckpointCreated.first)(m_Checkpoints.at(static_cast< size_t >(checkpoint)).mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::CheckpointCreated")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitKeyBindCreated(int32_t key_bind, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::KeyBindCreated(%d, %d, %s)", key_bind, header, NULL_SQOBJ_(payload))
    (*mOnKeyBindCreated.first)(m_KeyBinds.at(static_cast< size_t >(key_bind)).mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::KeyBindCreated")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectCreated(int32_t object, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ObjectCreated(%d, %d, %s)", object, header, NULL_SQOBJ_(payload))
    (*mOnObjectCreated.first)(m_Objects.at(static_cast< size_t >(object)).mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ObjectCreated")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupCreated(int32_t pickup, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupCreated(%d, %d, %s)", pickup, header, NULL_SQOBJ_(payload))
    (*mOnPickupCreated.first)(m_Pickups.at(static_cast< size_t >(pickup)).mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupCreated")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCreated(int32_t player, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerCreated(%d, %d, %s)", player, header, NULL_SQOBJ_(payload))
    (*mOnPlayerCreated.first)(m_Players.at(static_cast< size_t >(player)).mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerCreated")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerJoin"), m_Players.at(static_cast< size_t >(player)).mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleCreated(int32_t vehicle, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleCreated(%d, %d, %s)", vehicle, header, NULL_SQOBJ_(payload))
    (*mOnVehicleCreated.first)(m_Vehicles.at(static_cast< size_t >(vehicle)).mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleCreated")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitBlipDestroyed(int32_t blip, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::BlipDestroyed(%d, %d, %s)", blip, header, NULL_SQOBJ_(payload))
    BlipInst & _blip = m_Blips.at(static_cast< size_t >(blip));
    (*_blip.mOnDestroyed.first)(header, payload);
    (*mOnBlipDestroyed.first)(_blip.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::BlipDestroyed")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointDestroyed(int32_t checkpoint, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::CheckpointDestroyed(%d, %d, %s)", checkpoint, header, NULL_SQOBJ_(payload))
    CheckpointInst & _checkpoint = m_Checkpoints.at(static_cast< size_t >(checkpoint));
    (*_checkpoint.mOnDestroyed.first)(header, payload);
    (*mOnCheckpointDestroyed.first)(_checkpoint.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::CheckpointDestroyed")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitKeyBindDestroyed(int32_t keybind, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::KeyBindDestroyed(%d, %d, %s)", keybind, header, NULL_SQOBJ_(payload))
    KeyBindInst & _keybind = m_KeyBinds.at(static_cast< size_t >(keybind));
    (*_keybind.mOnDestroyed.first)(header, payload);
    (*mOnKeyBindDestroyed.first)(_keybind.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::KeyBindDestroyed")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectDestroyed(int32_t object, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ObjectDestroyed(%d, %d, %s)", object, header, NULL_SQOBJ_(payload))
    ObjectInst & _object = m_Objects.at(static_cast< size_t >(object));
    (*_object.mOnDestroyed.first)(header, payload);
    (*mOnObjectDestroyed.first)(_object.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ObjectDestroyed")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupDestroyed(int32_t pickup, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupDestroyed(%d, %d, %s)", pickup, header, NULL_SQOBJ_(payload))
    PickupInst & _pickup = m_Pickups.at(static_cast< size_t >(pickup));
    (*_pickup.mOnDestroyed.first)(header, payload);
    (*mOnPickupDestroyed.first)(_pickup.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupDestroyed")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerDestroyed(int32_t player, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerDestroyed(%d, %d, %s)", player, header, NULL_SQOBJ_(payload))
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player));
    (*_player.mOnDestroyed.first)(header, payload);
    (*mOnPlayerDestroyed.first)(_player.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerDestroyed")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerPart"), _player.mLgObj, header);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleDestroyed(int32_t vehicle, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleDestroyed(%d, %d, %s)", vehicle, header, NULL_SQOBJ_(payload))
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle));
    (*_vehicle.mOnDestroyed.first)(header, payload);
    (*mOnVehicleDestroyed.first)(_vehicle.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleDestroyed")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitBlipCustom(int32_t blip, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::BlipCustom(%d, %d, %s)", blip, header, NULL_SQOBJ_(payload))
    BlipInst & _blip = m_Blips.at(static_cast< size_t >(blip));
    (*_blip.mOnCustom.first)(header, payload);
    (*mOnBlipCustom.first)(_blip.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::BlipCustom")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointCustom(int32_t checkpoint, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::CheckpointCustom(%d, %d, %s)", checkpoint, header, NULL_SQOBJ_(payload))
    CheckpointInst & _checkpoint = m_Checkpoints.at(static_cast< size_t >(checkpoint));
    (*_checkpoint.mOnCustom.first)(header, payload);
    (*mOnCheckpointCustom.first)(_checkpoint.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::CheckpointCustom")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitKeyBindCustom(int32_t keybind, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::KeyBindCustom(%d, %d, %s)", keybind, header, NULL_SQOBJ_(payload))
    KeyBindInst & _keybind = m_KeyBinds.at(static_cast< size_t >(keybind));
    (*_keybind.mOnCustom.first)(header, payload);
    (*mOnKeyBindCustom.first)(_keybind.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::KeyBindCustom")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectCustom(int32_t object, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ObjectCustom(%d, %d, %s)", object, header, NULL_SQOBJ_(payload))
    ObjectInst & _object = m_Objects.at(static_cast< size_t >(object));
    (*_object.mOnCustom.first)(header, payload);
    (*mOnObjectCustom.first)(_object.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ObjectCustom")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupCustom(int32_t pickup, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupCustom(%d, %d, %s)", pickup, header, NULL_SQOBJ_(payload))
    PickupInst & _pickup = m_Pickups.at(static_cast< size_t >(pickup));
    (*_pickup.mOnCustom.first)(header, payload);
    (*mOnPickupCustom.first)(_pickup.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupCustom")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCustom(int32_t player, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerCustom(%d, %d, %s)", player, header, NULL_SQOBJ_(payload))
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player));
    (*_player.mOnCustom.first)(header, payload);
    (*mOnPlayerCustom.first)(_player.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerCustom")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleCustom(int32_t vehicle, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleCustom(%d, %d, %s)", vehicle, header, NULL_SQOBJ_(payload))
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle));
    (*_vehicle.mOnCustom.first)(header, payload);
    (*mOnVehicleCustom.first)(_vehicle.mObj, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleCustom")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerStartup() const
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ServerStartup()")
    (*mOnServerStartup.first)();
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ServerStartup")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onServerStart"));
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerShutdown() const
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ServerShutdown()")
    (*mOnServerShutdown.first)();
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ServerShutdown")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onServerStop"));
        ExecuteLegacyEvent(m_VM, _SC("onScriptUnload"));
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerFrame(float elapsed_time) const
{
    //SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ServerFrame(%f)", elapsed_time)
    (*mOnServerFrame.first)(elapsed_time);
    //SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ServerFrame")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        const int32_t hour = _Func->GetHour(), minute = _Func->GetMinute();
        // Check for onTimeChange triggers
        if(g_LastHour != hour || g_LastMinute != minute)
        {
            ExecuteLegacyEvent(m_VM, _SC("onTimeChange"), g_LastHour, g_LastMinute, hour, minute);
            // Update values
            g_LastHour = hour;
            g_LastMinute = minute;
        }
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPluginCommand(uint32_t command_identifier, const char * message) // NOLINT(readability-convert-member-functions-to-static)
{
#if defined(_DEBUG) || defined(SQMOD_ENABLE_SV_EV_TRACEBACK)
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PluginCommand(0x%08X, '%s')", command_identifier, message)
    // Ignored for now...
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PluginCommand")
#else
    // avoid unused parameter warnings!
    (void)command_identifier;
    (void)message;
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitIncomingConnection(char * player_name, size_t name_buffer_size, const char * user_password, const char * ip_address)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::IncomingConnection(%s, %" PRINT_SZ_FMT ", %s, %s)", player_name, name_buffer_size, user_password, ip_address)
    // Save the buffer information so that we can write to it from the string
    m_IncomingNameBuffer = player_name;
    m_IncomingNameCapacity = name_buffer_size;
    // Create reusable objects from parameters
    LightObj player_name_obj(player_name, -1);
    LightObj user_password_obj(user_password, -1);
    LightObj ip_address_obj(ip_address, -1);
    // Attempt to forward the event to the script callback
    try
    {
        (*mOnIncomingConnection.first)(player_name_obj, name_buffer_size, user_password_obj, ip_address_obj);
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
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::IncomingConnection")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        LightObj r = EvaluateLegacyEvent(m_VM, _SC("onLoginAttempt"), player_name_obj, user_password_obj, ip_address_obj);
        SetState(r.IsNull() ? 1 : r.Cast< int32_t >());
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerRequestClass(int32_t player_id, int32_t offset)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerRequestClass(%d, %d)", player_id, offset)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnRequestClass.first)(offset);
    (*mOnPlayerRequestClass.first)(_player.mObj, offset);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerRequestClass")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        LightObj r = EvaluateLegacyEvent(m_VM, _SC("onPlayerRequestClass"),
                        _player.mLgObj, offset, _Func->GetPlayerTeam(player_id), _Func->GetPlayerSkin(player_id));
        SetState(r.IsNull() ? 1 : r.Cast< int32_t >());
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerRequestSpawn(int32_t player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerRequestSpawn(%d)", player_id)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnRequestSpawn.first)();
    (*mOnPlayerRequestSpawn.first)(_player.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerRequestSpawn")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerRequestSpawn"), _player.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerSpawn(int32_t player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerSpawn(%d)", player_id)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnSpawn.first)();
    (*mOnPlayerSpawn.first)(_player.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerSpawn")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerSpawn"), _player.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerWasted(int32_t player_id, int32_t reason, vcmpBodyPart SQ_UNUSED_ARG(body_part))
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerWasted(%d, %d)", player_id, reason)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnWasted.first)(reason);
    (*mOnPlayerWasted.first)(_player.mObj, reason);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerWasted")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        if (reason == 43 || reason == 50) reason = 43; // drowned
        else if (reason == 39 && body_part == 7) reason = 39; // car crash
        else if (reason == 39 || reason == 40 || reason == 44) reason = 44; // fell
        ExecuteLegacyEvent(m_VM, _SC("onPlayerDeath"), _player.mLgObj, reason);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerKilled(int32_t player_id, int32_t killer_id, int32_t reason, vcmpBodyPart body_part, bool team_kill)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerKilled(%d, %d, %d, %d, %d)", player_id, killer_id, reason, body_part, team_kill)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    PlayerInst & _killer = m_Players.at(static_cast< size_t >(killer_id));
    (*_player.mOnKilled.first)(_killer.mObj, reason, static_cast< int32_t >(body_part), team_kill);
    (*mOnPlayerKilled.first)(_player.mObj, _killer.mObj, reason, static_cast< int32_t >(body_part), team_kill);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerKilled")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        if (!team_kill)
        {
            ExecuteLegacyEvent(m_VM, _SC("onPlayerTeamKill"), _killer.mLgObj, _player.mLgObj, reason, static_cast< int32_t >(body_part));
        }
        else
        {
            ExecuteLegacyEvent(m_VM, _SC("onPlayerKill"), _killer.mLgObj, _player.mLgObj, reason, static_cast< int32_t >(body_part));
        }
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerEmbarking(int32_t player_id, int32_t vehicle_id, int32_t slot_index)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerEmbarking(%d, %d, %d)", player_id, vehicle_id, slot_index)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_player.mOnEmbarking.first)(_vehicle.mObj, slot_index);
    (*_vehicle.mOnEmbarking.first)(_player.mObj, slot_index);
    (*mOnPlayerEmbarking.first)(_player.mObj, _vehicle.mObj, slot_index);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerEmbarking")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        LightObj r = EvaluateLegacyEvent(m_VM, _SC("onPlayerEnteringVehicle"),
                        _player.mLgObj, _vehicle.mLgObj, slot_index);
        SetState(r.IsNull() ? 1 : r.Cast< int32_t >());
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerEmbarked(int32_t player_id, int32_t vehicle_id, int32_t slot_index)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerEmbarked(%d, %d, %d)", player_id, vehicle_id, slot_index)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_player.mOnEmbarked.first)(_vehicle.mObj, slot_index);
    (*_vehicle.mOnEmbarked.first)(_player.mObj, slot_index);
    (*mOnPlayerEmbarked.first)(_player.mObj, _vehicle.mObj, slot_index);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerEmbarked")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerEnterVehicle"), _player.mLgObj, _vehicle.mLgObj, slot_index);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerDisembark(int32_t player_id, int32_t vehicle_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerDisembark(%d, %d)", player_id, vehicle_id)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_player.mOnDisembark.first)(_vehicle.mObj);
    (*_vehicle.mOnDisembark.first)(_player.mObj);
    (*mOnPlayerDisembark.first)(_player.mObj, _vehicle.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerDisembark")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerExitVehicle"), _player.mLgObj, _vehicle.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerRename(int32_t player_id, const char * old_name, const char * new_name)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerRename(%d, %s, %s)", player_id, old_name, new_name)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    LightObj oname(old_name, -1), nname(new_name, -1);
    (*_player.mOnRename.first)(oname, nname);
    (*mOnPlayerRename.first)(_player.mObj, oname, nname);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerRename")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerNameChange"), _player.mLgObj, oname, nname);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerState(int32_t player_id, int32_t old_state, int32_t new_state)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerState(%d, %d, %d)", player_id, old_state, new_state)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnState.first)(old_state, new_state);
    (*mOnPlayerState.first)(_player.mObj, old_state, new_state);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerState")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerActionChange"), _player.mLgObj, old_state, new_state);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateNone(int32_t player_id, int32_t old_state)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::StateNone(%d, %d)", player_id, old_state)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnStateNone.first)(old_state);
    (*mOnStateNone.first)(_player.mObj, old_state);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::StateNone")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateNormal(int32_t player_id, int32_t old_state)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::StateNormal(%d, %d)", player_id, old_state)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnStateNormal.first)(old_state);
    (*mOnStateNormal.first)(_player.mObj, old_state);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::StateNormal")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateAim(int32_t player_id, int32_t old_state)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::StateAim(%d, %d)", player_id, old_state)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnStateAim.first)(old_state);
    (*mOnStateAim.first)(_player.mObj, old_state);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::StateAim")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateDriver(int32_t player_id, int32_t old_state)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::StateDriver(%d, %d)", player_id, old_state)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnStateDriver.first)(old_state);
    (*mOnStateDriver.first)(_player.mObj, old_state);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::StateDriver")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStatePassenger(int32_t player_id, int32_t old_state)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::StatePassenger(%d, %d)", player_id, old_state)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnStatePassenger.first)(old_state);
    (*mOnStatePassenger.first)(_player.mObj, old_state);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::StatePassenger")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateEnterDriver(int32_t player_id, int32_t old_state)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::StateEnterDriver(%d, %d)", player_id, old_state)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnStateEnterDriver.first)(old_state);
    (*mOnStateEnterDriver.first)(_player.mObj, old_state);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::StateEnterDriver")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateEnterPassenger(int32_t player_id, int32_t old_state)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::StateEnterPassenger(%d, %d)", player_id, old_state)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnStateEnterPassenger.first)(old_state);
    (*mOnStateEnterPassenger.first)(_player.mObj, old_state);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::StateEnterPassenger")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateExit(int32_t player_id, int32_t old_state)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::StateExit(%d, %d)", player_id, old_state)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnStateExit.first)(old_state);
    (*mOnStateExit.first)(_player.mObj, old_state);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::StateExit")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitStateUnspawned(int32_t player_id, int32_t old_state)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::StateUnspawned(%d, %d)", player_id, old_state)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnStateUnspawned.first)(old_state);
    (*mOnStateUnspawned.first)(_player.mObj, old_state);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::StateUnspawned")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerAction(int32_t player_id, int32_t old_action, int32_t new_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerAction(%d, %d, %d)", player_id, old_action, new_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnAction.first)(old_action, new_action);
    (*mOnPlayerAction.first)(_player.mObj, old_action, new_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerAction")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerActionChange"), _player.mLgObj, old_action, new_action);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionNone(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionNone(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionNone.first)(old_action);
    (*mOnActionNone.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionNone")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionNormal(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionNormal(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionNormal.first)(old_action);
    (*mOnActionNormal.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionNormal")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionAiming(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionAiming(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionAiming.first)(old_action);
    (*mOnActionAiming.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionAiming")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionShooting(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionShooting(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionShooting.first)(old_action);
    (*mOnActionShooting.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionShooting")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionJumping(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionJumping(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionJumping.first)(old_action);
    (*mOnActionJumping.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionJumping")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionLieDown(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionLieDown(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionLieDown.first)(old_action);
    (*mOnActionLieDown.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionLieDown")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionGettingUp(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionGettingUp(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionGettingUp.first)(old_action);
    (*mOnActionGettingUp.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionGettingUp")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionJumpVehicle(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionJumpVehicle(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionJumpVehicle.first)(old_action);
    (*mOnActionJumpVehicle.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionJumpVehicle")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionDriving(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionDriving(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionDriving.first)(old_action);
    (*mOnActionDriving.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionDriving")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionDying(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionDying(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionDying.first)(old_action);
    (*mOnActionDying.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionDying")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionWasted(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionWasted(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionWasted.first)(old_action);
    (*mOnActionWasted.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionWasted")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionEmbarking(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionEmbarking(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionEmbarking.first)(old_action);
    (*mOnActionEmbarking.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionEmbarking")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitActionDisembarking(int32_t player_id, int32_t old_action)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ActionDisembarking(%d, %d)", player_id, old_action)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnActionDisembarking.first)(old_action);
    (*mOnActionDisembarking.first)(_player.mObj, old_action);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ActionDisembarking")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerBurning(int32_t player_id, bool is_on_fire)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerBurning(%d, %d)", player_id, is_on_fire)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnBurning.first)(is_on_fire);
    (*mOnPlayerBurning.first)(_player.mObj, is_on_fire);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerBurning")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerOnFireChange"), _player.mLgObj, is_on_fire);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCrouching(int32_t player_id, bool is_crouching)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerCrouching(%d, %d)", player_id, is_crouching)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnCrouching.first)(is_crouching);
    (*mOnPlayerCrouching.first)(_player.mObj, is_crouching);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerCrouching")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerCrouchChange"), _player.mLgObj, is_crouching);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerGameKeys(int32_t player_id, uint32_t old_keys, uint32_t new_keys)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerGameKeys(%d, %u, %u)", player_id, old_keys, new_keys)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnGameKeys.first)(old_keys, new_keys);
    (*mOnPlayerGameKeys.first)(_player.mObj, old_keys, new_keys);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerGameKeys")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerGameKeysChange"), _player.mLgObj, old_keys, new_keys);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerStartTyping(int32_t player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerStartTyping(%d)", player_id)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnStartTyping.first)();
    (*mOnPlayerStartTyping.first)(_player.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerStartTyping")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerBeginTyping"), _player.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerStopTyping(int32_t player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerStopTyping(%d)", player_id)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnStopTyping.first)();
    (*mOnPlayerStopTyping.first)(_player.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerStopTyping")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerEndTyping"), _player.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerAway(int32_t player_id, bool is_away)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerAway(%d, %d)", player_id, is_away)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnAway.first)(is_away);
    (*mOnPlayerAway.first)(_player.mObj, is_away);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerAway")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerAwayChange"), _player.mLgObj, is_away);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerMessage(int32_t player_id, const char * message)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerMessage(%d, %s)", player_id, message)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    LightObj msg(message, -1);
    (*_player.mOnMessage.first)(msg);
    (*mOnPlayerMessage.first)(_player.mObj, msg);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerMessage")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        LightObj r = EvaluateLegacyEvent(m_VM, _SC("onPlayerChat"), _player.mLgObj, msg);
        SetState(r.IsNull() ? 1 : r.Cast< int32_t >());
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCommand(int32_t player_id, const char * message)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerCommand(%d, %s)", player_id, message)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    LightObj msg(message, -1);
    (*_player.mOnCommand.first)(msg);
    (*mOnPlayerCommand.first)(_player.mObj, msg);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerCommand")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        LightObj text, args;
        // Find the first space character
        const char * split = std::strchr(message, ' ');
        // Do we need to split the command from arguments?
        if (split)
        {
            // Create a string object for the command (don't include the space character)
            text = LightObj(message, static_cast< SQInteger >(split - message));
            // Do we need to create a script object for arguments?
            if (std::strlen(split + 1) > 0)
            {
                args = LightObj(split + 1, -1); // Skip the space character
            }
        }
        else
        {
            text = std::move(msg); // Use the existing message object as is
        }
        LightObj r = EvaluateLegacyEvent(m_VM, _SC("onPlayerCommand"), _player.mLgObj, text, args);
        SetState(r.IsNull() ? 1 : r.Cast< int32_t >());
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerPrivateMessage(int32_t player_id, int32_t target_player_id, const char * message)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerPrivateMessage(%d, %d, %s)", player_id, target_player_id, message)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    PlayerInst & _receiver = m_Players.at(static_cast< size_t >(target_player_id));
    LightObj msg(message, -1);
    (*_player.mOnMessage.first)(_receiver.mObj,  msg);
    (*mOnPlayerPrivateMessage.first)(_player.mObj, _receiver.mObj, msg);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerPrivateMessage")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        LightObj r = EvaluateLegacyEvent(m_VM, _SC("onPlayerPM"), _player.mLgObj, _receiver.mLgObj, msg);
        SetState(r.IsNull() ? 1 : r.Cast< int32_t >());
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerKeyPress(int32_t player_id, int32_t bind_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerKeyPress(%d, %d)", player_id, bind_id)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    KeyBindInst & _keybind = m_KeyBinds.at(static_cast< size_t >(bind_id));
    (*_player.mOnKeyPress.first)(_keybind.mObj);
    (*_keybind.mOnKeyPress.first)(_player.mObj);
    (*mOnPlayerKeyPress.first)(_player.mObj, _keybind.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerKeyPress")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onKeyDown"), _player.mLgObj, bind_id);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerKeyRelease(int32_t player_id, int32_t bind_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerKeyRelease(%d, %d)", player_id, bind_id)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    KeyBindInst & _keybind = m_KeyBinds.at(static_cast< size_t >(bind_id));
    (*_keybind.mOnKeyRelease.first)(_player.mObj);
    (*_player.mOnKeyRelease.first)(_keybind.mObj);
    (*mOnPlayerKeyRelease.first)(_player.mObj, _keybind.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerKeyRelease")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onKeyUp"), _player.mLgObj, bind_id);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerSpectate(int32_t player_id, int32_t target_player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerSpectate(%d, %d)", player_id, target_player_id)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    PlayerInst & _target = m_Players.at(static_cast< size_t >(target_player_id));
    (*_player.mOnSpectate.first)(_target.mObj);
    (*mOnPlayerSpectate.first)(_player.mObj, _target.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerSpectate")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerSpectate"), _player.mLgObj, _target.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerUnspectate(int32_t player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerUnspectate(%d)", player_id)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnUnspectate.first)();
    (*mOnPlayerUnspectate.first)(_player.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerUnspectate")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerCrashReport(int32_t player_id, const char * report)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerCrashreport(%d, %s)", player_id, report)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    LightObj report_obj(report, -1);
    (*_player.mOnCrashReport.first)(report_obj);
    (*mOnPlayerCrashReport.first)(_player.mObj, report_obj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerCrashreport")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerCrashDump"), _player.mLgObj, report_obj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerModuleList(int32_t player_id, const char * list)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerModuleList(%d, %s)", player_id, list)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    LightObj list_obj(list, -1);
    (*_player.mOnModuleList.first)(list_obj);
    (*mOnPlayerModuleList.first)(_player.mObj, list_obj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerModuleList")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerModuleList"), _player.mLgObj, list_obj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleExplode(int32_t vehicle_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleExplode(%d)", vehicle_id)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnExplode.first)();
    (*mOnVehicleExplode.first)(_vehicle.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleExplode")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onVehicleExplode"), _vehicle.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleRespawn(int32_t vehicle_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleRespawn(%d)", vehicle_id)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnRespawn.first)();
    (*mOnVehicleRespawn.first)(_vehicle.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleRespawn")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onVehicleRespawn"), _vehicle.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectShot(int32_t object_id, int32_t player_id, int32_t weapon_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ObjectShot(%d, %d, %d)", object_id, player_id, weapon_id)
    ObjectInst & _object = m_Objects.at(static_cast< size_t >(object_id));
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_object.mOnShot.first)(_player.mObj, weapon_id);
    (*_player.mOnObjectShot.first)(_object.mObj, weapon_id);
    (*mOnObjectShot.first)(_player.mObj, _object.mObj, weapon_id);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ObjectShot")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onObjectShot"), _object.mLgObj, _player.mLgObj, weapon_id);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectTouched(int32_t object_id, int32_t player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ObjectTouched(%d, %d)", object_id, player_id)
    ObjectInst & _object = m_Objects.at(static_cast< size_t >(object_id));
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_object.mOnTouched.first)(_player.mObj);
    (*_player.mOnObjectTouched.first)(_object.mObj);
    (*mOnObjectTouched.first)(_player.mObj, _object.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ObjectTouched")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onObjectBump"), _object.mLgObj, _player.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupClaimed(int32_t pickup_id, int32_t player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupClaimed(%d, %d)", pickup_id, player_id)
    PickupInst & _pickup = m_Pickups.at(static_cast< size_t >(pickup_id));
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_pickup.mOnClaimed.first)(_player.mObj);
    (*_player.mOnPickupClaimed.first)(_pickup.mObj);
    (*mOnPickupClaimed.first)(_player.mObj, _pickup.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupClaimed")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        LightObj r = EvaluateLegacyEvent(m_VM, _SC("onPickupClaimPicked"), _player.mLgObj, _pickup.mLgObj);
        SetState(r.IsNull() ? 1 : r.Cast< int32_t >());
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupCollected(int32_t pickup_id, int32_t player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupCollected(%d, %d)", pickup_id, player_id)
    PickupInst & _pickup = m_Pickups.at(static_cast< size_t >(pickup_id));
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_pickup.mOnCollected.first)(_player.mObj);
    (*_player.mOnPickupCollected.first)(_pickup.mObj);
    (*mOnPickupCollected.first)(_player.mObj, _pickup.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupCollected")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPickupPickedUp"), _player.mLgObj, _pickup.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupRespawn(int32_t pickup_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupRespawn(%d)", pickup_id)
    PickupInst & _pickup = m_Pickups.at(static_cast< size_t >(pickup_id));
    (*_pickup.mOnRespawn.first)();
    (*mOnPickupRespawn.first)(_pickup.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupRespawn")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPickupRespawn"), _pickup.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointEntered(int32_t checkpoint_id, int32_t player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::CheckpointEntered(%d, %d)", checkpoint_id, player_id)
    CheckpointInst & _checkpoint = m_Checkpoints.at(static_cast< size_t >(checkpoint_id));
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_checkpoint.mOnEntered.first)(_player.mObj);
    (*_player.mOnCheckpointEntered.first)(_checkpoint.mObj);
    (*mOnCheckpointEntered.first)(_player.mObj, _checkpoint.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::CheckpointEntered")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onCheckpointEntered"), _player.mLgObj, _checkpoint.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointExited(int32_t checkpoint_id, int32_t player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::CheckpointExited(%d, %d)", checkpoint_id, player_id)
    CheckpointInst & _checkpoint = m_Checkpoints.at(static_cast< size_t >(checkpoint_id));
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_checkpoint.mOnExited.first)(_player.mObj);
    (*_player.mOnCheckpointExited.first)(_checkpoint.mObj);
    (*mOnCheckpointExited.first)(_player.mObj, _checkpoint.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::CheckpointExited")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onCheckpointExited"), _player.mLgObj, _checkpoint.mLgObj);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointWorld(int32_t checkpoint_id, int32_t old_world, int32_t new_world)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::CheckpointWorld(%d, %d, %d)", checkpoint_id, old_world, new_world)
    CheckpointInst & _checkpoint = m_Checkpoints.at(static_cast< size_t >(checkpoint_id));
    (*_checkpoint.mOnWorld.first)(old_world, new_world);
    (*mOnCheckpointWorld.first)(_checkpoint.mObj, old_world, new_world);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::CheckpointWorld")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointRadius(int32_t checkpoint_id, float old_radius, float new_radius)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::CheckpointRadius(%d, %f, %f)", checkpoint_id, old_radius, new_radius)
    CheckpointInst & _checkpoint = m_Checkpoints.at(static_cast< size_t >(checkpoint_id));
    (*_checkpoint.mOnRadius.first)(old_radius, new_radius);
    (*mOnCheckpointRadius.first)(_checkpoint.mObj, old_radius, new_radius);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::CheckpointRadius")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectWorld(int32_t object_id, int32_t old_world, int32_t new_world)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ObjectWorld(%d, %d, %d)", object_id, old_world, new_world)
    ObjectInst & _object = m_Objects.at(static_cast< size_t >(object_id));
    (*_object.mOnWorld.first)(old_world, new_world);
    (*mOnObjectWorld.first)(_object.mObj, old_world, new_world);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ObjectWorld")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectAlpha(int32_t object_id, int32_t old_alpha, int32_t new_alpha, int32_t time)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ObjectAlpha(%d, %d, %d, %d)", object_id, old_alpha, new_alpha, time)
    ObjectInst & _object = m_Objects.at(static_cast< size_t >(object_id));
    (*_object.mOnAlpha.first)(old_alpha, new_alpha, time);
    (*mOnObjectAlpha.first)(_object.mObj, old_alpha, new_alpha, time);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ObjectAlpha")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupWorld(int32_t pickup_id, int32_t old_world, int32_t new_world)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupWorld(%d, %d, %d)", pickup_id, old_world, new_world)
    PickupInst & _pickup = m_Pickups.at(static_cast< size_t >(pickup_id));
    (*_pickup.mOnWorld.first)(old_world, new_world);
    (*mOnPickupWorld.first)(_pickup.mObj, old_world, new_world);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupWorld")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupAlpha(int32_t pickup_id, int32_t old_alpha, int32_t new_alpha)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupAlpha(%d, %d, %d)", pickup_id, old_alpha, new_alpha)
    PickupInst & _pickup = m_Pickups.at(static_cast< size_t >(pickup_id));
    (*_pickup.mOnAlpha.first)(old_alpha, new_alpha);
    (*mOnPickupAlpha.first)(_pickup.mObj, old_alpha, new_alpha);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupAlpha")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupAutomatic(int32_t pickup_id, bool old_status, bool new_status)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupAutomatic(%d, %d, %d)", pickup_id, old_status, new_status)
    PickupInst & _pickup = m_Pickups.at(static_cast< size_t >(pickup_id));
    (*_pickup.mOnAutomatic.first)(old_status, new_status);
    (*mOnPickupAutomatic.first)(_pickup.mObj, old_status, new_status);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupAutomatic")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupAutoTimer(int32_t pickup_id, int32_t old_timer, int32_t new_timer)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupAutoTimer(%d, %d, %d)", pickup_id, old_timer, new_timer)
    PickupInst & _pickup = m_Pickups.at(static_cast< size_t >(pickup_id));
    (*_pickup.mOnAutoTimer.first)(old_timer, new_timer);
    (*mOnPickupAutoTimer.first)(_pickup.mObj, old_timer, new_timer);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupAutoTimer")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPickupOption(int32_t pickup_id, int32_t option_id, bool value,
                                int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupOption(%d, %d, %d, %d, %s)", pickup_id, option_id, value, header, NULL_SQOBJ_(payload))
    PickupInst & _pickup = m_Pickups.at(static_cast< size_t >(pickup_id));
    (*_pickup.mOnOption.first)(option_id, value, header, payload);
    (*mOnPickupOption.first)(_pickup.mObj, option_id, value, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupOption")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitObjectReport(int32_t object_id, bool old_status, bool new_status, bool touched)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ObjectReport(%d, %d, %d)", object_id, new_status, touched)
    ObjectInst & _object = m_Objects.at(static_cast< size_t >(object_id));
    (*_object.mOnReport.first)(old_status, new_status, touched);
    (*mOnObjectReport.first)(_object.mObj, old_status, new_status, touched);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ObjectReport")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerHealth(int32_t player_id, float old_health, float new_health)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerHealth(%d, %f, %f)", player_id, old_health, new_health)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnHealth.first)(old_health, new_health);
    (*mOnPlayerHealth.first)(_player.mObj, old_health, new_health);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerHealth")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerHealthChange"), _player.mLgObj, old_health, new_health);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerArmour(int32_t player_id, float old_armour, float new_armour)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerArmour(%d, %f, %f)", player_id, old_armour, new_armour)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnArmour.first)(old_armour, new_armour);
    (*mOnPlayerArmour.first)(_player.mObj, old_armour, new_armour);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerArmour")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerArmourChange"), _player.mLgObj, old_armour, new_armour);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerWeapon(int32_t player_id, int32_t old_weapon, int32_t new_weapon)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerWeapon(%d, %d, %d)", player_id, old_weapon, new_weapon)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnWeapon.first)(old_weapon, new_weapon);
    (*mOnPlayerWeapon.first)(_player.mObj, old_weapon, new_weapon);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerWeapon")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onPlayerWeaponChange"), _player.mLgObj, old_weapon, new_weapon);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerHeading(int32_t player_id, float old_heading, float new_heading)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerHeading(%d, %f, %f)", player_id, old_heading, new_heading)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnHeading.first)(old_heading, new_heading);
    (*mOnPlayerHeading.first)(_player.mObj, old_heading, new_heading);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerHeading")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerPosition(int32_t player_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerPosition(%d)", player_id)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnPosition.first)(_player.mTrackPositionHeader, _player.mTrackPositionPayload);
    (*mOnPlayerPosition.first)(_player.mObj, _player.mTrackPositionHeader, _player.mTrackPositionPayload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerPosition")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        Vector3 pos;
        _Func->GetPlayerPosition(player_id, &pos.x, &pos.y, &pos.z);
        ExecuteLegacyEvent(m_VM, _SC("onPlayerMove"), _player.mLgObj
            , _player.mLastPosition.x, _player.mLastPosition.y, _player.mLastPosition.z
            , pos.x, pos.y, pos.z);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerOption(int32_t player_id, int32_t option_id, bool value,
                                int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerOption(%d, %d, %d, %d, %s)", player_id, option_id, value, header, NULL_SQOBJ_(payload))
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnOption.first)(option_id, value, header, payload);
    (*mOnPlayerOption.first)(_player.mObj, option_id, value, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerOption")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerAdmin(int32_t player_id, bool old_status, bool new_status)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerAdmin(%d, %d, %d)", player_id, old_status, new_status)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnAdmin.first)(old_status, new_status);
    (*mOnPlayerAdmin.first)(_player.mObj, old_status, new_status);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerAdmin")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerWorld(int32_t player_id, int32_t old_world, int32_t new_world, bool secondary)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerWorld(%d, %d, %d, %d)", player_id, old_world, new_world, secondary)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnWorld.first)(old_world, new_world, secondary);
    (*mOnPlayerWorld.first)(_player.mObj, old_world, new_world, secondary);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerWorld")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerTeam(int32_t player_id, int32_t old_team, int32_t new_team)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerTeam(%d, %d, %d)", player_id, old_team, new_team)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnTeam.first)(old_team, new_team);
    (*mOnPlayerTeam.first)(_player.mObj, old_team, new_team);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerTeam")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerSkin(int32_t player_id, int32_t old_skin, int32_t new_skin)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerSkin(%d, %d, %d)", player_id, old_skin, new_skin)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnSkin.first)(old_skin, new_skin);
    (*mOnPlayerSkin.first)(_player.mObj, old_skin, new_skin);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerSkin")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerMoney(int32_t player_id, int32_t old_money, int32_t new_money)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerMoney(%d, %d, %d)", player_id, old_money, new_money)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnMoney.first)(old_money, new_money);
    (*mOnPlayerMoney.first)(_player.mObj, old_money, new_money);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerMoney")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerScore(int32_t player_id, int32_t old_score, int32_t new_score)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerScore(%d, %d, %d)", player_id, old_score, new_score)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnScore.first)(old_score, new_score);
    (*mOnPlayerScore.first)(_player.mObj, old_score, new_score);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerScore")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerWantedLevel(int32_t player_id, int32_t old_level, int32_t new_level)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerWantedLevel(%d, %d, %d)", player_id, old_level, new_level)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnWantedLevel.first)(old_level, new_level);
    (*mOnPlayerWantedLevel.first)(_player.mObj, old_level, new_level);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerWantedLevel")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerImmunity(int32_t player_id, int32_t old_immunity, int32_t new_immunity)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerImmunity(%d, %d, %d)", player_id, old_immunity, new_immunity)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnImmunity.first)(old_immunity, new_immunity);
    (*mOnPlayerImmunity.first)(_player.mObj, old_immunity, new_immunity);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerImmunity")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerAlpha(int32_t player_id, int32_t old_alpha, int32_t new_alpha, int32_t fade)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerAlpha(%d, %d, %d, %d)", player_id, old_alpha, new_alpha, fade)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnAlpha.first)(old_alpha, new_alpha, fade);
    (*mOnPlayerAlpha.first)(_player.mObj, old_alpha, new_alpha, fade);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerAlpha")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerEnterArea(int32_t player_id, LightObj & area_obj)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerEnterArea(%d, %s)", player_id, NULL_SQOBJ_(area_obj))
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnEnterArea.first)(area_obj);
    (*mOnPlayerEnterArea.first)(_player.mObj, area_obj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerEnterArea")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerLeaveArea(int32_t player_id, LightObj & area_obj)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerLeaveArea(%d, %s)", player_id, NULL_SQOBJ_(area_obj))
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    (*_player.mOnLeaveArea.first)(area_obj);
    (*mOnPlayerLeaveArea.first)(_player.mObj, area_obj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerLeaveArea")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleColor(int32_t vehicle_id, int32_t changed)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleColor(%d, %d)", vehicle_id, changed)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnColor.first)(changed);
    (*mOnVehicleColor.first)(_vehicle.mObj, changed);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleColor")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleHealth(int32_t vehicle_id, float old_health, float new_health)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleHealth(%d, %f, %f)", vehicle_id, old_health, new_health)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnHealth.first)(old_health, new_health);
    (*mOnVehicleHealth.first)(_vehicle.mObj, old_health, new_health);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleHealth")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onVehicleHealthChange"), _vehicle.mLgObj, old_health, new_health);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehiclePosition(int32_t vehicle_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehiclePosition(%d)", vehicle_id)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnPosition.first)();
    (*mOnVehiclePosition.first)(_vehicle.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehiclePosition")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        Vector3 pos;
        _Func->GetVehiclePosition(vehicle_id, &pos.x, &pos.y, &pos.z);
        ExecuteLegacyEvent(m_VM, _SC("onVehicleMove"), _vehicle.mLgObj
            , _vehicle.mLastPosition.x, _vehicle.mLastPosition.y, _vehicle.mLastPosition.z
            , pos.x, pos.y, pos.z);
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleRotation(int32_t vehicle_id)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleRotation(%d)", vehicle_id)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnRotation.first)();
    (*mOnVehicleRotation.first)(_vehicle.mObj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleRotation")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleOption(int32_t vehicle_id, int32_t option_id, bool value,
                                int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleOption(%d, %d, %d, %d, %s)", vehicle_id, option_id, value, header, NULL_SQOBJ_(payload))
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnOption.first)(option_id, value, header, payload);
    (*mOnVehicleOption.first)(_vehicle.mObj, option_id, value, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleOption")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleWorld(int32_t vehicle_id, int32_t old_world, int32_t new_world)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleWorld(%d, %d, %d)", vehicle_id, old_world, new_world)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnWorld.first)(old_world, new_world);
    (*mOnVehicleWorld.first)(_vehicle.mObj, old_world, new_world);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleWorld")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleImmunity(int32_t vehicle_id, int32_t old_immunity, int32_t new_immunity)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleImmunity(%d, %d, %d)", vehicle_id, old_immunity, new_immunity)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnImmunity.first)(old_immunity, new_immunity);
    (*mOnVehicleImmunity.first)(_vehicle.mObj, old_immunity, new_immunity);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleImmunity")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehiclePartStatus(int32_t vehicle_id, int32_t part, int32_t old_status, int32_t new_status)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehiclePartStatus(%d, %d, %d, %d)", vehicle_id, part, old_status, new_status)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnPartStatus.first)(part, old_status, new_status);
    (*mOnVehiclePartStatus.first)(_vehicle.mObj, part, old_status, new_status);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehiclePartStatus")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleTyreStatus(int32_t vehicle_id, int32_t tyre, int32_t old_status, int32_t new_status)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleTyreStatus(%d, %d, %d, %d)", vehicle_id, tyre, old_status, new_status)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnTyreStatus.first)(tyre, old_status, new_status);
    (*mOnVehicleTyreStatus.first)(_vehicle.mObj, tyre, old_status, new_status);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleTyreStatus")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleDamageData(int32_t vehicle_id, uint32_t old_data, uint32_t new_data)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleDamageData(%d, %u, %u)", vehicle_id, old_data, new_data)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnDamageData.first)(old_data, new_data);
    (*mOnVehicleDamageData.first)(_vehicle.mObj, old_data, new_data);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleDamageData")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleRadio(int32_t vehicle_id, int32_t old_radio, int32_t new_radio)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleRadio(%d, %d, %d)", vehicle_id, old_radio, new_radio)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnRadio.first)(old_radio, new_radio);
    (*mOnVehicleRadio.first)(_vehicle.mObj, old_radio, new_radio);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleRadio")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleHandlingRule(int32_t vehicle_id, int32_t rule, SQFloat old_data, SQFloat new_data)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleHandlingRule(%d, %d, %f, %f)", vehicle_id, rule, old_data, new_data)
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnHandlingRule.first)(rule, old_data, new_data);
    (*mOnVehicleHandlingRule.first)(_vehicle.mObj, rule, old_data, new_data);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleHandlingRule")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleEnterArea(int32_t vehicle_id, LightObj & area_obj)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleEnterArea(%d, %s)", vehicle_id, NULL_SQOBJ_(area_obj))
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnEnterArea.first)(area_obj);
    (*mOnVehicleEnterArea.first)(_vehicle.mObj, area_obj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleEnterArea")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleLeaveArea(int32_t vehicle_id, LightObj & area_obj)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleLeaveArea(%d, %s)", vehicle_id, NULL_SQOBJ_(area_obj))
    VehicleInst & _vehicle = m_Vehicles.at(static_cast< size_t >(vehicle_id));
    (*_vehicle.mOnLeaveArea.first)(area_obj);
    (*mOnVehicleLeaveArea.first)(_vehicle.mObj, area_obj);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleLeaveArea")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitServerOption(int32_t option, bool value, int32_t header, LightObj & payload)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ServerOption(%d, %d, %d, %s)", option, value, header, NULL_SQOBJ_(payload))
    if (m_CircularLocks & CCL_EMIT_SERVER_OPTION)
    {
        return; // Already inside this event!
    }
    // Prevent further calls to this event
    BitGuardU32 bg(m_CircularLocks, CCL_EMIT_SERVER_OPTION);
    // Now forward the event call
    (*mOnServerOption.first)(option, value, header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ServerOption")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitScriptReload(int32_t header, LightObj & payload) const
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ScriptReload(%d, %s)", header, NULL_SQOBJ_(payload))
    (*mOnScriptReload.first)(header, payload);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ScriptReload")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitScriptLoaded() const
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ScriptLoaded()")
    (*mOnScriptLoaded.first)();
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ScriptLoaded")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        ExecuteLegacyEvent(m_VM, _SC("onScriptLoad"));
    }
#endif
}

// ------------------------------------------------------------------------------------------------
void Core::EmitEntityPool(vcmpEntityPool entity_type, int32_t entity_id, bool is_deleted)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::EntityPool(%d, %d, %d)", static_cast<int32_t>(entity_type), entity_id, is_deleted)
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
        //break;
#if SQMOD_SDK_LEAST(2, 1)
        case vcmpEntityPoolPlayer:
            // @TODO Implement...
        //break;
        case vcmpEntityReserved1:
            // @TODO Implement...
        break;
#endif
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
                    STHROWF("Cannot allocate blip with invalid identifier: {}", entity_id);
                }
                // Retrieve the specified entity instance
                BlipInst & inst = m_Blips[entity_id];
                // Make sure that the instance isn't already allocated
                if (VALID_ENTITY(inst.mID))
                {
                    STHROWF("Cannot allocate blip that already exists: {}", entity_id);
                }
                // Information about the blip entity
                int32_t world, scale, sprid;
                uint32_t color;
                float x, y, z;
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
        default:
            LogErr("Unknown change in the entity pool: type %d > entity %d", static_cast<int32_t>(entity_type), entity_id);
    }
    // Finally, forward the event to the script
    (*mOnEntityPool.first)(static_cast< int32_t >(entity_type), entity_id, is_deleted);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::EntityPool")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerUpdate(int32_t player_id, vcmpPlayerUpdate update_type)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerUpdate(%d, %d)", player_id, static_cast<int32_t>(update_type))
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(player_id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Cannot update player with invalid identifier: {}", player_id);
    }
    // Retrieve the associated tracking instance
    PlayerInst & inst = m_Players[player_id];

    // Obtain the current heading of this instance
    float heading = _Func->GetPlayerHeading(player_id);
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
        // Should we check for distance traveled?
        if (inst.mFlags & ENF_DIST_TRACK)
        {
            inst.mDistance += inst.mLastPosition.GetDistanceTo(pos);
        }
        // Should we check for area collision?
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
    float health = _Func->GetPlayerHealth(player_id);
    // Did the health change since the last tracked value?
    if (!EpsEq(health, inst.mLastHealth))
    {
        // Trigger the event specific to this change
        EmitPlayerHealth(player_id, inst.mLastHealth, health);
        // Update the tracked value
        inst.mLastHealth = health;
    }

    // Obtain the current armor of this instance
    float armour = _Func->GetPlayerArmour(player_id);
    // Did the armor change since the last tracked value?
    if (!EpsEq(armour, inst.mLastArmour))
    {
        // Trigger the event specific to this change
        EmitPlayerArmour(player_id, inst.mLastArmour, armour);
        // Update the tracked value
        inst.mLastArmour = armour;
    }

    // Obtain the current weapon of this instance
    int32_t wep = _Func->GetPlayerWeapon(player_id);
    // Did the weapon change since the last tracked value?
    if (wep != inst.mLastWeapon)
    {
        // Trigger the event specific to this change
        EmitPlayerWeapon(player_id, inst.mLastWeapon, wep);
        // Update the tracked value
        inst.mLastWeapon = wep;
    }

    // Finally, forward the call to the update callback
    (*inst.mOnUpdate.first)(static_cast< int32_t >(update_type));
    (*mOnPlayerUpdate.first)(inst.mObj, static_cast< int32_t >(update_type));
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerUpdate")
}
#if SQMOD_SDK_LEAST(2, 1)
// ------------------------------------------------------------------------------------------------
void Core::EmitCheckpointStream(int32_t player_id, int32_t entity_id, bool is_deleted)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::CheckpointStream(%d, %d, %d)", player_id, entity_id, is_deleted)
    CheckpointInst & _checkpoint = m_Checkpoints.at(entity_id);
    if (_checkpoint.mObj.IsNull()) return; // At fisrt call, the entity does not exist!
    PlayerInst & _client = m_Players.at(player_id);
    (*_checkpoint.mOnStream.first)(_client.mObj, is_deleted);
    (*_client.mOnEntityStream.first)(_checkpoint.mObj, static_cast< int32_t >(vcmpEntityPoolCheckPoint), is_deleted);
    (*mOnCheckpointStream.first)(_client.mObj, _checkpoint.mObj, is_deleted);
    (*mOnEntityStream.first)(_client.mObj, _checkpoint.mObj, static_cast< int32_t >(vcmpEntityPoolCheckPoint), is_deleted);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::CheckpointStream")
}
// ------------------------------------------------------------------------------------------------
void Core::EmitObjectStream(int32_t player_id, int32_t entity_id, bool is_deleted)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ObjectStream(%d, %d, %d)", player_id, entity_id, is_deleted)
    ObjectInst & _object = m_Objects.at(entity_id);
    if (_object.mObj.IsNull()) return; // At fisrt call, the entity does not exist!
    PlayerInst & _client = m_Players.at(player_id);
    (*_object.mOnStream.first)(_client.mObj, is_deleted);
    (*_client.mOnEntityStream.first)(_object.mObj, static_cast< int32_t >(vcmpEntityPoolObject), is_deleted);
    (*mOnObjectStream.first)(_client.mObj, _object.mObj, is_deleted);
    (*mOnEntityStream.first)(_client.mObj, _object.mObj, static_cast< int32_t >(vcmpEntityPoolObject), is_deleted);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ObjectStream")
}
// ------------------------------------------------------------------------------------------------
void Core::EmitPickupStream(int32_t player_id, int32_t entity_id, bool is_deleted)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PickupStream(%d, %d, %d)", player_id, entity_id, is_deleted)
    PickupInst & _pickup = m_Pickups.at(entity_id);
    if (_pickup.mObj.IsNull()) return; // At fisrt call, the entity does not exist!
    PlayerInst & _client = m_Players.at(player_id);
    (*_pickup.mOnStream.first)(_client.mObj, is_deleted);
    (*_client.mOnEntityStream.first)(_pickup.mObj, static_cast< int32_t >(vcmpEntityPoolPickup), is_deleted);
    (*mOnPickupStream.first)(_client.mObj, _pickup.mObj, is_deleted);
    (*mOnEntityStream.first)(_client.mObj, _pickup.mObj, static_cast< int32_t >(vcmpEntityPoolPickup), is_deleted);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PickupStream")
}
// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerStream(int32_t player_id, int32_t entity_id, bool is_deleted)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::PlayerStream(%d, %d, %d)", player_id, entity_id, is_deleted)
    PlayerInst & _player = m_Players.at(entity_id);
    if (_player.mObj.IsNull()) return; // At fisrt call, the entity does not exist!
    PlayerInst & _client = m_Players.at(player_id);
    (*_player.mOnStream.first)(_client.mObj, is_deleted);
    (*_client.mOnEntityStream.first)(_player.mObj, static_cast< int32_t >(vcmpEntityPoolPlayer), is_deleted);
    (*mOnPlayerStream.first)(_client.mObj, _player.mObj, is_deleted);
    (*mOnEntityStream.first)(_client.mObj, _player.mObj, static_cast< int32_t >(vcmpEntityPoolPlayer), is_deleted);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::PlayerStream")
}
// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleStream(int32_t player_id, int32_t entity_id, bool is_deleted)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleStream(%d, %d, %d)", player_id, entity_id, is_deleted)
    VehicleInst & _vehicle = m_Vehicles.at(entity_id);
    if (_vehicle.mObj.IsNull()) return; // At fisrt call, the entity does not exist!
    PlayerInst & _client = m_Players.at(player_id);
    (*_vehicle.mOnStream.first)(_client.mObj, is_deleted);
    (*_client.mOnEntityStream.first)(_vehicle.mObj, static_cast< int32_t >(vcmpEntityPoolVehicle), is_deleted);
    (*mOnVehicleStream.first)(_client.mObj, _vehicle.mObj, is_deleted);
    (*mOnEntityStream.first)(_client.mObj, _vehicle.mObj, static_cast< int32_t >(vcmpEntityPoolVehicle), is_deleted);
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleStream")
}
// ------------------------------------------------------------------------------------------------
void Core::EmitEntityStreaming(int32_t player_id, int32_t entity_id, vcmpEntityPool entity_type, bool is_deleted)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::EntityStreaming(%d, %d, %d, %d)", player_id, entity_id, static_cast<int32_t>(entity_type), is_deleted)
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(player_id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Cannot notify player with invalid identifier about streaming: {}", player_id);
    }
    // See what type of entity changed
    switch (entity_type)
    {
        case vcmpEntityPoolVehicle: {
            // Make sure that the specified entity identifier is valid
            if (INVALID_ENTITYEX(entity_id, SQMOD_VEHICLE_POOL))
            {
                STHROWF("Cannot stream vehicle with invalid identifier: {}", entity_id);
            }
            // Forward the event to the dedicated handler
            EmitVehicleStream(player_id, entity_id, is_deleted);
        } break;
        case vcmpEntityPoolObject: {
            // Make sure that the specified entity identifier is valid
            if (INVALID_ENTITYEX(entity_id, SQMOD_OBJECT_POOL))
            {
                STHROWF("Cannot stream object with invalid identifier: {}", entity_id);
            }
            // Forward the event to the dedicated handler
            EmitObjectStream(player_id, entity_id, is_deleted);
        } break;
        case vcmpEntityPoolPickup: {
            // Make sure that the specified entity identifier is valid
            if (INVALID_ENTITYEX(entity_id, SQMOD_PICKUP_POOL))
            {
                STHROWF("Cannot stream pickup with invalid identifier: {}", entity_id);
            }
            // Forward the event to the dedicated handler
            EmitPickupStream(player_id, entity_id, is_deleted);
        } break;
        case vcmpEntityPoolPlayer: {
            // Make sure that the specified entity identifier is valid
            if (INVALID_ENTITYEX(entity_id, SQMOD_PLAYER_POOL))
            {
                STHROWF("Cannot stream player with invalid identifier: {}", entity_id);
            }
            // Forward the event to the dedicated handler
            EmitPlayerStream(player_id, entity_id, is_deleted);
        } break;
        case vcmpEntityPoolCheckPoint: {
            // Make sure that the specified entity identifier is valid
            if (INVALID_ENTITYEX(entity_id, SQMOD_CHECKPOINT_POOL))
            {
                STHROWF("Cannot stream checkpoint with invalid identifier: {}", entity_id);
            }
            // Forward the event to the dedicated handler
            EmitCheckpointStream(player_id, entity_id, is_deleted);
        } break;
        default:
            LogErr("Unknown change in entity streaming: client %d > type %d > entity %d", player_id, entity_type, entity_id);
    }
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::EntityStreaming")
}
#endif
// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleUpdate(int32_t vehicle_id, vcmpVehicleUpdate update_type)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::VehicleUpdate(%d, %d)", vehicle_id, static_cast<int32_t>(update_type))
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(vehicle_id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Cannot update vehicle with invalid identifier: {}", vehicle_id);
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
            // Should we check for distance traveled?
            if (inst.mFlags & ENF_DIST_TRACK)
            {
                inst.mDistance += inst.mLastPosition.GetDistanceTo(pos);
            }
            // Should we check for area collision?
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
            float health = _Func->GetVehicleHealth(vehicle_id);
            // Trigger the event specific to this change
            EmitVehicleHealth(vehicle_id, inst.mLastHealth, health);
            // Update the tracked value
            inst.mLastHealth = health;
        } break;
        case vcmpVehicleUpdateColour:
        {
            int32_t primary, secondary;
            // Obtain the current colors of this instance
            _Func->GetVehicleColour(vehicle_id, &primary, &secondary);
            // Which colors changed
            int32_t changed = 0;
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
            // Obtain the current health of this instance
            float health = _Func->GetVehicleHealth(vehicle_id);
            // Server is actually dumb and never triggers vcmpVehicleUpdateHealth
            if (!EpsEq(health, inst.mLastHealth))
            {
                // Trigger the event specific to this change
                EmitVehicleHealth(vehicle_id, inst.mLastHealth, health);
                // Update the tracked value
                inst.mLastHealth = health;
            }
            // Finally, forward the call to the update callback
            (*inst.mOnUpdate.first)(static_cast< int32_t >(update_type));
            (*mOnVehicleUpdate.first)(inst.mObj, static_cast< int32_t >(update_type));
        }
    }
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::VehicleUpdate")
}

// ------------------------------------------------------------------------------------------------
void Core::EmitClientScriptData(int32_t player_id, const uint8_t * data, size_t size)
{
    SQMOD_CO_EV_TRACEBACK("[TRACE<] Core::ClientScriptData(%d, [byte stream], %" PRINT_SZ_FMT ")", player_id, size)
    PlayerInst & _player = m_Players.at(static_cast< size_t >(player_id));
    // Don't even bother if there's no one listening
    if (!(_player.mOnClientScriptData.first->IsEmpty()) || !(mOnClientScriptData.first->IsEmpty()))
    {
        // Allocate a buffer with the received size
        Buffer b(static_cast< Buffer::SzType >(size));
        // Replicate the data to the allocated buffer
        b.Write(0, reinterpret_cast< Buffer::ConstPtr >(data), static_cast< Buffer::SzType >(size));
        // Prepare an object for the obtained buffer
        LightObj o(SqTypeIdentity< SqBuffer >{}, m_VM, std::move(b));
        // Forward the event call
        (*_player.mOnClientScriptData.first)(o, size);
        (*mOnClientScriptData.first)(_player.mObj, o, size);
    }
    SQMOD_CO_EV_TRACEBACK("[TRACE>] Core::ClientScriptData")
#ifdef VCMP_ENABLE_OFFICIAL
    if (IsOfficial())
    {
        LgStreamLoadInput(data, size);
        ExecuteLegacyEvent(m_VM, _SC("onClientScriptData"), _player.mLgObj);
    }
#endif
}

#undef NULL_SQOBJ_ // don't need this anymore

} // Namespace:: SqMod
