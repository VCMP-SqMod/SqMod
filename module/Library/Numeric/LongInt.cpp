// ------------------------------------------------------------------------------------------------
#include "Library/Numeric/LongInt.hpp"
#include "Library/Numeric/Random.hpp"
#include "Base/Shared.hpp"
#include "Base/DynArg.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cerrno>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(TypenameS, _SC("SLongInt"))
SQMODE_DECL_TYPENAME(TypenameU, _SC("ULongInt"))

// ------------------------------------------------------------------------------------------------
LongInt< Int64 >::LongInt(CSStr text)
    : m_Data(0), m_Text()
{
    m_Data = std::strtoll(text, nullptr, 10);
}

// ------------------------------------------------------------------------------------------------
LongInt< Int64 >::LongInt(CSStr text, Uint32 base)
    : m_Data(0), m_Text()
{
    m_Data = std::strtoll(text, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
LongInt< Int64 > & LongInt< Int64 >::operator = (CSStr text)
{
    m_Data = std::strtoll(text, nullptr, 10);
    return *this;
}

// ------------------------------------------------------------------------------------------------
CSStr LongInt< Int64 >::ToString()
{
    if (std::snprintf(m_Text, sizeof(m_Text), "%llu", m_Data) < 0)
    {
        m_Text[0] = 0;
    }

    return m_Text;
}

// ------------------------------------------------------------------------------------------------
void LongInt< Int64 >::Random()
{
    m_Data = GetRandomInt64();
}

// ------------------------------------------------------------------------------------------------
void LongInt< Int64 >::Random(Type n)
{
    m_Data = GetRandomInt64(n);
}

// ------------------------------------------------------------------------------------------------
void LongInt< Int64 >::Random(Type m, Type n)
{
    m_Data = GetRandomInt64(m, n);
}

// ------------------------------------------------------------------------------------------------
LongInt< Uint64 >::LongInt(CSStr text)
    : m_Data(0), m_Text()
{
    m_Data = std::strtoull(text, nullptr, 10);
}

// ------------------------------------------------------------------------------------------------
LongInt< Uint64 >::LongInt(CSStr text, Uint32 base)
    : m_Data(0), m_Text()
{
    m_Data = std::strtoull(text, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
LongInt< Uint64 > & LongInt< Uint64 >::operator = (CSStr text)
{
    m_Data = std::strtoull(text, nullptr, 10);
    return *this;
}

// ------------------------------------------------------------------------------------------------
CSStr LongInt< Uint64 >::ToString()
{
    if (std::snprintf(m_Text, sizeof(m_Text), "%llu", m_Data) < 0)
    {
        m_Text[0] = 0;
    }

    return m_Text;
}

// ------------------------------------------------------------------------------------------------
void LongInt< Uint64 >::Random()
{
    m_Data = GetRandomUint64();
}

// ------------------------------------------------------------------------------------------------
void LongInt< Uint64 >::Random(Type n)
{
    m_Data = GetRandomUint64(n);
}

// ------------------------------------------------------------------------------------------------
void LongInt< Uint64 >::Random(Type m, Type n)
{
    m_Data = GetRandomUint64(m, n);
}

// ------------------------------------------------------------------------------------------------
Int64 PopStackSLong(HSQUIRRELVM vm, SQInteger idx)
{
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return static_cast< Int64 >(val);
        } break;
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return ConvTo< Int64 >::From(val);
        } break;
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return static_cast< Int64 >(val);
        } break;
        case OT_STRING:
        {
            CSStr val = nullptr;
            // Attempt to retrieve and convert the string
            if (SQ_SUCCEEDED(sq_getstring(vm, idx, &val)) && val != nullptr && *val != '\0')
            {
                return std::strtoll(val, nullptr, 10);
            }
        } break;
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return static_cast< Int64 >(sq_getsize(vm, idx));
        } break;
        case OT_INSTANCE:
        {
            // Attempt to treat the value as a signed long instance
            try
            {
                return Var< const SLongInt & >(vm, idx).value.GetNum();
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to treat the value as a unsigned long instance
            try
            {
                return ConvTo< Int64 >::From(Var< const ULongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to get the size of the instance as a fall back
            return static_cast< Int64 >(sq_getsize(vm, idx));
        } break;
        default: break;
    }
    // Default to 0
    return 0;
}

// ------------------------------------------------------------------------------------------------
Uint64 PopStackULong(HSQUIRRELVM vm, SQInteger idx)
{
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return ConvTo< Uint64 >::From(val);
        } break;
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return ConvTo< Uint64 >::From(val);
        } break;
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return ConvTo< Uint64 >::From(val);
        } break;
        case OT_STRING:
        {
            CSStr val = nullptr;
            // Attempt to retrieve and convert the string
            if (SQ_SUCCEEDED(sq_getstring(vm, idx, &val)) && val != nullptr && *val != '\0')
            {
                return std::strtoull(val, nullptr, 10);
            }
        } break;
        case OT_ARRAY:
        case OT_TABLE:
        case OT_CLASS:
        case OT_USERDATA:
        {
            return ConvTo< Uint64 >::From(sq_getsize(vm, idx));
        } break;
        case OT_INSTANCE:
        {
            // Attempt to treat the value as a signed long instance
            try
            {
                return ConvTo< Uint64 >::From(Var< const SLongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to treat the value as a unsigned long instance
            try
            {
                return Var< const ULongInt & >(vm, idx).value.GetNum();
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to get the size of the instance as a fall back
            return ConvTo< Uint64 >::From(sq_getsize(vm, idx));
        } break;
        default: break;
    }
    // Default to 0
    return 0;
}

// ================================================================================================
void Register_LongInt(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(TypenameS::Str,
        Class< SLongInt >(vm, TypenameS::Str)
        // Constructors
        .Ctor()
        .Ctor< SLongInt::Type >()
        .template Ctor< CCStr, SQInteger >()
        // Properties
        .Prop(_SC("Str"), &SLongInt::GetCStr, &SLongInt::SetStr)
        .Prop(_SC("Num"), &SLongInt::GetSNum, &SLongInt::SetNum)
        // Core Meta-methods
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, SLongInt, ULongInt >)
        .SquirrelFunc(_SC("_typename"), &TypenameS::Fn)
        .Func(_SC("_tostring"), &SLongInt::ToString)
        // Core Functions
        .Func(_SC("tointeger"), &SLongInt::ToSqInteger)
        .Func(_SC("tofloat"), &SLongInt::ToSqFloat)
        .Func(_SC("tostring"), &SLongInt::ToSqString)
        .Func(_SC("tobool"), &SLongInt::ToSqBool)
        .Func(_SC("tochar"), &SLongInt::ToSqChar)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, SLongInt, ULongInt >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, SLongInt, ULongInt >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, SLongInt, ULongInt >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, SLongInt, ULongInt >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, SLongInt, ULongInt >)
        .Func< SLongInt (SLongInt::*)(void) const >(_SC("_unm"), &SLongInt::operator -)
        // Functions
        .Func(_SC("GetStr"), &SLongInt::GetCStr)
        .Func(_SC("SetStr"), &SLongInt::SetStr)
        .Func(_SC("GetNum"), &SLongInt::GetSNum)
        .Func(_SC("SetNum"), &SLongInt::SetNum)
        // Overloads
        .Overload< void (SLongInt::*)(void) >(_SC("Random"), &SLongInt::Random)
        .Overload< void (SLongInt::*)(SLongInt::Type) >(_SC("Random"), &SLongInt::Random)
        .Overload< void (SLongInt::*)(SLongInt::Type, SLongInt::Type) >(_SC("Random"), &SLongInt::Random)
    );

    RootTable(vm).Bind(TypenameU::Str,
        Class< ULongInt >(vm, TypenameU::Str)
        // Constructors
        .Ctor()
        .Ctor< ULongInt::Type >()
        .Ctor< CCStr, SQInteger >()
        // Properties
        .Prop(_SC("Str"), &ULongInt::GetCStr, &ULongInt::SetStr)
        .Prop(_SC("Num"), &ULongInt::GetSNum, &ULongInt::SetNum)
        // Core Meta-methods
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, ULongInt, SLongInt >)
        .SquirrelFunc(_SC("_typename"), &TypenameU::Fn)
        .Func(_SC("_tostring"), &ULongInt::ToString)
        // Core Functions
        .Func(_SC("tointeger"), &ULongInt::ToSqInteger)
        .Func(_SC("tofloat"), &ULongInt::ToSqFloat)
        .Func(_SC("tostring"), &ULongInt::ToSqString)
        .Func(_SC("tobool"), &ULongInt::ToSqBool)
        .Func(_SC("tochar"), &ULongInt::ToSqChar)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, ULongInt, SLongInt >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, ULongInt, SLongInt >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, ULongInt, SLongInt >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, ULongInt, SLongInt >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, CSStr, ULongInt, SLongInt >)
        .Func< ULongInt (ULongInt::*)(void) const >(_SC("_unm"), &ULongInt::operator -)
        // Functions
        .Func(_SC("GetStr"), &ULongInt::GetCStr)
        .Func(_SC("SetStr"), &ULongInt::SetStr)
        .Func(_SC("GetNum"), &ULongInt::GetSNum)
        .Func(_SC("SetNum"), &ULongInt::SetNum)
        // Overloads
        .Overload< void (ULongInt::*)(void) >(_SC("Random"), &ULongInt::Random)
        .Overload< void (ULongInt::*)(ULongInt::Type) >(_SC("Random"), &ULongInt::Random)
        .Overload< void (ULongInt::*)(ULongInt::Type, ULongInt::Type) >(_SC("Random"), &ULongInt::Random)
    );
}

} // Namespace:: SqMod
