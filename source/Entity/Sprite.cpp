#include "Entity/Sprite.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_CSprite(HSQUIRRELVM vm)
{
    if (!Register_Reference< CSprite >(vm, _SC("BaseSprite")))
    {
        LogDbg("Unable to register the base class <BaseSprite> for <CSprite> type");

        return false;
    }

    LogDbg("Beginning registration of <CSprite> type");

    Sqrat::RootTable(vm).Bind(_SC("CSprite"), Sqrat::DerivedClass< CSprite, Reference< CSprite > >(vm, _SC("CSprite"))
        .Ctor()
        .Ctor< SQInt32 >()
    );

    LogDbg("Registration of <CSprite> type was successful");

    return true;
}

} // Namespace:: SqMod
