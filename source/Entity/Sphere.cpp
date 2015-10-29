#include "Entity/Sphere.hpp"
#include "Base/Color3.hpp"
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
    // Output debugging information
    LogDbg("Beginning registration of <CSphere> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CSphere"), Sqrat::DerivedClass< CSphere, Reference< CSphere > >(vm, _SC("CSphere"))
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
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
