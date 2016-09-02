// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Logger.hpp"
#include "SqMod.h"

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

// --------------------------------------------------------------------------------------------
#define SQMOD_CATCH_EVENT_EXCEPTION(action) /*
*/ catch (const Sqrat::Exception & e) /*
*/ { /*
*/  LogErr("Squirrel exception caught " action); /*
*/  Logger::Get().Debug("%s", e.what()); /*
*/ } /*
*/

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
class ContainerCleaner
{
    // --------------------------------------------------------------------------------------------
    EntityType m_Type; // The type of entity container to clear.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    template < typename T > ContainerCleaner(T & container, EntityType type, bool destroy)
        : m_Type(type)
    {
        for (auto & ent : container)
        {
            ent.Destroy(destroy, SQMOD_DESTROY_CLEANUP, NullObject());
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
    , m_PendingScripts()
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
    , m_Shutdown(false)
    , m_LockPreLoadSignal(false)
    , m_LockPostLoadSignal(false)
    , m_LockUnloadSignal(false)
    , m_EmptyInit(false)
    , m_Verbosity(1)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Core::~Core()
{
    if (m_VM)
    {
        Terminate(true);
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
            {
                OutputError("Failed to load the configuration file. Probably invalid");
            } break;
            case SI_NOMEM:
            {
                OutputError("Run out of memory while loading the configuration file");
            } break;
            case SI_FILE:
            {
                OutputError("Failed to load the configuration file. %s", std::strerror(errno));
            } break;
            default: OutputError("Failed to load the configuration file for some unforeseen reason");
        }
        // Failed to load the configuration file
        return false;
    }

    // Configure the empty initialization
    m_EmptyInit = conf.GetBoolValue("Squirrel", "EmptyInit", false);
    // Configure the verbosity level
    m_Verbosity = conf.GetLongValue("Log", "VerbosityLevel", 1);
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

    cLogDbg(m_Verbosity >= 1, "Resizing the entity containers");
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

    cLogDbg(m_Verbosity >= 1, "Creating a virtual machine (%ld stack size)", stack_size);
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

    cLogDbg(m_Verbosity >= 1, "Registering the standard libraries");
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

    cLogDbg(m_Verbosity >= 1, "Setting the script output function");
    // Tell the VM to use these functions to output user on error messages
    sq_setprintfunc(m_VM, PrintFunc, ErrorFunc);

    cLogDbg(m_Verbosity >= 1, "Setting the script error handlers");
    // Tell the VM to trigger this function on compile time errors
    sq_setcompilererrorhandler(m_VM, CompilerErrorHandler);
    // Push the runtime error handler on the stack and create a closure
    sq_newclosure(m_VM, RuntimeErrorHandler, 0);
    // Tell the VM to trigger this function on runtime errors
    sq_seterrorhandler(m_VM);

    cLogDbg(m_Verbosity >= 1, "Registering the plug-in API");
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
    if (m_PendingScripts.empty() && !m_EmptyInit)
    {
        LogErr("No scripts loaded. No reason to load the plug-in");
        // No point in loading the plug-in
        return false;
    }

    cLogDbg(m_Verbosity >= 1, "Reading the options from the general section");
    // Read options only after loading was successful
    CSimpleIniA::TNamesDepend options;
    // Are there any options to read?
    if (conf.GetAllKeys("Options", options) || options.size() > 0)
    {
        cLogDbg(m_Verbosity >= 1, "Found (%u) options in the configuration file", options.size());
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
    if (m_PendingScripts.empty())
    {
        // Are we allowed to continue without any scripts?
        if (m_EmptyInit)
        {
            LogWrn("No scripts to execute. Empty initialization was forced");
            // Allow empty initialization since it was requested
            return true;
        }
        LogWrn("No scripts to execute. Plug-in has no purpose");
        // No reason to execute the plug-in
        return false;
    }

    // Unlock signal containers
    m_LockPreLoadSignal = false;
    m_LockPostLoadSignal = false;
    m_LockUnloadSignal = false;

    cLogDbg(m_Verbosity >= 1, "Signaling outside plug-ins to register their API");
    // Tell modules to do their monkey business
    _Func->SendPluginCommand(SQMOD_LOAD_CMD, "");

    // Load pending scripts while we're in the bounds of the allowed recursiveness
    for (unsigned levels = 0; (m_PendingScripts.empty() == false) && (levels < 8); ++levels)
    {
        // Remember the last script from the pool
        const Scripts::size_type last = m_Scripts.size();
        // Push pending scripts to the back of the list
        std::move(m_PendingScripts.begin(), m_PendingScripts.end(), std::back_inserter(m_Scripts));
        // Clear all pending scripts, if any
        m_PendingScripts.clear();
        // Process all pending scripts
        if (DoScripts(m_Scripts.begin() + last, m_Scripts.end()) == false)
        {
            return false; // One of the scripts failed to execute
        }
        cLogDbg(m_Verbosity >= 2, "Completed execution of stage (%u) scripts", levels);
    }

    // Create the null entity instances
    m_NullBlip = Object(new CBlip(-1));
    m_NullCheckpoint = Object(new CCheckpoint(-1));
    m_NullKeybind = Object(new CKeybind(-1));
    m_NullObject = Object(new CObject(-1));
    m_NullPickup = Object(new CPickup(-1));
    m_NullPlayer = Object(new CPlayer(-1));
    m_NullVehicle = Object(new CVehicle(-1));

    m_LockPreLoadSignal = true;
    // Trigger functions that must initialize stuff before the loaded event is triggered
    for (FuncData & fn : m_PreLoadSignal)
    {
        Emit(fn.first,  fn.second);
    }
    // Clear the functions
    m_PreLoadSignal.clear();

    // Notify the script callback that the scripts were loaded
    EmitScriptLoaded();

    m_LockPostLoadSignal = true;
    // Trigger functions that must initialize stuff after the loaded event is triggered
    for (FuncData & fn : m_PostLoadSignal)
    {
        Emit(fn.first, fn.second);
    }
    // Clear the functions
    m_PostLoadSignal.clear();

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
void Core::Terminate(bool shutdown)
{
    m_Shutdown = shutdown;
    // Is there a virtual machine present?
    if (m_VM)
    {
        m_LockUnloadSignal = true;
        // Trigger functions that must de-initialize stuff before the scripts are unloaded
        for (FuncData & fn : m_UnloadSignal)
        {
            Emit(fn.first, fn.second, shutdown);
        }
        // Clear the functions
        m_UnloadSignal.clear();

        cLogDbg(m_Verbosity >= 1, "Signaling outside plug-ins to release their resources");
        // Tell modules to do their monkey business
        _Func->SendPluginCommand(SQMOD_TERMINATE_CMD, "");
    }
    cLogDbg(m_Verbosity >= 1, "Clearing the entity containers");
    // Release all entity resources by clearing the containers
    const ContainerCleaner cc_players(m_Players, ENT_PLAYER, !shutdown);
    const ContainerCleaner cc_vehicles(m_Vehicles, ENT_VEHICLE, !shutdown);
    const ContainerCleaner cc_objects(m_Objects, ENT_OBJECT, !shutdown);
    const ContainerCleaner cc_pickups(m_Pickups, ENT_PICKUP, !shutdown);
    const ContainerCleaner cc_checkpoints(m_Checkpoints, ENT_CHECKPOINT, !shutdown);
    const ContainerCleaner cc_blips(m_Blips, ENT_BLIP, !shutdown);
    const ContainerCleaner cc_keybinds(m_Keybinds, ENT_KEYBIND, !shutdown);
    cLogDbg(m_Verbosity >= 1, "Terminating routines an commands");
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
    // Release null entity instances
    m_NullBlip.Release();
    m_NullCheckpoint.Release();
    m_NullKeybind.Release();
    m_NullObject.Release();
    m_NullPickup.Release();
    m_NullPlayer.Release();
    m_NullVehicle.Release();
    // Clear any functions added during shutdown
    m_PreLoadSignal.clear();
    m_PostLoadSignal.clear();
    m_UnloadSignal.clear();
    // Is there a VM to close?
    if (m_VM)
    {
        cLogDbg(m_Verbosity >= 1, "Closing the virtual machine");
        // Release all script callbacks
        ResetFunc();
        // Release the script instances
        m_Scripts.clear();
        m_PendingScripts.clear(); // Just in case
        // Specify that no scripts are left executed
        m_Executed = false;
        // Assertions during close may cause double delete/close!
        HSQUIRRELVM sq_vm = m_VM;
        m_VM = nullptr;

        cLogDbg(m_Verbosity >= 1, "Signaling outside plug-ins the virtual machine is closing");
        // Tell modules to do their monkey business
        _Func->SendPluginCommand(SQMOD_CLOSING_CMD, "");
        // Attempt to close the VM
        sq_close(sq_vm);

        cLogDbg(m_Verbosity >= 1, "Signaling outside plug-ins to release the virtual machine");
        // Tell modules to do their monkey business
        _Func->SendPluginCommand(SQMOD_RELEASED_CMD, "");
    }

    OutputMessage("Squirrel plug-in was successfully terminated");
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
    Terminate(false);
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
    // Also check the pending scripts container
    else if (std::find_if(m_PendingScripts.cbegin(), m_PendingScripts.cend(), [&path](Scripts::const_reference s) {
        return (s.mPath == path);
    }) != m_PendingScripts.end())
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
            // Remove the script container since it's invalid
            m_Scripts.pop_back();
            // Failed to compile properly
            return false;
        }
        // At this point the script should be completely loaded
        cLogDbg(m_Verbosity >= 3, "Successfully compiled script: %s", m_Scripts.back().mPath.c_str());

        // Attempt to execute the compiled script code
        try
        {
            m_Scripts.back().mExec.Run();
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to execute: %s", m_Scripts.back().mPath.c_str());
            // Remove the script container since it's invalid
            m_Scripts.pop_back();
            // Failed to execute properly
            return false;
        }
        // At this point the script should be completely loaded
        cLogScs(m_Verbosity >= 2, "Successfully executed script: %s", m_Scripts.back().mPath.c_str());
    }
    // We don't compile the scripts yet. We just store their path and prepare the objects
    else
    {
        // Create a new script container and insert it into the pending script pool
        m_PendingScripts.emplace_back(m_VM, std::move(path), delay, m_Debugging);
    }

    // At this point the script exists in one of the pools
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
bool Core::DoScripts(Scripts::iterator itr, Scripts::iterator end)
{
    Scripts::iterator itr_state = itr;

    cLogDbg(Get().m_Verbosity >= 1, "Attempting to compile the specified scripts");
    // Compile scripts first so that the constants can take effect
    for (; itr != end; ++itr)
    {
        // Is this script already compiled?
        if (!(*itr).mExec.IsNull())
        {
            continue; // Already compiled!
        }

        // Attempt to load and compile the script file
        try
        {
            (*itr).mExec.CompileFile((*itr).mPath);
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to compile: %s", (*itr).mPath.c_str());
            // Failed to execute properly
            return false;
        }

        cLogDbg(Get().m_Verbosity >= 3, "Successfully compiled script: %s", (*itr).mPath.c_str());

        // Should we delay the execution of this script?
        if ((*itr).mDelay)
        {
            continue; // Execute later!
        }

        // Attempt to execute the compiled script code
        try
        {
            (*itr).mExec.Run();
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to execute: %s", (*itr).mPath.c_str());
            // Failed to execute properly
            return false;
        }

        cLogScs(Get().m_Verbosity >= 2, "Successfully executed script: %s", (*itr).mPath.c_str());
    }

    cLogDbg(Get().m_Verbosity >= 1, "Attempting to execute the specified scripts");
    // Execute scripts only after compilation successful
    for (itr = itr_state; itr != end; ++itr)
    {
        // Was this script delayed from execution?
        if (!(*itr).mDelay)
        {
            continue; // Already executed!
        }

        // Attempt to execute the compiled script code
        try
        {
            (*itr).mExec.Run();
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to execute: %s", (*itr).mPath.c_str());
            // Failed to execute properly
            return false;
        }

        cLogScs(Get().m_Verbosity >= 2, "Successfully executed script: %s", (*itr).mPath.c_str());
    }

    // At this point the scripts were loaded and executed successfully
    return true;
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
void Core::BindPreLoad(Object & env, Function & func, Object & payload)
{
    if (m_LockPreLoadSignal)
    {
        STHROWF("Cannot bind functions to pre-load signal anymore");
    }
    else if (func.IsNull())
    {
        STHROWF("Cannot bind null as callback to pre-load signal");
    }
    // Does this function need a custom environment?
    else if (env.IsNull())
    {
        m_PreLoadSignal.emplace_back(func, payload);
    }
    // Assign the specified environment and function
    else
    {
        m_PreLoadSignal.emplace_back(Function(env.GetVM(), env, func.GetFunc()), payload);
    }
}

// ------------------------------------------------------------------------------------------------
void Core::BindPostLoad(Object & env, Function & func, Object & payload)
{
    if (m_LockPostLoadSignal)
    {
        STHROWF("Cannot bind functions to post-load signal anymore");
    }
    else if (func.IsNull())
    {
        STHROWF("Cannot bind null as callback to post-load signal");
    }
    // Does this function need a custom environment?
    else if (env.IsNull())
    {
        m_PostLoadSignal.emplace_back(func, payload);
    }
    // Assign the specified environment and function
    else
    {
        m_PostLoadSignal.emplace_back(Function(env.GetVM(), env, func.GetFunc()), payload);
    }
}

// ------------------------------------------------------------------------------------------------
void Core::BindUnload(Object & env, Function & func, Object & payload)
{
    if (m_LockUnloadSignal)
    {
        STHROWF("Cannot bind functions to unload signal anymore");
    }
    else if (func.IsNull())
    {
        STHROWF("Cannot bind null as callback to unload signal");
    }
    // Does this function need a custom environment?
    else if (env.IsNull())
    {
        m_UnloadSignal.emplace_back(func, payload);
    }
    // Assign the specified environment and function
    else
    {
        m_UnloadSignal.emplace_back(Function(env.GetVM(), env, func.GetFunc()), payload);
    }
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
void Core::BlipInst::Destroy(bool destroy, Int32 header, Object & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitBlipDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying blip")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DestroyCoordBlip(mID);
    }
    // Reset the instance to it's initial state
    Core::ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::CheckpointInst::Destroy(bool destroy, Int32 header, Object & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitCheckpointDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying checkpoint")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeleteCheckPoint(mID);
    }
    // Reset the instance to it's initial state
    Core::ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::KeybindInst::Destroy(bool destroy, Int32 header, Object & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitKeybindDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying keybind")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->RemoveKeyBind(mID);
    }
    // Reset the instance to it's initial state
    Core::ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::ObjectInst::Destroy(bool destroy, Int32 header, Object & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitObjectDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying object")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeleteObject(mID);
    }
    // Reset the instance to it's initial state
    Core::ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::PickupInst::Destroy(bool destroy, Int32 header, Object & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitPickupDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying pickup")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeletePickup(mID);
    }
    // Reset the instance to it's initial state
    Core::ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::PlayerInst::Destroy(bool /*destroy*/, Int32 header, Object & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitPlayerDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying player")
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
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Reset the instance to it's initial state
    Core::ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

// ------------------------------------------------------------------------------------------------
void Core::VehicleInst::Destroy(bool destroy, Int32 header, Object & payload)
{
    // Should we notify that this entity is being cleaned up?
    if (VALID_ENTITY(mID))
    {
        // Don't leave exceptions to prevent us from releasing this instance
        try
        {
            Core::Get().EmitVehicleDestroyed(mID, header, payload);
        }
        SQMOD_CATCH_EVENT_EXCEPTION("while destroying vehicle")
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Prevent further use of the manager instance
    mInst = nullptr;
    // Release the script object, if any
    mObj.Release();
    // Are we supposed to clean up this entity? (only at reload)
    if (destroy && VALID_ENTITY(mID) && (mFlags & ENF_OWNED))
    {
        // Block the entity pool changes notification from triggering the destroy event
        const BitGuardU16 bg(mFlags, static_cast< Uint16 >(ENF_LOCKED));
        // Now attempt to destroy this entity from the server
        _Func->DeleteVehicle(mID);
    }
    // Reset the instance to it's initial state
    Core::ResetInst(*this);
    // Don't release the callbacks abruptly
    Core::ResetFunc(*this);
}

} // Namespace:: SqMod
