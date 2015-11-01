#include "Common.hpp"
#include "Logger.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>

#if defined(_WIN32)
#include <winsock2.h>
#endif // _WIN32

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// --------------------------------------------------------------------------------------------
PluginFuncs*        _Func = 0;
PluginCallbacks*    _Clbk = 0;
PluginInfo*         _Info = 0;

// ------------------------------------------------------------------------------------------------
void BindCallbacks() noexcept;

// ------------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs * funcs, PluginCallbacks * calls, PluginInfo * info)
{
#if defined(_WIN32)
    WSADATA wsaData;
#endif // _WIN32
    // Verify that core components are working
    if (!_Log)
    {
        puts("[SQMOD] Unable to start because the logging class could not be instantiated");
        return SQMOD_FAILURE;
    }
    else if (!_Core)
    {
        puts("[SQMOD] Unable to start because the central core class could not be instantiated");
        return SQMOD_FAILURE;
    }
#if defined(_WIN32)
    // Initialize the sockets on windows
    else if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        puts("[SQMOD] Unable to start because the windows sockets could not be initialized");
        return SQMOD_FAILURE;
    }
#endif // _WIN32
    else
    {
        _Func = funcs;
        _Clbk = calls;
        _Info = info;

        _Info->uPluginVer = SQMOD_VERSION;
        std::strcpy(_Info->szName, SQMOD_NAME);
    }

    _Log->Msg("%s", CenterStr("STARTUP", '*'));

    _Log->Inf("%s", CenterStr(SQMOD_NAME, ' '));
    _Log->Inf("%s", CenterStr(SQMOD_COPYRIGHT, ' '));
    _Log->Inf("%s", CenterStr(SQMOD_VERSION_STR, ' '));

    if (!_Core->Init())
    {
        _Log->Ftl("The plugin failed to initialize");
        _Log->Msg("%s", CenterStr("ABORTING", '*'));

        _Core->Deinit();

        return SQMOD_FAILURE;
    }
    else if (_Clbk)
    {
        BindCallbacks();
    }
    else
    {
        _Log->Ftl("Unable to start because the server callbacks are missing");

        return SQMOD_FAILURE;
    }

    return SQMOD_SUCCESS;
}

// --------------------------------------------------------------------------------------------
static int VC_InitServer(void) noexcept
{
    if (_Core->Load())
    {
        _Core->OnServerStartup();
    }
    else
    {
        _Log->Ftl("Unable to load the plugin resources properly");
    }
    return _Core->GetState();
}

static void VC_ShutdownServer(void) noexcept
{
    _Core->OnServerShutdown();
    _Core->Terminate();
}

static void VC_Frame(float delta) noexcept
{
    _Core->OnServerFrame(delta);
}

static void VC_PlayerConnect(int player) noexcept
{
    static SqObj playload;
    playload.Release();
    _Core->ConnectPlayer(player, SQMOD_CREATE_AUTOMATIC, playload);
}

static void VC_PlayerDisconnect(int player, int reason) noexcept
{
    static SqObj playload;
    playload.Release();
    _Core->DisconnectPlayer(player, reason, playload);
}

static void VC_PlayerBeginTyping(int player) noexcept
{
    _Core->OnPlayerStartTyping(player);
}

static void VC_PlayerEndTyping(int player) noexcept
{
    _Core->OnPlayerStopTyping(player);
}

static int VC_PlayerRequestClass(int player, int offset) noexcept
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->OnPlayerRequestClass(player, offset);
    return _Core->GetState();
}

static int VC_PlayerRequestSpawn(int player) noexcept
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->OnPlayerRequestSpawn(player);
    return _Core->GetState();
}

static void VC_PlayerSpawn(int player) noexcept
{
    _Core->OnPlayerSpawn(player);
}

static void VC_PlayerDeath(int player, int killer, int reason, int body_part) noexcept
{
    if (_Func->IsPlayerConnected(killer))
    {
        _Core->OnPlayerKilled(player, killer, reason, body_part);
    }
    else
    {
        _Core->OnPlayerWasted(player, reason);
    }
}

