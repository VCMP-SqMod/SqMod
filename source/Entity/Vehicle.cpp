#include "Entity/Vehicle.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_CVehicle(HSQUIRRELVM vm)
{
    if (!Register_Reference< CVehicle >(vm, _SC("BaseVehicle")))
    {
        LogDbg("Unable to register the base class <BaseVehicle> for <CVehicle> type");

        return false;
    }

    LogDbg("Beginning registration of <CVehicle> type");

    Sqrat::RootTable(vm).Bind(_SC("CVehicle"), Sqrat::DerivedClass< CVehicle, Reference< CVehicle > >(vm, _SC("CVehicle"))
        .Ctor()
        .Ctor< SQInt32 >()
    );

    LogDbg("Registration of <CVehicle> type was successful");

    return true;
}

} // Namespace:: SqMod
