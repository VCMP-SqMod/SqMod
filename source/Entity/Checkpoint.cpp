#include "Entity/Checkpoint.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_CCheckpoint(HSQUIRRELVM vm)
{
    if (!Register_Reference< CCheckpoint >(vm, _SC("BaseCheckpoint")))
    {
        LogDbg("Unable to register the base class <BaseCheckpoint> for <CCheckpoint> type");

        return false;
    }

    LogDbg("Beginning registration of <CCheckpoint> type");

    Sqrat::RootTable(vm).Bind(_SC("CCheckpoint"), Sqrat::DerivedClass< CCheckpoint, Reference< CCheckpoint > >(vm, _SC("CCheckpoint"))
        .Ctor()
        .Ctor< SQInt32 >()
    );

    LogDbg("Registration of <CCheckpoint> type was successful");

    return true;
}

} // Namespace:: SqMod
