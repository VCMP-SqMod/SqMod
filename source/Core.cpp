// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Logger.hpp"

// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Entity/Checkpoint.hpp"
#include "Entity/Keybind.hpp"
#include "Entity/Object.hpp"
#include "Entity/Pickup.hpp"
#include "Entity/Player.hpp"
#include "Entity/Vehicle.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqstdio.h>
#include <sqstdblob.h>
#include <sqstdmath.h>
#include <sqstdsystem.h>
#include <sqstdstring.h>
#include <SimpleIni.h>

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdarg>
#include <exception>
#include <stdexcept>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern bool RegisterAPI(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void InitializeRoutines();
extern void TerminateRoutines();
extern void InitializeCmdManager();
extern void TerminateCmdManager();

// ------------------------------------------------------------------------------------------------
extern Object & ReloadPayload();

// ------------------------------------------------------------------------------------------------
Core Core::s_Inst;

// ------------------------------------------------------------------------------------------------
Core::Core()
    : m_State(0)
    , m_VM(nullptr)
    , m_Scripts()
    , m_Options()
    , m_Blips()
    , m_Checkpoints()
    , m_Keybinds()
    , m_Objects()
    , m_Pickups()
    , m_Players()
    , m_Vehicles()
    , m_CircularLocks(0)
    , m_IncomingNameBuffer(nullptr)
    , m_IncomingNameCapacity(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Core::~Core()
{
    if (m_VM)
    {
        Terminate();
    }
}

// ------------------------------------------------------------------------------------------------
bool Core::Initialize()
{
    // Make sure the plug-in was not already initialized
    if (m_VM != nullptr)
    {
        OutputError("Plug-in was already initialized");
        return true;
    }

    CSimpleIniA conf(false, true, true);
    // Attempt to load the configurations from disk
    SI_Error ini_ret = conf.LoadFile("sqmod.ini");
    // See if the configurations could be loaded
    if (ini_ret < 0)
    {
        switch (ini_ret)
        {
            case SI_FAIL:
                OutputError("Failed to load the configuration file. Probably invalid");
            break;
            case SI_NOMEM:
                OutputError("Run out of memory while loading the configuration file");
            break;
            case SI_FILE:
                OutputError("Failed to load the configuration file. %s", std::strerror(errno));
            break;
            default:
                OutputError("Failed to load the configuration file for some unforeseen reason");
        }
        // Failed to load the configuration file
        return false;
    }

    // Initialize the log filename
    Logger::Get().SetLogFilename(conf.GetValue("Log", "Filename", nullptr));
    // Configure the logging timestamps
    Logger::Get().ToggleConsoleTime(conf.GetBoolValue("Log", "ConsoleTimestamp", false));
    Logger::Get().ToggleLogFileTime(conf.GetBoolValue("Log", "LogFileTimestamp", true));
    // Apply the specified logging filters only after initialization was completed
    Logger::Get().ToggleConsoleLevel(LOGL_DBG, conf.GetBoolValue("Log", "ConsoleDebug", true));
    Logger::Get().ToggleConsoleLevel(LOGL_USR, conf.GetBoolValue("Log", "ConsoleUser", true));
    Logger::Get().ToggleConsoleLevel(LOGL_SCS, conf.GetBoolValue("Log", "ConsoleSuccess", true));
    Logger::Get().ToggleConsoleLevel(LOGL_INF, conf.GetBoolValue("Log", "ConsoleInfo", true));
    Logger::Get().ToggleConsoleLevel(LOGL_WRN, conf.GetBoolValue("Log", "ConsoleWarning", true));
    Logger::Get().ToggleConsoleLevel(LOGL_ERR, conf.GetBoolValue("Log", "ConsoleError", true));
    Logger::Get().ToggleConsoleLevel(LOGL_FTL, conf.GetBoolValue("Log", "ConsoleFatal", true));
    Logger::Get().ToggleLogFileLevel(LOGL_DBG, conf.GetBoolValue("Log", "LogFileDebug", true));
    Logger::Get().ToggleLogFileLevel(LOGL_USR, conf.GetBoolValue("Log", "LogFileUser", true));
    Logger::Get().ToggleLogFileLevel(LOGL_SCS, conf.GetBoolValue("Log", "LogFileSuccess", true));
    Logger::Get().ToggleLogFileLevel(LOGL_INF, conf.GetBoolValue("Log", "LogFileInfo", true));
    Logger::Get().ToggleLogFileLevel(LOGL_WRN, conf.GetBoolValue("Log", "LogFileWarning", true));
    Logger::Get().ToggleLogFileLevel(LOGL_ERR, conf.GetBoolValue("Log", "LogFileError", true));
    Logger::Get().ToggleLogFileLevel(LOGL_FTL, conf.GetBoolValue("Log", "LogFileFatal", true));

    LogDbg("Resizing the entity containers");
    // Make sure the entity containers have the proper size
    m_Blips.resize(SQMOD_BLIP_POOL);
    m_Checkpoints.resize(SQMOD_CHECKPOINT_POOL);
    m_Keybinds.resize(SQMOD_KEYBIND_POOL);
    m_Objects.resize(SQMOD_OBJECT_POOL);
    m_Pickups.resize(SQMOD_PICKUP_POOL);
    m_Players.resize(SQMOD_PLAYER_POOL);
    m_Vehicles.resize(SQMOD_VEHICLE_POOL);

    // Attempt to read the virtual machine stack size
    const LongI stack_size = conf.GetLongValue("Squirrel", "StackSize", SQMOD_STACK_SIZE);
    // Make sure that the retrieved number is within range
    if (!stack_size)
    {
        LogWrn("Invalid virtual machine stack size: %lu", stack_size);
        // Stop the initialization process
        return false;
    }

    LogDbg("Creating a virtual machine (%ld stack size)", stack_size);
    // Attempt to create the script virtual machine
    m_VM = sq_open(ConvTo< SQInteger >::From(stack_size));
    // See if the virtual machine could be created
    if (cLogFtl(!m_VM, "Unable to create the virtual machine"))
    {
        return false; // Unable to load the plug-in properly!
    }

    // Set this as the default VM
    DefaultVM::Set(m_VM);
    // Configure error handling
    ErrorHandling::Enable(conf.GetBoolValue("Squirrel", "ErrorHandling", true));

    LogDbg("Registering the standard libraries");
    // Push the root table on the stack
    sq_pushroottable(m_VM);
    // Register the standard library on the pushed table
    sqstd_register_iolib(m_VM);
    sqstd_register_bloblib(m_VM);
    sqstd_register_mathlib(m_VM);
    sqstd_register_systemlib(m_VM);
    sqstd_register_stringlib(m_VM);
    // Pop the root table from the stack
    sq_pop(m_VM, 1);

    LogDbg("Setting the script output function");
    // Tell the VM to use these functions to output user on error messages
    sq_setprintfunc(m_VM, PrintFunc, ErrorFunc);

    LogDbg("Setting the script error handlers");
    // Tell the VM to trigger this function on compile time errors
    sq_setcompilererrorhandler(m_VM, CompilerErrorHandler);
    // Push the runtime error handler on the stack and create a closure
    sq_newclosure(m_VM, RuntimeErrorHandler, 0);
    // Tell the VM to trigger this function on runtime errors
    sq_seterrorhandler(m_VM);

    LogDbg("Registering the plug-in API");
    // Attempt to register the plug-in API
    if (cLogFtl(!RegisterAPI(m_VM), "Unable to register the plug-in API"))
    {
        return false; // Can't execute scripts without a valid API!
    }

    // Attempt to retrieve the list of scripts specified in the configuration file
    CSimpleIniA::TNamesDepend scripts;
    conf.GetAllValues("Scripts", "Source", scripts);
    // See if any script was specified
    if (scripts.size() <= 0 && !conf.GetBoolValue("Squirrel", "EmptyInit", false))
    {
        LogWrn("No scripts specified in the configuration file");
        // No point in loading the plug-in
        return false;
    }
    else
    {
        LogDbg("Found (%u) scripts in the configuration file", scripts.size());
        // Sort the list in it's original order
        scripts.sort(CSimpleIniA::Entry::LoadOrder());
        // Process each specified script paths
        for (const auto & script : scripts)
        {
            // Attempt to queue the specified script path for loading
            LoadScript(script.pItem);
        }
    }
    // See if any script could be queued for loading
    if (m_Scripts.empty() && !conf.GetBoolValue("Squirrel", "EmptyInit", false))
    {
        LogErr("No scripts loaded. No reason to load the plug-in");
        // No point in loading the plug-in
        return false;
    }

    LogDbg("Reading the options from the general section");
    // Read options only after loading was successful
    CSimpleIniA::TNamesDepend options;
    // Are there any options to read?
    if (conf.GetAllKeys("Options", options) || options.size() > 0)
    {
        LogDbg("Found (%u) options in the configuration file", options);
        // Process all the specified keys under the [Options] section
        for (const auto & option : options)
        {
            CSimpleIniA::TNamesDepend values;
            // Get the values of all keys with the same name
            if (!conf.GetAllValues("Options", option.pItem, values))
            {
                continue;
            }
            // Sort the keys in their original order
            values.sort(CSimpleIniA::Entry::LoadOrder());
            // Save each option option and overwrite existing value
            for (const auto & value : values)
            {
                m_Options[option.pItem] = value.pItem;
            }
        }
    }

    // Initialize routines
    InitializeRoutines();
    // Initialize commands
    InitializeCmdManager();

    // Initialization successful
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::Execute()
{
    // Are there any scripts to execute?
    if (cLogErr(m_Scripts.empty(), "No scripts to execute. Plug-in has no purpose"))
    {
        return false; // No reason to execute the plug-in
    }

    LogDbg("Signaling outside plug-ins to register their API");
    // Tell modules to do their monkey business
    _Func->SendPluginCommand(0xDEADBABE, "");

    LogDbg("Attempting to execute the specified scripts");
    // Go through each specified script
    for (auto & s : m_Scripts)
    {
        // Attempt to load and compile the script file
        try
        {
            s.second.CompileFile(s.first);
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to compile: %s", s.first.c_str());
            // Failed to executed properly
            return false;
        }
        // Attempt to execute the compiled script code
        try
        {
            s.second.Run();
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to execute: %s", s.first.c_str());
            // Failed to executed properly
            return false;
        }
        // At this point the script should be completely loaded
        LogScs("Successfully executed script: %s", s.first.c_str());
    }

    // Import already existing entities
    ImportPlayers();
    ImportBlips();
    ImportCheckpoints();
    ImportKeybinds();
    ImportObjects();
    ImportPickups();
    ImportVehicles();

    // Notify the script callback that the scripts were loaded
    EmitScriptLoaded();

    // Successfully executed
    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::Terminate()
{
    if (m_VM)
    {

        LogDbg("Signaling outside plug-ins to release their resources");
        // Tell modules to do their monkey business
        _Func->SendPluginCommand(0xDEADC0DE, "");
    }
    // Release all entity resources by clearing the containers
    m_Players.clear();
    m_Blips.clear();
    m_Keybinds.clear();
    m_Objects.clear();
    m_Pickups.clear();
    m_Checkpoints.clear();
    m_Scripts.clear();
    // Release all resources from routines
    TerminateRoutines();
    // Release all resources from command manager
    TerminateCmdManager();
    // In case there's a payload for reload
    ReloadPayload().Release();
    // Release null objects in case any reference to valid objects is stored in them
    NullArray().Release();
    NullTable().Release();
    NullObject().Release();
    NullFunction().ReleaseGently();
    // Is there a VM to close?
    if (m_VM)
    {
        // Release all script callbacks
        ResetFunc();
        // Release the script instances
        m_Scripts.clear();
        // Assertions during close may cause double delete/close!
        HSQUIRRELVM sq_vm = m_VM;
        m_VM = nullptr;
        // Attempt to close the VM
        sq_close(sq_vm);
    }
}

// ------------------------------------------------------------------------------------------------
bool Core::Reload(Int32 header, Object & payload)
{
    // Are we already reloading?
    if (m_CircularLocks & CCL_RELOAD_SCRIPTS)
    {
        return false; // Already reloading!
    }
    // Prevent circular reloads when we send plugin commands
    const BitGuardU32 bg(m_CircularLocks, static_cast< Uint32 >(CCL_RELOAD_SCRIPTS));
    // Allow reloading by default
    Core::Get().SetState(1);
    // Emit the reload event
    Core::Get().EmitScriptReload(header, payload);
    // Are we allowed to reload?
    if (!Core::Get().GetState())
    {
        return false; // Request denied!
    }
    // Terminate the current VM and release resources
    Terminate();
    // Attempt to initialize it the central core
    if (!Initialize())
    {
        return false; // Reload failed!
    }
    // Attempt to load resources
    else if (!Execute())
    {
        return false; // Reload failed!
    }
    // At this point the reload is complete
    return true;
}

// ------------------------------------------------------------------------------------------------
CSStr Core::GetOption(CSStr name) const
{
    Options::const_iterator elem = m_Options.find(name);
    return (elem == m_Options.end()) ? _SC("") : elem->second.c_str();
}

// ------------------------------------------------------------------------------------------------
CSStr Core::GetOption(CSStr name, CSStr value) const
{
    Options::const_iterator elem = m_Options.find(name);
    return (elem == m_Options.end()) ? value : elem->second.c_str();
}

// ------------------------------------------------------------------------------------------------
void Core::SetOption(CSStr name, CSStr value)
{
    m_Options[name] = value;
}

// ------------------------------------------------------------------------------------------------
bool Core::LoadScript(CSStr filepath)
{
    // Is the specified path empty?
    if (!filepath || *filepath == '\0')
    {
        return false; // Simply ignore it!
    }
    // Get the file path as a string
    String path(filepath);
    // See if it wasn't already loaded
    if (m_Scripts.find(path) != m_Scripts.end())
    {
        LogWrn("Script was specified before: %s", path.c_str());
    }
    // We don't compile the scripts yet. We just store their path and prepare the objects
    else
    {
        // Create a new script container and insert it into the script pool
        m_Scripts.emplace(std::move(path), Script(m_VM));
    }
    // At this point the script exists in the pool
    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::SetIncomingName(CSStr name)
{
    // Is there an incoming connection buffer that we can write to?
    if (!m_IncomingNameBuffer)
    {
        STHROWF("No incoming player name buffer available");
    }
    // Is the specified name valid?
    else if (!name || *name == '\0')
    {
        STHROWF("Invalid player name for incoming connection");
    }
    // Calculate the length of the specified name
    const size_t len = std::strlen(name);
    // Is the length of the name out of bounds?
    if (len > m_IncomingNameCapacity)
    {
        STHROWF("The specified name exceeds the designated buffer");
    }
    // Does the name satisfy the minimum length required?
    else if (len < 2)
    {
        STHROWF("The specified name needs to be at least 2 characters: %zu", len);
    }
    // Copy the specified name to the assigned buffer
    std::strncpy(m_IncomingNameBuffer, name, m_IncomingNameCapacity);
    // Make sure that the name inside the buffer is null terminated
    m_IncomingNameBuffer[len] = '\0';
}

// ------------------------------------------------------------------------------------------------
void Core::PrintFunc(HSQUIRRELVM /*vm*/, CSStr msg, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, msg);
    // Forward the message to the logger
    Logger::Get().Send(LOGL_USR, false, msg, args);
    // Finalize the variable argument list
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Core::ErrorFunc(HSQUIRRELVM /*vm*/, CSStr msg, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, msg);
    // Tell the logger to display debugging information
    Logger::Get().Debug(msg, args);
    // Finalize the variable argument list
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
SQInteger Core::RuntimeErrorHandler(HSQUIRRELVM vm)
{
    // Was there a value thrown?
    if (sq_gettop(vm) < 1)
    {
        return SQ_OK; // No error to display!
    }
    // Temporary variable for the thrown error
    CSStr err_msg = nullptr;
    // Attempt to obtain the thrown value as a string
    if (SQ_SUCCEEDED(sq_getstring(vm, 2, &err_msg)))
    {
        Logger::Get().Debug(_SC("%s"), err_msg);
    }
    else
    {
        Logger::Get().Debug(_SC("Unknown runtime error has occurred"));
    }
    // We handled the error
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
void Core::CompilerErrorHandler(HSQUIRRELVM /*vm*/, CSStr desc, CSStr src, SQInteger line, SQInteger column)
{
    LogFtl("Message: %s\n[\n=>Location: %s\n=>Line: %d\n=>Column: %d\n]", desc, src, line, column);
}

// ------------------------------------------------------------------------------------------------
Core::BlipInst::~BlipInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        Core::Get().EmitBlipDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Are we supposed to clean up this entity?
    if (mID >= 0 && (mFlags & ENF_OWNED))
    {
        _Func->DestroyCoordBlip(mID);
    }
    // Don't release the callbacks abruptly in destructor
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
Core::CheckpointInst::~CheckpointInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        Core::Get().EmitCheckpointDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Are we supposed to clean up this entity?
    if (mID >= 0 && (mFlags & ENF_OWNED))
    {
        _Func->DeleteCheckPoint(mID);
    }
    // Don't release the callbacks abruptly in destructor
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
Core::KeybindInst::~KeybindInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        Core::Get().EmitKeybindDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Are we supposed to clean up this entity?
    if (mID >= 0 && (mFlags & ENF_OWNED))
    {
        _Func->RemoveKeyBind(mID);
    }
    // Don't release the callbacks abruptly in destructor
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
Core::ObjectInst::~ObjectInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        Core::Get().EmitObjectDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Are we supposed to clean up this entity?
    if (mID >= 0 && (mFlags & ENF_OWNED))
    {
        _Func->DeleteObject(mID);
    }
    // Don't release the callbacks abruptly in destructor
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
Core::PickupInst::~PickupInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        Core::Get().EmitPickupDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Are we supposed to clean up this entity?
    if (mID >= 0 && (mFlags & ENF_OWNED))
    {
        _Func->DeletePickup(mID);
    }
    // Don't release the callbacks abruptly in destructor
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
Core::PlayerInst::~PlayerInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        Core::Get().EmitPlayerDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
        // Release the used memory buffer
        mInst->m_Buffer.ResetAll();
    }
    // Don't release the callbacks abruptly in destructor
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
Core::VehicleInst::~VehicleInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        Core::Get().EmitVehicleDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Are we supposed to clean up this entity?
    if (mID >= 0 && (mFlags & ENF_OWNED))
    {
        _Func->DeleteVehicle(mID);
    }
    // Don't release the callbacks abruptly in destructor
    Core::ResetFunc(*this);
}

} // Namespace:: SqMod
