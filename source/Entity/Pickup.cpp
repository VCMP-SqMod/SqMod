#include "Entity/Pickup.hpp"
#include "Misc/Model.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CModel CPickup::s_Model;

// ------------------------------------------------------------------------------------------------
Vector3 CPickup::s_Vector3;

// ------------------------------------------------------------------------------------------------
bool CPickup::IsStreamedFor(const Reference< CPlayer > & player) const noexcept
{
    if (VALID_ENTITY(m_ID) && player)
    {
        return _Func->IsPickupStreamedForPlayer(m_ID, player);
    }
    else if (!player)
    {
        LogWrn(_SC("Attempting to <see if pickup is streamed for player> using an invalid argument: %d"), _SCI32(player));
    }
    else
    {
        LogWrn(_SC("Attempting to <see if pickup is streamed for player> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
const CModel & CPickup::GetModel() const noexcept
{
    // Clear any previous model
    s_Model.SetID(SQMOD_UNKNOWN);
    // Attempt to retrieve the model
    if (VALID_ENTITY(m_ID))
    {
        s_Model.SetID(_Func->PickupGetModel(m_ID));
    }
    else
    {
        LogWrn(_SC("Attempting to <get pickup model> using an invalid reference: %d"), m_ID);
    }
    // Return the model that could be retrieved
    return s_Model;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPickup::GetModelID() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->PickupGetModel(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get pickup model id> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPickup::GetWorld() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPickupWorld(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get pickup world> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetWorld(SQInt32 world) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPickupWorld(m_ID, world);
    }
    else
    {
        LogWrn(_SC("Attempting to <set pickup world> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPickup::GetAlpha() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->PickupGetAlpha(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get pickup alpha> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetAlpha(SQInt32 alpha) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->PickupSetAlpha(m_ID, alpha);
    }
    else
    {
        LogWrn(_SC("Attempting to <set pickup alpha> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
bool CPickup::GetAutomatic() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->PickupIsAutomatic(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <see if pickup is automatic> using an invalid argument: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetAutomatic(bool toggle) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->PickupSetAutomatic(m_ID, toggle);
    }
    else
    {
        LogWrn(_SC("Attempting to <set whether pickup is automatic> using an invalid argument: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPickup::GetAutoTimer() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->GetPickupAutoTimer(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get pickup auto timer> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetAutoTimer(SQInt32 timer) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->SetPickupAutoTimer(m_ID, timer);
    }
    else
    {
        LogWrn(_SC("Attempting to <set pickup auto timer> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CPickup::Refresh() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->PickupRefresh(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <refresh pickup> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
const Vector3 & CPickup::GetPosition() noexcept
{
    // Clear any previous position
    s_Vector3.Clear();
    // Attempt to retrieve the position
    if (VALID_ENTITY(m_ID))
    {
        _Func->PickupGetPos(m_ID, &s_Vector3.x, &s_Vector3.y, &s_Vector3.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <get pickup position> using an invalid reference: %d"), m_ID);
    }
    // Return the position that could be retrieved
    return s_Vector3;
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPosition(const Vector3 & pos) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->PickupSetPos(m_ID, pos.x, pos.y, pos.z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set pickup position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
void CPickup::SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        _Func->PickupSetPos(m_ID, x, y, z);
    }
    else
    {
        LogWrn(_SC("Attempting to <set pickup position> using an invalid reference: %d"), m_ID);
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 CPickup::GetQuantity() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        return _Func->PickupGetQuantity(m_ID);
    }
    else
    {
        LogWrn(_SC("Attempting to <get pickup quantity> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ================================================================================================
bool Register_CPickup(HSQUIRRELVM vm)
{
    // Attempt to register the base reference type before the actual implementation
    if (!Register_Reference< CPickup >(vm, _SC("BasePickup")))
    {
        LogFtl("Unable to register the base class <BasePickup> for <CPickup> type");
        // Registration failed
        return false;
    }
    // Output debugging information
    LogDbg("Beginning registration of <CPickup> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CPickup"), Sqrat::DerivedClass< CPickup, Reference< CPickup > >(vm, _SC("CPickup"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        /* Properties */
        .Prop(_SC("model"), &CPickup::GetModel)
        .Prop(_SC("model_id"), &CPickup::GetModelID)
        .Prop(_SC("world"), &CPickup::GetWorld, &CPickup::SetWorld)
        .Prop(_SC("alpha"), &CPickup::GetAlpha, &CPickup::SetAlpha)
        .Prop(_SC("automatic"), &CPickup::GetAutomatic, &CPickup::SetAutomatic)
        .Prop(_SC("auto_timer"), &CPickup::GetAutoTimer, &CPickup::SetAutoTimer)
        .Prop(_SC("position"), &CPickup::GetPosition, &CPickup::SetPosition)
        .Prop(_SC("quantity"), &CPickup::GetQuantity)
        /* Functions */
        .Func(_SC("streamed_for"), &CPickup::IsStreamedFor)
        .Func(_SC("refresh"), &CPickup::Refresh)
        .Func(_SC("set_position"), &CPickup::SetPositionEx)
    );
    // Output debugging information
    LogDbg("Registration of <CPickup> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
