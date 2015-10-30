#include "Entity/Keybind.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
    // Output debugging information
    LogDbg("Beginning registration of <CKeybind> type");
    // Attempt to register the actual reference that implements all of the entity functionality
    Sqrat::RootTable(vm).Bind(_SC("CKeybind"), Sqrat::DerivedClass< CKeybind, Reference< CKeybind > >(vm, _SC("CKeybind"))
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
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
