// ------------------------------------------------------------------------------------------------
#include "Entity/Checkpoint.hpp"
#include "Entity/Player.hpp"
#include "Base/Color4.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqCheckpoint"))

// ------------------------------------------------------------------------------------------------
Color4      CCheckpoint::s_Color4;
Vector3     CCheckpoint::s_Vector3;

// ------------------------------------------------------------------------------------------------
Int32      CCheckpoint::s_ColorR;
Int32      CCheckpoint::s_ColorG;
Int32      CCheckpoint::s_ColorB;
Int32      CCheckpoint::s_ColorA;

// ------------------------------------------------------------------------------------------------
const Int32 CCheckpoint::Max = SQMOD_CHECKPOINT_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CCheckpoint::SqGetNull(HSQUIRRELVM vm)
{
    sq_pushobject(vm, Core::Get().GetNullCheckpoint().GetObject());
    return 1;
}

// ------------------------------------------------------------------------------------------------
Object & CCheckpoint::GetNull()
{
    return Core::Get().GetNullCheckpoint();
}

// ------------------------------------------------------------------------------------------------
CCheckpoint::CCheckpoint(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_CHECKPOINT_POOL))
    , m_Tag(ToStrF("%d", id)), m_Data(), m_CircularLocks(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CCheckpoint::~CCheckpoint()
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
void CCheckpoint::SetTag(const StackStrF & tag)
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
CCheckpoint & CCheckpoint::ApplyTag(StackStrF & tag)
{
    SetTag(tag);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Object & CCheckpoint::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetData(Object & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CCheckpoint::Destroy(Int32 header, Object & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return Core::Get().DelCheckpoint(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::BindEvent(Int32 evid, Object & env, Function & func) const
{
    // Validate the managed identifier
    Validate();
    // Obtain the function instance called for this event
    Function & event = Core::Get().GetCheckpointEvent(m_ID, evid);
    // Is the specified callback function null?
    if (func.IsNull())
    {
        event.ReleaseGently(); // Then release the current callback
    }
    // Does this function need a custom environment?
    else if (env.IsNull())
    {
        event = func;
    }
    // Assign the specified environment and function
    else
    {
        event = Function(env.GetVM(), env, func.GetFunc());
    }
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::CustomEvent(Int32 header, Object & payload) const
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
Int32 CCheckpoint::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetCheckPointWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetWorld(Int32 world)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const Int32 current = _Func->GetCheckPointWorld(m_ID);
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
const Color4 & CCheckpoint::GetColor() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    s_ColorR = s_ColorG = s_ColorB = s_ColorA = 0;
    // Query the server for the color values
    _Func->GetCheckPointColour(m_ID, &s_ColorR, &s_ColorG, &s_ColorB, &s_ColorA);
    // Convert and assign the retrieved values
    s_Color4.SetColor4Ex(s_ColorR, s_ColorG, s_ColorB, s_ColorA);
    // Return the requested information
    return s_Color4;
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
void CCheckpoint::SetColorEx(Uint8 r, Uint8 g, Uint8 b) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, r, g, b, 0xFF);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, r, g, b, a);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CCheckpoint::GetPosition() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.Clear();
    // Query the server for the position values
    _Func->GetCheckPointPosition(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
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
void CCheckpoint::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetCheckPointPosition(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
Float32 CCheckpoint::GetRadius() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetCheckPointRadius(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetRadius(Float32 radius)
{
    // Validate the managed identifier
    Validate();
    // Grab the current value for this property
    const Float32 current = _Func->GetCheckPointRadius(m_ID);
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
Object & CCheckpoint::GetOwner() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return Core::Get().GetPlayer(_Func->GetCheckPointOwner(m_ID)).mObj;
}

// ------------------------------------------------------------------------------------------------
Int32 CCheckpoint::GetOwnerID() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetCheckPointOwner(m_ID);
}

// ------------------------------------------------------------------------------------------------
Float32 CCheckpoint::GetPositionX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.x = 0;
    // Query the server for the requested component value
    _Func->GetCheckPointPosition(m_ID, &s_Vector3.x, nullptr, nullptr);
    // Return the requested information
    return s_Vector3.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CCheckpoint::GetPositionY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.y = 0;
    // Query the server for the requested component value
    _Func->GetCheckPointPosition(m_ID, nullptr, &s_Vector3.y, nullptr);
    // Return the requested information
    return s_Vector3.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CCheckpoint::GetPositionZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.z = 0;
    // Query the server for the requested component value
    _Func->GetCheckPointPosition(m_ID, nullptr, nullptr, &s_Vector3.z);
    // Return the requested information
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPositionX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointPosition(m_ID, nullptr, &s_Vector3.y, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetCheckPointPosition(m_ID, x, s_Vector3.y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPositionY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointPosition(m_ID, &s_Vector3.x, nullptr, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetCheckPointPosition(m_ID, s_Vector3.x, y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPositionZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointPosition(m_ID, &s_Vector3.x, &s_Vector3.y, nullptr);
    // Perform the requested operation
    _Func->SetCheckPointPosition(m_ID, s_Vector3.z, s_Vector3.y, z);
}

// ------------------------------------------------------------------------------------------------
Int32 CCheckpoint::GetColorR() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    s_ColorR = 0;
    // Query the server for the requested component value
    _Func->GetCheckPointColour(m_ID, &s_ColorR, NULL, NULL, NULL);
    // Return the requested information
    return s_ColorR;
}

// ------------------------------------------------------------------------------------------------
Int32 CCheckpoint::GetColorG() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    s_ColorG = 0;
    // Query the server for the requested component value
    _Func->GetCheckPointColour(m_ID, NULL, &s_ColorG, NULL, NULL);
    // Return the requested information
    return s_ColorG;
}

// ------------------------------------------------------------------------------------------------
Int32 CCheckpoint::GetColorB() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    s_ColorB = 0;
    // Query the server for the requested component value
    _Func->GetCheckPointColour(m_ID, NULL, NULL, &s_ColorB, NULL);
    // Return the requested information
    return s_ColorB;
}

// ------------------------------------------------------------------------------------------------
Int32 CCheckpoint::GetColorA() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    s_ColorA = 0;
    // Query the server for the requested component value
    _Func->GetCheckPointColour(m_ID, NULL, NULL, NULL, &s_ColorA);
    // Return the requested information
    return s_ColorA;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorR(Int32 r) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointColour(m_ID, NULL, &s_ColorG, &s_ColorB, &s_ColorA);
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, r, s_ColorG, s_ColorB, s_ColorA);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorG(Int32 g) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointColour(m_ID, &s_ColorR, NULL, &s_ColorB, &s_ColorA);
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, s_ColorR, g, s_ColorB, s_ColorA);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorB(Int32 b) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointColour(m_ID, &s_ColorB, &s_ColorG, NULL, &s_ColorA);
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, s_ColorB, s_ColorG, b, s_ColorA);
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorA(Int32 a) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetCheckPointColour(m_ID, &s_ColorA, &s_ColorG, &s_ColorB, NULL);
    // Perform the requested operation
    _Func->SetCheckPointColour(m_ID, s_ColorA, s_ColorG, s_ColorB, a);
}

// ------------------------------------------------------------------------------------------------
static Object & Checkpoint_CreateEx(Int32 world, bool sphere, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius)
{
    return Core::Get().NewCheckpoint(-1, world, sphere, x, y, z, r, g, b, a, radius,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Checkpoint_CreateEx(Int32 world, bool sphere, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius,
                            Int32 header, Object & payload)
{
    return Core::Get().NewCheckpoint(-1, world, sphere, x, y, z, r, g, b, a,
                                radius, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Checkpoint_Create(Int32 world, bool sphere, const Vector3 & pos,
                            const Color4 & color, Float32 radius)
{
    return Core::Get().NewCheckpoint(-1, world, sphere, pos.x, pos.y, pos.z,
                                color.r, color.g, color.b, color.a, radius,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Checkpoint_Create(Int32 world, bool sphere, const Vector3 & pos,
                            const Color4 & color, Float32 radius, Int32 header, Object & payload)
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
        .Prop(_SC("ID"), &CCheckpoint::GetID)
        .Prop(_SC("Tag"), &CCheckpoint::GetTag, &CCheckpoint::SetTag)
        .Prop(_SC("Data"), &CCheckpoint::GetData, &CCheckpoint::SetData)
        .Prop(_SC("Active"), &CCheckpoint::IsActive)
        // Core Methods
        .Func(_SC("Bind"), &CCheckpoint::BindEvent)
        .FmtFunc(_SC("SetTag"), &CCheckpoint::ApplyTag)
        .Func(_SC("CustomEvent"), &CCheckpoint::CustomEvent)
        // Core Overloads
        .Overload< bool (CCheckpoint::*)(void) >(_SC("Destroy"), &CCheckpoint::Destroy)
        .Overload< bool (CCheckpoint::*)(Int32) >(_SC("Destroy"), &CCheckpoint::Destroy)
        .Overload< bool (CCheckpoint::*)(Int32, Object &) >(_SC("Destroy"), &CCheckpoint::Destroy)
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
        .Overload< void (CCheckpoint::*)(Uint8, Uint8, Uint8) const >
            (_SC("SetColor"), &CCheckpoint::SetColorEx)
        .Overload< void (CCheckpoint::*)(Uint8, Uint8, Uint8, Uint8) const >
            (_SC("SetColor"), &CCheckpoint::SetColorEx)
        // Static Overloads
        .StaticOverload< Object & (*)(Int32, bool, Float32, Float32, Float32, Uint8, Uint8, Uint8, Uint8, Float32) >
            (_SC("CreateEx"), &Checkpoint_CreateEx)
        .StaticOverload< Object & (*)(Int32, bool, Float32, Float32, Float32, Uint8, Uint8, Uint8, Uint8, Float32, Int32, Object &) >
            (_SC("CreateEx"), &Checkpoint_CreateEx)
        .StaticOverload< Object & (*)(Int32, bool, const Vector3 &, const Color4 &, Float32) >
            (_SC("Create"), &Checkpoint_Create)
        .StaticOverload< Object & (*)(Int32, bool, const Vector3 &, const Color4 &, Float32, Int32, Object &) >
            (_SC("Create"), &Checkpoint_Create)
        // Raw Squirrel Methods
        .SquirrelFunc(_SC("NullInst"), &CCheckpoint::SqGetNull)
    );
}

} // Namespace:: SqMod
