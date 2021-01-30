// ------------------------------------------------------------------------------------------------
#include "Entity/Checkpoint.hpp"
#include "Entity/Player.hpp"
#include "Base/Color4.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"
#include "Core/Tasks.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("SqCheckpoint"))

// ------------------------------------------------------------------------------------------------
const int32_t CCheckpoint::Max = SQMOD_CHECKPOINT_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CCheckpoint::SqGetNull(HSQUIRRELVM vm)
{
    sq_pushobject(vm, Core::Get().GetNullCheckpoint().GetObj());
    return 1;
}

// ------------------------------------------------------------------------------------------------
LightObj & CCheckpoint::GetNull()
{
    return Core::Get().GetNullCheckpoint();
}

// ------------------------------------------------------------------------------------------------
CCheckpoint::CCheckpoint(int32_t id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_CHECKPOINT_POOL))
    , m_Tag(fmt::format("{}", id)), m_Data(), m_CircularLocks(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
const String & CCheckpoint::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CCheckpoint::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetTag(StackStrF & tag)
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
CCheckpoint & CCheckpoint::ApplyTag(StackStrF & tag)
{
    SetTag(tag);
    return *this;
}

// ------------------------------------------------------------------------------------------------
LightObj & CCheckpoint::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetData(LightObj & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CCheckpoint::Destroy(int32_t header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return Core::Get().DelCheckpoint(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
LightObj & CCheckpoint::GetEvents() const
{
    // Validate the managed identifier
    Validate();
    // Return the associated event table
    return Core::Get().GetCheckpoint(m_ID).mEvents;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::CustomEvent(int32_t header, LightObj & payload) const
{
    // Validate the managed identifier
    Validate();
    // Perfrom the requested action
    Core::Get().EmitCheckpointCustom(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CCheckpoint::IsStreamedFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsCheckPointStreamedForPlayer(m_ID, player.GetID());
}

// ------------------------------------------------------------------------------------------------
bool CCheckpoint::IsSphere() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsCheckPointSphere(m_ID);
}

// ------------------------------------------------------------------------------------------------
int32_t CCheckpoint::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetCheckPointWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetWorld(int32_t world)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const int32_t current = _Func->GetCheckPointWorld(m_ID);
    // Don't even bother if it's the same value
    if (current == world)
    {
        return;
    }
    // Avoid property unwind from a recursive call
    _Func->SetCheckPointWorld(m_ID, world);
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & CHECKPOINTCL_EMIT_CHECKPOINT_WORLD))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, CHECKPOINTCL_EMIT_CHECKPOINT_WORLD);
        // Now forward the event call
        Core::Get().EmitCheckpointWorld(m_ID, current, world);
    }
}

// ------------------------------------------------------------------------------------------------
Color4 CCheckpoint::GetColor() const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the color information
    int32_t r, g, b, a;
    // Query the server for the color values
    _Func->GetCheckPointColour(m_ID, &r, &g, &b, &a);
    // Return the requested information
    return {ConvTo< Color4::Value >::From(r), ConvTo< Color4::Value >::From(g),
            ConvTo< Color4::Value >::From(b), ConvTo< Color4::Value >::From(a)};
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColor(const Color4 & col) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, col.r, col.g, col.b, col.a);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorEx3(uint8_t r, uint8_t g, uint8_t b) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, r, g, b, 0xFF);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorEx4(uint8_t r, uint8_t g, uint8_t b, uint8_t a) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, r, g, b, a);
}

// ------------------------------------------------------------------------------------------------
Vector3 CCheckpoint::GetPosition() const
{
    // Validate the managed identifier
    Validate();
    // Create a default vector instance
    Vector3 vec;
    // Query the server for the position values
    _Func->GetCheckPointPosition(m_ID, &vec.x, &vec.y, &vec.z);
    // Return the requested information
    return vec;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPosition(const Vector3 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetCheckPointPosition(m_ID, pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPositionEx(float x, float y, float z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetCheckPointPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
float CCheckpoint::GetRadius() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetCheckPointRadius(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetRadius(float radius)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const float current = _Func->GetCheckPointRadius(m_ID);
    // Avoid property unwind from a recursive call
    _Func->SetCheckPointRadius(m_ID, radius);
    // Avoid infinite recursive event loops
    if (!(m_CircularLocks & CHECKPOINTCL_EMIT_CHECKPOINT_RADIUS))
    {
        // Prevent this event from triggering while executed
        BitGuardU32 bg(m_CircularLocks, CHECKPOINTCL_EMIT_CHECKPOINT_RADIUS);
        // Now forward the event call
        Core::Get().EmitCheckpointRadius(m_ID, current, radius);
    }
}

// ------------------------------------------------------------------------------------------------
LightObj & CCheckpoint::GetOwner() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetPlayer(_Func->GetCheckPointOwner(m_ID)).mObj;
}

// ------------------------------------------------------------------------------------------------
int32_t CCheckpoint::GetOwnerID() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetCheckPointOwner(m_ID);
}

// ------------------------------------------------------------------------------------------------
float CCheckpoint::GetPositionX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    float x = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetCheckPointPosition(m_ID, &x, &dummy, &dummy);
    // Return the requested information
    return x;
}

// ------------------------------------------------------------------------------------------------
float CCheckpoint::GetPositionY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    float y = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetCheckPointPosition(m_ID, &dummy, &y, &dummy);
    // Return the requested information
    return y;
}

