// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Logger.hpp"
#include "Core/Areas.hpp"
#include "Core/Signal.hpp"
#include "Core/Buffer.hpp"
#include "Core/ThreadPool.hpp"
#include "Library/IO/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Entity/Checkpoint.hpp"
#include "Entity/KeyBind.hpp"
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
extern void ZmqTerminate();
extern void AnnounceTerminate();
extern void InitializeTasks();
extern void InitializeRoutines();
extern void TerminateAreas();
extern void TerminateTasks();
extern void TerminatePrivileges();
extern void TerminateRoutines();
extern void TerminateCommands();
extern void TerminateSignals();
extern void TerminateNet();
extern void TerminatePocoNet();
extern void TerminatePocoData();

// ------------------------------------------------------------------------------------------------
extern Buffer GetRealFilePath(const SQChar * path);

// ------------------------------------------------------------------------------------------------
#ifdef VCMP_ENABLE_OFFICIAL
    extern void LgCheckpointSetID(LgCheckpoint * inst, int32_t id);
    extern void LgObjectSetID(LgObject * inst, int32_t id);
    extern void LgPickupSetID(LgPickup * inst, int32_t id);
    extern void LgPlayerSetID(LgPlayer * inst, int32_t id);
    extern void LgVehicleSetID(LgVehicle * inst, int32_t id);
    extern LightObj LgCheckpointObj(HSQUIRRELVM vm, int32_t id);
    extern LightObj LgObjectObj(HSQUIRRELVM vm, int32_t id);
    extern LightObj LgPickupObj(HSQUIRRELVM vm, int32_t id);
    extern LightObj LgPlayerObj(HSQUIRRELVM vm, int32_t id);
    extern LightObj LgVehicleObj(HSQUIRRELVM vm, int32_t id);
    extern void LgStreamLoadInput(const void * data, size_t size);
#endif