static void VC_PlayerUpdate(int player, int type) noexcept
{
    _Core->OnPlayerUpdate(player, type);
}

static int VC_PlayerRequestEnter(int player, int vehicle, int slot) noexcept
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->OnPlayerEmbarking(player, vehicle, slot);
    return _Core->GetState();
}

static void VC_PlayerEnterVehicle(int player, int vehicle, int slot) noexcept
{
    _Core->OnPlayerEmbarked(player, vehicle, slot);
}

static void VC_PlayerExitVehicle(int player, int vehicle) noexcept
{
    _Core->OnPlayerDisembark(player, vehicle);
}

static int VC_PickupClaimPicked(int pickup, int player) noexcept
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->OnPickupClaimed(player, pickup);
    return _Core->GetState();
}

static void VC_PickupPickedUp(int pickup, int player) noexcept
{
    _Core->OnPickupCollected(player, pickup);
}

static void VC_PickupRespawn(int pickup) noexcept
{
    _Core->OnPickupRespawn(pickup);
}

static void VC_VehicleUpdate(int vehicle, int type) noexcept
{
    _Core->OnVehicleUpdate(vehicle, type);
}

static void VC_VehicleExplode(int vehicle) noexcept
{
    _Core->OnVehicleExplode(vehicle);
}

static void VC_VehicleRespawn(int vehicle) noexcept
{
    _Core->OnVehicleRespawn(vehicle);
}

static void VC_ObjectShot(int object, int player, int weapon) noexcept
{
    _Core->OnObjectShot(player, object, weapon);
}

static void VC_ObjectBump(int object, int player) noexcept
{
    _Core->OnObjectBump(player, object);
}

static int VC_PublicMessage(int player, const char * text) noexcept
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->OnPlayerChat(player, static_cast<const SQChar *>(text));
    return _Core->GetState();
}

static int VC_CommandMessage(int player, const char * text) noexcept
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->OnPlayerCommand(player, static_cast<const SQChar *>(text));
    return _Core->GetState();
}

static int VC_PrivateMessage(int player, int target, const char * text) noexcept
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->OnPlayerMessage(player, target, static_cast<const SQChar *>(text));
    return _Core->GetState();
}

static int VC_InternalCommand(unsigned int type, const char * text) noexcept
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->OnInternalCommand(type, static_cast<const SQChar *>(text));
    return _Core->GetState();
}

static int VC_LoginAttempt(char * name, const char * passwd, const char * address) noexcept
{
    _Core->SetState(SQMOD_SUCCESS);
    _Core->OnLoginAttempt(static_cast<const SQChar *>(name), static_cast<const SQChar *>(passwd), static_cast<const SQChar *>(address));
    return _Core->GetState();
}

static void VC_EntityPool(int type, int id, unsigned int deleted) noexcept
{
    _Core->OnEntityPool(type, id, static_cast<bool>(deleted));
}

static void VC_KeyBindDown(int player, int bind) noexcept
{
    _Core->OnPlayerKeyPress(player, bind);
}

static void VC_KeyBindUp(int player, int bind) noexcept
{
    _Core->OnPlayerKeyRelease(player, bind);
}

static void VC_PlayerAway(int player, unsigned int status) noexcept
{
    _Core->OnPlayerAway(player, static_cast<bool>(status));
}

static void VC_PlayerSpectate(int player, int target) noexcept
{
    _Core->OnPlayerSpectate(player, target);
}

static void VC_PlayerCrashReport(int player, const char * report) noexcept
{
    _Core->OnPlayerCrashreport(player, static_cast<const SQChar *>(report));
}

static void VC_ServerPerformanceReport(int count, const char ** description, unsigned long long * millis) noexcept
{
    // Ignored for now...
    SQMOD_UNUSED_VAR(count);
    SQMOD_UNUSED_VAR(description);
    SQMOD_UNUSED_VAR(millis);
}

