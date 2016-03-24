// --------------------------------------------------------------------------------------------
#include "Logger.hpp"
#include "Core.hpp"
#include "Command.hpp"
#include "SqMod.h"

// ------------------------------------------------------------------------------------------------
#include <stdio.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {
extern void InitExports();
} // Namespace:: SqMod

// ------------------------------------------------------------------------------------------------
using namespace SqMod;

// ------------------------------------------------------------------------------------------------
void BindCallbacks();
void UnbindCallbacks();

// ------------------------------------------------------------------------------------------------
void DestroyComponents()
{
    // Destroy command component
    if (_Cmd)
    {
        SQMOD_MANAGEDPTR_DEL(CmdManager, _Cmd);
        SQMOD_MANAGEDPTR_MAKE(CmdManager, nullptr);
    }
    // Destroy core component
    if (_Core)
    {
        SQMOD_MANAGEDPTR_DEL(Core, _Core);
        SQMOD_MANAGEDPTR_MAKE(Core, nullptr);
    }
    // Destroy logger component
    if (_Log)
    {
        SQMOD_MANAGEDPTR_DEL(Logger, _Log);
        SQMOD_MANAGEDPTR_MAKE(Logger, nullptr);
    }
}

// ------------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs * funcs, PluginCallbacks * calls, PluginInfo * info)
{
    // Output plugin header
    puts("");
    OutputMessage("--------------------------------------------------------------------");
    OutputMessage("Plugin: %s", SQMOD_NAME);
    OutputMessage("Author: %s", SQMOD_AUTHOR);
    OutputMessage("Legal: %s", SQMOD_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    puts("");

    // Verify that core components are working
    if (!Logger::Get())
    {
        puts("[SQMOD] Unable to start because the logging class could not be instantiated");
        return SQMOD_FAILURE;
    }
    if (!Core::Get())
    {
        DestroyComponents();
        puts("[SQMOD] Unable to start because the central core class could not be instantiated");
        return SQMOD_FAILURE;
    }
    if (!CmdManager::Get())
    {
        DestroyComponents();
        puts("[SQMOD] Unable to start because the command class could not be instantiated");
        return SQMOD_FAILURE;
    }
    // Store server proxies
    _Func = funcs;
    _Clbk = calls;
    _Info = info;
    // Assign plugin information
    _Info->uPluginVer = SQMOD_VERSION;
    strcpy(_Info->szName, SQMOD_HOST_NAME);
    // Attempt to initialize the plugin
    if (!_Core->Init())
    {
        LogFtl("The plugin failed to initialize");
        _Core->Terminate();
        DestroyComponents();
        return SQMOD_FAILURE;
    }
    else if (_Clbk)
        BindCallbacks();
    else
    {
        _Core->Terminate();
        DestroyComponents();
        LogFtl("Unable to start because the server callbacks are missing");
        return SQMOD_FAILURE;
    }
    // Attempt to initialize the plugin exports
    InitExports();
    // Notify that the plugin was successfully loaded
    OutputMessage("Successfully loaded %s", SQMOD_NAME);
    // Dummy spacing
    puts("");
    // Initialization was successful
    return SQMOD_SUCCESS;
}

#define SQMOD_CATCH_EVENT_EXCEPTION(ev) /*
*/ catch (const Sqrat::Exception & e) /*
*/ { /*
*/  LogErr("Squirrel exception caught during (" #ev ") event"); /*
*/  LogInf("Message: %s", e.Message().c_str()); /*
*/ } /*
*/ catch (const std::exception & e) /*
*/ { /*
*/  LogErr("Program exception caught during (" #ev ") event"); /*
*/  LogInf("Message: %s", e.what()); /*
*/ } /*
*/ catch (...) /*
*/ { /*
*/  LogErr("Unknown exception caught during (" #ev ") event"); /*
*/ } /*
*/

// --------------------------------------------------------------------------------------------
static int VC_InitServer(void)
{
    // Don't even try to initialize if there's no core instance
    if (!_Core)
    {
        return SQMOD_FAILURE;
    }
    // Mark the initialization as successful by default
    _Core->SetState(1);
    // Attempt to forward the event
    try
    {

        // Obtain the API version as a string
        String apiver(ToStrF("%d", SQMOD_API_VER));
        // Signal outside plug-ins to do fetch our proxies
        _Func->SendCustomCommand(0xDABBAD00, apiver.c_str());
        // Attempt to load the module core
        if (_Core->Load())
        {
            _Core->EmitServerStartup();
        }
        else
        {
            LogFtl("Unable to load the plugin resources properly");
        }
    }
    SQMOD_CATCH_EVENT_EXCEPTION(InitServer)
    // Return the last known plug-in state
    return _Core->GetState();
}

static void VC_ShutdownServer(void)
{
    // Don't even try to de-initialize if there's no core instance
    if (!_Core)
    {
        return;
    }
    // Attempt to forward the event
    try
    {
        _Core->EmitServerShutdown();
        // Deallocate and release everything obtained at startup
        _Core->Terminate();
        // The server still triggers callbacks and we deallocated everything!
        UnbindCallbacks();
        // Destroy components
        DestroyComponents();
    }
    SQMOD_CATCH_EVENT_EXCEPTION(ShutdownServer)
}

static void VC_Frame(float delta)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitServerFrame(delta);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(Frame)
}

static void VC_PlayerConnect(int player)
{
    // Attempt to forward the event
    try
    {
        _Core->ConnectPlayer(player, SQMOD_CREATE_AUTOMATIC, NullObject());
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerConnect)
}

static void VC_PlayerDisconnect(int player, int reason)
{
    // Attempt to forward the event
    try
    {
        _Core->DisconnectPlayer(player, reason, NullObject());
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerDisconnect)
}

static void VC_PlayerBeginTyping(int player)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerStartTyping(player);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerBeginTyping)
}

static void VC_PlayerEndTyping(int player)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerStopTyping(player);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerEndTyping)
}

