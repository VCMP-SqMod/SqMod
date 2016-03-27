// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>
#include <cstdarg>

// ------------------------------------------------------------------------------------------------
#include <algorithm>

// ------------------------------------------------------------------------------------------------
#include <libtcc.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static SQChar g_Buffer[4096]; // Common buffer to reduce memory allocations.

// ------------------------------------------------------------------------------------------------
StateHnd::Handles StateHnd::s_Handles;

// ------------------------------------------------------------------------------------------------
SStr GetTempBuff()
{
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
Uint32 GetTempBuffSize()
{
    return sizeof(g_Buffer);
}

// ------------------------------------------------------------------------------------------------
void SqThrowF(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (vsnprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
    {
        strcpy(g_Buffer, "Unknown error has occurred");
    }
    // Release the argument list
    va_end(args);
    // Throw the exception with the resulted message
    throw Sqrat::Exception(g_Buffer);
}

// ------------------------------------------------------------------------------------------------
CSStr FmtStr(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (snprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
        g_Buffer[0] = 0; /* make sure the string is terminated */
    // Release the argument list
    va_end(args);
    // Return the data from the buffer
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
StackGuard::StackGuard(HSQUIRRELVM vm)
    : m_Top(sq_gettop(vm)), m_VM(vm)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
StackGuard::~StackGuard()
{
    sq_pop(m_VM, sq_gettop(m_VM) - m_Top);
}

// --------------------------------------------------------------------------------------------
StackStrF::StackStrF(HSQUIRRELVM vm, SQInteger idx, bool fmt)
    : mPtr(nullptr)
    , mLen(-1)
    , mRes(SQ_OK)
    , mObj()
    , mVM(vm)
{
    const Int32 top = sq_gettop(vm);
    // Reset the converted value object
    sq_resetobject(&mObj);
    // Was the string or value specified?
    if (top <= (idx - 1))
    {
        mRes = sq_throwerror(vm, "Missing string or value");
    }
    // Do we have enough values to call the format function and are we allowed to?
    else if (top > idx && fmt)
    {
        // Pointer to the generated string
        SStr str = nullptr;
        // Attempt to generate the specified string format
        mRes = sqstd_format(vm, idx, &mLen, &str);
        // Did the format succeeded but ended up with a null string pointer?
        if (SQ_SUCCEEDED(mRes) && !str)
        {
            mRes = sq_throwerror(vm, "Unable to generate the string");
        }
        else
        {
            mPtr = const_cast< CSStr >(str);
        }
    }
    // Is the value on the stack an actual string?
    else if (sq_gettype(vm, idx) == OT_STRING)
    {
        // Obtain a reference to the string object
        mRes = sq_getstackobj(vm, idx, &mObj);
        // Could we retrieve the object from the stack?
        if (SQ_SUCCEEDED(mRes))
        {
            // Keep a strong reference to the object
            sq_addref(vm, &mObj);
            // Attempt to retrieve the string value from the stack
            mRes = sq_getstring(vm, idx, &mPtr);
        }
        // Did the retrieval succeeded but ended up with a null string pointer?
        if (SQ_SUCCEEDED(mRes) && !mPtr)
        {
            mRes = sq_throwerror(vm, "Unable to retrieve the string");
        }
    }
    // We have to try and convert it to string
    else
    {
        // Attempt to convert the value from the stack to a string
        mRes = sq_tostring(vm, idx);
        // Could we convert the specified value to string?
        if (SQ_SUCCEEDED(mRes))
        {
            // Obtain a reference to the resulted object
            mRes = sq_getstackobj(vm, -1, &mObj);
            // Could we retrieve the object from the stack?
            if (SQ_SUCCEEDED(mRes))
            {
                // Keep a strong reference to the object
                sq_addref(vm, &mObj);
                // Attempt to obtain the string pointer
                mRes = sq_getstring(vm, -1, &mPtr);
            }
        }
        // Pop a value from the stack regardless of the result
        sq_pop(vm, 1);
        // Did the retrieval succeeded but ended up with a null string pointer?
        if (SQ_SUCCEEDED(mRes) && !mPtr)
        {
            mRes = sq_throwerror(vm, "Unable to retrieve the value");
        }
    }
}

// ------------------------------------------------------------------------------------------------
StackStrF::~StackStrF()
{
    if (mVM && !sq_isnull(mObj))
    {
        sq_release(mVM, &mObj);
    }
}

// ------------------------------------------------------------------------------------------------
void StateHnd::ErrorHandler(void * opaque, const char * msg)
{
    // Cast the opaque pointer when created to the associated handle instance
    Handle * hnd = reinterpret_cast< Handle * >(opaque);
    // Is there a valid handle instance associated?
    if (!hnd)
    {
        // Just drop the message to the regular error output
        OutputError(msg);
    }
    // Is there a custom error handler assigned by the script?
    else if (hnd->mErrorHandler.IsNull())
    {
        // Are we allowed to handle the error in absence of this handler?
        if (!(hnd->mNoErrors))
        {
            // Just drop the message to the regular error output
            OutputError(msg);
        }
    }
    else
    {
        // Attempt to forward the error to the script callback
        try
        {
            hnd->mErrorHandler.Execute(msg);
        }
        catch (const Sqrat::Exception & e)
        {
            OutputError("Error handler failed: %s", e.Message().c_str());
        }
        catch (...)
        {
            OutputError("Error handler failed for unknown reasons");
        }
    }
}

// ------------------------------------------------------------------------------------------------
StateHnd::Handle::Handle(Counter counter, CSStr name)
    : mPtr(tcc_new())
    , mRef(counter)
    , mOnInitServer(nullptr)
    , mOnShutdownServer(nullptr)
    , mOnFrame(nullptr)
    , mOnPlayerConnect(nullptr)
    , mOnPlayerDisconnect(nullptr)
    , mOnPlayerBeginTyping(nullptr)
    , mOnPlayerEndTyping(nullptr)
    , mOnPlayerRequestClass(nullptr)
    , mOnPlayerRequestSpawn(nullptr)
    , mOnPlayerSpawn(nullptr)
    , mOnPlayerDeath(nullptr)
    , mOnPlayerUpdate(nullptr)
    , mOnPlayerRequestEnter(nullptr)
    , mOnPlayerEnterVehicle(nullptr)
    , mOnPlayerExitVehicle(nullptr)
    , mOnPlayerNameChange(nullptr)
    , mOnPlayerStateChange(nullptr)
    , mOnPlayerActionChange(nullptr)
    , mOnPlayerOnFireChange(nullptr)
    , mOnPlayerCrouchChange(nullptr)
    , mOnPlayerGameKeysChange(nullptr)
    , mOnPickupClaimPicked(nullptr)
    , mOnPickupPickedUp(nullptr)
    , mOnPickupRespawn(nullptr)
    , mOnVehicleUpdate(nullptr)
    , mOnVehicleExplode(nullptr)
    , mOnVehicleRespawn(nullptr)
    , mOnObjectShot(nullptr)
    , mOnObjectBump(nullptr)
    , mOnPublicMessage(nullptr)
    , mOnCommandMessage(nullptr)
    , mOnPrivateMessage(nullptr)
    , mOnInternalCommand(nullptr)
    , mOnLoginAttempt(nullptr)
    , mOnEntityPoolChange(nullptr)
    , mOnKeyBindDown(nullptr)
    , mOnKeyBindUp(nullptr)
    , mOnPlayerAwayChange(nullptr)
    , mOnPlayerSpectate(nullptr)
    , mOnPlayerCrashReport(nullptr)
    , mOnServerPerformanceReport(nullptr)
    , mOnCheckpointEntered(nullptr)
    , mOnCheckpointExited(nullptr)
    , mOnSphereEntered(nullptr)
    , mOnSphereExited(nullptr)
    , mRelocated(false)
    , mNoErrors(false)
    , mErrorHandler()
    , mName(name ? name : _SC(""))
{
    // See if the state could be created
    if (!mPtr)
    {
        STHROWF("Unable to create compiler state");
    }
    else
    {
        // Make the state output data to memory
        tcc_set_output_type(mPtr, TCC_OUTPUT_MEMORY);
        // Assign our error function
        tcc_set_error_func(mPtr, this, &StateHnd::ErrorHandler);
        // Receive events
        Remember(this);
    }
}

// ------------------------------------------------------------------------------------------------
StateHnd::Handle::~Handle()
{
    // Stop receiving events
    Forget(this);
    // Do we have any state to destroy?
    if (mPtr != nullptr)
    {
        // Release the state resources
        tcc_delete(mPtr);
    }
    // Release the error callback
    mErrorHandler.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
void StateHnd::Validate() const
{
    if (!m_Hnd || !(m_Hnd->mPtr))
    {
        STHROWF("Invalid TCC state reference");
    }
}

// ------------------------------------------------------------------------------------------------
void StateHnd::PushSymbol(CSStr name, const void * value)
{
    // Validate the managed handle
    Validate();
    // Perform the requested operation
    tcc_add_symbol(m_Hnd->mPtr, name, value);
}

// ------------------------------------------------------------------------------------------------
void * StateHnd::PullSymbol(CSStr name) const
{
    // Validate the managed handle
    Validate();
    // Return the requested information
    return tcc_get_symbol(m_Hnd->mPtr, name);
}

// ------------------------------------------------------------------------------------------------
void StateHnd::SetOptions(CSStr opt) const
{
    // Validate the managed handle
    Validate();
    // Perform the requested operation
    tcc_set_options(m_Hnd->mPtr, opt);
}

// ------------------------------------------------------------------------------------------------
void StateHnd::DefineSymbol(CSStr sym, CSStr value) const
{
    // Validate the managed handle
    Validate();
    // Perform the requested operation
    tcc_define_symbol(m_Hnd->mPtr, sym, value);
}

// ------------------------------------------------------------------------------------------------
void StateHnd::UndefineSymbol(CSStr sym) const
{
    // Validate the managed handle
    Validate();
    // Perform the requested operation
    tcc_undefine_symbol(m_Hnd->mPtr, sym);
}

// ------------------------------------------------------------------------------------------------
void StateHnd::AddIncludePath(CSStr path) const
{
    // Validate the managed handle
    Validate();
    // Perform the requested operation
    tcc_add_include_path(m_Hnd->mPtr, path);
}

// ------------------------------------------------------------------------------------------------
void StateHnd::AddSysIncludePath(CSStr path) const
{
    // Validate the managed handle
    Validate();
    // Perform the requested operation
    tcc_add_sysinclude_path(m_Hnd->mPtr, path);
}

// ------------------------------------------------------------------------------------------------
void StateHnd::SetLibPath(CSStr path) const
{
    // Validate the managed handle
    Validate();
    // Perform the requested operation
    tcc_set_lib_path(m_Hnd->mPtr, path);
}

// ------------------------------------------------------------------------------------------------
void StateHnd::AddLibraryPath(CSStr path) const
{
    // Validate the managed handle
    Validate();
    // Perform the requested operation
    tcc_add_library_path(m_Hnd->mPtr, path);
}

// ------------------------------------------------------------------------------------------------
void StateHnd::AddLibrary(CSStr name) const
{
    // Validate the managed handle
    Validate();
    // Perform the requested operation
    if (tcc_add_library(m_Hnd->mPtr, name) < 0)
    {
        STHROWF("Unable to add library: '%s'", name);
    }
}

// ------------------------------------------------------------------------------------------------
void StateHnd::CompileString(CSStr str) const
{
    // Validate the managed handle
    Validate();
    // Perform the requested operation
    if (tcc_compile_string(m_Hnd->mPtr, str) < 0)
    {
        STHROWF("Unable to compile string");
    }
}

// ------------------------------------------------------------------------------------------------
void StateHnd::AddFile(CSStr filename, Int32 type) const
{
    // Validate the managed handle
    Validate();
    // Perform the requested operation
    if (tcc_add_file(m_Hnd->mPtr, filename, type) < 0)
    {
        STHROWF("Unable to add file: '%s'", filename);
    }
}

// ------------------------------------------------------------------------------------------------
void StateHnd::Relocate() const
{
    // Validate the managed handle
    Validate();
    // Make sure the state was not already relocated
    if (m_Hnd->mRelocated)
    {
        STHROWF("Compiler state was already relocated");
    }
    // Attempt to relocate the state
    else if (tcc_relocate(m_Hnd->mPtr, TCC_RELOCATE_AUTO) < 0)
    {
        STHROWF("Unable to relocate compiler state");
    }
    // Specify that the compiler was relocated
    m_Hnd->mRelocated = true;
    // Retrieve the event callbacks
    m_Hnd->mOnInitServer                = GetSymbol< SDK_OnInitServer >("onInitServer");
    m_Hnd->mOnShutdownServer            = GetSymbol< SDK_OnShutdownServer >("onShutdownServer");
    m_Hnd->mOnFrame                     = GetSymbol< SDK_OnFrame >("onFrame");
    m_Hnd->mOnPlayerConnect             = GetSymbol< SDK_OnPlayerConnect >("onPlayerConnect");
    m_Hnd->mOnPlayerDisconnect          = GetSymbol< SDK_OnPlayerDisconnect >("onPlayerDisconnect");
    m_Hnd->mOnPlayerBeginTyping         = GetSymbol< SDK_OnPlayerBeginTyping >("onPlayerBeginTyping");
    m_Hnd->mOnPlayerEndTyping           = GetSymbol< SDK_OnPlayerEndTyping >("onPlayerEndTyping");
    m_Hnd->mOnPlayerRequestClass        = GetSymbol< SDK_OnPlayerRequestClass >("onPlayerRequestClass");
    m_Hnd->mOnPlayerRequestSpawn        = GetSymbol< SDK_OnPlayerRequestSpawn >("onPlayerRequestSpawn");
    m_Hnd->mOnPlayerSpawn               = GetSymbol< SDK_OnPlayerSpawn >("onPlayerSpawn");
    m_Hnd->mOnPlayerDeath               = GetSymbol< SDK_OnPlayerDeath >("onPlayerDeath");
    m_Hnd->mOnPlayerUpdate              = GetSymbol< SDK_OnPlayerUpdate >("onPlayerUpdate");
    m_Hnd->mOnPlayerRequestEnter        = GetSymbol< SDK_OnPlayerRequestEnter >("onPlayerRequestEnter");
    m_Hnd->mOnPlayerEnterVehicle        = GetSymbol< SDK_OnPlayerEnterVehicle >("onPlayerEnterVehicle");
    m_Hnd->mOnPlayerExitVehicle         = GetSymbol< SDK_OnPlayerExitVehicle >("onPlayerExitVehicle");
    m_Hnd->mOnPlayerNameChange          = GetSymbol< SDK_OnPlayerNameChange >("onPlayerNameChange");
    m_Hnd->mOnPlayerStateChange         = GetSymbol< SDK_OnPlayerStateChange >("onPlayerStateChange");
    m_Hnd->mOnPlayerActionChange        = GetSymbol< SDK_OnPlayerActionChange >("onPlayerActionChange");
    m_Hnd->mOnPlayerOnFireChange        = GetSymbol< SDK_OnPlayerOnFireChange >("onPlayerOnFireChange");
    m_Hnd->mOnPlayerCrouchChange        = GetSymbol< SDK_OnPlayerCrouchChange >("onPlayerCrouchChange");
    m_Hnd->mOnPlayerGameKeysChange      = GetSymbol< SDK_OnPlayerGameKeysChange >("onPlayerGameKeysChange");
    m_Hnd->mOnPickupClaimPicked         = GetSymbol< SDK_OnPickupClaimPicked >("onPickupClaimPicked");
    m_Hnd->mOnPickupPickedUp            = GetSymbol< SDK_OnPickupPickedUp >("onPickupPickedUp");
    m_Hnd->mOnPickupRespawn             = GetSymbol< SDK_OnPickupRespawn >("onPickupRespawn");
    m_Hnd->mOnVehicleUpdate             = GetSymbol< SDK_OnVehicleUpdate >("onVehicleUpdate");
    m_Hnd->mOnVehicleExplode            = GetSymbol< SDK_OnVehicleExplode >("onVehicleExplode");
    m_Hnd->mOnVehicleRespawn            = GetSymbol< SDK_OnVehicleRespawn >("onVehicleRespawn");
    m_Hnd->mOnObjectShot                = GetSymbol< SDK_OnObjectShot >("onObjectShot");
    m_Hnd->mOnObjectBump                = GetSymbol< SDK_OnObjectBump >("onObjectBump");
    m_Hnd->mOnPublicMessage             = GetSymbol< SDK_OnPublicMessage >("onPublicMessage");
    m_Hnd->mOnCommandMessage            = GetSymbol< SDK_OnCommandMessage >("onCommandMessage");
    m_Hnd->mOnPrivateMessage            = GetSymbol< SDK_OnPrivateMessage >("onPrivateMessage");
    m_Hnd->mOnInternalCommand           = GetSymbol< SDK_OnInternalCommand >("onInternalCommand");
    m_Hnd->mOnLoginAttempt              = GetSymbol< SDK_OnLoginAttempt >("onLoginAttempt");
    m_Hnd->mOnEntityPoolChange          = GetSymbol< SDK_OnEntityPoolChange >("onEntityPoolChange");
    m_Hnd->mOnKeyBindDown               = GetSymbol< SDK_OnKeyBindDown >("onKeyBindDown");
    m_Hnd->mOnKeyBindUp                 = GetSymbol< SDK_OnKeyBindUp >("onKeyBindUp");
    m_Hnd->mOnPlayerAwayChange          = GetSymbol< SDK_OnPlayerAwayChange >("onPlayerAwayChange");
    m_Hnd->mOnPlayerSpectate            = GetSymbol< SDK_OnPlayerSpectate >("onPlayerSpectate");
    m_Hnd->mOnPlayerCrashReport         = GetSymbol< SDK_OnPlayerCrashReport >("onPlayerCrashReport");
    m_Hnd->mOnServerPerformanceReport   = GetSymbol< SDK_OnServerPerformanceReport >("onServerPerformanceReport");
    m_Hnd->mOnCheckpointEntered         = GetSymbol< SDK_OnCheckpointEntered >("onCheckpointEntered");
    m_Hnd->mOnCheckpointExited          = GetSymbol< SDK_OnCheckpointExited >("onCheckpointExited");
    m_Hnd->mOnSphereEntered             = GetSymbol< SDK_OnSphereEntered >("onSphereEntered");
    m_Hnd->mOnSphereExited              = GetSymbol< SDK_OnSphereExited >("onSphereExited");
}

// ------------------------------------------------------------------------------------------------
bool StateHnd::IsListed(Handle * hnd)
{
    return (std::find(s_Handles.cbegin(), s_Handles.cend(), hnd) != s_Handles.cend());
}

// ------------------------------------------------------------------------------------------------
void StateHnd::Remember(Handle * hnd)
{
    // Make sure this handle doesn't exist already
    if (!IsListed(hnd))
    {
        s_Handles.push_back(hnd);
    }
}

// ------------------------------------------------------------------------------------------------
void StateHnd::Forget(Handle * hnd)
{
    // Attempt to find this handle in the list
    Handles::const_iterator itr = std::find(s_Handles.cbegin(), s_Handles.cend(), hnd);
    // Does this handle exist in the list?
    if (itr != s_Handles.cend())
    {
        s_Handles.erase(itr);
    }
}

// ------------------------------------------------------------------------------------------------
void StateHnd::ToHead(Handle * hnd)
{
    // Attempt to find this handle in the list
    Handles::const_iterator itr = std::find(s_Handles.cbegin(), s_Handles.cend(), hnd);
    // Does this handle exist in the list?
    if (itr != s_Handles.cend())
    {
        // Erase it from the current position
        s_Handles.erase(itr);
    }
    // Insert it at the beginning of the list
    s_Handles.insert(s_Handles.cbegin(), hnd);
}

// ------------------------------------------------------------------------------------------------
int StateHnd::OnInitServer(void)
{
    // Allow to startup by default
    int res = 1;
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnInitServer)
        {
            // Forward the event and save the result
            res = hnd->mOnInitServer();
        }
        // Are we allowed to continue?
        if (!res)
        {
            break;
        }
    }
    // Return the result
    return res;
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnShutdownServer(void)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnShutdownServer)
        {
            // Forward the event
            hnd->mOnShutdownServer();
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnFrame(float delta)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnFrame)
        {
            // Forward the event
            hnd->mOnFrame(delta);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerConnect(int player)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerConnect)
        {
            // Forward the event
            hnd->mOnPlayerConnect(player);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerDisconnect(int player, int reason)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerDisconnect)
        {
            // Forward the event
            hnd->mOnPlayerDisconnect(player, reason);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerBeginTyping(int player)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerBeginTyping)
        {
            // Forward the event
            hnd->mOnPlayerBeginTyping(player);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerEndTyping(int player)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerEndTyping)
        {
            // Forward the event
            hnd->mOnPlayerEndTyping(player);
        }
    }
}


// ------------------------------------------------------------------------------------------------
int StateHnd::OnPlayerRequestClass(int player, int offset)
{
    // Allow to startup by default
    int res = 1;
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerRequestClass)
        {
            // Forward the event and save the result
            res = hnd->mOnPlayerRequestClass(player, offset);
        }
        // Are we allowed to continue?
        if (!res)
        {
            break;
        }
    }
    // Return the result
    return res;
}


