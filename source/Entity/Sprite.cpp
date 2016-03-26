// ------------------------------------------------------------------------------------------------
#include "Entity/Sprite.hpp"
#include "Entity/Player.hpp"
#include "Base/Vector2i.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Int32 CSprite::Max = SQMOD_SPRITE_POOL;

// ------------------------------------------------------------------------------------------------
SQInteger CSprite::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqSprite");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
CSprite::CSprite(Int32 id)
    : m_ID(VALID_ENTITYGETEX(id, SQMOD_SPRITE_POOL))
    , m_Tag(ToStrF("%d", id))
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

// ------------------------------------------------------------------------------------------------
const String & CSprite::ToString() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
const String & CSprite::GetTag() const
{
    return m_Tag;
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetTag(CSStr tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
Object & CSprite::GetData()
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetData(Object & data)
{
    // Validate the managed identifier
    Validate();
    // Apply the specified value
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
bool CSprite::Destroy(Int32 header, Object & payload)
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    return _Core->DelSprite(m_ID, header, payload);
}

// ------------------------------------------------------------------------------------------------
void CSprite::BindEvent(Int32 evid, Object & env, Function & func) const
{
    // Validate the managed identifier
    Validate();
    // Obtain the function instance called for this event
    Function & event = _Core->GetSpriteEvent(m_ID, evid);
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
void CSprite::ShowAll() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->ShowSprite(m_ID, -1);
}

// ------------------------------------------------------------------------------------------------
void CSprite::ShowFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->ShowSprite(m_ID, player.GetID());

}

// ------------------------------------------------------------------------------------------------
void CSprite::ShowRange(Int32 first, Int32 last) const
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
            _Func->ShowSprite(m_ID, first);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::HideAll() const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->HideSprite(m_ID, -1);
}

// ------------------------------------------------------------------------------------------------
void CSprite::HideFor(CPlayer & player) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->HideSprite(m_ID, player.GetID());
}

// ------------------------------------------------------------------------------------------------
void CSprite::HideRange(Int32 first, Int32 last) const
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
            _Func->HideSprite(m_ID, first);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionAll(const Vector2i & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveSprite(m_ID, -1, pos.x, pos.y);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionAllEx(Int32 x, Int32 y) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveSprite(m_ID, -1, x, y);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionFor(CPlayer & player, const Vector2i & pos) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveSprite(m_ID, player.GetID(), pos.x, pos.y);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionForEx(CPlayer & player, Int32 x, Int32 y) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->MoveSprite(m_ID, player.GetID(), x, y);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionRange(Int32 first, Int32 last, const Vector2i & pos) const
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
            _Func->MoveSprite(m_ID, first, pos.x, pos.y);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterAll(const Vector2i & pos) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSpriteCenter(m_ID, -1, pos.x, pos.y);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterAllEx(Int32 x, Int32 y) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSpriteCenter(m_ID, -1, x, y);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterFor(CPlayer & player, const Vector2i & pos) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSpriteCenter(m_ID, player.GetID(), pos.x, pos.y);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterForEx(CPlayer & player, Int32 x, Int32 y) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSpriteCenter(m_ID, player.GetID(), x, y);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterRange(Int32 first, Int32 last, const Vector2i & pos) const
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
            // Then center this textdraw on his client
            _Func->SetSpriteCenter(m_ID, first, pos.x, pos.y);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetRotationAll(Float32 rot) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateSprite(m_ID, -1, rot);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetRotationFor(CPlayer & player, Float32 rot) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->RotateSprite(m_ID, player.GetID(), rot);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetRotationRange(Int32 first, Int32 last, Float32 rot) const
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
            // Then rotate this textdraw on his client
            _Func->RotateSprite(m_ID, first, rot);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetAlphaAll(Uint8 alpha) const
{
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSpriteAlpha(m_ID, -1, alpha);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetAlphaFor(CPlayer & player, Uint8 alpha) const
{
    // Is the specified player even valid?
    if (!player.IsActive())
    {
        STHROWF("Invalid player argument: null");
    }
    // Validate the managed identifier
    Validate();
    // Perform the requested operation
    _Func->SetSpriteAlpha(m_ID, player.GetID(), alpha);
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetAlphaRange(Int32 first, Int32 last, Uint8 alpha) const
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
            // Then colorize this textdraw on his client
            _Func->SetSpriteAlpha(m_ID, first, alpha);
        }
    }
}

