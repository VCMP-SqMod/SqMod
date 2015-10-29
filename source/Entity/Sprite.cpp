#include "Entity/Sprite.hpp"
#include "Base/Vector2u.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void CSprite::ShowAll() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->ShowSprite(m_ID, SQMOD_UNKNOWN);
    }
    else
    {
        LogWrn(_SC("Attempting to <show sprite to all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::ShowFor(const Reference< CPlayer > & player) const noexcept
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->ShowSprite(m_ID, player);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <show sprite to player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <show sprite to player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::ShowRange(SQInt32 first, SQInt32 last) const noexcept
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
        LogWrn(_SC("Attempting to <show sprite to range> using an invalid argument: %d > %d"), first, last);
    }
    else
    {
        LogWrn(_SC("Attempting to <show sprite to range> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::HideAll() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->HideSprite(m_ID, SQMOD_UNKNOWN);
    }
    else
    {
        LogWrn(_SC("Attempting to <hide sprite from all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::HideFor(const Reference< CPlayer > & player) const noexcept
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->HideSprite(m_ID, player);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <hide sprite from player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <hide sprite from player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::HideRange(SQInt32 first, SQInt32 last) const noexcept
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
        LogWrn(_SC("Attempting to <hide sprite from range> using an invalid argument: %d > %d"), first, last);
    }
    else
    {
        LogWrn(_SC("Attempting to <hide sprite from range> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionAll(const Vector2u & pos) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveSprite(m_ID, SQMOD_UNKNOWN, pos.x, pos.y);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite position for all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionAllEx(SQUnsignedInteger32 x, SQUnsignedInteger32 y) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveSprite(m_ID, SQMOD_UNKNOWN, x, y);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite position for all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionFor(const Reference< CPlayer > & player, const Vector2u & pos) const noexcept
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->MoveSprite(m_ID, player, pos.x, pos.y);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <set sprite position for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite position for player> using an invalid reference: %d"), m_ID);

    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionForEx(const Reference< CPlayer > & player, SQUnsignedInteger32 x, SQUnsignedInteger32 y) const noexcept
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->MoveSprite(m_ID, player, x, y);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <set sprite position for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite position for player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetPositionRange(SQInt32 first, SQInt32 last, const Vector2u & pos) const noexcept
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
        LogWrn(_SC("Attempting to <set sprite position for range> using an invalid argument: %d > %d"), first, last);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite position for range> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterAll(const Vector2u & pos) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSpriteCenter(m_ID, SQMOD_UNKNOWN, pos.x, pos.y);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite center for all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterAllEx(SQUnsignedInteger32 x, SQUnsignedInteger32 y) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSpriteCenter(m_ID, SQMOD_UNKNOWN, x, y);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite center for all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterFor(const Reference< CPlayer > & player, const Vector2u & pos) const noexcept
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->SetSpriteCenter(m_ID, player, pos.x, pos.y);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <set sprite center for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite center for player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterForEx(const Reference< CPlayer > & player, SQUnsignedInteger32 x, SQUnsignedInteger32 y) const noexcept
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->SetSpriteCenter(m_ID, player, x, y);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <set sprite center for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite center for player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetCenterRange(SQInt32 first, SQInt32 last, const Vector2u & pos) const noexcept
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
        LogWrn(_SC("Attempting to <set sprite center for range> using an invalid argument: %d > %d"), first, last);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite center for range> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetRotationAll(SQFloat rot) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->RotateSprite(m_ID, SQMOD_UNKNOWN, rot);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite rotation for all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetRotationFor(const Reference< CPlayer > & player, SQFloat rot) const noexcept
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->RotateSprite(m_ID, player, rot);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <set sprite rotation for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite rotation for player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetRotationRange(SQInt32 first, SQInt32 last, SQFloat rot) const noexcept
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
        LogWrn(_SC("Attempting to <set sprite rotation for range> using an invalid argument: %d > %d"), first, last);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite rotation for range> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetAlphaAll(Uint8 alpha) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSpriteAlpha(m_ID, SQMOD_UNKNOWN, alpha);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite alpha for all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetAlphaFor(const Reference< CPlayer > & player, Uint8 alpha) const noexcept
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->SetSpriteAlpha(m_ID, player, alpha);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <set sprite alpha for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite alpha for player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSprite::SetAlphaRange(SQInt32 first, SQInt32 last, Uint8 alpha) const noexcept
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
        LogWrn(_SC("Attempting to <set sprite alpha for range> using an invalid argument: %d > %d"), first, last);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sprite alpha for range> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CSprite::GetFilePath() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return RefType::Get(m_ID).Path.c_str();
    }
    else
    {
        LogWrn(_SC("Attempting to <get sprite file path> using an invalid reference: %d"), m_ID);
    }

    return _SC("");
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
    // Output debugging information
    LogDbg("Beginning registration of <CSprite> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CSprite"), Sqrat::DerivedClass< CSprite, Reference< CSprite > >(vm, _SC("CSprite"))
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
        .Overload< void (CSprite::*)(const Vector2u &) const >
            (_SC("set_position_all"), &CSprite::SetPositionAll)
        .Overload< void (CSprite::*)(SQUnsignedInteger32, SQUnsignedInteger32) const >
            (_SC("set_position_all"), &CSprite::SetPositionAllEx)
        .Overload< void (CSprite::*)(const Reference< CPlayer > &, const Vector2u &) const >
            (_SC("set_position_for"), &CSprite::SetPositionFor)
        .Overload< void (CSprite::*)(const Reference< CPlayer > &, SQUnsignedInteger32, SQUnsignedInteger32) const >
            (_SC("set_position_for"), &CSprite::SetPositionForEx)
        .Overload< void (CSprite::*)(SQInt32, SQInt32, const Vector2u &) const >
            (_SC("set_position_range"), &CSprite::SetPositionRange)
        .Overload< void (CSprite::*)(const Vector2u &) const >
            (_SC("set_center_all"), &CSprite::SetCenterAll)
        .Overload< void (CSprite::*)(SQUnsignedInteger32, SQUnsignedInteger32) const >
            (_SC("set_center_all"), &CSprite::SetCenterAllEx)
        .Overload< void (CSprite::*)(const Reference< CPlayer > &, const Vector2u &) const >
            (_SC("set_center_for"), &CSprite::SetCenterFor)
        .Overload< void (CSprite::*)(const Reference< CPlayer > &, SQUnsignedInteger32, SQUnsignedInteger32) const >
            (_SC("set_center_for"), &CSprite::SetCenterForEx)
        .Overload< void (CSprite::*)(SQInt32, SQInt32, const Vector2u &) const >
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
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
