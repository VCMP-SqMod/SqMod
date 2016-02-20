// ------------------------------------------------------------------------------------------------
#include "Entity/Sprite.hpp"
#include "Entity/Player.hpp"
#include "Base/Vector2i.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQChar CSprite::s_StrID[SQMOD_SPRITE_POOL][8];

// ------------------------------------------------------------------------------------------------
const Int32 CSprite::Max = SQMOD_SPRITE_POOL;

// ------------------------------------------------------------------------------------------------
CSprite::CSprite(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_SPRITE_POOL))
    , m_Tag(VALID_ENTITY(m_ID) ? s_StrID[m_ID] : _SC("-1"))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CSprite::~CSprite()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 CSprite::Cmp(const CSprite & o) const
{
    if (m_ID == o.m_ID)
        return 0;
    else if (m_ID > o.m_ID)
        return 1;
    else
        return -1;
}

CSStr CSprite::ToString() const
{
    return VALID_ENTITYEX(m_ID, SQMOD_SPRITE_POOL) ? s_StrID[m_ID] : _SC("-1");
}

// ------------------------------------------------------------------------------------------------
CSStr CSprite::GetTag() const
{
    return m_Tag.c_str();
}

void CSprite::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

Object & CSprite::GetData()
{
    if (Validate())
        return m_Data;
    return NullObject();
}

void CSprite::SetData(Object & data)
{
    if (Validate())
        m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CSprite::Destroy(Int32 header, Object & payload)
{
    return _Core->DelSprite(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CSprite::BindEvent(Int32 evid, Object & env, Function & func) const
{
    if (!Validate())
        return false;

    Function & event = _Core->GetSpriteEvent(m_ID, evid);

    if (func.IsNull())
        event.Release();
    else
        event = Function(env.GetVM(), env, func.GetFunc());

    return true;
}

// ------------------------------------------------------------------------------------------------
void CSprite::ShowAll() const
{
    if (Validate())
        _Func->ShowSprite(m_ID, -1);
}

void CSprite::ShowFor(CPlayer & player) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->ShowSprite(m_ID, player.GetID());

}

void CSprite::ShowRange(Int32 first, Int32 last) const
{
    if (first > last)
        SqThrow("Invalid player range: %d > %d", first, last);
    else if (Validate())
        for (; first <= last; ++first)
        {
            if (_Func->IsPlayerConnected(first))
                _Func->ShowSprite(m_ID, first);
        }
}

void CSprite::HideAll() const
{
    if (Validate())
        _Func->HideSprite(m_ID, -1);
}

void CSprite::HideFor(CPlayer & player) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->HideSprite(m_ID, player.GetID());
}

void CSprite::HideRange(Int32 first, Int32 last) const
{
    if (first > last)
        SqThrow("Invalid player range: %d > %d", first, last);
    else if (Validate())
        for (; first <= last; ++first)
        {
            if (_Func->IsPlayerConnected(first))
                _Func->HideSprite(m_ID, first);
        }
}

void CSprite::SetPositionAll(const Vector2i & pos) const
{
    if (Validate())
        _Func->MoveSprite(m_ID, -1, pos.x, pos.y);
}

void CSprite::SetPositionAllEx(Int32 x, Int32 y) const
{
    if (Validate())
        _Func->MoveSprite(m_ID, -1, x, y);
}

void CSprite::SetPositionFor(CPlayer & player, const Vector2i & pos) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->MoveSprite(m_ID, player.GetID(), pos.x, pos.y);
}

void CSprite::SetPositionForEx(CPlayer & player, Int32 x, Int32 y) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->MoveSprite(m_ID, player.GetID(), x, y);
}

void CSprite::SetPositionRange(Int32 first, Int32 last, const Vector2i & pos) const
{
    if (first > last)
        SqThrow("Invalid player range: %d > %d", first, last);
    else if (Validate())
        for (; first <= last; ++first)
        {
            if (_Func->IsPlayerConnected(first))
                _Func->MoveSprite(m_ID, first, pos.x, pos.y);
        }
}

void CSprite::SetCenterAll(const Vector2i & pos) const
{
    if (Validate())
        _Func->SetSpriteCenter(m_ID, -1, pos.x, pos.y);
}

void CSprite::SetCenterAllEx(Int32 x, Int32 y) const
{
    if (Validate())
        _Func->SetSpriteCenter(m_ID, -1, x, y);
}

void CSprite::SetCenterFor(CPlayer & player, const Vector2i & pos) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->SetSpriteCenter(m_ID, player.GetID(), pos.x, pos.y);
}

void CSprite::SetCenterForEx(CPlayer & player, Int32 x, Int32 y) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->SetSpriteCenter(m_ID, player.GetID(), x, y);
}

