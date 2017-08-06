// ------------------------------------------------------------------------------------------------
#include "Entity/Pickup.hpp"
#include "Entity/Player.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"
#include "Tasks.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqPickup"))

// ------------------------------------------------------------------------------------------------
const Int32 CPickup::Max = SQMOD_PICKUP_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CPickup::SqGetNull(HSQUIRRELVM vm)
{
    sq_pushobject(vm, Core::Get().GetNullPickup().GetObject());
    return 1;
}

// ------------------------------------------------------------------------------------------------
LightObj & CPickup::GetNull()
{
    return Core::Get().GetNullPickup();
}

// ------------------------------------------------------------------------------------------------
CPickup::CPickup(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_PICKUP_POOL))
    , m_Tag(ToStrF("%d", id)), m_Data(), m_CircularLocks(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CPickup::~CPickup()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
const String & CPickup::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CPickup::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetTag(const StackStrF & tag)
{
    if (tag.mLen > 0)
    {
        m_Tag.assign(tag.mPtr, tag.mLen);
    }
    else
    {
        m_Tag.clear();
    }
}

// ------------------------------------------------------------------------------------------------
CPickup & CPickup::ApplyTag(const StackStrF & tag)
{
    SetTag(tag);
    return *this;
}

// ------------------------------------------------------------------------------------------------
LightObj & CPickup::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetData(LightObj & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CPickup::Destroy(Int32 header, LightObj & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return Core::Get().DelPickup(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
LightObj & CPickup::GetEvents() const
{
    // Validate the managed identifier
    Validate();
    // Return the associated event table
    return Core::Get().GetPickup(m_ID).mEvents;
}

// ------------------------------------------------------------------------------------------------
void CPickup::CustomEvent(Int32 header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perfrom the requested action
    Core::Get().EmitPickupCustom(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CPickup::IsStreamedFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPickupStreamedForPlayer(m_ID, player.GetID());
}

// ------------------------------------------------------------------------------------------------
Int32 CPickup::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetWorld(Int32 world)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const Int32 current = _Func->GetPickupWorld(m_ID);
    // Don't even bother if it's the same value
    if (current == world)
    {
        return;
    }
    // Avoid property unwind from a recursive call
    _Func->SetPickupWorld(m_ID, world);
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & PICKUPCL_EMIT_PICKUP_WORLD))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, PICKUPCL_EMIT_PICKUP_WORLD);
        // Now forward the event call
        Core::Get().EmitPickupWorld(m_ID, current, world);
    }
}

// ------------------------------------------------------------------------------------------------
Int32 CPickup::GetAlpha() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupAlpha(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetAlpha(Int32 alpha)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const Int32 current = _Func->GetPickupAlpha(m_ID);
    // Don't even bother if it's the same value
    if (current == alpha)
    {
        return;
    }
    // Avoid property unwind from a recursive call
    _Func->SetPickupAlpha(m_ID, alpha);
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & PICKUPCL_EMIT_PICKUP_ALPHA))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, PICKUPCL_EMIT_PICKUP_ALPHA);
        // Now forward the event call
        Core::Get().EmitPickupAlpha(m_ID, current, alpha);
    }
}

// ------------------------------------------------------------------------------------------------
bool CPickup::GetAutomatic() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsPickupAutomatic(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetAutomatic(bool toggle)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const bool current = _Func->IsPickupAutomatic(m_ID);
    // Don't even bother if it's the same value
    if (current == toggle)
    {
        return;
    }
    // Avoid property unwind from a recursive call
    _Func->SetPickupIsAutomatic(m_ID, toggle);
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & PICKUPCL_EMIT_PICKUP_AUTOMATIC))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, PICKUPCL_EMIT_PICKUP_AUTOMATIC);
        // Now forward the event call
        Core::Get().EmitPickupAutomatic(m_ID, current, toggle);
    }
}

// ------------------------------------------------------------------------------------------------
Int32 CPickup::GetAutoTimer() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupAutoTimer(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetAutoTimer(Int32 timer)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const Int32 current = _Func->GetPickupAutoTimer(m_ID);
    // Don't even bother if it's the same value
    if (current == timer)
    {
        return;
    }
    // Avoid property unwind from a recursive call
    _Func->SetPickupAutoTimer(m_ID, timer);
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & PICKUPCL_EMIT_PICKUP_AUTOTIMER))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, PICKUPCL_EMIT_PICKUP_AUTOTIMER);
        // Now forward the event call
        Core::Get().EmitPickupAutoTimer(m_ID, current, timer);
    }
}

// ------------------------------------------------------------------------------------------------
void CPickup::Refresh() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RefreshPickup(m_ID);
}

// ------------------------------------------------------------------------------------------------
Vector3 CPickup::GetPosition()
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the position values
    _Func->GetPickupPosition(m_ID, &vec.x, &vec.y, &vec.z);
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPosition(const Vector3 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
Int32 CPickup::GetModel() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupModel(m_ID);
}

// ------------------------------------------------------------------------------------------------
Int32 CPickup::GetQuantity() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupQuantity(m_ID);
}

// ------------------------------------------------------------------------------------------------
Float32 CPickup::GetPositionX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    Float32 x = 0.0f;
    // Query the server for the requested component value
    _Func->GetPickupPosition(m_ID, &x, nullptr, nullptr);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
Float32 CPickup::GetPositionY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    Float32 y = 0.0f;
    // Query the server for the requested component value
    _Func->GetPickupPosition(m_ID, nullptr, &y, nullptr);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