// ------------------------------------------------------------------------------------------------
const String & CSprite::GetFilePath() const
{
    // Validate the managed identifier
    Validate();
    // Return the requested information
    return _Core->GetSprite(m_ID).mPath;
}

// ------------------------------------------------------------------------------------------------
static Object & Sprite_CreateEx(CSStr file, Int32 xp, Int32 yp, Int32 xr, Int32 yr,
                                Float32 angle, Int32 alpha, bool rel)
{
    return _Core->NewSprite(-1, file, xp, yp, xr, yr, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

// ------------------------------------------------------------------------------------------------
static Object & Sprite_CreateEx(CSStr file, Int32 xp, Int32 yp, Int32 xr, Int32 yr,
                                Float32 angle, Int32 alpha, bool rel, Int32 header, Object & payload)
{
    return _Core->NewSprite(-1, file, xp, yp, xr, yr, angle, alpha, rel, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Sprite_CreateEx(Int32 index, CSStr file, Int32 xp, Int32 yp, Int32 xr, Int32 yr,
                                Float32 angle, Int32 alpha, bool rel)
{
    return _Core->NewSprite(index, file, xp, yp, xr, yr, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

// ------------------------------------------------------------------------------------------------
static Object & Sprite_CreateEx(Int32 index, CSStr file, Int32 xp, Int32 yp, Int32 xr, Int32 yr,
                                Float32 angle, Int32 alpha, bool rel, Int32 header, Object & payload)
{
    return _Core->NewSprite(index, file, xp, yp, xr, yr, angle, alpha, rel, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Sprite_Create(CSStr file, const Vector2i & pos, const Vector2i & rot,
                                Float32 angle, Int32 alpha, bool rel)
{
    return _Core->NewSprite(-1, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

// ------------------------------------------------------------------------------------------------
static Object & Sprite_Create(CSStr file, const Vector2i & pos, const Vector2i & rot,
                                Float32 angle, Int32 alpha, bool rel, Int32 header, Object & payload)
{
    return _Core->NewSprite(-1, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel, header, payload);
}

// ------------------------------------------------------------------------------------------------
static Object & Sprite_Create(Int32 index, CSStr file, const Vector2i & pos, const Vector2i & rot,
                                Float32 angle, Int32 alpha, bool rel)
{
    return _Core->NewSprite(index, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullObject());
}

// ------------------------------------------------------------------------------------------------
static Object & Sprite_Create(Int32 index, CSStr file, const Vector2i & pos, const Vector2i & rot,
                                Float32 angle, Int32 alpha, bool rel, Int32 header, Object & payload)
{
    return _Core->NewSprite(index, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel, header, payload);
}

// ------------------------------------------------------------------------------------------------
static const Object & Sprite_FindByID(Int32 id)
{
    // Perform a range check on the specified identifier
    if (INVALID_ENTITYEX(id, SQMOD_SPRITE_POOL))
    {
        STHROWF("The specified sprite identifier is invalid: %d", id);
    }
    // Obtain the ends of the entity pool
    Core::Sprites::const_iterator itr = _Core->GetSprites().cbegin();
    Core::Sprites::const_iterator end = _Core->GetSprites().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does the identifier match the specified one?
        if (itr->mID == id)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a sprite matching the specified identifier
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static const Object & Sprite_FindByTag(CSStr tag)
{
    // Perform a validity check on the specified tag
    if (!tag || *tag == '\0')
    {
        STHROWF("The specified sprite tag is invalid: null/empty");
    }
    // Obtain the ends of the entity pool
    Core::Sprites::const_iterator itr = _Core->GetSprites().cbegin();
    Core::Sprites::const_iterator end = _Core->GetSprites().cend();
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Does this entity even exist and does the tag match the specified one?
        if (itr->mInst != nullptr && itr->mInst->GetTag().compare(tag) == 0)
        {
            return itr->mObj; // Stop searching and return this entity
        }
    }
    // Unable to locate a sprite matching the specified tag
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
static Array Sprite_FindActive()
{
    // Remember the initial stack size
    StackGuard sg;
    // Obtain the ends of the entity pool
    Core::Sprites::const_iterator itr = _Core->GetSprites().cbegin();
    Core::Sprites::const_iterator end = _Core->GetSprites().cend();
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
void Register_CSprite(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqSprite"),
        Class< CSprite, NoConstructor< CSprite > >(vm, _SC("SqSprite"))
        // Metamethods
        .Func(_SC("_cmp"), &CSprite::Cmp)
        .SquirrelFunc(_SC("_typename"), &CSprite::Typename)
        .Func(_SC("_tostring"), &CSprite::ToString)
        // Static Values
        .SetStaticValue(_SC("MaxID"), CSprite::Max)
        // Core Properties
        .Prop(_SC("ID"), &CSprite::GetID)
        .Prop(_SC("Tag"), &CSprite::GetTag, &CSprite::SetTag)
        .Prop(_SC("Data"), &CSprite::GetData, &CSprite::SetData)
        .Prop(_SC("Active"), &CSprite::IsActive)
        // Core Methods
        .Func(_SC("Bind"), &CSprite::BindEvent)
        // Core Overloads
        .Overload< bool (CSprite::*)(void) >(_SC("Destroy"), &CSprite::Destroy)
        .Overload< bool (CSprite::*)(Int32) >(_SC("Destroy"), &CSprite::Destroy)
        .Overload< bool (CSprite::*)(Int32, Object &) >(_SC("Destroy"), &CSprite::Destroy)
        // Properties
        .Prop(_SC("Path"), &CSprite::GetFilePath)
        // Member Methods
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
        // Member Overloads
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
        // Static Functions
        .StaticFunc(_SC("FindByID"), &Sprite_FindByID)
        .StaticFunc(_SC("FindByTag"), &Sprite_FindByTag)
        .StaticFunc(_SC("FindActive"), &Sprite_FindActive)
        // Static Overloads
        .StaticOverload< Object & (*)(CSStr, Int32, Int32, Int32, Int32, Float32, Int32, bool rel) >
            (_SC("CreateEx"), &Sprite_CreateEx)
        .StaticOverload< Object & (*)(CSStr, Int32, Int32, Int32, Int32, Float32, Int32, bool rel, Int32, Object &) >
            (_SC("CreateEx"), &Sprite_CreateEx)
        .StaticOverload< Object & (*)(Int32, CSStr, Int32, Int32, Int32, Int32, Float32, Int32, bool rel) >
            (_SC("CreateEx"), &Sprite_CreateEx)
        .StaticOverload< Object & (*)(Int32, CSStr, Int32, Int32, Int32, Int32, Float32, Int32, bool rel, Int32, Object &) >
            (_SC("CreateEx"), &Sprite_CreateEx)
        .StaticOverload< Object & (*)(CSStr, const Vector2i &, const Vector2i &, Float32, Int32, bool) >
            (_SC("Create"), &Sprite_Create)
        .StaticOverload< Object & (*)(CSStr, const Vector2i &, const Vector2i &, Float32, Int32, bool, Int32, Object &) >
            (_SC("Create"), &Sprite_Create)
        .StaticOverload< Object & (*)(Int32, CSStr, const Vector2i &, const Vector2i &, Float32, Int32, bool) >
            (_SC("Create"), &Sprite_Create)
        .StaticOverload< Object & (*)(Int32, CSStr, const Vector2i &, const Vector2i &, Float32, Int32, bool, Int32, Object &) >
            (_SC("Create"), &Sprite_Create)
    );
}

} // Namespace:: SqMod
