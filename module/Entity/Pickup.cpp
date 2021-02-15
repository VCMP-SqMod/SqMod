// ------------------------------------------------------------------------------------------------
#include "Entity/Pickup.hpp"
#include "Entity/Player.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"
#include "Core/Tasks.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqPickup"))

// ------------------------------------------------------------------------------------------------
const int32_t CPickup::Max = SQMOD_PICKUP_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CPickup::SqGetNull(HSQUIRRELVM vm)
{
    sq_pushobject(vm, Core::Get().GetNullPickup().GetObj());
    return 1;
}

// ------------------------------------------------------------------------------------------------
LightObj & CPickup::GetNull()
{
    return Core::Get().GetNullPickup();
}

// ------------------------------------------------------------------------------------------------
CPickup::CPickup(int32_t id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_PICKUP_POOL))
    , m_Tag(fmt::format("{}", id)), m_Data(), m_CircularLocks(0)
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
void CPickup::SetTag(StackStrF & tag)
{
    if (tag.mLen > 0)
    {
        m_Tag.assign(tag.mPtr, static_cast< size_t >(tag.mLen));
    }
    else
    {
        m_Tag.clear();
    }
}

// ------------------------------------------------------------------------------------------------
CPickup & CPickup::ApplyTag(StackStrF & tag)
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
bool CPickup::Destroy(int32_t header, LightObj & payload) const
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
void CPickup::CustomEvent(int32_t header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested action
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
bool CPickup::GetOption(int32_t option_id) const
{
    // Attempt to obtain the current value of the specified option
    const bool value = _Func->GetPickupOption(m_ID, static_cast< vcmpPickupOption >(option_id));
    // Check for errors
    if (_Func->GetLastError() == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid option identifier: {}", option_id);
    }
    // Return the requested value
    return value;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetOption(int32_t option_id, bool toggle)
{
    // Attempt to obtain the current value of the specified option
    const bool value = _Func->GetPickupOption(m_ID, static_cast< vcmpPickupOption >(option_id));
    // Attempt to modify the current value of the specified option
    if (_Func->SetPickupOption(m_ID, static_cast< vcmpPickupOption >(option_id),
                               static_cast< uint8_t >(toggle)) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid option identifier: {}", option_id);
    }
    else if (!(m_CircularLocks & PICKUPCL_EMIT_PICKUP_OPTION))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, PICKUPCL_EMIT_PICKUP_OPTION);
        // Now forward the event call
        Core::Get().EmitPickupOption(m_ID, option_id, value, 0, NullLightObj());
    }
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetOptionEx(int32_t option_id, bool toggle, int32_t header, LightObj & payload)
{
    // Attempt to obtain the current value of the specified option
    const bool value = _Func->GetPickupOption(m_ID, static_cast< vcmpPickupOption >(option_id));
    // Attempt to modify the current value of the specified option
    if (_Func->SetPickupOption(m_ID, static_cast< vcmpPickupOption >(option_id),
                               static_cast< uint8_t >(toggle)) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Invalid option identifier: {}", option_id);
    }
    else if (!(m_CircularLocks & PICKUPCL_EMIT_PICKUP_OPTION))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, PICKUPCL_EMIT_PICKUP_OPTION);
        // Now forward the event call
        Core::Get().EmitPickupOption(m_ID, option_id, value, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
int32_t CPickup::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetWorld(int32_t world)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const int32_t current = _Func->GetPickupWorld(m_ID);
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
int32_t CPickup::GetAlpha() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupAlpha(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetAlpha(int32_t alpha)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const int32_t current = _Func->GetPickupAlpha(m_ID);
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
    _Func->SetPickupIsAutomatic(m_ID, static_cast< uint8_t >(toggle));
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
int32_t CPickup::GetAutoTimer() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupAutoTimer(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetAutoTimer(int32_t timer)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const int32_t current = _Func->GetPickupAutoTimer(m_ID);
    // Don't even bother if it's the same value
    if (current == timer)
    {
        return;
    }
    // Avoid property unwind from a recursive call
    _Func->SetPickupAutoTimer(m_ID, static_cast< uint32_t >(timer));
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
Vector3 CPickup::GetPosition() const
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
void CPickup::SetPositionEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
int32_t CPickup::GetModel() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupModel(m_ID);
}

// ------------------------------------------------------------------------------------------------
int32_t CPickup::GetQuantity() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetPickupQuantity(m_ID);
}

// ------------------------------------------------------------------------------------------------
float CPickup::GetPositionX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetPickupPosition(m_ID, &x, &dummy, &dummy);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CPickup::GetPositionY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetPickupPosition(m_ID, &dummy, &y, &dummy);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CPickup::GetPositionZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetPickupPosition(m_ID, &dummy, &dummy, &z);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetPickupPosition(m_ID, &dummy, &y, &z);
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetPickupPosition(m_ID, &x, &dummy, &z);
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetPickupPosition(m_ID, &x, &y, &dummy);
    // Perform the requested operation
    _Func->SetPickupPosition(m_ID, z, y, z);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Pickup_CreateEx1a(int32_t model, int32_t world, int32_t quantity,
                        float x, float y, float z, int32_t alpha, bool automatic)
{
    return Core::Get().NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Pickup_CreateEx1b(int32_t model, int32_t world, int32_t quantity,
                        float x, float y, float z, int32_t alpha, bool automatic,
                        int32_t header, LightObj & payload)
{
    return Core::Get().NewPickup(model, world, quantity, x, y, z, alpha, automatic, header, payload);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Pickup_Create1a(int32_t model, int32_t world, int32_t quantity, const Vector3 & pos,
                            int32_t alpha, bool automatic)
{
    return Core::Get().NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Pickup_Create1b(int32_t model, int32_t world, int32_t quantity, const Vector3 & pos,
                            int32_t alpha, bool automatic, int32_t header, LightObj & payload)
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
        .Overload(_SC("Destroy"), &CPickup::Destroy0)
        .Overload(_SC("Destroy"), &CPickup::Destroy1)
        .Overload(_SC("Destroy"), &CPickup::Destroy)
        // Properties
        .Prop(_SC("Model"), &CPickup::GetModel)
        .Prop(_SC("World"), &CPickup::GetWorld, &CPickup::SetWorld)
        .Prop(_SC("Alpha"), &CPickup::GetAlpha, &CPickup::SetAlpha)
        .Prop(_SC("Auto"), &CPickup::GetAutomatic, &CPickup::SetAutomatic)
        .Prop(_SC("Automatic"), &CPickup::GetAutomatic, &CPickup::SetAutomatic)
        .Prop(_SC("Timer"), &CPickup::GetAutoTimer, &CPickup::SetAutoTimer)
        .Prop(_SC("AutoTimer"), &CPickup::GetAutoTimer, &CPickup::SetAutoTimer)
        .Prop(_SC("Pos"), &CPickup::GetPosition, &CPickup::SetPosition)
        .Prop(_SC("Position"), &CPickup::GetPosition, &CPickup::SetPosition)
        .Prop(_SC("Quantity"), &CPickup::GetQuantity)
        .Prop(_SC("PosX"), &CPickup::GetPositionX, &CPickup::SetPositionX)
        .Prop(_SC("PosY"), &CPickup::GetPositionY, &CPickup::SetPositionY)
        .Prop(_SC("PosZ"), &CPickup::GetPositionZ, &CPickup::SetPositionZ)
        // Member Methods
        .Func(_SC("StreamedFor"), &CPickup::IsStreamedFor)
        .Func(_SC("GetOption"), &CPickup::GetOption)
        .Func(_SC("SetOption"), &CPickup::SetOption)
        .Func(_SC("SetOptionEx"), &CPickup::SetOptionEx)
        .Func(_SC("Refresh"), &CPickup::Refresh)
        .Func(_SC("SetPos"), &CPickup::SetPositionEx)
        .Func(_SC("SetPosition"), &CPickup::SetPositionEx)
        // Static Overloads
        .StaticOverload(_SC("CreateEx"), &Pickup_CreateEx1a)
        .StaticOverload(_SC("CreateEx"), &Pickup_CreateEx1b)
        .StaticOverload(_SC("Create"), &Pickup_Create1a)
        .StaticOverload(_SC("Create"), &Pickup_Create1b)
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("NullInst"), &CPickup::SqGetNull)
        .SquirrelFunc(_SC("MakeTask"), &Tasks::MakeTask< CPickup, ENT_PICKUP >)
        .SquirrelFunc(_SC("DropTask"), &Tasks::DropTask< CPickup, ENT_PICKUP >)
        .SquirrelFunc(_SC("DoesTask"), &Tasks::DoesTask< CPickup, ENT_PICKUP >)
        .SquirrelFunc(_SC("FindTask"), &Tasks::FindTask< CPickup, ENT_PICKUP >)
    );
}

} // Namespace:: SqMod
