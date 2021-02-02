// ------------------------------------------------------------------------------------------------
#include "PocoLib/Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern Register_POCO_Crypto(HSQUIRRELVM vm, Table & ns);
extern Register_POCO_Data(HSQUIRRELVM vm, Table & ns);
extern Register_POCO_JSON(HSQUIRRELVM vm, Table & ns);
extern Register_POCO_Net(HSQUIRRELVM vm, Table & ns);
extern Register_POCO_RegEx(HSQUIRRELVM vm, Table & ns);
extern Register_POCO_Time(HSQUIRRELVM vm, Table & ns);
extern Register_POCO_Util(HSQUIRRELVM vm, Table & ns);
extern Register_POCO_XML(HSQUIRRELVM vm, Table & ns);

// ================================================================================================
void Register_POCO(HSQUIRRELVM vm)
{
    Table ns(vm);

	Register_POCO_Crypto(vm, ns);
	Register_POCO_Data(vm, ns);
	Register_POCO_JSON(vm, ns);
	Register_POCO_Net(vm, ns);
	Register_POCO_RegEx(vm, ns);
	Register_POCO_Time(vm, ns);
	Register_POCO_Util(vm, ns);
	Register_POCO_XML(vm, ns);

    RootTable(vm).Bind(_SC("Sq"), ns);
}

} // Namespace:: SqMod
