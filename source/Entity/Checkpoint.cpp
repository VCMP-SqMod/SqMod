#include "Entity/Checkpoint.hpp"
#include "Core.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Color4   CCheckpoint::s_Color4;
Vector3  CCheckpoint::s_Vector3;

// ------------------------------------------------------------------------------------------------
SQUint32  CCheckpoint::s_ColorR;
SQUint32  CCheckpoint::s_ColorG;
SQUint32  CCheckpoint::s_ColorB;
SQUint32  CCheckpoint::s_ColorA;

// ------------------------------------------------------------------------------------------------
CCheckpoint::CCheckpoint(const Reference< CCheckpoint > & o) noexcept
    : Reference(o)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
bool CCheckpoint::IsStreamedFor(const Reference< CPlayer > & player) const noexcept
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->IsCheckpointStreamedForPlayer(m_ID, player);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <see if checkpoint is streamed for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <see if checkpoint is streamed for player> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CCheckpoint::GetWorld() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetCheckpointWorld(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get checkpoint world> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetWorld(SQInt32 world) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointWorld(m_ID, world);
    }
    else
    {
        LogWrn(_SC("Attempting to <set checkpoint world> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Color4 & CCheckpoint::GetColor() const noexcept
{
    // Clear any previous color
    s_Color4.Clear();
    // Attempt to retrieve the color
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetCheckpointColor(m_ID, &s_ColorR, &s_ColorG, &s_ColorB, &s_ColorA);
        s_Color4.Set(s_ColorR, s_ColorG, s_ColorB, s_ColorA);
    }
    else
    {
        LogWrn(_SC("Attempting to <get checkpoint color> using an invalid reference: %d"), m_ID);
    }
    // Return the color that could be retrieved
    return s_Color4;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColor(const Color4 & col) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointColor(m_ID, col.r, col.g, col.b, col.a);
    }
    else
    {
        LogWrn(_SC("Attempting to <set checkpoint color> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointColor(m_ID, r, g, b, a);
    }
    else
    {
        LogWrn(_SC("Attempting to <set checkpoint color> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CCheckpoint::GetPosition() const noexcept
{
    // Clear any previous position
    s_Vector3.Clear();
    // Attempt to retrieve the position
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetCheckpointPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get checkpoint position> using an invalid reference: %d"), m_ID);
    }
    // Return the position that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPosition(const Vector3 & pos) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointPos(m_ID, pos.x, pos.y, pos.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set checkpoint position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointPos(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set checkpoint position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQFloat CCheckpoint::GetRadius() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetCheckpointRadius(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get checkpoint radius> using an invalid reference: %d"), m_ID);
    }

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetRadius(SQFloat radius) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointRadius(m_ID, radius);
    }
    else
    {
        LogWrn(_SC("Attempting to <set checkpoint radius> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
Reference< CPlayer > CCheckpoint::GetOwner() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return Reference< CPlayer >(_Func->GetCheckpointOwner(m_ID));
    }
    else
    {
        LogWrn(_SC("Attempting to <get checkpoint owner> using an invalid reference: %d"), m_ID);
    }

    return Reference< CPlayer >();
}

// ------------------------------------------------------------------------------------------------
SQInt32 CCheckpoint::GetOwnerID() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetCheckpointOwner(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get checkpoint owner id> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
Reference< CCheckpoint > CreateBaseCheckpoint_PEF(SQInt32 player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                            SQFloat radius) noexcept
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CCheckpoint > CreateBaseCheckpoint_PEF(SQInt32 player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                            SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CCheckpoint > CreateBaseCheckpoint_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius) noexcept
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CCheckpoint > CreateBaseCheckpoint_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CCheckpoint > CreateBaseCheckpoint_EF(const Reference< CPlayer > & player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                            SQFloat radius) noexcept
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CCheckpoint > CreateBaseCheckpoint_EF(const Reference< CPlayer > & player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                            SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CCheckpoint > CreateBaseCheckpoint_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius) noexcept
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CCheckpoint > CreateBaseCheckpoint_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CCheckpoint CreateCheckpoint_PEF(SQInt32 player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                SQFloat radius) noexcept
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CCheckpoint CreateCheckpoint_PEF(SQInt32 player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                SQFloat radius,
                                SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CCheckpoint CreateCheckpoint_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius) noexcept
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CCheckpoint CreateCheckpoint_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CCheckpoint CreateCheckpoint_EF(const Reference< CPlayer > & player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                SQFloat radius) noexcept
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CCheckpoint CreateCheckpoint_EF(const Reference< CPlayer > & player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                SQFloat radius,
                                SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CCheckpoint CreateCheckpoint_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius) noexcept
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CCheckpoint CreateCheckpoint_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                header, payload);
}

// ================================================================================================
bool Register_CCheckpoint(HSQUIRRELVM vm)
{
    // Attempt to register the base reference type before the actual implementation
    if (!Register_Reference< CCheckpoint >(vm, _SC("BaseCheckpoint")))
    {
        LogFtl("Unable to register the base class <BaseCheckpoint> for <CCheckpoint> type");
        // Registration failed
        return false;
    }
    // Typedef the base reference type for simplicity
    typedef Reference< CCheckpoint > RefType;
    // Output debugging information
    LogDbg("Beginning registration of <CCheckpoint> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CCheckpoint"), Sqrat::DerivedClass< CCheckpoint, RefType >(vm, _SC("CCheckpoint"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        /* Properties */
        .Prop(_SC("world"), &CCheckpoint::GetWorld, &CCheckpoint::SetWorld)
        .Prop(_SC("color"), &CCheckpoint::GetColor, &CCheckpoint::SetColor)
        .Prop(_SC("position"), &CCheckpoint::GetPosition, &CCheckpoint::SetPosition)
        .Prop(_SC("radius"), &CCheckpoint::GetRadius, &CCheckpoint::SetRadius)
        .Prop(_SC("owner"), &CCheckpoint::GetOwner)
        .Prop(_SC("owner_id"), &CCheckpoint::GetOwnerID)
        /* Functions */
        .Func(_SC("streamed_for"), &CCheckpoint::IsStreamedFor)
        .Func(_SC("set_color"), &CCheckpoint::SetColorEx)
        .Func(_SC("set_position"), &CCheckpoint::SetPositionEx)
    );
    // Output debugging information
    LogDbg("Registration of <CCheckpoint> type was successful");
    // Output debugging information
    LogDbg("Beginning registration of <Checkpoint functions> type");
    // Register global functions related to this entity type
    Sqrat::RootTable(vm)
    /* Create BaseCheckpoint [P]rimitive [E]xtended [F]Full */
    .Overload< RefType (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, Uint8, SQFloat) >
        (_SC("CreateBaseCheckpoint_PEF"), &CreateBaseCheckpoint_PEF)
    .Overload< RefType (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, Uint8, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateBaseCheckpoint_PEF"), &CreateBaseCheckpoint_PEF)
    /* Create BaseCheckpoint [P]rimitive [C]ompact [F]ull */
    .Overload< RefType (*)(SQInt32, SQInt32, const Vector3 &, const Color4 &, SQFloat) >
        (_SC("CreateBaseCheckpoint_PCF"), &CreateBaseCheckpoint_PCF)
    .Overload< RefType (*)(SQInt32, SQInt32, const Vector3 &, const Color4 &, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateBaseCheckpoint_PCF"), &CreateBaseCheckpoint_PCF)
    /* Create BaseCheckpoint [E]xtended [F]Full */
    .Overload< RefType (*)(const Reference< CPlayer > &, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, Uint8, SQFloat) >
        (_SC("CreateBaseCheckpoint_EF"), &CreateBaseCheckpoint_EF)
    .Overload< RefType (*)(const Reference< CPlayer > &, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, Uint8, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateBaseCheckpoint_EF"), &CreateBaseCheckpoint_EF)
    /* Create BaseCheckpoint [C]ompact [F]ull */
    .Overload< RefType (*)(const Reference< CPlayer > &, SQInt32, const Vector3 &, const Color4 &, SQFloat) >
        (_SC("CreateBaseCheckpoint_CF"), &CreateBaseCheckpoint_CF)
    .Overload< RefType (*)(const Reference< CPlayer > &, SQInt32, const Vector3 &, const Color4 &, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateBaseCheckpoint_CF"), &CreateBaseCheckpoint_CF)
    /* Create CCheckpoint [P]rimitive [E]xtended [F]Full */
    .Overload< CCheckpoint (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, Uint8, SQFloat) >
        (_SC("CreateCheckpoint_PEF"), &CreateCheckpoint_PEF)
    .Overload< CCheckpoint (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, Uint8, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateCheckpoint_PEF"), &CreateCheckpoint_PEF)
    /* Create CCheckpoint [P]rimitive [C]ompact [F]ull */
    .Overload< CCheckpoint (*)(SQInt32, SQInt32, const Vector3 &, const Color4 &, SQFloat) >
        (_SC("CreateCheckpoint_PCF"), &CreateCheckpoint_PCF)
    .Overload< CCheckpoint (*)(SQInt32, SQInt32, const Vector3 &, const Color4 &, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateCheckpoint_PCF"), &CreateCheckpoint_PCF)
    /* Create CCheckpoint [E]xtended [F]Full */
    .Overload< CCheckpoint (*)(const Reference< CPlayer > &, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, Uint8, SQFloat) >
        (_SC("CreateCheckpoint_EF"), &CreateCheckpoint_EF)
    .Overload< CCheckpoint (*)(const Reference< CPlayer > &, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, Uint8, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateCheckpoint_EF"), &CreateCheckpoint_EF)
    /* Create CCheckpoint [C]ompact [F]ull */
    .Overload< CCheckpoint (*)(const Reference< CPlayer > &, SQInt32, const Vector3 &, const Color4 &, SQFloat) >
        (_SC("CreateCheckpoint_CF"), &CreateCheckpoint_CF)
    .Overload< CCheckpoint (*)(const Reference< CPlayer > &, SQInt32, const Vector3 &, const Color4 &, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateCheckpoint_CF"), &CreateCheckpoint_CF);
    // Output debugging information
    LogDbg("Registration of <Checkpoint functions> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
