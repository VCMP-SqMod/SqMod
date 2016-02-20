// ------------------------------------------------------------------------------------------------
#include "Entity/Textdraw.hpp"
#include "Entity/Player.hpp"
#include "Base/Vector2i.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQChar CTextdraw::s_StrID[SQMOD_TEXTDRAW_POOL][8];

// ------------------------------------------------------------------------------------------------
const Int32 CTextdraw::Max = SQMOD_TEXTDRAW_POOL;

// ------------------------------------------------------------------------------------------------
CTextdraw::CTextdraw(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_TEXTDRAW_POOL))
    , m_Tag(VALID_ENTITY(m_ID) ? s_StrID[m_ID] : _SC("-1"))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CTextdraw::~CTextdraw()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Int32 CTextdraw::Cmp(const CTextdraw & o) const
{
    if (m_ID == o.m_ID)
        return 0;
    else if (m_ID > o.m_ID)
        return 1;
    else
        return -1;
}

CSStr CTextdraw::ToString() const
{
    return VALID_ENTITYEX(m_ID, SQMOD_TEXTDRAW_POOL) ? s_StrID[m_ID] : _SC("-1");
}

// ------------------------------------------------------------------------------------------------
CSStr CTextdraw::GetTag() const
{
    return m_Tag.c_str();
}

void CTextdraw::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

Object & CTextdraw::GetData()
{
    if (Validate())
        return m_Data;
    return NullObject();
}

void CTextdraw::SetData(Object & data)
{
    if (Validate())
        m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CTextdraw::Destroy(Int32 header, Object & payload)
{
    return _Core->DelTextdraw(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
bool CTextdraw::BindEvent(Int32 evid, Object & env, Function & func) const
{
    if (!Validate())
        return false;

    Function & event = _Core->GetTextdrawEvent(m_ID, evid);

    if (func.IsNull())
        event.Release();
    else
        event = Function(env.GetVM(), env, func.GetFunc());

    return true;
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::ShowAll() const
{
    if (Validate())
        _Func->ShowTextdraw(m_ID, -1);
}

void CTextdraw::ShowFor(CPlayer & player) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->ShowTextdraw(m_ID, player.GetID());
}

void CTextdraw::ShowRange(Int32 first, Int32 last) const
{
    if (first > last)
        SqThrow("Invalid player range: %d > %d", first, last);
    else if (Validate())
        for (; first <= last; ++first)
        {
            if (_Func->IsPlayerConnected(first))
                _Func->ShowTextdraw(m_ID, first);
        }
}

void CTextdraw::HideAll() const
{
    if (Validate())
        _Func->HideTextdraw(m_ID, -1);
}

void CTextdraw::HideFor(CPlayer & player) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->HideTextdraw(m_ID, player.GetID());
}

void CTextdraw::HideRange(Int32 first, Int32 last) const
{
    if (first > last)
        SqThrow("Invalid player range: %d > %d", first, last);
    else if (Validate())
        for (; first <= last; ++first)
        {
            if (_Func->IsPlayerConnected(first))
                _Func->HideTextdraw(m_ID, first);
        }
}

void CTextdraw::SetPositionAll(const Vector2i & pos) const
{
    if (Validate())
        _Func->MoveTextdraw(m_ID, -1, pos.x, pos.y);
}

void CTextdraw::SetPositionAllEx(Int32 x, Int32 y) const
{
    if (Validate())
        _Func->MoveTextdraw(m_ID, -1, x, y);
}

void CTextdraw::SetPositionFor(CPlayer & player, const Vector2i & pos) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->MoveTextdraw(m_ID, player.GetID(), pos.x, pos.y);
}

void CTextdraw::SetPositionForEx(CPlayer & player, Int32 x, Int32 y) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->MoveTextdraw(m_ID, player.GetID(), x, y);
}

void CTextdraw::SetPositionRange(Int32 first, Int32 last, const Vector2i & pos) const
{
    if (first > last)
        SqThrow("Invalid player range: %d > %d", first, last);
    else if (Validate())
        for (; first <= last; ++first)
        {
            if (_Func->IsPlayerConnected(first))
                _Func->MoveTextdraw(m_ID, first, pos.x, pos.y);
        }
}

