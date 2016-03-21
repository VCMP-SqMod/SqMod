// ------------------------------------------------------------------------------------------------
#include "Entity/Forcefield.hpp"
#include "Entity/Player.hpp"
#include "Base/Color3.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Color3      CForcefield::s_Color3;
Vector3     CForcefield::s_Vector3;

// ------------------------------------------------------------------------------------------------
Uint32      CForcefield::s_ColorR;
Uint32      CForcefield::s_ColorG;
Uint32      CForcefield::s_ColorB;

// ------------------------------------------------------------------------------------------------
const Int32 CForcefield::Max = SQMOD_FORCEFIELD_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CForcefield::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqForcefield");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
CForcefield::CForcefield(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_FORCEFIELD_POOL))
    , m_Tag(ToStrF("%d", id))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CForcefield::~CForcefield()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 CForcefield::Cmp(const CForcefield & o) const
{
    if (m_ID == o.m_ID)
        return 0;
    else if (m_ID > o.m_ID)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
const String & CForcefield::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CForcefield::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
Object & CForcefield::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetData(Object & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CForcefield::Destroy(Int32 header, Object & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return _Core->DelForcefield(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
void CForcefield::BindEvent(Int32 evid, Object & env, Function & func) const
{
    // Validate the managed identifier
    Validate();
    // Obtain the function instance called for this event
    Function & event = _Core->GetForcefieldEvent(m_ID, evid);
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
bool CForcefield::IsStreamedFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->IsSphereStreamedForPlayer(m_ID, player.GetID());
}

// ------------------------------------------------------------------------------------------------
Int32 CForcefield::GetWorld() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetSphereWorld(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetWorld(Int32 world) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSphereWorld(m_ID, world);
}

// ------------------------------------------------------------------------------------------------
const Color3 & CForcefield::GetColor() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    s_Color3.Clear();
    // Query the server for the color values
    _Func->GetSphereColor(m_ID, &s_ColorR, &s_ColorG, &s_ColorB);
    // Convert and assign the retrieved values
    s_Color3.Set(s_ColorR, s_ColorG, s_ColorB);
    // Return the requested information
    return s_Color3;
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetColor(const Color3 & col) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSphereColor(m_ID, col.r, col.g, col.b);
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetColorEx(Uint8 r, Uint8 g, Uint8 b) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSphereColor(m_ID, r, g, b);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CForcefield::GetPosition() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.Clear();
    // Query the server for the position values
    _Func->GetSpherePos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    // Return the requested information
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetPosition(const Vector3 & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSpherePos(m_ID, pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSpherePos(m_ID, x, y, z);
}

// ------------------------------------------------------------------------------------------------
Float32 CForcefield::GetRadius() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetSphereRadius(m_ID);
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetRadius(Float32 radius) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSphereRadius(m_ID, radius);
}

// ------------------------------------------------------------------------------------------------
Object & CForcefield::GetOwner() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetPlayer(_Func->GetSphereOwner(m_ID)).mObj;
}

// ------------------------------------------------------------------------------------------------
Int32 CForcefield::GetOwnerID() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Func->GetSphereOwner(m_ID);
}

// ------------------------------------------------------------------------------------------------
Float32 CForcefield::GetPosX() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.x = 0;
    // Query the server for the requested component value
    _Func->GetSpherePos(m_ID, &s_Vector3.x, NULL, NULL);
    // Return the requested information
    return s_Vector3.x;
}

// ------------------------------------------------------------------------------------------------
Float32 CForcefield::GetPosY() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.y = 0;
    // Query the server for the requested component value
    _Func->GetSpherePos(m_ID, NULL, &s_Vector3.y, NULL);
    // Return the requested information
    return s_Vector3.y;
}

