#include "Entity/Object.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_CObject(HSQUIRRELVM vm)
{
    if (!Register_Reference< CObject >(vm, _SC("BaseObject")))
    {
        LogDbg("Unable to register the base class <BaseObject> for <CObject> type");

        return false;
    }

    LogDbg("Beginning registration of <CObject> type");

    Sqrat::RootTable(vm).Bind(_SC("CObject"), Sqrat::DerivedClass< CObject, Reference< CObject > >(vm, _SC("CObject"))
        .Ctor()
        .Ctor< SQInt32 >()
    );

    LogDbg("Registration of <CObject> type was successful");

    return true;
}

} // Namespace:: SqMod