static void VC_PlayerName(int player, const char * previous, const char * current) noexcept
{
    _Core->OnPlayerName(player, static_cast<const SQChar *>(previous), static_cast<const SQChar *>(current));
}

static void VC_PlayerState(int player, int previous, int current) noexcept
{
    _Core->OnPlayerState(player, previous, current);

    switch (current)
    {
        case SQMOD_PLAYER_STATE_NONE:
            _Core->OnStateNone(player, previous);
        break;
        case SQMOD_PLAYER_STATE_NORMAL:
            _Core->OnStateNormal(player, previous);
        break;
        case SQMOD_PLAYER_STATE_SHOOTING:
            _Core->OnStateShooting(player, previous);
        break;
        case SQMOD_PLAYER_STATE_DRIVER:
            _Core->OnStateDriver(player, previous);
        break;
        case SQMOD_PLAYER_STATE_PASSENGER:
            _Core->OnStatePassenger(player, previous);
        break;
        case SQMOD_PLAYER_STATE_ENTERING_AS_DRIVER:
            _Core->OnStateEnterDriver(player, previous);
        break;
        case SQMOD_PLAYER_STATE_ENTERING_AS_PASSENGER:
            _Core->OnStateEnterPassenger(player, previous);
        break;
        case SQMOD_PLAYER_STATE_EXITING_VEHICLE:
            _Core->OnStateExitVehicle(player, previous);
        break;
        case SQMOD_PLAYER_STATE_UNSPAWNED:
            _Core->OnStateUnspawned(player, previous);
        break;
    }
}

static void VC_PlayerAction(int player, int previous, int current) noexcept
{
    _Core->OnPlayerAction(player, previous, current);

    switch (current)
    {
        case SQMOD_PLAYER_ACTION_NONE:
            _Core->OnActionNone(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_NORMAL:
            _Core->OnActionNormal(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_AIMING:
            _Core->OnActionAiming(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_SHOOTING:
            _Core->OnActionShooting(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_JUMPING:
            _Core->OnActionJumping(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_LYING_ON_GROUND:
            _Core->OnActionLieDown(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_GETTING_UP:
            _Core->OnActionGettingUp(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_JUMPING_FROM_VEHICLE:
            _Core->OnActionJumpVehicle(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_DRIVING:
            _Core->OnActionDriving(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_DYING:
            _Core->OnActionDying(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_WASTED:
            _Core->OnActionWasted(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_ENTERING_VEHICLE:
            _Core->OnActionEmbarking(player, previous);
        break;
        case SQMOD_PLAYER_ACTION_EXITING_VEHICLE:
            _Core->OnActionDisembarking(player, previous);
        break;
    }
}

static void VC_PlayerOnFire(int player, unsigned int state) noexcept
{
    _Core->OnPlayerBurning(player, static_cast<bool>(state));
}

static void VC_PlayerCrouch(int player, unsigned int state) noexcept
{
    _Core->OnPlayerCrouching(player, static_cast<bool>(state));
}

static void VC_PlayerGameKeys(int player, int previous, int current) noexcept
{
    _Core->OnPlayerGameKeys(player, previous, current);
}

static void VC_OnCheckpointEntered(int checkpoint, int player) noexcept
{
    _Core->OnCheckpointEntered(player, checkpoint);
}

static void VC_OnCheckpointExited(int checkpoint, int player) noexcept
{
    _Core->OnCheckpointExited(player, checkpoint);
}

static void VC_OnSphereEntered(int sphere, int player) noexcept
{
    _Core->OnSphereEntered(player, sphere);
}

static void VC_OnSphereExited(int sphere, int player) noexcept
{
    _Core->OnSphereExited(player, sphere);
}

// ------------------------------------------------------------------------------------------------
void BindCallbacks() noexcept
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
SqObj & NullData() noexcept
{
    static SqObj d;
    d.Release();
    return d;
}

} // Namespace:: SqMod