// ------------------------------------------------------------------------------------------------
int StateHnd::OnPlayerRequestSpawn(int player)
{
    // Allow to startup by default
    int res = 1;
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerRequestSpawn)
        {
            // Forward the event and save the result
            res = hnd->mOnPlayerRequestSpawn(player);
        }
        // Are we allowed to continue?
        if (!res)
        {
            break;
        }
    }
    // Return the result
    return res;
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerSpawn(int player)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerSpawn)
        {
            // Forward the event
            hnd->mOnPlayerSpawn(player);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerDeath(int player, int killer, int reason, int body_part)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerDeath)
        {
            // Forward the event
            hnd->mOnPlayerDeath(player, killer, reason, body_part);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerUpdate(int player, int type)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerUpdate)
        {
            // Forward the event
            hnd->mOnPlayerUpdate(player, type);
        }
    }
}


// ------------------------------------------------------------------------------------------------
int StateHnd::OnPlayerRequestEnter(int player, int vehicle, int slot)
{
    // Allow to startup by default
    int res = 1;
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerRequestEnter)
        {
            // Forward the event and save the result
            res = hnd->mOnPlayerRequestEnter(player, vehicle, slot);
        }
        // Are we allowed to continue?
        if (!res)
        {
            break;
        }
    }
    // Return the result
    return res;
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerEnterVehicle(int player, int vehicle, int slot)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerEnterVehicle)
        {
            // Forward the event
            hnd->mOnPlayerEnterVehicle(player, vehicle, slot);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerExitVehicle(int player, int vehicle)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerExitVehicle)
        {
            // Forward the event
            hnd->mOnPlayerExitVehicle(player, vehicle);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerNameChange(int player, const char * previous, const char * current)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerNameChange)
        {
            // Forward the event
            hnd->mOnPlayerNameChange(player, previous, current);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerStateChange(int player, int previous, int current)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerStateChange)
        {
            // Forward the event
            hnd->mOnPlayerStateChange(player, previous, current);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerActionChange(int player, int previous, int current)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerActionChange)
        {
            // Forward the event
            hnd->mOnPlayerActionChange(player, previous, current);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerOnFireChange(int player, unsigned int state)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerOnFireChange)
        {
            // Forward the event
            hnd->mOnPlayerOnFireChange(player, state);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerCrouchChange(int player, unsigned int state)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerCrouchChange)
        {
            // Forward the event
            hnd->mOnPlayerCrouchChange(player, state);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerGameKeysChange(int player, int previous, int current)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerGameKeysChange)
        {
            // Forward the event
            hnd->mOnPlayerGameKeysChange(player, previous, current);
        }
    }
}


