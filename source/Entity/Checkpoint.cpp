// ------------------------------------------------------------------------------------------------
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
CCheckpoint::CCheckpoint(const Reference< CCheckpoint > & o)
    : Reference(o)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
bool CCheckpoint::IsStreamedFor(const Reference< CPlayer > & player) const
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->IsCheckpointStreamedForPlayer(m_ID, player);
    }
    else if (!player)
    {
        BadArg("streamed_for", "see whether is streamed for player", _SCI32(player));
    }
    else
    {
        BadRef("streamed_for", "see whether is streamed for player");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CCheckpoint::GetWorld() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetCheckpointWorld(m_ID);
    }
    else
    {
        BadRef("@world", "get world");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetWorld(SQInt32 world) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointWorld(m_ID, world);
    }
    else
    {
        BadRef("@world", "set world");
    }
}

// ------------------------------------------------------------------------------------------------
const Color4 & CCheckpoint::GetColor() const
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
        BadRef("@color", "get color");
    }
    // Return the color that could be retrieved
    return s_Color4;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColor(const Color4 & col) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointColor(m_ID, col.r, col.g, col.b, col.a);
    }
    else
    {
        BadRef("@color", "set color");
    }
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetColorEx(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointColor(m_ID, r, g, b, a);
    }
    else
    {
        BadRef("set_color", "set color");
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CCheckpoint::GetPosition() const
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
        BadRef("@position", "get position");
    }
    // Return the position that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPosition(const Vector3 & pos) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointPos(m_ID, pos.x, pos.y, pos.z);
    }
    else
    {
        BadRef("@position", "set position");
    }
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointPos(m_ID, x, y, z);
    }
    else
    {
        BadRef("set_position", "set position");
    }
}

// ------------------------------------------------------------------------------------------------
SQFloat CCheckpoint::GetRadius() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetCheckpointRadius(m_ID);
    }
    else
    {
        BadRef("@radius", "get radius");
    }

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
void CCheckpoint::SetRadius(SQFloat radius) const
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetCheckpointRadius(m_ID, radius);
    }
    else
    {
        BadRef("@radius", "set radius");
    }
}

// ------------------------------------------------------------------------------------------------
Reference< CPlayer > CCheckpoint::GetOwner() const
{
    if (VALID_ENTITY(m_ID))
    {
        return Reference< CPlayer >(_Func->GetCheckpointOwner(m_ID));
    }
    else
    {
        BadRef("@owner", "get owner");
    }

    return Reference< CPlayer >();
}

// ------------------------------------------------------------------------------------------------
SQInt32 CCheckpoint::GetOwnerID() const
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetCheckpointOwner(m_ID);
    }
    else
    {
        BadRef("@owner_id", "get owner id");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
Reference< CCheckpoint > CreateBaseCheckpoint_PEF(SQInt32 player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                            SQFloat radius)
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CCheckpoint > CreateBaseCheckpoint_PEF(SQInt32 player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                            SQFloat radius,
                            SQInt32 header, SqObj & payload)
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CCheckpoint > CreateBaseCheckpoint_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius)
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CCheckpoint > CreateBaseCheckpoint_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload)
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CCheckpoint > CreateBaseCheckpoint_EF(const Reference< CPlayer > & player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                            SQFloat radius)
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CCheckpoint > CreateBaseCheckpoint_EF(const Reference< CPlayer > & player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                            SQFloat radius,
                            SQInt32 header, SqObj & payload)
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CCheckpoint > CreateBaseCheckpoint_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius)
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CCheckpoint > CreateBaseCheckpoint_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload)
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CCheckpoint CreateCheckpoint_PEF(SQInt32 player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                SQFloat radius)
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CCheckpoint CreateCheckpoint_PEF(SQInt32 player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                SQFloat radius,
                                SQInt32 header, SqObj & payload)
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CCheckpoint CreateCheckpoint_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius)
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CCheckpoint CreateCheckpoint_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload)
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CCheckpoint CreateCheckpoint_EF(const Reference< CPlayer > & player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                SQFloat radius)
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CCheckpoint CreateCheckpoint_EF(const Reference< CPlayer > & player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                SQFloat radius,
                                SQInt32 header, SqObj & payload)
{
    return _Core->NewCheckpoint(player, world, x, y, z, r, g, b, a, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CCheckpoint CreateCheckpoint_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius)
{
    return _Core->NewCheckpoint(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CCheckpoint CreateCheckpoint_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color4 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload)
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
    LogDbg("Beginning registration of <Checkpoint> functions");
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
    LogDbg("Registration of <Checkpoint> functions was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
