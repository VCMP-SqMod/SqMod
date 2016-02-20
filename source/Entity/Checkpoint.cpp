// ------------------------------------------------------------------------------------------------
#include "Entity/Checkpoint.hpp"
#include "Entity/Player.hpp"
#include "Base/Color4.hpp"
#include "Base/Vector3.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Color4      CCheckpoint::s_Color4;
Vector3     CCheckpoint::s_Vector3;

// ------------------------------------------------------------------------------------------------
Uint32      CCheckpoint::s_ColorR;
Uint32      CCheckpoint::s_ColorG;
Uint32      CCheckpoint::s_ColorB;
Uint32      CCheckpoint::s_ColorA;

// ------------------------------------------------------------------------------------------------
SQChar CCheckpoint::s_StrID[SQMOD_CHECKPOINT_POOL][8];

// ------------------------------------------------------------------------------------------------
const Int32 CCheckpoint::Max = SQMOD_CHECKPOINT_POOL;

// ------------------------------------------------------------------------------------------------
CCheckpoint::CCheckpoint(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_CHECKPOINT_POOL))
    , m_Tag(VALID_ENTITY(m_ID) ? s_StrID[m_ID] : _SC("-1"))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CCheckpoint::~CCheckpoint()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 CCheckpoint::Cmp(const CCheckpoint & o) const
{
    if (m_ID == o.m_ID)
        return 0;
    else if (m_ID > o.m_ID)
        return 1;
    else
        return -1;
}

CSStr CCheckpoint::ToString() const
{
    return VALID_ENTITYEX(m_ID, SQMOD_CHECKPOINT_POOL) ? s_StrID[m_ID] : _SC("-1");
}

// ------------------------------------------------------------------------------------------------
CSStr CCheckpoint::GetTag() const
{
    return m_Tag.c_str();
}

void CCheckpoint::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

Object & CCheckpoint::GetData()
{
    if (Validate())
        return m_Data;
    return NullObject();
}

