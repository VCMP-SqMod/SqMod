#include "Entity/Pickup.hpp"
#include "Misc/Model.hpp"
#include "Core.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CModel CPickup::s_Model;

// ------------------------------------------------------------------------------------------------
Vector3 CPickup::s_Vector3;

// ------------------------------------------------------------------------------------------------
CPickup::CPickup(const Reference< CPickup > & o)
    : Reference(o)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
bool CPickup::IsStreamedFor(const Reference< CPlayer > & player) const
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
const CModel & CPickup::GetModel() const
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
SQInt32 CPickup::GetModelID() const
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
SQInt32 CPickup::GetWorld() const
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
void CPickup::SetWorld(SQInt32 world) const
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
SQInt32 CPickup::GetAlpha() const
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
void CPickup::SetAlpha(SQInt32 alpha) const
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
bool CPickup::GetAutomatic() const
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
void CPickup::SetAutomatic(bool toggle) const
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
SQInt32 CPickup::GetAutoTimer() const
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
void CPickup::SetAutoTimer(SQInt32 timer) const
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
void CPickup::Refresh() const
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
const Vector3 & CPickup::GetPosition()
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
void CPickup::SetPosition(const Vector3 & pos) const
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
void CPickup::SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const
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
SQInt32 CPickup::GetQuantity() const
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