static int VC_PlayerRequestClass(int player, int offset)
{
    // Mark the initialization as successful by default
    _Core->SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerRequestClass(player, offset);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerEndTyping)
    // Return the last known plug-in state
    return _Core->GetState();
}

static int VC_PlayerRequestSpawn(int player)
{
    // Mark the initialization as successful by default
    _Core->SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerRequestSpawn(player);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerRequestSpawn)
    // Return the last known plug-in state
    return _Core->GetState();
}

static void VC_PlayerSpawn(int player)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerSpawn(player);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerSpawn)
}

static void VC_PlayerDeath(int player, int killer, int reason, int body_part)
{
    // Attempt to forward the event
    try
    {
        if (_Func->IsPlayerConnected(killer))
        {
            _Core->EmitPlayerKilled(player, killer, reason, body_part);
        }
        else
        {
            _Core->EmitPlayerWasted(player, reason);
        }
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerDeath)
}

static void VC_PlayerUpdate(int player, int type)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerUpdate(player, type);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerUpdate)
}

static int VC_PlayerRequestEnter(int player, int vehicle, int slot)
{
    // Mark the initialization as successful by default
    _Core->SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerEmbarking(player, vehicle, slot);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerRequestEnter)
    // Return the last known plug-in state
    return _Core->GetState();
}

static void VC_PlayerEnterVehicle(int player, int vehicle, int slot)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerEmbarked(player, vehicle, slot);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerEnterVehicle)
}

static void VC_PlayerExitVehicle(int player, int vehicle)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerDisembark(player, vehicle);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerExitVehicle)
}

static int VC_PickupClaimPicked(int pickup, int player)
{
    // Mark the initialization as successful by default
    _Core->SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        _Core->EmitPickupClaimed(player, pickup);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PickupClaimPicked)
    // Return the last known plug-in state
    return _Core->GetState();
}

static void VC_PickupPickedUp(int pickup, int player)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPickupCollected(player, pickup);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PickupPickedUp)
}

static void VC_PickupRespawn(int pickup)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPickupRespawn(pickup);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PickupRespawn)
}

static void VC_VehicleUpdate(int vehicle, int type)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitVehicleUpdate(vehicle, type);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(VehicleUpdate)
}

static void VC_VehicleExplode(int vehicle)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitVehicleExplode(vehicle);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(VehicleExplode)
}

static void VC_VehicleRespawn(int vehicle)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitVehicleRespawn(vehicle);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(VehicleRespawn)
}

static void VC_ObjectShot(int object, int player, int weapon)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitObjectShot(player, object, weapon);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(ObjectShot)
}