// ------------------------------------------------------------------------------------------------
Float32 CForcefield::GetPosZ() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous position information, if any
    s_Vector3.z = 0;
    // Query the server for the requested component value
    _Func->GetSpherePos(m_ID, NULL, NULL, &s_Vector3.z);
    // Return the requested information
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetPosX(Float32 x) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetSpherePos(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetSpherePos(m_ID, x, s_Vector3.y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetPosY(Float32 y) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetSpherePos(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
    // Perform the requested operation
    _Func->SetSpherePos(m_ID, s_Vector3.x, y, s_Vector3.z);
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetPosZ(Float32 z) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetSpherePos(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
    // Perform the requested operation
    _Func->SetSpherePos(m_ID, s_Vector3.z, s_Vector3.y, z);
}

// ------------------------------------------------------------------------------------------------
Uint32 CForcefield::GetColR() const
{
    // Validate the managed identifier
    Validate();
    // Clear previous color information, if any
    s_ColorR = 0;
    // Query the server for the requested component value
    _Func->GetSphereColor(m_ID, &s_ColorR, NULL, NULL);
    // Return the requested information
    return s_ColorR;
}

// ------------------------------------------------------------------------------------------------
Uint32 CForcefield::GetColG() const
{
    // Validate the managed identifier
    Validate();
    // Query the server for the requested component value
    s_ColorG = 0;
    // Query the server for the requested component value
    _Func->GetSphereColor(m_ID, NULL, &s_ColorG, NULL);
    // Return the requested information
    return s_ColorG;
}

// ------------------------------------------------------------------------------------------------
Uint32 CForcefield::GetColB() const
{
    // Validate the managed identifier
    Validate();
    // Query the server for the requested component value
    s_ColorB = 0;
    // Query the server for the requested component value
    _Func->GetSphereColor(m_ID, NULL, NULL, &s_ColorB);
    // Return the requested information
    return s_ColorB;
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetColR(Uint32 r) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetSphereColor(m_ID, NULL, &s_ColorG, &s_ColorB);
    // Perform the requested operation
    _Func->SetSphereColor(m_ID, r, s_ColorG, s_ColorB);
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetColG(Uint32 g) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetSphereColor(m_ID, &s_ColorR, NULL, &s_ColorB);
    // Perform the requested operation
    _Func->SetSphereColor(m_ID, s_ColorR, g, s_ColorB);
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetColB(Uint32 b) const
{
    // Validate the managed identifier
    Validate();
    // Retrieve the current values for unchanged components
    _Func->GetSphereColor(m_ID, &s_ColorB, &s_ColorG, NULL);
    // Perform the requested operation
    _Func->SetSphereColor(m_ID, s_ColorB, s_ColorG, b);
}

// ------------------------------------------------------------------------------------------------
static Object & Forcefield_CreateEx(CPlayer & player, Int32 world, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Float32 radius)
{
    return _Core->NewForcefield(player.GetID(), world, x, y, z, r, g, b, radius,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Forcefield_CreateEx(CPlayer & player, Int32 world, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Float32 radius,
                            Int32 header, Object & payload)
{
    return _Core->NewForcefield(player.GetID(), world, x, y, z, r, g, b, radius, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Forcefield_Create(CPlayer & player, Int32 world, const Vector3 & pos,
                            const Color3 & color, Float32 radius)
{
    return _Core->NewForcefield(player.GetID(), world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & Forcefield_Create(CPlayer & player, Int32 world, const Vector3 & pos,
                            const Color3 & color, Float32 radius, Int32 header, Object & payload)
{
    return _Core->NewForcefield(player.GetID(), world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
static const Object & Forcefield_FindByID(Int32 id)
{
    // Perform a range check on the specified identifier
    if (INVALID_ENTITYEX(id, SQMOD_FORCEFIELD_POOL))
    {
        STHROWF("The specified forcefield identifier is invalid: %d", id);
    }
    // Obtain the ends of the entity pool
    Core::Forcefields::const_iterator itr = _Core->GetForcefields().cbegin();
    Core::Forcefields::const_iterator end = _Core->GetForcefields().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mID == id)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a forcefield matching the specified identifier
    return NullObject();
}

static const Object & Forcefield_FindByTag(CSStr tag)
{
    // Perform a validity check on the specified tag
    if (!tag || *tag == '\0')
    {
        STHROWF("The specified forcefield tag is invalid: null/empty");
    }
    // Obtain the ends of the entity pool
    Core::Forcefields::const_iterator itr = _Core->GetForcefields().cbegin();
    Core::Forcefields::const_iterator end = _Core->GetForcefields().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does this entity even exist and does the tag match the specified one?
        if (itr->mInst != nullptr && itr->mInst->GetTag().compare(tag) == 0)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a forcefield matching the specified tag
    return NullObject();
}

// ================================================================================================
void Register_CForcefield(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqForcefield"),
        Class< CForcefield, NoConstructor< CForcefield > >(vm, _SC("SqForcefield"))
        // Metamethods
        .Func(_SC("_cmp"), &CForcefield::Cmp)
        .SquirrelFunc(_SC("_typename"), &CForcefield::Typename)
        .Func(_SC("_tostring"), &CForcefield::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CForcefield::Max)
        // Core Properties
        .Prop(_SC("ID"), &CForcefield::GetID)
        .Prop(_SC("Tag"), &CForcefield::GetTag, &CForcefield::SetTag)
        .Prop(_SC("Data"), &CForcefield::GetData, &CForcefield::SetData)
        .Prop(_SC("Active"), &CForcefield::IsActive)
        // Core Methods
        .Func(_SC("Bind"), &CForcefield::BindEvent)
        // Core Overloads
        .Overload< bool (CForcefield::*)(void) >(_SC("Destroy"), &CForcefield::Destroy)
        .Overload< bool (CForcefield::*)(Int32) >(_SC("Destroy"), &CForcefield::Destroy)
        .Overload< bool (CForcefield::*)(Int32, Object &) >(_SC("Destroy"), &CForcefield::Destroy)
        // Properties
        .Prop(_SC("World"), &CForcefield::GetWorld, &CForcefield::SetWorld)
        .Prop(_SC("Color"), &CForcefield::GetColor, &CForcefield::SetColor)
        .Prop(_SC("Pos"), &CForcefield::GetPosition, &CForcefield::SetPosition)
        .Prop(_SC("Position"), &CForcefield::GetPosition, &CForcefield::SetPosition)
        .Prop(_SC("Radius"), &CForcefield::GetRadius, &CForcefield::SetRadius)
        .Prop(_SC("Owner"), &CForcefield::GetOwner)
        .Prop(_SC("OwnerID"), &CForcefield::GetOwnerID)
        .Prop(_SC("X"), &CForcefield::GetPosX, &CForcefield::SetPosX)
        .Prop(_SC("Y"), &CForcefield::GetPosY, &CForcefield::SetPosY)
        .Prop(_SC("Z"), &CForcefield::GetPosZ, &CForcefield::SetPosZ)
        .Prop(_SC("R"), &CForcefield::GetColR, &CForcefield::SetColR)
        .Prop(_SC("G"), &CForcefield::GetColG, &CForcefield::SetColG)
        .Prop(_SC("B"), &CForcefield::GetColB, &CForcefield::SetColB)
        // Member Methods
        .Func(_SC("StreamedFor"), &CForcefield::IsStreamedFor)
        .Func(_SC("SetColor"), &CForcefield::SetColorEx)
        .Func(_SC("SetPos"), &CForcefield::SetPositionEx)
        .Func(_SC("SetPosition"), &CForcefield::SetPositionEx)
        // Static Functions
        .StaticFunc(_SC("FindByID"), &Forcefield_FindByID)
        .StaticFunc(_SC("FindByTag"), &Forcefield_FindByTag)
        // Static Overloads
        .StaticOverload< Object & (*)(CPlayer &, Int32, Float32, Float32, Float32, Uint8, Uint8, Uint8, Float32) >
            (_SC("CreateEx"), &Forcefield_CreateEx)
        .StaticOverload< Object & (*)(CPlayer &, Int32, Float32, Float32, Float32, Uint8, Uint8, Uint8, Float32, Int32, Object &) >
            (_SC("CreateEx"), &Forcefield_CreateEx)
        .StaticOverload< Object & (*)(CPlayer &, Int32, const Vector3 &, const Color3 &, Float32) >
            (_SC("Create"), &Forcefield_Create)
        .StaticOverload< Object & (*)(CPlayer &, Int32, const Vector3 &, const Color3 &, Float32, Int32, Object &) >
            (_SC("Create"), &Forcefield_Create)
    );
}

} // Namespace:: SqMod
