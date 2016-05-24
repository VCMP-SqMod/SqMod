// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Base/Stack.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void EnableReload(Int32 header, Object & payload);
extern void DisableReload();
extern bool ReloadEnabled();

// ------------------------------------------------------------------------------------------------
static Object & GetBlip(Int32 id) { return Core::Get().GetBlip(id).mObj; }
static Object & GetCheckpoint(Int32 id) { return Core::Get().GetCheckpoint(id).mObj; }
static Object & GetKeybind(Int32 id) { return Core::Get().GetKeybind(id).mObj; }
static Object & GetObject(Int32 id) { return Core::Get().GetObject(id).mObj; }
static Object & GetPickup(Int32 id) { return Core::Get().GetPickup(id).mObj; }
static Object & GetPlayer(Int32 id) { return Core::Get().GetPlayer(id).mObj; }
static Object & GetVehicle(Int32 id) { return Core::Get().GetVehicle(id).mObj; }

// ------------------------------------------------------------------------------------------------
static bool DelBlip(Int32 id, Int32 header, Object & payload)
{
    return Core::Get().DelBlip(id, header, payload);
}

static bool DelCheckpoint(Int32 id, Int32 header, Object & payload)
{
    return Core::Get().DelCheckpoint(id, header, payload);
}

static bool DelKeybind(Int32 id, Int32 header, Object & payload)
{
    return Core::Get().DelKeybind(id, header, payload);
}

static bool DelObject(Int32 id, Int32 header, Object & payload)
{
    return Core::Get().DelObject(id, header, payload);
}

static bool DelPickup(Int32 id, Int32 header, Object & payload)
{
    return Core::Get().DelPickup(id, header, payload);
}

static bool DelVehicle(Int32 id, Int32 header, Object & payload)
{
    return Core::Get().DelVehicle(id, header, payload);
}

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
static Int32 GetState()
{
    return Core::Get().GetState();
}

static void SetState(Int32 value)
{
    return Core::Get().SetState(value);
}

// ------------------------------------------------------------------------------------------------
static CSStr GetOption(CSStr name)
{
    return Core::Get().GetOption(name);
}

static CSStr GetOptionOr(CSStr name, CSStr value)
{
    return Core::Get().GetOption(name, value);
}

static void SetOption(CSStr name, CSStr value)
{
    return Core::Get().SetOption(name, value);
}

// ------------------------------------------------------------------------------------------------
static void SetReload(bool toggle)
{
    if (toggle)
    {
        EnableReload(0, NullObject());
    }
    else
    {
        DisableReload();
    }
}

// ------------------------------------------------------------------------------------------------
static void SetReloadBecause(Int32 header, Object & payload)
{
    EnableReload(header, payload);
}

// ------------------------------------------------------------------------------------------------
static bool IsReloading()
{
    return ReloadEnabled();
}

// ================================================================================================
void Register_Core(HSQUIRRELVM vm)
{
    RootTable(vm)
    .Bind(_SC("SqCore"), Table(vm)
        .Func(_SC("Bind"), &BindEvent)
        .Func(_SC("CustomEvent"), &CustomEvent)
        .Func(_SC("Reload"), &SetReload)
        .Func(_SC("ReloadBecause"), &SetReloadBecause)
        .Func(_SC("Reloading"), &IsReloading)
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
