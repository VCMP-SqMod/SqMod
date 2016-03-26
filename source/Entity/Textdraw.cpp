// ------------------------------------------------------------------------------------------------
#include "Entity/Textdraw.hpp"
#include "Entity/Player.hpp"
#include "Base/Vector2i.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Int32 CTextdraw::Max = SQMOD_TEXTDRAW_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CTextdraw::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqTextdraw");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
CTextdraw::CTextdraw(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_TEXTDRAW_POOL))
    , m_Tag(ToStrF("%d", id))
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

// ------------------------------------------------------------------------------------------------
const String & CTextdraw::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CTextdraw::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
Object & CTextdraw::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetData(Object & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CTextdraw::Destroy(Int32 header, Object & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return _Core->DelTextdraw(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::BindEvent(Int32 evid, Object & env, Function & func) const
{
    // Validate the managed identifier
    Validate();
    // Obtain the function instance called for this event
    Function & event = _Core->GetTextdrawEvent(m_ID, evid);
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
void CTextdraw::ShowAll() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->ShowTextdraw(m_ID, -1);
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::ShowFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->ShowTextdraw(m_ID, player.GetID());
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::ShowRange(Int32 first, Int32 last) const
{
    // Validate the specified range
    if (first > last)
    {
        STHROWF("Invalid player range: %d > %d", first, last);
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    for (; first <= last; ++first)
    {
        // Is the currently processed player even connected?
        if (_Func->IsPlayerConnected(first))
        {
            // Then show this textdraw on his client
            _Func->ShowTextdraw(m_ID, first);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::HideAll() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->HideTextdraw(m_ID, -1);
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::HideFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->HideTextdraw(m_ID, player.GetID());
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::HideRange(Int32 first, Int32 last) const
{
    // Validate the specified range
    if (first > last)
    {
        STHROWF("Invalid player range: %d > %d", first, last);
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    for (; first <= last; ++first)
    {
        // Is the currently processed player even connected?
        if (_Func->IsPlayerConnected(first))
        {
            // Then hide this textdraw on his client
            _Func->HideTextdraw(m_ID, first);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetPositionAll(const Vector2i & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveTextdraw(m_ID, -1, pos.x, pos.y);
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetPositionAllEx(Int32 x, Int32 y) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveTextdraw(m_ID, -1, x, y);
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetPositionFor(CPlayer & player, const Vector2i & pos) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveTextdraw(m_ID, player.GetID(), pos.x, pos.y);
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetPositionForEx(CPlayer & player, Int32 x, Int32 y) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveTextdraw(m_ID, player.GetID(), x, y);
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetPositionRange(Int32 first, Int32 last, const Vector2i & pos) const
{
    // Validate the specified range
    if (first > last)
    {
        STHROWF("Invalid player range: %d > %d", first, last);
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    for (; first <= last; ++first)
    {
        // Is the currently processed player even connected?
        if (_Func->IsPlayerConnected(first))
        {
            // Then move this textdraw on his client
            _Func->MoveTextdraw(m_ID, first, pos.x, pos.y);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetColorAll(const Color4 & col) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetTextdrawColour(m_ID, -1, col.GetRGBA());
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetColorAllEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetTextdrawColour(m_ID, -1, SQMOD_PACK_RGBA(r, g, b, a));
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetColorFor(CPlayer & player, const Color4 & col) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetTextdrawColour(m_ID, player.GetID(), col.GetRGBA());
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetColorForEx(CPlayer & player, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetTextdrawColour(m_ID, player.GetID(), SQMOD_PACK_RGBA(r, g, b, a));
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetColorRange(Int32 first, Int32 last, const Color4 & col) const
{
    // Validate the specified range
    if (first > last)
    {
        STHROWF("Invalid player range: %d > %d", first, last);
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    for (const Uint32 color = col.GetRGBA(); first <= last; ++first)
    {
        // Is the currently processed player even connected?
        if (_Func->IsPlayerConnected(first))
        {
            // Then colorize this textdraw on his client
            _Func->SetTextdrawColour(m_ID, first, color);
        }
    }
}

// ------------------------------------------------------------------------------------------------
const String & CTextdraw::GetText() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetTextdraw(m_ID).mText;
}

// ------------------------------------------------------------------------------------------------
static Object & Textdraw_CreateEx(CSStr text, Int32 xp, Int32 yp,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool rel)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, xp, yp, SQMOD_PACK_ARGB(a, r, g, b), rel,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

// ------------------------------------------------------------------------------------------------
static Object & Textdraw_CreateEx(CSStr text, Int32 xp, Int32 yp,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool rel,
                                Int32 header, Object & payload)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, xp, yp, SQMOD_PACK_ARGB(a, r, g, b), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Textdraw_CreateEx(Int32 index, CSStr text, Int32 xp, Int32 yp,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool rel)
{
    return _Core->NewTextdraw(index,text, xp, yp, SQMOD_PACK_ARGB(a, r, g, b), rel,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

// ------------------------------------------------------------------------------------------------
static Object & Textdraw_CreateEx(Int32 index, CSStr text, Int32 xp, Int32 yp,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool rel,
                                Int32 header, Object & payload)
{
    return _Core->NewTextdraw(index, text, xp, yp, SQMOD_PACK_ARGB(a, r, g, b), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Textdraw_Create(CSStr text, const Vector2i & pos, const Color4 & color, bool rel)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, pos.x, pos.y, color.GetARGB(), rel,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

// ------------------------------------------------------------------------------------------------
static Object & Textdraw_Create(CSStr text, const Vector2i & pos, const Color4 & color, bool rel,
                                Int32 header, Object & payload)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, pos.x, pos.y, color.GetARGB(), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Textdraw_Create(Int32 index, CSStr text, const Vector2i & pos, const Color4 & color,
                                bool rel)
{
    return _Core->NewTextdraw(index, text, pos.x, pos.y, color.GetARGB(), rel,
                                SQMOD_CREATE_DEFAULT, NullObject());
}

// ------------------------------------------------------------------------------------------------
static Object & Textdraw_Create(Int32 index, CSStr text, const Vector2i & pos, const Color4 & color,
                                bool rel, Int32 header, Object & payload)
{
    return _Core->NewTextdraw(index, text, pos.x, pos.y, color.GetARGB(), rel, header, payload);
}

// ------------------------------------------------------------------------------------------------
static const Object & Textdraw_FindByID(Int32 id)
{
    // Perform a range check on the specified identifier
    if (INVALID_ENTITYEX(id, SQMOD_TEXTDRAW_POOL))
    {
        STHROWF("The specified textdraw identifier is invalid: %d", id);
    }
    // Obtain the ends of the entity pool
    Core::Textdraws::const_iterator itr = _Core->GetTextdraws().cbegin();
    Core::Textdraws::const_iterator end = _Core->GetTextdraws().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mID == id)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a textdraw matching the specified identifier
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static const Object & Textdraw_FindByTag(CSStr tag)
{
    // Perform a validity check on the specified tag
    if (!tag || *tag == '\0')
    {
        STHROWF("The specified textdraw tag is invalid: null/empty");
    }
    // Obtain the ends of the entity pool
    Core::Textdraws::const_iterator itr = _Core->GetTextdraws().cbegin();
    Core::Textdraws::const_iterator end = _Core->GetTextdraws().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does this entity even exist and does the tag match the specified one?
        if (itr->mInst != nullptr && itr->mInst->GetTag().compare(tag) == 0)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a textdraw matching the specified tag
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static Array Textdraw_FindActive()
{
    // Remember the initial stack size
    StackGuard sg;
    // Obtain the ends of the entity pool
    Core::Textdraws::const_iterator itr = _Core->GetTextdraws().cbegin();
    Core::Textdraws::const_iterator end = _Core->GetTextdraws().cend();
    // Allocate an empty array on the stack
    sq_newarray(DefaultVM::Get(), 0);
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this entity instance active?
        if (VALID_ENTITY(itr->mID))
        {
            // Push the script object on the stack
            sq_pushobject(DefaultVM::Get(), (HSQOBJECT &)((*itr).mObj));
            // Append the object at the back of the array
            if (SQ_FAILED(sq_arrayappend(DefaultVM::Get(), -1)))
            {
                STHROWF("Unable to append entity instance to the list");
            }
        }
    }
    // Return the array at the top of the stack
    return Var< Array >(DefaultVM::Get(), -1).value;
}

// ================================================================================================
void Register_CTextdraw(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqTextdraw"),
        Class< CTextdraw, NoConstructor< CTextdraw > >(vm, _SC("SqTextdraw"))
        // Metamethods
        .Func(_SC("_cmp"), &CTextdraw::Cmp)
        .SquirrelFunc(_SC("_typename"), &CTextdraw::Typename)
        .Func(_SC("_tostring"), &CTextdraw::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CTextdraw::Max)
        // Core Properties
        .Prop(_SC("ID"), &CTextdraw::GetID)
        .Prop(_SC("Tag"), &CTextdraw::GetTag, &CTextdraw::SetTag)
        .Prop(_SC("Data"), &CTextdraw::GetData, &CTextdraw::SetData)
        .Prop(_SC("Active"), &CTextdraw::IsActive)
        // Core Methods
        .Func(_SC("Bind"), &CTextdraw::BindEvent)
        // Core Overloads
        .Overload< bool (CTextdraw::*)(void) >(_SC("Destroy"), &CTextdraw::Destroy)
        .Overload< bool (CTextdraw::*)(Int32) >(_SC("Destroy"), &CTextdraw::Destroy)
        .Overload< bool (CTextdraw::*)(Int32, Object &) >(_SC("Destroy"), &CTextdraw::Destroy)
        // Properties
        .Prop(_SC("Text"), &CTextdraw::GetText)
        // Member Methods
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
        // Member Overloads
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
        // Static Functions
        .StaticFunc(_SC("FindByID"), &Textdraw_FindByID)
        .StaticFunc(_SC("FindByTag"), &Textdraw_FindByTag)
        .StaticFunc(_SC("FindActive"), &Textdraw_FindActive)
        // Static Overloads
        .StaticOverload< Object & (*)(CSStr, Int32, Int32, Uint8, Uint8, Uint8, Uint8, bool) >
            (_SC("CreateEx"), &Textdraw_CreateEx)
        .StaticOverload< Object & (*)(CSStr, Int32, Int32, Uint8, Uint8, Uint8, Uint8, bool, Int32, Object &) >
            (_SC("CreateEx"), &Textdraw_CreateEx)
        .StaticOverload< Object & (*)(Int32, CSStr, Int32, Int32, Uint8, Uint8, Uint8, Uint8, bool) >
            (_SC("CreateEx"), &Textdraw_CreateEx)
        .StaticOverload< Object & (*)(Int32, CSStr, Int32, Int32, Uint8, Uint8, Uint8, Uint8, bool, Int32, Object &) >
            (_SC("CreateEx"), &Textdraw_CreateEx)
        .StaticOverload< Object & (*)(CSStr, const Vector2i &, const Color4 &, bool) >
            (_SC("Create"), &Textdraw_Create)
        .StaticOverload< Object & (*)(CSStr, const Vector2i &, const Color4 &, bool, Int32, Object &) >
            (_SC("Create"), &Textdraw_Create)
        .StaticOverload< Object & (*)(Int32, CSStr, const Vector2i &, const Color4 &, bool) >
            (_SC("Create"), &Textdraw_Create)
        .StaticOverload< Object & (*)(Int32, CSStr, const Vector2i &, const Color4 &, bool, Int32, Object &) >
            (_SC("Create"), &Textdraw_Create)
    );
}

} // Namespace:: SqMod