// ------------------------------------------------------------------------------------------------
int StateHnd::OnPickupClaimPicked(int pickup, int player)
{
    // Allow to startup by default
    int res = 1;
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPickupClaimPicked)
        {
            // Forward the event and save the result
            res = hnd->mOnPickupClaimPicked(pickup, player);
        }
        // Are we allowed to continue?
        if (!res)
        {
            break;
        }
    }
    // Return the result
    return res;
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPickupPickedUp(int pickup, int player)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPickupPickedUp)
        {
            // Forward the event
            hnd->mOnPickupPickedUp(pickup, player);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPickupRespawn(int pickup)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPickupRespawn)
        {
            // Forward the event
            hnd->mOnPickupRespawn(pickup);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnVehicleUpdate(int vehicle, int type)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnVehicleUpdate)
        {
            // Forward the event
            hnd->mOnVehicleUpdate(vehicle, type);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnVehicleExplode(int vehicle)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnVehicleExplode)
        {
            // Forward the event
            hnd->mOnVehicleExplode(vehicle);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnVehicleRespawn(int vehicle)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnVehicleRespawn)
        {
            // Forward the event
            hnd->mOnVehicleRespawn(vehicle);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnObjectShot(int object, int player, int weapon)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnObjectShot)
        {
            // Forward the event
            hnd->mOnObjectShot(object, player, weapon);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnObjectBump(int object, int player)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnObjectBump)
        {
            // Forward the event
            hnd->mOnObjectBump(object, player);
        }
    }
}


