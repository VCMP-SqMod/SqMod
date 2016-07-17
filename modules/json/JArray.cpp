// ------------------------------------------------------------------------------------------------
#include "JArray.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
std::size_t JArray::s_Flags = JSON_ENCODE_ANY;

// ------------------------------------------------------------------------------------------------
SQInteger JArray::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqJSONArray");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Object JArray::ToString() const
{
    // Dump the values to a string
    const CStrGuard csg(json_dumps(m_Ptr, s_Flags));
    // Remember the current stack size
    const StackGuard sg;
    // Transform the string into a script object
    sq_pushstring(DefaultVM::Get(), csg.mPtr ? csg.mPtr : _SC(""), -1);
    // Return the created script object
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ================================================================================================
void Register_JArray(Table & jns)
{
    jns.Bind(_SC("Array"), Class< JArray >(jns.GetVM(), _SC("SqJSONArray"))
        // Constructors
        .Ctor()
        .Ctor< const JArray & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &JArray::Cmp)
        .SquirrelFunc(_SC("_typename"), &JArray::Typename)
        .Func(_SC("_tostring"), &JArray::ToString)
        // Properties
        //.Prop(_SC("Prop"), &JArray::Prop)
        // Member Methods
        //.Func(_SC("Func"), &JArray::Func)
    );
}

} // Namespace:: SqMod