/* ------------------------------------------------------------------------------------------------
 * Loader used to process a section from the configuration file and look for scripts to load.
*/
struct ScriptLoader
{
    // --------------------------------------------------------------------------------------------
    CSimpleIniA &   m_Config; // The processed configuration.
    Function        m_Callback{}; // Null callback.
    LightObj        m_Context{}; // Callback context.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    explicit ScriptLoader(CSimpleIniA & conf)
        : m_Config(conf), m_Callback(), m_Context()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Function call operator.
    */
    bool operator () (const char * key, const char * val)
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
            return Core::Get().LoadScript(val, m_Callback, m_Context, true);
        }
        else if (std::strcmp(key, "Execute") == 0)
        {
            return Core::Get().LoadScript(val, m_Callback, m_Context, false);
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
            ent.Destroy(destroy, SQMOD_DESTROY_CLEANUP, NullLightObj());
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
Core::Core() noexcept
    : m_State(0)
#ifdef VCMP_ENABLE_OFFICIAL
    , m_Official(false)
#endif
    , m_VM(nullptr)
    , m_Scripts()
    , m_PendingScripts()
    , m_Options()
    , m_Blips()
    , m_Checkpoints()
    , m_KeyBinds()
    , m_Objects()
    , m_Pickups()
    , m_Players()
    , m_Vehicles()
    , m_Events()
    , m_CircularLocks(0)
    , m_ReloadHeader(0)
    , m_ReloadPayload()
    , m_IncomingNameBuffer(nullptr)
    , m_IncomingNameCapacity(0)
    , m_AreasEnabled(false)
    , m_Debugging(false)
    , m_Executed(false)
    , m_Shutdown(false)
    , m_LockPreLoadSignal(false)
    , m_LockPostLoadSignal(false)
    , m_LockUnloadSignal(false)
    , m_EmptyInit(false)
    , m_Verbosity(1)
    , m_ClientData()
    , m_NullBlip()
    , m_NullCheckpoint()
    , m_NullKeyBind()
    , m_NullObject()
    , m_NullPickup()
    , m_NullPlayer()
    , m_NullVehicle()
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
    // Attempt to initialize the thread pool
    if (!ThreadPool::Get().Initialize(conf.GetLongValue("General", "WorkerThreads",
                                                                static_cast< long >(std::thread::hardware_concurrency()))))
    {
        ThreadPool::Get().Terminate();
        return false;
    } else cLogDbg(m_Verbosity >= 1, "Initialized %zu worker threads", ThreadPool::Get().GetThreadCount());
#ifdef VCMP_ENABLE_OFFICIAL
    // See if debugging options should be enabled
    m_Official = conf.GetBoolValue("Squirrel", "OfficialCompatibility", m_Official);
#endif
    // See if debugging options should be enabled
    m_Debugging = conf.GetBoolValue("Squirrel", "Debugging", m_Debugging);
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
    m_KeyBinds.resize(SQMOD_KEYBIND_POOL);
    m_Objects.resize(SQMOD_OBJECT_POOL);
    m_Pickups.resize(SQMOD_PICKUP_POOL);
    m_Players.resize(SQMOD_PLAYER_POOL);
    m_Vehicles.resize(SQMOD_VEHICLE_POOL);

    // Attempt to read the virtual machine stack size
    const long stack_size = conf.GetLongValue("Squirrel", "StackSize", SQMOD_STACK_SIZE);
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
    // Create VM context instance
    sq_setforeignptr(m_VM, new VMContext());

    // Prevent common null objects from using dead virtual machines
    NullArray() = Array();
    NullTable() = Table();
    NullObject() = Object();
    NullLightObj() = LightObj();
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
    if (!RegisterAPI(m_VM))
    {
        LogFtl("Unable to register the plug-in API");
        return false; // Can't execute scripts without a valid API!
    }

    // Initialize the module global events
    InitEvents();

    // Initialize load stage signals
    InitSignalPair(mOnPreLoad, NullLightObj(), nullptr);
    InitSignalPair(mOnPostLoad, NullLightObj(), nullptr);
    InitSignalPair(mOnUnload, NullLightObj(), nullptr);
    InitSignalPair(mOnScript, NullLightObj(), nullptr);

    CSimpleIniA::TNamesDepend scripts;
    // Attempt to retrieve the list of keys to make sure there's actually something to process
    if (conf.GetAllKeys("Scripts", scripts) && !scripts.empty())
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
    if (conf.GetAllKeys("Options", options) || !options.empty())
    {
        cLogDbg(m_Verbosity >= 1, "Found (%" PRINT_SZ_FMT ") options in the configuration file",
                    static_cast< SQUnsignedInteger >(options.size()));
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

    // Initialize routines and tasks
    InitializeRoutines();
    InitializeTasks();
    // Initialize third-party

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

    //cLogDbg(m_Verbosity >= 1, "Signaling outside plug-ins to register their API");
    // Tell modules to do their monkey business
    //_Func->SendPluginCommand(0xDEADBABE, "");

    // Load pending scripts while we're in the bounds of the allowed recursiveness
    for (unsigned levels = 0; !m_PendingScripts.empty() && (levels < 8); ++levels)
    {
        // Remember the last script from the pool
        const Scripts::size_type last = m_Scripts.size();
        // Push pending scripts to the back of the list
        std::move(m_PendingScripts.begin(), m_PendingScripts.end(), std::back_inserter(m_Scripts));
        // Clear all pending scripts, if any
        m_PendingScripts.clear();
        // Process all pending scripts
        if (!DoScripts(m_Scripts.begin() + last, m_Scripts.end()))
        {
            return false; // One of the scripts failed to execute
        }

        cLogDbg(m_Verbosity >= 2, "Completed execution of stage (%u) scripts. Pending scripts %" PRINT_SZ_FMT,
                    levels, static_cast< SQUnsignedInteger >(m_PendingScripts.size()));
    }
    // Force enable null entities if not already enabled by script
    EnableNullEntities();

    // At this point, the scripts were executed
    m_Executed = true;

    m_LockPreLoadSignal = true;
    // Trigger callbacks that must initialize stuff before the loaded event is triggered
    (*mOnPreLoad.first)();
    // Clear the callbacks
    ResetSignalPair(mOnPreLoad);

    // Notify the script callback that the scripts were loaded
    EmitScriptLoaded();

    m_LockPostLoadSignal = true;
    // Trigger callbacks that must initialize stuff after the loaded event is triggered
    (*mOnPostLoad.first)();
    // Clear the callbacks
    ResetSignalPair(mOnPostLoad);

    // Import already existing entities
    ImportPlayers();
    ImportBlips();
    ImportCheckpoints();
    ImportKeyBinds();
    ImportObjects();
    ImportPickups();
    ImportVehicles();

    cLogDbg(m_Executed && m_Verbosity >= 2, "Successfully executed specified scripts");
    // Successfully executed
    return m_Executed;
}

// ------------------------------------------------------------------------------------------------
void Core::Terminate(bool shutdown)
{
    m_Shutdown = shutdown;
    // Is there a virtual machine present?
    if (m_VM)
    {
        m_LockUnloadSignal = true;
        // Trigger callbacks that must de-initialize stuff before the scripts are unloaded
        (*mOnUnload.first)();
        // Clear the callbacks
        ResetSignalPair(mOnUnload);

        //cLogDbg(m_Verbosity >= 1, "Signaling outside plug-ins to release their resources");
        // Tell modules to do their monkey business
        //_Func->SendPluginCommand(0xDEADC0DE, "");
    }

    cLogDbg(m_Verbosity >= 1, "Clearing the entity containers");
    // Release all entity resources by clearing the containers
    const ContainerCleaner cc_players(m_Players, ENT_PLAYER, !shutdown);
    const ContainerCleaner cc_vehicles(m_Vehicles, ENT_VEHICLE, !shutdown);
    const ContainerCleaner cc_objects(m_Objects, ENT_OBJECT, !shutdown);
    const ContainerCleaner cc_pickups(m_Pickups, ENT_PICKUP, !shutdown);
    const ContainerCleaner cc_checkpoints(m_Checkpoints, ENT_CHECKPOINT, !shutdown);
    const ContainerCleaner cc_blips(m_Blips, ENT_BLIP, !shutdown);
    const ContainerCleaner cc_keybinds(m_KeyBinds, ENT_KEYBIND, !shutdown);

    // Terminate the thread pool
    ThreadPool::Get().Terminate();
    cLogDbg(m_Verbosity >= 1, "Thread pool terminated");
    // Release all resources from routines and tasks
    TerminateRoutines();
    cLogDbg(m_Verbosity >= 2, "Routines terminated");
    TerminateTasks();
    cLogDbg(m_Verbosity >= 2, "Tasks terminated");
    // Release all resources from command managers
    TerminateCommands();
    cLogDbg(m_Verbosity >= 2, "Commands terminated");
    // Release all resources from signals
    TerminateSignals();
    cLogDbg(m_Verbosity >= 2, "Signals terminated");
    // Release all managed areas
    TerminateAreas();
    cLogDbg(m_Verbosity >= 2, "Areas terminated");
    // Release privilege managers
    TerminatePrivileges();
    cLogDbg(m_Verbosity >= 2, "Privileges terminated");
    // Release announcers
    AnnounceTerminate();
    cLogDbg(m_Verbosity >= 1, "Announcer terminated");
    // Release network
    TerminateNet();
    cLogDbg(m_Verbosity >= 1, "Network terminated");
    // Release Poco statement results
    TerminatePocoNet();
    TerminatePocoData();
    cLogDbg(m_Verbosity >= 1, "Poco terminated");
    // Release ZMQ sockets
    ZmqTerminate();
    cLogDbg(m_Verbosity >= 1, "ZMQ terminated");
    // In case there's a payload for reload
    m_ReloadPayload.Release();
    // Release null objects in case any reference to valid objects is stored in them
    NullArray().Release();
    NullTable().Release();
    NullObject().Release();
    NullLightObj().Release();
    NullFunction().Release();
    // Release client data buffer, if any
    m_ClientData.Release();
    // Release null entity instances
    m_NullBlip.Release();
    m_NullCheckpoint.Release();
    m_NullKeyBind.Release();
    m_NullObject.Release();
    m_NullPickup.Release();
    m_NullPlayer.Release();
    m_NullVehicle.Release();
    cLogDbg(m_Verbosity >= 2, "Temporary script objects released");
    // Is there a VM to close?
    if (m_VM)
    {
        cLogDbg(m_Verbosity >= 1, "Releasing any final resources and all loaded scripts");
        // Release all script callbacks
        ResetSignalPair(mOnScript);
        DropEvents();
        // Release the script instances
        m_Scripts.clear();
        m_PendingScripts.clear(); // Just in case
        // Specify that no scripts are left executed
        m_Executed = false;
        // Retrieve the VM context before closing
        VMContext * ctx = GetVMContext(m_VM);
        // Assertions during close may cause double delete/close!
        HSQUIRRELVM sq_vm = m_VM;
        m_VM = nullptr;

        //cLogDbg(m_Verbosity >= 1, "Signaling outside plug-ins the virtual machine is closing");
        // Tell modules to do their monkey business
        //_Func->SendPluginCommand(0xBAAAAAAD, "");
        // Release any callbacks from the logger
        Logger::Get().Release();
        cLogDbg(m_Verbosity >= 2, "Closing Virtual Machine");
        // Release class object references before closing the VM
        for (auto & c : ctx->classes)
        {
            c.second->Release();
        }
        // Attempt to close the VM
        sq_close(sq_vm);
        // Reset class objects before destructor is invoked
        for (auto & c : ctx->classes)
        {
            c.second->Reset();
        }
        // Go through each class and check for leftover objects
        for (auto & c : ctx->classes)
        {
            // Retrieve the number of elements left in the container
            size_t n = c.second->InstanceCount();
            // Log however many are left, if any
            if (n != 0)
            {
                cLogDbg(m_Verbosity >= 1, "%zu leaked (%s) objects", n, c.first.c_str());
            }
        }
        cLogDbg(m_Verbosity >= 2, "Destroying VM Context");
        // Destroy the VM context, if any
        delete ctx;

        //cLogDbg(m_Verbosity >= 1, "Signaling outside plug-ins to release the virtual machine");
        // Tell modules to do their monkey business
        //_Func->SendPluginCommand(0xDEADBEAF, "");
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
    const BitGuardU32 bg(m_CircularLocks, static_cast< uint32_t >(CCL_RELOAD_SCRIPTS));
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
void Core::EnableNullEntities()
{
    // Create the null entity instances
    if (m_NullBlip.IsNull()) m_NullBlip = LightObj(DeleteGuard< CBlip >(new CBlip(-1)));
    if (m_NullCheckpoint.IsNull()) m_NullCheckpoint = LightObj(DeleteGuard< CCheckpoint >(new CCheckpoint(-1)));
    if (m_NullKeyBind.IsNull()) m_NullKeyBind = LightObj(DeleteGuard< CKeyBind >(new CKeyBind(-1)));
    if (m_NullObject.IsNull()) m_NullObject = LightObj(DeleteGuard< CObject >(new CObject(-1)));
    if (m_NullPickup.IsNull()) m_NullPickup = LightObj(DeleteGuard< CPickup >(new CPickup(-1)));
    if (m_NullPlayer.IsNull()) m_NullPlayer = LightObj(DeleteGuard< CPlayer >(new CPlayer(-1)));
    if (m_NullVehicle.IsNull()) m_NullVehicle = LightObj(DeleteGuard< CVehicle >(new CVehicle(-1)));
}

// ------------------------------------------------------------------------------------------------
const String & Core::GetOption(const String & name) const
{
    auto elem = m_Options.find(name);
    return (elem == m_Options.end()) ? NullString() : elem->second;
}

// ------------------------------------------------------------------------------------------------
const String & Core::GetOption(const String & name, const String & value) const
{
    auto elem = m_Options.find(name);
    return (elem == m_Options.end()) ? value : elem->second;
}

// ------------------------------------------------------------------------------------------------
void Core::SetOption(const String & name, const String & value)
{
    m_Options[name] = value;
}

// ------------------------------------------------------------------------------------------------
Core::Scripts::iterator Core::FindScript(const SQChar * src)
{
    // Iterate over loaded scripts
    for (auto itr = m_Scripts.begin(); itr != m_Scripts.end(); ++itr)
    {
        // Is this script the source we're looking for?
        if (itr->mPath.compare(0, String::npos, src) == 0)
        {
            return itr;
        }
    }
    // Not found!
    return m_Scripts.end();
}

// ------------------------------------------------------------------------------------------------
Core::Scripts::iterator Core::FindPendingScript(const SQChar * src)
{
    // Iterate over loaded scripts
    for (auto itr = m_PendingScripts.begin(); itr != m_PendingScripts.end(); ++itr)
    {
        // Is this script the source we're looking for?
        if (itr->mPath.compare(0, String::npos, src) == 0)
        {
            return itr;
        }
    }
    // Not found!
    return m_PendingScripts.end();
}

// ------------------------------------------------------------------------------------------------
bool Core::LoadScript(const SQChar * filepath, Function & cb, LightObj & ctx, bool delay)
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
#ifdef SQMOD_OS_WINDOWS
        // Windows does not have case sensitive filenames and we can end up trying to load the same file more than once
        return s.mPath.size() == path.size() && strnicmp(s.mPath.c_str(), path.c_str(), path.size()) == 0;
#else
        return (s.mPath == path);
#endif
    }) != m_Scripts.end())
    { // NOLINT(bugprone-branch-clone)
        LogWrn("Script was specified before: %s", path.c_str());
        return false; // We didn't actually fail to load it
    }
    // Also check the pending scripts container
    else if (std::find_if(m_PendingScripts.cbegin(), m_PendingScripts.cend(), [&path](Scripts::const_reference s) {
#ifdef SQMOD_OS_WINDOWS
        // Windows does not have case sensitive filenames and we can end up trying to load the same file more than once
        return s.mPath.size() == path.size() && strnicmp(s.mPath.c_str(), path.c_str(), path.size()) == 0;
#else
        return (s.mPath == path);
#endif
    }) != m_PendingScripts.end())
    {
        LogWrn("Script was specified before: %s", path.c_str());
        return false; // We didn't actually fail to load it
    }
    // Were the scripts already executed? Then there's no need to queue them
    else if (m_Executed)
    {
        // Create a new script container and insert it into the script pool
        m_Scripts.emplace_back(path, cb, ctx, delay, m_Debugging);

        // Attempt to load and compile the script file
        try
        {
            m_Scripts.back().mExec.CompileFile(path);
        }
        catch (const std::exception & e)
        {
            LogFtl("Unable to compile (%s) exception caught: %s", path.c_str(), e.what());
            // Remove the script container since it's invalid
            m_Scripts.pop_back();
            // Failed to compile properly
            return false;
        }
        // At this point the script should be completely loaded
        cLogDbg(m_Verbosity >= 3, "Compiled script: %s", path.c_str());

        // Attempt to execute the compiled script code
        try
        {
            auto & s = m_Scripts.back();
            // Attempt to run the script
            s.mExec.Run();
            // Does someone need to be notified?
            if (!s.mFunc.IsNull())
            {
                // Invoke the associated script
                s.mFunc.Execute(s.mPath, s.mCtx);
                // Release callback since we know it exists
                s.mFunc.Release();
            }
            // Invoke the global callback
            (*mOnScript.first)(s.mPath, s.mCtx);
            // Release context, if any
            s.mCtx.Release();
        }
        catch (const std::exception & e)
        {
            LogFtl("Unable to execute (%s) exception caught: %s", path.c_str(), e.what());
            // Release callback and context objects, if any
            m_Scripts.back().mFunc.Release();
            m_Scripts.back().mCtx.Release();
            // Remove the script container since it's invalid
            m_Scripts.pop_back();
            // Failed to execute properly
            return false;
        }
        // At this point the script should be completely loaded
        cLogScs(m_Verbosity >= 2, "Executed script: %s", path.c_str());
    }
    // We don't compile the scripts yet. We just store their path and prepare the objects
    else
    {
        cLogDbg(m_Verbosity >= 2, "Pending %s script: %s", (delay ? "deferred" : "immediate"), path.c_str());
        // Attempt to queue the script
        try
        {
            // Create a new script container and insert it into the pending script pool
            m_PendingScripts.emplace_back(path, cb, ctx, delay, m_Debugging);
        }
        catch (const std::exception & e)
        {
            LogFtl("Unable to queue: %s", e.what());
            // Failed to queue script
            return false;
        }
    }

    // At this point the script exists in one of the pools
    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::SetIncomingName(const SQChar * name)
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
        STHROWF("The specified name needs to be at least 2 characters: {}", len);
    }
    // Copy the specified name to the assigned buffer
    std::strncpy(m_IncomingNameBuffer, name, m_IncomingNameCapacity);
    // Make sure that the name inside the buffer is null terminated
    m_IncomingNameBuffer[len] = '\0';
}

// ------------------------------------------------------------------------------------------------
String Core::FetchCodeLine(const SQChar * src, SQInteger line, bool trim)
{
    // Find the script we're looking for
    auto script = FindScript(src);
    // Do we have a valid script and line?
    if ((script == m_Scripts.end()) || !(script->mInfo) || (static_cast< SQInteger >(script->mLine.size()) < line))
    {
        return String{}; // No such script!
    }
    // Fetch the line of code
    return script->FetchLine(line, trim);
}