// ------------------------------------------------------------------------------------------------
int StateHnd::OnPublicMessage(int player, const char * message)
{
    // Allow to startup by default
    int res = 1;
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPublicMessage)
        {
            // Forward the event and save the result
            res = hnd->mOnPublicMessage(player, message);
        }
        // Are we allowed to continue?
        if (!res)
        {
            break;
        }
    }
    // Return the result
    return res;
}


// ------------------------------------------------------------------------------------------------
int StateHnd::OnCommandMessage(int player, const char * command)
{
    // Allow to startup by default
    int res = 1;
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnCommandMessage)
        {
            // Forward the event and save the result
            res = hnd->mOnCommandMessage(player, command);
        }
        // Are we allowed to continue?
        if (!res)
        {
            break;
        }
    }
    // Return the result
    return res;
}


// ------------------------------------------------------------------------------------------------
int StateHnd::OnPrivateMessage(int player, int target, const char * message)
{
    // Allow to startup by default
    int res = 1;
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPrivateMessage)
        {
            // Forward the event and save the result
            res = hnd->mOnPrivateMessage(player, target, message);
        }
        // Are we allowed to continue?
        if (!res)
        {
            break;
        }
    }
    // Return the result
    return res;
}


// ------------------------------------------------------------------------------------------------
int StateHnd::OnInternalCommand(unsigned int type, const char * text)
{
    // Allow to startup by default
    int res = 1;
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnInternalCommand)
        {
            // Forward the event and save the result
            res = hnd->mOnInternalCommand(type, text);
        }
        // Are we allowed to continue?
        if (!res)
        {
            break;
        }
    }
    // Return the result
    return res;
}