// ------------------------------------------------------------------------------------------------
float CCheckpoint::GetPositionZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    float z = 0.0f, dummy;
    // Query the server for the requested component value
    _Func->GetCheckPointPosition(m_ID, &dummy, &dummy, &z);
    // Return the requested information
    return z;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPositionX(float x) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float y, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointPosition(m_ID, &dummy, &y, &z);
    // Perform the requested operation
    _Func->SetCheckPointPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPositionY(float y) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, z, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointPosition(m_ID, &x, &dummy, &z);
    // Perform the requested operation
    _Func->SetCheckPointPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPositionZ(float z) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary floats to retrieve the missing components
    float x, y, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointPosition(m_ID, &x, &y, &dummy);
    // Perform the requested operation
    _Func->SetCheckPointPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
int32_t CCheckpoint::GetColorR() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    int32_t r = 0, dummy;
    // Query the server for the requested component value
    _Func->GetCheckPointColour(m_ID, &r, &dummy, &dummy, &dummy);
    // Return the requested information
    return r;
}

// ------------------------------------------------------------------------------------------------
int32_t CCheckpoint::GetColorG() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    int32_t g = 0, dummy;
    // Query the server for the requested component value
    _Func->GetCheckPointColour(m_ID, &dummy, &g, &dummy, &dummy);
    // Return the requested information
    return g;
}

// ------------------------------------------------------------------------------------------------
int32_t CCheckpoint::GetColorB() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    int32_t b = 0, dummy;
    // Query the server for the requested component value
    _Func->GetCheckPointColour(m_ID, &dummy, &dummy, &b, &dummy);
    // Return the requested information
    return b;
}

