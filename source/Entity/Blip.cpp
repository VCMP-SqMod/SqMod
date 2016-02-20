// ------------------------------------------------------------------------------------------------
#include "Entity/Blip.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQChar CBlip::s_StrID[SQMOD_BLIP_POOL][8];

// ------------------------------------------------------------------------------------------------
const Int32 CBlip::Max = SQMOD_BLIP_POOL;

// ------------------------------------------------------------------------------------------------
CBlip::CBlip(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_BLIP_POOL))
    , m_Tag(VALID_ENTITY(m_ID) ? s_StrID[m_ID] : _SC("-1"))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CBlip::~CBlip()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 CBlip::Cmp(const CBlip & o) const
{
    if (m_ID == o.m_ID)
        return 0;
    else if (m_ID > o.m_ID)
        return 1;
    else
        return -1;
}

CSStr CBlip::ToString() const
{
    return VALID_ENTITYEX(m_ID, SQMOD_BLIP_POOL) ? s_StrID[m_ID] : _SC("-1");
}

// ------------------------------------------------------------------------------------------------
CSStr CBlip::GetTag() const
{
    return m_Tag.c_str();
}

void CBlip::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

Object & CBlip::GetData()
{
    if (Validate())
        return m_Data;
    return NullObject();
}

void CBlip::SetData(Object & data)
{
    if (Validate())
        m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CBlip::Destroy(Int32 header, Object & payload)
{
    return _Core->DelBlip(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CBlip::BindEvent(Int32 evid, Object & env, Function & func) const
{
    if (!Validate())
        return false;

    Function & event = _Core->GetBlipEvent(m_ID, evid);

    if (func.IsNull())
        event.Release();
    else
        event = Function(env.GetVM(), env, func.GetFunc());

    return true;
}

// ------------------------------------------------------------------------------------------------
Int32 CBlip::GetWorld() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mWorld;
    return -1;
}

Int32 CBlip::GetScale() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mScale;
    return -1;
}

const Vector3 & CBlip::GetPosition() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mPosition;
    return Vector3::NIL;
}

const Color4 & CBlip::GetColor() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mColor;
    return Color4::NIL;
}

Int32 CBlip::GetSprID() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mSprID;
    return -1;
}

// ------------------------------------------------------------------------------------------------
Float32 CBlip::GetPosX() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mPosition.x;
    return 0;
}

Float32 CBlip::GetPosY() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mPosition.y;
    return 0;
}

Float32 CBlip::GetPosZ() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mPosition.z;
    return 0;
}

// ------------------------------------------------------------------------------------------------
Int32 CBlip::GetColorR() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mColor.r;
    return 0;
}

Int32 CBlip::GetColorG() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mColor.g;
    return 0;
}

Int32 CBlip::GetColorB() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mColor.b;
    return 0;
}

Int32 CBlip::GetColorA() const
{
    if (Validate())
        return _Core->GetBlip(m_ID).mColor.a;
    return 0;
}

