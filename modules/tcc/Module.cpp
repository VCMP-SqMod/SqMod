// --------------------------------------------------------------------------------------------
#include "Module.hpp"
#include "Common.hpp"
#include "State.hpp"
#include "Symbol.hpp"

// --------------------------------------------------------------------------------------------
#include <sqrat.h>

// --------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

// --------------------------------------------------------------------------------------------
#if defined(WIN32) || defined(_WIN32)
    #include <Windows.h>
#endif

namespace SqMod {

// --------------------------------------------------------------------------------------------
PluginFuncs*        _Func = nullptr;
PluginCallbacks*    _Clbk = nullptr;
PluginInfo*         _Info = nullptr;

// --------------------------------------------------------------------------------------------
HSQAPI              _SqAPI = nullptr;
HSQEXPORTS          _SqMod = nullptr;
HSQUIRRELVM         _SqVM = nullptr;

/* ------------------------------------------------------------------------------------------------
 * Bind speciffic functions to certain server events.
*/
void BindCallbacks();

/* ------------------------------------------------------------------------------------------------
 * Undo changes made with BindCallbacks().
*/
void UnbindCallbacks();

/* --------------------------------------------------------------------------------------------
 * Register the module API under the specified virtual machine.
*/
void RegisterAPI(HSQUIRRELVM vm);

/* --------------------------------------------------------------------------------------------
 * Initialize the plugin by obtaining the API provided by the host plugin.
*/
void OnSquirrelInitialize()
{
    // Attempt to import the plugin API exported by the host plugin
    _SqMod = sq_api_import(_Func);
    // Did we failed to obtain the plugin exports?
    if(!_SqMod)
    {
        OutputError("Failed to attach [%s] on host plugin.", SQTCC_NAME);
    }
    else
    {
        // Obtain the Squirrel API
        _SqAPI = _SqMod->GetSquirrelAPI();
        // Expand the Squirrel API into global functions
        sq_api_expand(_SqAPI);
    }
}

/* --------------------------------------------------------------------------------------------
 * Load the module on the virtual machine provided by the host module.
*/
void OnSquirrelLoad()
{
    // Make sure that we have a valid plugin API
    if (!_SqMod)
    {
        return; /* Unable to proceed. */
    }
    // Obtain the Squirrel API and VM
    _SqVM = _SqMod->GetSquirrelVM();
    // Make sure that a valid virtual machine exists
    if (!_SqVM)
    {
        return; /* Unable to proceed. */
    }
    // Set this as the default database
    DefaultVM::Set(_SqVM);
    // Register the module API
    RegisterAPI(_SqVM);
    // Notify about the current status
    OutputMessage("Registered: %s", SQTCC_NAME);
}

/* --------------------------------------------------------------------------------------------
 * The virtual machine is about to be terminated and script resources should be released.
*/
void OnSquirrelTerminate()
{
    OutputMessage("Terminating: %s", SQTCC_NAME);
    // Release the current database (if any)
    DefaultVM::Set(nullptr);
    // Release script resources...
}

/* --------------------------------------------------------------------------------------------
 * Validate the module API to make sure we don't run into issues.
*/
bool CheckAPIVer(CCStr ver)
{
    // Obtain the numeric representation of the API version
    long vernum = std::strtol(ver, nullptr, 10);
    // Check against version mismatch
    if (vernum == SQMOD_API_VER)
    {
        return true;
    }
    // Log the incident
    OutputError("API version mismatch on %s", SQTCC_NAME);
    OutputMessage("=> Requested: %ld Have: %ld", vernum, SQMOD_API_VER);
    // Invoker should not attempt to communicate through the module API
    return false;
}

/* --------------------------------------------------------------------------------------------
 * React to command sent by other plugins.
*/
static int OnInternalCommand(unsigned int type, const char * text)
{
    // Forward the call to compiler states
    int res =  StateHnd::OnInternalCommand(type, text);
    // Now we can process it
    switch(type)
    {
        case SQMOD_INITIALIZE_CMD:
            if (CheckAPIVer(text))
            {
                OnSquirrelInitialize();
            }
        break;
        case SQMOD_LOAD_CMD:
            OnSquirrelLoad();
        break;
        case SQMOD_TERMINATE_CMD:
            OnSquirrelTerminate();
        break;
        default: break;
    }
    // Return the result
    return res;
}

/* --------------------------------------------------------------------------------------------
 * The server was initialized and this plugin was loaded successfully.
*/
static int OnInitServer()
{
    return StateHnd::OnInitServer();
}

static void OnShutdownServer(void)
{
    // Forward the call to compiler states
    StateHnd::OnShutdownServer();
    // The server may still send callbacks
    UnbindCallbacks();
}

// ------------------------------------------------------------------------------------------------
void BindCallbacks()
{
    _Clbk->OnInitServer                 = OnInitServer;
    _Clbk->OnInternalCommand            = OnInternalCommand;
    _Clbk->OnShutdownServer             = OnShutdownServer;
    _Clbk->OnFrame                      = &StateHnd::OnFrame;
    _Clbk->OnPlayerConnect              = &StateHnd::OnPlayerConnect;
    _Clbk->OnPlayerDisconnect           = &StateHnd::OnPlayerDisconnect;
    _Clbk->OnPlayerBeginTyping          = &StateHnd::OnPlayerBeginTyping;
    _Clbk->OnPlayerEndTyping            = &StateHnd::OnPlayerEndTyping;
    _Clbk->OnPlayerRequestClass         = &StateHnd::OnPlayerRequestClass;
    _Clbk->OnPlayerRequestSpawn         = &StateHnd::OnPlayerRequestSpawn;
    _Clbk->OnPlayerSpawn                = &StateHnd::OnPlayerSpawn;
    _Clbk->OnPlayerDeath                = &StateHnd::OnPlayerDeath;
    _Clbk->OnPlayerUpdate               = &StateHnd::OnPlayerUpdate;
    _Clbk->OnPlayerRequestEnter         = &StateHnd::OnPlayerRequestEnter;
    _Clbk->OnPlayerEnterVehicle         = &StateHnd::OnPlayerEnterVehicle;
    _Clbk->OnPlayerExitVehicle          = &StateHnd::OnPlayerExitVehicle;
    _Clbk->OnPlayerNameChange           = &StateHnd::OnPlayerNameChange;
    _Clbk->OnPlayerStateChange          = &StateHnd::OnPlayerStateChange;
    _Clbk->OnPlayerActionChange         = &StateHnd::OnPlayerActionChange;
    _Clbk->OnPlayerOnFireChange         = &StateHnd::OnPlayerOnFireChange;
    _Clbk->OnPlayerCrouchChange         = &StateHnd::OnPlayerCrouchChange;
    _Clbk->OnPlayerGameKeysChange       = &StateHnd::OnPlayerGameKeysChange;
    _Clbk->OnPickupClaimPicked          = &StateHnd::OnPickupClaimPicked;
    _Clbk->OnPickupPickedUp             = &StateHnd::OnPickupPickedUp;
    _Clbk->OnPickupRespawn              = &StateHnd::OnPickupRespawn;
    _Clbk->OnVehicleUpdate              = &StateHnd::OnVehicleUpdate;
    _Clbk->OnVehicleExplode             = &StateHnd::OnVehicleExplode;
    _Clbk->OnVehicleRespawn             = &StateHnd::OnVehicleRespawn;
    _Clbk->OnObjectShot                 = &StateHnd::OnObjectShot;
    _Clbk->OnObjectBump                 = &StateHnd::OnObjectBump;
    _Clbk->OnPublicMessage              = &StateHnd::OnPublicMessage;
    _Clbk->OnCommandMessage             = &StateHnd::OnCommandMessage;
    _Clbk->OnPrivateMessage             = &StateHnd::OnPrivateMessage;
    _Clbk->OnLoginAttempt               = &StateHnd::OnLoginAttempt;
    _Clbk->OnEntityPoolChange           = &StateHnd::OnEntityPoolChange;
    _Clbk->OnKeyBindDown                = &StateHnd::OnKeyBindDown;
    _Clbk->OnKeyBindUp                  = &StateHnd::OnKeyBindUp;
    _Clbk->OnPlayerAwayChange           = &StateHnd::OnPlayerAwayChange;
    _Clbk->OnPlayerSpectate             = &StateHnd::OnPlayerSpectate;
    _Clbk->OnPlayerCrashReport          = &StateHnd::OnPlayerCrashReport;
    _Clbk->OnServerPerformanceReport    = &StateHnd::OnServerPerformanceReport;
    _Clbk->OnCheckpointEntered          = &StateHnd::OnCheckpointEntered;
    _Clbk->OnCheckpointExited           = &StateHnd::OnCheckpointExited;
    _Clbk->OnSphereEntered              = &StateHnd::OnSphereEntered;
    _Clbk->OnSphereExited               = &StateHnd::OnSphereExited;
}

// ------------------------------------------------------------------------------------------------
void UnbindCallbacks()
{
    _Clbk->OnInitServer                 = nullptr;
    _Clbk->OnInternalCommand            = nullptr;
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
    _Clbk->OnPlayerNameChange           = nullptr;
    _Clbk->OnPlayerStateChange          = nullptr;
    _Clbk->OnPlayerActionChange         = nullptr;
    _Clbk->OnPlayerOnFireChange         = nullptr;
    _Clbk->OnPlayerCrouchChange         = nullptr;
    _Clbk->OnPlayerGameKeysChange       = nullptr;
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
    _Clbk->OnLoginAttempt               = nullptr;
    _Clbk->OnEntityPoolChange           = nullptr;
    _Clbk->OnKeyBindDown                = nullptr;
    _Clbk->OnKeyBindUp                  = nullptr;
    _Clbk->OnPlayerAwayChange           = nullptr;
    _Clbk->OnPlayerSpectate             = nullptr;
    _Clbk->OnPlayerCrashReport          = nullptr;
    _Clbk->OnServerPerformanceReport    = nullptr;
    _Clbk->OnCheckpointEntered          = nullptr;
    _Clbk->OnCheckpointExited           = nullptr;
    _Clbk->OnSphereEntered              = nullptr;
    _Clbk->OnSphereExited               = nullptr;
}

// --------------------------------------------------------------------------------------------
void RegisterAPI(HSQUIRRELVM vm)
{
    Table tccns(vm);

    tccns.Bind(_SC("State"), Class< State >(vm, _SC("SqTccState"))
        // Constructors
        .Ctor< CSStr >()
        // Metamethods
        .Func(_SC("_cmp"), &State::Cmp)
        .SquirrelFunc(_SC("_typename"), &State::Typename)
        .Func(_SC("_tostring"), &State::ToString)
        // Properties
        .Prop(_SC("Valid"), &State::IsValid)
        .Prop(_SC("Refs"), &State::GetRefCount)
        .Prop(_SC("Name"), &State::GetName, &State::SetName)
        .Prop(_SC("Events"), &State::GetEvents, &State::SetEvents)
        // Member Methods
        .Func(_SC("Release"), &State::Release)
        .Func(_SC("MakeHead"), &State::MakeHead)
        .Func(_SC("SetOptions"), &State::SetOptions)
        .Func(_SC("UndefineSymbol"), &State::UndefineSymbol)
        .Func(_SC("AddIncludePath"), &State::AddIncludePath)
        .Func(_SC("AddSysIncludePath"), &State::AddSysIncludePath)
        .Func(_SC("SetLibPath"), &State::SetLibPath)
        .Func(_SC("AddLibraryPath"), &State::AddLibraryPath)
        .Func(_SC("AddLibrary"), &State::AddLibrary)
        .Func(_SC("CompileString"), &State::CompileString)
        .Func(_SC("AddSource"), &State::AddSource)
        .Func(_SC("AddBinary"), &State::AddBinary)
        .Func(_SC("AddAsm"), &State::AddAsm)
        .Func(_SC("AddAsmPP"), &State::AddAsmPP)
        .Func(_SC("Relocate"), &State::Relocate)
        .Func(_SC("GetSymbol"), &State::GetSymbol)
        // Member Overloads
        .Overload< void (State::*)(CSStr) const >(_SC("DefineSymbol"), &State::DefineSymbol)
        .Overload< void (State::*)(CSStr, CSStr) const >(_SC("DefineSymbol"), &State::DefineSymbol)
    );

    tccns.Bind(_SC("Symbol"), Class< Symbol >(vm, _SC("SqTccSymbol"))
        // Constructors
        .Ctor< const State &, CSStr >()
        // Metamethods
        .Func(_SC("_cmp"), &Symbol::Cmp)
        .SquirrelFunc(_SC("_typename"), &Symbol::Typename)
        .Func(_SC("_tostring"), &Symbol::ToString)
        // Properties
        .Prop(_SC("Valid"), &Symbol::IsValid)
        .Prop(_SC("Refs"), &Symbol::GetRefCount)
        .Prop(_SC("State"), &Symbol::GetStateRef)
        // Member Methods
        .Func(_SC("Release"), &Symbol::Release)
        // Squirrel Methods
        .SquirrelFunc(_SC("Forward"), &Symbol::Forward)
    );

    tccns.Func(_SC("ToggleEvent"), &StateHnd::ToggleEvent);

    RootTable(vm).Bind(_SC("SqTCC"), tccns);

    ConstTable(vm).Enum(_SC("ETccEvent"), Enumeration(vm)
        .Const(_SC("Frame"),                        EvOnFrame)
        .Const(_SC("PlayerConnect"),                EvOnPlayerConnect)
        .Const(_SC("PlayerDisconnect"),             EvOnPlayerDisconnect)
        .Const(_SC("PlayerBeginTyping"),            EvOnPlayerBeginTyping)
        .Const(_SC("PlayerEndTyping"),              EvOnPlayerEndTyping)
        .Const(_SC("PlayerRequestClass"),           EvOnPlayerRequestClass)
        .Const(_SC("PlayerRequestSpawn"),           EvOnPlayerRequestSpawn)
        .Const(_SC("PlayerSpawn"),                  EvOnPlayerSpawn)
        .Const(_SC("PlayerDeath"),                  EvOnPlayerDeath)
        .Const(_SC("PlayerUpdate"),                 EvOnPlayerUpdate)
        .Const(_SC("PlayerRequestEnter"),           EvOnPlayerRequestEnter)
        .Const(_SC("PlayerEnterVehicle"),           EvOnPlayerEnterVehicle)
        .Const(_SC("PlayerExitVehicle"),            EvOnPlayerExitVehicle)
        .Const(_SC("PlayerNameChange"),             EvOnPlayerNameChange)
        .Const(_SC("PlayerStateChange"),            EvOnPlayerStateChange)
        .Const(_SC("PlayerActionChange"),           EvOnPlayerActionChange)
        .Const(_SC("PlayerOnFireChange"),           EvOnPlayerOnFireChange)
        .Const(_SC("PlayerCrouchChange"),           EvOnPlayerCrouchChange)
        .Const(_SC("PlayerGameKeysChange"),         EvOnPlayerGameKeysChange)
        .Const(_SC("PickupClaimPicked"),            EvOnPickupClaimPicked)
        .Const(_SC("PickupPickedUp"),               EvOnPickupPickedUp)
        .Const(_SC("PickupRespawn"),                EvOnPickupRespawn)
        .Const(_SC("VehicleUpdate"),                EvOnVehicleUpdate)
        .Const(_SC("VehicleExplode"),               EvOnVehicleExplode)
        .Const(_SC("VehicleRespawn"),               EvOnVehicleRespawn)
        .Const(_SC("ObjectShot"),                   EvOnObjectShot)
        .Const(_SC("ObjectBump"),                   EvOnObjectBump)
        .Const(_SC("PublicMessage"),                EvOnPublicMessage)
        .Const(_SC("CommandMessage"),               EvOnCommandMessage)
        .Const(_SC("PrivateMessage"),               EvOnPrivateMessage)
        .Const(_SC("LoginAttempt"),                 EvOnLoginAttempt)
        .Const(_SC("EntityPoolChange"),             EvOnEntityPoolChange)
        .Const(_SC("KeyBindDown"),                  EvOnKeyBindDown)
        .Const(_SC("KeyBindUp"),                    EvOnKeyBindUp)
        .Const(_SC("PlayerAwayChange"),             EvOnPlayerAwayChange)
        .Const(_SC("PlayerSpectate"),               EvOnPlayerSpectate)
        .Const(_SC("PlayerCrashReport"),            EvOnPlayerCrashReport)
        .Const(_SC("ServerPerformanceReport"),      EvOnServerPerformanceReport)
        .Const(_SC("CheckpointEntered"),            EvOnCheckpointEntered)
        .Const(_SC("CheckpointExited"),             EvOnCheckpointExited)
        .Const(_SC("SphereEntered"),                EvOnSphereEntered)
        .Const(_SC("SphereExited"),                 EvOnSphereExited)
    );
}

// --------------------------------------------------------------------------------------------
void OutputMessageImpl(const char * msg, va_list args)
{
#if defined(WIN32) || defined(_WIN32)
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN);
    printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    vprintf(msg, args);
    puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    printf("%c[0;32m[SQMOD]%c[0;37m", 27, 27);
    vprintf(msg, args);
    puts("");
#endif
}

