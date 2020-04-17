// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern bool GetReloadStatus();
extern void SetReloadStatus(bool toggle);

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(CoreStateTypename, _SC("SqCoreState"))

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
    if (SQ_FAILED(val.Proc(true)))
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
static SQInteger SqGetEvents(HSQUIRRELVM vm)
{
    // Push the events table object on the stack
    sq_pushobject(vm, Core::Get().GetEvents().mObj);
    // Specify that we're returning a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static void SqEmitCustomEvent(Int32 group, Int32 header, LightObj & payload)
{
    Core::Get().EmitCustomEvent(group, header, payload);
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqForceEnableNullEntities(HSQUIRRELVM vm)
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
static void SqReloadBecause(Int32 header, LightObj & payload)
{
    // Assign the reload info
    Core::Get().SetReloadInfo(header, payload);
    // Enable reloading
    SetReloadStatus(true);
}

// ------------------------------------------------------------------------------------------------
static void SqSetReloadInfo(Int32 header, LightObj & payload)
{
    Core::Get().SetReloadInfo(header, payload);
}

// ------------------------------------------------------------------------------------------------
static Int32 SqGetReloadHeader()
{
    return Core::Get().GetReloadHeader();
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetReloadPayload()
{
    return Core::Get().GetReloadPayload();
}

// ------------------------------------------------------------------------------------------------
static Int32 SqGetState()
{
    return Core::Get().GetState();
}

// ------------------------------------------------------------------------------------------------
static void SqSetState(Int32 value)
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
static CSStr SqGetOption(CSStr name)
{
    return Core::Get().GetOption(name);
}

// ------------------------------------------------------------------------------------------------
static CSStr SqGetOptionOr(CSStr name, CSStr value)
{
    return Core::Get().GetOption(name, value);
}

// ------------------------------------------------------------------------------------------------
static void SqSetOption(CSStr name, CSStr value)
{
    return Core::Get().SetOption(name, value);
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetBlip(Int32 id)
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
static LightObj & SqGetCheckpoint(Int32 id)
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
static LightObj & SqGetKeybind(Int32 id)
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
static LightObj & SqGetObj(Int32 id)
{
    // Validate the identifier first
    if (INVALID_ENTITYEX(id, SQMOD_OBJECT_POOL))
    {
        STHROWF("Out of range object identifier: %d", id);
    }
    // Return the requested information
    return Core::Get().GetObj(id).mObj;
}

// ------------------------------------------------------------------------------------------------
static LightObj & SqGetPickup(Int32 id)
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
static LightObj & SqGetPlayer(Int32 id)
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
static LightObj & SqGetVehicle(Int32 id)
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
static bool SqDelBlip(Int32 id, Int32 header, LightObj & payload)
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
static bool SqDelCheckpoint(Int32 id, Int32 header, LightObj & payload)
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
static bool SqDelKeybind(Int32 id, Int32 header, LightObj & payload)
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
static bool SqDelObject(Int32 id, Int32 header, LightObj & payload)
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
static bool SqDelPickup(Int32 id, Int32 header, LightObj & payload)
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
static bool SqDelVehicle(Int32 id, Int32 header, LightObj & payload)
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
    Table corens(vm);

    corens.Bind(_SC("State"),
        Class< CoreState, NoCopy< CoreState > >(vm, CoreStateTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< int >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &CoreStateTypename::Fn)
        // Member Properties
        .Prop(_SC("Value"), &CoreState::GetValue)
    );

    corens
        .Func(_SC("Reload"), &SqSetReloadStatus)
        .Func(_SC("Reloading"), &SqGetReloadStatus)
        .Func(_SC("ReloadBecause"), &SqReloadBecause)
        .Func(_SC("SetReloadInfo"), &SqSetReloadInfo)
        .Func(_SC("GetReloadHeader"), &SqGetReloadHeader)
        .Func(_SC("GetReloadPayload"), &SqGetReloadPayload)
        .Func(_SC("EmitCustomEvent"), &SqEmitCustomEvent)
        .Func(_SC("GetState"), &SqGetState)
        .Func(_SC("SetState"), &SqSetState)
        .Func(_SC("AreasEnabled"), &SqGetAreasEnabled)
        .Func(_SC("SetAreasEnabled"), &SqSetAreasEnabled)
        .Func(_SC("GetOption"), &SqGetOption)
        .Func(_SC("GetOptionOr"), &SqGetOptionOr)
        .Func(_SC("SetOption"), &SqSetOption)
        .Func(_SC("GetBlip"), &SqGetBlip)
        .Func(_SC("GetCheckpoint"), &SqGetCheckpoint)
        .Func(_SC("GetKeybind"), &SqGetKeybind)
        .Func(_SC("GetObj"), &SqGetObj)
        .Func(_SC("GetPickup"), &SqGetPickup)
        .Func(_SC("GetPlayer"), &SqGetPlayer)
        .Func(_SC("GetVehicle"), &SqGetVehicle)
        .Func(_SC("DestroyBlip"), &SqDelBlip)
        .Func(_SC("DestroyCheckpoint"), &SqDelCheckpoint)
        .Func(_SC("DestroyKeybind"), &SqDelKeybind)
        .Func(_SC("DestroyObject"), &SqDelObject)
        .Func(_SC("DestroyPickup"), &SqDelPickup)
        .Func(_SC("DestroyVehicle"), &SqDelVehicle)
        .Func(_SC("OnPreLoad"), &SqGetPreLoadEvent)
        .Func(_SC("OnPostLoad"), &SqGetPostLoadEvent)
        .Func(_SC("OnUnload"), &SqGetUnloadEvent)
        .SquirrelFunc(_SC("ForceEnableNullEntities"), &SqForceEnableNullEntities)
        .SquirrelFunc(_SC("LoadScript"), &SqLoadScript, -3, ".b.")
        .SquirrelFunc(_SC("On"), &SqGetEvents);

    RootTable(vm).Bind(_SC("SqCore"), corens);
}

} // Namespace:: SqMod