// ------------------------------------------------------------------------------------------------
static Object & CreateBlipEx(Int32 world, Float32 x, Float32 y, Float32 z, Int32 scale,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Int32 sprid)
{
    return _Core->NewBlip(-1, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), sprid,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateBlipEx(Int32 world, Float32 x, Float32 y, Float32 z, Int32 scale,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Int32 sprid,
                            Int32 header, Object & payload)
{
    return _Core->NewBlip(-1, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateBlipEx(Int32 index, Int32 world, Float32 x, Float32 y, Float32 z,
                            Int32 scale, Uint8 r, Uint8 g, Uint8 b, Uint8 a, Int32 sprid)
{
    return _Core->NewBlip(index, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), sprid,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateBlipEx(Int32 index, Int32 world, Float32 x, Float32 y, Float32 z, Int32 scale,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a, Int32 sprid,
                            Int32 header, Object & payload)
{
    return _Core->NewBlip(index, world, x, y, z, scale, SQMOD_PACK_RGBA(r, g, b, a), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateBlip(Int32 world, const Vector3 & pos, Int32 scale, const Color4 & color,
                            Int32 sprid)
{
    return _Core->NewBlip(-1, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateBlip(Int32 world, const Vector3 & pos, Int32 scale, const Color4 & color,
                            Int32 sprid, Int32 header, Object & payload)
{
    return _Core->NewBlip(-1, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateBlip(Int32 index, Int32 world, const Vector3 & pos, Int32 scale,
                            const Color4 & color, Int32 sprid)
{
    return _Core->NewBlip(index, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateBlip(Int32 index, Int32 world, const Vector3 & pos, Int32 scale,
                            const Color4 & color, Int32 sprid, Int32 header, Object & payload)
{
    return _Core->NewBlip(index, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprid,
                            header, payload);
}

// ================================================================================================
void Register_CBlip(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqBlip"),
        Class< CBlip, NoConstructor< CBlip > >(vm, _SC("SqBlip"))
        /* Metamethods */
        .Func(_SC("_cmp"), &CBlip::Cmp)
        .Func(_SC("_tostring"), &CBlip::ToString)
        /* Core Properties */
        .Prop(_SC("ID"), &CBlip::GetID)
        .Prop(_SC("Tag"), &CBlip::GetTag, &CBlip::SetTag)
        .Prop(_SC("Data"), &CBlip::GetData, &CBlip::SetData)
        .Prop(_SC("MaxID"), &CBlip::GetMaxID)
        .Prop(_SC("Active"), &CBlip::IsActive)
        /* Core Functions */
        .Func(_SC("Bind"), &CBlip::BindEvent)
        /* Core Overloads */
        .Overload< bool (CBlip::*)(void) >(_SC("Destroy"), &CBlip::Destroy)
        .Overload< bool (CBlip::*)(Int32) >(_SC("Destroy"), &CBlip::Destroy)
        .Overload< bool (CBlip::*)(Int32, Object &) >(_SC("Destroy"), &CBlip::Destroy)
        /* Properties */
        .Prop(_SC("World"), &CBlip::GetWorld)
        .Prop(_SC("Scale"), &CBlip::GetScale)
        .Prop(_SC("Pos"), &CBlip::GetPosition)
        .Prop(_SC("Position"), &CBlip::GetPosition)
        .Prop(_SC("Color"), &CBlip::GetColor)
        .Prop(_SC("SprID"), &CBlip::GetSprID)
        .Prop(_SC("X"), &CBlip::GetPosX)
        .Prop(_SC("Y"), &CBlip::GetPosY)
        .Prop(_SC("Z"), &CBlip::GetPosZ)
        .Prop(_SC("R"), &CBlip::GetColorR)
        .Prop(_SC("G"), &CBlip::GetColorG)
        .Prop(_SC("B"), &CBlip::GetColorB)
        .Prop(_SC("A"), &CBlip::GetColorA)
    );

    RootTable(vm)
    .Overload< Object & (*)(Int32, Float32, Float32, Float32, Int32, Uint8, Uint8, Uint8, Uint8, Int32) >
        (_SC("CreateBlipEx"), &CreateBlipEx)
    .Overload< Object & (*)(Int32, Float32, Float32, Float32, Int32, Uint8, Uint8, Uint8, Uint8, Int32, Int32, Object &) >
        (_SC("CreateBlipEx"), &CreateBlipEx)
    .Overload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Int32, Uint8, Uint8, Uint8, Uint8, Int32) >
        (_SC("CreateBlipEx"), &CreateBlipEx)
    .Overload< Object & (*)(Int32, Int32, Float32, Float32, Float32, Int32, Uint8, Uint8, Uint8, Uint8, Int32, Int32, Object &) >
        (_SC("CreateBlipEx"), &CreateBlipEx)
    .Overload< Object & (*)(Int32, const Vector3 &, Int32, const Color4 &, Int32) >
        (_SC("CreateBlip"), &CreateBlip)
    .Overload< Object & (*)(Int32, const Vector3 &, Int32, const Color4 &, Int32, Int32, Object &) >
        (_SC("CreateBlip"), &CreateBlip)
    .Overload< Object & (*)(Int32, Int32, const Vector3 &, Int32, const Color4 &, Int32) >
        (_SC("CreateBlip"), &CreateBlip)
    .Overload< Object & (*)(Int32, Int32, const Vector3 &, Int32, const Color4 &, Int32, Int32, Object &) >
        (_SC("CreateBlip"), &CreateBlip);
}

} // Namespace:: SqMod