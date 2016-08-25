// ------------------------------------------------------------------------------------------------
#include "Logger.hpp"
#include "Core.hpp"
#include "SqMod.h"

// ------------------------------------------------------------------------------------------------
#include <cstdio>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static bool g_Reload = false;

// ------------------------------------------------------------------------------------------------
extern void InitExports();
extern void ProcessRoutines();

/* ------------------------------------------------------------------------------------------------
 * Will the scripts be reloaded at the end of the current event?
*/
bool GetReloadStatus()
{
    return g_Reload;
}

/* ------------------------------------------------------------------------------------------------
 * Toggles the reload status.
*/
void SetReloadStatus(bool toggle)
{
    g_Reload = toggle;
}
/* ------------------------------------------------------------------------------------------------
 * Helper class to make sure that the reload is disabled and the payload is released.
*/
struct ReloadGuard
{
    ~ReloadGuard()
    {
        g_Reload = false;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Perform the actual reload.
*/
void DoReload()
{
    // Are we already trying to reload?
    if (Core::Get().GetCircularLock() & CCL_RELOAD_SCRIPTS)
    {
        return; // Don't even bother!
    }
    // Make sure reloading is disabled at the end of this function
    const ReloadGuard rg;
    // Tell the central core to attempt to reload
    if (!Core::Get().Reload())
    {
        LogFtl("Unable to reload scripts");
    }
}

// --------------------------------------------------------------------------------------------
#define SQMOD_CATCH_EVENT_EXCEPTION(ev) /*
*/ catch (const Sqrat::Exception & e) /*
*/ { /*
*/  LogErr("Squirrel exception caught (" #ev ") event"); /*
*/  Logger::Get().Debug("%s", e.what()); /*
*/ } /*
*/ catch (const std::exception & e) /*
*/ { /*
*/  LogErr("Program exception caught (" #ev ") event [%s]", e.what()); /*
*/ } /*
*/ catch (...) /*
*/ { /*
*/  LogErr("Unknown exception caught (" #ev ") event"); /*
*/ } /*
*/

// --------------------------------------------------------------------------------------------
#define SQMOD_RELOAD_CHECK(exp) if (exp) DoReload();

// ------------------------------------------------------------------------------------------------
static uint8_t OnServerInitialise(void)
{
    // Mark the initialization as successful by default
    Core::Get().SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        // Signal outside plug-ins to do fetch our proxies
        _Func->SendPluginCommand(SQMOD_INITIALIZE_CMD, "%d", SQMOD_API_VER);
        // Attempt to load the module core
        if (Core::Get().Execute())
        {
            Core::Get().EmitServerStartup();
            // Add a notification to let the user know the plug-in was loaded
            OutputMessage("The Squirrel plug-in was loaded successfully");
        }
        else
        {
            OutputError("Unable to load the plug-in resources properly");
            // Failed to initialize
            return SQMOD_FAILURE;
        }
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnServerInitialise)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
    // Return the last known plug-in state
    return ConvTo< Uint8 >::From(Core::Get().GetState());
}

// ------------------------------------------------------------------------------------------------
static void OnServerShutdown(void)
{
    // Attempt to forward the event
    try
    {
        // Tell the script that the server is shutting down
        Core::Get().EmitServerShutdown();
        // Deallocate and release everything obtained at startup
        Core::Get().Terminate(true);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnServerShutdown)
    // See if a reload was requested (quite useless here but why not)
    SQMOD_RELOAD_CHECK(g_Reload)
    // The server still triggers callbacks and we deallocated everything!
    _Clbk->OnServerInitialise           = nullptr;
    _Clbk->OnServerShutdown             = nullptr;
    _Clbk->OnServerFrame                = nullptr;
    _Clbk->OnPluginCommand              = nullptr;
    _Clbk->OnIncomingConnection         = nullptr;
    _Clbk->OnClientScriptData           = nullptr;
    _Clbk->OnPlayerConnect              = nullptr;
    _Clbk->OnPlayerDisconnect           = nullptr;
    _Clbk->OnPlayerRequestClass         = nullptr;
    _Clbk->OnPlayerRequestSpawn         = nullptr;
    _Clbk->OnPlayerSpawn                = nullptr;
    _Clbk->OnPlayerDeath                = nullptr;
    _Clbk->OnPlayerUpdate               = nullptr;
    _Clbk->OnPlayerRequestEnterVehicle  = nullptr;
    _Clbk->OnPlayerEnterVehicle         = nullptr;
    _Clbk->OnPlayerExitVehicle          = nullptr;
    _Clbk->OnPlayerNameChange           = nullptr;
    _Clbk->OnPlayerStateChange          = nullptr;
    _Clbk->OnPlayerActionChange         = nullptr;
    _Clbk->OnPlayerOnFireChange         = nullptr;
    _Clbk->OnPlayerCrouchChange         = nullptr;
    _Clbk->OnPlayerGameKeysChange       = nullptr;
    _Clbk->OnPlayerBeginTyping          = nullptr;
    _Clbk->OnPlayerEndTyping            = nullptr;
    _Clbk->OnPlayerAwayChange           = nullptr;
    _Clbk->OnPlayerMessage              = nullptr;
    _Clbk->OnPlayerCommand              = nullptr;
    _Clbk->OnPlayerPrivateMessage       = nullptr;
    _Clbk->OnPlayerKeyBindDown          = nullptr;
    _Clbk->OnPlayerKeyBindUp            = nullptr;
    _Clbk->OnPlayerSpectate             = nullptr;
    _Clbk->OnPlayerCrashReport          = nullptr;
    _Clbk->OnVehicleUpdate              = nullptr;
    _Clbk->OnVehicleExplode             = nullptr;
    _Clbk->OnVehicleRespawn             = nullptr;
    _Clbk->OnObjectShot                 = nullptr;
    _Clbk->OnObjectTouched              = nullptr;
    _Clbk->OnPickupPickAttempt          = nullptr;
    _Clbk->OnPickupPicked               = nullptr;
    _Clbk->OnPickupRespawn              = nullptr;
    _Clbk->OnCheckpointEntered          = nullptr;
    _Clbk->OnCheckpointExited           = nullptr;
    _Clbk->OnEntityPoolChange           = nullptr;
    _Clbk->OnServerPerformanceReport    = nullptr;
}

// ------------------------------------------------------------------------------------------------
static void OnServerFrame(float elapsed_time)
{
    // Attempt to forward the event
    try
    {
        // Now forward the event
        Core::Get().EmitServerFrame(elapsed_time);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnServerFrame)
    // Process routines, if any
    ProcessRoutines();
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static uint8_t OnPluginCommand(uint32_t command_identifier, CCStr message)
{
    // Mark the initialization as successful by default
    Core::Get().SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPluginCommand(command_identifier, message);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPluginCommand)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
    // Return the last known plug-in state
    return ConvTo< Uint8 >::From(Core::Get().GetState());
}

// ------------------------------------------------------------------------------------------------
static uint8_t OnIncomingConnection(CStr player_name, size_t name_buffer_size,
                                    CCStr user_password, CCStr ip_address)
{
    // Mark the initialization as successful by default
    Core::Get().SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        Core::Get().EmitIncomingConnection(player_name, name_buffer_size, user_password, ip_address);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnIncomingConnection)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
    // Return the last known plug-in state
    return ConvTo< Uint8 >::From(Core::Get().GetState());
}

// ------------------------------------------------------------------------------------------------
static void OnClientScriptData(int32_t player_id, const uint8_t * data, size_t size)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitClientScriptData(player_id, data, size);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnClientScriptData)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerConnect(int32_t player_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().ConnectPlayer(player_id, SQMOD_CREATE_AUTOMATIC, NullObject());
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerConnect)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerDisconnect(int32_t player_id, vcmpDisconnectReason reason)
{
    // Attempt to forward the event
    try
    {
        if (reason == vcmpDisconnectReasonKick)
        {
            Core::Get().DisconnectPlayer(player_id, Core::Get().GetPlayer(player_id).mKickBanHeader,
                                                    Core::Get().GetPlayer(player_id).mKickBanPayload);
        }
        else
        {
            Core::Get().DisconnectPlayer(player_id, reason, NullObject());
        }
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerDisconnect)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static uint8_t OnPlayerRequestClass(int32_t player_id, int32_t offset)
{
    // Mark the initialization as successful by default
    Core::Get().SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerRequestClass(player_id, offset);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerRequestClass)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
    // Return the last known plug-in state
    return ConvTo< Uint8 >::From(Core::Get().GetState());
}

// ------------------------------------------------------------------------------------------------
static uint8_t OnPlayerRequestSpawn(int32_t player_id)
{
    // Mark the initialization as successful by default
    Core::Get().SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerRequestSpawn(player_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerRequestSpawn)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
    // Return the last known plug-in state
    return ConvTo< Uint8 >::From(Core::Get().GetState());
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerSpawn(int32_t player_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerSpawn(player_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerSpawn)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerDeath(int32_t player_id, int32_t killer_id, int32_t reason, vcmpBodyPart body_part)
{
    // Attempt to forward the event
    try
    {
        if (_Func->IsPlayerConnected(killer_id))
        {
            const int32_t pt = _Func->GetPlayerTeam(player_id), kt = _Func->GetPlayerTeam(killer_id);
            Core::Get().EmitPlayerKilled(player_id, killer_id, reason, body_part,
                                            (pt == kt && pt >= 0 && kt >= 0));
        }
        else
        {
            Core::Get().EmitPlayerWasted(player_id, reason);
        }
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerDeath)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerUpdate(int32_t player_id, vcmpPlayerUpdate update_type)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerUpdate(player_id, update_type);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerUpdate)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static uint8_t OnPlayerRequestEnterVehicle(int32_t player_id, int32_t vehicle_id, int32_t slot_index)
{
    // Mark the initialization as successful by default
    Core::Get().SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerEmbarking(player_id, vehicle_id, slot_index);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerRequestEnterVehicle)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
    // Return the last known plug-in state
    return ConvTo< Uint8 >::From(Core::Get().GetState());
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerEnterVehicle(int32_t player_id, int32_t vehicle_id, int32_t slot_index)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerEmbarked(player_id, vehicle_id, slot_index);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerEnterVehicle)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerExitVehicle(int32_t player_id, int32_t vehicle_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerDisembark(player_id, vehicle_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerExitVehicle)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerNameChange(int32_t player_id, CCStr old_name, CCStr new_name)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerRename(player_id, old_name, new_name);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerNameChange)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerStateChange(int32_t player_id, vcmpPlayerState old_state, vcmpPlayerState new_state)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerState(player_id, old_state, new_state);
        // Identify the current state and trigger the listeners specific to that
        switch (new_state)
        {
            case vcmpPlayerStateNone:
                Core::Get().EmitStateNone(player_id, old_state);
            break;
            case vcmpPlayerStateNormal:
                Core::Get().EmitStateNormal(player_id, old_state);
            break;
            case vcmpPlayerStateAim:
                Core::Get().EmitStateAim(player_id, old_state);
            break;
            case vcmpPlayerStateDriver:
                Core::Get().EmitStateDriver(player_id, old_state);
            break;
            case vcmpPlayerStatePassenger:
                Core::Get().EmitStatePassenger(player_id, old_state);
            break;
            case vcmpPlayerStateEnterDriver:
                Core::Get().EmitStateEnterDriver(player_id, old_state);
            break;
            case vcmpPlayerStateEnterPassenger:
                Core::Get().EmitStateEnterPassenger(player_id, old_state);
            break;
            case vcmpPlayerStateExit:
                Core::Get().EmitStateExit(player_id, old_state);
            break;
            case vcmpPlayerStateUnspawned:
                Core::Get().EmitStateUnspawned(player_id, old_state);
            break;
            default: LogErr("Unknown player state change: %d", static_cast< Int32 >(new_state));
        }
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerStateChange)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerActionChange(int32_t player_id, int32_t old_action, int32_t new_action)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerAction(player_id, old_action, new_action);
        // Identify the current action and trigger the listeners specific to that
        switch (new_action)
        {
            case SQMOD_PLAYER_ACTION_NONE:
                Core::Get().EmitActionNone(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_NORMAL:
                Core::Get().EmitActionNormal(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_AIMING:
                Core::Get().EmitActionAiming(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_SHOOTING:
                Core::Get().EmitActionShooting(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_JUMPING:
                Core::Get().EmitActionJumping(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_LYING_ON_GROUND:
                Core::Get().EmitActionLieDown(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_GETTING_UP:
                Core::Get().EmitActionGettingUp(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_JUMPING_FROM_VEHICLE:
                Core::Get().EmitActionJumpVehicle(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_DRIVING:
                Core::Get().EmitActionDriving(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_DYING:
                Core::Get().EmitActionDying(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_WASTED:
                Core::Get().EmitActionWasted(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_ENTERING_VEHICLE:
                Core::Get().EmitActionEmbarking(player_id, old_action);
            break;
            case SQMOD_PLAYER_ACTION_EXITING_VEHICLE:
                Core::Get().EmitActionDisembarking(player_id, old_action);
            break;
        }
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerActionChange)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerOnFireChange(int32_t player_id, uint8_t is_on_fire)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerBurning(player_id, is_on_fire);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerOnFireChange)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerCrouchChange(int32_t player_id, uint8_t is_crouching)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerCrouching(player_id, is_crouching);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerCrouchChange)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerGameKeysChange(int32_t player_id, uint32_t old_keys, uint32_t new_keys)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerGameKeys(player_id, old_keys, new_keys);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerGameKeysChange)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerBeginTyping(int32_t player_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerStartTyping(player_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerBeginTyping)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerEndTyping(int32_t player_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerStopTyping(player_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerEndTyping)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerAwayChange(int32_t player_id, uint8_t is_away)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerAway(player_id, is_away);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerAwayChange)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static uint8_t OnPlayerMessage(int32_t player_id, CCStr message)
{
    // Mark the initialization as successful by default
    Core::Get().SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerMessage(player_id, message);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerMessage)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
    // Return the last known plug-in state
    return ConvTo< Uint8 >::From(Core::Get().GetState());
}

// ------------------------------------------------------------------------------------------------
static uint8_t OnPlayerCommand(int32_t player_id, CCStr message)
{
    // Mark the initialization as successful by default
    Core::Get().SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerCommand(player_id, message);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerCommand)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
    // Return the last known plug-in state
    return ConvTo< Uint8 >::From(Core::Get().GetState());
}

// ------------------------------------------------------------------------------------------------
static uint8_t OnPlayerPrivateMessage(int32_t player_id, int32_t target_player_id, CCStr message)
{
    // Mark the initialization as successful by default
    Core::Get().SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerPrivateMessage(player_id, target_player_id, message);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerPrivateMessage)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
    // Return the last known plug-in state
    return ConvTo< Uint8 >::From(Core::Get().GetState());
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerKeyBindDown(int32_t player_id, int32_t bind_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerKeyPress(player_id, bind_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerKeyBindDown)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerKeyBindUp(int32_t player_id, int32_t bind_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerKeyRelease(player_id, bind_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerKeyBindUp)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(g_Reload)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerSpectate(int32_t player_id, int32_t target_player_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerSpectate(player_id, target_player_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerSpectate)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static void OnPlayerCrashReport(int32_t player_id, CCStr report)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPlayerCrashreport(player_id, report);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPlayerCrashReport)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static void OnVehicleUpdate(int32_t vehicle_id, vcmpVehicleUpdate update_type)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitVehicleUpdate(vehicle_id, update_type);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnVehicleUpdate)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static void OnVehicleExplode(int32_t vehicle_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitVehicleExplode(vehicle_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnVehicleExplode)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static void OnVehicleRespawn(int32_t vehicle_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitVehicleRespawn(vehicle_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnVehicleRespawn)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static void OnObjectShot(int32_t object_id, int32_t player_id, int32_t weapon_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitObjectShot(object_id, player_id, weapon_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnObjectShot)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static void OnObjectTouched(int32_t object_id, int32_t player_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitObjectTouched(object_id, player_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnObjectTouched)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static uint8_t OnPickupPickAttempt(int32_t pickup_id, int32_t player_id)
{
    // Mark the initialization as successful by default
    Core::Get().SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPickupClaimed(pickup_id, player_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPickupPickAttempt)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
    // Return the last known plug-in state
    return ConvTo< Uint8 >::From(Core::Get().GetState());
}

// ------------------------------------------------------------------------------------------------
static void OnPickupPicked(int32_t pickup_id, int32_t player_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPickupCollected(pickup_id, player_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPickupPicked)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static void OnPickupRespawn(int32_t pickup_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitPickupRespawn(pickup_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnPickupRespawn)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static void OnCheckpointEntered(int32_t checkpoint_id, int32_t player_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitCheckpointEntered(checkpoint_id, player_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnCheckpointEntered)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static void OnCheckpointExited(int32_t checkpoint_id, int32_t player_id)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitCheckpointExited(checkpoint_id, player_id);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnCheckpointExited)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static void OnEntityPoolChange(vcmpEntityPool entity_type, int32_t entity_id, uint8_t is_deleted)
{
    // Attempt to forward the event
    try
    {
        Core::Get().EmitEntityPool(entity_type, entity_id, is_deleted);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(OnEntityPoolChange)
    // See if a reload was requested
    SQMOD_RELOAD_CHECK(false)
}

// ------------------------------------------------------------------------------------------------
static void OnServerPerformanceReport(size_t /*entry_count*/, CCStr * /*descriptions*/, uint64_t * /*times*/)
{
    // Ignored for now...
}

} // Namespace:: SqMod

/* ------------------------------------------------------------------------------------------------
 * Plugiun initialization procedure.
*/
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs * funcs, PluginCallbacks * calls, PluginInfo * info)
{
    using namespace SqMod;
    // Output plug-in header
    puts("");
    OutputMessage("--------------------------------------------------------------------");
    OutputMessage("Plug-in: %s", SQMOD_NAME);
    OutputMessage("Author: %s", SQMOD_AUTHOR);
    OutputMessage("Legal: %s", SQMOD_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    puts("");
    // Store server proxies
    _Func = funcs;
    _Clbk = calls;
    _Info = info;
    // Assign plug-in version
    _Info->pluginVersion = SQMOD_VERSION;
    _Info->apiMajorVersion = PLUGIN_API_MAJOR;
    _Info->apiMinorVersion = PLUGIN_API_MINOR;
    // Assign the plug-in name
    std::snprintf(_Info->name, sizeof(_Info->name), "%s", SQMOD_HOST_NAME);
    // Attempt to initialize the logger before anything else
    Logger::Get().Initialize(nullptr);
    // Attempt to initialize the plug-in core
    if (!Core::Get().Initialize())
    {
        LogFtl("Unable to initialize the plug-in central core");
        // Attempt to terminate
        Core::Get().Terminate(false);
        // Stop here!
        return SQMOD_FAILURE;
    }
    // Bind to the server callbacks
    _Clbk->OnServerInitialise           = OnServerInitialise;
    _Clbk->OnServerShutdown             = OnServerShutdown;
    _Clbk->OnServerFrame                = OnServerFrame;
    _Clbk->OnPluginCommand              = OnPluginCommand;
    _Clbk->OnIncomingConnection         = OnIncomingConnection;
    _Clbk->OnClientScriptData           = OnClientScriptData;
    _Clbk->OnPlayerConnect              = OnPlayerConnect;
    _Clbk->OnPlayerDisconnect           = OnPlayerDisconnect;
    _Clbk->OnPlayerRequestClass         = OnPlayerRequestClass;
    _Clbk->OnPlayerRequestSpawn         = OnPlayerRequestSpawn;
    _Clbk->OnPlayerSpawn                = OnPlayerSpawn;
    _Clbk->OnPlayerDeath                = OnPlayerDeath;
    _Clbk->OnPlayerUpdate               = OnPlayerUpdate;
    _Clbk->OnPlayerRequestEnterVehicle  = OnPlayerRequestEnterVehicle;
    _Clbk->OnPlayerEnterVehicle         = OnPlayerEnterVehicle;
    _Clbk->OnPlayerExitVehicle          = OnPlayerExitVehicle;
    _Clbk->OnPlayerNameChange           = OnPlayerNameChange;
    _Clbk->OnPlayerStateChange          = OnPlayerStateChange;
    _Clbk->OnPlayerActionChange         = OnPlayerActionChange;
    _Clbk->OnPlayerOnFireChange         = OnPlayerOnFireChange;
    _Clbk->OnPlayerCrouchChange         = OnPlayerCrouchChange;
    _Clbk->OnPlayerGameKeysChange       = OnPlayerGameKeysChange;
    _Clbk->OnPlayerBeginTyping          = OnPlayerBeginTyping;
    _Clbk->OnPlayerEndTyping            = OnPlayerEndTyping;
    _Clbk->OnPlayerAwayChange           = OnPlayerAwayChange;
    _Clbk->OnPlayerMessage              = OnPlayerMessage;
    _Clbk->OnPlayerCommand              = OnPlayerCommand;
    _Clbk->OnPlayerPrivateMessage       = OnPlayerPrivateMessage;
    _Clbk->OnPlayerKeyBindDown          = OnPlayerKeyBindDown;
    _Clbk->OnPlayerKeyBindUp            = OnPlayerKeyBindUp;
    _Clbk->OnPlayerSpectate             = OnPlayerSpectate;
    _Clbk->OnPlayerCrashReport          = OnPlayerCrashReport;
    _Clbk->OnVehicleUpdate              = OnVehicleUpdate;
    _Clbk->OnVehicleExplode             = OnVehicleExplode;
    _Clbk->OnVehicleRespawn             = OnVehicleRespawn;
    _Clbk->OnObjectShot                 = OnObjectShot;
    _Clbk->OnObjectTouched              = OnObjectTouched;
    _Clbk->OnPickupPickAttempt          = OnPickupPickAttempt;
    _Clbk->OnPickupPicked               = OnPickupPicked;
    _Clbk->OnPickupRespawn              = OnPickupRespawn;
    _Clbk->OnCheckpointEntered          = OnCheckpointEntered;
    _Clbk->OnCheckpointExited           = OnCheckpointExited;
    _Clbk->OnEntityPoolChange           = OnEntityPoolChange;
    _Clbk->OnServerPerformanceReport    = OnServerPerformanceReport;
    // Attempt to initialize the plug-in exports
    InitExports();
    // Dummy spacing
    puts("");
    // Initialization was successful
    return SQMOD_SUCCESS;
}