void CTextdraw::SetColorAll(const Color4 & col) const
{
    if (Validate())
        _Func->SetTextdrawColour(m_ID, -1, col.GetRGBA());
}

void CTextdraw::SetColorAllEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    if (Validate())
        _Func->SetTextdrawColour(m_ID, -1, SQMOD_PACK_RGBA(r, g, b, a));
}

void CTextdraw::SetColorFor(CPlayer & player, const Color4 & col) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->SetTextdrawColour(m_ID, player.GetID(), col.GetRGBA());
}

void CTextdraw::SetColorForEx(CPlayer & player, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    if (!player.IsActive())
        SqThrow("Invalid player argument: null");
    else if (Validate())
        _Func->SetTextdrawColour(m_ID, player.GetID(), SQMOD_PACK_RGBA(r, g, b, a));
}

void CTextdraw::SetColorRange(Int32 first, Int32 last, const Color4 & col) const
{
    if (first > last)
        SqThrow("Invalid player range: %d > %d", first, last);
    else if (Validate())
        for (const Uint32 color = col.GetRGBA(); first <= last; ++first)
        {
            if (_Func->IsPlayerConnected(first))
                _Func->SetTextdrawColour(m_ID, first, color);
        }
}

CSStr CTextdraw::GetText() const
{
    if (Validate())
        _Core->GetTextdraw(m_ID).mText.c_str();
    return g_EmptyStr;
}

