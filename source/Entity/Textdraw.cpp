#include "Entity/Textdraw.hpp"
#include "Base/Vector2i.hpp"
#include "Core.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CTextdraw::CTextdraw(const Reference< CTextdraw > & o)
    : Reference(o)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::ShowAll() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->ShowTextdraw(m_ID, SQMOD_UNKNOWN);
    }
    else
    {
        LogWrn(_SC("Attempting to <show textdraw to all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::ShowFor(const Reference< CPlayer > & player) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->ShowTextdraw(m_ID, player);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <show textdraw to player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <show textdraw to player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::ShowRange(SQInt32 first, SQInt32 last) const
{
    if (VALID_ENTITY(m_ID) && (last >= first))
    {
        for (; first <= last; ++first)
        {
            if (Reference< CPlayer >::Verify(first))
            {
                _Func->ShowTextdraw(m_ID, first);
            }
        }
    }
    else if (first < last)
    {
        LogWrn(_SC("Attempting to <show textdraw to range> using an invalid argument: %d > %d"), first, last);
    }
    else
    {
        LogWrn(_SC("Attempting to <show textdraw to range> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::HideAll() const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->HideTextdraw(m_ID, SQMOD_UNKNOWN);
    }
    else
    {
        LogWrn(_SC("Attempting to <hide textdraw to all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::HideFor(const Reference< CPlayer > & player) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->HideTextdraw(m_ID, player);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <hide textdraw to player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <hide textdraw to player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::HideRange(SQInt32 first, SQInt32 last) const
{
    if (VALID_ENTITY(m_ID) && (last >= first))
    {
        for (; first <= last; ++first)
        {
            if (Reference< CPlayer >::Verify(first))
            {
                _Func->HideTextdraw(m_ID, first);
            }
        }
    }
    else if (first < last)
    {
        LogWrn(_SC("Attempting to <hide textdraw to range> using an invalid argument: %d > %d"), first, last);
    }
    else
    {
        LogWrn(_SC("Attempting to <hide textdraw to range> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetPositionAll(const Vector2i & pos) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveTextdraw(m_ID, SQMOD_UNKNOWN, pos.x, pos.y);
    }
    else
    {
        LogWrn(_SC("Attempting to <set textdraw position for all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetPositionAllEx(SQInt32 x, SQInt32 y) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->MoveTextdraw(m_ID, SQMOD_UNKNOWN, x, y);
    }
    else
    {
        LogWrn(_SC("Attempting to <set textdraw position for all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetPositionFor(const Reference< CPlayer > & player, const Vector2i & pos) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->MoveTextdraw(m_ID, player, pos.x, pos.y);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <set textdraw position for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <set textdraw position for player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetPositionForEx(const Reference< CPlayer > & player, SQInt32 x, SQInt32 y) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->MoveTextdraw(m_ID, player, x, y);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <set textdraw position for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <set textdraw position for player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetPositionRange(SQInt32 first, SQInt32 last, const Vector2i & pos) const
{
    if (VALID_ENTITY(m_ID) && (last >= first))
    {
        for (; first <= last; ++first)
        {
            if (Reference< CPlayer >::Verify(first))
            {
                _Func->MoveTextdraw(m_ID, first, pos.x, pos.y);
            }
        }
    }
    else if (first < last)
    {
        LogWrn(_SC("Attempting to <set textdraw position for range> using an invalid argument: %d > %d"), first, last);
    }
    else
    {
        LogWrn(_SC("Attempting to <set textdraw position for range> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetColorAll(const Color4 & col) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetTextdrawColour(m_ID, SQMOD_UNKNOWN, col.GetRGBA());
    }
    else
    {
        LogWrn(_SC("Attempting to <set textdraw color for all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetColorAllEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetTextdrawColour(m_ID, SQMOD_UNKNOWN, PACK_RGBA(r, g, b, a));
    }
    else
    {
        LogWrn(_SC("Attempting to <set textdraw color for all> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetColorFor(const Reference< CPlayer > & player, const Color4 & col) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->SetTextdrawColour(m_ID, player, col.GetRGBA());
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <set textdraw color for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <set textdraw color for player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetColorForEx(const Reference< CPlayer > & player, Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        _Func->SetTextdrawColour(m_ID, player, PACK_RGBA(r, g, b, a));
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <set textdraw color for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <set textdraw color for player> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CTextdraw::SetColorRange(SQInt32 first, SQInt32 last, const Color4 & col) const
{
    if (VALID_ENTITY(m_ID) && (last >= first))
    {
        for (const SQUint32 color = col.GetRGBA(); first <= last; ++first)
        {
            if (Reference< CPlayer >::Verify(first))
            {
                _Func->SetTextdrawColour(m_ID, first, color);
            }
        }
    }
    else if (first < last)
    {
        LogWrn(_SC("Attempting to <set textdraw color for range> using an invalid argument: %d > %d"), first, last);
    }
    else
    {
        LogWrn(_SC("Attempting to <set textdraw color for range> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CTextdraw::GetText() const
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Text.c_str();
    }
    else
    {
        LogWrn(_SC("Attempting to <get textdraw text> using an invalid reference: %d"), m_ID);
    }

    return _SC("");
}

// ------------------------------------------------------------------------------------------------
Reference< CTextdraw > CreateBaseTextdraw_ES(const SQChar * text,
                        SQInt32 xp, SQInt32 yp,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                        bool rel)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, xp, yp, PACK_ARGB(a, r, g, b), rel,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CTextdraw > CreateBaseTextdraw_ES(const SQChar * text,
                        SQInt32 xp, SQInt32 yp,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                        bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, xp, yp, PACK_ARGB(a, r, g, b), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CTextdraw > CreateBaseTextdraw_EF(SQInt32 index, const SQChar * text,
                        SQInt32 xp, SQInt32 yp,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                        bool rel)
{
    return _Core->NewTextdraw(index,text, xp, yp, PACK_ARGB(a, r, g, b), rel,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CTextdraw > CreateBaseTextdraw_EF(SQInt32 index, const SQChar * text,
                        SQInt32 xp, SQInt32 yp,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                        bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewTextdraw(index, text, xp, yp, PACK_ARGB(a, r, g, b), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CTextdraw > CreateBaseTextdraw_CS(const SQChar * text,
                        const Vector2i & pos, const Color4 & color, bool rel)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, pos.x, pos.y, color.GetARGB(), rel,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CTextdraw > CreateBaseTextdraw_CS(const SQChar * text,
                        const Vector2i & pos, const Color4 & color, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, pos.x, pos.y, color.GetARGB(), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CTextdraw > CreateBaseTextdraw_CF(SQInt32 index, const SQChar * text,
                        const Vector2i & pos, const Color4 & color, bool rel)
{
    return _Core->NewTextdraw(index, text, pos.x, pos.y, color.GetARGB(), rel,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CTextdraw > CreateBaseTextdraw_CF(SQInt32 index, const SQChar * text,
                        const Vector2i & pos, const Color4 & color, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewTextdraw(index, text, pos.x, pos.y, color.GetARGB(), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CTextdraw CreateTextdraw_ES(const SQChar * text,
                        SQInt32 xp, SQInt32 yp,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                        bool rel)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, xp, yp, PACK_ARGB(a, r, g, b), rel,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CTextdraw CreateTextdraw_ES(const SQChar * text,
                        SQInt32 xp, SQInt32 yp,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                        bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, xp, yp, PACK_ARGB(a, r, g, b), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CTextdraw CreateTextdraw_EF(SQInt32 index, const SQChar * text,
                        SQInt32 xp, SQInt32 yp,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                        bool rel)
{
    return _Core->NewTextdraw(index,text, xp, yp, PACK_ARGB(a, r, g, b), rel,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CTextdraw CreateTextdraw_EF(SQInt32 index, const SQChar * text,
                        SQInt32 xp, SQInt32 yp,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                        bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewTextdraw(index, text, xp, yp, PACK_ARGB(a, r, g, b), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CTextdraw CreateTextdraw_CS(const SQChar * text,
                        const Vector2i & pos, const Color4 & color, bool rel)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, pos.x, pos.y, color.GetARGB(), rel,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CTextdraw CreateTextdraw_CS(const SQChar * text,
                        const Vector2i & pos, const Color4 & color, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewTextdraw(SQMOD_UNKNOWN, text, pos.x, pos.y, color.GetARGB(), rel,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CTextdraw CreateTextdraw_CF(SQInt32 index, const SQChar * text,
                        const Vector2i & pos, const Color4 & color, bool rel)
{
    return _Core->NewTextdraw(index, text, pos.x, pos.y, color.GetARGB(), rel,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CTextdraw CreateTextdraw_CF(SQInt32 index, const SQChar * text,
                        const Vector2i & pos, const Color4 & color, bool rel,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewTextdraw(index, text, pos.x, pos.y, color.GetARGB(), rel,
                                header, payload);
}

// ================================================================================================
bool Register_CTextdraw(HSQUIRRELVM vm)
{
    // Attempt to register the base reference type before the actual implementation
    if (!Register_Reference< CTextdraw >(vm, _SC("BaseTextdraw")))
    {
        LogFtl("Unable to register the base class <BaseTextdraw> for <CTextdraw> type");
        // Registration failed
        return false;
    }
    // Typedef the base reference type for simplicity
    typedef Reference< CTextdraw > RefType;
    // Output debugging information
    LogDbg("Beginning registration of <CTextdraw> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CTextdraw"), Sqrat::DerivedClass< CTextdraw, RefType >(vm, _SC("CTextdraw"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        /* Properties */
        .Prop(_SC("text"), &CTextdraw::GetText)
        /* Functions */
        .Func(_SC("show_all"), &CTextdraw::ShowAll)
        .Func(_SC("show_for"), &CTextdraw::ShowFor)
        .Func(_SC("show_range"), &CTextdraw::ShowRange)
        .Func(_SC("hide_all"), &CTextdraw::HideAll)
        .Func(_SC("hide_for"), &CTextdraw::HideFor)
        .Func(_SC("hide_range"), &CTextdraw::HideRange)
        /* Overloads */
        .Overload< void (CTextdraw::*)(const Vector2i &) const >
            (_SC("set_position_all"), &CTextdraw::SetPositionAll)
        .Overload< void (CTextdraw::*)(SQInt32, SQInt32) const >
            (_SC("set_position_all"), &CTextdraw::SetPositionAllEx)
        .Overload< void (CTextdraw::*)(const Reference< CPlayer > &, const Vector2i &) const >
            (_SC("set_position_for"), &CTextdraw::SetPositionFor)
        .Overload< void (CTextdraw::*)(const Reference< CPlayer > &, SQInt32, SQInt32) const >
            (_SC("set_position_for"), &CTextdraw::SetPositionForEx)
        .Overload< void (CTextdraw::*)(SQInt32, SQInt32, const Vector2i &) const >
            (_SC("set_position_range"), &CTextdraw::SetPositionRange)
        .Overload< void (CTextdraw::*)(const Color4 &) const >
            (_SC("set_color_all"), &CTextdraw::SetColorAll)
        .Overload< void (CTextdraw::*)(Uint8, Uint8, Uint8, Uint8) const >
            (_SC("set_color_all"), &CTextdraw::SetColorAllEx)
        .Overload< void (CTextdraw::*)(const Reference< CPlayer > &, const Color4 &) const >
            (_SC("set_color_for"), &CTextdraw::SetColorFor)
        .Overload< void (CTextdraw::*)(const Reference< CPlayer > &, Uint8, Uint8, Uint8, Uint8) const >
            (_SC("set_color_for"), &CTextdraw::SetColorForEx)
        .Overload< void (CTextdraw::*)(SQInt32, SQInt32, const Color4 &) const >
            (_SC("set_color_range"), &CTextdraw::SetColorRange)
    );
    // Output debugging information
    LogDbg("Registration of <CTextdraw> type was successful");
    // Output debugging information
    LogDbg("Beginning registration of <Textdraw functions> type");
    // Register global functions related to this entity type
    Sqrat::RootTable(vm)
    /* Create BaseTextdraw [E]xtended [S]ubstitute */
    .Overload< RefType (*)(const SQChar *, SQInt32, SQInt32, Uint8, Uint8, Uint8, Uint8, bool) >
        (_SC("CreateBaseTextdraw_ES"), &CreateBaseTextdraw_ES)
    .Overload< RefType (*)(const SQChar *, SQInt32, SQInt32, Uint8, Uint8, Uint8, Uint8, bool, SQInt32, SqObj &) >
        (_SC("CreateBaseTextdraw_ES"), &CreateBaseTextdraw_ES)
    /* Create BaseTextdraw [E]xtended [F]Full */
    .Overload< RefType (*)(SQInt32, const SQChar *, SQInt32, SQInt32, Uint8, Uint8, Uint8, Uint8, bool) >
        (_SC("CreateBaseTextdraw_EF"), &CreateBaseTextdraw_EF)
    .Overload< RefType (*)(SQInt32, const SQChar *, SQInt32, SQInt32, Uint8, Uint8, Uint8, Uint8, bool, SQInt32, SqObj &) >
        (_SC("CreateBaseTextdraw_EF"), &CreateBaseTextdraw_EF)
    /* Create BaseTextdraw [C]ompact [S]ubstitute */
    .Overload< RefType (*)(const SQChar *, const Vector2i &, const Color4 &, bool) >
        (_SC("CreateBaseTextdraw_CS"), &CreateBaseTextdraw_CS)
    .Overload< RefType (*)(const SQChar *, const Vector2i &, const Color4 &, bool, SQInt32, SqObj &) >
        (_SC("CreateBaseTextdraw_CS"), &CreateBaseTextdraw_CS)
    /* Create BaseTextdraw [C]ompact [F]ull */
    .Overload< RefType (*)(SQInt32, const SQChar *, const Vector2i &, const Color4 &, bool) >
        (_SC("CreateBaseTextdraw_CF"), &CreateBaseTextdraw_CF)
    .Overload< RefType (*)(SQInt32, const SQChar *, const Vector2i &, const Color4 &, bool, SQInt32, SqObj &) >
        (_SC("CreateBaseTextdraw_CF"), &CreateBaseTextdraw_CF)
    /* Create CTextdraw [E]xtended [S]ubstitute */
    .Overload< CTextdraw (*)(const SQChar *, SQInt32, SQInt32, Uint8, Uint8, Uint8, Uint8, bool) >
        (_SC("CreateTextdraw_ES"), &CreateTextdraw_ES)
    .Overload< CTextdraw (*)(const SQChar *, SQInt32, SQInt32, Uint8, Uint8, Uint8, Uint8, bool, SQInt32, SqObj &) >
        (_SC("CreateTextdraw_ES"), &CreateTextdraw_ES)
    /* Create CTextdraw [E]xtended [F]Full */
    .Overload< CTextdraw (*)(SQInt32, const SQChar *, SQInt32, SQInt32, Uint8, Uint8, Uint8, Uint8, bool) >
        (_SC("CreateTextdraw_EF"), &CreateTextdraw_EF)
    .Overload< CTextdraw (*)(SQInt32, const SQChar *, SQInt32, SQInt32, Uint8, Uint8, Uint8, Uint8, bool, SQInt32, SqObj &) >
        (_SC("CreateTextdraw_EF"), &CreateTextdraw_EF)
    /* Create CTextdraw [C]ompact [S]ubstitute */
    .Overload< CTextdraw (*)(const SQChar *, const Vector2i &, const Color4 &, bool) >
        (_SC("CreateTextdraw_CS"), &CreateTextdraw_CS)
    .Overload< CTextdraw (*)(const SQChar *, const Vector2i &, const Color4 &, bool, SQInt32, SqObj &) >
        (_SC("CreateTextdraw_CS"), &CreateTextdraw_CS)
    /* Create CTextdraw [C]ompact [F]ull */
    .Overload< CTextdraw (*)(SQInt32, const SQChar *, const Vector2i &, const Color4 &, bool) >
        (_SC("CreateTextdraw_CF"), &CreateTextdraw_CF)
    .Overload< CTextdraw (*)(SQInt32, const SQChar *, const Vector2i &, const Color4 &, bool, SQInt32, SqObj &) >
        (_SC("CreateTextdraw_CF"), &CreateTextdraw_CF);
    // Output debugging information
    LogDbg("Registration of <Textdraw functions> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