// ------------------------------------------------------------------------------------------------
bool Core::DoScripts(Scripts::iterator itr, Scripts::iterator end)
{
    auto itr_state = itr;

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
        catch (const std::exception & e)
        {
            LogFtl("Unable to compile (%s) exception caught: %s", (*itr).mPath.c_str(), e.what());
            // Failed to execute properly
            return false;
        }

        cLogDbg(Get().m_Verbosity >= 3, "Compiled script: %s", (*itr).mPath.c_str());

        // Should we delay the execution of this script?
        if ((*itr).mDelay)
        {
            continue; // Execute later!
        }

        // Attempt to execute the compiled script code
        try
        {
            auto & s = *itr;
            // Attempt to run the script
            s.mExec.Run();
            // Does someone need to be notified?
            if (!s.mFunc.IsNull())
            {
                // Invoke the associated script
                s.mFunc.Execute(s.mPath, s.mCtx);
                // Release callback since we know it exists
                s.mFunc.Release();
            }
            // Invoke the global callback
            (*Core::Get().mOnScript.first)(s.mPath, s.mCtx);
            // Release context, if any
            s.mCtx.Release();
        }
        catch (const std::exception & e)
        {
            LogFtl("Unable to execute (%s) exception caught: %s", (*itr).mPath.c_str(), e.what());
            // Release callback and context objects, if any
            itr->mFunc.Release();
            itr->mCtx.Release();
            // Failed to execute properly
            return false;
        }

        cLogScs(Get().m_Verbosity >= 2, "Executed script: %s", (*itr).mPath.c_str());
    }

    cLogDbg(Get().m_Verbosity >= 1, "Attempting to execute the delayed scripts");
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
            auto & s = *itr;
            // Attempt to run the script
            s.mExec.Run();
            // Does someone need to be notified?
            if (!s.mFunc.IsNull())
            {
                // Invoke the associated script
                s.mFunc.Execute(s.mPath, s.mCtx);
                // Release callback since we know it exists
                s.mFunc.Release();
            }
            // Invoke the global callback
            (*Core::Get().mOnScript.first)(s.mPath, s.mCtx);
            // Release context, if any
            s.mCtx.Release();
        }
        catch (const std::exception & e)
        {
            LogFtl("Unable to execute (%s) exception caught: %s", (*itr).mPath.c_str(), e.what());
            // Release callback and context objects, if any
            itr->mFunc.Release();
            itr->mCtx.Release();
            // Failed to execute properly
            return false;
        }

        cLogScs(Get().m_Verbosity >= 2, "Executed script: %s", (*itr).mPath.c_str());
    }

    // At this point the scripts were loaded and executed successfully
    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::PrintFunc(HSQUIRRELVM /*vm*/, const SQChar * msg, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, msg);
    // Forward the message to the logger
    Logger::Get().SendFv(LOGL_USR, false, msg, args);
    // Finalize the variable argument list
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Core::ErrorFunc(HSQUIRRELVM vm, const SQChar * msg, ...)
{
    // Initialize the variable argument list
    va_list args;
    va_start(args, msg);
    // Tell the logger to display debugging information
    Logger::Get().DebugFv(vm, msg, args);
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
    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(false)))
    {
        Logger::Get().DebugF(vm, _SC("Unknown runtime error has occurred"));
    }
    else
    {
        Logger::Get().DebugF(vm, _SC("%s"), val.mPtr);
    }
    // We handled the error
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
void Core::CompilerErrorHandler(HSQUIRRELVM /*vm*/, const SQChar * desc, const SQChar * src, SQInteger line, SQInteger column)
{
    // Should we include code in output? (we count lines from 0, squirrel counts from 1)
    if ((line <= 0) || !Core::Get().IsDebugging() || !Core::Get().CompilerErrorHandlerEx(desc, src, --line, column)) {
        LogFtl("Message: %s\n[\n=>Location: %s\n=>Line: %" PRINT_INT_FMT "\n=>Column: %" PRINT_INT_FMT "\n]", desc, src, line, column);
    }
}

// ------------------------------------------------------------------------------------------------
bool Core::CompilerErrorHandlerEx(const SQChar * desc, const SQChar * src, SQInteger line, SQInteger column)
{
    // Grab the associated line of code
    String code = FetchCodeLine(src, line, true);
    // Contains an arrow pointing at the exact column
    String point;
    // Do we have a valid column?
    if (column >= 0)
    {
        // The `=>Code: ` is included as white space
        point.resize(8, ' ');
        // The line towards the arrow
        point.resize(column+7, '-');
        // Append the actual arrow
        point.push_back('^');
        // Include the new line here
        point.push_back('\n');
    }
    // Valid line of code?
    if (!code.empty())
    {
        // Display the error message with the code included
        LogFtl("Message: %s\n[\n=>Location: %s\n=>Line: %" PRINT_SZ_FMT "\n=>Column: %" PRINT_INT_FMT "\n=>Code: %s\n%s]",
            desc, src, ++line, column, code.c_str(), point.c_str());
        // We displayed the information
        return true;
    }
    // No code to show. Fall back to normal message
    return true;
}