// ------------------------------------------------------------------------------------------------
int32_t CCheckpoint::GetColorA() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    int32_t a = 0, dummy;
    // Query the server for the requested component value
    _Func->GetCheckPointColour(m_ID, &dummy, &dummy, &dummy, &a);
    // Return the requested information
    return a;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorR(int32_t r) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary integers to retrieve the missing components
    int32_t g, b, a, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointColour(m_ID, &dummy, &g, &b, &a);
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, r, g, b, a);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorG(int32_t g) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary integers to retrieve the missing components
    int32_t r, b, a, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointColour(m_ID, &r, &dummy, &b, &a);
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, r, g, b, a);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorB(int32_t b) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary integers to retrieve the missing components
    int32_t r, g, a, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointColour(m_ID, &r, &g, &dummy, &a);
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, r, g, b, a);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorA(int32_t a) const
{
    // Validate the managed identifier
    Validate();
    // Reserve some temporary integers to retrieve the missing components
    int32_t r, g, b, dummy;
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointColour(m_ID, &r, &g, &b, &dummy);
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, r, g, b, a);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Checkpoint_CreateEx1a(int32_t world, bool sphere, float x, float y, float z,
                            uint8_t r, uint8_t g, uint8_t b, uint8_t a, float radius)
{
    return Core::Get().NewCheckpoint(-1, world, sphere, x, y, z, r, g, b, a, radius,
                                SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Checkpoint_CreateEx1b(int32_t world, bool sphere, float x, float y, float z,
                            uint8_t r, uint8_t g, uint8_t b, uint8_t a, float radius,
                            int32_t header, LightObj & payload)
{
    return Core::Get().NewCheckpoint(-1, world, sphere, x, y, z, r, g, b, a,
                                radius, header, payload);
}

// ------------------------------------------------------------------------------------------------
static LightObj & Checkpoint_Create1a(int32_t world, bool sphere, const Vector3 & pos,
                            const Color4 & color, float radius)
{
    return Core::Get().NewCheckpoint(-1, world, sphere, pos.x, pos.y, pos.z,
                                color.r, color.g, color.b, color.a, radius,
                                SQMOD_CREATE_DEFAULT, NullLightObj());
}

static LightObj & Checkpoint_Create1b(int32_t world, bool sphere, const Vector3 & pos,
                            const Color4 & color, float radius, int32_t header, LightObj & payload)
{
    return Core::Get().NewCheckpoint(-1, world, sphere, pos.x, pos.y, pos.z,
                                color.r, color.g, color.b, color.a, radius, header, payload);
}

// ================================================================================================
void Register_CCheckpoint(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(Typename::Str,
        Class< CCheckpoint, NoConstructor< CCheckpoint > >(vm, Typename::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &CCheckpoint::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CCheckpoint::Max)
        // Core Properties
        .Prop(_SC("On"), &CCheckpoint::GetEvents)
        .Prop(_SC("ID"), &CCheckpoint::GetID)
        .Prop(_SC("Tag"), &CCheckpoint::GetTag, &CCheckpoint::SetTag)
        .Prop(_SC("Data"), &CCheckpoint::GetData, &CCheckpoint::SetData)
        .Prop(_SC("Active"), &CCheckpoint::IsActive)
        // Core Methods
        .FmtFunc(_SC("SetTag"), &CCheckpoint::ApplyTag)
        .Func(_SC("CustomEvent"), &CCheckpoint::CustomEvent)
        // Core Overloads
        .Overload(_SC("Destroy"), &CCheckpoint::Destroy0)
        .Overload(_SC("Destroy"), &CCheckpoint::Destroy1)
        .Overload(_SC("Destroy"), &CCheckpoint::Destroy)
        // Properties
        .Prop(_SC("Sphere"), &CCheckpoint::IsSphere)
        .Prop(_SC("World"), &CCheckpoint::GetWorld, &CCheckpoint::SetWorld)
        .Prop(_SC("Color"), &CCheckpoint::GetColor, &CCheckpoint::SetColor)
        .Prop(_SC("Colour"), &CCheckpoint::GetColor, &CCheckpoint::SetColor)
        .Prop(_SC("Pos"), &CCheckpoint::GetPosition, &CCheckpoint::SetPosition)
        .Prop(_SC("Position"), &CCheckpoint::GetPosition, &CCheckpoint::SetPosition)
        .Prop(_SC("Radius"), &CCheckpoint::GetRadius, &CCheckpoint::SetRadius)
        .Prop(_SC("Owner"), &CCheckpoint::GetOwner)
        .Prop(_SC("OwnerID"), &CCheckpoint::GetOwnerID)
        .Prop(_SC("PosX"), &CCheckpoint::GetPositionX, &CCheckpoint::SetPositionX)
        .Prop(_SC("PosY"), &CCheckpoint::GetPositionY, &CCheckpoint::SetPositionY)
        .Prop(_SC("PosZ"), &CCheckpoint::GetPositionZ, &CCheckpoint::SetPositionZ)
        .Prop(_SC("Red"), &CCheckpoint::GetColorR, &CCheckpoint::SetColorR)
        .Prop(_SC("Green"), &CCheckpoint::GetColorG, &CCheckpoint::SetColorG)
        .Prop(_SC("Blue"), &CCheckpoint::GetColorB, &CCheckpoint::SetColorB)
        .Prop(_SC("Alpha"), &CCheckpoint::GetColorA, &CCheckpoint::SetColorA)
        // Member Methods
        .Func(_SC("StreamedFor"), &CCheckpoint::IsStreamedFor)
        .Func(_SC("SetPos"), &CCheckpoint::SetPositionEx)
        .Func(_SC("SetPosition"), &CCheckpoint::SetPositionEx)
        // Member Overloads
        .Overload(_SC("SetColor"), &CCheckpoint::SetColorEx3)
        .Overload(_SC("SetColor"), &CCheckpoint::SetColorEx4)
        // Static Overloads
        .StaticOverload(_SC("CreateEx"), &Checkpoint_CreateEx1a)
        .StaticOverload(_SC("CreateEx"), &Checkpoint_CreateEx1b)
        .StaticOverload(_SC("Create"), &Checkpoint_Create1a)
        .StaticOverload(_SC("Create"), &Checkpoint_Create1b)
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("NullInst"), &CCheckpoint::SqGetNull)
        .SquirrelFunc(_SC("MakeTask"), &Tasks::MakeTask< CCheckpoint, ENT_CHECKPOINT >)
        .SquirrelFunc(_SC("DropTask"), &Tasks::DropTask< CCheckpoint, ENT_CHECKPOINT >)
        .SquirrelFunc(_SC("DoesTask"), &Tasks::DoesTask< CCheckpoint, ENT_CHECKPOINT >)
        .SquirrelFunc(_SC("FindTask"), &Tasks::FindTask< CCheckpoint, ENT_CHECKPOINT >)
    );
}

} // Namespace:: SqMod