// ------------------------------------------------------------------------------------------------
Reference< CPickup > CreateBasePickup_PEF(SQInt32 model, SQInt32 world, SQInt32 quantity,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha, bool automatic)
{
    return _Core->NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CPickup > CreateBasePickup_PEF(SQInt32 model, SQInt32 world, SQInt32 quantity,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha, bool automatic,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CPickup > CreateBasePickup_PCF(SQInt32 model, SQInt32 world, SQInt32 quantity,
                        const Vector3 & pos, SQInt32 alpha, bool automatic)
{
    return _Core->NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CPickup > CreateBasePickup_PCF(SQInt32 model, SQInt32 world, SQInt32 quantity,
                        const Vector3 & pos, SQInt32 alpha, bool automatic,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CPickup > CreateBasePickup_EF(const CModel & model, SQInt32 world, SQInt32 quantity,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha, bool automatic)
{
    return _Core->NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CPickup > CreateBasePickup_EF(const CModel & model, SQInt32 world, SQInt32 quantity,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha, bool automatic,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CPickup > CreateBasePickup_CF(const CModel & model, SQInt32 world, SQInt32 quantity,
                        const Vector3 & pos, SQInt32 alpha, bool automatic)
{
    return _Core->NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CPickup > CreateBasePickup_CF(const CModel & model, SQInt32 world, SQInt32 quantity,
                        const Vector3 & pos, SQInt32 alpha, bool automatic,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CPickup CreatePickup_PEF(SQInt32 model, SQInt32 world, SQInt32 quantity,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha, bool automatic)
{
    return _Core->NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CPickup CreatePickup_PEF(SQInt32 model, SQInt32 world, SQInt32 quantity,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha, bool automatic,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CPickup CreatePickup_PCF(SQInt32 model, SQInt32 world, SQInt32 quantity,
                        const Vector3 & pos, SQInt32 alpha, bool automatic)
{
    return _Core->NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CPickup CreatePickup_PCF(SQInt32 model, SQInt32 world, SQInt32 quantity,
                        const Vector3 & pos, SQInt32 alpha, bool automatic,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CPickup CreatePickup_EF(const CModel & model, SQInt32 world, SQInt32 quantity,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha, bool automatic)
{
    return _Core->NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CPickup CreatePickup_EF(const CModel & model, SQInt32 world, SQInt32 quantity,
                        SQFloat x, SQFloat y, SQFloat z,
                        SQInt32 alpha, bool automatic,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewPickup(model, world, quantity, x, y, z, alpha, automatic,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CPickup CreatePickup_CF(const CModel & model, SQInt32 world, SQInt32 quantity,
                        const Vector3 & pos, SQInt32 alpha, bool automatic)
{
    return _Core->NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CPickup CreatePickup_CF(const CModel & model, SQInt32 world, SQInt32 quantity,
                        const Vector3 & pos, SQInt32 alpha, bool automatic,
                        SQInt32 header, SqObj & payload)
{
    return _Core->NewPickup(model, world, quantity, pos.x, pos.y, pos.z, alpha, automatic,
                            header, payload);
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
    // Typedef the base reference type for simplicity
    typedef Reference< CPickup > RefType;
    // Output debugging information
    LogDbg("Beginning registration of <CPickup> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CPickup"), Sqrat::DerivedClass< CPickup, RefType >(vm, _SC("CPickup"))
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
    // Output debugging information
    LogDbg("Beginning registration of <Pickup functions> type");
    // Register global functions related to this entity type
    Sqrat::RootTable(vm)
    /* Create BasePickup [P]rimitive [E]xtended [F]Full */
    .Overload< RefType (*)(SQInt32, SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, bool) >
        (_SC("CreateBasePickup_PEF"), &CreateBasePickup_PEF)
    .Overload< RefType (*)(SQInt32, SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreateBasePickup_PEF"), &CreateBasePickup_PEF)
    /* Create BasePickup [P]rimitive [C]ompact [F]ull */
    .Overload< RefType (*)(SQInt32, SQInt32, SQInt32, const Vector3 &, SQInt32, bool) >
        (_SC("CreateBasePickup_PCF"), &CreateBasePickup_PCF)
    .Overload< RefType (*)(SQInt32, SQInt32, SQInt32, const Vector3 &, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreateBasePickup_PCF"), &CreateBasePickup_PCF)
    /* Create BasePickup [E]xtended [F]Full */
    .Overload< RefType (*)(const CModel &, SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, bool) >
        (_SC("CreateBasePickup_EF"), &CreateBasePickup_EF)
    .Overload< RefType (*)(const CModel &, SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreateBasePickup_EF"), &CreateBasePickup_EF)
    /* Create BasePickup [C]ompact [F]ull */
    .Overload< RefType (*)(const CModel &, SQInt32, SQInt32, const Vector3 &, SQInt32, bool) >
        (_SC("CreateBasePickup_CF"), &CreateBasePickup_CF)
    .Overload< RefType (*)(const CModel &, SQInt32, SQInt32, const Vector3 &, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreateBasePickup_CF"), &CreateBasePickup_CF)
    /* Create CPickup [P]rimitive [E]xtended [F]Full */
    .Overload< CPickup (*)(SQInt32, SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, bool) >
        (_SC("CreatePickup_PEF"), &CreatePickup_PEF)
    .Overload< CPickup (*)(SQInt32, SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreatePickup_PEF"), &CreatePickup_PEF)
    /* Create CPickup [P]rimitive [C]ompact [F]ull */
    .Overload< CPickup (*)(SQInt32, SQInt32, SQInt32, const Vector3 &, SQInt32, bool) >
        (_SC("CreatePickup_PCF"), &CreatePickup_PCF)
    .Overload< CPickup (*)(SQInt32, SQInt32, SQInt32, const Vector3 &, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreatePickup_PCF"), &CreatePickup_PCF)
    /* Create CPickup [E]xtended [F]Full */
    .Overload< CPickup (*)(const CModel &, SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, bool) >
        (_SC("CreatePickup_EF"), &CreatePickup_EF)
    .Overload< CPickup (*)(const CModel &, SQInt32, SQInt32, SQFloat, SQFloat, SQFloat, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreatePickup_EF"), &CreatePickup_EF)
    /* Create CPickup [C]ompact [F]ull */
    .Overload< CPickup (*)(const CModel &, SQInt32, SQInt32, const Vector3 &, SQInt32, bool) >
        (_SC("CreatePickup_CF"), &CreatePickup_CF)
    .Overload< CPickup (*)(const CModel &, SQInt32, SQInt32, const Vector3 &, SQInt32, bool, SQInt32, SqObj &) >
        (_SC("CreatePickup_CF"), &CreatePickup_CF);
    // Output debugging information
    LogDbg("Registration of <Pickup functions> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