// --------------------------------------------------------------------------------------------
void OutputErrorImpl(const char * msg, va_list args)
{
#if defined(WIN32) || defined(_WIN32)
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    vprintf(msg, args);
    puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    printf("%c[0;32m[SQMOD]%c[0;37m", 27, 27);
    vprintf(msg, args);
    puts("");
#endif
}

// --------------------------------------------------------------------------------------------
void OutputDebug(const char * msg, ...)
{
#ifdef _DEBUG
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
#else
    SQMOD_UNUSED_VAR(msg);
#endif
}

// --------------------------------------------------------------------------------------------
void OutputMessage(const char * msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

// --------------------------------------------------------------------------------------------
void OutputError(const char * msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputErrorImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

} // Namespace:: SqMod

// --------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs* functions, PluginCallbacks* callbacks, PluginInfo* info)
{
    using namespace SqMod;
    // Output plugin header
    puts("");
    OutputMessage("--------------------------------------------------------------------");
    OutputMessage("Plugin: %s", SQTCC_NAME);
    OutputMessage("Author: %s", SQTCC_AUTHOR);
    OutputMessage("Legal: %s", SQTCC_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    puts("");
    // Attempt to find the host plugin ID
    int host_plugin_id = functions->FindPlugin((char *)(SQMOD_HOST_NAME));
    // See if our plugin was loaded after the host plugin
    if (host_plugin_id < 0)
    {
        OutputError("%s could find the host plugin", SQTCC_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Should never reach this point but just in case
    else if (host_plugin_id > (info->nPluginId))
    {
        OutputError("%s loaded after the host plugin", SQTCC_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plugin information
    _Info->uPluginVer = SQTCC_VERSION;
    std::strcpy(_Info->szName, SQTCC_HOST_NAME);
    // Bind callbacks
    BindCallbacks();
    // Notify that the plugin was successfully loaded
    OutputMessage("Successfully loaded %s", SQTCC_NAME);
    // Dummy spacing
    puts("");
    // Done!
    return SQMOD_SUCCESS;
}
