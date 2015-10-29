#include "Entity/Checkpoint.hpp"
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
    // Output debugging information
    LogDbg("Beginning registration of <CCheckpoint> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CCheckpoint"), Sqrat::DerivedClass< CCheckpoint, Reference< CCheckpoint > >(vm, _SC("CCheckpoint"))
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
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
