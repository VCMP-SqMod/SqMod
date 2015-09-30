#include "Entity/Textdraw.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_CTextdraw(HSQUIRRELVM vm)
{
    if (!Register_Reference< CTextdraw >(vm, _SC("BaseTextdraw")))
    {
        LogDbg("Unable to register the base class <BaseTextdraw> for <CTextdraw> type");

        return false;
    }

    LogDbg("Beginning registration of <CTextdraw> type");

    Sqrat::RootTable(vm).Bind(_SC("CTextdraw"), Sqrat::DerivedClass< CTextdraw, Reference< CTextdraw > >(vm, _SC("CTextdraw"))
        .Ctor()
        .Ctor< SQInt32 >()
    );

    LogDbg("Registration of <CTextdraw> type was successful");

    return true;
}

} // Namespace:: SqMod