void CSprite::SetCenterRange(Int32 first, Int32 last, const Vector2i & pos) const
{
    if (first > last)
        SqThrow("Invalid player range: %d > %d", first, last);
    else if (Validate())
        for (; first <= last; ++first)
        {
            if (_Func->IsPlayerConnected(first))
                _Func->SetSpriteCenter(m_ID, first, pos.x, pos.y);
        }
}

void CSprite::SetRotationAll(Float32 rot) const
{
    if (Validate())
        _Func->RotateSprite(m_ID, -1, rot);
}

void CSprite::SetRotationFor(CPlayer & player, Float32 rot) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->RotateSprite(m_ID, player.GetID(), rot);
}

void CSprite::SetRotationRange(Int32 first, Int32 last, Float32 rot) const
{
    if (first > last)
        SqThrow("Invalid player range: %d > %d", first, last);
    else if (Validate())
        for (; first <= last; ++first)
        {
            if (_Func->IsPlayerConnected(first))
                _Func->RotateSprite(m_ID, first, rot);
        }
}

void CSprite::SetAlphaAll(Uint8 alpha) const
{
    if (Validate())
        _Func->SetSpriteAlpha(m_ID, -1, alpha);
}

void CSprite::SetAlphaFor(CPlayer & player, Uint8 alpha) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->SetSpriteAlpha(m_ID, player.GetID(), alpha);
}

void CSprite::SetAlphaRange(Int32 first, Int32 last, Uint8 alpha) const
{
    if (first > last)
        SqThrow("Invalid player range: %d > %d", first, last);
    else if (Validate())
        for (; first <= last; ++first)
        {
            if (_Func->IsPlayerConnected(first))
                _Func->SetSpriteAlpha(m_ID, first, alpha);
        }
}

CSStr CSprite::GetFilePath() const
{
    if (Validate())
        _Core->GetSprite(m_ID).mPath.c_str();
    return g_EmptyStr;
}

