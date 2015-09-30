#include "Entity/Keybind.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_CKeybind(HSQUIRRELVM vm)
{
    if (!Register_Reference< CKeybind >(vm, _SC("BaseKeybind")))
    {
        LogDbg("Unable to register the base class <BaseKeybind> for <CKeybind> type");

        return false;
    }

    LogDbg("Beginning registration of <CKeybind> type");

    Sqrat::RootTable(vm).Bind(_SC("CKeybind"), Sqrat::DerivedClass< CKeybind, Reference< CKeybind > >(vm, _SC("CKeybind"))
        .Ctor()
        .Ctor< SQInt32 >()
    );

    LogDbg("Registration of <CKeybind> type was successful");

    return true;
}

} // Namespace:: SqMod
