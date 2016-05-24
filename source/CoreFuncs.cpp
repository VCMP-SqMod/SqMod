// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Base/Stack.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern bool GetReloadStatus();
extern void SetReloadStatus(bool toggle);

// ------------------------------------------------------------------------------------------------
static void BindEvent(Int32 id, Object & env, Function & func)
{
    Core::Get().BindEvent(id, env, func);
}

// ------------------------------------------------------------------------------------------------
static void CustomEvent(Int32 group, Int32 header, Object & payload)
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
        .Func(_SC("Bind"), &BindEvent)
        .Func(_SC("CustomEvent"), &CustomEvent)
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
    );
}

} // Namespace:: SqMod
