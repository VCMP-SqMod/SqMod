// ------------------------------------------------------------------------------------------------
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Datetime.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Date::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqChronoDate");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Date Date::operator + (const Date & o) const
{
    return Date(o);
}

// ------------------------------------------------------------------------------------------------
Date Date::operator - (const Date & o) const
{
    return Date(o);
}

// ------------------------------------------------------------------------------------------------
Date Date::operator * (const Date & o) const
{
    return Date(o);
}

// ------------------------------------------------------------------------------------------------
Date Date::operator / (const Date & o) const
{
    return Date(o);
}

// ================================================================================================
void Register_ChronoDate(HSQUIRRELVM vm, Table & cns)
{

}

} // Namespace:: SqMod