void CCheckpoint::SetData(Object & data)
{
    if (Validate())
        m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CCheckpoint::Destroy(Int32 header, Object & payload)
{
    return _Core->DelCheckpoint(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CCheckpoint::BindEvent(Int32 evid, Object & env, Function & func) const
{
    if (!Validate())
        return false;

    Function & event = _Core->GetCheckpointEvent(m_ID, evid);

    if (func.IsNull())
        event.Release();
    else
        event = Function(env.GetVM(), env, func.GetFunc());

    return true;
}

// ------------------------------------------------------------------------------------------------
bool CCheckpoint::IsStreamedFor(CPlayer & player) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        return _Func->IsCheckpointStreamedForPlayer(m_ID, player.GetID());
    return false;
}

Int32 CCheckpoint::GetWorld() const
{
    if (Validate())
        return _Func->GetCheckpointWorld(m_ID);
    return -1;
}

void CCheckpoint::SetWorld(Int32 world) const
{
    if (Validate())
        _Func->SetCheckpointWorld(m_ID, world);
}

const Color4 & CCheckpoint::GetColor() const
{
    s_Color4.Clear();
    if (Validate())
    {
        _Func->GetCheckpointColor(m_ID, &s_ColorR, &s_ColorG, &s_ColorB, &s_ColorA);
        s_Color4.Set(s_ColorR, s_ColorG, s_ColorB, s_ColorA);
    }
    return s_Color4;
}

void CCheckpoint::SetColor(const Color4 & col) const
{
    if (Validate())
        _Func->SetCheckpointColor(m_ID, col.r, col.g, col.b, col.a);
}

void CCheckpoint::SetColorEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    if (Validate())
        _Func->SetCheckpointColor(m_ID, r, g, b, a);
}

const Vector3 & CCheckpoint::GetPosition() const
{
    s_Vector3.Clear();
    if (Validate())
        _Func->GetCheckpointPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    return s_Vector3;
}

void CCheckpoint::SetPosition(const Vector3 & pos) const
{
    if (Validate())
        _Func->SetCheckpointPos(m_ID, pos.x, pos.y, pos.z);
}

void CCheckpoint::SetPositionEx(Float32 x, Float32 y, Float32 z) const
{
    if (Validate())
        _Func->SetCheckpointPos(m_ID, x, y, z);
}

Float32 CCheckpoint::GetRadius() const
{
    if (Validate())
        _Func->GetCheckpointRadius(m_ID);
    return 0;
}

void CCheckpoint::SetRadius(Float32 radius) const
{
    if (Validate())
        _Func->SetCheckpointRadius(m_ID, radius);
}

Object & CCheckpoint::GetOwner() const
{
    if (Validate())
        return _Core->GetPlayer(_Func->GetCheckpointOwner(m_ID)).mObj;
    return NullObject();
}

Int32 CCheckpoint::GetOwnerID() const
{
    if (Validate())
        return _Func->GetCheckpointOwner(m_ID);
    return -1;
}

// ------------------------------------------------------------------------------------------------
Float32 CCheckpoint::GetPosX() const
{
    s_Vector3.x = 0;
    if (Validate())
        _Func->GetCheckpointPos(m_ID, &s_Vector3.x, NULL, NULL);
    return s_Vector3.x;
}

Float32 CCheckpoint::GetPosY() const
{
    s_Vector3.y = 0;
    if (Validate())
        _Func->GetCheckpointPos(m_ID, NULL, &s_Vector3.y, NULL);
    return s_Vector3.y;
}

Float32 CCheckpoint::GetPosZ() const
{
    s_Vector3.z = 0;
    if (Validate())
        _Func->GetCheckpointPos(m_ID, NULL, NULL, &s_Vector3.z);
    return s_Vector3.z;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPosX(Float32 x) const
{
    if (Validate())
    {
        _Func->GetCheckpointPos(m_ID, NULL, &s_Vector3.y, &s_Vector3.z);
        _Func->SetCheckpointPos(m_ID, x, s_Vector3.y, s_Vector3.z);
    }
}

void CCheckpoint::SetPosY(Float32 y) const
{
    if (Validate())
    {
        _Func->GetCheckpointPos(m_ID, &s_Vector3.x, NULL, &s_Vector3.z);
        _Func->SetCheckpointPos(m_ID, s_Vector3.x, y, s_Vector3.z);
    }
}

void CCheckpoint::SetPosZ(Float32 z) const
{
    if (Validate())
    {
        _Func->GetCheckpointPos(m_ID, &s_Vector3.x, &s_Vector3.y, NULL);
        _Func->SetCheckpointPos(m_ID, s_Vector3.z, s_Vector3.y, z);
    }
}

// ------------------------------------------------------------------------------------------------
Uint32 CCheckpoint::GetColR() const
{
    s_ColorR = 0;
    if (Validate())
        _Func->GetCheckpointColor(m_ID, &s_ColorR, NULL, NULL, NULL);
    return s_ColorR;
}

Uint32 CCheckpoint::GetColG() const
{
    s_ColorG = 0;
    if (Validate())
        _Func->GetCheckpointColor(m_ID, NULL, &s_ColorG, NULL, NULL);
    return s_ColorG;
}

Uint32 CCheckpoint::GetColB() const
{
    s_ColorB = 0;
    if (Validate())
        _Func->GetCheckpointColor(m_ID, NULL, NULL, &s_ColorB, NULL);
    return s_ColorB;
}

Uint32 CCheckpoint::GetColA() const
{
    s_ColorA = 0;
    if (Validate())
        _Func->GetCheckpointColor(m_ID, NULL, NULL, NULL, &s_ColorA);
    return s_ColorA;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColR(Uint32 r) const
{
    if (Validate())
    {
        _Func->GetCheckpointColor(m_ID, NULL, &s_ColorG, &s_ColorB, &s_ColorA);
        _Func->SetCheckpointColor(m_ID, r, s_ColorG, s_ColorB, s_ColorA);
    }
}

void CCheckpoint::SetColG(Uint32 g) const
{
    if (Validate())
    {
        _Func->GetCheckpointColor(m_ID, &s_ColorR, NULL, &s_ColorB, &s_ColorA);
        _Func->SetCheckpointColor(m_ID, s_ColorR, g, s_ColorB, s_ColorA);
    }
}

void CCheckpoint::SetColB(Uint32 b) const
{
    if (Validate())
    {
        _Func->GetCheckpointColor(m_ID, &s_ColorB, &s_ColorG, NULL, &s_ColorA);
        _Func->SetCheckpointColor(m_ID, s_ColorB, s_ColorG, b, s_ColorA);
    }
}

void CCheckpoint::SetColA(Uint32 a) const
{
    if (Validate())
    {
        _Func->GetCheckpointColor(m_ID, &s_ColorA, &s_ColorG, &s_ColorB, NULL);
        _Func->SetCheckpointColor(m_ID, s_ColorA, s_ColorG, s_ColorB, a);
    }
}

// ------------------------------------------------------------------------------------------------
static Object & CreateCheckpointEx(CPlayer & player, Int32 world, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius)
{
    return _Core->NewCheckpoint(player.GetID(), world, x, y, z, r, g, b, a, radius,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateCheckpointEx(CPlayer & player, Int32 world, Float32 x, Float32 y, Float32 z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Float32 radius,
                            Int32 header, Object & payload)
{
    return _Core->NewCheckpoint(player.GetID(), world, x, y, z, r, g, b, a, radius, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateCheckpoint(CPlayer & player, Int32 world, const Vector3 & pos,
                            const Color4 & color, Float32 radius)
{
    return _Core->NewCheckpoint(player.GetID(), world, pos.x, pos.y, pos.z,
                                color.r, color.g, color.b, color.a, radius,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateCheckpoint(CPlayer & player, Int32 world, const Vector3 & pos,
                            const Color4 & color, Float32 radius, Int32 header, Object & payload)
{
    return _Core->NewCheckpoint(player.GetID(), world, pos.x, pos.y, pos.z,
                                color.r, color.g, color.b, color.a, radius, header, payload);
}

// ================================================================================================
void Register_CCheckpoint(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqCheckpoint"),
        Class< CCheckpoint, NoConstructor< CCheckpoint > >(vm, _SC("SqCheckpoint"))
        /* Metamethods */
        .Func(_SC("_cmp"), &CCheckpoint::Cmp)
        .Func(_SC("_tostring"), &CCheckpoint::ToString)
        /* Core Properties */
        .Prop(_SC("ID"), &CCheckpoint::GetID)
        .Prop(_SC("Tag"), &CCheckpoint::GetTag, &CCheckpoint::SetTag)
        .Prop(_SC("Data"), &CCheckpoint::GetData, &CCheckpoint::SetData)
        .Prop(_SC("MaxID"), &CCheckpoint::GetMaxID)
        .Prop(_SC("Active"), &CCheckpoint::IsActive)
        /* Core Functions */
        .Func(_SC("Bind"), &CCheckpoint::BindEvent)
        /* Core Overloads */
        .Overload< bool (CCheckpoint::*)(void) >(_SC("Destroy"), &CCheckpoint::Destroy)
        .Overload< bool (CCheckpoint::*)(Int32) >(_SC("Destroy"), &CCheckpoint::Destroy)
        .Overload< bool (CCheckpoint::*)(Int32, Object &) >(_SC("Destroy"), &CCheckpoint::Destroy)
        /* Properties */
        .Prop(_SC("World"), &CCheckpoint::GetWorld, &CCheckpoint::SetWorld)
        .Prop(_SC("Color"), &CCheckpoint::GetColor, &CCheckpoint::SetColor)
        .Prop(_SC("Pos"), &CCheckpoint::GetPosition, &CCheckpoint::SetPosition)
        .Prop(_SC("Position"), &CCheckpoint::GetPosition, &CCheckpoint::SetPosition)
        .Prop(_SC("Radius"), &CCheckpoint::GetRadius, &CCheckpoint::SetRadius)
        .Prop(_SC("Owner"), &CCheckpoint::GetOwner)
        .Prop(_SC("OwnerID"), &CCheckpoint::GetOwnerID)
        .Prop(_SC("X"), &CCheckpoint::GetPosX, &CCheckpoint::SetPosX)
        .Prop(_SC("Y"), &CCheckpoint::GetPosY, &CCheckpoint::SetPosY)
        .Prop(_SC("Z"), &CCheckpoint::GetPosZ, &CCheckpoint::SetPosZ)
        .Prop(_SC("R"), &CCheckpoint::GetColR, &CCheckpoint::SetColR)
        .Prop(_SC("G"), &CCheckpoint::GetColG, &CCheckpoint::SetColG)
        .Prop(_SC("B"), &CCheckpoint::GetColB, &CCheckpoint::SetColB)
        .Prop(_SC("A"), &CCheckpoint::GetColA, &CCheckpoint::SetColA)
        /* Functions */
        .Func(_SC("StreamedFor"), &CCheckpoint::IsStreamedFor)
        .Func(_SC("SetColor"), &CCheckpoint::SetColorEx)
        .Func(_SC("SetPos"), &CCheckpoint::SetPositionEx)
        .Func(_SC("SetPosition"), &CCheckpoint::SetPositionEx)
    );

    RootTable(vm)
    .Overload< Object & (*)(CPlayer &, Int32, Float32, Float32, Float32, Uint8, Uint8, Uint8, Uint8, Float32) >
        (_SC("CreateCheckpointEx"), &CreateCheckpointEx)
    .Overload< Object & (*)(CPlayer &, Int32, Float32, Float32, Float32, Uint8, Uint8, Uint8, Uint8, Float32, Int32, Object &) >
        (_SC("CreateCheckpointEx"), &CreateCheckpointEx)
    .Overload< Object & (*)(CPlayer &, Int32, const Vector3 &, const Color4 &, Float32) >
        (_SC("CreateCheckpoint"), &CreateCheckpoint)
    .Overload< Object & (*)(CPlayer &, Int32, const Vector3 &, const Color4 &, Float32, Int32, Object &) >
        (_SC("CreateCheckpoint"), &CreateCheckpoint);
}

} // Namespace:: SqMod