Float32 CPickup::GetPositionZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    Float32 z = 0.0f;
    // Query the server for the requested component value
    _Func->GetPickupPosition(m_ID, nullptr, nullptr, &z);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 y, z;
    // Retrieve the current values for unchanged components
    _Func->GetPickupPosition(m_ID, nullptr, &y, &z);
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 x, z;
    // Retrieve the current values for unchanged components
    _Func->GetPickupPosition(m_ID, &x, nullptr, &z);
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    Float32 x, y;
    // Retrieve the current values for unchanged components
    _Func->GetPickupPosition(m_ID, &x, &y, nullptr);
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, z, y, z);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Pickup_CreateEx(Int32 model, Int32 world, Int32 quantity,
                        Float32 x, Float32 y, Float32 z, Int32 alpha, bool automatic)
{
    return Core::Get().NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Pickup_CreateEx(Int32 model, Int32 world, Int32 quantity,
                        Float32 x, Float32 y, Float32 z, Int32 alpha, bool automatic,
                        Int32 header, LightObj & payload)
{
    return Core::Get().NewPickup(model, world, quantity, x, y, z, alpha, automatic, header, payload);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Pickup_Create(Int32 model, Int32 world, Int32 quantity, const Vector3 & pos,
                            Int32 alpha, bool automatic)
{
    return Core::Get().NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Pickup_Create(Int32 model, Int32 world, Int32 quantity, const Vector3 & pos,
                            Int32 alpha, bool automatic, Int32 header, LightObj & payload)
{
    return Core::Get().NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            header, payload);
}

// ================================================================================================
void Register_CPickup(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(Typename::Str,
        Class< CPickup, NoConstructor< CPickup > >(vm, Typename::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &CPickup::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CPickup::Max)
        // Core Properties
        .Prop(_SC("On"), &CPickup::GetEvents)
        .Prop(_SC("ID"), &CPickup::GetID)
        .Prop(_SC("Tag"), &CPickup::GetTag, &CPickup::SetTag)
        .Prop(_SC("Data"), &CPickup::GetData, &CPickup::SetData)
        .Prop(_SC("Active"), &CPickup::IsActive)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &CPickup::ApplyTag)
        .Func(_SC("CustomEvent"), &CPickup::CustomEvent)
        // Core Overloads
        .Overload< bool (CPickup::*)(void) >(_SC("Destroy"), &CPickup::Destroy)
        .Overload< bool (CPickup::*)(Int32) >(_SC("Destroy"), &CPickup::Destroy)
        .Overload< bool (CPickup::*)(Int32, LightObj &) >(_SC("Destroy"), &CPickup::Destroy)
        // Properties
        .Prop(_SC("Model"), &CPickup::GetModel)
        .Prop(_SC("World"), &CPickup::GetWorld, &CPickup::SetWorld)
        .Prop(_SC("Alpha"), &CPickup::GetAlpha, &CPickup::SetAlpha)
        .Prop(_SC("Auto"), &CPickup::GetAutomatic, &CPickup::SetAutomatic)
        .Prop(_SC("Automatic"), &CPickup::GetAutomatic, &CPickup::SetAutomatic)
        .Prop(_SC("Timer"), &CPickup::GetAutoTimer, &CPickup::SetAutoTimer)
        .Prop(_SC("Autotimer"), &CPickup::GetAutoTimer, &CPickup::SetAutoTimer)
        .Prop(_SC("Pos"), &CPickup::GetPosition, &CPickup::SetPosition)
        .Prop(_SC("Position"), &CPickup::GetPosition, &CPickup::SetPosition)
        .Prop(_SC("Quantity"), &CPickup::GetQuantity)
        .Prop(_SC("PosX"), &CPickup::GetPositionX, &CPickup::SetPositionX)
        .Prop(_SC("PosY"), &CPickup::GetPositionY, &CPickup::SetPositionY)
        .Prop(_SC("PosZ"), &CPickup::GetPositionZ, &CPickup::SetPositionZ)
        // Member Methods
        .Func(_SC("StreamedFor"), &CPickup::IsStreamedFor)
        .Func(_SC("Refresh"), &CPickup::Refresh)
        .Func(_SC("SetPos"), &CPickup::SetPositionEx)
        .Func(_SC("SetPosition"), &CPickup::SetPositionEx)
        // Static Overloads
        .StaticOverload< LightObj & (*)(Int32, Int32, Int32, Float32, Float32, Float32, Int32, bool) >
            (_SC("CreateEx"), &Pickup_CreateEx)
        .StaticOverload< LightObj & (*)(Int32, Int32, Int32, Float32, Float32, Float32, Int32, bool, Int32, LightObj &) >
            (_SC("CreateEx"), &Pickup_CreateEx)
        .StaticOverload< LightObj & (*)(Int32, Int32, Int32, const Vector3 &, Int32, bool) >
            (_SC("Create"), &Pickup_Create)
        .StaticOverload< LightObj & (*)(Int32, Int32, Int32, const Vector3 &, Int32, bool, Int32, LightObj &) >
            (_SC("Create"), &Pickup_Create)
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("NullInst"), &CPickup::SqGetNull)
        .SquirrelFunc(_SC("MakeTask"), &Tasks::MakeTask< CPickup, ENT_PICKUP >)
        .SquirrelFunc(_SC("DropTask"), &Tasks::DropTask< CPickup, ENT_PICKUP >)
        .SquirrelFunc(_SC("DoesTask"), &Tasks::DoesTask< CPickup, ENT_PICKUP >)
        .SquirrelFunc(_SC("FindTask"), &Tasks::FindTask< CPickup, ENT_PICKUP >)
    );
}

} // Namespace:: SqMod