// ------------------------------------------------------------------------------------------------
static Object & CreateTextdrawEx(CSStr text, Int32 xp, Int32 yp,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool rel)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, xp, yp, SQMOD_PACK_ARGB(a, r, g, b), rel,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateTextdrawEx(CSStr text, Int32 xp, Int32 yp,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool rel,
                                Int32 header, Object & payload)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, xp, yp, SQMOD_PACK_ARGB(a, r, g, b), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateTextdrawEx(Int32 index, CSStr text, Int32 xp, Int32 yp,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool rel)
{
    return _Core->NewTextdraw(index,text, xp, yp, SQMOD_PACK_ARGB(a, r, g, b), rel,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateTextdrawEx(Int32 index, CSStr text, Int32 xp, Int32 yp,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool rel,
                                Int32 header, Object & payload)
{
    return _Core->NewTextdraw(index, text, xp, yp, SQMOD_PACK_ARGB(a, r, g, b), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateTextdraw(CSStr text, const Vector2i & pos, const Color4 & color, bool rel)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, pos.x, pos.y, color.GetARGB(), rel,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateTextdraw(CSStr text, const Vector2i & pos, const Color4 & color, bool rel,
                                Int32 header, Object & payload)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, pos.x, pos.y, color.GetARGB(), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & CreateTextdraw(Int32 index, CSStr text, const Vector2i & pos, const Color4 & color,
                                bool rel)
{
    return _Core->NewTextdraw(index, text, pos.x, pos.y, color.GetARGB(), rel,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

static Object & CreateTextdraw(Int32 index, CSStr text, const Vector2i & pos, const Color4 & color,
                                bool rel, Int32 header, Object & payload)
{
    return _Core->NewTextdraw(index, text, pos.x, pos.y, color.GetARGB(), rel, header, payload);
}

// ================================================================================================
void Register_CTextdraw(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqTextdraw"),
        Class< CTextdraw, NoConstructor< CTextdraw > >(vm, _SC("SqTextdraw"))
        /* Metamethods */
        .Func(_SC("_cmp"), &CTextdraw::Cmp)
        .Func(_SC("_tostring"), &CTextdraw::ToString)
        /* Core Properties */
        .Prop(_SC("ID"), &CTextdraw::GetID)
        .Prop(_SC("Tag"), &CTextdraw::GetTag, &CTextdraw::SetTag)
        .Prop(_SC("Data"), &CTextdraw::GetData, &CTextdraw::SetData)
        .Prop(_SC("MaxID"), &CTextdraw::GetMaxID)
        .Prop(_SC("Active"), &CTextdraw::IsActive)
        /* Core Functions */
        .Func(_SC("Bind"), &CTextdraw::BindEvent)
        /* Core Overloads */
        .Overload< bool (CTextdraw::*)(void) >(_SC("Destroy"), &CTextdraw::Destroy)
        .Overload< bool (CTextdraw::*)(Int32) >(_SC("Destroy"), &CTextdraw::Destroy)
        .Overload< bool (CTextdraw::*)(Int32, Object &) >(_SC("Destroy"), &CTextdraw::Destroy)
        /* Properties */
        .Prop(_SC("Text"), &CTextdraw::GetText)
        /* Functions */
        .Func(_SC("ShowAll"), &CTextdraw::ShowAll)
        .Func(_SC("ShowTo"), &CTextdraw::ShowFor)
        .Func(_SC("ShowFor"), &CTextdraw::ShowFor)
        .Func(_SC("ShowRange"), &CTextdraw::ShowRange)
        .Func(_SC("HideAll"), &CTextdraw::HideAll)
        .Func(_SC("HideFor"), &CTextdraw::HideFor)
        .Func(_SC("HideFrom"), &CTextdraw::HideFor)
        .Func(_SC("HideRange"), &CTextdraw::HideRange)
        .Func(_SC("SetPositionRange"), &CTextdraw::SetPositionRange)
        .Func(_SC("SetColorRange"), &CTextdraw::SetColorRange)
        /* Overloads */
        .Overload< void (CTextdraw::*)(const Vector2i &) const >
            (_SC("SetPositionAll"), &CTextdraw::SetPositionAll)
        .Overload< void (CTextdraw::*)(Int32, Int32) const >
            (_SC("SetPositionAll"), &CTextdraw::SetPositionAllEx)
        .Overload< void (CTextdraw::*)(CPlayer &, const Vector2i &) const >
            (_SC("SetPositionFor"), &CTextdraw::SetPositionFor)
        .Overload< void (CTextdraw::*)(CPlayer &, Int32, Int32) const >
            (_SC("SetPositionFor"), &CTextdraw::SetPositionForEx)
        .Overload< void (CTextdraw::*)(const Color4 &) const >
            (_SC("SetColorAll"), &CTextdraw::SetColorAll)
        .Overload< void (CTextdraw::*)(Uint8, Uint8, Uint8, Uint8) const >
            (_SC("SetColorAll"), &CTextdraw::SetColorAllEx)
        .Overload< void (CTextdraw::*)(CPlayer &, const Color4 &) const >
            (_SC("SetColorFor"), &CTextdraw::SetColorFor)
        .Overload< void (CTextdraw::*)(CPlayer &, Uint8, Uint8, Uint8, Uint8) const >
            (_SC("SetColorFor"), &CTextdraw::SetColorForEx)
    );

    RootTable(vm)
    .Overload< Object & (*)(CSStr, Int32, Int32, Uint8, Uint8, Uint8, Uint8, bool) >
        (_SC("CreateTextdrawEx"), &CreateTextdrawEx)
    .Overload< Object & (*)(CSStr, Int32, Int32, Uint8, Uint8, Uint8, Uint8, bool, Int32, Object &) >
        (_SC("CreateTextdrawEx"), &CreateTextdrawEx)
    .Overload< Object & (*)(Int32, CSStr, Int32, Int32, Uint8, Uint8, Uint8, Uint8, bool) >
        (_SC("CreateTextdrawEx"), &CreateTextdrawEx)
    .Overload< Object & (*)(Int32, CSStr, Int32, Int32, Uint8, Uint8, Uint8, Uint8, bool, Int32, Object &) >
        (_SC("CreateTextdrawEx"), &CreateTextdrawEx)
    .Overload< Object & (*)(CSStr, const Vector2i &, const Color4 &, bool) >
        (_SC("CreateTextdraw"), &CreateTextdraw)
    .Overload< Object & (*)(CSStr, const Vector2i &, const Color4 &, bool, Int32, Object &) >
        (_SC("CreateTextdraw"), &CreateTextdraw)
    .Overload< Object & (*)(Int32, CSStr, const Vector2i &, const Color4 &, bool) >
        (_SC("CreateTextdraw"), &CreateTextdraw)
    .Overload< Object & (*)(Int32, CSStr, const Vector2i &, const Color4 &, bool, Int32, Object &) >
        (_SC("CreateTextdraw"), &CreateTextdraw);
}

} // Namespace:: SqMod