// ------------------------------------------------------------------------------------------------
int StateHnd::OnLoginAttempt(char * name, const char * passwd, const char * ipaddr)
{
    // Disallow to startup by default
    int res = 0;
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnLoginAttempt)
        {
            // Forward the event and save the result
            res = hnd->mOnLoginAttempt(name, passwd, ipaddr);
        }
        // Are we allowed to continue?
        if (!res)
        {
            break;
        }
    }
    // Return the result
    return res;
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnEntityPoolChange(int type, int entity, unsigned int deleted)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnEntityPoolChange)
        {
            // Forward the event
            hnd->mOnEntityPoolChange(type, entity, deleted);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnKeyBindDown(int player, int keybind)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnKeyBindDown)
        {
            // Forward the event
            hnd->mOnKeyBindDown(player, keybind);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnKeyBindUp(int player, int keybind)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnKeyBindUp)
        {
            // Forward the event
            hnd->mOnKeyBindUp(player, keybind);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerAwayChange(int player, unsigned int status)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerAwayChange)
        {
            // Forward the event
            hnd->mOnPlayerAwayChange(player, status);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerSpectate(int player, int target)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerSpectate)
        {
            // Forward the event
            hnd->mOnPlayerSpectate(player, target);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnPlayerCrashReport(int player, const char * report)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnPlayerCrashReport)
        {
            // Forward the event
            hnd->mOnPlayerCrashReport(player, report);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnServerPerformanceReport(int count, const char ** description, unsigned long long * millispent)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnServerPerformanceReport)
        {
            // Forward the event
            hnd->mOnServerPerformanceReport(count, description, millispent);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnCheckpointEntered(int checkpoint, int player)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnCheckpointEntered)
        {
            // Forward the event
            hnd->mOnCheckpointEntered(checkpoint, player);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnCheckpointExited(int checkpoint, int player)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnCheckpointExited)
        {
            // Forward the event
            hnd->mOnCheckpointExited(checkpoint, player);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnSphereEntered(int sphere, int player)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnSphereEntered)
        {
            // Forward the event
            hnd->mOnSphereEntered(sphere, player);
        }
    }
}