// ------------------------------------------------------------------------------------------------
static Object & CreateSpriteEx(CSStr file, Int32 xp, Int32 yp, Int32 xr, Int32 yr,
                                Float32 angle, Int32 alpha, bool rel)
{
    return _Core->NewSprite(-1, file, xp, yp, xr, yr, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateSpriteEx(CSStr file, Int32 xp, Int32 yp, Int32 xr, Int32 yr,
                                Float32 angle, Int32 alpha, bool rel, Int32 header, Object & payload)
{
    return _Core->NewSprite(-1, file, xp, yp, xr, yr, angle, alpha, rel, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateSpriteEx(Int32 index, CSStr file, Int32 xp, Int32 yp, Int32 xr, Int32 yr,
                                Float32 angle, Int32 alpha, bool rel)
{
    return _Core->NewSprite(index, file, xp, yp, xr, yr, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateSpriteEx(Int32 index, CSStr file, Int32 xp, Int32 yp, Int32 xr, Int32 yr,
                                Float32 angle, Int32 alpha, bool rel, Int32 header, Object & payload)
{
    return _Core->NewSprite(index, file, xp, yp, xr, yr, angle, alpha, rel, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateSprite(CSStr file, const Vector2i & pos, const Vector2i & rot,
                                Float32 angle, Int32 alpha, bool rel)
{
    return _Core->NewSprite(-1, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateSprite(CSStr file, const Vector2i & pos, const Vector2i & rot,
                                Float32 angle, Int32 alpha, bool rel, Int32 header, Object & payload)
{
    return _Core->NewSprite(-1, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateSprite(Int32 index, CSStr file, const Vector2i & pos, const Vector2i & rot,
                                Float32 angle, Int32 alpha, bool rel)
{
    return _Core->NewSprite(index, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateSprite(Int32 index, CSStr file, const Vector2i & pos, const Vector2i & rot,
                                Float32 angle, Int32 alpha, bool rel, Int32 header, Object & payload)
{
    return _Core->NewSprite(index, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel, header, payload);
}

// ================================================================================================
void Register_CSprite(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqSprite"),
        Class< CSprite, NoConstructor< CSprite > >(vm, _SC("SqSprite"))
        /* Metamethods */
        .Func(_SC("_cmp"), &CSprite::Cmp)
        .Func(_SC("_tostring"), &CSprite::ToString)
        /* Core Properties */
        .Prop(_SC("ID"), &CSprite::GetID)
        .Prop(_SC("Tag"), &CSprite::GetTag, &CSprite::SetTag)
        .Prop(_SC("Data"), &CSprite::GetData, &CSprite::SetData)
        .Prop(_SC("MaxID"), &CSprite::GetMaxID)
        .Prop(_SC("Active"), &CSprite::IsActive)
        /* Core Functions */
        .Func(_SC("Bind"), &CSprite::BindEvent)
        /* Core Overloads */
        .Overload< bool (CSprite::*)(void) >(_SC("Destroy"), &CSprite::Destroy)
        .Overload< bool (CSprite::*)(Int32) >(_SC("Destroy"), &CSprite::Destroy)
        .Overload< bool (CSprite::*)(Int32, Object &) >(_SC("Destroy"), &CSprite::Destroy)
        /* Properties */
        .Prop(_SC("Path"), &CSprite::GetFilePath)
        /* Functions */
        .Func(_SC("ShowAll"), &CSprite::ShowAll)
        .Func(_SC("ShowTo"), &CSprite::ShowFor)
        .Func(_SC("ShowFor"), &CSprite::ShowFor)
        .Func(_SC("ShowRange"), &CSprite::ShowRange)
        .Func(_SC("HideAll"), &CSprite::HideAll)
        .Func(_SC("HideFor"), &CSprite::HideFor)
        .Func(_SC("HideFrom"), &CSprite::HideFor)
        .Func(_SC("HideRange"), &CSprite::HideRange)
        .Func(_SC("SetPositionRange"), &CSprite::SetPositionRange)
        .Func(_SC("SetCenterRange"), &CSprite::SetCenterRange)
        .Func(_SC("SetRotationAll"), &CSprite::SetRotationAll)
        .Func(_SC("SetRotationFor"), &CSprite::SetRotationFor)
        .Func(_SC("SetRotationRange"), &CSprite::SetRotationRange)
        .Func(_SC("SetAlphaAll"), &CSprite::SetAlphaAll)
        .Func(_SC("SetAlphaFor"), &CSprite::SetAlphaFor)
        .Func(_SC("SetAlphaRange"), &CSprite::SetAlphaRange)
        /* Overloads */
        .Overload< void (CSprite::*)(const Vector2i &) const >
            (_SC("SetPositionAll"), &CSprite::SetPositionAll)
        .Overload< void (CSprite::*)(Int32, Int32) const >
            (_SC("SetPositionAll"), &CSprite::SetPositionAllEx)
        .Overload< void (CSprite::*)(CPlayer &, const Vector2i &) const >
            (_SC("SetPositionFor"), &CSprite::SetPositionFor)
        .Overload< void (CSprite::*)(CPlayer &, Int32, Int32) const >
            (_SC("SetPositionFor"), &CSprite::SetPositionForEx)
        .Overload< void (CSprite::*)(const Vector2i &) const >
            (_SC("SetCenterAll"), &CSprite::SetCenterAll)
        .Overload< void (CSprite::*)(Int32, Int32) const >
            (_SC("SetCenterAll"), &CSprite::SetCenterAllEx)
        .Overload< void (CSprite::*)(CPlayer &, const Vector2i &) const >
            (_SC("SetCenterFor"), &CSprite::SetCenterFor)
        .Overload< void (CSprite::*)(CPlayer &, Int32, Int32) const >
            (_SC("SetCenterFor"), &CSprite::SetCenterForEx)
    );

    RootTable(vm)
    .Overload< Object & (*)(CSStr, Int32, Int32, Int32, Int32, Float32, Int32, bool rel) >
        (_SC("CreateSpriteEx"), &CreateSpriteEx)
    .Overload< Object & (*)(CSStr, Int32, Int32, Int32, Int32, Float32, Int32, bool rel, Int32, Object &) >
        (_SC("CreateSpriteEx"), &CreateSpriteEx)
    .Overload< Object & (*)(Int32, CSStr, Int32, Int32, Int32, Int32, Float32, Int32, bool rel) >
        (_SC("CreateSpriteEx"), &CreateSpriteEx)
    .Overload< Object & (*)(Int32, CSStr, Int32, Int32, Int32, Int32, Float32, Int32, bool rel, Int32, Object &) >
        (_SC("CreateSpriteEx"), &CreateSpriteEx)
    .Overload< Object & (*)(CSStr, const Vector2i &, const Vector2i &, Float32, Int32, bool) >
        (_SC("CreateSprite"), &CreateSprite)
    .Overload< Object & (*)(CSStr, const Vector2i &, const Vector2i &, Float32, Int32, bool, Int32, Object &) >
        (_SC("CreateSprite"), &CreateSprite)
    .Overload< Object & (*)(Int32, CSStr, const Vector2i &, const Vector2i &, Float32, Int32, bool) >
        (_SC("CreateSprite"), &CreateSprite)
    .Overload< Object & (*)(Int32, CSStr, const Vector2i &, const Vector2i &, Float32, Int32, bool, Int32, Object &) >
        (_SC("CreateSprite"), &CreateSprite);
}

} // Namespace:: SqMod