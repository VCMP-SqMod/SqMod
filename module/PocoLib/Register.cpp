// ------------------------------------------------------------------------------------------------
#include "PocoLib/Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void Register_POCO_Crypto(HSQUIRRELVM vm, Table & ns);
extern void Register_POCO_Data(HSQUIRRELVM vm, Table & ns);
extern void Register_POCO_Net(HSQUIRRELVM vm, Table & ns);
extern void Register_POCO_Time(HSQUIRRELVM vm, Table & ns);
extern void Register_POCO_Util(HSQUIRRELVM vm, Table & ns);

// ================================================================================================
void Register_POCO(HSQUIRRELVM vm)
{
    Table ns(vm);

	Register_POCO_Crypto(vm, ns);
	Register_POCO_Data(vm, ns);
	Register_POCO_Net(vm, ns);
	Register_POCO_Time(vm, ns);
	Register_POCO_Util(vm, ns);

    RootTable(vm).Bind(_SC("Sq"), ns);
}

} // Namespace:: SqMod
