#include "Entity/Sprite.hpp"
#include "Base/Vector2i.hpp"
#include "Core.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CSprite::CSprite(const Reference< CSprite > & o)
    : Reference(o)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
void CSprite::ShowAll() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->ShowSprite(m_ID, SQMOD_UNKNOWN);
    }
    else
    {
        BadRef("show_all", "show to all");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::ShowFor(const Reference< CPlayer > & player) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->ShowSprite(m_ID, player);
    }
    else if (!player)
    {
        BadArg("show_for", "show to player", _SCI32(player));
    }
    else
    {
        BadRef("show_for", "show to player");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::ShowRange(SQInt32 first, SQInt32 last) const
{
    if (VALID_ENTITY(m_ID) && (first <= last))
    {
        for (; first <= last; ++first)
        {
            if (Reference< CPlayer >::Verify(first))
            {
                _Func->ShowSprite(m_ID, first);
            }
        }
    }
    else if (first > last)
    {
        BadArg("show_range", "show to range", "using an out of range start", first);
    }
    else
    {
        BadRef("show_range", "show to range");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::HideAll() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->HideSprite(m_ID, SQMOD_UNKNOWN);
    }
    else
    {
        BadRef("hide_all", "hide from all");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::HideFor(const Reference< CPlayer > & player) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->HideSprite(m_ID, player);
    }
    else if (!player)
    {
        BadArg("hide_for", "hide from player", _SCI32(player));
    }
    else
    {
        BadRef("hide_for", "hide from player");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::HideRange(SQInt32 first, SQInt32 last) const
{
    if (VALID_ENTITY(m_ID) && (first <= last))
    {
        for (; first <= last; ++first)
        {
            if (Reference< CPlayer >::Verify(first))
            {
                _Func->HideSprite(m_ID, first);
            }
        }
    }
    else if (first > last)
    {
        BadArg("hide_range", "hide from range", "using an out of range start", first);
    }
    else
    {
        BadRef("hide_range", "hide from range");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionAll(const Vector2i & pos) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveSprite(m_ID, SQMOD_UNKNOWN, pos.x, pos.y);
    }
    else
    {
        BadRef("set_position_all", "set position for all");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionAllEx(SQInt32 x, SQInt32 y) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveSprite(m_ID, SQMOD_UNKNOWN, x, y);
    }
    else
    {
        BadRef("set_position_all", "set position for all");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionFor(const Reference< CPlayer > & player, const Vector2i & pos) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->MoveSprite(m_ID, player, pos.x, pos.y);
    }
    else if (!player)
    {
        BadArg("set_position_for", "set position for player", _SCI32(player));
    }
    else
    {
        BadRef("set_position_for", "set position for player");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionForEx(const Reference< CPlayer > & player, SQInt32 x, SQInt32 y) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->MoveSprite(m_ID, player, x, y);
    }
    else if (!player)
    {
        BadArg("set_position_for", "set position for player", _SCI32(player));
    }
    else
    {
        BadRef("set_position_for", "set position for player");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionRange(SQInt32 first, SQInt32 last, const Vector2i & pos) const
{
    if (VALID_ENTITY(m_ID) && (first <= last))
    {
        for (; first <= last; ++first)
        {
            if (Reference< CPlayer >::Verify(first))
            {
                _Func->MoveSprite(m_ID, first, pos.x, pos.y);
            }
        }
    }
    else if (first > last)
    {
        BadArg("set_position_range", "set position for range", "using an out of range start", first);
    }
    else
    {
        BadRef("set_position_range", "set position for range");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterAll(const Vector2i & pos) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSpriteCenter(m_ID, SQMOD_UNKNOWN, pos.x, pos.y);
    }
    else
    {
        BadRef("set_center_all", "set center for all");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterAllEx(SQInt32 x, SQInt32 y) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSpriteCenter(m_ID, SQMOD_UNKNOWN, x, y);
    }
    else
    {
        BadRef("set_center_all", "set center for all");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterFor(const Reference< CPlayer > & player, const Vector2i & pos) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->SetSpriteCenter(m_ID, player, pos.x, pos.y);
    }
    else if (!player)
    {
        BadArg("set_center_for", "set center for player", _SCI32(player));
    }
    else
    {
        BadRef("set_center_for", "set center for player");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterForEx(const Reference< CPlayer > & player, SQInt32 x, SQInt32 y) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->SetSpriteCenter(m_ID, player, x, y);
    }
    else if (!player)
    {
        BadArg("set_center_for", "set center for player", _SCI32(player));
    }
    else
    {
        BadRef("set_center_for", "set center for player");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterRange(SQInt32 first, SQInt32 last, const Vector2i & pos) const
{
    if (VALID_ENTITY(m_ID) && (first <= last))
    {
        for (; first <= last; ++first)
        {
            if (Reference< CPlayer >::Verify(first))
            {
                _Func->SetSpriteCenter(m_ID, first, pos.x, pos.y);
            }
        }
    }
    else if (first > last)
    {
        BadArg("set_center_range", "set center for range", "using an out of range start", first);
    }
    else
    {
        BadRef("set_center_range", "set center for range");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetRotationAll(SQFloat rot) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotateSprite(m_ID, SQMOD_UNKNOWN, rot);
    }
    else
    {
        BadRef("set_rotation_all", "set rotation for all");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetRotationFor(const Reference< CPlayer > & player, SQFloat rot) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->RotateSprite(m_ID, player, rot);
    }
    else if (!player)
    {
        BadArg("set_rotation_for", "set rotation for player", _SCI32(player));
    }
    else
    {
        BadRef("set_rotation_for", "set rotation for player");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetRotationRange(SQInt32 first, SQInt32 last, SQFloat rot) const
{
    if (VALID_ENTITY(m_ID) && (first <= last))
    {
        for (; first <= last; ++first)
        {
            if (Reference< CPlayer >::Verify(first))
            {
                _Func->RotateSprite(m_ID, first, rot);
            }
        }
    }
    else if (first > last)
    {
        BadArg("set_rotation_range", "set rotation for range", "using an out of range start", first);
    }
    else
    {
        BadRef("set_rotation_range", "set rotation for range");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetAlphaAll(Uint8 alpha) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSpriteAlpha(m_ID, SQMOD_UNKNOWN, alpha);
    }
    else
    {
        BadRef("set_alpha_all", "set alpha for all");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetAlphaFor(const Reference< CPlayer > & player, Uint8 alpha) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->SetSpriteAlpha(m_ID, player, alpha);
    }
    else if (!player)
    {
        BadArg("set_alpha_for", "set alpha for player", _SCI32(player));
    }
    else
    {
        BadRef("set_alpha_for", "set alpha for player");
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetAlphaRange(SQInt32 first, SQInt32 last, Uint8 alpha) const
{
    if (VALID_ENTITY(m_ID) && (first <= last))
    {
        for (; first <= last; ++first)
        {
            if (Reference< CPlayer >::Verify(first))
            {
                _Func->SetSpriteAlpha(m_ID, first, alpha);
            }
        }
    }
    else if (first > last)
    {
        BadArg("set_alpha_range", "set alpha for range", "using an out of range start", first);
    }
    else
    {
        BadRef("set_alpha_range", "set alpha for range");
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CSprite::GetFilePath() const
{
    if (VALID_ENTITY(m_ID))
    {
        return RefType::Get(m_ID).Path.c_str();
    }
    else
    {
        BadRef("@path", "get file path");
    }

    return _SC("");
}

// ------------------------------------------------------------------------------------------------
Reference< CSprite > CreateBaseSprite_ES(const SQChar * file,
                        SQInt32 xp, SQInt32 yp,
                        SQInt32 xr, SQInt32 yr,
                        SQFloat angle, SQInt32 alpha, bool rel)
{
    return _Core->NewSprite(SQMOD_UNKNOWN, file, xp, yp, xr, yr, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CSprite > CreateBaseSprite_ES(const SQChar * file,
                        SQInt32 xp, SQInt32 yp,
                        SQInt32 xr, SQInt32 yr,
                        SQFloat angle, SQInt32 alpha, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewSprite(SQMOD_UNKNOWN, file, xp, yp, xr, yr, angle, alpha, rel,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CSprite > CreateBaseSprite_EF(SQInt32 index, const SQChar * file,
                        SQInt32 xp, SQInt32 yp,
                        SQInt32 xr, SQInt32 yr,
                        SQFloat angle, SQInt32 alpha, bool rel)
{
    return _Core->NewSprite(index, file, xp, yp, xr, yr, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CSprite > CreateBaseSprite_EF(SQInt32 index, const SQChar * file,
                        SQInt32 xp, SQInt32 yp,
                        SQInt32 xr, SQInt32 yr,
                        SQFloat angle, SQInt32 alpha, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewSprite(index, file, xp, yp, xr, yr, angle, alpha, rel,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CSprite > CreateBaseSprite_CS(const SQChar * file,
                        const Vector2i & pos, const Vector2i & rot,
                        SQFloat angle, SQInt32 alpha, bool rel)
{
    return _Core->NewSprite(SQMOD_UNKNOWN, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CSprite > CreateBaseSprite_CS(const SQChar * file,
                        const Vector2i & pos, const Vector2i & rot,
                        SQFloat angle, SQInt32 alpha, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewSprite(SQMOD_UNKNOWN, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CSprite > CreateBaseSprite_CF(SQInt32 index, const SQChar * file,
                        const Vector2i & pos, const Vector2i & rot,
                        SQFloat angle, SQInt32 alpha, bool rel)
{
    return _Core->NewSprite(index, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CSprite > CreateBaseSprite_CF(SQInt32 index, const SQChar * file,
                        const Vector2i & pos, const Vector2i & rot,
                        SQFloat angle, SQInt32 alpha, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewSprite(index, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CSprite CreateSprite_ES(const SQChar * file,
                        SQInt32 xp, SQInt32 yp,
                        SQInt32 xr, SQInt32 yr,
                        SQFloat angle, SQInt32 alpha, bool rel)
{
    return _Core->NewSprite(SQMOD_UNKNOWN, file, xp, yp, xr, yr, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CSprite CreateSprite_ES(const SQChar * file,
                        SQInt32 xp, SQInt32 yp,
                        SQInt32 xr, SQInt32 yr,
                        SQFloat angle, SQInt32 alpha, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewSprite(SQMOD_UNKNOWN, file, xp, yp, xr, yr, angle, alpha, rel,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CSprite CreateSprite_EF(SQInt32 index, const SQChar * file,
                        SQInt32 xp, SQInt32 yp,
                        SQInt32 xr, SQInt32 yr,
                        SQFloat angle, SQInt32 alpha, bool rel)
{
    return _Core->NewSprite(index, file, xp, yp, xr, yr, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CSprite CreateSprite_EF(SQInt32 index, const SQChar * file,
                        SQInt32 xp, SQInt32 yp,
                        SQInt32 xr, SQInt32 yr,
                        SQFloat angle, SQInt32 alpha, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewSprite(index, file, xp, yp, xr, yr, angle, alpha, rel,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CSprite CreateSprite_CS(const SQChar * file,
                        const Vector2i & pos, const Vector2i & rot,
                        SQFloat angle, SQInt32 alpha, bool rel)
{
    return _Core->NewSprite(SQMOD_UNKNOWN, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CSprite CreateSprite_CS(const SQChar * file,
                        const Vector2i & pos, const Vector2i & rot,
                        SQFloat angle, SQInt32 alpha, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewSprite(SQMOD_UNKNOWN, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CSprite CreateSprite_CF(SQInt32 index, const SQChar * file,
                        const Vector2i & pos, const Vector2i & rot,
                        SQFloat angle, SQInt32 alpha, bool rel)
{
    return _Core->NewSprite(index, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CSprite CreateSprite_CF(SQInt32 index, const SQChar * file,
                        const Vector2i & pos, const Vector2i & rot,
                        SQFloat angle, SQInt32 alpha, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewSprite(index, file, pos.x, pos.y, rot.x, rot.y, angle, alpha, rel,
                            header, payload);
}

// ================================================================================================
bool Register_CSprite(HSQUIRRELVM vm)
{
    // Attempt to register the base reference type before the actual implementation
    if (!Register_Reference< CSprite >(vm, _SC("BaseSprite")))
    {
        LogFtl("Unable to register the base class <BaseSprite> for <CSprite> type");
        // Registration failed
        return false;
    }
    // Typedef the base reference type for simplicity
    typedef Reference< CSprite > RefType;
    // Output debugging information
    LogDbg("Beginning registration of <CSprite> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CSprite"), Sqrat::DerivedClass< CSprite, RefType >(vm, _SC("CSprite"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        /* Properties */
        .Prop(_SC("path"), &CSprite::GetFilePath)
        /* Functions */
        .Func(_SC("show_all"), &CSprite::ShowAll)
        .Func(_SC("show_for"), &CSprite::ShowFor)
        .Func(_SC("show_range"), &CSprite::ShowRange)
        .Func(_SC("hide_all"), &CSprite::HideAll)
        .Func(_SC("hide_for"), &CSprite::HideFor)
        .Func(_SC("hide_range"), &CSprite::HideRange)
        /* Overloads */
        .Overload< void (CSprite::*)(const Vector2i &) const >
            (_SC("set_position_all"), &CSprite::SetPositionAll)
        .Overload< void (CSprite::*)(SQInt32, SQInt32) const >
            (_SC("set_position_all"), &CSprite::SetPositionAllEx)
        .Overload< void (CSprite::*)(const Reference< CPlayer > &, const Vector2i &) const >
            (_SC("set_position_for"), &CSprite::SetPositionFor)
        .Overload< void (CSprite::*)(const Reference< CPlayer > &, SQInt32, SQInt32) const >
            (_SC("set_position_for"), &CSprite::SetPositionForEx)
        .Overload< void (CSprite::*)(SQInt32, SQInt32, const Vector2i &) const >
            (_SC("set_position_range"), &CSprite::SetPositionRange)
        .Overload< void (CSprite::*)(const Vector2i &) const >
            (_SC("set_center_all"), &CSprite::SetCenterAll)
        .Overload< void (CSprite::*)(SQInt32, SQInt32) const >
            (_SC("set_center_all"), &CSprite::SetCenterAllEx)
        .Overload< void (CSprite::*)(const Reference< CPlayer > &, const Vector2i &) const >
            (_SC("set_center_for"), &CSprite::SetCenterFor)
        .Overload< void (CSprite::*)(const Reference< CPlayer > &, SQInt32, SQInt32) const >
            (_SC("set_center_for"), &CSprite::SetCenterForEx)
        .Overload< void (CSprite::*)(SQInt32, SQInt32, const Vector2i &) const >
            (_SC("set_center_range"), &CSprite::SetCenterRange)
        .Overload< void (CSprite::*)(SQFloat) const >
            (_SC("set_rotation_all"), &CSprite::SetRotationAll)
        .Overload< void (CSprite::*)(const Reference< CPlayer > &, SQFloat) const >
            (_SC("set_rotation_for"), &CSprite::SetRotationFor)
        .Overload< void (CSprite::*)(SQInt32, SQInt32, SQFloat) const >
            (_SC("set_rotation_range"), &CSprite::SetRotationRange)
        .Overload< void (CSprite::*)(Uint8) const >
            (_SC("set_alpha_all"), &CSprite::SetAlphaAll)
        .Overload< void (CSprite::*)(const Reference< CPlayer > &, Uint8) const >
            (_SC("set_alpha_for"), &CSprite::SetAlphaFor)
        .Overload< void (CSprite::*)(SQInt32, SQInt32, Uint8) const >
            (_SC("set_alpha_range"), &CSprite::SetAlphaRange)
    );
    // Output debugging information
    LogDbg("Registration of <CSprite> type was successful");
    // Output debugging information
    LogDbg("Beginning registration of <Sprite> functions");
    // Register global functions related to this entity type
    Sqrat::RootTable(vm)
    /* Create BaseSprite [E]xtended [S]ubstitute */
    .Overload< RefType (*)(const SQChar *, SQInt32, SQInt32, SQInt32, SQInt32, SQFloat, SQInt32, bool rel) >
        (_SC("CreateBaseSprite_ES"), &CreateBaseSprite_ES)
    .Overload< RefType (*)(const SQChar *, SQInt32, SQInt32, SQInt32, SQInt32, SQFloat, SQInt32, bool rel, SQInt32, SqObj &) >
        (_SC("CreateBaseSprite_ES"), &CreateBaseSprite_ES)
    /* Create BaseSprite [E]xtended [F]Full */
    .Overload< RefType (*)(SQInt32, const SQChar *, SQInt32, SQInt32, SQInt32, SQInt32, SQFloat, SQInt32, bool rel) >
        (_SC("CreateBaseSprite_EF"), &CreateBaseSprite_EF)
    .Overload< RefType (*)(SQInt32, const SQChar *, SQInt32, SQInt32, SQInt32, SQInt32, SQFloat, SQInt32, bool rel, SQInt32, SqObj &) >
        (_SC("CreateBaseSprite_EF"), &CreateBaseSprite_EF)
    /* Create BaseSprite [C]ompact [S]ubstitute */
    .Overload< RefType (*)(const SQChar *, const Vector2i &, const Vector2i &, SQFloat, SQInt32, bool) >
        (_SC("CreateBaseSprite_CS"), &CreateBaseSprite_CS)
    .Overload< RefType (*)(const SQChar *, const Vector2i &, const Vector2i &, SQFloat, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreateBaseSprite_CS"), &CreateBaseSprite_CS)
    /* Create BaseSprite [C]ompact [F]ull */
    .Overload< RefType (*)(SQInt32, const SQChar *, const Vector2i &, const Vector2i &, SQFloat, SQInt32, bool) >
        (_SC("CreateBaseSprite_CF"), &CreateBaseSprite_CF)
    .Overload< RefType (*)(SQInt32, const SQChar *, const Vector2i &, const Vector2i &, SQFloat, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreateBaseSprite_CF"), &CreateBaseSprite_CF)
    /* Create CSprite [E]xtended [S]ubstitute */
    .Overload< CSprite (*)(const SQChar *, SQInt32, SQInt32, SQInt32, SQInt32, SQFloat, SQInt32, bool rel) >
        (_SC("CreateSprite_ES"), &CreateSprite_ES)
    .Overload< CSprite (*)(const SQChar *, SQInt32, SQInt32, SQInt32, SQInt32, SQFloat, SQInt32, bool rel, SQInt32, SqObj &) >
        (_SC("CreateSprite_ES"), &CreateSprite_ES)
    /* Create CSprite [E]xtended [F]Full */
    .Overload< CSprite (*)(SQInt32, const SQChar *, SQInt32, SQInt32, SQInt32, SQInt32, SQFloat, SQInt32, bool rel) >
        (_SC("CreateSprite_EF"), &CreateSprite_EF)
    .Overload< CSprite (*)(SQInt32, const SQChar *, SQInt32, SQInt32, SQInt32, SQInt32, SQFloat, SQInt32, bool rel, SQInt32, SqObj &) >
        (_SC("CreateSprite_EF"), &CreateSprite_EF)
    /* Create CSprite [C]ompact [S]ubstitute */
    .Overload< CSprite (*)(const SQChar *, const Vector2i &, const Vector2i &, SQFloat, SQInt32, bool) >
        (_SC("CreateSprite_CS"), &CreateSprite_CS)
    .Overload< CSprite (*)(const SQChar *, const Vector2i &, const Vector2i &, SQFloat, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreateSprite_CS"), &CreateSprite_CS)
    /* Create CSprite [C]ompact [F]ull */
    .Overload< CSprite (*)(SQInt32, const SQChar *, const Vector2i &, const Vector2i &, SQFloat, SQInt32, bool) >
        (_SC("CreateSprite_CF"), &CreateSprite_CF)
    .Overload< CSprite (*)(SQInt32, const SQChar *, const Vector2i &, const Vector2i &, SQFloat, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreateSprite_CF"), &CreateSprite_CF);
    // Output debugging information
    LogDbg("Registration of <Sprite> functions was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
