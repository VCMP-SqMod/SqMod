#include "Entity/Sphere.hpp"
#include "Base/Color3.hpp"
#include "Core.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Color3   CSphere::s_Color3;
Vector3  CSphere::s_Vector3;

// ------------------------------------------------------------------------------------------------
SQUint32  CSphere::s_ColorR;
SQUint32  CSphere::s_ColorG;
SQUint32  CSphere::s_ColorB;

// ------------------------------------------------------------------------------------------------
CSphere::CSphere(const Reference< CSphere > & o) noexcept
    : Reference(o)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
bool CSphere::IsStreamedFor(const Reference< CPlayer > & player) const noexcept
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->IsSphereStreamedForPlayer(m_ID, player);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <see if sphere is streamed for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <see if sphere is streamed for player> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CSphere::GetWorld() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetSphereWorld(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get sphere world> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CSphere::SetWorld(SQInt32 world) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSphereWorld(m_ID, world);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sphere world> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Color3 & CSphere::GetColor() const noexcept
{
    // Clear any previous color
    s_Color3.Clear();
    // Attempt to retrieve the color
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetSphereColor(m_ID, &s_ColorR, &s_ColorG, &s_ColorB);
        s_Color3.Set(s_ColorR, s_ColorG, s_ColorB);
    }
    else
    {
        LogWrn(_SC("Attempting to <get sphere color> using an invalid reference: %d"), m_ID);
    }
    // Return the color that could be retrieved
    return s_Color3;
}

