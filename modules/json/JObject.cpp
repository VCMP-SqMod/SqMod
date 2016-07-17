// ------------------------------------------------------------------------------------------------
#include "JObject.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
std::size_t JObject::s_Flags = JSON_ENCODE_ANY;

// ------------------------------------------------------------------------------------------------
SQInteger JObject::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqJSONObject");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Object JObject::ToString() const
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
void Register_JObject(Table & jns)
{
    jns.Bind(_SC("Object"), Class< JObject >(jns.GetVM(), _SC("SqJSONObject"))
        // Constructors
        .Ctor()
        .Ctor< const JObject & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &JObject::Cmp)
        .SquirrelFunc(_SC("_typename"), &JObject::Typename)
        .Func(_SC("_tostring"), &JObject::ToString)
        // Properties
        //.Prop(_SC("Prop"), &JObject::Prop)
        // Member Methods
        //.Func(_SC("Func"), &JObject::Func)
    );
}

} // Namespace:: SqMod
