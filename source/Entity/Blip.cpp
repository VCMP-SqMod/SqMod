#include "Entity/Blip.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger CBlip::GetWorld() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).World;
    }
    else
    {
        LogWrn(_SC("Attempting to <get blip world> using an invalid reference: %d"), m_ID);
    }
    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInteger CBlip::GetScale() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Scale;
    }
    else
    {
        LogWrn(_SC("Attempting to <get blip scale> using an invalid reference: %d"), m_ID);
    }
    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CBlip::GetPosition() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Position;
    }
    else
    {
        LogWrn(_SC("Attempting to <get blip position> using an invalid reference: %d"), m_ID);
    }

    return Vector3::NIL;
}

// ------------------------------------------------------------------------------------------------
SQFloat CBlip::GetPositionX() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Position.x;
    }
    else
    {
        LogWrn(_SC("Attempting to <get blip position> using an invalid reference: %d"), m_ID);
    }

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
SQFloat CBlip::GetPositionY() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Position.y;
    }
    else
    {
        LogWrn(_SC("Attempting to <get blip position> using an invalid reference: %d"), m_ID);
    }

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
SQFloat CBlip::GetPositionZ() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Position.z;
    }
    else
    {
        LogWrn(_SC("Attempting to <get blip position> using an invalid reference: %d"), m_ID);
    }

    return 0.0;
}

// ------------------------------------------------------------------------------------------------
const Color4 & CBlip::GetColor() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Color;
    }
    else
    {
        LogWrn(_SC("Attempting to <get blip color> using an invalid reference: %d"), m_ID);
    }

    return Color4::NIL;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CBlip::GetSprID() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).SprID;
    }
    else
    {
        LogWrn(_SC("Attempting to <get blip sprite> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ================================================================================================
bool Register_CBlip(HSQUIRRELVM vm)
{
    // Attempt to register the base reference type before the actual implementation
    if (!Register_Reference< CBlip >(vm, _SC("BaseBlip")))
    {
        LogFtl("Unable to register the base class <BaseBlip> for <CBlip> type");
        // Registration failed
        return false;
    }
    // Output debugging information
    LogDbg("Beginning registration of <CBlip> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CBlip"), Sqrat::DerivedClass< CBlip, Reference< CBlip > >(vm, _SC("CBlip"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        /* Properties */
        .Prop(_SC("world"), &CBlip::GetWorld)
        .Prop(_SC("scale"), &CBlip::GetScale)
        .Prop(_SC("pos"), &CBlip::GetPosition)
        .Prop(_SC("position"), &CBlip::GetPosition)
        .Prop(_SC("color"), &CBlip::GetColor)
        .Prop(_SC("spr_id"), &CBlip::GetSprID)
        .Prop(_SC("pos_x"), &CBlip::GetPositionX)
        .Prop(_SC("pos_y"), &CBlip::GetPositionY)
        .Prop(_SC("pos_z"), &CBlip::GetPositionZ)
    );
    // Output debugging information
    LogDbg("Registration of <CBlip> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
