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
extern void TerminateCommands();

// ------------------------------------------------------------------------------------------------
extern Buffer GetRealFilePath(CSStr path);

/* ------------------------------------------------------------------------------------------------
 * Loader used to process a section from the configuration file and look for scripts to load.
*/
class ScriptLoader
{
    // --------------------------------------------------------------------------------------------
    CSimpleIniA & m_Config; // The processed configuration.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ScriptLoader(CSimpleIniA & conf)
        : m_Config(conf)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    bool operator () (CCStr key, CCStr val) const
    {
        // Validate the specified key
        if (!key || *key == '\0')
        {
            return true; // Move to the next element!
        }
        // Identify the load option
        if (std::strcmp(key, "Section") == 0)
        {
            return m_Config.ProcAllValues(val, ScriptLoader(m_Config));
        }
        else if (std::strcmp(key, "Compile") == 0)
        {
            return Core::Get().LoadScript(val, true);
        }
        else if (std::strcmp(key, "Execute") == 0)
        {
            return Core::Get().LoadScript(val, false);
        }
        // Move to the next element!
        return true;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Implements RAII to make sure that entity containers area cleaned up at all costs.
*/
template < typename T > class ContainerCleaner
{
    // --------------------------------------------------------------------------------------------
    const EntityType m_Type; // The type of entity container to clear.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ContainerCleaner(T & container, EntityType type)
        : m_Type(type)
    {
        for (auto & ent : container)
        {
            ent.Destroy();
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ContainerCleaner()
    {
        Core::Get().ClearContainer(m_Type);
    }
};

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
    , m_ReloadHeader(0)
    , m_ReloadPayload()
    , m_IncomingNameBuffer(nullptr)
    , m_IncomingNameCapacity(0)
    , m_Debugging(false)
    , m_Executed(false)
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
    // See if debugging options should be enabled
    m_Debugging = conf.GetBoolValue("Squirrel", "Debugging", false);

    // Prevent common null objects from using dead virtual machines
    NullArray() = Array();
    NullTable() = Table();
    NullObject() = Object();
    NullFunction() = Function();

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

    CSimpleIniA::TNamesDepend scripts;
    // Attempt to retrieve the list of keys to make sure there's actually something to process
    if (conf.GetAllKeys("Scripts", scripts) && scripts.size() > 0)
    {
        // Attempt to load the specified scripts
        if (!conf.ProcAllValues("Scripts", ScriptLoader(conf)))
        {
            LogErr("Unable to load the specified scripts");
            // Either no script was found or failed to load
            return false;
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
        LogDbg("Found (%u) options in the configuration file", options.size());
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

    LogDbg("Attempting to compile the specified scripts");
    // Compile scripts first so that the constants can take effect
    for (auto & s : m_Scripts)
    {
        // Is this script already compiled?
        if (!s.mExec.IsNull())
        {
            continue; // Already compiled!
        }

        // Attempt to load and compile the script file
        try
        {
            s.mExec.CompileFile(s.mPath);
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to compile: %s", s.mPath.c_str());
            // Failed to execute properly
            return false;
        }

        LogDbg("Successfully compiled script: %s", s.mPath.c_str());

        // Should we delay the execution of this script?
        if (s.mDelay)
        {
            continue; // Execute later!
        }

        // Attempt to execute the compiled script code
        try
        {
            s.mExec.Run();
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to execute: %s", s.mPath.c_str());
            // Failed to execute properly
            return false;
        }

        LogScs("Successfully executed script: %s", s.mPath.c_str());
    }

    LogDbg("Attempting to execute the specified scripts");
    // Execute scripts only after compilation successful
    for (auto & s : m_Scripts)
    {
        // Was this script delayed from execution?
        if (!s.mDelay)
        {
            continue; // Already executed!
        }

        // Attempt to execute the compiled script code
        try
        {
            s.mExec.Run();
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to execute: %s", s.mPath.c_str());
            // Failed to execute properly
            return false;
        }

        LogScs("Successfully executed script: %s", s.mPath.c_str());
    }

    // Notify the script callback that the scripts were loaded
    EmitScriptLoaded();

    // Import already existing entities
    ImportPlayers();
    ImportBlips();
    ImportCheckpoints();
    ImportKeybinds();
    ImportObjects();
    ImportPickups();
    ImportVehicles();

    // Successfully executed
    return (m_Executed = true);
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
    LogDbg("Clearing the entity containers");
    // Release all entity resources by clearing the containers
    ContainerCleaner< Players > cc_players(m_Players, ENT_PLAYER);
    ContainerCleaner< Vehicles > cc_vehicles(m_Vehicles, ENT_VEHICLE);
    ContainerCleaner< Objects > cc_objects(m_Objects, ENT_OBJECT);
    ContainerCleaner< Pickups > cc_pickups(m_Pickups, ENT_PICKUP);
    ContainerCleaner< Checkpoints > cc_checkpoints(m_Checkpoints, ENT_CHECKPOINT);
    ContainerCleaner< Blips > cc_blips(m_Blips, ENT_BLIP);
    ContainerCleaner< Keybinds > cc_keybinds(m_Keybinds, ENT_KEYBIND);
    LogDbg("Terminating routines an commands");
    // Release all resources from routines
    TerminateRoutines();
    // Release all resources from command managers
    TerminateCommands();
    // In case there's a payload for reload
    m_ReloadPayload.Release();
    // Release null objects in case any reference to valid objects is stored in them
    NullArray().Release();
    NullTable().Release();
    NullObject().Release();
    NullFunction().ReleaseGently();
    // Is there a VM to close?
    if (m_VM)
    {
        LogDbg("Closing the virtual machine");
        // Release all script callbacks
        ResetFunc();
        // Release the script instances
        m_Scripts.clear();
        // Specify that no scripts are left executed
        m_Executed = false;
        // Assertions during close may cause double delete/close!
        HSQUIRRELVM sq_vm = m_VM;
        m_VM = nullptr;
        // Attempt to close the VM
        sq_close(sq_vm);

        LogDbg("Signaling outside plug-ins to release the virtual machine");
        // Tell modules to do their monkey business
        _Func->SendPluginCommand(0xDEADBEAF, "");
    }

    LogDbg("Squirrel plugin was successfully terminated");
}

// ------------------------------------------------------------------------------------------------
bool Core::Reload()
{
    // Are we already reloading?
    if (m_CircularLocks & CCL_RELOAD_SCRIPTS)
    {
        return false; // Already reloading!
    }
    // Prevent circular reloads when we send plug-in commands
    const BitGuardU32 bg(m_CircularLocks, static_cast< Uint32 >(CCL_RELOAD_SCRIPTS));
    // Allow reloading by default
    Core::Get().SetState(1);
    // Emit the reload event
    Core::Get().EmitScriptReload(m_ReloadHeader, m_ReloadPayload);
    // Are we allowed to reload?
    if (!Core::Get().GetState())
    {
        return false; // Request denied!
    }
    // Terminate the current VM and release resources
    Terminate();
    // Reset the reload header after termination
    m_ReloadHeader = -1;
    // Attempt to initialize the central core and load resources
    return (Initialize() && Execute());
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
bool Core::LoadScript(CSStr filepath, bool delay)
{
    // Is the specified path empty?
    if (!filepath || *filepath == '\0')
    {
        LogErr("Cannot load script with empty or invalid path");
        // Failed to load
        return false;
    }
    Buffer bpath;
    // Attempt to get the real file path
    try
    {
        bpath = GetRealFilePath(filepath);
    }
    catch (const Sqrat::Exception & e)
    {
        LogErr("Unable to load script: %s", e.what());
        // Failed to load
        return false;
    }
    catch (const std::exception & e)
    {
        LogErr("Unable to load script: %s", e.what());
        // Failed to load
        return false;
    }
    // Make the path into a string
    String path(bpath.Data(), bpath.Position());
    // See if it wasn't already loaded
    if (std::find_if(m_Scripts.cbegin(), m_Scripts.cend(), [&path](Scripts::const_reference s) {
        return (s.mPath == path);
    }) != m_Scripts.end())
    {
        LogWrn("Script was specified before: %s", path.c_str());
    }
    // Were the scripts already executed? Then there's no need to queue them
    else if (m_Executed)
    {
        // Create a new script container and insert it into the script pool
        m_Scripts.emplace_back(m_VM, std::move(path), delay, m_Debugging);

        // Attempt to load and compile the script file
        try
        {
            m_Scripts.back().mExec.CompileFile(m_Scripts.back().mPath);
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to compile: %s", m_Scripts.back().mPath.c_str());
            // Failed to execute properly
            return false;
        }
        // At this point the script should be completely loaded
        LogDbg("Successfully compiled script: %s", m_Scripts.back().mPath.c_str());

        // Attempt to execute the compiled script code
        try
        {
            m_Scripts.back().mExec.Run();
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to execute: %s", m_Scripts.back().mPath.c_str());
            // Failed to execute properly
            return false;
        }
        // At this point the script should be completely loaded
        LogScs("Successfully executed script: %s", m_Scripts.back().mPath.c_str());
    }
    // We don't compile the scripts yet. We just store their path and prepare the objects
    else
    {
        // Create a new script container and insert it into the script pool
        m_Scripts.emplace_back(m_VM, std::move(path), delay, m_Debugging);
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
void Core::BindEvent(Int32 id, Object & env, Function & func)
{
    // Obtain the function instance called for this event
    Function & event = GetEvent(id);
    // Is the specified callback function null?
    if (func.IsNull())
    {
        event.ReleaseGently(); // Then release the current callback
    }
    // Does this function need a custom environment?
    else if (env.IsNull())
    {
        event = func;
    }
    // Assign the specified environment and function
    else
    {
        event = Function(env.GetVM(), env, func.GetFunc());
    }
}

// ------------------------------------------------------------------------------------------------
void Core::BlipInst::Destroy()
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
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
    if (VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DestroyCoordBlip(mID);
    }
    // Reset the instance to it's initial state
    ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::CheckpointInst::Destroy()
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
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
    if (VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeleteCheckPoint(mID);
    }
    // Reset the instance to it's initial state
    ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::KeybindInst::Destroy()
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
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
    if (VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->RemoveKeyBind(mID);
    }
    // Reset the instance to it's initial state
    ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::ObjectInst::Destroy()
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
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
    if (VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeleteObject(mID);
    }
    // Reset the instance to it's initial state
    ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::PickupInst::Destroy()
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
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
    if (VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeletePickup(mID);
    }
    // Reset the instance to it's initial state
    ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::PlayerInst::Destroy()
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
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
    // Reset the instance to it's initial state
    ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::VehicleInst::Destroy()
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
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
    if (VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeleteVehicle(mID);
    }
    // Reset the instance to it's initial state
    ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

} // Namespace:: SqMod
