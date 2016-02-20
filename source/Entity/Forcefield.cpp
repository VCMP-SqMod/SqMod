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
SQChar CForcefield::s_StrID[SQMOD_FORCEFIELD_POOL][8];

// ------------------------------------------------------------------------------------------------
const Int32 CForcefield::Max = SQMOD_FORCEFIELD_POOL;

// ------------------------------------------------------------------------------------------------
CForcefield::CForcefield(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_FORCEFIELD_POOL))
    , m_Tag(VALID_ENTITY(m_ID) ? s_StrID[m_ID] : _SC("-1"))
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

CSStr CForcefield::ToString() const
{
    return VALID_ENTITYEX(m_ID, SQMOD_FORCEFIELD_POOL) ? s_StrID[m_ID] : _SC("-1");
}

// ------------------------------------------------------------------------------------------------
CSStr CForcefield::GetTag() const
{
    return m_Tag.c_str();
}

void CForcefield::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

Object & CForcefield::GetData()
{
    if (Validate())
        return m_Data;
    return NullObject();
}

void CForcefield::SetData(Object & data)
{
    if (Validate())
        m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CForcefield::Destroy(Int32 header, Object & payload)
{
    return _Core->DelForcefield(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CForcefield::BindEvent(Int32 evid, Object & env, Function & func) const
{
    if (!Validate())
        return false;

    Function & event = _Core->GetForcefieldEvent(m_ID, evid);

    if (func.IsNull())
        event.Release();
    else
        event = Function(env.GetVM(), env, func.GetFunc());

    return true;
}

// ------------------------------------------------------------------------------------------------
bool CForcefield::IsStreamedFor(CPlayer & player) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        return _Func->IsSphereStreamedForPlayer(m_ID, player.GetID());
    return false;
}

Int32 CForcefield::GetWorld() const
{
    if (Validate())
        return _Func->GetSphereWorld(m_ID);
    return -1;
}

void CForcefield::SetWorld(Int32 world) const
{
    if (Validate())
        _Func->SetSphereWorld(m_ID, world);
}

const Color3 & CForcefield::GetColor() const
{
    s_Color3.Clear();
    if (Validate())
    {
        _Func->GetSphereColor(m_ID, &s_ColorR, &s_ColorG, &s_ColorB);
        s_Color3.Set(s_ColorR, s_ColorG, s_ColorB);
    }
    return s_Color3;
}

void CForcefield::SetColor(const Color3 & col) const
{
    if (Validate())
        _Func->SetSphereColor(m_ID, col.r, col.g, col.b);
}

void CForcefield::SetColorEx(Uint8 r, Uint8 g, Uint8 b) const
{
    if (Validate())
        _Func->SetSphereColor(m_ID, r, g, b);
}

const Vector3 & CForcefield::GetPosition() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetSpherePos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CForcefield::SetPosition(const Vector3 & pos) const
{
    if (Validate())
        _Func->SetSpherePos(m_ID, pos.x, pos.y, pos.z);
}

void CForcefield::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetSpherePos(m_ID, x, y, z);
}

Float32 CForcefield::GetRadius() const
{
    if (Validate())
        _Func->GetSphereRadius(m_ID);
    return 0;
}

void CForcefield::SetRadius(Float32 radius) const
{
    if (Validate())
        _Func->SetSphereRadius(m_ID, radius);
}

Object & CForcefield::GetOwner() const
{
    if (Validate())
        return _Core->GetPlayer(_Func->GetSphereOwner(m_ID)).mObj;
    return NullObject();
}

Int32 CForcefield::GetOwnerID() const
{
    if (Validate())
        _Func->GetSphereOwner(m_ID);
    return -1;
}

// ------------------------------------------------------------------------------------------------
Float32 CForcefield::GetPosX() const
{
    s_Vector3.x = 0;
    if (Validate())
        _Func->GetSpherePos(m_ID, &s_Vector3.x, NULL, NULL);
    return s_Vector3.x;
}

Float32 CForcefield::GetPosY() const
{
    s_Vector3.y = 0;
    if (Validate())
        _Func->GetSpherePos(m_ID, NULL, &s_Vector3.y, NULL);
    return s_Vector3.y;
}

Float32 CForcefield::GetPosZ() const
{
    s_Vector3.z = 0;
    if (Validate())
        _Func->GetSpherePos(m_ID, NULL, NULL, &s_Vector3.z);
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetPosX(Float32 x) const
{
    if (Validate())
    {
        _Func->GetSpherePos(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
        _Func->SetSpherePos(m_ID, x, s_Vector3.y, s_Vector3.z);
    }
}

void CForcefield::SetPosY(Float32 y) const
{
    if (Validate())
    {
        _Func->GetSpherePos(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
        _Func->SetSpherePos(m_ID, s_Vector3.x, y, s_Vector3.z);
    }
}

void CForcefield::SetPosZ(Float32 z) const
{
    if (Validate())
    {
        _Func->GetSpherePos(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
        _Func->SetSpherePos(m_ID, s_Vector3.z, s_Vector3.y, z);
    }
}

// ------------------------------------------------------------------------------------------------
Uint32 CForcefield::GetColR() const
{
    s_ColorR = 0;
    if (Validate())
        _Func->GetSphereColor(m_ID, &s_ColorR, NULL, NULL);
    return s_ColorR;
}

Uint32 CForcefield::GetColG() const
{
    s_ColorG = 0;
    if (Validate())
        _Func->GetSphereColor(m_ID, NULL, &s_ColorG, NULL);
    return s_ColorG;
}

Uint32 CForcefield::GetColB() const
{
    s_ColorB = 0;
    if (Validate())
        _Func->GetSphereColor(m_ID, NULL, NULL, &s_ColorB);
    return s_ColorB;
}

// ------------------------------------------------------------------------------------------------
void CForcefield::SetColR(Uint32 r) const
{
    if (Validate())
    {
        _Func->GetSphereColor(m_ID, NULL, &s_ColorG, &s_ColorB);
        _Func->SetSphereColor(m_ID, r, s_ColorG, s_ColorB);
    }
}

void CForcefield::SetColG(Uint32 g) const
{
    if (Validate())
    {
        _Func->GetSphereColor(m_ID, &s_ColorR, NULL, &s_ColorB);
        _Func->SetSphereColor(m_ID, s_ColorR, g, s_ColorB);
    }
}

void CForcefield::SetColB(Uint32 b) const
{
    if (Validate())
    {
        _Func->GetSphereColor(m_ID, &s_ColorB, &s_ColorG, NULL);
        _Func->SetSphereColor(m_ID, s_ColorB, s_ColorG, b);
    }
}

// ------------------------------------------------------------------------------------------------
static Object & CreateForcefieldEx(CPlayer & player, Int32 world, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Float32 radius)
{
    return _Core->NewForcefield(player.GetID(), world, x, y, z, r, g, b, radius,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateForcefieldEx(CPlayer & player, Int32 world, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Float32 radius,
                            Int32 header, Object & payload)
{
    return _Core->NewForcefield(player.GetID(), world, x, y, z, r, g, b, radius, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateForcefield(CPlayer & player, Int32 world, const Vector3 & pos,
                            const Color3 & color, Float32 radius)
{
    return _Core->NewForcefield(player.GetID(), world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateForcefield(CPlayer & player, Int32 world, const Vector3 & pos,
                            const Color3 & color, Float32 radius, Int32 header, Object & payload)
{
    return _Core->NewForcefield(player.GetID(), world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                header, payload);
}

// ================================================================================================
void Register_CForcefield(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqForcefield"),
        Class< CForcefield, NoConstructor< CForcefield > >(vm, _SC("SqForcefield"))
        /* Metamethods */
        .Func(_SC("_cmp"), &CForcefield::Cmp)
        .Func(_SC("_tostring"), &CForcefield::ToString)
        /* Core Properties */
        .Prop(_SC("ID"), &CForcefield::GetID)
        .Prop(_SC("Tag"), &CForcefield::GetTag, &CForcefield::SetTag)
        .Prop(_SC("Data"), &CForcefield::GetData, &CForcefield::SetData)
        .Prop(_SC("MaxID"), &CForcefield::GetMaxID)
        .Prop(_SC("Active"), &CForcefield::IsActive)
        /* Core Functions */
        .Func(_SC("Bind"), &CForcefield::BindEvent)
        /* Core Overloads */
        .Overload< bool (CForcefield::*)(void) >(_SC("Destroy"), &CForcefield::Destroy)
        .Overload< bool (CForcefield::*)(Int32) >(_SC("Destroy"), &CForcefield::Destroy)
        .Overload< bool (CForcefield::*)(Int32, Object &) >(_SC("Destroy"), &CForcefield::Destroy)
        /* Properties */
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
        /* Functions */
        .Func(_SC("StreamedFor"), &CForcefield::IsStreamedFor)
        .Func(_SC("SetColor"), &CForcefield::SetColorEx)
        .Func(_SC("SetPos"), &CForcefield::SetPositionEx)
        .Func(_SC("SetPosition"), &CForcefield::SetPositionEx)
    );

    RootTable(vm)
    .Overload< Object & (*)(CPlayer &, Int32, Float32, Float32, Float32, Uint8, Uint8, Uint8, Float32) >
        (_SC("CreateForcefieldEx"), &CreateForcefieldEx)
    .Overload< Object & (*)(CPlayer &, Int32, Float32, Float32, Float32, Uint8, Uint8, Uint8, Float32, Int32, Object &) >
        (_SC("CreateForcefieldEx"), &CreateForcefieldEx)
    .Overload< Object & (*)(CPlayer &, Int32, const Vector3 &, const Color3 &, Float32) >
        (_SC("CreateForcefield"), &CreateForcefield)
    .Overload< Object & (*)(CPlayer &, Int32, const Vector3 &, const Color3 &, Float32, Int32, Object &) >
        (_SC("CreateForcefield"), &CreateForcefield);
}

} // Namespace:: SqMod