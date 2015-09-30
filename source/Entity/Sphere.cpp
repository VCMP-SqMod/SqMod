#include "Entity/Sphere.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_CSphere(HSQUIRRELVM vm)
{
    if (!Register_Reference< CSphere >(vm, _SC("BaseSphere")))
    {
        LogDbg("Unable to register the base class <BaseSphere> for <CSphere> type");

        return false;
    }

    LogDbg("Beginning registration of <CSphere> type");

    Sqrat::RootTable(vm).Bind(_SC("CSphere"), Sqrat::DerivedClass< CSphere, Reference< CSphere > >(vm, _SC("CSphere"))
        .Ctor()
        .Ctor< SQInt32 >()
    );

    LogDbg("Registration of <CSphere> type was successful");

    return true;
}

} // Namespace:: SqMod
