#include "Entity/Blip.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_CBlip(HSQUIRRELVM vm)
{
    if (!Register_Reference< CBlip >(vm, _SC("BaseBlip")))
    {
        LogDbg("Unable to register the base class <BaseBlip> for <CBlip> type");

        return false;
    }

    LogDbg("Beginning registration of <CBlip> type");

    Sqrat::RootTable(vm).Bind(_SC("CBlip"), Sqrat::DerivedClass< CBlip, Reference< CBlip > >(vm, _SC("CBlip"))
        .Ctor()
        .Ctor< SQInt32 >()
    );

    LogDbg("Registration of <CBlip> type was successful");

    return true;
}

} // Namespace:: SqMod
