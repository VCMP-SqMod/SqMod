#include "Entity/Pickup.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_CPickup(HSQUIRRELVM vm)
{
    if (!Register_Reference< CPickup >(vm, _SC("BasePickup")))
    {
        LogDbg("Unable to register the base class <BasePickup> for <CPickup> type");

        return false;
    }

    LogDbg("Beginning registration of <CPickup> type");

    Sqrat::RootTable(vm).Bind(_SC("CPickup"), Sqrat::DerivedClass< CPickup, Reference< CPickup > >(vm, _SC("CPickup"))
        .Ctor()
        .Ctor< SQInt32 >()
    );

    LogDbg("Registration of <CPickup> type was successful");

    return true;
}

} // Namespace:: SqMod
