// ------------------------------------------------------------------------------------------------
#include "JValue.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
std::size_t JValue::s_Flags = JSON_ENCODE_ANY;

// ------------------------------------------------------------------------------------------------
SQInteger JValue::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqJSONValue");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Object JValue::ToString() const
{
    // Dump the values to a string
    const CStrGuard csg(json_dumps(m_Ptr, s_Flags));
    // Remember the current stack size
    const StackGuard sg;
    // Transform the string into a script object
    sq_pushstring(_SqVM, csg.mPtr ? csg.mPtr : _SC(""), -1);
    // Return the created script object
    return Var< Object >(_SqVM, -1).value;
}

// ------------------------------------------------------------------------------------------------
Object JValue::GetValue() const
{
    return Object();
}

// ================================================================================================
void Register_JValue(Table & jns)
{
    jns.Bind(_SC("Value"), Class< JValue >(jns.GetVM(), _SC("SqJSONValue"))
        // Constructors
        .Ctor()
        .Ctor< const JValue & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &JValue::Cmp)
        .SquirrelFunc(_SC("_typename"), &JValue::Typename)
        .Func(_SC("_tostring"), &JValue::ToString)
        // Properties
        .Prop(_SC("IsObject"), &JValue::IsObject)
        .Prop(_SC("IsArray"), &JValue::IsArray)
        .Prop(_SC("IsString"), &JValue::IsString)
        .Prop(_SC("IsInteger"), &JValue::IsInteger)
        .Prop(_SC("IsReal"), &JValue::IsReal)
        .Prop(_SC("IsNumber"), &JValue::IsNumber)
        .Prop(_SC("IsTrue"), &JValue::IsTrue)
        .Prop(_SC("IsFalse"), &JValue::IsFalse)
        .Prop(_SC("IsBoolean"), &JValue::IsBoolean)
        .Prop(_SC("IsNull"), &JValue::IsNull)
        .Prop(_SC("Value"), &JValue::GetValue)
        // Member Methods
        //.Func(_SC("Func"), &JValue::Func)
    );
}

} // Namespace:: SqMod
