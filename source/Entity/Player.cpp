#include "Entity/Player.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_CPlayer(HSQUIRRELVM vm)
{
    if (!Register_Reference< CPlayer >(vm, _SC("BasePlayer")))
    {
        LogDbg("Unable to register the base class <BasePlayer> for <CPlayer> type");

        return false;
    }

    LogDbg("Beginning registration of <CPlayer> type");

    Sqrat::RootTable(vm).Bind(_SC("CPlayer"), Sqrat::DerivedClass< CPlayer, Reference< CPlayer > >(vm, _SC("CPlayer"))
        .Ctor()
        .Ctor< SQInt32 >()
    );

    LogDbg("Registration of <CPlayer> type was successful");

    return true;
}

} // Namespace:: SqMod
