#include "Entity/Keybind.hpp"
#include "Core.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CKeybind::CKeybind(const Reference< CKeybind > & o) noexcept
    : Reference(o)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
SQInt32 CKeybind::GetPrimary() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Primary;
    }
    else
    {
        LogWrn(_SC("Attempting to <get keybind primary keycode> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CKeybind::GetSecondary() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Secondary;
    }
    else
    {
        LogWrn(_SC("Attempting to <get keybind secondary keycode> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CKeybind::GetAlternative() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Alternative;
    }
    else
    {
        LogWrn(_SC("Attempting to <get keybind alternative keycode> using an invalid reference: %d"), m_ID);
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
bool CKeybind::IsRelease() const noexcept
{
    if (VALID_ENTITY(m_ID))
    {
        RefType::Get(m_ID).Release;
    }
    else
    {
        LogWrn(_SC("Attempting to <see if keybind reacts on release> using an invalid reference: %d"), m_ID);
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
Reference< CKeybind > CreateBaseKeybind_ES(bool release,
                        SQInt32 primary, SQInt32 secondary, SQInt32 alternative) noexcept
{
    return _Core->NewKeybind(SQMOD_UNKNOWN, release, primary, secondary, alternative,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CKeybind > CreateBaseKeybind_ES(bool release,
                        SQInt32 primary, SQInt32 secondary, SQInt32 alternative,
                        SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewKeybind(SQMOD_UNKNOWN, release, primary, secondary, alternative,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
Reference< CKeybind > CreateBaseKeybind_EF(SQInt32 slot, bool release,
                        SQInt32 primary, SQInt32 secondary, SQInt32 alternative) noexcept
{
    return _Core->NewKeybind(slot, release, primary, secondary, alternative,
                            SQMOD_CREATE_DEFAULT, NullData());
}

Reference< CKeybind > CreateBaseKeybind_EF(SQInt32 slot, bool release,
                        SQInt32 primary, SQInt32 secondary, SQInt32 alternative,
                        SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewKeybind(slot, release, primary, secondary, alternative,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CKeybind CreateKeybind_ES(bool release,
                        SQInt32 primary, SQInt32 secondary, SQInt32 alternative) noexcept
{
    return _Core->NewKeybind(SQMOD_UNKNOWN, release, primary, secondary, alternative,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CKeybind CreateKeybind_ES(bool release,
                        SQInt32 primary, SQInt32 secondary, SQInt32 alternative,
                        SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewKeybind(SQMOD_UNKNOWN, release, primary, secondary, alternative,
                            header, payload);
}

// ------------------------------------------------------------------------------------------------
CKeybind CreateKeybind_EF(SQInt32 slot, bool release,
                        SQInt32 primary, SQInt32 secondary, SQInt32 alternative) noexcept
{
    return _Core->NewKeybind(slot, release, primary, secondary, alternative,
                            SQMOD_CREATE_DEFAULT, NullData());
}

CKeybind CreateKeybind_EF(SQInt32 slot, bool release,
                        SQInt32 primary, SQInt32 secondary, SQInt32 alternative,
                        SQInt32 header, SqObj & payload) noexcept
{
    return _Core->NewKeybind(slot, release, primary, secondary, alternative,
                            header, payload);
}

// ================================================================================================
bool Register_CKeybind(HSQUIRRELVM vm)
{
    // Attempt to register the base reference type before the actual implementation
    if (!Register_Reference< CKeybind >(vm, _SC("BaseKeybind")))
    {
        LogFtl("Unable to register the base class <BaseKeybind> for <CKeybind> type");
        // Registration failed
        return false;
    }
    // Typedef the base reference type for simplicity
    typedef Reference< CKeybind > RefType;
    // Output debugging information
    LogDbg("Beginning registration of <CKeybind> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CKeybind"), Sqrat::DerivedClass< CKeybind, RefType >(vm, _SC("CKeybind"))
        /* Constructors */
        .Ctor()
        .Ctor< SQInt32 >()
        /* Properties */
        .Prop(_SC("primary"), &CKeybind::GetPrimary)
        .Prop(_SC("secondary"), &CKeybind::GetSecondary)
        .Prop(_SC("alternative"), &CKeybind::GetAlternative)
        .Prop(_SC("is_release"), &CKeybind::IsRelease)
    );
    // Output debugging information
    LogDbg("Registration of <CKeybind> type was successful");
    // Output debugging information
    LogDbg("Beginning registration of <Keybind functions> type");
    // Register global functions related to this entity type
    Sqrat::RootTable(vm)
    /* Create BaseKeybind [E]xtended [S]ubstitute */
    .Overload< RefType (*)(bool, SQInt32, SQInt32, SQInt32) >
        (_SC("CreateBaseKeybind_ES"), &CreateBaseKeybind_ES)
    .Overload< RefType (*)(bool, SQInt32, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseKeybind_ES"), &CreateBaseKeybind_ES)
    /* Create BaseKeybind [E]xtended [F]Full */
    .Overload< RefType (*)(SQInt32, bool, SQInt32, SQInt32, SQInt32) >
        (_SC("CreateBaseKeybind_EF"), &CreateBaseKeybind_EF)
    .Overload< RefType (*)(SQInt32, bool, SQInt32, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateBaseKeybind_EF"), &CreateBaseKeybind_EF)
    /* Create CKeybind [E]xtended [S]ubstitute */
    .Overload< CKeybind (*)(bool, SQInt32, SQInt32, SQInt32) >
        (_SC("CreateKeybind_ES"), &CreateKeybind_ES)
    .Overload< CKeybind (*)(bool, SQInt32, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateKeybind_ES"), &CreateKeybind_ES)
    /* Create CKeybind [E]xtended [F]Full */
    .Overload< CKeybind (*)(SQInt32, bool, SQInt32, SQInt32, SQInt32) >
        (_SC("CreateKeybind_EF"), &CreateKeybind_EF)
    .Overload< CKeybind (*)(SQInt32, bool, SQInt32, SQInt32, SQInt32, SQInt32, SqObj &) >
        (_SC("CreateKeybind_EF"), &CreateKeybind_EF);
    // Output debugging information
    LogDbg("Registration of <Keybind functions> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
