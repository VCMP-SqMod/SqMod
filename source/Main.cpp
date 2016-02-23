// --------------------------------------------------------------------------------------------
#include "Logger.hpp"
#include "Core.hpp"
#include "Command.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>

#if defined(_WIN32)
#include <winsock2.h>
#endif // _WIN32

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
        delete _Cmd;
        _Cmd = NULL;
    }
    // Destroy core component
    if (_Core)
    {
        delete _Core;
        _Core = NULL;
    }
    // Destroy logger component
    if (_Log)
    {
        delete _Log;
        _Log = NULL;
    }
}

// ------------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs * funcs, PluginCallbacks * calls, PluginInfo * info)
{
#if defined(_WIN32)
    WSADATA wsaData;
#endif // _WIN32
    _Log = Logger::Get();
    // Verify that core components are working
    if (!_Log)
    {
        puts("[SQMOD] Unable to start because the logging class could not be instantiated");
        return SQMOD_FAILURE;
    }
    _Core = Core::Get();
    if (!_Core)
    {
        DestroyComponents();
        puts("[SQMOD] Unable to start because the central core class could not be instantiated");
        return SQMOD_FAILURE;
    }
    _Cmd = CmdManager::Get();
    if (!_Cmd)
    {
        DestroyComponents();
        puts("[SQMOD] Unable to start because the command class could not be instantiated");
        return SQMOD_FAILURE;
    }
#if defined(_WIN32)
    // Initialize the sockets on windows
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        DestroyComponents();
        puts("[SQMOD] Unable to start because the windows sockets could not be initialized");
        return SQMOD_FAILURE;
    }
#endif // _WIN32

    _Func = funcs;
    _Clbk = calls;
    _Info = info;

    _Info->uPluginVer = SQMOD_VERSION;
    strcpy(_Info->szName, SQMOD_HOST_NAME);

    if (!_Core->Init())
    {
        LogFtl("The plugin failed to initialize");
        _Core->Terminate();
        DestroyComponents();
        return SQMOD_FAILURE;
    }
    else if (_Clbk)
    {
        BindCallbacks();
    }
    else
    {
        _Core->Terminate();
        DestroyComponents();
        LogFtl("Unable to start because the server callbacks are missing");
        return SQMOD_FAILURE;
    }
    // Attempt to initialize the plugin exports
    InitExports();
    // Initialization was successful
    return SQMOD_SUCCESS;
}

// --------------------------------------------------------------------------------------------
static int VC_InitServer(void)
{
    if (!_Core)
        return SQMOD_FAILURE;

    _Core->SetState(1);

    if (_Core->Load())
        _Core->EmitServerStartup();
    else
        LogFtl("Unable to load the plugin resources properly");

    return _Core->GetState();
}

static void VC_ShutdownServer(void)
{
    if (!_Core)
        return;

    _Core->EmitServerShutdown();
    // Deallocate and release everything obtained at startup
    _Core->Terminate();
    // The server still triggers callbacks and we deallocated everything!
    UnbindCallbacks();
    // Destroy components
    DestroyComponents();
}

static void VC_Frame(float delta)
{
    _Core->EmitServerFrame(delta);
}

static void VC_PlayerConnect(int player)
{
    _Core->ConnectPlayer(player, SQMOD_CREATE_AUTOMATIC, NullObject());
}

static void VC_PlayerDisconnect(int player, int reason)
{
    _Core->DisconnectPlayer(player, reason, NullObject());
}

static void VC_PlayerBeginTyping(int player)
{
    _Core->EmitPlayerStartTyping(player);
}

static void VC_PlayerEndTyping(int player)
{
    _Core->EmitPlayerStopTyping(player);
}

static int VC_PlayerRequestClass(int player, int offset)
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->EmitPlayerRequestClass(player, offset);
    return _Core->GetState();
}

static int VC_PlayerRequestSpawn(int player)
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->EmitPlayerRequestSpawn(player);
    return _Core->GetState();
}