// ------------------------------------------------------------------------------------------------
void CSphere::SetColor(const Color3 & col) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSphereColor(m_ID, col.r, col.g, col.b);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sphere color> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSphere::SetColorEx(Uint8 r, Uint8 g, Uint8 b) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSphereColor(m_ID, r, g, b);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sphere color> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CSphere::GetPosition() const noexcept
{
    // Clear any previous position
    s_Vector3.Clear();
    // Attempt to retrieve the position
    if (VALID_ENTITY(m_ID))
    {
        _Func->GetSpherePos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get sphere position> using an invalid reference: %d"), m_ID);
    }
    // Return the position that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CSphere::SetPosition(const Vector3 & pos) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSpherePos(m_ID, pos.x, pos.y, pos.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sphere position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CSphere::SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSpherePos(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sphere position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQFloat CSphere::GetRadius() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetSphereRadius(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get sphere radius> using an invalid reference: %d"), m_ID);
    }

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
void CSphere::SetRadius(SQFloat radius) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetSphereRadius(m_ID, radius);
    }
    else
    {
        LogWrn(_SC("Attempting to <set sphere radius> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
Reference< CPlayer > CSphere::GetOwner() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return Reference< CPlayer >(_Func->GetSphereOwner(m_ID));
    }
    else
    {
        LogWrn(_SC("Attempting to <get sphere owner> using an invalid reference: %d"), m_ID);
    }

    return Reference< CPlayer >();
}

// ------------------------------------------------------------------------------------------------
SQInt32 CSphere::GetOwnerID() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetSphereOwner(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get sphere owner id> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
Reference< CSphere > CreateBaseSphere_PEF(SQInt32 player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b,
                            SQFloat radius) noexcept
{
    return _Core->NewSphere(player, world, x, y, z, r, g, b, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CSphere > CreateBaseSphere_PEF(SQInt32 player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b,
                            SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewSphere(player, world, x, y, z, r, g, b, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CSphere > CreateBaseSphere_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color3 & color, SQFloat radius) noexcept
{
    return _Core->NewSphere(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CSphere > CreateBaseSphere_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color3 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewSphere(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CSphere > CreateBaseSphere_EF(const Reference< CPlayer > & player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b,
                            SQFloat radius) noexcept
{
    return _Core->NewSphere(player, world, x, y, z, r, g, b, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CSphere > CreateBaseSphere_EF(const Reference< CPlayer > & player, SQInt32 world,
                            SQFloat x, SQFloat y, SQFloat z,
                            Uint8 r, Uint8 g, Uint8 b,
                            SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewSphere(player, world, x, y, z, r, g, b, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CSphere > CreateBaseSphere_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color3 & color, SQFloat radius) noexcept
{
    return _Core->NewSphere(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CSphere > CreateBaseSphere_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color3 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewSphere(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CSphere CreateSphere_PEF(SQInt32 player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b,
                                SQFloat radius) noexcept
{
    return _Core->NewSphere(player, world, x, y, z, r, g, b, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CSphere CreateSphere_PEF(SQInt32 player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b,
                                SQFloat radius,
                                SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewSphere(player, world, x, y, z, r, g, b, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CSphere CreateSphere_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color3 & color, SQFloat radius) noexcept
{
    return _Core->NewSphere(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CSphere CreateSphere_PCF(SQInt32 player, SQInt32 world,
                            const Vector3 & pos, const Color3 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewSphere(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CSphere CreateSphere_EF(const Reference< CPlayer > & player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b,
                                SQFloat radius) noexcept
{
    return _Core->NewSphere(player, world, x, y, z, r, g, b, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CSphere CreateSphere_EF(const Reference< CPlayer > & player, SQInt32 world,
                                SQFloat x, SQFloat y, SQFloat z,
                                Uint8 r, Uint8 g, Uint8 b,
                                SQFloat radius,
                                SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewSphere(player, world, x, y, z, r, g, b, radius,
                                header, payload);
}

// ------------------------------------------------------------------------------------------------
CSphere CreateSphere_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color3 & color, SQFloat radius) noexcept
{
    return _Core->NewSphere(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                SQMOD_CREATE_DEFAULT, NullData());
}

CSphere CreateSphere_CF(const Reference< CPlayer > & player, SQInt32 world,
                            const Vector3 & pos, const Color3 & color, SQFloat radius,
                            SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewSphere(player, world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius,
                                header, payload);
}

// ================================================================================================
bool Register_CSphere(HSQUIRRELVM vm)
{
    // Attempt to register the base reference type before the actual implementation
    if (!Register_Reference< CSphere >(vm, _SC("BaseSphere")))
    {
        LogFtl("Unable to register the base class <BaseSphere> for <CSphere> type");
        // Registration failed
        return false;
    }
    // Typedef the base reference type for simplicity
    typedef Reference< CSphere > RefType;
    // Output debugging information
    LogDbg("Beginning registration of <CSphere> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CSphere"), Sqrat::DerivedClass< CSphere, RefType >(vm, _SC("CSphere"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        /* Properties */
        .Prop(_SC("world"), &CSphere::GetWorld, &CSphere::SetWorld)
        .Prop(_SC("color"), &CSphere::GetColor, &CSphere::SetColor)
        .Prop(_SC("position"), &CSphere::GetPosition, &CSphere::SetPosition)
        .Prop(_SC("radius"), &CSphere::GetRadius, &CSphere::SetRadius)
        .Prop(_SC("owner"), &CSphere::GetOwner)
        .Prop(_SC("owner_id"), &CSphere::GetOwnerID)
        /* Functions */
        .Func(_SC("streamed_for"), &CSphere::IsStreamedFor)
        .Func(_SC("set_color"), &CSphere::SetColorEx)
        .Func(_SC("set_position"), &CSphere::SetPositionEx)
    );
    // Output debugging information
    LogDbg("Registration of <CSphere> type was successful");
    // Output debugging information
    LogDbg("Beginning registration of <Sphere functions> type");
    // Register global functions related to this entity type
    Sqrat::RootTable(vm)
    /* Create BaseSphere [P]rimitive [E]xtended [F]Full */
    .Overload< RefType (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, SQFloat) >
        (_SC("CreateBaseSphere_PEF"), &CreateBaseSphere_PEF)
    .Overload< RefType (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateBaseSphere_PEF"), &CreateBaseSphere_PEF)
    /* Create BaseSphere [P]rimitive [C]ompact [F]ull */
    .Overload< RefType (*)(SQInt32, SQInt32, const Vector3 &, const Color3 &, SQFloat) >
        (_SC("CreateBaseSphere_PCF"), &CreateBaseSphere_PCF)
    .Overload< RefType (*)(SQInt32, SQInt32, const Vector3 &, const Color3 &, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateBaseSphere_PCF"), &CreateBaseSphere_PCF)
    /* Create BaseSphere [E]xtended [F]Full */
    .Overload< RefType (*)(const Reference< CPlayer > &, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, SQFloat) >
        (_SC("CreateBaseSphere_EF"), &CreateBaseSphere_EF)
    .Overload< RefType (*)(const Reference< CPlayer > &, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateBaseSphere_EF"), &CreateBaseSphere_EF)
    /* Create BaseSphere [C]ompact [F]ull */
    .Overload< RefType (*)(const Reference< CPlayer > &, SQInt32, const Vector3 &, const Color3 &, SQFloat) >
        (_SC("CreateBaseSphere_CF"), &CreateBaseSphere_CF)
    .Overload< RefType (*)(const Reference< CPlayer > &, SQInt32, const Vector3 &, const Color3 &, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateBaseSphere_CF"), &CreateBaseSphere_CF)
    /* Create CSphere [P]rimitive [E]xtended [F]Full */
    .Overload< CSphere (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, SQFloat) >
        (_SC("CreateSphere_PEF"), &CreateSphere_PEF)
    .Overload< CSphere (*)(SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateSphere_PEF"), &CreateSphere_PEF)
    /* Create CSphere [P]rimitive [C]ompact [F]ull */
    .Overload< CSphere (*)(SQInt32, SQInt32, const Vector3 &, const Color3 &, SQFloat) >
        (_SC("CreateSphere_PCF"), &CreateSphere_PCF)
    .Overload< CSphere (*)(SQInt32, SQInt32, const Vector3 &, const Color3 &, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateSphere_PCF"), &CreateSphere_PCF)
    /* Create CSphere [E]xtended [F]Full */
    .Overload< CSphere (*)(const Reference< CPlayer > &, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, SQFloat) >
        (_SC("CreateSphere_EF"), &CreateSphere_EF)
    .Overload< CSphere (*)(const Reference< CPlayer > &, SQInt32, SQFloat, SQFloat, SQFloat, Uint8, Uint8, Uint8, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateSphere_EF"), &CreateSphere_EF)
    /* Create CSphere [C]ompact [F]ull */
    .Overload< CSphere (*)(const Reference< CPlayer > &, SQInt32, const Vector3 &, const Color3 &, SQFloat) >
        (_SC("CreateSphere_CF"), &CreateSphere_CF)
    .Overload< CSphere (*)(const Reference< CPlayer > &, SQInt32, const Vector3 &, const Color3 &, SQFloat, SQInt32, SqObj &) >
        (_SC("CreateSphere_CF"), &CreateSphere_CF);
    // Output debugging information
    LogDbg("Registration of <Sphere functions> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