static void VC_ObjectBump(int object, int player)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitObjectBump(player, object);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(ObjectBump)
}

static int VC_PublicMessage(int player, const char * text)
{
    // Mark the initialization as successful by default
    _Core->SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerChat(player, text);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PublicMessage)
    // Return the last known plug-in state
    return _Core->GetState();
}

static int VC_CommandMessage(int player, const char * text)
{
    // Mark the initialization as successful by default
    _Core->SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerCommand(player, text);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(CommandMessage)
    // Return the last known plug-in state
    return _Core->GetState();
}

static int VC_PrivateMessage(int player, int target, const char * text)
{
    // Mark the initialization as successful by default
    _Core->SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerMessage(player, target, text);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PrivateMessage)
    // Return the last known plug-in state
    return _Core->GetState();
}

static int VC_InternalCommand(unsigned int type, const char * text)
{
    // Mark the initialization as successful by default
    _Core->SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        _Core->EmitInternalCommand(type, text);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(InternalCommand)
    // Return the last known plug-in state
    return _Core->GetState();
}

static int VC_LoginAttempt(char * name, const char * passwd, const char * address)
{
    // Mark the initialization as successful by default
    _Core->SetState(SQMOD_SUCCESS);
    // Attempt to forward the event
    try
    {
        _Core->EmitLoginAttempt(name, passwd, address);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(LoginAttempt)
    // Return the last known plug-in state
    return _Core->GetState();
}

static void VC_EntityPool(int type, int id, unsigned int deleted)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitEntityPool(type, id, static_cast< bool >(deleted));
    }
    SQMOD_CATCH_EVENT_EXCEPTION(EntityPool)
}

static void VC_KeyBindDown(int player, int bind)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerKeyPress(player, bind);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(KeyBindDown)
}

static void VC_KeyBindUp(int player, int bind)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerKeyRelease(player, bind);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(KeyBindUp)
}

static void VC_PlayerAway(int player, unsigned int status)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerAway(player, static_cast< bool >(status));
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerAway)
}

static void VC_PlayerSpectate(int player, int target)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerSpectate(player, target);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerSpectate)
}

static void VC_PlayerCrashReport(int player, const char * report)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerCrashreport(player, report);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerCrashReport)
}

static void VC_ServerPerformanceReport(int /*count*/, const char ** /*description*/, unsigned long long * /*millis*/)
{
    // Ignored for now...
}

static void VC_PlayerName(int player, const char * previous, const char * current)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerRename(player, previous, current);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerName)
}

static void VC_PlayerState(int player, int previous, int current)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerState(player, previous, current);
        // Identify the current state and trigger the listeners specific to that
        switch (current)
        {
            case SQMOD_PLAYER_STATE_NONE:
                _Core->EmitStateNone(player, previous);
            break;
            case SQMOD_PLAYER_STATE_NORMAL:
                _Core->EmitStateNormal(player, previous);
            break;
            case SQMOD_PLAYER_STATE_SHOOTING:
                _Core->EmitStateShooting(player, previous);
            break;
            case SQMOD_PLAYER_STATE_DRIVER:
                _Core->EmitStateDriver(player, previous);
            break;
            case SQMOD_PLAYER_STATE_PASSENGER:
                _Core->EmitStatePassenger(player, previous);
            break;
            case SQMOD_PLAYER_STATE_ENTERING_AS_DRIVER:
                _Core->EmitStateEnterDriver(player, previous);
            break;
            case SQMOD_PLAYER_STATE_ENTERING_AS_PASSENGER:
                _Core->EmitStateEnterPassenger(player, previous);
            break;
            case SQMOD_PLAYER_STATE_EXITING_VEHICLE:
                _Core->EmitStateExitVehicle(player, previous);
            break;
            case SQMOD_PLAYER_STATE_UNSPAWNED:
                _Core->EmitStateUnspawned(player, previous);
            break;
        }
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerState)
}