static void VC_PlayerSpawn(int player)
{
    _Core->EmitPlayerSpawn(player);
}

static void VC_PlayerDeath(int player, int killer, int reason, int body_part)
{
    if (_Func->IsPlayerConnected(killer))
        _Core->EmitPlayerKilled(player, killer, reason, body_part);
    else
        _Core->EmitPlayerWasted(player, reason);
}

static void VC_PlayerUpdate(int player, int type)
{
    _Core->EmitPlayerUpdate(player, type);
}

static int VC_PlayerRequestEnter(int player, int vehicle, int slot)
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->EmitPlayerEmbarking(player, vehicle, slot);
    return _Core->GetState();
}

static void VC_PlayerEnterVehicle(int player, int vehicle, int slot)
{
    _Core->EmitPlayerEmbarked(player, vehicle, slot);
}

static void VC_PlayerExitVehicle(int player, int vehicle)
{
    _Core->EmitPlayerDisembark(player, vehicle);
}

static int VC_PickupClaimPicked(int pickup, int player)
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->EmitPickupClaimed(player, pickup);
    return _Core->GetState();
}

static void VC_PickupPickedUp(int pickup, int player)
{
    _Core->EmitPickupCollected(player, pickup);
}

static void VC_PickupRespawn(int pickup)
{
    _Core->EmitPickupRespawn(pickup);
}

static void VC_VehicleUpdate(int vehicle, int type)
{
    _Core->EmitVehicleUpdate(vehicle, type);
}

static void VC_VehicleExplode(int vehicle)
{
    _Core->EmitVehicleExplode(vehicle);
}

static void VC_VehicleRespawn(int vehicle)
{
    _Core->EmitVehicleRespawn(vehicle);
}

static void VC_ObjectShot(int object, int player, int weapon)
{
    _Core->EmitObjectShot(player, object, weapon);
}

static void VC_ObjectBump(int object, int player)
{
    _Core->EmitObjectBump(player, object);
}

static int VC_PublicMessage(int player, const char * text)
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->EmitPlayerChat(player, text);
    return _Core->GetState();
}

static int VC_CommandMessage(int player, const char * text)
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->EmitPlayerCommand(player, text);
    return _Core->GetState();
}

static int VC_PrivateMessage(int player, int target, const char * text)
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->EmitPlayerMessage(player, target, text);
    return _Core->GetState();
}

static int VC_InternalCommand(unsigned int type, const char * text)
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->EmitInternalCommand(type, text);
    return _Core->GetState();
}

static int VC_LoginAttempt(char * name, const char * passwd, const char * address)
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->EmitLoginAttempt(name, passwd, address);
    return _Core->GetState();
}

static void VC_EntityPool(int type, int id, unsigned int deleted)
{
    _Core->EmitEntityPool(type, id, static_cast<bool>(deleted));
}

static void VC_KeyBindDown(int player, int bind)
{
    _Core->EmitPlayerKeyPress(player, bind);
}

static void VC_KeyBindUp(int player, int bind)
{
    _Core->EmitPlayerKeyRelease(player, bind);
}

static void VC_PlayerAway(int player, unsigned int status)
{
    _Core->EmitPlayerAway(player, static_cast<bool>(status));
}

static void VC_PlayerSpectate(int player, int target)
{
    _Core->EmitPlayerSpectate(player, target);
}

static void VC_PlayerCrashReport(int player, const char * report)
{
    _Core->EmitPlayerCrashreport(player, report);
}

static void VC_ServerPerformanceReport(int count, const char ** description, unsigned long long * millis)
{
    // Ignored for now...
    SQMOD_UNUSED_VAR(count);
    SQMOD_UNUSED_VAR(description);
    SQMOD_UNUSED_VAR(millis);
}

static void VC_PlayerName(int player, const char * previous, const char * current)
{
    _Core->EmitPlayerRename(player, previous, current);
}

