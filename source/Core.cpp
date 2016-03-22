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
extern bool RegisterAPI(HSQUIRRELVM vm);

// ------------------------------------------------------------------------------------------------
extern void ProcessRoutine();
extern void TerminateRoutine();

// ------------------------------------------------------------------------------------------------
extern Int32 RunCommand(Int32 invoker, CSStr command);
extern void TerminateCommand();

// ------------------------------------------------------------------------------------------------
Core * _Core = NULL;

// ------------------------------------------------------------------------------------------------
Core::Core()
    : m_State(0)
    , m_VM(NULL)
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
    /* ... */
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
    m_Blips.clear();
    m_Checkpoints.clear();
    m_Keybinds.clear();
    m_Objects.clear();
    m_Pickups.clear();
    m_Players.clear();
    m_Forcefields.clear();
    m_Sprites.clear();
    m_Textdraws.clear();
    m_Vehicles.clear();
    m_Scripts.clear();
    // Release all resources from routines
    TerminateRoutine();
    // Release all resources from command manager
    TerminateCommand();
    // Is there a VM to close?
    if (m_VM)
    {
        // Release all script callbacks
        ResetFunc();
        // Release the script instances
        m_Scripts.clear();
        // Assertions during close may cause double delete!
        HSQUIRRELVM sq_vm = m_VM;
        m_VM = NULL;
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
void Core::PrintFunc(HSQUIRRELVM vm, CSStr msg, ...)
{
    SQMOD_UNUSED_VAR(vm);

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
        return 0;

    CSStr err_msg = NULL;

    if (SQ_SUCCEEDED(sq_getstring(vm, 2, &err_msg)))
        _Log->Debug("%s", err_msg);
    else
        _Log->Debug(_SC("unknown runtime error has occurred"));

    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
void Core::CompilerErrorHandler(HSQUIRRELVM /*vm*/, CSStr desc, CSStr src, SQInteger line, SQInteger column)
{
    LogFtl("Message: %s\n[\n=>Location: %s\n=>Line: %d\n=>Column: %d\n]", desc, src, line, column);
}

// --------------------------------------------------------------------------------------------
Object & Core::NewBlip(Int32 index, Int32 world, Float32 x, Float32 y, Float32 z,
                            Int32 scale, Uint32 color, Int32 sprid,
                            Int32 header, Object & payload)
{
    Int32 id = _Func->CreateCoordBlip(index, world, x, y, z, scale, color, sprid);

    if (INVALID_ENTITY(id))
    {
        STHROWF("Server returned invalid blip: %d", id);
    }

    BlipInst & inst = m_Blips.at(id);

    inst.mInst = new CBlip(id);
    inst.mObj = Object(inst.mInst, m_VM);

    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a blip instance for: %d", id);
    }

    inst.mID = id;
    inst.mFlags |= ENF_OWNED;
    EmitBlipCreated(id, header, payload);

    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::NewCheckpoint(Int32 player, Int32 world, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius,
                            Int32 header, Object & payload)
{
    if (INVALID_ENTITY(m_Players.at(player).mID))
    {
        STHROWF("Invalid player reference: %d", player);
    }

    Int32 id = _Func->CreateCheckpoint(player, world, x, y, z, r, g, b, a, radius);

    if (INVALID_ENTITY(id))
    {
        STHROWF("Server returned invalid checkpoint: %d", id);
    }

    CheckpointInst & inst = m_Checkpoints.at(id);

    inst.mInst = new CCheckpoint(id);
    inst.mObj = Object(inst.mInst, m_VM);

    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a checkpoint instance for: %d", id);
    }

    inst.mID = id;
    inst.mFlags |= ENF_OWNED;
    EmitCheckpointCreated(id, header, payload);

    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::NewForcefield(Int32 player, Int32 world, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Float32 radius,
                            Int32 header, Object & payload)
{
    if (INVALID_ENTITY(m_Players.at(player).mID))
    {
        STHROWF("Invalid player reference: %d", player);
    }

    Int32 id = _Func->CreateSphere(player, world, x, y, z, r, g, b, radius);

    if (INVALID_ENTITY(id))
    {
        STHROWF("Server returned invalid forcefield: %d", id);
    }

    ForcefieldInst & inst = m_Forcefields.at(id);

    inst.mInst = new CForcefield(id);
    inst.mObj = Object(inst.mInst, m_VM);

    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a forcefield instance for: %d", id);
    }

    inst.mID = id;
    inst.mFlags |= ENF_OWNED;
    EmitForcefieldCreated(id, header, payload);

    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::NewKeybind(Int32 slot, bool release,
                            Int32 primary, Int32 secondary, Int32 alternative,
                            Int32 header, Object & payload)
{
    Int32 id = _Func->RegisterKeyBind(slot, release, primary, secondary, alternative);

    if (INVALID_ENTITY(id))
    {
        STHROWF("Server returned invalid keybind: %d", id);
    }

    KeybindInst & inst = m_Keybinds.at(id);

    inst.mInst = new CKeybind(id);
    inst.mObj = Object(inst.mInst, m_VM);

    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a keybind instance for: %d", id);
    }

    inst.mID = id;
    inst.mFlags |= ENF_OWNED;
    EmitKeybindCreated(id, header, payload);

    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::NewObject(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                            Int32 alpha, Int32 header, Object & payload)
{
    Int32 id = _Func->CreateObject(model, world, x, y, z, alpha);

    if (INVALID_ENTITY(id))
    {
        STHROWF("Server returned invalid object: %d", id);
    }

    ObjectInst & inst = m_Objects.at(id);

    inst.mInst = new CObject(id);
    inst.mObj = Object(inst.mInst, m_VM);

    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a object instance for: %d", id);
    }

    inst.mID = id;
    inst.mFlags |= ENF_OWNED;
    EmitObjectCreated(id, header, payload);

    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::NewPickup(Int32 model, Int32 world, Int32 quantity,
                            Float32 x, Float32 y, Float32 z, Int32 alpha, bool automatic,
                            Int32 header, Object & payload)
{
    Int32 id = _Func->CreatePickup(model, world, quantity, x, y, z, alpha, automatic);

    if (INVALID_ENTITY(id))
    {
        STHROWF("Server returned invalid pickup: %d", id);
    }

    PickupInst & inst = m_Pickups.at(id);

    inst.mInst = new CPickup(id);
    inst.mObj = Object(inst.mInst, m_VM);

    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a pickup instance for: %d", id);
    }

    inst.mID = id;
    inst.mFlags |= ENF_OWNED;
    EmitPickupCreated(id, header, payload);

    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::NewSprite(Int32 index, CSStr file, Int32 xp, Int32 yp,
                            Int32 xr, Int32 yr, Float32 angle, Int32 alpha, bool rel,
                            Int32 header, Object & payload)
{
    Int32 id = _Func->CreateSprite(index, file, xp, yp, xr, yr, angle, alpha, rel);

    if (INVALID_ENTITY(id))
    {
        STHROWF("Server returned invalid sprite: %d", id);
    }

    SpriteInst & inst = m_Sprites.at(id);

    inst.mInst = new CSprite(id);
    inst.mObj = Object(inst.mInst, m_VM);

    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a sprite instance for: %d", id);
    }

    inst.mID = id;
    inst.mFlags |= ENF_OWNED;
    EmitSpriteCreated(id, header, payload);

    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::NewTextdraw(Int32 index, CSStr text, Int32 xp, Int32 yp,
                            Uint32 color, bool rel, Int32 header, Object & payload)
{
    Int32 id = _Func->CreateTextdraw(index, text, xp, yp, color, rel);

    if (INVALID_ENTITY(id))
    {
        STHROWF("Server returned invalid textdraw: %d", id);
    }

    TextdrawInst & inst = m_Textdraws.at(id);

    inst.mInst = new CTextdraw(id);
    inst.mObj = Object(inst.mInst, m_VM);

    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a textdraw instance for: %d", id);
    }

    inst.mID = id;
    inst.mFlags |= ENF_OWNED;
    EmitTextdrawCreated(id, header, payload);

    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
Object & Core::NewVehicle(Int32 model, Int32 world, Float32 x, Float32 y, Float32 z,
                            Float32 angle, Int32 primary, Int32 secondary,
                            Int32 header, Object & payload)
{
    Int32 id = _Func->CreateVehicle(model, world, x, y, z, angle, primary, secondary);

    if (INVALID_ENTITY(id))
    {
        STHROWF("Server returned invalid vehicle: %d", id);
    }

    VehicleInst & inst = m_Vehicles.at(id);

    inst.mInst = new CVehicle(id);
    inst.mObj = Object(inst.mInst, m_VM);

    if (!inst.mInst || inst.mObj.IsNull())
    {
        ResetInst(inst);
        STHROWF("Unable to create a vehicle instance for: %d", id);
    }

    inst.mID = id;
    inst.mFlags |= ENF_OWNED;
    EmitVehicleCreated(id, header, payload);

    return inst.mObj;
}

// --------------------------------------------------------------------------------------------
bool Core::DelBlip(Int32 id, Int32 header, Object & payload)
{
    BlipInst & inst = m_Blips.at(id);
    EmitBlipDestroyed(id, header, payload);

    if (inst.mInst)
    {
        inst.mInst->m_ID = -1;
        inst.mInst->m_Data.Release();
    }

    _Func->DestroyCoordBlip(inst.mID);

    ResetInst(inst);
    ResetFunc(inst);

    inst.mInst = NULL;
    inst.mObj.Release();

    return true;
}

bool Core::DelCheckpoint(Int32 id, Int32 header, Object & payload)
{
    CheckpointInst & inst = m_Checkpoints.at(id);
    EmitCheckpointDestroyed(id, header, payload);

    if (inst.mInst)
    {
        inst.mInst->m_ID = -1;
        inst.mInst->m_Data.Release();
    }

    _Func->DeleteCheckpoint(inst.mID);

    ResetInst(inst);
    ResetFunc(inst);

    inst.mInst = NULL;
    inst.mObj.Release();

    return true;
}

bool Core::DelForcefield(Int32 id, Int32 header, Object & payload)
{
    ForcefieldInst & inst = m_Forcefields.at(id);
    EmitForcefieldDestroyed(id, header, payload);

    if (inst.mInst)
    {
        inst.mInst->m_ID = -1;
        inst.mInst->m_Data.Release();
    }

    _Func->DeleteSphere(inst.mID);

    ResetInst(inst);
    ResetFunc(inst);

    inst.mInst = NULL;
    inst.mObj.Release();

    return true;
}

bool Core::DelKeybind(Int32 id, Int32 header, Object & payload)
{
    KeybindInst & inst = m_Keybinds.at(id);
    EmitKeybindDestroyed(id, header, payload);

    if (inst.mInst)
    {
        inst.mInst->m_ID = -1;
        inst.mInst->m_Data.Release();
    }

    _Func->RemoveKeyBind(inst.mID);

    ResetInst(inst);
    ResetFunc(inst);

    inst.mInst = NULL;
    inst.mObj.Release();

    return true;
}

bool Core::DelObject(Int32 id, Int32 header, Object & payload)
{
    ObjectInst & inst = m_Objects.at(id);
    EmitObjectDestroyed(id, header, payload);

    if (inst.mInst)
    {
        inst.mInst->m_ID = -1;
        inst.mInst->m_Data.Release();
    }

    _Func->DeleteObject(inst.mID);

    ResetInst(inst);
    ResetFunc(inst);

    inst.mInst = NULL;
    inst.mObj.Release();

    return true;
}

bool Core::DelPickup(Int32 id, Int32 header, Object & payload)
{
    PickupInst & inst = m_Pickups.at(id);
    EmitPickupDestroyed(id, header, payload);

    if (inst.mInst)
    {
        inst.mInst->m_ID = -1;
        inst.mInst->m_Data.Release();
    }

    _Func->DeletePickup(inst.mID);

    ResetInst(inst);
    ResetFunc(inst);

    inst.mInst = NULL;
    inst.mObj.Release();

    return true;
}

bool Core::DelSprite(Int32 id, Int32 header, Object & payload)
{
    SpriteInst & inst = m_Sprites.at(id);
    EmitSpriteDestroyed(id, header, payload);

    if (inst.mInst)
    {
        inst.mInst->m_ID = -1;
        inst.mInst->m_Data.Release();
    }

    _Func->DestroySprite(inst.mID);

    ResetInst(inst);
    ResetFunc(inst);

    inst.mInst = NULL;
    inst.mObj.Release();

    return true;
}

bool Core::DelTextdraw(Int32 id, Int32 header, Object & payload)
{
    TextdrawInst & inst = m_Textdraws.at(id);
    EmitTextdrawDestroyed(id, header, payload);

    if (inst.mInst)
    {
        inst.mInst->m_ID = -1;
        inst.mInst->m_Data.Release();
    }

    _Func->DestroyTextdraw(inst.mID);

    ResetInst(inst);
    ResetFunc(inst);

    inst.mInst = NULL;
    inst.mObj.Release();

    return true;
}

bool Core::DelVehicle(Int32 id, Int32 header, Object & payload)
{
    VehicleInst & inst = m_Vehicles.at(id);
    EmitVehicleDestroyed(id, header, payload);

    if (inst.mInst)
    {
        inst.mInst->m_ID = -1;
        inst.mInst->m_Data.Release();
    }

    _Func->DeleteVehicle(inst.mID);

    ResetInst(inst);
    ResetFunc(inst);

    inst.mInst = NULL;
    inst.mObj.Release();

    return true;
}

// --------------------------------------------------------------------------------------------
void Core::ConnectPlayer(Int32 id, Int32 header, Object & payload)
{
    PlayerInst & inst = m_Players.at(id);
    inst.mInst = new CPlayer(id);
    inst.mObj = Object(inst.mInst, m_VM);

    if (!inst.mInst || inst.mObj.IsNull())
    {
        LogErr("Unable to create a player instance for: %d", id);
    }
    else
    {
        inst.mID = id;
        EmitPlayerCreated(id, header, payload);
    }
}

void Core::DisconnectPlayer(Int32 id, Int32 header, Object & payload)
{
    if (static_cast< Uint32 >(id) >= m_Players.size())
        return;

    PlayerInst & inst = m_Players.at(id);
    EmitPlayerDestroyed(id, header, payload);

    if (inst.mInst)
    {
        inst.mInst->m_ID = -1;
        inst.mInst->m_Data.Release();
    }

    ResetInst(inst);
    ResetFunc(inst);

    inst.mInst = NULL;
    inst.mObj.Release();
}

// ------------------------------------------------------------------------------------------------
bool Core::BindEvent(Int32 id, Object & env, Function & func)
{
    Function & event = GetEvent(id);

    if (func.IsNull())
        event.Release();
    else
        event = Function(env.GetVM(), env, func.GetFunc());

    return true;
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

void Core::EmitScriptUnload()
{
    Emit(mOnScriptUnload);
}

void Core::EmitPlayerUpdate(Int32 /*player*/, Int32 /*type*/)
{

}

void Core::EmitVehicleUpdate(Int32 /*vehicle*/, Int32 /*type*/)
{

}

void Core::EmitEntityPool(Int32 /*type*/, Int32 /*id*/, bool /*deleted*/)
{

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
        inst.mPrefixes[n].clear();
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
    inst.mOnDestroyed.Release();
    inst.mOnCustom.Release();
}

void Core::ResetFunc(CheckpointInst & inst)
{
    inst.mOnDestroyed.Release();
    inst.mOnCustom.Release();
    inst.mOnEntered.Release();
    inst.mOnExited.Release();
}

void Core::ResetFunc(ForcefieldInst & inst)
{
    inst.mOnDestroyed.Release();
    inst.mOnCustom.Release();
    inst.mOnEntered.Release();
    inst.mOnExited.Release();
}

void Core::ResetFunc(KeybindInst & inst)
{
    inst.mOnDestroyed.Release();
    inst.mOnCustom.Release();
    inst.mOnKeyPress.Release();
    inst.mOnKeyRelease.Release();
}

void Core::ResetFunc(ObjectInst & inst)
{
    inst.mOnDestroyed.Release();
    inst.mOnCustom.Release();
    inst.mOnShot.Release();
    inst.mOnBump.Release();
}

void Core::ResetFunc(PickupInst & inst)
{
    inst.mOnDestroyed.Release();
    inst.mOnCustom.Release();
    inst.mOnRespawn.Release();
    inst.mOnClaimed.Release();
    inst.mOnCollected.Release();
}

void Core::ResetFunc(PlayerInst & inst)
{
    inst.mOnDestroyed.Release();
    inst.mOnCustom.Release();
    inst.mOnAway.Release();
    inst.mOnGameKeys.Release();
    inst.mOnRename.Release();
    inst.mOnRequestClass.Release();
    inst.mOnRequestSpawn.Release();
    inst.mOnSpawn.Release();
    inst.mOnStartTyping.Release();
    inst.mOnStopTyping.Release();
    inst.mOnChat.Release();
    inst.mOnCommand.Release();
    inst.mOnMessage.Release();
    inst.mOnHealth.Release();
    inst.mOnArmour.Release();
    inst.mOnWeapon.Release();
    inst.mOnMove.Release();
    inst.mOnWasted.Release();
    inst.mOnKilled.Release();
    inst.mOnTeamKill.Release();
    inst.mOnSpectate.Release();
    inst.mOnCrashreport.Release();
    inst.mOnBurning.Release();
    inst.mOnCrouching.Release();
    inst.mOnState.Release();
    inst.mOnAction.Release();
    inst.mOnStateNone.Release();
    inst.mOnStateNormal.Release();
    inst.mOnStateShooting.Release();
    inst.mOnStateDriver.Release();
    inst.mOnStatePassenger.Release();
    inst.mOnStateEnterDriver.Release();
    inst.mOnStateEnterPassenger.Release();
    inst.mOnStateExitVehicle.Release();
    inst.mOnStateUnspawned.Release();
    inst.mOnActionNone.Release();
    inst.mOnActionNormal.Release();
    inst.mOnActionAiming.Release();
    inst.mOnActionShooting.Release();
    inst.mOnActionJumping.Release();
    inst.mOnActionLieDown.Release();
    inst.mOnActionGettingUp.Release();
    inst.mOnActionJumpVehicle.Release();
    inst.mOnActionDriving.Release();
    inst.mOnActionDying.Release();
    inst.mOnActionWasted.Release();
    inst.mOnActionEmbarking.Release();
    inst.mOnActionDisembarking.Release();
    inst.mOnKeyPress.Release();
    inst.mOnKeyRelease.Release();
    inst.mOnEmbarking.Release();
    inst.mOnEmbarked.Release();
    inst.mOnDisembark.Release();
    inst.mOnPickupClaimed.Release();
    inst.mOnPickupCollected.Release();
    inst.mOnObjectShot.Release();
    inst.mOnObjectBump.Release();
    inst.mOnCheckpointEntered.Release();
    inst.mOnCheckpointExited.Release();
    inst.mOnForcefieldEntered.Release();
    inst.mOnForcefieldExited.Release();
}

void Core::ResetFunc(SpriteInst & inst)
{
    inst.mOnDestroyed.Release();
    inst.mOnCustom.Release();
}

void Core::ResetFunc(TextdrawInst & inst)
{
    inst.mOnDestroyed.Release();
    inst.mOnCustom.Release();
}

void Core::ResetFunc(VehicleInst & inst)
{
    inst.mOnDestroyed.Release();
    inst.mOnCustom.Release();
    inst.mOnRespawn.Release();
    inst.mOnExplode.Release();
    inst.mOnHealth.Release();
    inst.mOnMove.Release();
    inst.mOnEmbarking.Release();
    inst.mOnEmbarked.Release();
    inst.mOnDisembark.Release();
}

void Core::ResetFunc()
{
    mOnBlipCreated.Release();
    mOnCheckpointCreated.Release();
    mOnForcefieldCreated.Release();
    mOnKeybindCreated.Release();
    mOnObjectCreated.Release();
    mOnPickupCreated.Release();
    mOnPlayerCreated.Release();
    mOnSpriteCreated.Release();
    mOnTextdrawCreated.Release();
    mOnVehicleCreated.Release();
    mOnBlipDestroyed.Release();
    mOnCheckpointDestroyed.Release();
    mOnForcefieldDestroyed.Release();
    mOnKeybindDestroyed.Release();
    mOnObjectDestroyed.Release();
    mOnPickupDestroyed.Release();
    mOnPlayerDestroyed.Release();
    mOnSpriteDestroyed.Release();
    mOnTextdrawDestroyed.Release();
    mOnVehicleDestroyed.Release();
    mOnBlipCustom.Release();
    mOnCheckpointCustom.Release();
    mOnForcefieldCustom.Release();
    mOnKeybindCustom.Release();
    mOnObjectCustom.Release();
    mOnPickupCustom.Release();
    mOnPlayerCustom.Release();
    mOnSpriteCustom.Release();
    mOnTextdrawCustom.Release();
    mOnVehicleCustom.Release();
    mOnPlayerAway.Release();
    mOnPlayerGameKeys.Release();
    mOnPlayerRename.Release();
    mOnPlayerRequestClass.Release();
    mOnPlayerRequestSpawn.Release();
    mOnPlayerSpawn.Release();
    mOnPlayerStartTyping.Release();
    mOnPlayerStopTyping.Release();
    mOnPlayerChat.Release();
    mOnPlayerCommand.Release();
    mOnPlayerMessage.Release();
    mOnPlayerHealth.Release();
    mOnPlayerArmour.Release();
    mOnPlayerWeapon.Release();
    mOnPlayerMove.Release();
    mOnPlayerWasted.Release();
    mOnPlayerKilled.Release();
    mOnPlayerTeamKill.Release();
    mOnPlayerSpectate.Release();
    mOnPlayerCrashreport.Release();
    mOnPlayerBurning.Release();
    mOnPlayerCrouching.Release();
    mOnPlayerState.Release();
    mOnPlayerAction.Release();
    mOnStateNone.Release();
    mOnStateNormal.Release();
    mOnStateShooting.Release();
    mOnStateDriver.Release();
    mOnStatePassenger.Release();
    mOnStateEnterDriver.Release();
    mOnStateEnterPassenger.Release();
    mOnStateExitVehicle.Release();
    mOnStateUnspawned.Release();
    mOnActionNone.Release();
    mOnActionNormal.Release();
    mOnActionAiming.Release();
    mOnActionShooting.Release();
    mOnActionJumping.Release();
    mOnActionLieDown.Release();
    mOnActionGettingUp.Release();
    mOnActionJumpVehicle.Release();
    mOnActionDriving.Release();
    mOnActionDying.Release();
    mOnActionWasted.Release();
    mOnActionEmbarking.Release();
    mOnActionDisembarking.Release();
    mOnVehicleRespawn.Release();
    mOnVehicleExplode.Release();
    mOnVehicleHealth.Release();
    mOnVehicleMove.Release();
    mOnPickupRespawn.Release();
    mOnKeybindKeyPress.Release();
    mOnKeybindKeyRelease.Release();
    mOnVehicleEmbarking.Release();
    mOnVehicleEmbarked.Release();
    mOnVehicleDisembark.Release();
    mOnPickupClaimed.Release();
    mOnPickupCollected.Release();
    mOnObjectShot.Release();
    mOnObjectBump.Release();
    mOnCheckpointEntered.Release();
    mOnCheckpointExited.Release();
    mOnForcefieldEntered.Release();
    mOnForcefieldExited.Release();
    mOnServerFrame.Release();
    mOnServerStartup.Release();
    mOnServerShutdown.Release();
    mOnInternalCommand.Release();
    mOnLoginAttempt.Release();
    mOnCustomEvent.Release();
    mOnWorldOption.Release();
    mOnWorldToggle.Release();
    mOnScriptReload.Release();
    mOnScriptUnload.Release();
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
        case EVT_SCRIPTUNLOAD:          return mOnScriptUnload;
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
    if (mID >=0 && (mFlags & ENF_OWNED))
    {
        _Core->EmitBlipDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
        _Func->DestroyCoordBlip(mID);
    }
}

Core::CheckpointInst::~CheckpointInst()
{
    if (mID >=0 && (mFlags & ENF_OWNED))
    {
        _Core->EmitCheckpointDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
        _Func->DeleteCheckpoint(mID);
    }
}

Core::ForcefieldInst::~ForcefieldInst()
{
    if (mID >=0 && (mFlags & ENF_OWNED))
    {
        _Core->EmitForcefieldDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
        _Func->DeleteSphere(mID);
    }
}

Core::KeybindInst::~KeybindInst()
{
    if (mID >=0 && (mFlags & ENF_OWNED))
    {
        _Core->EmitKeybindDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
        _Func->RemoveKeyBind(mID);
    }
}

Core::ObjectInst::~ObjectInst()
{
    if (mID >=0 && (mFlags & ENF_OWNED))
    {
        _Core->EmitObjectDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
        _Func->DeleteObject(mID);
    }
}

Core::PickupInst::~PickupInst()
{
    if (mID >=0 && (mFlags & ENF_OWNED))
    {
        _Core->EmitPickupDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
        _Func->DeletePickup(mID);
    }
}

Core::SpriteInst::~SpriteInst()
{
    if (mID >=0 && (mFlags & ENF_OWNED))
    {
        _Core->EmitSpriteDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
        _Func->DestroySprite(mID);
    }
}

Core::TextdrawInst::~TextdrawInst()
{
    if (mID >=0 && (mFlags & ENF_OWNED))
    {
        _Core->EmitTextdrawDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
        _Func->DestroyTextdraw(mID);
    }
}

Core::VehicleInst::~VehicleInst()
{
    if (mID >=0 && (mFlags & ENF_OWNED))
    {
        _Core->EmitVehicleDestroyed(mID, SQMOD_DESTROY_CLEANUP, NullObject());
        _Func->DeleteVehicle(mID);
    }
}

} // Namespace:: SqMod
