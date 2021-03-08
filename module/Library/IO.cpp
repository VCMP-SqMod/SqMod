// ------------------------------------------------------------------------------------------------
#include "Library/IO.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void Register_Buffer(HSQUIRRELVM vm);
extern void Register_Stream(HSQUIRRELVM vm);
extern void Register_INI(HSQUIRRELVM vm);

// ================================================================================================
void Register_IO(HSQUIRRELVM vm)
{
    Register_Buffer(vm);
    Register_Stream(vm);
    Register_INI(vm);
}

} // Namespace:: SqMod