static void VC_PlayerState(int player, int previous, int current)
{
    _Core->EmitPlayerState(player, previous, current);

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

static void VC_PlayerAction(int player, int previous, int current)
{
    _Core->EmitPlayerAction(player, previous, current);

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

static void VC_PlayerOnFire(int player, unsigned int state)
{
    _Core->EmitPlayerBurning(player, static_cast<bool>(state));
}

static void VC_PlayerCrouch(int player, unsigned int state)
{
    _Core->EmitPlayerCrouching(player, static_cast<bool>(state));
}

static void VC_PlayerGameKeys(int player, int previous, int current)
{
    _Core->EmitPlayerGameKeys(player, previous, current);
}

static void VC_OnCheckpointEntered(int checkpoint, int player)
{
    _Core->EmitCheckpointEntered(player, checkpoint);
}

static void VC_OnCheckpointExited(int checkpoint, int player)
{
    _Core->EmitCheckpointExited(player, checkpoint);
}

static void VC_OnSphereEntered(int sphere, int player)
{
    _Core->EmitForcefieldEntered(player, sphere);
}

static void VC_OnSphereExited(int sphere, int player)
{
    _Core->EmitForcefieldExited(player, sphere);
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
    _Clbk->OnInitServer                 = NULL;
    _Clbk->OnShutdownServer             = NULL;
    _Clbk->OnFrame                      = NULL;
    _Clbk->OnPlayerConnect              = NULL;
    _Clbk->OnPlayerDisconnect           = NULL;
    _Clbk->OnPlayerBeginTyping          = NULL;
    _Clbk->OnPlayerEndTyping            = NULL;
    _Clbk->OnPlayerRequestClass         = NULL;
    _Clbk->OnPlayerRequestSpawn         = NULL;
    _Clbk->OnPlayerSpawn                = NULL;
    _Clbk->OnPlayerDeath                = NULL;
    _Clbk->OnPlayerUpdate               = NULL;
    _Clbk->OnPlayerRequestEnter         = NULL;
    _Clbk->OnPlayerEnterVehicle         = NULL;
    _Clbk->OnPlayerExitVehicle          = NULL;
    _Clbk->OnPickupClaimPicked          = NULL;
    _Clbk->OnPickupPickedUp             = NULL;
    _Clbk->OnPickupRespawn              = NULL;
    _Clbk->OnVehicleUpdate              = NULL;
    _Clbk->OnVehicleExplode             = NULL;
    _Clbk->OnVehicleRespawn             = NULL;
    _Clbk->OnObjectShot                 = NULL;
    _Clbk->OnObjectBump                 = NULL;
    _Clbk->OnPublicMessage              = NULL;
    _Clbk->OnCommandMessage             = NULL;
    _Clbk->OnPrivateMessage             = NULL;
    _Clbk->OnInternalCommand            = NULL;
    _Clbk->OnLoginAttempt               = NULL;
    _Clbk->OnEntityPoolChange           = NULL;
    _Clbk->OnKeyBindDown                = NULL;
    _Clbk->OnKeyBindUp                  = NULL;
    _Clbk->OnPlayerAwayChange           = NULL;
    _Clbk->OnPlayerSpectate             = NULL;
    _Clbk->OnPlayerCrashReport          = NULL;
    _Clbk->OnServerPerformanceReport    = NULL;
    _Clbk->OnPlayerNameChange           = NULL;
    _Clbk->OnPlayerStateChange          = NULL;
    _Clbk->OnPlayerActionChange         = NULL;
    _Clbk->OnPlayerOnFireChange         = NULL;
    _Clbk->OnPlayerCrouchChange         = NULL;
    _Clbk->OnPlayerGameKeysChange       = NULL;
    _Clbk->OnCheckpointEntered          = NULL;
    _Clbk->OnCheckpointExited           = NULL;
    _Clbk->OnSphereEntered              = NULL;
    _Clbk->OnSphereExited               = NULL;
}