// --------------------------------------------------------------------------------------------
void Core::ImportBlips()
{
    // Information about the blip entity
    int32_t world = -1, scale = -1, sprid = -1;
    uint32_t color = 0;
    float x = 0.0, y = 0.0, z = 0.0;

    for (int32_t i = 0; i < SQMOD_BLIP_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->CheckEntityExists(vcmpEntityPoolBlip, i) && INVALID_ENTITY(m_Blips[i].mID))
        {
            _Func->GetCoordBlipInfo(i, &world, &x, &y, &z, &scale, &color, &sprid);
            // Make the properties available before triggering the event
            m_Blips[i].mWorld = world;
            m_Blips[i].mScale = scale;
            m_Blips[i].mSprID = sprid;
            m_Blips[i].mPosition.SetVector3Ex(x, y, z);
            m_Blips[i].mColor.SetRGBA(color);
            // Attempt to allocate the instance
            AllocBlip(i, false, SQMOD_CREATE_IMPORT, NullLightObj());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ImportCheckpoints()
{
    for (int32_t i = 0; i < SQMOD_CHECKPOINT_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->CheckEntityExists(vcmpEntityPoolCheckPoint, i) && INVALID_ENTITY(m_Checkpoints[i].mID))
        {
            AllocCheckpoint(i, false, SQMOD_CREATE_IMPORT, NullLightObj());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ImportKeyBinds()
{
    /* @NOTE This function is disabled because VC:MP server seems bugged
     * and does not return vcmpErrorNoSuchEntity when the key-bind does not exist.
     * Therefore causing incorrect behavior in the plugin.
    */
    /*
    // Information about the key-bind entity
    uint8_t release = 0;
    int32_t first = -1, second = -1, third = -1;

    for (int32_t i = 0; i < SQMOD_KEYBIND_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if ((_Func->GetKeyBindData(i, &release, &first, &second, &third) != vcmpErrorNoSuchEntity)
            && (INVALID_ENTITY(m_KeyBinds[i].mID)))
        {
            // Make the properties available before triggering the event
            m_KeyBinds[i].mFirst = first;
            m_KeyBinds[i].mSecond = second;
            m_KeyBinds[i].mThird = third;
            m_KeyBinds[i].mRelease = release;
            // Attempt to allocate the instance
            AllocKeyBind(i, false, SQMOD_CREATE_IMPORT, NullLightObj());
        }
    }
    */
}

// ------------------------------------------------------------------------------------------------
void Core::ImportObjects()
{
    for (int32_t i = 0; i < SQMOD_OBJECT_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->CheckEntityExists(vcmpEntityPoolObject, i) && INVALID_ENTITY(m_Objects[i].mID))
        {
            AllocObject(i, false, SQMOD_CREATE_IMPORT, NullLightObj());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ImportPickups()
{
    for (int32_t i = 0; i < SQMOD_PICKUP_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->CheckEntityExists(vcmpEntityPoolPickup, i) && (INVALID_ENTITY(m_Pickups[i].mID)))
        {
            AllocPickup(i, false, SQMOD_CREATE_IMPORT, NullLightObj());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ImportPlayers()
{
    for (int32_t i = 0; i < SQMOD_PLAYER_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->IsPlayerConnected(i) && (INVALID_ENTITY(m_Players[i].mID)))
        {
            ConnectPlayer(i, SQMOD_CREATE_IMPORT, NullLightObj());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ImportVehicles()
{
    for (int32_t i = 0; i < SQMOD_VEHICLE_POOL; ++i)
    {
        // See if this entity exists on the server and whether was not allocated already
        if (_Func->CheckEntityExists(vcmpEntityPoolVehicle, i) && INVALID_ENTITY(m_Vehicles[i].mID))
        {
            AllocVehicle(i, false, SQMOD_CREATE_IMPORT, NullLightObj());
        }
    }
}

// --------------------------------------------------------------------------------------------
BlipInst & Core::AllocBlip(int32_t id, bool owned, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Cannot allocate blip with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    BlipInst & inst = m_Blips[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst; // Return the existing instance
    }
    // Instantiate the entity manager
    DeleteGuard< CBlip > dg(new CBlip(id));
    // Create the script object
    inst.mObj = LightObj(dg.Get(), m_VM);
    // Store the manager instance itself
    inst.mInst = dg.Get();
    // The instance is now managed by the script
    dg.Release();
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        inst.ResetInstance();
        // Make sure that if an instance was created, it points to nothing
        if (inst.mInst)
        {
            inst.mInst->m_ID = -1;
        }
        // Make sure that if an object was created, it is released
        if (!inst.mObj.IsNull())
        {
            inst.mObj.Release();
        }
        // Now we can throw the error
        STHROWF("Unable to create a blip instance for: {}", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Initialize the instance events
    inst.InitEvents();
    // Let the script callbacks know about this entity
    EmitBlipCreated(id, header, payload);
    // Return the allocated instance
    return inst;
}

// --------------------------------------------------------------------------------------------
CheckpointInst & Core::AllocCheckpoint(int32_t id, bool owned, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Cannot allocate checkpoint with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    CheckpointInst & inst = m_Checkpoints[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst; // Return the existing instance
    }
#ifdef VCMP_ENABLE_OFFICIAL
    // Do we need to support legacy API?
    if (IsOfficial())
    {
        // Create the legacy wrapper object
        inst.mLgObj = LgCheckpointObj(m_VM, id);
        // Obtain the legacy wrapper instance
        inst.mLgInst = inst.mLgObj.CastI< LgCheckpoint >();
        // Make sure that both the instance and script object could be created
        if (!inst.mLgInst || inst.mLgObj.IsNull())
        {
            inst.ResetInstance();
            // Make sure that if an instance was created, it points to nothing
            if (inst.mLgInst)
            {
                LgCheckpointSetID(inst.mLgInst, -1);
            }
            // Make sure that if an object was created, it is released
            if (!inst.mLgObj.IsNull())
            {
                inst.mLgObj.Release();
            }
            // Now we can throw the error
            STHROWF("Unable to create a checkpoint wrapper instance for: {}", id);
        }
    }
#endif
    // Instantiate the entity manager
    DeleteGuard< CCheckpoint > dg(new CCheckpoint(id));
    // Create the script object
    inst.mObj = LightObj(dg.Get(), m_VM);
    // Store the manager instance itself
    inst.mInst = dg.Get();
    // The instance is now managed by the script
    dg.Release();
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        inst.ResetInstance();
#ifdef VCMP_ENABLE_OFFICIAL
        // Discard legacy wrapper instance
        if (IsOfficial())
        {
            LgCheckpointSetID(inst.mLgInst, -1);
            inst.mLgObj.Release();
            inst.mLgInst = nullptr;
        }
#endif
        // Make sure that if an instance was created, it points to nothing
        if (inst.mInst)
        {
            inst.mInst->m_ID = -1;
        }
        // Make sure that if an object was created, it is released
        if (!inst.mObj.IsNull())
        {
            inst.mObj.Release();
        }
        // Now we can throw the error
        STHROWF("Unable to create a checkpoint instance for: {}", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Initialize the instance events
    inst.InitEvents();
    // Let the script callbacks know about this entity
    EmitCheckpointCreated(id, header, payload);
    // Return the allocated instance
    return inst;
}

// --------------------------------------------------------------------------------------------
KeyBindInst & Core::AllocKeyBind(int32_t id, bool owned, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Cannot allocate keybind with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    KeyBindInst & inst = m_KeyBinds[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst; // Return the existing instance
    }
    // Instantiate the entity manager
    DeleteGuard< CKeyBind > dg(new CKeyBind(id));
    // Create the script object
    inst.mObj = LightObj(dg.Get(), m_VM);
    // Store the manager instance itself
    inst.mInst = dg.Get();
    // The instance is now managed by the script
    dg.Release();
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        inst.ResetInstance();
        // Make sure that if an instance was created, it points to nothing
        if (inst.mInst)
        {
            inst.mInst->m_ID = -1;
        }
        // Make sure that if an object was created, it is released
        if (!inst.mObj.IsNull())
        {
            inst.mObj.Release();
        }
        // Now we can throw the error
        STHROWF("Unable to create a keybind instance for: {}", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Initialize the instance events
    inst.InitEvents();
    // Let the script callbacks know about this entity
    EmitKeyBindCreated(id, header, payload);
    // Return the allocated instance
    return inst;
}

// --------------------------------------------------------------------------------------------
ObjectInst & Core::AllocObject(int32_t id, bool owned, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Cannot allocate object with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    ObjectInst & inst = m_Objects[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst; // Return the existing instance
    }
#ifdef VCMP_ENABLE_OFFICIAL
    // Do we need to support legacy API?
    if (IsOfficial())
    {
        // Create the legacy wrapper object
        inst.mLgObj = LgObjectObj(m_VM, id);
        // Obtain the legacy wrapper instance
        inst.mLgInst = inst.mLgObj.CastI< LgObject >();
        // Make sure that both the instance and script object could be created
        if (!inst.mLgInst || inst.mLgObj.IsNull())
        {
            inst.ResetInstance();
            // Make sure that if an instance was created, it points to nothing
            if (inst.mLgInst)
            {
                LgObjectSetID(inst.mLgInst, -1);
            }
            // Make sure that if an object was created, it is released
            if (!inst.mLgObj.IsNull())
            {
                inst.mLgObj.Release();
            }
            // Now we can throw the error
            STHROWF("Unable to create a object wrapper instance for: {}", id);
        }
    }
#endif
    // Instantiate the entity manager
    DeleteGuard< CObject > dg(new CObject(id));
    // Create the script object
    inst.mObj = LightObj(dg.Get(), m_VM);
    // Store the manager instance itself
    inst.mInst = dg.Get();
    // The instance is now managed by the script
    dg.Release();
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        inst.ResetInstance();
#ifdef VCMP_ENABLE_OFFICIAL
        // Discard legacy wrapper instance
        if (IsOfficial())
        {
            LgObjectSetID(inst.mLgInst, -1);
            inst.mLgObj.Release();
            inst.mLgInst = nullptr;
        }
#endif
        // Make sure that if an instance was created, it points to nothing
        if (inst.mInst)
        {
            inst.mInst->m_ID = -1;
        }
        // Make sure that if an object was created, it is released
        if (!inst.mObj.IsNull())
        {
            inst.mObj.Release();
        }
        // Now we can throw the error
        STHROWF("Unable to create a object instance for: {}", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Initialize the instance events
    inst.InitEvents();
    // Let the script callbacks know about this entity
    EmitObjectCreated(id, header, payload);
    // Return the allocated instance
    return inst;
}

// --------------------------------------------------------------------------------------------
PickupInst & Core::AllocPickup(int32_t id, bool owned, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Cannot allocate pickup with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    PickupInst & inst = m_Pickups[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst; // Return the existing instance
    }
#ifdef VCMP_ENABLE_OFFICIAL
    // Do we need to support legacy API?
    if (IsOfficial())
    {
        // Create the legacy wrapper object
        inst.mLgObj = LgPickupObj(m_VM, id);
        // Obtain the legacy wrapper instance
        inst.mLgInst = inst.mLgObj.CastI< LgPickup >();
        // Make sure that both the instance and script object could be created
        if (!inst.mLgInst || inst.mLgObj.IsNull())
        {
            inst.ResetInstance();
            // Make sure that if an instance was created, it points to nothing
            if (inst.mLgInst)
            {
                LgPickupSetID(inst.mLgInst, -1);
            }
            // Make sure that if an object was created, it is released
            if (!inst.mLgObj.IsNull())
            {
                inst.mLgObj.Release();
            }
            // Now we can throw the error
            STHROWF("Unable to create a pickup wrapper instance for: {}", id);
        }
    }
#endif
    // Instantiate the entity manager
    DeleteGuard< CPickup > dg(new CPickup(id));
    // Create the script object
    inst.mObj = LightObj(dg.Get(), m_VM);
    // Store the manager instance itself
    inst.mInst = dg.Get();
    // The instance is now managed by the script
    dg.Release();
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        inst.ResetInstance();
#ifdef VCMP_ENABLE_OFFICIAL
        // Discard legacy wrapper instance
        if (IsOfficial())
        {
            LgPickupSetID(inst.mLgInst, -1);
            inst.mLgObj.Release();
            inst.mLgInst = nullptr;
        }
#endif
        // Make sure that if an instance was created, it points to nothing
        if (inst.mInst)
        {
            inst.mInst->m_ID = -1;
        }
        // Make sure that if an object was created, it is released
        if (!inst.mObj.IsNull())
        {
            inst.mObj.Release();
        }
        // Now we can throw the error
        STHROWF("Unable to create a pickup instance for: {}", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Initialize the instance events
    inst.InitEvents();
    // Let the script callbacks know about this entity
    EmitPickupCreated(id, header, payload);
    // Return the allocated instance
    return inst;
}

// --------------------------------------------------------------------------------------------
VehicleInst & Core::AllocVehicle(int32_t id, bool owned, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Cannot allocate vehicle with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    VehicleInst & inst = m_Vehicles[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst; // Return the existing instance
    }
#ifdef VCMP_ENABLE_OFFICIAL
    // Do we need to support legacy API?
    if (IsOfficial())
    {
        // Create the legacy wrapper object
        inst.mLgObj = LgVehicleObj(m_VM, id);
        // Obtain the legacy wrapper instance
        inst.mLgInst = inst.mLgObj.CastI< LgVehicle >();
        // Make sure that both the instance and script object could be created
        if (!inst.mLgInst || inst.mLgObj.IsNull())
        {
            inst.ResetInstance();
            // Make sure that if an instance was created, it points to nothing
            if (inst.mLgInst)
            {
                LgVehicleSetID(inst.mLgInst, -1);
            }
            // Make sure that if an object was created, it is released
            if (!inst.mLgObj.IsNull())
            {
                inst.mLgObj.Release();
            }
            // Now we can throw the error
            STHROWF("Unable to create a vehicle wrapper instance for: {}", id);
        }
    }
#endif
    // Instantiate the entity manager
    DeleteGuard< CVehicle > dg(new CVehicle(id));
    // Create the script object
    inst.mObj = LightObj(dg.Get(), m_VM);
    // Store the manager instance itself
    inst.mInst = dg.Get();
    // The instance is now managed by the script
    dg.Release();
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        inst.ResetInstance();
#ifdef VCMP_ENABLE_OFFICIAL
        // Discard legacy wrapper instance
        if (IsOfficial())
        {
            LgVehicleSetID(inst.mLgInst, -1);
            inst.mLgObj.Release();
            inst.mLgInst = nullptr;
        }
#endif
        // Make sure that if an instance was created, it points to nothing
        if (inst.mInst)
        {
            inst.mInst->m_ID = -1;
        }
        // Make sure that if an object was created, it is released
        if (!inst.mObj.IsNull())
        {
            inst.mObj.Release();
        }
        // Now we can throw the error
        STHROWF("Unable to create a vehicle instance for: {}", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Specify whether the entity is owned by this plug-in
    if (owned)
    {
        inst.mFlags |= ENF_OWNED;
    }
    else if (inst.mFlags & ENF_OWNED)
    {
        inst.mFlags ^= ENF_OWNED;
    }
    // Should we enable area tracking?
    if (m_AreasEnabled)
    {
        inst.mFlags |= ENF_AREA_TRACK;
    }
    // Initialize the instance events
    inst.InitEvents();
    // Let the script callbacks know about this entity
    EmitVehicleCreated(id, header, payload);
    // Return the allocated instance
    return inst;
}

// --------------------------------------------------------------------------------------------
void Core::DeallocBlip(int32_t id, bool destroy, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Cannot deallocate blip with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    BlipInst & inst = m_Blips[id];
    // Make sure that the instance is even allocated and we are allowed to destroy it
    if (VALID_ENTITY(inst.mID) && !(inst.mFlags & ENF_LOCKED))
    {
        inst.Destroy(destroy, header, payload); // Now attempt to destroy the entity from the server
    }
}

// --------------------------------------------------------------------------------------------
void Core::DeallocCheckpoint(int32_t id, bool destroy, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Cannot deallocate checkpoint with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    CheckpointInst & inst = m_Checkpoints[id];
    // Make sure that the instance is even allocated and we are allowed to destroy it
    if (VALID_ENTITY(inst.mID) && !(inst.mFlags & ENF_LOCKED))
    {
        inst.Destroy(destroy, header, payload); // Now attempt to destroy the entity from the server
    }
}

// --------------------------------------------------------------------------------------------
void Core::DeallocKeyBind(int32_t id, bool destroy, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Cannot deallocate keybind with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    KeyBindInst & inst = m_KeyBinds[id];
    // Make sure that the instance is even allocated and we are allowed to destroy it
    if (VALID_ENTITY(inst.mID) && !(inst.mFlags & ENF_LOCKED))
    {
        inst.Destroy(destroy, header, payload); // Now attempt to destroy the entity from the server
    }
}

// --------------------------------------------------------------------------------------------
void Core::DeallocObject(int32_t id, bool destroy, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Cannot deallocate object with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    ObjectInst & inst = m_Objects[id];
    // Make sure that the instance is even allocated and we are allowed to destroy it
    if (VALID_ENTITY(inst.mID) && !(inst.mFlags & ENF_LOCKED))
    {
        inst.Destroy(destroy, header, payload); // Now attempt to destroy the entity from the server
    }
}

// --------------------------------------------------------------------------------------------
void Core::DeallocPickup(int32_t id, bool destroy, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Cannot deallocate pickup with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    PickupInst & inst = m_Pickups[id];
    // Make sure that the instance is even allocated and we are allowed to destroy it
    if (VALID_ENTITY(inst.mID) && !(inst.mFlags & ENF_LOCKED))
    {
        inst.Destroy(destroy, header, payload); // Now attempt to destroy the entity from the server
    }
}

// --------------------------------------------------------------------------------------------
void Core::DeallocVehicle(int32_t id, bool destroy, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Cannot deallocate vehicle with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    VehicleInst & inst = m_Vehicles[id];
    // Make sure that the instance is even allocated and we are allowed to destroy it
    if (VALID_ENTITY(inst.mID) && !(inst.mFlags & ENF_LOCKED))
    {
        inst.Destroy(destroy, header, payload); // Now attempt to destroy the entity from the server
    }
}

// --------------------------------------------------------------------------------------------
BlipInst & Core::NewBlip(int32_t index, int32_t world, float x, float y, float z,
                            int32_t scale, uint32_t color, int32_t spr_id,
                            int32_t header, LightObj & payload)
{
    // Request the server to create this entity
    const int32_t id = _Func->CreateCoordBlip(index, world, x, y, z, scale, color, spr_id);
    // See if the entity creation failed on the server
    if (_Func->GetLastError() == vcmpErrorPoolExhausted)
    {
        STHROWF("Blip pool was exhausted: {}", id);
    }
    // Validate the identifier returned by the server
    else if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Server returned invalid blip: {}", id);
    }
    // Attempt to allocate this entity and grab the reference to the instance
    BlipInst & inst = AllocBlip(id, true, header, payload);
    // Just in case it was created during the notification for changes in entity pool
    if (VALID_ENTITY(inst.mID))
    {
        inst.mFlags |= ENF_OWNED;
    }
    // Now we can return the script object
    return inst;
}

// --------------------------------------------------------------------------------------------
CheckpointInst & Core::NewCheckpoint(int32_t player, int32_t world, bool sphere, float x, float y, float z,
                                uint8_t r, uint8_t g, uint8_t b, uint8_t a, float radius,
                                int32_t header, LightObj & payload)
{
    // Request the server to create this entity
    const int32_t id = _Func->CreateCheckPoint(player, world, sphere, x, y, z, r, g, b, a, radius);
    // See if the entity creation failed on the server
    if (_Func->GetLastError() == vcmpErrorNoSuchEntity)
    {
        STHROWF("Invalid player reference: {}", player);
    }
    else if (_Func->GetLastError() == vcmpErrorPoolExhausted)
    {
        STHROWF("Checkpoint pool was exhausted: {}", id);
    }
    // Validate the identifier returned by the server
    else if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Server returned invalid checkpoint: {}", id);
    }
    // Attempt to allocate this entity and grab the reference to the instance
    CheckpointInst & inst = AllocCheckpoint(id, true, header, payload);
    // Just in case it was created during the notification for changes in entity pool
    if (VALID_ENTITY(inst.mID))
    {
        inst.mFlags |= ENF_OWNED;
    }
    // Now we can return the script object
    return inst;
}

// --------------------------------------------------------------------------------------------
KeyBindInst & Core::NewKeyBind(int32_t slot, bool release, int32_t primary, int32_t secondary, int32_t alternative,
                            int32_t header, LightObj & payload)
{
    // Should we obtain a new keybind slot automatically?
    if (slot < 0)
    {
        slot = _Func->GetKeyBindUnusedSlot();
    }
    // Validate the keybind slot returned by the server
    if (INVALID_ENTITYEX(slot, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Server returned invalid keybind slot: {}", slot);
    }
    // Request the server to create this entity
    const vcmpError result = _Func->RegisterKeyBind(slot, release, primary, secondary, alternative);
    // See if the entity creation failed on the server
    if (result == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Out of bounds keybind argument: {}", slot);
    }
    // Attempt to allocate this entity and grab the reference to the instance
    KeyBindInst & inst = AllocKeyBind(slot, true, header, payload);
    // Just in case it was created during the notification for changes in entity pool
    if (VALID_ENTITY(inst.mID))
    {
        inst.mFlags |= ENF_OWNED;
    }
    // Now we can return the script object
    return inst;
}

// --------------------------------------------------------------------------------------------
ObjectInst & Core::NewObject(int32_t model, int32_t world, float x, float y, float z,  int32_t alpha,
                            int32_t header, LightObj & payload)
{
    // Request the server to create this entity
    const int32_t id = _Func->CreateObject(model, world, x, y, z, alpha);
    // See if the entity creation failed on the server
    if (_Func->GetLastError() == vcmpErrorPoolExhausted)
    {
        STHROWF("Object pool was exhausted: {}", id);
    }
    // Validate the identifier returned by the server
    else if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Server returned invalid object: {}", id);
    }
    // Attempt to allocate this entity and grab the reference to the instance
    ObjectInst & inst = AllocObject(id, true, header, payload);
    // Just in case it was created during the notification for changes in entity pool
    if (VALID_ENTITY(inst.mID))
    {
        inst.mFlags |= ENF_OWNED;
    }
    // Now we can return the script object
    return inst;
}

// --------------------------------------------------------------------------------------------
PickupInst & Core::NewPickup(int32_t model, int32_t world, int32_t quantity,
                            float x, float y, float z, int32_t alpha, bool automatic,
                            int32_t header, LightObj & payload)
{
    // Request the server to create this entity
    const int32_t id = _Func->CreatePickup(model, world, quantity, x, y, z, alpha, automatic);
    // See if the entity creation failed on the server
    if (_Func->GetLastError() == vcmpErrorPoolExhausted)
    {
        STHROWF("Pickup pool was exhausted: {}", id);
    }
    // Validate the identifier returned by the server
    else if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Server returned invalid pickup: {}", id);
    }
    // Attempt to allocate this entity and grab the reference to the instance
    PickupInst & inst = AllocPickup(id, true, header, payload);
    // Just in case it was created during the notification for changes in entity pool
    if (VALID_ENTITY(inst.mID))
    {
        inst.mFlags |= ENF_OWNED;
    }
    // Now we can return the script object
    return inst;
}

// --------------------------------------------------------------------------------------------
VehicleInst & Core::NewVehicle(int32_t model, int32_t world, float x, float y, float z,
                            float angle, int32_t primary, int32_t secondary,
                            int32_t header, LightObj & payload)
{

    // Request the server to create this entity
    const int32_t id = _Func->CreateVehicle(model, world, x, y, z, angle, primary, secondary);
    // See if the entity creation failed on the server
    if (_Func->GetLastError() == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Out of bounds vehicle argument: {}", id);
    }
    else if (_Func->GetLastError() == vcmpErrorPoolExhausted)
    {
        STHROWF("Vehicle pool was exhausted: {}", id);
    }
    // Validate the identifier returned by the server
    else if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Server returned invalid vehicle: {}", id);
    }
    // Attempt to allocate this entity and grab the reference to the instance
    VehicleInst & inst = AllocVehicle(id, true, header, payload);
    // Just in case it was created during the notification for changes in entity pool
    if (VALID_ENTITY(inst.mID))
    {
        inst.mFlags |= ENF_OWNED;
    }
    // Now we can return the script object
    return inst;
}

// --------------------------------------------------------------------------------------------
bool Core::DelBlip(int32_t id, int32_t header, LightObj & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocBlip(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DelCheckpoint(int32_t id, int32_t header, LightObj & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocCheckpoint(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DelKeyBind(int32_t id, int32_t header, LightObj & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocKeyBind(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DelObject(int32_t id, int32_t header, LightObj & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocObject(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DelPickup(int32_t id, int32_t header, LightObj & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocPickup(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DelVehicle(int32_t id, int32_t header, LightObj & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocVehicle(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::ConnectPlayer(int32_t id, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Cannot allocate player with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    PlayerInst & inst = m_Players[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return; // Nothing to allocate!
    }
#ifdef VCMP_ENABLE_OFFICIAL
    // Do we need to support legacy API?
    if (IsOfficial())
    {
        // Create the legacy wrapper object
        inst.mLgObj = LgPlayerObj(m_VM, id);
        // Obtain the legacy wrapper instance
        inst.mLgInst = inst.mLgObj.CastI< LgPlayer >();
        // Make sure that both the instance and script object could be created
        if (!inst.mLgInst || inst.mLgObj.IsNull())
        {
            inst.ResetInstance();
            // Make sure that if an instance was created, it points to nothing
            if (inst.mLgInst)
            {
                LgPlayerSetID(inst.mLgInst, -1);
            }
            // Make sure that if an object was created, it is released
            if (!inst.mLgObj.IsNull())
            {
                inst.mLgObj.Release();
            }
            // Now we can throw the error
            STHROWF("Unable to create a player wrapper instance for: {}", id);
        }
    }
#endif
    // Instantiate the entity manager
    DeleteGuard< CPlayer > dg(new CPlayer(id));
    // Create the script object
    inst.mObj = LightObj(dg.Get(), m_VM);
    // Store the manager instance itself
    inst.mInst = dg.Get();
    // The instance is now managed by the script
    dg.Release();
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        inst.ResetInstance();
#ifdef VCMP_ENABLE_OFFICIAL
        // Discard legacy wrapper instance
        if (IsOfficial())
        {
            LgPlayerSetID(inst.mLgInst, -1);
            inst.mLgObj.Release();
            inst.mLgInst = nullptr;
        }
#endif
        // Make sure that if an instance was created, it points to nothing
        if (inst.mInst)
        {
            inst.mInst->m_ID = -1;
        }
        // Make sure that if an object was created, it is released
        if (!inst.mObj.IsNull())
        {
            inst.mObj.Release();
        }
        // Now we can throw the error
        STHROWF("Unable to create a player instance for: {}", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Should we enable area tracking?
    if (m_AreasEnabled)
    {
        inst.mFlags |= ENF_AREA_TRACK;
    }
    // Initialize the position
    _Func->GetPlayerPosition(id, &inst.mLastPosition.x, &inst.mLastPosition.y, &inst.mLastPosition.z);
    // Initialize the remaining attributes
    inst.mLastWeapon = _Func->GetPlayerWeapon(id);
    inst.mLastHealth = _Func->GetPlayerHealth(id);
    inst.mLastArmour = _Func->GetPlayerArmour(id);
    inst.mLastHeading = _Func->GetPlayerHeading(id);
    // Initialize the instance events
    inst.InitEvents();
    // Let the script callbacks know about this entity
    EmitPlayerCreated(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::DisconnectPlayer(int32_t id, int32_t header, LightObj & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Cannot deallocate player with invalid identifier: {}", id);
    }
    // Retrieve the specified entity instance
    PlayerInst & inst = m_Players[id];
    // Make sure that the instance is even allocated and we are allowed to destroy it
    if (VALID_ENTITY(inst.mID) && !(inst.mFlags & ENF_LOCKED))
    {
        inst.Destroy(false, header, payload); // Now attempt to destroy the entity from the server
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ClearContainer(EntityType type)
{
    switch (type)
    {
        case ENT_BLIP:
        {
            m_Blips.clear();
        } break;
        case ENT_CHECKPOINT:
        {
            m_Checkpoints.clear();
        } break;
        case ENT_KEYBIND:
        {
            m_KeyBinds.clear();
        } break;
        case ENT_OBJECT:
        {
            m_Objects.clear();
        } break;
        case ENT_PICKUP:
        {
            m_Pickups.clear();
        } break;
        case ENT_PLAYER:
        {
            m_Players.clear();
        } break;
        case ENT_VEHICLE:
        {
            m_Vehicles.clear();
        } break;
        default: STHROWF("Cannot clear unknown entity type container");
    }
}

// ------------------------------------------------------------------------------------------------
void Core::InitEvents()
{
    // Ignore the call if already initialized
    if (!m_Events.IsNull())
    {
        return;
    }
    // Create a new table on the stack
    sq_newtableex(SqVM(), 128);
    // Grab the table object from the stack
    m_Events = LightObj(-1, SqVM());
    // Pop the table object from the stack
    sq_pop(SqVM(), 1);
    // Proceed to initializing the events
    InitSignalPair(mOnCustomEvent, m_Events, "CustomEvent");
    InitSignalPair(mOnBlipCreated, m_Events, "BlipCreated");
    InitSignalPair(mOnCheckpointCreated, m_Events, "CheckpointCreated");
    InitSignalPair(mOnKeyBindCreated, m_Events, "KeyBindCreated");
    InitSignalPair(mOnObjectCreated, m_Events, "ObjectCreated");
    InitSignalPair(mOnPickupCreated, m_Events, "PickupCreated");
    InitSignalPair(mOnPlayerCreated, m_Events, "PlayerCreated");
    InitSignalPair(mOnVehicleCreated, m_Events, "VehicleCreated");
    InitSignalPair(mOnBlipDestroyed, m_Events, "BlipDestroyed");
    InitSignalPair(mOnCheckpointDestroyed, m_Events, "CheckpointDestroyed");
    InitSignalPair(mOnKeyBindDestroyed, m_Events, "KeyBindDestroyed");
    InitSignalPair(mOnObjectDestroyed, m_Events, "ObjectDestroyed");
    InitSignalPair(mOnPickupDestroyed, m_Events, "PickupDestroyed");
    InitSignalPair(mOnPlayerDestroyed, m_Events, "PlayerDestroyed");
    InitSignalPair(mOnVehicleDestroyed, m_Events, "VehicleDestroyed");
    InitSignalPair(mOnBlipCustom, m_Events, "BlipCustom");
    InitSignalPair(mOnCheckpointCustom, m_Events, "CheckpointCustom");
    InitSignalPair(mOnKeyBindCustom, m_Events, "KeyBindCustom");
    InitSignalPair(mOnObjectCustom, m_Events, "ObjectCustom");
    InitSignalPair(mOnPickupCustom, m_Events, "PickupCustom");
    InitSignalPair(mOnPlayerCustom, m_Events, "PlayerCustom");
    InitSignalPair(mOnVehicleCustom, m_Events, "VehicleCustom");
#if SQMOD_SDK_LEAST(2, 1)
    InitSignalPair(mOnCheckpointStream, m_Events, "CheckpointStream");
    InitSignalPair(mOnObjectStream, m_Events, "ObjectStream");
    InitSignalPair(mOnPickupStream, m_Events, "PickupStream");
    InitSignalPair(mOnPlayerStream, m_Events, "PlayerStream");
    InitSignalPair(mOnVehicleStream, m_Events, "VehicleStream");
#endif
    InitSignalPair(mOnServerStartup, m_Events, "ServerStartup");
    InitSignalPair(mOnServerShutdown, m_Events, "ServerShutdown");
    InitSignalPair(mOnServerFrame, m_Events, "ServerFrame");
    InitSignalPair(mOnIncomingConnection, m_Events, "IncomingConnection");
    InitSignalPair(mOnPlayerRequestClass, m_Events, "PlayerRequestClass");
    InitSignalPair(mOnPlayerRequestSpawn, m_Events, "PlayerRequestSpawn");
    InitSignalPair(mOnPlayerSpawn, m_Events, "PlayerSpawn");
    InitSignalPair(mOnPlayerWasted, m_Events, "PlayerWasted");
    InitSignalPair(mOnPlayerKilled, m_Events, "PlayerKilled");
    InitSignalPair(mOnPlayerEmbarking, m_Events, "PlayerEmbarking");
    InitSignalPair(mOnPlayerEmbarked, m_Events, "PlayerEmbarked");
    InitSignalPair(mOnPlayerDisembark, m_Events, "PlayerDisembark");
    InitSignalPair(mOnPlayerRename, m_Events, "PlayerRename");
    InitSignalPair(mOnPlayerState, m_Events, "PlayerState");
    InitSignalPair(mOnStateNone, m_Events, "StateNone");
    InitSignalPair(mOnStateNormal, m_Events, "StateNormal");
    InitSignalPair(mOnStateAim, m_Events, "StateAim");
    InitSignalPair(mOnStateDriver, m_Events, "StateDriver");
    InitSignalPair(mOnStatePassenger, m_Events, "StatePassenger");
    InitSignalPair(mOnStateEnterDriver, m_Events, "StateEnterDriver");
    InitSignalPair(mOnStateEnterPassenger, m_Events, "StateEnterPassenger");
    InitSignalPair(mOnStateExit, m_Events, "StateExit");
    InitSignalPair(mOnStateUnspawned, m_Events, "StateUnspawned");
    InitSignalPair(mOnPlayerAction, m_Events, "PlayerAction");
    InitSignalPair(mOnActionNone, m_Events, "ActionNone");
    InitSignalPair(mOnActionNormal, m_Events, "ActionNormal");
    InitSignalPair(mOnActionAiming, m_Events, "ActionAiming");
    InitSignalPair(mOnActionShooting, m_Events, "ActionShooting");
    InitSignalPair(mOnActionJumping, m_Events, "ActionJumping");
    InitSignalPair(mOnActionLieDown, m_Events, "ActionLieDown");
    InitSignalPair(mOnActionGettingUp, m_Events, "ActionGettingUp");
    InitSignalPair(mOnActionJumpVehicle, m_Events, "ActionJumpVehicle");
    InitSignalPair(mOnActionDriving, m_Events, "ActionDriving");
    InitSignalPair(mOnActionDying, m_Events, "ActionDying");
    InitSignalPair(mOnActionWasted, m_Events, "ActionWasted");
    InitSignalPair(mOnActionEmbarking, m_Events, "ActionEmbarking");
    InitSignalPair(mOnActionDisembarking, m_Events, "ActionDisembarking");
    InitSignalPair(mOnPlayerBurning, m_Events, "PlayerBurning");
    InitSignalPair(mOnPlayerCrouching, m_Events, "PlayerCrouching");
    InitSignalPair(mOnPlayerGameKeys, m_Events, "PlayerGameKeys");
    InitSignalPair(mOnPlayerStartTyping, m_Events, "PlayerStartTyping");
    InitSignalPair(mOnPlayerStopTyping, m_Events, "PlayerStopTyping");
    InitSignalPair(mOnPlayerAway, m_Events, "PlayerAway");
    InitSignalPair(mOnPlayerMessage, m_Events, "PlayerMessage");
    InitSignalPair(mOnPlayerCommand, m_Events, "PlayerCommand");
    InitSignalPair(mOnPlayerPrivateMessage, m_Events, "PlayerPrivateMessage");
    InitSignalPair(mOnPlayerKeyPress, m_Events, "PlayerKeyPress");
    InitSignalPair(mOnPlayerKeyRelease, m_Events, "PlayerKeyRelease");
    InitSignalPair(mOnPlayerSpectate, m_Events, "PlayerSpectate");
    InitSignalPair(mOnPlayerUnspectate, m_Events, "PlayerUnspectate");
    InitSignalPair(mOnPlayerCrashReport, m_Events, "PlayerCrashReport");
    InitSignalPair(mOnPlayerModuleList, m_Events, "PlayerModuleList");
    InitSignalPair(mOnVehicleExplode, m_Events, "VehicleExplode");
    InitSignalPair(mOnVehicleRespawn, m_Events, "VehicleRespawn");
    InitSignalPair(mOnObjectShot, m_Events, "ObjectShot");
    InitSignalPair(mOnObjectTouched, m_Events, "ObjectTouched");
    InitSignalPair(mOnObjectWorld, m_Events, "ObjectWorld");
    InitSignalPair(mOnObjectAlpha, m_Events, "ObjectAlpha");
    InitSignalPair(mOnObjectReport, m_Events, "ObjectReport");
    InitSignalPair(mOnPickupClaimed, m_Events, "PickupClaimed");
    InitSignalPair(mOnPickupCollected, m_Events, "PickupCollected");
    InitSignalPair(mOnPickupRespawn, m_Events, "PickupRespawn");
    InitSignalPair(mOnPickupWorld, m_Events, "PickupWorld");
    InitSignalPair(mOnPickupAlpha, m_Events, "PickupAlpha");
    InitSignalPair(mOnPickupAutomatic, m_Events, "PickupAutomatic");
    InitSignalPair(mOnPickupAutoTimer, m_Events, "PickupAutoTimer");
    InitSignalPair(mOnPickupOption, m_Events, "PickupOption");
    InitSignalPair(mOnCheckpointEntered, m_Events, "CheckpointEntered");
    InitSignalPair(mOnCheckpointExited, m_Events, "CheckpointExited");
    InitSignalPair(mOnCheckpointWorld, m_Events, "CheckpointWorld");
    InitSignalPair(mOnCheckpointRadius, m_Events, "CheckpointRadius");
    InitSignalPair(mOnEntityPool, m_Events, "EntityPool");
    InitSignalPair(mOnClientScriptData, m_Events, "ClientScriptData");
    InitSignalPair(mOnPlayerUpdate, m_Events, "PlayerUpdate");
    InitSignalPair(mOnVehicleUpdate, m_Events, "VehicleUpdate");
    InitSignalPair(mOnPlayerHealth, m_Events, "PlayerHealth");
    InitSignalPair(mOnPlayerArmour, m_Events, "PlayerArmour");
    InitSignalPair(mOnPlayerWeapon, m_Events, "PlayerWeapon");
    InitSignalPair(mOnPlayerHeading, m_Events, "PlayerHeading");
    InitSignalPair(mOnPlayerPosition, m_Events, "PlayerPosition");
    InitSignalPair(mOnPlayerOption, m_Events, "PlayerOption");
    InitSignalPair(mOnPlayerAdmin, m_Events, "PlayerAdmin");
    InitSignalPair(mOnPlayerWorld, m_Events, "PlayerWorld");
    InitSignalPair(mOnPlayerTeam, m_Events, "PlayerTeam");
    InitSignalPair(mOnPlayerSkin, m_Events, "PlayerSkin");
    InitSignalPair(mOnPlayerMoney, m_Events, "PlayerMoney");
    InitSignalPair(mOnPlayerScore, m_Events, "PlayerScore");
    InitSignalPair(mOnPlayerWantedLevel, m_Events, "PlayerWantedLevel");
    InitSignalPair(mOnPlayerImmunity, m_Events, "PlayerImmunity");
    InitSignalPair(mOnPlayerAlpha, m_Events, "PlayerAlpha");
    InitSignalPair(mOnPlayerEnterArea, m_Events, "PlayerEnterArea");
    InitSignalPair(mOnPlayerLeaveArea, m_Events, "PlayerLeaveArea");
    InitSignalPair(mOnVehicleColor, m_Events, "VehicleColor");
    InitSignalPair(mOnVehicleHealth, m_Events, "VehicleHealth");
    InitSignalPair(mOnVehiclePosition, m_Events, "VehiclePosition");
    InitSignalPair(mOnVehicleRotation, m_Events, "VehicleRotation");
    InitSignalPair(mOnVehicleOption, m_Events, "VehicleOption");
    InitSignalPair(mOnVehicleWorld, m_Events, "VehicleWorld");
    InitSignalPair(mOnVehicleImmunity, m_Events, "VehicleImmunity");
    InitSignalPair(mOnVehiclePartStatus, m_Events, "VehiclePartStatus");
    InitSignalPair(mOnVehicleTyreStatus, m_Events, "VehicleTyreStatus");
    InitSignalPair(mOnVehicleDamageData, m_Events, "VehicleDamageData");
    InitSignalPair(mOnVehicleRadio, m_Events, "VehicleRadio");
    InitSignalPair(mOnVehicleHandlingRule, m_Events, "VehicleHandlingRule");
    InitSignalPair(mOnVehicleEnterArea, m_Events, "VehicleEnterArea");
    InitSignalPair(mOnVehicleLeaveArea, m_Events, "VehicleLeaveArea");
#if SQMOD_SDK_LEAST(2, 1)
    InitSignalPair(mOnEntityStream, m_Events, "EntityStream");
#endif
    InitSignalPair(mOnServerOption, m_Events, "ServerOption");
    InitSignalPair(mOnScriptReload, m_Events, "ScriptReload");
    InitSignalPair(mOnScriptLoaded, m_Events, "ScriptLoaded");
}
// ------------------------------------------------------------------------------------------------
void Core::DropEvents()
{
    ResetSignalPair(mOnCustomEvent);
    ResetSignalPair(mOnBlipCreated);
    ResetSignalPair(mOnCheckpointCreated);
    ResetSignalPair(mOnKeyBindCreated);
    ResetSignalPair(mOnObjectCreated);
    ResetSignalPair(mOnPickupCreated);
    ResetSignalPair(mOnPlayerCreated);
    ResetSignalPair(mOnVehicleCreated);
    ResetSignalPair(mOnBlipDestroyed);
    ResetSignalPair(mOnCheckpointDestroyed);
    ResetSignalPair(mOnKeyBindDestroyed);
    ResetSignalPair(mOnObjectDestroyed);
    ResetSignalPair(mOnPickupDestroyed);
    ResetSignalPair(mOnPlayerDestroyed);
    ResetSignalPair(mOnVehicleDestroyed);
    ResetSignalPair(mOnBlipCustom);
    ResetSignalPair(mOnCheckpointCustom);
    ResetSignalPair(mOnKeyBindCustom);
    ResetSignalPair(mOnObjectCustom);
    ResetSignalPair(mOnPickupCustom);
    ResetSignalPair(mOnPlayerCustom);
    ResetSignalPair(mOnVehicleCustom);
#if SQMOD_SDK_LEAST(2, 1)
    ResetSignalPair(mOnCheckpointStream);
    ResetSignalPair(mOnObjectStream);
    ResetSignalPair(mOnPickupStream);
    ResetSignalPair(mOnPlayerStream);
    ResetSignalPair(mOnVehicleStream);
#endif
    ResetSignalPair(mOnServerStartup);
    ResetSignalPair(mOnServerShutdown);
    ResetSignalPair(mOnServerFrame);
    ResetSignalPair(mOnIncomingConnection);
    ResetSignalPair(mOnPlayerRequestClass);
    ResetSignalPair(mOnPlayerRequestSpawn);
    ResetSignalPair(mOnPlayerSpawn);
    ResetSignalPair(mOnPlayerWasted);
    ResetSignalPair(mOnPlayerKilled);
    ResetSignalPair(mOnPlayerEmbarking);
    ResetSignalPair(mOnPlayerEmbarked);
    ResetSignalPair(mOnPlayerDisembark);
    ResetSignalPair(mOnPlayerRename);
    ResetSignalPair(mOnPlayerState);
    ResetSignalPair(mOnStateNone);
    ResetSignalPair(mOnStateNormal);
    ResetSignalPair(mOnStateAim);
    ResetSignalPair(mOnStateDriver);
    ResetSignalPair(mOnStatePassenger);
    ResetSignalPair(mOnStateEnterDriver);
    ResetSignalPair(mOnStateEnterPassenger);
    ResetSignalPair(mOnStateExit);
    ResetSignalPair(mOnStateUnspawned);
    ResetSignalPair(mOnPlayerAction);
    ResetSignalPair(mOnActionNone);
    ResetSignalPair(mOnActionNormal);
    ResetSignalPair(mOnActionAiming);
    ResetSignalPair(mOnActionShooting);
    ResetSignalPair(mOnActionJumping);
    ResetSignalPair(mOnActionLieDown);
    ResetSignalPair(mOnActionGettingUp);
    ResetSignalPair(mOnActionJumpVehicle);
    ResetSignalPair(mOnActionDriving);
    ResetSignalPair(mOnActionDying);
    ResetSignalPair(mOnActionWasted);
    ResetSignalPair(mOnActionEmbarking);
    ResetSignalPair(mOnActionDisembarking);
    ResetSignalPair(mOnPlayerBurning);
    ResetSignalPair(mOnPlayerCrouching);
    ResetSignalPair(mOnPlayerGameKeys);
    ResetSignalPair(mOnPlayerStartTyping);
    ResetSignalPair(mOnPlayerStopTyping);
    ResetSignalPair(mOnPlayerAway);
    ResetSignalPair(mOnPlayerMessage);
    ResetSignalPair(mOnPlayerCommand);
    ResetSignalPair(mOnPlayerPrivateMessage);
    ResetSignalPair(mOnPlayerKeyPress);
    ResetSignalPair(mOnPlayerKeyRelease);
    ResetSignalPair(mOnPlayerSpectate);
    ResetSignalPair(mOnPlayerUnspectate);
    ResetSignalPair(mOnPlayerCrashReport);
    ResetSignalPair(mOnPlayerModuleList);
    ResetSignalPair(mOnVehicleExplode);
    ResetSignalPair(mOnVehicleRespawn);
    ResetSignalPair(mOnObjectShot);
    ResetSignalPair(mOnObjectTouched);
    ResetSignalPair(mOnObjectWorld);
    ResetSignalPair(mOnObjectAlpha);
    ResetSignalPair(mOnObjectReport);
    ResetSignalPair(mOnPickupClaimed);
    ResetSignalPair(mOnPickupCollected);
    ResetSignalPair(mOnPickupRespawn);
    ResetSignalPair(mOnPickupWorld);
    ResetSignalPair(mOnPickupAlpha);
    ResetSignalPair(mOnPickupAutomatic);
    ResetSignalPair(mOnPickupAutoTimer);
    ResetSignalPair(mOnPickupOption);
    ResetSignalPair(mOnCheckpointEntered);
    ResetSignalPair(mOnCheckpointExited);
    ResetSignalPair(mOnCheckpointWorld);
    ResetSignalPair(mOnCheckpointRadius);
    ResetSignalPair(mOnEntityPool);
    ResetSignalPair(mOnClientScriptData);
    ResetSignalPair(mOnPlayerUpdate);
    ResetSignalPair(mOnVehicleUpdate);
    ResetSignalPair(mOnPlayerHealth);
    ResetSignalPair(mOnPlayerArmour);
    ResetSignalPair(mOnPlayerWeapon);
    ResetSignalPair(mOnPlayerHeading);
    ResetSignalPair(mOnPlayerPosition);
    ResetSignalPair(mOnPlayerOption);
    ResetSignalPair(mOnPlayerAdmin);
    ResetSignalPair(mOnPlayerWorld);
    ResetSignalPair(mOnPlayerTeam);
    ResetSignalPair(mOnPlayerSkin);
    ResetSignalPair(mOnPlayerMoney);
    ResetSignalPair(mOnPlayerScore);
    ResetSignalPair(mOnPlayerWantedLevel);
    ResetSignalPair(mOnPlayerImmunity);
    ResetSignalPair(mOnPlayerAlpha);
    ResetSignalPair(mOnPlayerEnterArea);
    ResetSignalPair(mOnPlayerLeaveArea);
    ResetSignalPair(mOnVehicleColor);
    ResetSignalPair(mOnVehicleHealth);
    ResetSignalPair(mOnVehiclePosition);
    ResetSignalPair(mOnVehicleRotation);
    ResetSignalPair(mOnVehicleOption);
    ResetSignalPair(mOnVehicleWorld);
    ResetSignalPair(mOnVehicleImmunity);
    ResetSignalPair(mOnVehiclePartStatus);
    ResetSignalPair(mOnVehicleTyreStatus);
    ResetSignalPair(mOnVehicleDamageData);
    ResetSignalPair(mOnVehicleRadio);
    ResetSignalPair(mOnVehicleHandlingRule);
    ResetSignalPair(mOnVehicleEnterArea);
    ResetSignalPair(mOnVehicleLeaveArea);
#if SQMOD_SDK_LEAST(2, 1)
    ResetSignalPair(mOnEntityStream);
#endif
    ResetSignalPair(mOnServerOption);
    ResetSignalPair(mOnScriptReload);
    ResetSignalPair(mOnScriptLoaded);
    m_Events.Release();
}

// ------------------------------------------------------------------------------------------------
extern bool GetReloadStatus();
extern void SetReloadStatus(bool toggle);

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(CoreStateTypename, _SC("SqCoreState"))

// ------------------------------------------------------------------------------------------------
static SQInteger SqLoadScript(HSQUIRRELVM vm)
{
    const int32_t top = sq_gettop(vm);
    // Was the delay option specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing delay parameter");
    }
    // Was the script path specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing script path");
    }
    // Whether the script execution is delayed
    SQBool delay = SQFalse;
    // Attempt to generate the string value
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    else if (SQ_FAILED(sq_getbool(vm, 2, &delay)))
    {
        return sq_throwerror(vm, "Failed to retrieve the delay parameter");
    }
    Function cb;
    LightObj ctx;
    // Forward the call to the actual implementation
    sq_pushbool(vm, Core::Get().LoadScript(val.mPtr, cb, ctx, static_cast< bool >(delay)));
    // We have an argument on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static bool SqLoadScriptNotify(bool delay, StackStrF & path, LightObj & ctx, Function & cb)
{
    return Core::Get().LoadScript(path.mPtr, cb, ctx, delay);
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqGetEvents(HSQUIRRELVM vm)
{
    // Push the events table object on the stack
    sq_pushobject(vm, Core::Get().GetEvents().mObj);
    // Specify that we're returning a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqGetOnScript(HSQUIRRELVM vm)
{
    // Push the events table object on the stack
    sq_pushobject(vm, Core::Get().mOnScript.second);
    // Specify that we're returning a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static void SqEmitCustomEvent(int32_t group, int32_t header, LightObj & payload)
{
    Core::Get().EmitCustomEvent(group, header, payload);
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqForceEnableNullEntities(HSQUIRRELVM SQ_UNUSED_ARG(vm))
{
    Core::Get().EnableNullEntities();
    return 0;
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetPreLoadEvent()
{
    return Core::Get().GetPreLoadEvent();
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetPostLoadEvent()
{
    return Core::Get().GetPostLoadEvent();
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetUnloadEvent()
{
    return Core::Get().GetUnloadEvent();
}

// ------------------------------------------------------------------------------------------------
static bool SqGetReloadStatus()
{
    return GetReloadStatus();
}

// ------------------------------------------------------------------------------------------------
static void SqSetReloadStatus(bool toggle)
{
    SetReloadStatus(toggle);
}

// ------------------------------------------------------------------------------------------------
static void SqReloadBecause(int32_t header, LightObj & payload)
{
    // Assign the reload info
    Core::Get().SetReloadInfo(header, payload);
    // Enable reloading
    SetReloadStatus(true);
}

// ------------------------------------------------------------------------------------------------
static void SqSetReloadInfo(int32_t header, LightObj & payload)
{
    Core::Get().SetReloadInfo(header, payload);
}

// ------------------------------------------------------------------------------------------------
static int32_t SqGetReloadHeader()
{
    return Core::Get().GetReloadHeader();
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetReloadPayload()
{
    return Core::Get().GetReloadPayload();
}

// ------------------------------------------------------------------------------------------------
static int32_t SqGetState()
{
    return Core::Get().GetState();
}

// ------------------------------------------------------------------------------------------------
static void SqSetState(int32_t value)
{
    return Core::Get().SetState(value);
}

// ------------------------------------------------------------------------------------------------
static bool SqGetAreasEnabled()
{
    return Core::Get().AreasEnabled();
}

// ------------------------------------------------------------------------------------------------
static void SqSetAreasEnabled(bool toggle)
{
    Core::Get().AreasEnabled(toggle);
}

// ------------------------------------------------------------------------------------------------
static const String & SqGetOption(StackStrF & name)
{
    return Core::Get().GetOption(String(name.mPtr, name.mLen));
}

// ------------------------------------------------------------------------------------------------
static const String & SqGetOptionOr(StackStrF & name, StackStrF & value)
{
    static String s;
    s.assign(value.mPtr, value.GetSize());
    return Core::Get().GetOption(String(name.mPtr, name.mLen), s);
}

// ------------------------------------------------------------------------------------------------
static void SqSetOption(StackStrF & name, StackStrF & value)
{
    Core::Get().SetOption(String(name.mPtr, name.mLen), String(value.mPtr, value.mLen));
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetBlip(int32_t id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Out of range blip identifier: {}", id);
    }
    // Return the requested information
    return Core::Get().GetBlip(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetCheckpoint(int32_t id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Out of range checkpoint identifier: {}", id);
    }
    // Return the requested information
    return Core::Get().GetCheckpoint(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetKeyBind(int32_t id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Out of range keybind identifier: {}", id);
    }
    // Return the requested information
    return Core::Get().GetKeyBind(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetObj(int32_t id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Out of range object identifier: {}", id);
    }
    // Return the requested information
    return Core::Get().GetObj(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetPickup(int32_t id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Out of range blip identifier: {}", id);
    }
    // Return the requested information
    return Core::Get().GetPickup(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetPlayer(int32_t id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Out of range player identifier: {}", id);
    }
    // Return the requested information
    return Core::Get().GetPlayer(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetVehicle(int32_t id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Out of range vehicle identifier: {}", id);
    }
    // Return the requested information
    return Core::Get().GetVehicle(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static bool SqDelBlip(int32_t id, int32_t header, LightObj & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Out of range blip identifier: {}", id);
    }
    // Perform the requested operation
    return Core::Get().DelBlip(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static bool SqDelCheckpoint(int32_t id, int32_t header, LightObj & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Out of range checkpoint identifier: {}", id);
    }
    // Perform the requested operation
    return Core::Get().DelCheckpoint(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static bool SqDelKeyBind(int32_t id, int32_t header, LightObj & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Out of range keybind identifier: {}", id);
    }
    // Perform the requested operation
    return Core::Get().DelKeyBind(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static bool SqDelObject(int32_t id, int32_t header, LightObj & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Out of range object identifier: {}", id);
    }
    // Perform the requested operation
    return Core::Get().DelObject(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static bool SqDelPickup(int32_t id, int32_t header, LightObj & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Out of range blip identifier: {}", id);
    }
    // Perform the requested operation
    return Core::Get().DelPickup(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static bool SqDelVehicle(int32_t id, int32_t header, LightObj & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Out of range vehicle identifier: {}", id);
    }
    // Perform the requested operation
    return Core::Get().DelVehicle(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetClientDataBuffer()
{
    return Core::Get().GetClientDataBuffer();
}

// ================================================================================================
void Register_Core(HSQUIRRELVM vm)
{
    Table corens(vm);

    corens.Bind(_SC("State"),
        Class< CoreState, NoCopy< CoreState > >(vm, CoreStateTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< int >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &CoreStateTypename::Fn)
        // Member Properties
        .Prop(_SC("Value"), &CoreState::GetState, &CoreState::SetState)
        .Prop(_SC("Init"), &CoreState::GetStart)
        // Member Methods
        .Func(_SC("Propose"), &CoreState::SetState)
        .Func(_SC("Regress"), &CoreState::Regress)
    );

    corens
        .Func(_SC("Reload"), &SqSetReloadStatus)
        .Func(_SC("Reloading"), &SqGetReloadStatus)
        .Func(_SC("ReloadBecause"), &SqReloadBecause)
        .Func(_SC("SetReloadInfo"), &SqSetReloadInfo)
        .Func(_SC("GetReloadHeader"), &SqGetReloadHeader)
        .Func(_SC("GetReloadPayload"), &SqGetReloadPayload)
        .Func(_SC("CustomEvent"), &SqEmitCustomEvent)
        .Func(_SC("GetState"), &SqGetState)
        .Func(_SC("SetState"), &SqSetState)
        .Func(_SC("AreasEnabled"), &SqGetAreasEnabled)
        .Func(_SC("SetAreasEnabled"), &SqSetAreasEnabled)
        .Func(_SC("GetOption"), &SqGetOption)
        .Func(_SC("GetOptionOr"), &SqGetOptionOr)
        .Func(_SC("SetOption"), &SqSetOption)
        .Func(_SC("GetBlip"), &SqGetBlip)
        .Func(_SC("GetCheckpoint"), &SqGetCheckpoint)
        .Func(_SC("GetKeyBind"), &SqGetKeyBind)
        .Func(_SC("GetObj"), &SqGetObj)
        .Func(_SC("GetPickup"), &SqGetPickup)
        .Func(_SC("GetPlayer"), &SqGetPlayer)
        .Func(_SC("GetVehicle"), &SqGetVehicle)
        .Func(_SC("DestroyBlip"), &SqDelBlip)
        .Func(_SC("DestroyCheckpoint"), &SqDelCheckpoint)
        .Func(_SC("DestroyKeyBind"), &SqDelKeyBind)
        .Func(_SC("DestroyObject"), &SqDelObject)
        .Func(_SC("DestroyPickup"), &SqDelPickup)
        .Func(_SC("DestroyVehicle"), &SqDelVehicle)
        .Func(_SC("ClientDataBuffer"), &SqGetClientDataBuffer)
        .Func(_SC("OnPreLoad"), &SqGetPreLoadEvent)
        .Func(_SC("OnPostLoad"), &SqGetPostLoadEvent)
        .Func(_SC("OnUnload"), &SqGetUnloadEvent)
        .CbFunc(_SC("LoadScriptNotify"), &SqLoadScriptNotify)
        .SquirrelFunc(_SC("ForceEnableNullEntities"), &SqForceEnableNullEntities)
        .SquirrelFunc(_SC("LoadScript"), &SqLoadScript, -3, ".b.")
        .SquirrelFunc(_SC("OnScript"), &SqGetOnScript)
        .SquirrelFunc(_SC("On"), &SqGetEvents);

    RootTable(vm).Bind(_SC("SqCore"), corens);
}

} // Namespace:: SqMod

/* ------------------------------------------------------------------------------------------------
 * Include event functionality.
*/
#include "Core/Events.inc"