// ------------------------------------------------------------------------------------------------
void StateHnd::OnSphereExited(int sphere, int player)
{
    // Forward the call to the listed handles
    for (auto & hnd : s_Handles)
    {
        // Does the state have this callback?
        if (hnd->mOnSphereExited)
        {
            // Forward the event
            hnd->mOnSphereExited(sphere, player);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void StateHnd::ToggleEvent(Int32 evt, bool toggle)
{
    switch (evt)
    {
        case EvOnFrame:
            _Clbk->OnFrame                      = toggle ? &StateHnd::OnFrame : nullptr;
        break;
        case EvOnPlayerConnect:
            _Clbk->OnPlayerConnect              = toggle ? &StateHnd::OnPlayerConnect : nullptr;
        break;
        case EvOnPlayerDisconnect:
            _Clbk->OnPlayerDisconnect           = toggle ? &StateHnd::OnPlayerDisconnect : nullptr;
        break;
        case EvOnPlayerBeginTyping:
            _Clbk->OnPlayerBeginTyping          = toggle ? &StateHnd::OnPlayerBeginTyping : nullptr;
        break;
        case EvOnPlayerEndTyping:
            _Clbk->OnPlayerEndTyping            = toggle ? &StateHnd::OnPlayerEndTyping : nullptr;
        break;
        case EvOnPlayerRequestClass:
            _Clbk->OnPlayerRequestClass         = toggle ? &StateHnd::OnPlayerRequestClass : nullptr;
        break;
        case EvOnPlayerRequestSpawn:
            _Clbk->OnPlayerRequestSpawn         = toggle ? &StateHnd::OnPlayerRequestSpawn : nullptr;
        break;
        case EvOnPlayerSpawn:
            _Clbk->OnPlayerSpawn                = toggle ? &StateHnd::OnPlayerSpawn : nullptr;
        break;
        case EvOnPlayerDeath:
            _Clbk->OnPlayerDeath                = toggle ? &StateHnd::OnPlayerDeath : nullptr;
        break;
        case EvOnPlayerUpdate:
            _Clbk->OnPlayerUpdate               = toggle ? &StateHnd::OnPlayerUpdate : nullptr;
        break;
        case EvOnPlayerRequestEnter:
            _Clbk->OnPlayerRequestEnter         = toggle ? &StateHnd::OnPlayerRequestEnter : nullptr;
        break;
        case EvOnPlayerEnterVehicle:
            _Clbk->OnPlayerEnterVehicle         = toggle ? &StateHnd::OnPlayerEnterVehicle : nullptr;
        break;
        case EvOnPlayerExitVehicle:
            _Clbk->OnPlayerExitVehicle          = toggle ? &StateHnd::OnPlayerExitVehicle : nullptr;
        break;
        case EvOnPlayerNameChange:
            _Clbk->OnPlayerNameChange           = toggle ? &StateHnd::OnPlayerNameChange : nullptr;
        break;
        case EvOnPlayerStateChange:
            _Clbk->OnPlayerStateChange          = toggle ? &StateHnd::OnPlayerStateChange : nullptr;
        break;
        case EvOnPlayerActionChange:
            _Clbk->OnPlayerActionChange         = toggle ? &StateHnd::OnPlayerActionChange : nullptr;
        break;
        case EvOnPlayerOnFireChange:
            _Clbk->OnPlayerOnFireChange         = toggle ? &StateHnd::OnPlayerOnFireChange : nullptr;
        break;
        case EvOnPlayerCrouchChange:
            _Clbk->OnPlayerCrouchChange         = toggle ? &StateHnd::OnPlayerCrouchChange : nullptr;
        break;
        case EvOnPlayerGameKeysChange:
            _Clbk->OnPlayerGameKeysChange       = toggle ? &StateHnd::OnPlayerGameKeysChange : nullptr;
        break;
        case EvOnPickupClaimPicked:
            _Clbk->OnPickupClaimPicked          = toggle ? &StateHnd::OnPickupClaimPicked : nullptr;
        break;
        case EvOnPickupPickedUp:
            _Clbk->OnPickupPickedUp             = toggle ? &StateHnd::OnPickupPickedUp : nullptr;
        break;
        case EvOnPickupRespawn:
            _Clbk->OnPickupRespawn              = toggle ? &StateHnd::OnPickupRespawn : nullptr;
        break;
        case EvOnVehicleUpdate:
            _Clbk->OnVehicleUpdate              = toggle ? &StateHnd::OnVehicleUpdate : nullptr;
        break;
        case EvOnVehicleExplode:
            _Clbk->OnVehicleExplode             = toggle ? &StateHnd::OnVehicleExplode : nullptr;
        break;
        case EvOnVehicleRespawn:
            _Clbk->OnVehicleRespawn             = toggle ? &StateHnd::OnVehicleRespawn : nullptr;
        break;
        case EvOnObjectShot:
            _Clbk->OnObjectShot                 = toggle ? &StateHnd::OnObjectShot : nullptr;
        break;
        case EvOnObjectBump:
            _Clbk->OnObjectBump                 = toggle ? &StateHnd::OnObjectBump : nullptr;
        break;
        case EvOnPublicMessage:
            _Clbk->OnPublicMessage              = toggle ? &StateHnd::OnPublicMessage : nullptr;
        break;
        case EvOnCommandMessage:
            _Clbk->OnCommandMessage             = toggle ? &StateHnd::OnCommandMessage : nullptr;
        break;
        case EvOnPrivateMessage:
            _Clbk->OnPrivateMessage             = toggle ? &StateHnd::OnPrivateMessage : nullptr;
        break;
        case EvOnLoginAttempt:
            _Clbk->OnLoginAttempt               = toggle ? &StateHnd::OnLoginAttempt : nullptr;
        break;
        case EvOnEntityPoolChange:
            _Clbk->OnEntityPoolChange           = toggle ? &StateHnd::OnEntityPoolChange : nullptr;
        break;
        case EvOnKeyBindDown:
            _Clbk->OnKeyBindDown                = toggle ? &StateHnd::OnKeyBindDown : nullptr;
        break;
        case EvOnKeyBindUp:
            _Clbk->OnKeyBindUp                  = toggle ? &StateHnd::OnKeyBindUp : nullptr;
        break;
        case EvOnPlayerAwayChange:
            _Clbk->OnPlayerAwayChange           = toggle ? &StateHnd::OnPlayerAwayChange : nullptr;
        break;
        case EvOnPlayerSpectate:
            _Clbk->OnPlayerSpectate             = toggle ? &StateHnd::OnPlayerSpectate : nullptr;
        break;
        case EvOnPlayerCrashReport:
            _Clbk->OnPlayerCrashReport          = toggle ? &StateHnd::OnPlayerCrashReport : nullptr;
        break;
        case EvOnServerPerformanceReport:
            _Clbk->OnServerPerformanceReport    = toggle ? &StateHnd::OnServerPerformanceReport : nullptr;
        break;
        case EvOnCheckpointEntered:
            _Clbk->OnCheckpointEntered          = toggle ? &StateHnd::OnCheckpointEntered : nullptr;
        break;
        case EvOnCheckpointExited:
            _Clbk->OnCheckpointExited           = toggle ? &StateHnd::OnCheckpointExited : nullptr;
        break;
        case EvOnSphereEntered:
            _Clbk->OnSphereEntered              = toggle ? &StateHnd::OnSphereEntered : nullptr;
        break;
        case EvOnSphereExited:
            _Clbk->OnSphereExited               = toggle ? &StateHnd::OnSphereExited : nullptr;
        break;
        default:
            STHROWF("Unknown server event");
    }
}

} // Namespace:: SqMod