static void VC_PlayerAction(int player, int previous, int current)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerAction(player, previous, current);
        // Identify the current action and trigger the listeners specific to that
        switch (current)
        {
            case SQMOD_PLAYER_ACTION_NONE:
                _Core->EmitActionNone(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_NORMAL:
                _Core->EmitActionNormal(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_AIMING:
                _Core->EmitActionAiming(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_SHOOTING:
                _Core->EmitActionShooting(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_JUMPING:
                _Core->EmitActionJumping(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_LYING_ON_GROUND:
                _Core->EmitActionLieDown(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_GETTING_UP:
                _Core->EmitActionGettingUp(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_JUMPING_FROM_VEHICLE:
                _Core->EmitActionJumpVehicle(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_DRIVING:
                _Core->EmitActionDriving(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_DYING:
                _Core->EmitActionDying(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_WASTED:
                _Core->EmitActionWasted(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_ENTERING_VEHICLE:
                _Core->EmitActionEmbarking(player, previous);
            break;
            case SQMOD_PLAYER_ACTION_EXITING_VEHICLE:
                _Core->EmitActionDisembarking(player, previous);
            break;
        }
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerAction)
}

static void VC_PlayerOnFire(int player, unsigned int state)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerBurning(player, static_cast< bool >(state));
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerOnFire)
}

static void VC_PlayerCrouch(int player, unsigned int state)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerCrouching(player, static_cast< bool >(state));
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerCrouch)
}

static void VC_PlayerGameKeys(int player, int previous, int current)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitPlayerGameKeys(player, previous, current);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(PlayerGameKeys)
}

static void VC_OnCheckpointEntered(int checkpoint, int player)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitCheckpointEntered(player, checkpoint);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(CheckpointEntered)
}

static void VC_OnCheckpointExited(int checkpoint, int player)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitCheckpointExited(player, checkpoint);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(CheckpointExited)
}

static void VC_OnSphereEntered(int sphere, int player)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitForcefieldEntered(player, sphere);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(SphereEntered)
}

static void VC_OnSphereExited(int sphere, int player)
{
    // Attempt to forward the event
    try
    {
        _Core->EmitForcefieldExited(player, sphere);
    }
    SQMOD_CATCH_EVENT_EXCEPTION(SphereExited)
}

// ------------------------------------------------------------------------------------------------
void BindCallbacks()
{
    _Clbk->OnInitServer                 = VC_InitServer;
    _Clbk->OnShutdownServer             = VC_ShutdownServer;
    _Clbk->OnFrame                      = VC_Frame;
    _Clbk->OnPlayerConnect              = VC_PlayerConnect;
    _Clbk->OnPlayerDisconnect           = VC_PlayerDisconnect;
    _Clbk->OnPlayerBeginTyping          = VC_PlayerBeginTyping;
    _Clbk->OnPlayerEndTyping            = VC_PlayerEndTyping;
    _Clbk->OnPlayerRequestClass         = VC_PlayerRequestClass;
    _Clbk->OnPlayerRequestSpawn         = VC_PlayerRequestSpawn;
    _Clbk->OnPlayerSpawn                = VC_PlayerSpawn;
    _Clbk->OnPlayerDeath                = VC_PlayerDeath;
    _Clbk->OnPlayerUpdate               = VC_PlayerUpdate;
    _Clbk->OnPlayerRequestEnter         = VC_PlayerRequestEnter;
    _Clbk->OnPlayerEnterVehicle         = VC_PlayerEnterVehicle;
    _Clbk->OnPlayerExitVehicle          = VC_PlayerExitVehicle;
    _Clbk->OnPickupClaimPicked          = VC_PickupClaimPicked;
    _Clbk->OnPickupPickedUp             = VC_PickupPickedUp;
    _Clbk->OnPickupRespawn              = VC_PickupRespawn;
    _Clbk->OnVehicleUpdate              = VC_VehicleUpdate;
    _Clbk->OnVehicleExplode             = VC_VehicleExplode;
    _Clbk->OnVehicleRespawn             = VC_VehicleRespawn;
    _Clbk->OnObjectShot                 = VC_ObjectShot;
    _Clbk->OnObjectBump                 = VC_ObjectBump;
    _Clbk->OnPublicMessage              = VC_PublicMessage;
    _Clbk->OnCommandMessage             = VC_CommandMessage;
    _Clbk->OnPrivateMessage             = VC_PrivateMessage;
    _Clbk->OnInternalCommand            = VC_InternalCommand;
    _Clbk->OnLoginAttempt               = VC_LoginAttempt;
    _Clbk->OnEntityPoolChange           = VC_EntityPool;
    _Clbk->OnKeyBindDown                = VC_KeyBindDown;
    _Clbk->OnKeyBindUp                  = VC_KeyBindUp;
    _Clbk->OnPlayerAwayChange           = VC_PlayerAway;
    _Clbk->OnPlayerSpectate             = VC_PlayerSpectate;
    _Clbk->OnPlayerCrashReport          = VC_PlayerCrashReport;
    _Clbk->OnServerPerformanceReport    = VC_ServerPerformanceReport;
    _Clbk->OnPlayerNameChange           = VC_PlayerName;
    _Clbk->OnPlayerStateChange          = VC_PlayerState;
    _Clbk->OnPlayerActionChange         = VC_PlayerAction;
    _Clbk->OnPlayerOnFireChange         = VC_PlayerOnFire;
    _Clbk->OnPlayerCrouchChange         = VC_PlayerCrouch;
    _Clbk->OnPlayerGameKeysChange       = VC_PlayerGameKeys;
    _Clbk->OnCheckpointEntered          = VC_OnCheckpointEntered;
    _Clbk->OnCheckpointExited           = VC_OnCheckpointExited;
    _Clbk->OnSphereEntered              = VC_OnSphereEntered;
    _Clbk->OnSphereExited               = VC_OnSphereExited;
}

// ------------------------------------------------------------------------------------------------
void UnbindCallbacks()
{
    _Clbk->OnInitServer                 = nullptr;
    _Clbk->OnShutdownServer             = nullptr;
    _Clbk->OnFrame                      = nullptr;
    _Clbk->OnPlayerConnect              = nullptr;
    _Clbk->OnPlayerDisconnect           = nullptr;
    _Clbk->OnPlayerBeginTyping          = nullptr;
    _Clbk->OnPlayerEndTyping            = nullptr;
    _Clbk->OnPlayerRequestClass         = nullptr;
    _Clbk->OnPlayerRequestSpawn         = nullptr;
    _Clbk->OnPlayerSpawn                = nullptr;
    _Clbk->OnPlayerDeath                = nullptr;
    _Clbk->OnPlayerUpdate               = nullptr;
    _Clbk->OnPlayerRequestEnter         = nullptr;
    _Clbk->OnPlayerEnterVehicle         = nullptr;
    _Clbk->OnPlayerExitVehicle          = nullptr;
    _Clbk->OnPickupClaimPicked          = nullptr;
    _Clbk->OnPickupPickedUp             = nullptr;
    _Clbk->OnPickupRespawn              = nullptr;
    _Clbk->OnVehicleUpdate              = nullptr;
    _Clbk->OnVehicleExplode             = nullptr;
    _Clbk->OnVehicleRespawn             = nullptr;
    _Clbk->OnObjectShot                 = nullptr;
    _Clbk->OnObjectBump                 = nullptr;
    _Clbk->OnPublicMessage              = nullptr;
    _Clbk->OnCommandMessage             = nullptr;
    _Clbk->OnPrivateMessage             = nullptr;
    _Clbk->OnInternalCommand            = nullptr;
    _Clbk->OnLoginAttempt               = nullptr;
    _Clbk->OnEntityPoolChange           = nullptr;
    _Clbk->OnKeyBindDown                = nullptr;
    _Clbk->OnKeyBindUp                  = nullptr;
    _Clbk->OnPlayerAwayChange           = nullptr;
    _Clbk->OnPlayerSpectate             = nullptr;
    _Clbk->OnPlayerCrashReport          = nullptr;
    _Clbk->OnServerPerformanceReport    = nullptr;
    _Clbk->OnPlayerNameChange           = nullptr;
    _Clbk->OnPlayerStateChange          = nullptr;
    _Clbk->OnPlayerActionChange         = nullptr;
    _Clbk->OnPlayerOnFireChange         = nullptr;
    _Clbk->OnPlayerCrouchChange         = nullptr;
    _Clbk->OnPlayerGameKeysChange       = nullptr;
    _Clbk->OnCheckpointEntered          = nullptr;
    _Clbk->OnCheckpointExited           = nullptr;
    _Clbk->OnSphereEntered              = nullptr;
    _Clbk->OnSphereExited               = nullptr;
}
