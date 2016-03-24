// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Logger.hpp"

// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Entity/Checkpoint.hpp"
#include "Entity/Forcefield.hpp"
#include "Entity/Keybind.hpp"
#include "Entity/Object.hpp"
#include "Entity/Pickup.hpp"
#include "Entity/Player.hpp"
#include "Entity/Sprite.hpp"
#include "Entity/Textdraw.hpp"
#include "Entity/Vehicle.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqstdio.h>
#include <sqstdblob.h>
#include <sqstdmath.h>
#include <sqstdsystem.h>
#include <sqstdstring.h>
#include <SimpleIni.h>

// ------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>

// ------------------------------------------------------------------------------------------------
#include <exception>
#include <stdexcept>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void DisableReload();
extern bool RegisterAPI(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void ProcessRoutine();
extern void TerminateRoutine();

// ------------------------------------------------------------------------------------------------
extern Int32 RunCommand(Int32 invoker, CSStr command);
extern void TerminateCommand();

// ------------------------------------------------------------------------------------------------
SQMOD_MANAGEDPTR_TYPE(Core) _Core = SQMOD_MANAGEDPTR_MAKE(Core, nullptr);

/* ------------------------------------------------------------------------------------------------
 * Utility class used to release the destroy lock if unable to complete the process.
*/
struct EntLockGuard
{
private:

    // --------------------------------------------------------------------------------------------
    Uint16 & m_Flags; // Reference to the guarded entity flags.

public:

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    EntLockGuard(Uint16 & ref)
        : m_Flags(ref)
    {
        m_Flags |= ENF_LOCKED;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~EntLockGuard()
    {
        if (m_Flags & ENF_LOCKED)
        {
            m_Flags ^= ENF_LOCKED;
        }
    }
};

// ------------------------------------------------------------------------------------------------
Core::Core()
    : m_State(0)
    , m_VM(nullptr)
    , m_Scripts()
    , m_Options()
    , m_Blips()
    , m_Checkpoints()
    , m_Forcefields()
    , m_Keybinds()
    , m_Objects()
    , m_Pickups()
    , m_Players()
    , m_Sprites()
    , m_Textdraws()
    , m_Vehicles()
{

}

// ------------------------------------------------------------------------------------------------
Core::~Core()
{
    // Don't release the callbacks abruptly in destructor
    ResetFunc();
}

// ------------------------------------------------------------------------------------------------
bool Core::Init()
{
    if (cLogWrn(m_VM, "Already initialized"))
        return true;

    LogDbg("Resizing the entity containers");
    // Make sure the entity containers have the proper size
    m_Blips.resize(SQMOD_BLIP_POOL);
    m_Checkpoints.resize(SQMOD_CHECKPOINT_POOL);
    m_Forcefields.resize(SQMOD_FORCEFIELD_POOL);
    m_Keybinds.resize(SQMOD_KEYBIND_POOL);
    m_Objects.resize(SQMOD_OBJECT_POOL);
    m_Pickups.resize(SQMOD_PICKUP_POOL);
    m_Players.resize(SQMOD_PLAYER_POOL);
    m_Sprites.resize(SQMOD_SPRITE_POOL);
    m_Textdraws.resize(SQMOD_TEXTDRAW_POOL);
    m_Vehicles.resize(SQMOD_VEHICLE_POOL);

    LogDbg("Initializing entity options to defaults");
    // Initialize player messaging options to default values
    for (Players::iterator itr = m_Players.begin(); itr != m_Players.end(); ++itr)
    {
        for (unsigned n = 0; n < SQMOD_PLAYER_MSG_PREFIXES; ++n)
        {
            itr->mPrefixes[n].clear();
        }
        itr->mMessageColor = 0x6599FFFF;
        itr->mAnnounceStyle = 1;
    }

    LogDbg("Reading configuration file (sqmod.ini)");
    CSimpleIniA conf(false, true, true);
    // Attempt to load the configurations from disk
    SI_Error ini_ret = conf.LoadFile("sqmod.ini");
    // See if the configurations could be loaded
    if (ini_ret < 0)
    {
        switch (ini_ret)
        {
            case SI_FAIL:
                LogFtl("Failed to load the configuration file. Probably invalid");
            break;
            case SI_NOMEM:
                LogFtl("Run out of memory while loading the configuration file");
            break;
            case SI_FILE:
                LogFtl("Failed to load the configuration file. %s", strerror(errno));
            break;
            default:
                LogFtl("Failed to load the configuration file for some unforeseen reason");
        }
        // Failed to load the configuration file
        return false;
    }

    Uint16 stack_size = SQMOD_STACK_SIZE;
    // Attempt to read the database port number
    try
    {
        Ulong num = conf.GetLongValue("Config", "StackSize", SQMOD_STACK_SIZE);
        // Make sure that the retrieved number is within range
        if (!num)
            throw std::out_of_range("stack size too small");
        else if (num >= std::numeric_limits< Uint16 >::max())
            throw std::out_of_range("stack size too big");
        // Save the port number
        stack_size = static_cast< Uint16 >(num);
    }
    catch (const std::exception & e)
    {
        LogWrn("Unable to obtain a valid stack size [%s]", e.what());
    }

    LogDbg("Creating a virtual machine with a stack size of (%d)", stack_size);
    // Attempt to create the VM
    m_VM = sq_open(stack_size);
    // See if the virtual machine could be created
    if (cLogFtl(!m_VM, "Unable to create the virtual machine"))
    {
        // Explicitly prevent further use of this pointer
        m_VM = nullptr;
        // Unable to load the plugin properly
        return false;
    }
    // Set this as the default VM
    DefaultVM::Set(m_VM);
    // Enable error handling
    ErrorHandling::Enable(true);

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

    LogDbg("Setting the base output function");
    // Tell the VM to use these functions to output user on error messages
    sq_setprintfunc(m_VM, PrintFunc, ErrorFunc);

    LogDbg("Setting the base error handlers");
    // Tell the VM to trigger this function on compile time errors
    sq_setcompilererrorhandler(m_VM, CompilerErrorHandler);
    // Push the runtime error handler on the stack and create a closure
    sq_newclosure(m_VM, RuntimeErrorHandler, 0);
    // Tell the VM to trigger this function on runtime errors
    sq_seterrorhandler(m_VM);

    LogDbg("Registering the plug-in API");
    // Attempt to register the plugin API
    if (cLogFtl(!RegisterAPI(m_VM), "Unable to register the plug-in API"))
        return false; // Can't execute scripts without a valid API!

    // Attempt to retrieve the list of strings specified in the config
    CSimpleIniA::TNamesDepend scripts;
    conf.GetAllValues("Scripts", "Source", scripts);
    // See if any script was specified
    if (scripts.size() <= 0 && !conf.GetBoolValue("Config", "EmptyInit", false))
    {
        LogWrn("No scripts specified in the configuration file");
        // No point in loading the plug-in
        return false;
    }
    else
    {
        // Sort the list in it's original order
        scripts.sort(CSimpleIniA::Entry::LoadOrder());
        // Process each specified script paths
        for (const auto & script : scripts)
            // Attempt to queue the specified script path for loading
            LoadScript(script.pItem);
    }
    // See if any script could be queued for loading
    if (m_Scripts.empty() && !conf.GetBoolValue("Config", "EmptyInit", false))
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
        // Process all the specified keys under the [Options] section
        for (const auto & option : options)
        {
            CSimpleIniA::TNamesDepend values;
            // Get the values of all keys with the same name
            if (!conf.GetAllValues("Options", option.pItem, values))
                continue;
            // Sort the keys in their original order
            values.sort(CSimpleIniA::Entry::LoadOrder());
            // Save each option option and overwrite existing value
            for (const auto & value : values)
                m_Options[option.pItem] = value.pItem;
        }
    }
    else
        LogInf("No options specified in the configuration file");

    LogDbg("Applying the specified logging filters");
    // Apply the specified logging filters only after initialization was completed
    if (!conf.GetBoolValue("Log", "Debug", true))
        _Log->DisableLevel(LL_DBG);
    if (!conf.GetBoolValue("Log", "User", true))
        _Log->DisableLevel(LL_USR);
    if (!conf.GetBoolValue("Log", "Success", true))
        _Log->DisableLevel(LL_SCS);
    if (!conf.GetBoolValue("Log", "Info", true))
        _Log->DisableLevel(LL_INF);
    if (!conf.GetBoolValue("Log", "Warning", true))
        _Log->DisableLevel(LL_WRN);
    if (!conf.GetBoolValue("Log", "Error", true))
        _Log->DisableLevel(LL_ERR);
    if (!conf.GetBoolValue("Log", "Fatal", true))
        _Log->DisableLevel(LL_FTL);

    // Initialization successful
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::Load()
{
    // Are there any scripts to execute?
    if (cLogErr(m_Scripts.empty(), "No scripts to execute. Plug-in has no purpose"))
        return false; // No reason to load the plug-in

    LogDbg("Signaling outside plugins to register their API");
    // Signal outside plugins to do their monkey business
    _Func->SendCustomCommand(0xDEADBABE, "");

    LogDbg("Attempting to execute the specified scripts");
    // Go through each specified script
    for (Scripts::iterator itr = m_Scripts.begin(); itr != m_Scripts.end(); ++itr)
    {
        // Attempt to load and compile the script file
        try
        {
            itr->second.CompileFile(itr->first);
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to compile: %s", itr->first.c_str());
            // Failed to load properly
            return false;
        }
        // Attempt to execute the compiled script code
        try
        {
            itr->second.Run();
        }
        catch (const Sqrat::Exception & e)
        {
            LogFtl("Unable to execute: %s", itr->first.c_str());
            // Failed to load properly
            return false;
        }
        // At this point the script should be completely loaded
        LogScs("Successfully executed script: %s", itr->first.c_str());
    }

    // Import already existing entities
    ImportPlayers();
    ImportBlips();
    ImportCheckpoints();
    ImportForcefields();
    ImportKeybinds();
    ImportObjects();
    ImportPickups();
    ImportSprites();
    ImportTextdraws();
    ImportVehicles();

    // Notify the script callback that the script was loaded
    EmitScriptLoaded();

    // Successfully loaded
    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::Terminate()
{
    LogDbg("Signaling outside plugins to release their resources");
    // Signal outside plugins to do their monkey business
    _Func->SendCustomCommand(0xDEADC0DE, "");
    // Release all entity resources by clearing the containers
    m_Players.clear();
    m_Blips.clear();
    m_Checkpoints.clear();
    m_Keybinds.clear();
    m_Objects.clear();
    m_Pickups.clear();
    m_Forcefields.clear();
    m_Sprites.clear();
    m_Textdraws.clear();
    m_Vehicles.clear();
    m_Scripts.clear();
    // Release all resources from routines
    TerminateRoutine();
    // Release all resources from command manager
    TerminateCommand();
    // In case there's a payload for reload
    DisableReload();
    // Is there a VM to close?
    if (m_VM)
    {
        // Release all script callbacks
        ResetFunc();
        // Release the script instances
        m_Scripts.clear();
        // Assertions during close may cause double delete!
        HSQUIRRELVM sq_vm = m_VM;
        m_VM = nullptr;
        // Attempt to close the VM
        sq_close(sq_vm);
    }
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
    if (!filepath || *filepath == 0)
        return false; // Simply ignore it
    // Get the file path as a string
    String path(filepath);
    // See if it wasn't already loaded
    if (m_Scripts.find(path) != m_Scripts.end())
        LogWrn("Script was specified before: %s", path.c_str());
    // We don't compile the scripts yet. We just store their path and prepare the objects.
    else
        // Create a new script container and insert it into the script pool
        m_Scripts.emplace(std::move(path), Script(m_VM));
    // At this point the script exists in the pool
    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::PrintFunc(HSQUIRRELVM /*vm*/, CSStr msg, ...)
{
    va_list args;
    va_start(args, msg);
    _Log->Send(LL_USR, false, msg, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void Core::ErrorFunc(HSQUIRRELVM /*vm*/, CSStr msg, ...)
{
    va_list args;
    va_start(args, msg);
    _Log->Debug(msg, args);
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
SQInteger Core::RuntimeErrorHandler(HSQUIRRELVM vm)
{
    if (sq_gettop(vm) < 1)
    {
        return 0;
    }

    CSStr err_msg = nullptr;

    if (SQ_SUCCEEDED(sq_getstring(vm, 2, &err_msg)))
    {
        _Log->Debug("%s", err_msg);
    }
    else
    {
        _Log->Debug(_SC("unknown runtime error has occurred"));
    }

    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
void Core::CompilerErrorHandler(HSQUIRRELVM /*vm*/, CSStr desc, CSStr src, SQInteger line, SQInteger column)
{
    LogFtl("Message: %s\n[\n=>Location: %s\n=>Line: %d\n=>Column: %d\n]", desc, src, line, column);
}

// --------------------------------------------------------------------------------------------
void Core::ImportBlips()
{
    for (Int32 i = 0; i < SQMOD_BLIP_POOL; ++i)
    {
        // Information about the blip entity
        Int32 world = -1, scale = -1, sprid = -1;
        Uint32 color = 0;
        Float32 x = 0.0, y = 0.0, z = 0.0;
        // See if this entity exists on the server
        if (!_Func->GetCoordBlipInfo(i, &world, &x, &y, &z, &scale, &color, &sprid))
        {
            continue; // Nothing to import!
        }
        // Make sure this entity was not allocated already
        else if (INVALID_ENTITY(m_Blips[i].mID))
        {
            AllocBlip(i, false, SQMOD_CREATE_IMPORT, NullObject());
            // Assign the instance values
            m_Blips[i].mWorld = world;
            m_Blips[i].mScale = scale;
            m_Blips[i].mSprID = sprid;
            m_Blips[i].mPosition.Set(x, y, z);
            m_Blips[i].mColor.SetRGBA(color);
        }
    }
}

void Core::ImportCheckpoints()
{
    for (Int32 i = 0; i < SQMOD_CHECKPOINT_POOL; ++i)
    {
        // See if this entity exists on the server
        if (_Func->GetCheckpointWorld(i) < 0)
        {
            continue; // Nothing to import!
        }
        // Make sure this entity was not allocated already
        else if (INVALID_ENTITY(m_Checkpoints[i].mID))
        {
            AllocCheckpoint(i, false, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

void Core::ImportForcefields()
{
    for (Int32 i = 0; i < SQMOD_FORCEFIELD_POOL; ++i)
    {
        // See if this entity exists on the server
        if (_Func->GetSphereWorld(i) < 0)
        {
            continue; // Nothing to import!
        }
        // Make sure this entity was not allocated already
        else if (INVALID_ENTITY(m_Forcefields[i].mID))
        {
            AllocForcefield(i, false, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

void Core::ImportKeybinds()
{
    for (Int32 i = 0; i < SQMOD_KEYBIND_POOL; ++i)
    {
        Uint32 release = 0;
        Int32 first = -1, second = -1, third = -1;
        // See if this entity exists on the server
        if (!_Func->GetKeyBindData(i, &release, &first, &second, &third))
        {
            continue; // Nothing to import!
        }
        // Make sure this entity was not allocated already
        else if (INVALID_ENTITY(m_Keybinds[i].mID))
        {
            AllocKeybind(i, false, SQMOD_CREATE_IMPORT, NullObject());
            // Assign the instance values
            m_Keybinds[i].mFirst = first;
            m_Keybinds[i].mSecond = second;
            m_Keybinds[i].mThird = third;
            m_Keybinds[i].mRelease = static_cast< bool >(release);
        }
    }
}

void Core::ImportObjects()
{
    for (Int32 i = 0; i < SQMOD_OBJECT_POOL; ++i)
    {
        // See if this entity exists on the server
        if (_Func->GetObjectModel(i) <= 0)
        {
            continue; // Nothing to import!
        }
        // Make sure this entity was not allocated already
        else if (INVALID_ENTITY(m_Objects[i].mID))
        {
            AllocObject(i, false, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

void Core::ImportPickups()
{
    for (Int32 i = 0; i < SQMOD_PICKUP_POOL; ++i)
    {
        // See if this entity exists on the server
        if (_Func->PickupGetModel(i) <= 0)
        {
            continue; // Nothing to import!
        }
        // Make sure this entity was not allocated already
        else if (INVALID_ENTITY(m_Pickups[i].mID))
        {
            AllocPickup(i, false, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

void Core::ImportPlayers()
{
    for (Int32 i = 0; i < SQMOD_PLAYER_POOL; ++i)
    {
        // See if this entity exists on the server
        if (!_Func->IsPlayerConnected(i))
        {
            continue; // Nothing to import!
        }
        // Make sure this entity was not allocated already
        else if (INVALID_ENTITY(m_Players[i].mID))
        {
            ConnectPlayer(i, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}

void Core::ImportSprites()
{
    /*
    for (Int32 i = 0; i < SQMOD_SPRITE_POOL; ++i)
    {
        // Not possible to detect with current SDK
    }
    */
}

void Core::ImportTextdraws()
{
    /*
    for (Int32 i = 0; i < SQMOD_TEXTDRAW_POOL; ++i)
    {
        // Not possible to detect with current SDK
    }
    */
}

void Core::ImportVehicles()
{
    for (Int32 i = 0; i < SQMOD_VEHICLE_POOL; ++i)
    {
        // See if this entity exists on the server
        if (_Func->GetVehicleModel(i) <= 0)
        {
            continue; // Nothing to import!
        }
        // Make sure this entity was not allocated already
        else if (INVALID_ENTITY(m_Vehicles[i].mID))
        {
            AllocVehicle(i, false, SQMOD_CREATE_IMPORT, NullObject());
        }
    }
}


// --------------------------------------------------------------------------------------------
Object & Core::AllocBlip(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Cannot allocate blip with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    BlipInst & inst = m_Blips[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CBlip(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a blip instance for: %d", id);
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
    // Let the script callbacks know about this entity
    EmitBlipCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocCheckpoint(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Cannot allocate checkpoint with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    CheckpointInst & inst = m_Checkpoints[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CCheckpoint(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a checkpoint instance for: %d", id);
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
    // Let the script callbacks know about this entity
    EmitCheckpointCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocForcefield(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_FORCEFIELD_POOL))
    {
        STHROWF("Cannot allocate forcefield with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    ForcefieldInst & inst = m_Forcefields[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CForcefield(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a forcefield instance for: %d", id);
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
    // Let the script callbacks know about this entity
    EmitForcefieldCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocKeybind(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Cannot allocate keybind with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    KeybindInst & inst = m_Keybinds[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CKeybind(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a keybind instance for: %d", id);
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
    // Let the script callbacks know about this entity
    EmitKeybindCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocObject(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Cannot allocate object with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    ObjectInst & inst = m_Objects[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CObject(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a object instance for: %d", id);
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
    // Let the script callbacks know about this entity
    EmitObjectCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocPickup(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Cannot allocate pickup with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    PickupInst & inst = m_Pickups[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CPickup(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a pickup instance for: %d", id);
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
    // Let the script callbacks know about this entity
    EmitPickupCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocSprite(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_SPRITE_POOL))
    {
        STHROWF("Cannot allocate sprite with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    SpriteInst & inst = m_Sprites[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CSprite(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a sprite instance for: %d", id);
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
    // Let the script callbacks know about this entity
    EmitSpriteCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocTextdraw(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_TEXTDRAW_POOL))
    {
        STHROWF("Cannot allocate textdraw with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    TextdrawInst & inst = m_Textdraws[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CTextdraw(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a textdraw instance for: %d", id);
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
    // Let the script callbacks know about this entity
    EmitTextdrawCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::AllocVehicle(Int32 id, bool owned, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Cannot allocate vehicle with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    VehicleInst & inst = m_Vehicles[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return inst.mObj; // Return the existing instance
    }
    // Instantiate the entity manager
    inst.mInst = new CVehicle(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a vehicle instance for: %d", id);
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
    // Retrieve the associated tracking instance
    VehicleTrack & track = m_VehicleTrack[id];
    // Initialize the position
    _Func->GetVehiclePos(id, &track.mPosition.x, &track.mPosition.y, &track.mPosition.z);
    // Initialize the remaining components
    track.mHealth = _Func->GetVehicleHealth(id);
    // Let the script callbacks know about this entity
    EmitVehicleCreated(id, header, payload);
    // Return the script object
    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
void Core::DeallocBlip(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Cannot deallocate blip with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    BlipInst & inst = m_Blips[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const EntLockGuard elg(inst.mFlags);
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitBlipDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DestroyCoordBlip(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocCheckpoint(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Cannot deallocate checkpoint with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    CheckpointInst & inst = m_Checkpoints[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const EntLockGuard elg(inst.mFlags);
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitCheckpointDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DeleteCheckpoint(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocForcefield(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_FORCEFIELD_POOL))
    {
        STHROWF("Cannot deallocate forcefield with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    ForcefieldInst & inst = m_Forcefields[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const EntLockGuard elg(inst.mFlags);
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitForcefieldDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DeleteSphere(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocKeybind(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Cannot deallocate keybind with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    KeybindInst & inst = m_Keybinds[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const EntLockGuard elg(inst.mFlags);
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitBlipDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->RemoveKeyBind(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocObject(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Cannot deallocate object with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    ObjectInst & inst = m_Objects[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const EntLockGuard elg(inst.mFlags);
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitObjectDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DeleteObject(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocPickup(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Cannot deallocate pickup with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    PickupInst & inst = m_Pickups[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const EntLockGuard elg(inst.mFlags);
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitPickupDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DeletePickup(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocSprite(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_SPRITE_POOL))
    {
        STHROWF("Cannot deallocate sprite with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    SpriteInst & inst = m_Sprites[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const EntLockGuard elg(inst.mFlags);
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitSpriteDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DestroySprite(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocTextdraw(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_TEXTDRAW_POOL))
    {
        STHROWF("Cannot deallocate textdraw with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    TextdrawInst & inst = m_Textdraws[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const EntLockGuard elg(inst.mFlags);
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitTextdrawDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DestroyTextdraw(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
void Core::DeallocVehicle(Int32 id, bool destroy, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Cannot deallocate vehicle with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    VehicleInst & inst = m_Vehicles[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const EntLockGuard elg(inst.mFlags);
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitVehicleDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Should we delete this entity from the server as well?
    if (destroy || (inst.mFlags & ENF_OWNED))
    {
        _Func->DeleteVehicle(inst.mID);
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// --------------------------------------------------------------------------------------------
Object & Core::NewBlip(Int32 index, Int32 world, Float32 x, Float32 y, Float32 z,
                            Int32 scale, Uint32 color, Int32 sprid,
                            Int32 header, Object & payload)
{
    // Request the server to create this entity
    Int32 id = _Func->CreateCoordBlip(index, world, x, y, z, scale, color, sprid);
    // Validate the identifier returned by the server
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Server returned invalid blip: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocBlip(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewCheckpoint(Int32 player, Int32 world, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius,
                            Int32 header, Object & payload)
{
    // Is the specified player instance even valid?
    if (INVALID_ENTITY(m_Players.at(player).mID))
    {
        STHROWF("Invalid player reference: %d", player);
    }
    // Request the server to create this entity
    Int32 id = _Func->CreateCheckpoint(player, world, x, y, z, r, g, b, a, radius);
    // Validate the identifier returned by the server
    if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Server returned invalid checkpoint: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocCheckpoint(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewForcefield(Int32 player, Int32 world, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Float32 radius,
                            Int32 header, Object & payload)
{
    // Is the specified player instance even valid?
    if (INVALID_ENTITY(m_Players.at(player).mID))
    {
        STHROWF("Invalid player reference: %d", player);
    }
    // Request the server to create this entity
    Int32 id = _Func->CreateSphere(player, world, x, y, z, r, g, b, radius);
    // Validate the identifier returned by the server
    if (INVALID_ENTITYEX(id, SQMOD_FORCEFIELD_POOL))
    {
        STHROWF("Server returned invalid forcefield: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocForcefield(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewKeybind(Int32 slot, bool release,
                            Int32 primary, Int32 secondary, Int32 alternative,
                            Int32 header, Object & payload)
{
    // Request the server to create this entity
    Int32 id = _Func->RegisterKeyBind(slot, release, primary, secondary, alternative);
    // Validate the identifier returned by the server
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Server returned invalid keybind: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocKeybind(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewObject(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                            Int32 alpha, Int32 header, Object & payload)
{
    // Request the server to create this entity
    Int32 id = _Func->CreateObject(model, world, x, y, z, alpha);
    // Validate the identifier returned by the server
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Server returned invalid object: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocObject(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewPickup(Int32 model, Int32 world, Int32 quantity,
                            Float32 x, Float32 y, Float32 z, Int32 alpha, bool automatic,
                            Int32 header, Object & payload)
{
    // Request the server to create this entity
    Int32 id = _Func->CreatePickup(model, world, quantity, x, y, z, alpha, automatic);
    // Validate the identifier returned by the server
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Server returned invalid pickup: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocPickup(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewSprite(Int32 index, CSStr file, Int32 xp, Int32 yp,
                            Int32 xr, Int32 yr, Float32 angle, Int32 alpha, bool rel,
                            Int32 header, Object & payload)
{
    // Request the server to create this entity
    Int32 id = _Func->CreateSprite(index, file, xp, yp, xr, yr, angle, alpha, rel);
    // Validate the identifier returned by the server
    if (INVALID_ENTITYEX(id, SQMOD_SPRITE_POOL))
    {
        STHROWF("Server returned invalid sprite: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocSprite(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewTextdraw(Int32 index, CSStr text, Int32 xp, Int32 yp,
                            Uint32 color, bool rel, Int32 header, Object & payload)
{
    // Request the server to create this entity
    Int32 id = _Func->CreateTextdraw(index, text, xp, yp, color, rel);
    // Validate the identifier returned by the server
    if (INVALID_ENTITYEX(id, SQMOD_TEXTDRAW_POOL))
    {
        STHROWF("Server returned invalid textdraw: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocTextdraw(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewVehicle(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                            Float32 angle, Int32 primary, Int32 secondary,
                            Int32 header, Object & payload)
{
    // Request the server to create this entity
    Int32 id = _Func->CreateVehicle(model, world, x, y, z, angle, primary, secondary);
    // Validate the identifier returned by the server
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Server returned invalid vehicle: %d", id);
    }
    // Attempt to allocate this entity and return the result
    return AllocVehicle(id, true, header, payload);
}

// --------------------------------------------------------------------------------------------
bool Core::DelBlip(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocBlip(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

bool Core::DelCheckpoint(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocCheckpoint(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

bool Core::DelForcefield(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocForcefield(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

bool Core::DelKeybind(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocKeybind(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

bool Core::DelObject(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocObject(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

bool Core::DelPickup(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocPickup(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

bool Core::DelSprite(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocSprite(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

bool Core::DelTextdraw(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocTextdraw(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

bool Core::DelVehicle(Int32 id, Int32 header, Object & payload)
{
    // Attempt to destroy and deallocate the specified entity instance
    DeallocVehicle(id, true, header, payload);
    // The entity could be destroyed
    return true;
}

// --------------------------------------------------------------------------------------------
void Core::ConnectPlayer(Int32 id, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Cannot allocate player with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    PlayerInst & inst = m_Players[id];
    // Make sure that the instance isn't already allocated
    if (VALID_ENTITY(inst.mID))
    {
        return; // Nothing to allocate!
    }
    // Instantiate the entity manager
    inst.mInst = new CPlayer(id);
    // Create the script object
    inst.mObj = Object(inst.mInst, m_VM);
    // Make sure that both the instance and script object could be created
    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a player instance for: %d", id);
    }
    // Assign the specified entity identifier
    inst.mID = id;
    // Retrieve the associated tracking instance
    PlayerTrack & track = m_PlayerTrack[id];
    // Initialize the position
    _Func->GetPlayerPos(id, &track.mPosition.x, &track.mPosition.y, &track.mPosition.z);
    // Initialize the remaining components
    track.mHealth = _Func->GetPlayerHealth(id);
    track.mArmour = _Func->GetPlayerArmour(id);
    track.mWeapon = _Func->GetPlayerWeapon(id);
    // Let the script callbacks know about this entity
    EmitPlayerCreated(id, header, payload);
}

void Core::DisconnectPlayer(Int32 id, Int32 header, Object & payload)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Cannot deallocate player with invalid identifier: %d", id);
    }
    // Retrieve the specified entity instance
    PlayerInst & inst = m_Players[id];
    // Make sure that the instance is even allocated
    if (INVALID_ENTITY(inst.mID) || (inst.mFlags & ENF_LOCKED))
    {
        return; // Nothing to deallocate!
    }
    // Prevent further attempts to delete this entity
    const EntLockGuard elg(inst.mFlags);
    // Let the script callbacks know this entity should no longer be used
    try
    {
        EmitPlayerDestroyed(id, header, payload);
    }
    catch (...)
    {
        // The error was probably dealt with already
    }
    // Is there a manager instance associated with this entity?
    if (inst.mInst)
    {
        // Prevent further use of this entity
        inst.mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        inst.mInst->m_Data.Release();
    }
    // Reset the entity instance
    ResetInst(inst);
    // Release associated script callbacks
    ResetFunc(inst);
    // Prevent further use of the manager instance
    inst.mInst = nullptr;
    // Release the script object, if any
    inst.mObj.Release();
}

// ------------------------------------------------------------------------------------------------
void Core::BindEvent(Int32 id, Object & env, Function & func)
{
    // Obtain the function instance called for this event
    Function & event = GetEvent(id);
    // Is the specified callback function null?
    if (func.IsNull())
    {
        event.Release(); // Then release the current callback
    }
    // Assign the specified environment and function
    else
    {
        event = Function(env.GetVM(), env, func.GetFunc());
    }
}

// ------------------------------------------------------------------------------------------------
void Core::EmitBlipCreated(Int32 blip, Int32 header, Object & payload)
{
    Emit(mOnBlipCreated, m_Blips.at(blip).mObj, header, payload);
}

void Core::EmitCheckpointCreated(Int32 checkpoint, Int32 header, Object & payload)
{
    Emit(mOnCheckpointCreated, m_Checkpoints.at(checkpoint).mObj, header, payload);
}

void Core::EmitForcefieldCreated(Int32 forcefield, Int32 header, Object & payload)
{
    Emit(mOnForcefieldCreated, m_Forcefields.at(forcefield).mObj, header, payload);
}

void Core::EmitKeybindCreated(Int32 keybind, Int32 header, Object & payload)
{
    Emit(mOnKeybindCreated, m_Keybinds.at(keybind).mObj, header, payload);
}

void Core::EmitObjectCreated(Int32 object, Int32 header, Object & payload)
{
    Emit(mOnObjectCreated, m_Objects.at(object).mObj, header, payload);
}

void Core::EmitPickupCreated(Int32 pickup, Int32 header, Object & payload)
{
    Emit(mOnPickupCreated, m_Pickups.at(pickup).mObj, header, payload);
}

void Core::EmitPlayerCreated(Int32 player, Int32 header, Object & payload)
{
    Emit(mOnPlayerCreated, m_Players.at(player).mObj, header, payload);
}

void Core::EmitSpriteCreated(Int32 sprite, Int32 header, Object & payload)
{
    Emit(mOnSpriteCreated, m_Sprites.at(sprite).mObj, header, payload);
}

void Core::EmitTextdrawCreated(Int32 textdraw, Int32 header, Object & payload)
{
    Emit(mOnTextdrawCreated, m_Textdraws.at(textdraw).mObj, header, payload);
}

void Core::EmitVehicleCreated(Int32 vehicle, Int32 header, Object & payload)
{
    Emit(mOnVehicleCreated, m_Vehicles.at(vehicle).mObj, header, payload);
}

void Core::EmitBlipDestroyed(Int32 blip, Int32 header, Object & payload)
{
    BlipInst & _blip = m_Blips.at(blip);
    Emit(_blip.mOnDestroyed, header, payload);
    Emit(mOnBlipDestroyed, _blip.mObj, header, payload);
}

void Core::EmitCheckpointDestroyed(Int32 checkpoint, Int32 header, Object & payload)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint);
    Emit(_checkpoint.mOnDestroyed, header, payload);
    Emit(mOnCheckpointDestroyed, _checkpoint.mObj, header, payload);
}

void Core::EmitForcefieldDestroyed(Int32 forcefield, Int32 header, Object & payload)
{
    ForcefieldInst & _forcefield = m_Forcefields.at(forcefield);
    Emit(_forcefield.mOnDestroyed, header, payload);
    Emit(mOnForcefieldDestroyed, _forcefield.mObj, header, payload);
}

void Core::EmitKeybindDestroyed(Int32 keybind, Int32 header, Object & payload)
{
    KeybindInst & _keybind = m_Keybinds.at(keybind);
    Emit(_keybind.mOnDestroyed, header, payload);
    Emit(mOnKeybindDestroyed, _keybind.mObj, header, payload);
}

void Core::EmitObjectDestroyed(Int32 object, Int32 header, Object & payload)
{
    ObjectInst & _object = m_Objects.at(object);
    Emit(_object.mOnDestroyed, header, payload);
    Emit(mOnObjectDestroyed, _object.mObj, header, payload);
}

void Core::EmitPickupDestroyed(Int32 pickup, Int32 header, Object & payload)
{
    PickupInst & _pickup = m_Pickups.at(pickup);
    Emit(_pickup.mOnDestroyed, header, payload);
    Emit(mOnPickupDestroyed, _pickup.mObj, header, payload);
}

void Core::EmitPlayerDestroyed(Int32 player, Int32 header, Object & payload)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnDestroyed, header, payload);
    Emit(mOnPlayerDestroyed, _player.mObj, header, payload);
}

void Core::EmitSpriteDestroyed(Int32 sprite, Int32 header, Object & payload)
{
    SpriteInst & _sprite = m_Sprites.at(sprite);
    Emit(_sprite.mOnDestroyed, header, payload);
    Emit(mOnSpriteDestroyed, _sprite.mObj, header, payload);
}

void Core::EmitTextdrawDestroyed(Int32 textdraw, Int32 header, Object & payload)
{
    TextdrawInst & _textdraw = m_Textdraws.at(textdraw);
    Emit(_textdraw.mOnDestroyed, header, payload);
    Emit(mOnTextdrawDestroyed, _textdraw.mObj, header, payload);
}

void Core::EmitVehicleDestroyed(Int32 vehicle, Int32 header, Object & payload)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    Emit(_vehicle.mOnDestroyed, header, payload);
    Emit(mOnVehicleDestroyed, _vehicle.mObj, header, payload);
}

void Core::EmitBlipCustom(Int32 blip, Int32 header, Object & payload)
{
    BlipInst & _blip = m_Blips.at(blip);
    Emit(_blip.mOnCustom, header, payload);
    Emit(mOnBlipCustom, _blip.mObj, header, payload);
}

void Core::EmitCheckpointCustom(Int32 checkpoint, Int32 header, Object & payload)
{
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint);
    Emit(_checkpoint.mOnCustom, header, payload);
    Emit(mOnCheckpointCustom, _checkpoint.mObj, header, payload);
}

void Core::EmitForcefieldCustom(Int32 forcefield, Int32 header, Object & payload)
{
    ForcefieldInst & _forcefield = m_Forcefields.at(forcefield);
    Emit(_forcefield.mOnCustom, header, payload);
    Emit(mOnForcefieldCustom, _forcefield.mObj, header, payload);
}

void Core::EmitKeybindCustom(Int32 keybind, Int32 header, Object & payload)
{
    KeybindInst & _keybind = m_Keybinds.at(keybind);
    Emit(_keybind.mOnCustom, header, payload);
    Emit(mOnKeybindCustom, _keybind.mObj, header, payload);
}

void Core::EmitObjectCustom(Int32 object, Int32 header, Object & payload)
{
    ObjectInst & _object = m_Objects.at(object);
    Emit(_object.mOnCustom, header, payload);
    Emit(mOnObjectCustom, _object.mObj, header, payload);
}

void Core::EmitPickupCustom(Int32 pickup, Int32 header, Object & payload)
{
    PickupInst & _pickup = m_Pickups.at(pickup);
    Emit(_pickup.mOnCustom, header, payload);
    Emit(mOnPickupCustom, _pickup.mObj, header, payload);
}

void Core::EmitPlayerCustom(Int32 player, Int32 header, Object & payload)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnCustom, header, payload);
    Emit(mOnPlayerCustom, _player.mObj, header, payload);
}

void Core::EmitSpriteCustom(Int32 sprite, Int32 header, Object & payload)
{
    SpriteInst & _sprite = m_Sprites.at(sprite);
    Emit(_sprite.mOnCustom, header, payload);
    Emit(mOnSpriteCustom, _sprite.mObj, header, payload);
}

void Core::EmitTextdrawCustom(Int32 textdraw, Int32 header, Object & payload)
{
    TextdrawInst & _textdraw = m_Textdraws.at(textdraw);
    Emit(_textdraw.mOnCustom, header, payload);
    Emit(mOnTextdrawCustom, _textdraw.mObj, header, payload);
}

void Core::EmitVehicleCustom(Int32 vehicle, Int32 header, Object & payload)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    Emit(_vehicle.mOnCustom, header, payload);
    Emit(mOnVehicleCustom, _vehicle.mObj, header, payload);
}

void Core::EmitPlayerAway(Int32 player, bool status)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnAway, status);
    Emit(mOnPlayerAway, _player.mObj, status);
}

void Core::EmitPlayerGameKeys(Int32 player, Int32 previous, Int32 current)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnGameKeys, previous, current);
    Emit(mOnPlayerGameKeys, _player.mObj, previous, current);
}

void Core::EmitPlayerRename(Int32 player, CCStr previous, CCStr current)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnRename, previous, current);
    Emit(mOnPlayerRename, _player.mObj, previous, current);
}

void Core::EmitPlayerRequestClass(Int32 player, Int32 offset)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnRequestClass, offset);
    Emit(mOnPlayerRequestClass, _player.mObj, offset);
}

void Core::EmitPlayerRequestSpawn(Int32 player)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnRequestSpawn);
    Emit(mOnPlayerRequestSpawn, _player.mObj);
}

void Core::EmitPlayerSpawn(Int32 player)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnSpawn);
    Emit(mOnPlayerSpawn, _player.mObj);
}

void Core::EmitPlayerStartTyping(Int32 player)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnStartTyping);
    Emit(mOnPlayerStartTyping, _player.mObj);
}

void Core::EmitPlayerStopTyping(Int32 player)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnStopTyping);
    Emit(mOnPlayerStopTyping, _player.mObj);
}

void Core::EmitPlayerChat(Int32 player, CCStr message)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnChat, message);
    Emit(mOnPlayerChat, _player.mObj, message);
}

void Core::EmitPlayerCommand(Int32 player, CCStr command)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnCommand, command);
    Emit(mOnPlayerCommand, _player.mObj, command);
    // Send it to the command manager
    RunCommand(player, command);
}

void Core::EmitPlayerMessage(Int32 player, Int32 receiver, CCStr message)
{
    PlayerInst & _player = m_Players.at(player);
    PlayerInst & _receiver = m_Players.at(receiver);
    Emit(_player.mOnMessage,_receiver.mObj,  message);
    Emit(mOnPlayerMessage, _player.mObj, _receiver.mObj, message);
}

void Core::EmitPlayerHealth(Int32 player, Float32 previous, Float32 current)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnHealth, previous, current);
    Emit(mOnPlayerHealth, _player.mObj, previous, current);
}

void Core::EmitPlayerArmour(Int32 player, Float32 previous, Float32 current)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnArmour, previous, current);
    Emit(mOnPlayerArmour, _player.mObj, previous, current);
}

void Core::EmitPlayerWeapon(Int32 player, Int32 previous, Int32 current)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnWeapon, previous, current);
    Emit(mOnPlayerWeapon, _player.mObj, previous, current);
}

void Core::EmitPlayerMove(Int32 player, const Vector3 & previous, const Vector3 & current)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnMove, previous, current);
    Emit(mOnPlayerMove, _player.mObj, previous, current);
}

void Core::EmitPlayerWasted(Int32 player, Int32 reason)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnWasted, reason);
    Emit(mOnPlayerWasted, _player.mObj, reason);
}

void Core::EmitPlayerKilled(Int32 player, Int32 killer, Int32 reason, Int32 body_part)
{
    PlayerInst & _player = m_Players.at(player);
    PlayerInst & _killer = m_Players.at(killer);
    Emit(_player.mOnKilled, _killer.mObj, reason, body_part);
    Emit(mOnPlayerKilled, _player.mObj, _killer.mObj, reason, body_part);
}

void Core::EmitPlayerTeamKill(Int32 player, Int32 killer, Int32 reason, Int32 body_part)
{
    PlayerInst & _player = m_Players.at(player);
    PlayerInst & _killer = m_Players.at(killer);
    Emit(_player.mOnTeamKill, _killer.mObj, reason, body_part);
    Emit(mOnPlayerTeamKill, _player.mObj, _killer.mObj, reason, body_part);
}

void Core::EmitPlayerSpectate(Int32 player, Int32 target)
{
    PlayerInst & _player = m_Players.at(player);
    PlayerInst & _target = m_Players.at(target);
    Emit(_player.mOnSpectate, _target.mObj);
    Emit(mOnPlayerSpectate, _player.mObj, _target.mObj);
}

void Core::EmitPlayerCrashreport(Int32 player, CCStr report)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnCrashreport, report);
    Emit(mOnPlayerCrashreport, _player.mObj, report);
}

void Core::EmitPlayerBurning(Int32 player, bool state)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnBurning, state);
    Emit(mOnPlayerBurning, _player.mObj, state);
}

void Core::EmitPlayerCrouching(Int32 player, bool state)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnCrouching, state);
    Emit(mOnPlayerCrouching, _player.mObj, state);
}

void Core::EmitPlayerState(Int32 player, Int32 previous, Int32 current)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnState, previous, current);
    Emit(mOnPlayerState, _player.mObj, previous, current);
}

void Core::EmitPlayerAction(Int32 player, Int32 previous, Int32 current)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnAction, previous, current);
    Emit(mOnPlayerAction, _player.mObj, previous, current);
}

void Core::EmitStateNone(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnStateNone, previous);
    Emit(mOnStateNone, _player.mObj, previous);
}

void Core::EmitStateNormal(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnStateNormal, previous);
    Emit(mOnStateNormal, _player.mObj, previous);
}

void Core::EmitStateShooting(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnStateShooting, previous);
    Emit(mOnStateShooting, _player.mObj, previous);
}

void Core::EmitStateDriver(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnStateDriver, previous);
    Emit(mOnStateDriver, _player.mObj, previous);
}

void Core::EmitStatePassenger(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnStatePassenger, previous);
    Emit(mOnStatePassenger, _player.mObj, previous);
}

void Core::EmitStateEnterDriver(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnStateEnterDriver, previous);
    Emit(mOnStateEnterDriver, _player.mObj, previous);
}

void Core::EmitStateEnterPassenger(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnStateEnterPassenger, previous);
    Emit(mOnStateEnterPassenger, _player.mObj, previous);
}

void Core::EmitStateExitVehicle(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnStateExitVehicle, previous);
    Emit(mOnStateExitVehicle, _player.mObj, previous);
}

void Core::EmitStateUnspawned(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnStateUnspawned, previous);
    Emit(mOnStateUnspawned, _player.mObj, previous);
}

void Core::EmitActionNone(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionNone, previous);
    Emit(mOnActionNone, _player.mObj, previous);
}

void Core::EmitActionNormal(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionNormal, previous);
    Emit(mOnActionNormal, _player.mObj, previous);
}

void Core::EmitActionAiming(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionAiming, previous);
    Emit(mOnActionAiming, _player.mObj, previous);
}

void Core::EmitActionShooting(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionShooting, previous);
    Emit(mOnActionShooting, _player.mObj, previous);
}

void Core::EmitActionJumping(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionJumping, previous);
    Emit(mOnActionJumping, _player.mObj, previous);
}

void Core::EmitActionLieDown(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionLieDown, previous);
    Emit(mOnActionLieDown, _player.mObj, previous);
}

void Core::EmitActionGettingUp(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionGettingUp, previous);
    Emit(mOnActionGettingUp, _player.mObj, previous);
}

void Core::EmitActionJumpVehicle(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionJumpVehicle, previous);
    Emit(mOnActionJumpVehicle, _player.mObj, previous);
}

void Core::EmitActionDriving(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionDriving, previous);
    Emit(mOnActionDriving, _player.mObj, previous);
}

void Core::EmitActionDying(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionDying, previous);
    Emit(mOnActionDying, _player.mObj, previous);
}

void Core::EmitActionWasted(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionWasted, previous);
    Emit(mOnActionWasted, _player.mObj, previous);
}

void Core::EmitActionEmbarking(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionEmbarking, previous);
    Emit(mOnActionEmbarking, _player.mObj, previous);
}

void Core::EmitActionDisembarking(Int32 player, Int32 previous)
{
    PlayerInst & _player = m_Players.at(player);
    Emit(_player.mOnActionDisembarking, previous);
    Emit(mOnActionDisembarking, _player.mObj, previous);
}

void Core::EmitVehicleRespawn(Int32 vehicle)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    Emit(_vehicle.mOnRespawn);
    Emit(mOnVehicleRespawn, _vehicle.mObj);
}

void Core::EmitVehicleExplode(Int32 vehicle)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    Emit(_vehicle.mOnExplode);
    Emit(mOnVehicleExplode, _vehicle.mObj);
}

void Core::EmitVehicleHealth(Int32 vehicle, Float32 previous, Float32 current)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    Emit(_vehicle.mOnHealth, previous, current);
    Emit(mOnVehicleHealth, _vehicle.mObj, previous, current);
}

void Core::EmitVehicleMove(Int32 vehicle, const Vector3 & previous, const Vector3 & current)
{
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    Emit(_vehicle.mOnMove, previous, current);
    Emit(mOnVehicleMove, _vehicle.mObj, previous, current);
}

void Core::EmitPickupRespawn(Int32 pickup)
{
    PickupInst & _pickup = m_Pickups.at(pickup);
    Emit(_pickup.mOnRespawn);
    Emit(mOnPickupRespawn, _pickup.mObj);
}

void Core::EmitPlayerKeyPress(Int32 player, Int32 keybind)
{
    PlayerInst & _player = m_Players.at(player);
    KeybindInst & _keybind = m_Keybinds.at(keybind);
    Emit(_keybind.mOnKeyPress, _player.mObj);
    Emit(_player.mOnKeyPress, _keybind.mObj);
    Emit(mOnKeybindKeyPress, _player.mObj, _keybind.mObj);
}

void Core::EmitPlayerKeyRelease(Int32 player, Int32 keybind)
{
    PlayerInst & _player = m_Players.at(player);
    KeybindInst & _keybind = m_Keybinds.at(keybind);
    Emit(_keybind.mOnKeyRelease, _player.mObj);
    Emit(_player.mOnKeyRelease, _keybind.mObj);
    Emit(mOnKeybindKeyRelease, _player.mObj, _keybind.mObj);
}

void Core::EmitPlayerEmbarking(Int32 player, Int32 vehicle, Int32 slot)
{
    PlayerInst & _player = m_Players.at(player);
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    Emit(_vehicle.mOnEmbarking, _player.mObj, slot);
    Emit(_player.mOnEmbarking, _vehicle.mObj, slot);
    Emit(mOnVehicleEmbarking, _player.mObj, _vehicle.mObj, slot);
}

void Core::EmitPlayerEmbarked(Int32 player, Int32 vehicle, Int32 slot)
{
    PlayerInst & _player = m_Players.at(player);
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    Emit(_vehicle.mOnEmbarked, _player.mObj, slot);
    Emit(_player.mOnEmbarked, _vehicle.mObj, slot);
    Emit(mOnVehicleEmbarked, _player.mObj, _vehicle.mObj, slot);
}

void Core::EmitPlayerDisembark(Int32 player, Int32 vehicle)
{
    PlayerInst & _player = m_Players.at(player);
    VehicleInst & _vehicle = m_Vehicles.at(vehicle);
    Emit(_vehicle.mOnDisembark, _player.mObj);
    Emit(_player.mOnDisembark, _vehicle.mObj);
    Emit(mOnVehicleDisembark, _player.mObj, _vehicle.mObj);
}

void Core::EmitPickupClaimed(Int32 player, Int32 pickup)
{
    PlayerInst & _player = m_Players.at(player);
    PickupInst & _pickup = m_Pickups.at(pickup);
    Emit(_pickup.mOnClaimed, _player.mObj);
    Emit(_player.mOnPickupClaimed, _pickup.mObj);
    Emit(mOnPickupClaimed, _player.mObj, _pickup.mObj);
}

void Core::EmitPickupCollected(Int32 player, Int32 pickup)
{
    PlayerInst & _player = m_Players.at(player);
    PickupInst & _pickup = m_Pickups.at(pickup);
    Emit(_pickup.mOnCollected, _player.mObj);
    Emit(_player.mOnPickupCollected, _pickup.mObj);
    Emit(mOnPickupCollected, _player.mObj, _pickup.mObj);
}

void Core::EmitObjectShot(Int32 player, Int32 object, Int32 weapon)
{
    PlayerInst & _player = m_Players.at(player);
    ObjectInst & _object = m_Objects.at(object);
    Emit(_object.mOnShot, _player.mObj, weapon);
    Emit(_player.mOnObjectShot, _object.mObj, weapon);
    Emit(mOnObjectShot, _player.mObj, _object.mObj, weapon);
}

void Core::EmitObjectBump(Int32 player, Int32 object)
{
    PlayerInst & _player = m_Players.at(player);
    ObjectInst & _object = m_Objects.at(object);
    Emit(_object.mOnBump, _player.mObj);
    Emit(_player.mOnObjectBump, _object.mObj);
    Emit(mOnObjectBump, _player.mObj, _object.mObj);
}

void Core::EmitCheckpointEntered(Int32 player, Int32 checkpoint)
{
    PlayerInst & _player = m_Players.at(player);
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint);
    Emit(_checkpoint.mOnEntered, _player.mObj);
    Emit(_player.mOnCheckpointEntered, _checkpoint.mObj);
    Emit(mOnCheckpointEntered, _player.mObj, _checkpoint.mObj);
}

void Core::EmitCheckpointExited(Int32 player, Int32 checkpoint)
{
    PlayerInst & _player = m_Players.at(player);
    CheckpointInst & _checkpoint = m_Checkpoints.at(checkpoint);
    Emit(_checkpoint.mOnExited, _player.mObj);
    Emit(_player.mOnCheckpointExited, _checkpoint.mObj);
    Emit(mOnCheckpointExited, _player.mObj, _checkpoint.mObj);
}

void Core::EmitForcefieldEntered(Int32 player, Int32 forcefield)
{
    PlayerInst & _player = m_Players.at(player);
    ForcefieldInst & _forcefield = m_Forcefields.at(forcefield);
    Emit(_forcefield.mOnEntered, _player.mObj);
    Emit(_player.mOnForcefieldEntered, _forcefield.mObj);
    Emit(mOnForcefieldEntered, _player.mObj, _forcefield.mObj);
}

void Core::EmitForcefieldExited(Int32 player, Int32 forcefield)
{
    PlayerInst & _player = m_Players.at(player);
    ForcefieldInst & _forcefield = m_Forcefields.at(forcefield);
    Emit(_forcefield.mOnExited, _player.mObj);
    Emit(_player.mOnForcefieldExited, _forcefield.mObj);
    Emit(mOnForcefieldExited, _player.mObj, _forcefield.mObj);
}

void Core::EmitServerFrame(Float32 delta)
{
    Emit(mOnServerFrame, delta);
    // Update routines
    ProcessRoutine();
}

void Core::EmitServerStartup()
{
    Emit(mOnServerStartup);
}

void Core::EmitServerShutdown()
{
    Emit(mOnServerShutdown);
}

void Core::EmitInternalCommand(Int32 type, CCStr text)
{
    Emit(mOnInternalCommand, type, text);
}

void Core::EmitLoginAttempt(CCStr name, CCStr passwd, CCStr ip)
{
    Emit(mOnLoginAttempt, name, passwd, ip);
}

void Core::EmitCustomEvent(Int32 group, Int32 header, Object & payload)
{
    Emit(mOnCustomEvent, group, header, payload);
}

void Core::EmitWorldOption(Int32 option, Object & value)
{
    Emit(mOnWorldOption, option, value);
}

void Core::EmitWorldToggle(Int32 option, bool value)
{
    Emit(mOnWorldToggle, option, value);
}

void Core::EmitScriptReload(Int32 header, Object & payload)
{
    Emit(mOnScriptReload, header, payload);
}

void Core::EmitScriptLoaded()
{
    Emit(mOnScriptLoaded);
}

// ------------------------------------------------------------------------------------------------
void Core::EmitPlayerUpdate(Int32 player, Int32 /*type*/)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(player, SQMOD_PLAYER_POOL))
    {
        STHROWF("Cannot update player with invalid identifier: %d", player);
    }
    // We allocate a dummy vector upfront to retrieve the position
    Vector3 pos;
    // Retrieve the associated tracking instance
    PlayerTrack & inst = m_PlayerTrack[player];
    // Obtain the current position of this instance
    _Func->GetPlayerPos(player, &pos.x, &pos.y, &pos.z);
    // Did the position change since the last tracked value?
    if (pos != inst.mPosition)
    {
        // Trigger the event specific to this change
        EmitPlayerMove(player, inst.mPosition, pos);
        // Update the tracked value
        inst.mPosition = pos;
    }
    // Obtain the current health of this instance
    Float32 health = _Func->GetPlayerHealth(player);
    // Did the health change since the last tracked value?
    if (!EpsEq(health, inst.mHealth))
    {
        // Trigger the event specific to this change
        EmitPlayerHealth(player, inst.mHealth, health);
        // Update the tracked value
        inst.mHealth = health;
    }
    // Obtain the current armor of this instance
    Float32 armour = _Func->GetPlayerArmour(player);
    // Did the armor change since the last tracked value?
    if (!EpsEq(armour, inst.mArmour))
    {
        // Trigger the event specific to this change
        EmitPlayerArmour(player, inst.mArmour, armour);
        // Update the tracked value
        inst.mArmour = armour;
    }
    // Obtain the current weapon of this instance
    SQInteger wep = _Func->GetPlayerWeapon(player);
    // Did the weapon change since the last tracked value?
    if (wep != inst.mWeapon)
    {
        // Trigger the event specific to this change
        EmitPlayerWeapon(player, inst.mWeapon, wep);
        // Update the tracked value
        inst.mWeapon = wep;
    }
}

// ------------------------------------------------------------------------------------------------
void Core::EmitVehicleUpdate(Int32 vehicle, Int32 /*type*/)
{
    // Make sure that the specified entity identifier is valid
    if (INVALID_ENTITYEX(vehicle, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Cannot update vehicle with invalid identifier: %d", vehicle);
    }
    // We allocate a dummy vector upfront to retrieve the position
    Vector3 pos;
    // Retrieve the associated tracking instance
    VehicleTrack & inst = m_VehicleTrack[vehicle];
    // Obtain the current position of this instance
    _Func->GetVehiclePos(vehicle, &pos.x, &pos.y, &pos.z);
    // Did the position change since the last tracked value?
    if (pos != inst.mPosition)
    {
        // Trigger the event specific to this change
        EmitVehicleMove(vehicle, inst.mPosition, pos);
        // Update the tracked value
        inst.mPosition = pos;
    }
    // Obtain the current health of this instance
    Float32 health = _Func->GetVehicleHealth(vehicle);
    // Did the health change since the last tracked value?
    if (!EpsEq(health, inst.mHealth))
    {
        // Trigger the event specific to this change
        EmitVehicleHealth(vehicle, inst.mHealth, health);
        // Update the tracked value
        inst.mHealth = health;
    }
}

// ------------------------------------------------------------------------------------------------
void Core::EmitEntityPool(Int32 type, Int32 id, bool deleted)
{
    // See what type of change happened in the entity pool
    switch (type)
    {
        case SQMOD_ENTITY_POOL_VEHICLE:
            if (deleted && VALID_ENTITY(m_Vehicles[id].mID))
            {
                DeallocVehicle(id, false, SQMOD_DESTROY_POOL, NullObject());
            }
            else if (INVALID_ENTITY(m_Vehicles[id].mID))
            {
                AllocVehicle(id, false, SQMOD_CREATE_POOL, NullObject());
            }
        break;
        case SQMOD_ENTITY_POOL_OBJECT:
            if (deleted && VALID_ENTITY(m_Objects[id].mID))
            {
                DeallocObject(id, false, SQMOD_DESTROY_POOL, NullObject());
            }
            else if (INVALID_ENTITY(m_Objects[id].mID))
            {
                AllocObject(id, false, SQMOD_CREATE_POOL, NullObject());
            }
        break;
        case SQMOD_ENTITY_POOL_PICKUP:
            if (deleted && VALID_ENTITY(m_Pickups[id].mID))
            {
                DeallocPickup(id, false, SQMOD_DESTROY_POOL, NullObject());
            }
            else if (INVALID_ENTITY(m_Pickups[id].mID))
            {
                AllocPickup(id, false, SQMOD_CREATE_POOL, NullObject());
            }
        break;
        case SQMOD_ENTITY_POOL_RADIO:
            // @TODO Implement...
        break;
        case SQMOD_ENTITY_POOL_SPRITE:
            if (deleted && VALID_ENTITY(m_Sprites[id].mID))
            {
                DeallocPickup(id, false, SQMOD_DESTROY_POOL, NullObject());
            }
            else if (INVALID_ENTITY(m_Sprites[id].mID))
            {
                AllocPickup(id, false, SQMOD_CREATE_POOL, NullObject());
            }
        break;
        case SQMOD_ENTITY_POOL_TEXTDRAW:
            if (deleted && VALID_ENTITY(m_Textdraws[id].mID))
            {
                DeallocTextdraw(id, false, SQMOD_DESTROY_POOL, NullObject());
            }
            else if (INVALID_ENTITY(m_Textdraws[id].mID))
            {
                AllocTextdraw(id, false, SQMOD_CREATE_POOL, NullObject());
            }
        break;
        case SQMOD_ENTITY_POOL_BLIP:
            if (deleted && VALID_ENTITY(m_Blips[id].mID))
            {
                DeallocBlip(id, false, SQMOD_DESTROY_POOL, NullObject());
            }
            else if (INVALID_ENTITY(m_Blips[id].mID))
            {
                // Make sure that the specified entity identifier is valid
                if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
                {
                    STHROWF("Cannot allocate blip with invalid identifier: %d", id);
                }
                // Retrieve the specified entity instance
                BlipInst & inst = m_Blips[id];
                // Make sure that the instance isn't already allocated
                if (VALID_ENTITY(inst.mID))
                {
                    STHROWF("Cannot allocate blip that already exists: %d", id);
                }
                // Information about the blip entity
                Int32 world, scale, sprid;
                Uint32 color;
                Float32 x, y, z;
                // Get the blip information from the server
                _Func->GetCoordBlipInfo(id, &world, &x, &y, &z, &scale, &color, &sprid);
                // Assign the obtain information
                inst.mWorld = world;
                inst.mScale = scale;
                inst.mSprID = sprid;
                inst.mColor.SetRGBA(color);
                inst.mPosition.Set(x, y, z);
                // Now we can allocate the instance after we have all the information
                AllocBlip(id, false, SQMOD_CREATE_POOL, NullObject());
            }
        break;
        default:
            LogErr("Unknown change in the entity pool of type: %d", type);
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ResetInst(BlipInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
    inst.mWorld = -1;
    inst.mScale = -1;
    inst.mSprID = -1;
    inst.mPosition.Clear();
    inst.mColor.Clear();
}

void Core::ResetInst(CheckpointInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
}

void Core::ResetInst(ForcefieldInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
}

void Core::ResetInst(KeybindInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
    inst.mFirst = -1;
    inst.mSecond = -1;
    inst.mThird = -1;
    inst.mRelease = -1;
}

void Core::ResetInst(ObjectInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
}

void Core::ResetInst(PickupInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
}

void Core::ResetInst(PlayerInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
    inst.mAuthority = 0;
    for (unsigned n = 0; n < SQMOD_PLAYER_MSG_PREFIXES; ++n)
    {
        inst.mPrefixes[n].assign("");
    }
    inst.mMessageColor = 0x6599FFFF;
    inst.mAnnounceStyle = 1;
}

void Core::ResetInst(SpriteInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
    inst.mPath.clear();
}

void Core::ResetInst(TextdrawInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
    inst.mText.clear();
}

void Core::ResetInst(VehicleInst & inst)
{
    inst.mID = -1;
    inst.mFlags = ENF_DEFAULT;
}

// ------------------------------------------------------------------------------------------------
void Core::ResetFunc(BlipInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
}

void Core::ResetFunc(CheckpointInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnEntered.ReleaseGently();
    inst.mOnExited.ReleaseGently();
}

void Core::ResetFunc(ForcefieldInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnEntered.ReleaseGently();
    inst.mOnExited.ReleaseGently();
}

void Core::ResetFunc(KeybindInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnKeyPress.ReleaseGently();
    inst.mOnKeyRelease.ReleaseGently();
}

void Core::ResetFunc(ObjectInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnShot.ReleaseGently();
    inst.mOnBump.ReleaseGently();
}

void Core::ResetFunc(PickupInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnRespawn.ReleaseGently();
    inst.mOnClaimed.ReleaseGently();
    inst.mOnCollected.ReleaseGently();
}

void Core::ResetFunc(PlayerInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnAway.ReleaseGently();
    inst.mOnGameKeys.ReleaseGently();
    inst.mOnRename.ReleaseGently();
    inst.mOnRequestClass.ReleaseGently();
    inst.mOnRequestSpawn.ReleaseGently();
    inst.mOnSpawn.ReleaseGently();
    inst.mOnStartTyping.ReleaseGently();
    inst.mOnStopTyping.ReleaseGently();
    inst.mOnChat.ReleaseGently();
    inst.mOnCommand.ReleaseGently();
    inst.mOnMessage.ReleaseGently();
    inst.mOnHealth.ReleaseGently();
    inst.mOnArmour.ReleaseGently();
    inst.mOnWeapon.ReleaseGently();
    inst.mOnMove.ReleaseGently();
    inst.mOnWasted.ReleaseGently();
    inst.mOnKilled.ReleaseGently();
    inst.mOnTeamKill.ReleaseGently();
    inst.mOnSpectate.ReleaseGently();
    inst.mOnCrashreport.ReleaseGently();
    inst.mOnBurning.ReleaseGently();
    inst.mOnCrouching.ReleaseGently();
    inst.mOnState.ReleaseGently();
    inst.mOnAction.ReleaseGently();
    inst.mOnStateNone.ReleaseGently();
    inst.mOnStateNormal.ReleaseGently();
    inst.mOnStateShooting.ReleaseGently();
    inst.mOnStateDriver.ReleaseGently();
    inst.mOnStatePassenger.ReleaseGently();
    inst.mOnStateEnterDriver.ReleaseGently();
    inst.mOnStateEnterPassenger.ReleaseGently();
    inst.mOnStateExitVehicle.ReleaseGently();
    inst.mOnStateUnspawned.ReleaseGently();
    inst.mOnActionNone.ReleaseGently();
    inst.mOnActionNormal.ReleaseGently();
    inst.mOnActionAiming.ReleaseGently();
    inst.mOnActionShooting.ReleaseGently();
    inst.mOnActionJumping.ReleaseGently();
    inst.mOnActionLieDown.ReleaseGently();
    inst.mOnActionGettingUp.ReleaseGently();
    inst.mOnActionJumpVehicle.ReleaseGently();
    inst.mOnActionDriving.ReleaseGently();
    inst.mOnActionDying.ReleaseGently();
    inst.mOnActionWasted.ReleaseGently();
    inst.mOnActionEmbarking.ReleaseGently();
    inst.mOnActionDisembarking.ReleaseGently();
    inst.mOnKeyPress.ReleaseGently();
    inst.mOnKeyRelease.ReleaseGently();
    inst.mOnEmbarking.ReleaseGently();
    inst.mOnEmbarked.ReleaseGently();
    inst.mOnDisembark.ReleaseGently();
    inst.mOnPickupClaimed.ReleaseGently();
    inst.mOnPickupCollected.ReleaseGently();
    inst.mOnObjectShot.ReleaseGently();
    inst.mOnObjectBump.ReleaseGently();
    inst.mOnCheckpointEntered.ReleaseGently();
    inst.mOnCheckpointExited.ReleaseGently();
    inst.mOnForcefieldEntered.ReleaseGently();
    inst.mOnForcefieldExited.ReleaseGently();
}

void Core::ResetFunc(SpriteInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
}

void Core::ResetFunc(TextdrawInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
}

void Core::ResetFunc(VehicleInst & inst)
{
    inst.mOnDestroyed.ReleaseGently();
    inst.mOnCustom.ReleaseGently();
    inst.mOnRespawn.ReleaseGently();
    inst.mOnExplode.ReleaseGently();
    inst.mOnHealth.ReleaseGently();
    inst.mOnMove.ReleaseGently();
    inst.mOnEmbarking.ReleaseGently();
    inst.mOnEmbarked.ReleaseGently();
    inst.mOnDisembark.ReleaseGently();
}

void Core::ResetFunc()
{
    mOnBlipCreated.ReleaseGently();
    mOnCheckpointCreated.ReleaseGently();
    mOnForcefieldCreated.ReleaseGently();
    mOnKeybindCreated.ReleaseGently();
    mOnObjectCreated.ReleaseGently();
    mOnPickupCreated.ReleaseGently();
    mOnPlayerCreated.ReleaseGently();
    mOnSpriteCreated.ReleaseGently();
    mOnTextdrawCreated.ReleaseGently();
    mOnVehicleCreated.ReleaseGently();
    mOnBlipDestroyed.ReleaseGently();
    mOnCheckpointDestroyed.ReleaseGently();
    mOnForcefieldDestroyed.ReleaseGently();
    mOnKeybindDestroyed.ReleaseGently();
    mOnObjectDestroyed.ReleaseGently();
    mOnPickupDestroyed.ReleaseGently();
    mOnPlayerDestroyed.ReleaseGently();
    mOnSpriteDestroyed.ReleaseGently();
    mOnTextdrawDestroyed.ReleaseGently();
    mOnVehicleDestroyed.ReleaseGently();
    mOnBlipCustom.ReleaseGently();
    mOnCheckpointCustom.ReleaseGently();
    mOnForcefieldCustom.ReleaseGently();
    mOnKeybindCustom.ReleaseGently();
    mOnObjectCustom.ReleaseGently();
    mOnPickupCustom.ReleaseGently();
    mOnPlayerCustom.ReleaseGently();
    mOnSpriteCustom.ReleaseGently();
    mOnTextdrawCustom.ReleaseGently();
    mOnVehicleCustom.ReleaseGently();
    mOnPlayerAway.ReleaseGently();
    mOnPlayerGameKeys.ReleaseGently();
    mOnPlayerRename.ReleaseGently();
    mOnPlayerRequestClass.ReleaseGently();
    mOnPlayerRequestSpawn.ReleaseGently();
    mOnPlayerSpawn.ReleaseGently();
    mOnPlayerStartTyping.ReleaseGently();
    mOnPlayerStopTyping.ReleaseGently();
    mOnPlayerChat.ReleaseGently();
    mOnPlayerCommand.ReleaseGently();
    mOnPlayerMessage.ReleaseGently();
    mOnPlayerHealth.ReleaseGently();
    mOnPlayerArmour.ReleaseGently();
    mOnPlayerWeapon.ReleaseGently();
    mOnPlayerMove.ReleaseGently();
    mOnPlayerWasted.ReleaseGently();
    mOnPlayerKilled.ReleaseGently();
    mOnPlayerTeamKill.ReleaseGently();
    mOnPlayerSpectate.ReleaseGently();
    mOnPlayerCrashreport.ReleaseGently();
    mOnPlayerBurning.ReleaseGently();
    mOnPlayerCrouching.ReleaseGently();
    mOnPlayerState.ReleaseGently();
    mOnPlayerAction.ReleaseGently();
    mOnStateNone.ReleaseGently();
    mOnStateNormal.ReleaseGently();
    mOnStateShooting.ReleaseGently();
    mOnStateDriver.ReleaseGently();
    mOnStatePassenger.ReleaseGently();
    mOnStateEnterDriver.ReleaseGently();
    mOnStateEnterPassenger.ReleaseGently();
    mOnStateExitVehicle.ReleaseGently();
    mOnStateUnspawned.ReleaseGently();
    mOnActionNone.ReleaseGently();
    mOnActionNormal.ReleaseGently();
    mOnActionAiming.ReleaseGently();
    mOnActionShooting.ReleaseGently();
    mOnActionJumping.ReleaseGently();
    mOnActionLieDown.ReleaseGently();
    mOnActionGettingUp.ReleaseGently();
    mOnActionJumpVehicle.ReleaseGently();
    mOnActionDriving.ReleaseGently();
    mOnActionDying.ReleaseGently();
    mOnActionWasted.ReleaseGently();
    mOnActionEmbarking.ReleaseGently();
    mOnActionDisembarking.ReleaseGently();
    mOnVehicleRespawn.ReleaseGently();
    mOnVehicleExplode.ReleaseGently();
    mOnVehicleHealth.ReleaseGently();
    mOnVehicleMove.ReleaseGently();
    mOnPickupRespawn.ReleaseGently();
    mOnKeybindKeyPress.ReleaseGently();
    mOnKeybindKeyRelease.ReleaseGently();
    mOnVehicleEmbarking.ReleaseGently();
    mOnVehicleEmbarked.ReleaseGently();
    mOnVehicleDisembark.ReleaseGently();
    mOnPickupClaimed.ReleaseGently();
    mOnPickupCollected.ReleaseGently();
    mOnObjectShot.ReleaseGently();
    mOnObjectBump.ReleaseGently();
    mOnCheckpointEntered.ReleaseGently();
    mOnCheckpointExited.ReleaseGently();
    mOnForcefieldEntered.ReleaseGently();
    mOnForcefieldExited.ReleaseGently();
    mOnServerFrame.ReleaseGently();
    mOnServerStartup.ReleaseGently();
    mOnServerShutdown.ReleaseGently();
    mOnInternalCommand.ReleaseGently();
    mOnLoginAttempt.ReleaseGently();
    mOnCustomEvent.ReleaseGently();
    mOnWorldOption.ReleaseGently();
    mOnWorldToggle.ReleaseGently();
    mOnScriptReload.ReleaseGently();
    mOnScriptLoaded.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
Function & Core::GetEvent(Int32 evid)
{
    switch (evid)
    {
        case EVT_BLIPCREATED:           return mOnBlipCreated;
        case EVT_CHECKPOINTCREATED:     return mOnCheckpointCreated;
        case EVT_FORCEFIELDCREATED:     return mOnForcefieldCreated;
        case EVT_KEYBINDCREATED:        return mOnKeybindCreated;
        case EVT_OBJECTCREATED:         return mOnObjectCreated;
        case EVT_PICKUPCREATED:         return mOnPickupCreated;
        case EVT_PLAYERCREATED:         return mOnPlayerCreated;
        case EVT_SPRITECREATED:         return mOnSpriteCreated;
        case EVT_TEXTDRAWCREATED:       return mOnTextdrawCreated;
        case EVT_VEHICLECREATED:        return mOnVehicleCreated;
        case EVT_BLIPDESTROYED:         return mOnBlipDestroyed;
        case EVT_CHECKPOINTDESTROYED:   return mOnCheckpointDestroyed;
        case EVT_FORCEFIELDDESTROYED:   return mOnForcefieldDestroyed;
        case EVT_KEYBINDDESTROYED:      return mOnKeybindDestroyed;
        case EVT_OBJECTDESTROYED:       return mOnObjectDestroyed;
        case EVT_PICKUPDESTROYED:       return mOnPickupDestroyed;
        case EVT_PLAYERDESTROYED:       return mOnPlayerDestroyed;
        case EVT_SPRITEDESTROYED:       return mOnSpriteDestroyed;
        case EVT_TEXTDRAWDESTROYED:     return mOnTextdrawDestroyed;
        case EVT_VEHICLEDESTROYED:      return mOnVehicleDestroyed;
        case EVT_BLIPCUSTOM:            return mOnBlipCustom;
        case EVT_CHECKPOINTCUSTOM:      return mOnCheckpointCustom;
        case EVT_FORCEFIELDCUSTOM:      return mOnForcefieldCustom;
        case EVT_KEYBINDCUSTOM:         return mOnKeybindCustom;
        case EVT_OBJECTCUSTOM:          return mOnObjectCustom;
        case EVT_PICKUPCUSTOM:          return mOnPickupCustom;
        case EVT_PLAYERCUSTOM:          return mOnPlayerCustom;
        case EVT_SPRITECUSTOM:          return mOnSpriteCustom;
        case EVT_TEXTDRAWCUSTOM:        return mOnTextdrawCustom;
        case EVT_VEHICLECUSTOM:         return mOnVehicleCustom;
        case EVT_PLAYERAWAY:            return mOnPlayerAway;
        case EVT_PLAYERGAMEKEYS:        return mOnPlayerGameKeys;
        case EVT_PLAYERRENAME:          return mOnPlayerRename;
        case EVT_PLAYERREQUESTCLASS:    return mOnPlayerRequestClass;
        case EVT_PLAYERREQUESTSPAWN:    return mOnPlayerRequestSpawn;
        case EVT_PLAYERSPAWN:           return mOnPlayerSpawn;
        case EVT_PLAYERSTARTTYPING:     return mOnPlayerStartTyping;
        case EVT_PLAYERSTOPTYPING:      return mOnPlayerStopTyping;
        case EVT_PLAYERCHAT:            return mOnPlayerChat;
        case EVT_PLAYERCOMMAND:         return mOnPlayerCommand;
        case EVT_PLAYERMESSAGE:         return mOnPlayerMessage;
        case EVT_PLAYERHEALTH:          return mOnPlayerHealth;
        case EVT_PLAYERARMOUR:          return mOnPlayerArmour;
        case EVT_PLAYERWEAPON:          return mOnPlayerWeapon;
        case EVT_PLAYERMOVE:            return mOnPlayerMove;
        case EVT_PLAYERWASTED:          return mOnPlayerWasted;
        case EVT_PLAYERKILLED:          return mOnPlayerKilled;
        case EVT_PLAYERTEAMKILL:        return mOnPlayerTeamKill;
        case EVT_PLAYERSPECTATE:        return mOnPlayerSpectate;
        case EVT_PLAYERCRASHREPORT:     return mOnPlayerCrashreport;
        case EVT_PLAYERBURNING:         return mOnPlayerBurning;
        case EVT_PLAYERCROUCHING:       return mOnPlayerCrouching;
        case EVT_PLAYERSTATE:           return mOnPlayerState;
        case EVT_PLAYERACTION:          return mOnPlayerAction;
        case EVT_STATENONE:             return mOnStateNone;
        case EVT_STATENORMAL:           return mOnStateNormal;
        case EVT_STATESHOOTING:         return mOnStateShooting;
        case EVT_STATEDRIVER:           return mOnStateDriver;
        case EVT_STATEPASSENGER:        return mOnStatePassenger;
        case EVT_STATEENTERDRIVER:      return mOnStateEnterDriver;
        case EVT_STATEENTERPASSENGER:   return mOnStateEnterPassenger;
        case EVT_STATEEXITVEHICLE:      return mOnStateExitVehicle;
        case EVT_STATEUNSPAWNED:        return mOnStateUnspawned;
        case EVT_ACTIONNONE:            return mOnActionNone;
        case EVT_ACTIONNORMAL:          return mOnActionNormal;
        case EVT_ACTIONAIMING:          return mOnActionAiming;
        case EVT_ACTIONSHOOTING:        return mOnActionShooting;
        case EVT_ACTIONJUMPING:         return mOnActionJumping;
        case EVT_ACTIONLIEDOWN:         return mOnActionLieDown;
        case EVT_ACTIONGETTINGUP:       return mOnActionGettingUp;
        case EVT_ACTIONJUMPVEHICLE:     return mOnActionJumpVehicle;
        case EVT_ACTIONDRIVING:         return mOnActionDriving;
        case EVT_ACTIONDYING:           return mOnActionDying;
        case EVT_ACTIONWASTED:          return mOnActionWasted;
        case EVT_ACTIONEMBARKING:       return mOnActionEmbarking;
        case EVT_ACTIONDISEMBARKING:    return mOnActionDisembarking;
        case EVT_VEHICLERESPAWN:        return mOnVehicleRespawn;
        case EVT_VEHICLEEXPLODE:        return mOnVehicleExplode;
        case EVT_VEHICLEHEALTH:         return mOnVehicleHealth;
        case EVT_VEHICLEMOVE:           return mOnVehicleMove;
        case EVT_PICKUPRESPAWN:         return mOnPickupRespawn;
        case EVT_KEYBINDKEYPRESS:       return mOnKeybindKeyPress;
        case EVT_KEYBINDKEYRELEASE:     return mOnKeybindKeyRelease;
        case EVT_VEHICLEEMBARKING:      return mOnVehicleEmbarking;
        case EVT_VEHICLEEMBARKED:       return mOnVehicleEmbarked;
        case EVT_VEHICLEDISEMBARK:      return mOnVehicleDisembark;
        case EVT_PICKUPCLAIMED:         return mOnPickupClaimed;
        case EVT_PICKUPCOLLECTED:       return mOnPickupCollected;
        case EVT_OBJECTSHOT:            return mOnObjectShot;
        case EVT_OBJECTBUMP:            return mOnObjectBump;
        case EVT_CHECKPOINTENTERED:     return mOnCheckpointEntered;
        case EVT_CHECKPOINTEXITED:      return mOnCheckpointExited;
        case EVT_FORCEFIELDENTERED:     return mOnForcefieldEntered;
        case EVT_FORCEFIELDEXITED:      return mOnForcefieldExited;
        case EVT_SERVERFRAME:           return mOnServerFrame;
        case EVT_SERVERSTARTUP:         return mOnServerStartup;
        case EVT_SERVERSHUTDOWN:        return mOnServerShutdown;
        case EVT_INTERNALCOMMAND:       return mOnInternalCommand;
        case EVT_LOGINATTEMPT:          return mOnLoginAttempt;
        case EVT_CUSTOMEVENT:           return mOnCustomEvent;
        case EVT_WORLDOPTION:           return mOnWorldOption;
        case EVT_WORLDTOGGLE:           return mOnWorldToggle;
        case EVT_SCRIPTRELOAD:          return mOnScriptReload;
        case EVT_SCRIPTLOADED:          return mOnScriptLoaded;
        default:                        return NullFunction();
    }
}


// ------------------------------------------------------------------------------------------------
Function & Core::GetBlipEvent(Int32 id, Int32 evid)
{
    BlipInst & inst = m_Blips.at(id);

    switch (evid)
    {
        case EVT_BLIPDESTROYED:         return inst.mOnDestroyed;
        case EVT_BLIPCUSTOM:            return inst.mOnCustom;
        default:                        return NullFunction();
    }
}

Function & Core::GetCheckpointEvent(Int32 id, Int32 evid)
{
    CheckpointInst & inst = m_Checkpoints.at(id);

    switch (evid)
    {
        case EVT_CHECKPOINTDESTROYED:   return inst.mOnDestroyed;
        case EVT_CHECKPOINTCUSTOM:      return inst.mOnCustom;
        case EVT_CHECKPOINTENTERED:     return inst.mOnEntered;
        case EVT_CHECKPOINTEXITED:      return inst.mOnExited;
        default:                        return NullFunction();
    }
}

Function & Core::GetForcefieldEvent(Int32 id, Int32 evid)
{
    ForcefieldInst & inst = m_Forcefields.at(id);

    switch (evid)
    {
        case EVT_FORCEFIELDDESTROYED:   return inst.mOnDestroyed;
        case EVT_FORCEFIELDCUSTOM:      return inst.mOnCustom;
        case EVT_FORCEFIELDENTERED:     return inst.mOnEntered;
        case EVT_FORCEFIELDEXITED:      return inst.mOnExited;
        default:                        return NullFunction();
    }
}

Function & Core::GetKeybindEvent(Int32 id, Int32 evid)
{
    KeybindInst & inst = m_Keybinds.at(id);

    switch (evid)
    {
        case EVT_KEYBINDDESTROYED:      return inst.mOnDestroyed;
        case EVT_KEYBINDCUSTOM:         return inst.mOnCustom;
        case EVT_KEYBINDKEYPRESS:       return inst.mOnKeyPress;
        case EVT_KEYBINDKEYRELEASE:     return inst.mOnKeyRelease;
        default:                        return NullFunction();
    }
}

Function & Core::GetObjectEvent(Int32 id, Int32 evid)
{
    ObjectInst & inst = m_Objects.at(id);

    switch (evid)
    {
        case EVT_OBJECTDESTROYED:       return inst.mOnDestroyed;
        case EVT_OBJECTCUSTOM:          return inst.mOnCustom;
        case EVT_OBJECTSHOT:            return inst.mOnShot;
        case EVT_OBJECTBUMP:            return inst.mOnBump;
        default:                        return NullFunction();
    }
}

Function & Core::GetPickupEvent(Int32 id, Int32 evid)
{
    PickupInst & inst = m_Pickups.at(id);

    switch (evid)
    {
        case EVT_PICKUPDESTROYED:       return inst.mOnDestroyed;
        case EVT_PICKUPCUSTOM:          return inst.mOnCustom;
        case EVT_PICKUPRESPAWN:         return inst.mOnRespawn;
        case EVT_PICKUPCLAIMED:         return inst.mOnClaimed;
        case EVT_PICKUPCOLLECTED:       return inst.mOnCollected;
        default:                        return NullFunction();
    }
}

Function & Core::GetPlayerEvent(Int32 id, Int32 evid)
{
    PlayerInst & inst = m_Players.at(id);

    switch (evid)
    {
        case EVT_PLAYERDESTROYED:       return inst.mOnDestroyed;
        case EVT_PLAYERCUSTOM:          return inst.mOnCustom;
        case EVT_PLAYERAWAY:            return inst.mOnAway;
        case EVT_PLAYERGAMEKEYS:        return inst.mOnGameKeys;
        case EVT_PLAYERRENAME:          return inst.mOnRename;
        case EVT_PLAYERREQUESTCLASS:    return inst.mOnRequestClass;
        case EVT_PLAYERREQUESTSPAWN:    return inst.mOnRequestSpawn;
        case EVT_PLAYERSPAWN:           return inst.mOnSpawn;
        case EVT_PLAYERSTARTTYPING:     return inst.mOnStartTyping;
        case EVT_PLAYERSTOPTYPING:      return inst.mOnStopTyping;
        case EVT_PLAYERCHAT:            return inst.mOnChat;
        case EVT_PLAYERCOMMAND:         return inst.mOnCommand;
        case EVT_PLAYERMESSAGE:         return inst.mOnMessage;
        case EVT_PLAYERHEALTH:          return inst.mOnHealth;
        case EVT_PLAYERARMOUR:          return inst.mOnArmour;
        case EVT_PLAYERWEAPON:          return inst.mOnWeapon;
        case EVT_PLAYERMOVE:            return inst.mOnMove;
        case EVT_PLAYERWASTED:          return inst.mOnWasted;
        case EVT_PLAYERKILLED:          return inst.mOnKilled;
        case EVT_PLAYERTEAMKILL:        return inst.mOnTeamKill;
        case EVT_PLAYERSPECTATE:        return inst.mOnSpectate;
        case EVT_PLAYERCRASHREPORT:     return inst.mOnCrashreport;
        case EVT_PLAYERBURNING:         return inst.mOnBurning;
        case EVT_PLAYERCROUCHING:       return inst.mOnCrouching;
        case EVT_PLAYERSTATE:           return inst.mOnState;
        case EVT_PLAYERACTION:          return inst.mOnAction;
        case EVT_STATENONE:             return inst.mOnStateNone;
        case EVT_STATENORMAL:           return inst.mOnStateNormal;
        case EVT_STATESHOOTING:         return inst.mOnStateShooting;
        case EVT_STATEDRIVER:           return inst.mOnStateDriver;
        case EVT_STATEPASSENGER:        return inst.mOnStatePassenger;
        case EVT_STATEENTERDRIVER:      return inst.mOnStateEnterDriver;
        case EVT_STATEENTERPASSENGER:   return inst.mOnStateEnterPassenger;
        case EVT_STATEEXITVEHICLE:      return inst.mOnStateExitVehicle;
        case EVT_STATEUNSPAWNED:        return inst.mOnStateUnspawned;
        case EVT_ACTIONNONE:            return inst.mOnActionNone;
        case EVT_ACTIONNORMAL:          return inst.mOnActionNormal;
        case EVT_ACTIONAIMING:          return inst.mOnActionAiming;
        case EVT_ACTIONSHOOTING:        return inst.mOnActionShooting;
        case EVT_ACTIONJUMPING:         return inst.mOnActionJumping;
        case EVT_ACTIONLIEDOWN:         return inst.mOnActionLieDown;
        case EVT_ACTIONGETTINGUP:       return inst.mOnActionGettingUp;
        case EVT_ACTIONJUMPVEHICLE:     return inst.mOnActionJumpVehicle;
        case EVT_ACTIONDRIVING:         return inst.mOnActionDriving;
        case EVT_ACTIONDYING:           return inst.mOnActionDying;
        case EVT_ACTIONWASTED:          return inst.mOnActionWasted;
        case EVT_ACTIONEMBARKING:       return inst.mOnActionEmbarking;
        case EVT_ACTIONDISEMBARKING:    return inst.mOnActionDisembarking;
        case EVT_KEYBINDKEYPRESS:       return inst.mOnKeyPress;
        case EVT_KEYBINDKEYRELEASE:     return inst.mOnKeyRelease;
        case EVT_VEHICLEEMBARKING:      return inst.mOnEmbarking;
        case EVT_VEHICLEEMBARKED:       return inst.mOnEmbarked;
        case EVT_VEHICLEDISEMBARK:      return inst.mOnDisembark;
        case EVT_PICKUPCLAIMED:         return inst.mOnPickupClaimed;
        case EVT_PICKUPCOLLECTED:       return inst.mOnPickupCollected;
        case EVT_OBJECTSHOT:            return inst.mOnObjectShot;
        case EVT_OBJECTBUMP:            return inst.mOnObjectBump;
        case EVT_CHECKPOINTENTERED:     return inst.mOnCheckpointEntered;
        case EVT_CHECKPOINTEXITED:      return inst.mOnCheckpointExited;
        case EVT_FORCEFIELDENTERED:     return inst.mOnForcefieldEntered;
        case EVT_FORCEFIELDEXITED:      return inst.mOnForcefieldExited;
        default:                        return NullFunction();
    }
}

Function & Core::GetSpriteEvent(Int32 id, Int32 evid)
{
    SpriteInst & inst = m_Sprites.at(id);

    switch (evid)
    {
        case EVT_SPRITEDESTROYED:       return inst.mOnDestroyed;
        case EVT_SPRITECUSTOM:          return inst.mOnCustom;
        default:                        return NullFunction();
    }
}

Function & Core::GetTextdrawEvent(Int32 id, Int32 evid)
{
    TextdrawInst & inst = m_Textdraws.at(id);

    switch (evid)
    {
        case EVT_TEXTDRAWDESTROYED:     return inst.mOnDestroyed;
        case EVT_TEXTDRAWCUSTOM:        return inst.mOnCustom;
        default:                        return NullFunction();
    }
}

Function & Core::GetVehicleEvent(Int32 id, Int32 evid)
{
    VehicleInst & inst = m_Vehicles.at(id);

    switch (evid)
    {
        case EVT_VEHICLEDESTROYED:      return inst.mOnDestroyed;
        case EVT_VEHICLECUSTOM:         return inst.mOnCustom;
        case EVT_VEHICLERESPAWN:        return inst.mOnRespawn;
        case EVT_VEHICLEEXPLODE:        return inst.mOnExplode;
        case EVT_VEHICLEHEALTH:         return inst.mOnHealth;
        case EVT_VEHICLEMOVE:           return inst.mOnMove;
        case EVT_VEHICLEEMBARKING:      return inst.mOnEmbarking;
        case EVT_VEHICLEEMBARKED:       return inst.mOnEmbarked;
        case EVT_VEHICLEDISEMBARK:      return inst.mOnDisembark;
        default:                        return NullFunction();
    }
}

// ------------------------------------------------------------------------------------------------
Core::BlipInst::~BlipInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        _Core->EmitBlipDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
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
    _Core->ResetFunc(*this);
}

Core::CheckpointInst::~CheckpointInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        _Core->EmitCheckpointDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
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
        _Func->DeleteCheckpoint(mID);
    }
    // Don't release the callbacks abruptly in destructor
    _Core->ResetFunc(*this);
}

Core::ForcefieldInst::~ForcefieldInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        _Core->EmitForcefieldDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
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
        _Func->DeleteSphere(mID);
    }
    // Don't release the callbacks abruptly in destructor
    _Core->ResetFunc(*this);
}

Core::KeybindInst::~KeybindInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        _Core->EmitKeybindDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
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
    _Core->ResetFunc(*this);
}

Core::ObjectInst::~ObjectInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        _Core->EmitObjectDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
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
    _Core->ResetFunc(*this);
}

Core::PickupInst::~PickupInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        _Core->EmitPickupDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
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
    _Core->ResetFunc(*this);
}

Core::PlayerInst::~PlayerInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        _Core->EmitPlayerDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
    }
    // Is there a manager instance associated with this entity?
    if (mInst)
    {
        // Prevent further use of this entity
        mInst->m_ID = -1;
        // Release user data to avoid dangling or circular references
        mInst->m_Data.Release();
    }
    // Don't release the callbacks abruptly in destructor
    _Core->ResetFunc(*this);
}

Core::SpriteInst::~SpriteInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        _Core->EmitSpriteDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
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
        _Func->DestroySprite(mID);
    }
    // Don't release the callbacks abruptly in destructor
    _Core->ResetFunc(*this);
}

Core::TextdrawInst::~TextdrawInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        _Core->EmitTextdrawDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
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
        _Func->DestroyTextdraw(mID);
    }
    // Don't release the callbacks abruptly in destructor
    _Core->ResetFunc(*this);
}

Core::VehicleInst::~VehicleInst()
{
    // Should we notify that this entity is being cleaned up?
    if (mID >= 0)
    {
        _Core->EmitVehicleDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
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
    _Core->ResetFunc(*this);
}

} // Namespace:: SqMod
