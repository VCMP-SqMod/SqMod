// ------------------------------------------------------------------------------------------------
#include "Library/Numeric.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void Register_LongInt(HSQUIRRELVM vm);
extern void Register_Math(HSQUIRRELVM vm);
extern void Register_Random(HSQUIRRELVM vm);

// ================================================================================================
void Register_Numeric(HSQUIRRELVM vm)
{
    Register_LongInt(vm);
    Register_Math(vm);
    Register_Random(vm);
}

} // Namespace:: SqMod
