// ------------------------------------------------------------------------------------------------
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern bool GetReloadStatus();
extern void SetReloadStatus(bool toggle);

// ------------------------------------------------------------------------------------------------
static SQInteger SqLoadScript(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
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
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    else if (SQ_FAILED(sq_getbool(vm, 2, &delay)))
    {
        return sq_throwerror(vm, "Failed to retrieve the delay parameter");
    }
    // Forward the call to the actual implementation
    sq_pushbool(vm, Core::Get().LoadScript(val.mPtr, static_cast< bool >(delay)));
    // We have an argument on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static void SqBindPreLoad(Object & env, Function & func)
{
    Core::Get().BindPreLoad(env, func, NullObject());
}

// ------------------------------------------------------------------------------------------------
static void SqBindPostLoad(Object & env, Function & func)
{
    Core::Get().BindPostLoad(env, func, NullObject());
}

// ------------------------------------------------------------------------------------------------
static void SqBindUnload(Object & env, Function & func)
{
    Core::Get().BindUnload(env, func, NullObject());
}

// ------------------------------------------------------------------------------------------------
static void SqBindPreLoadEx(Object & env, Function & func, Object & payload)
{
    Core::Get().BindPreLoad(env, func, payload);
}

// ------------------------------------------------------------------------------------------------
static void SqBindPostLoadEx(Object & env, Function & func, Object & payload)
{
    Core::Get().BindPostLoad(env, func, payload);
}

// ------------------------------------------------------------------------------------------------
static void SqBindUnloadEx(Object & env, Function & func, Object & payload)
{
    Core::Get().BindUnload(env, func, payload);
}

// ------------------------------------------------------------------------------------------------
static void SqBindEvent(Int32 id, Object & env, Function & func)
{
    Core::Get().BindEvent(id, env, func);
}

// ------------------------------------------------------------------------------------------------
static void SqCustomEvent(Int32 group, Int32 header, Object & payload)
{
    Core::Get().EmitCustomEvent(group, header, payload);
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
static void SqReloadBecause(Int32 header, Object & payload)
{
    // Assign the reload info
    Core::Get().SetReloadInfo(header, payload);
    // Enable reloading
    SetReloadStatus(true);
}

// ------------------------------------------------------------------------------------------------
static void SqSetReloadInfo(Int32 header, Object & payload)
{
    Core::Get().SetReloadInfo(header, payload);
}

// ------------------------------------------------------------------------------------------------
static Int32 SqGetReloadHeader()
{
    return Core::Get().GetReloadHeader();
}

// ------------------------------------------------------------------------------------------------
static Object & SqGetReloadPayload()
{
    return Core::Get().GetReloadPayload();
}

// ------------------------------------------------------------------------------------------------
static Int32 GetState()
{
    return Core::Get().GetState();
}

// ------------------------------------------------------------------------------------------------
static void SetState(Int32 value)
{
    return Core::Get().SetState(value);
}

// ------------------------------------------------------------------------------------------------
static CSStr GetOption(CSStr name)
{
    return Core::Get().GetOption(name);
}

// ------------------------------------------------------------------------------------------------
static CSStr GetOptionOr(CSStr name, CSStr value)
{
    return Core::Get().GetOption(name, value);
}

// ------------------------------------------------------------------------------------------------
static void SetOption(CSStr name, CSStr value)
{
    return Core::Get().SetOption(name, value);
}

// ------------------------------------------------------------------------------------------------
static Object & GetBlip(Int32 id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Out of range blip identifier: %d", id);
    }
    // Return the requested information
    return Core::Get().GetBlip(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static Object & GetCheckpoint(Int32 id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Out of range checkpoint identifier: %d", id);
    }
    // Return the requested information
    return Core::Get().GetCheckpoint(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static Object & GetKeybind(Int32 id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Out of range keybind identifier: %d", id);
    }
    // Return the requested information
    return Core::Get().GetKeybind(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static Object & GetObject(Int32 id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Out of range object identifier: %d", id);
    }
    // Return the requested information
    return Core::Get().GetObject(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static Object & GetPickup(Int32 id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Out of range blip identifier: %d", id);
    }
    // Return the requested information
    return Core::Get().GetPickup(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static Object & GetPlayer(Int32 id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_PLAYER_POOL))
    {
        STHROWF("Out of range player identifier: %d", id);
    }
    // Return the requested information
    return Core::Get().GetPlayer(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static Object & GetVehicle(Int32 id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Out of range vehicle identifier: %d", id);
    }
    // Return the requested information
    return Core::Get().GetVehicle(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static bool DelBlip(Int32 id, Int32 header, Object & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_BLIP_POOL))
    {
        STHROWF("Out of range blip identifier: %d", id);
    }
    // Perform the requested operation
    return Core::Get().DelBlip(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static bool DelCheckpoint(Int32 id, Int32 header, Object & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_CHECKPOINT_POOL))
    {
        STHROWF("Out of range checkpoint identifier: %d", id);
    }
    // Perform the requested operation
    return Core::Get().DelCheckpoint(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static bool DelKeybind(Int32 id, Int32 header, Object & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_KEYBIND_POOL))
    {
        STHROWF("Out of range keybind identifier: %d", id);
    }
    // Perform the requested operation
    return Core::Get().DelKeybind(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static bool DelObject(Int32 id, Int32 header, Object & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Out of range object identifier: %d", id);
    }
    // Perform the requested operation
    return Core::Get().DelObject(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static bool DelPickup(Int32 id, Int32 header, Object & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_PICKUP_POOL))
    {
        STHROWF("Out of range blip identifier: %d", id);
    }
    // Perform the requested operation
    return Core::Get().DelPickup(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static bool DelVehicle(Int32 id, Int32 header, Object & payload)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_VEHICLE_POOL))
    {
        STHROWF("Out of range vehicle identifier: %d", id);
    }
    // Perform the requested operation
    return Core::Get().DelVehicle(id, header, payload);
}

// ================================================================================================
void Register_Core(HSQUIRRELVM vm)
{
    RootTable(vm)
    .Bind(_SC("SqCore"), Table(vm)
        .Func(_SC("Bind"), &SqBindEvent)
        .Func(_SC("BindPreLoad"), &SqBindPreLoad)
        .Func(_SC("BindPostLoad"), &SqBindPostLoad)
        .Func(_SC("BindUnload"), &SqBindUnload)
        .Func(_SC("BindPreLoadEx"), &SqBindPreLoadEx)
        .Func(_SC("BindPostLoadEx"), &SqBindPostLoadEx)
        .Func(_SC("BindUnloadEx"), &SqBindUnloadEx)
        .Func(_SC("CustomEvent"), &SqCustomEvent)
        .Func(_SC("Reload"), &SqSetReloadStatus)
        .Func(_SC("Reloading"), &SqGetReloadStatus)
        .Func(_SC("ReloadBecause"), &SqReloadBecause)
        .Func(_SC("SetReloadInfo"), &SqSetReloadInfo)
        .Func(_SC("GetReloadHeader"), &SqGetReloadHeader)
        .Func(_SC("GetReloadPayload"), &SqGetReloadPayload)
        .Func(_SC("GetState"), &GetState)
        .Func(_SC("SetState"), &SetState)
        .Func(_SC("GetOption"), &GetOption)
        .Func(_SC("GetOptionOr"), &GetOptionOr)
        .Func(_SC("SetOption"), &SetOption)
        .Func(_SC("GetBlip"), &GetBlip)
        .Func(_SC("GetCheckpoint"), &GetCheckpoint)
        .Func(_SC("GetKeybind"), &GetKeybind)
        .Func(_SC("GetObject"), &GetObject)
        .Func(_SC("GetPickup"), &GetPickup)
        .Func(_SC("GetPlayer"), &GetPlayer)
        .Func(_SC("GetVehicle"), &GetVehicle)
        .Func(_SC("DestroyBlip"), &DelBlip)
        .Func(_SC("DestroyCheckpoint"), &DelCheckpoint)
        .Func(_SC("DestroyKeybind"), &DelKeybind)
        .Func(_SC("DestroyObject"), &DelObject)
        .Func(_SC("DestroyPickup"), &DelPickup)
        .Func(_SC("DestroyVehicle"), &DelVehicle)
        .SquirrelFunc(_SC("LoadScript"), &SqLoadScript)
    );
}

} // Namespace:: SqMod
