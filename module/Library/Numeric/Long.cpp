// ------------------------------------------------------------------------------------------------
#include "Library/Numeric/Long.hpp"
#include "Library/Numeric/Random.hpp"
#include "Base/DynArg.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(TypenameS, _SC("SLongInt"))
SQMOD_DECL_TYPENAME(TypenameU, _SC("ULongInt"))

// ------------------------------------------------------------------------------------------------
LongInt< int64_t >::LongInt(const SQChar * text)
    : m_Data(0), m_Text()
{
    m_Data = std::strtoll(text, nullptr, 10);
}

// ------------------------------------------------------------------------------------------------
LongInt< int64_t >::LongInt(const SQChar * text, uint32_t base)
    : m_Data(0), m_Text()
{
    m_Data = std::strtoll(text, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
LongInt< int64_t > & LongInt< int64_t >::operator = (const SQChar * text)
{
    m_Data = std::strtoll(text, nullptr, 10);
    return *this;
}

// ------------------------------------------------------------------------------------------------
const SQChar * LongInt< int64_t >::ToString()
{
    if (std::snprintf(m_Text, sizeof(m_Text), "%llu", m_Data) < 0)
    {
        m_Text[0] = 0;
    }

    return m_Text;
}

// ------------------------------------------------------------------------------------------------
void LongInt< int64_t >::Random()
{
    m_Data = GetRandomInt64();
}

// ------------------------------------------------------------------------------------------------
void LongInt< int64_t >::Random(Type n)
{
    m_Data = GetRandomInt64(n);
}

// ------------------------------------------------------------------------------------------------
void LongInt< int64_t >::Random(Type m, Type n)
{
    m_Data = GetRandomInt64(m, n);
}

// ------------------------------------------------------------------------------------------------
LongInt< uint64_t >::LongInt(const SQChar * text)
    : m_Data(0), m_Text()
{
    m_Data = std::strtoull(text, nullptr, 10);
}

// ------------------------------------------------------------------------------------------------
LongInt< uint64_t >::LongInt(const SQChar * text, uint32_t base)
    : m_Data(0), m_Text()
{
    m_Data = std::strtoull(text, nullptr, base);
}

// ------------------------------------------------------------------------------------------------
LongInt< uint64_t > & LongInt< uint64_t >::operator = (const SQChar * text)
{
    m_Data = std::strtoull(text, nullptr, 10);
    return *this;
}

// ------------------------------------------------------------------------------------------------
const SQChar * LongInt< uint64_t >::ToString()
{
    if (std::snprintf(m_Text, sizeof(m_Text), "%llu", m_Data) < 0)
    {
        m_Text[0] = 0;
    }

    return m_Text;
}

// ------------------------------------------------------------------------------------------------
void LongInt< uint64_t >::Random()
{
    m_Data = GetRandomUint64();
}

// ------------------------------------------------------------------------------------------------
void LongInt< uint64_t >::Random(Type n)
{
    m_Data = GetRandomUint64(n);
}

// ------------------------------------------------------------------------------------------------
void LongInt< uint64_t >::Random(Type m, Type n)
{
    m_Data = GetRandomUint64(m, n);
}

// ------------------------------------------------------------------------------------------------
int64_t PopStackSLong(HSQUIRRELVM vm, SQInteger idx)
{
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return static_cast< int64_t >(val);
        }
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return ConvTo< int64_t >::From(val);
        }
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return static_cast< int64_t >(val);
        }
        case OT_STRING:
        {
            const SQChar * val = nullptr;
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
            return static_cast< int64_t >(sq_getsize(vm, idx));
        }
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
                return ConvTo< int64_t >::From(Var< const ULongInt & >(vm, idx).value.GetNum());
            }
            catch (...)
            {
                // Just ignore it...
            }
            // Attempt to get the size of the instance as a fall back
            return static_cast< int64_t >(sq_getsize(vm, idx));
        }
        default: break;
    }
    // Default to 0
    return 0;
}

// ------------------------------------------------------------------------------------------------
uint64_t PopStackULong(HSQUIRRELVM vm, SQInteger idx)
{
    // Identify which type must be extracted
    switch (sq_gettype(vm, idx))
    {
        case OT_INTEGER:
        {
            SQInteger val;
            sq_getinteger(vm, idx, &val);
            return ConvTo< uint64_t >::From(val);
        }
        case OT_FLOAT:
        {
            SQFloat val;
            sq_getfloat(vm, idx, &val);
            return ConvTo< uint64_t >::From(val);
        }
        case OT_BOOL:
        {
            SQBool val;
            sq_getbool(vm, idx, &val);
            return ConvTo< uint64_t >::From(val);
        }
        case OT_STRING:
        {
            const SQChar * val = nullptr;
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
            return ConvTo< uint64_t >::From(sq_getsize(vm, idx));
        }
        case OT_INSTANCE:
        {
            // Attempt to treat the value as a signed long instance
            try
            {
                return ConvTo< uint64_t >::From(Var< const SLongInt & >(vm, idx).value.GetNum());
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
            return ConvTo< uint64_t >::From(sq_getsize(vm, idx));
        }
        default: break;
    }
    // Default to 0
    return 0;
}

// ------------------------------------------------------------------------------------------------
const SLongInt & GetSLongInt()
{
    static SLongInt l;
    l.SetNum(0);
    return l;
}

const SLongInt & GetSLongInt(int64_t n)
{
    static SLongInt l;
    l.SetNum(n);
    return l;
}

const SLongInt & GetSLongInt(const SQChar * s)
{
    static SLongInt l;
    l = s;
    return l;
}

const ULongInt & GetULongInt()
{
    static ULongInt l;
    l.SetNum(0);
    return l;
}

const ULongInt & GetULongInt(uint64_t n)
{
    static ULongInt l;
    l.SetNum(n);
    return l;
}

const ULongInt & GetULongInt(const SQChar * s)
{
    static ULongInt l;
    l = s;
    return l;
}

// ================================================================================================
void Register_LongInt(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(TypenameS::Str,
        Class< SLongInt >(vm, TypenameS::Str)
        // Constructors
        .Ctor()
        .Ctor< SLongInt::Type >()
        .template Ctor< const char *, SQInteger >()
        // Properties
        .Prop(_SC("Str"), &SLongInt::GetCStr, &SLongInt::SetStr)
        .Prop(_SC("Num"), &SLongInt::GetSNum, &SLongInt::SetNum)
        // Core Meta-methods
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, SLongInt, ULongInt >)
        .SquirrelFunc(_SC("_typename"), &TypenameS::Fn)
        .Func(_SC("_tostring"), &SLongInt::ToString)
        // Core Functions
        .Func(_SC("tointeger"), &SLongInt::ToSqInteger)
        .Func(_SC("tofloat"), &SLongInt::ToSqFloat)
        .Func(_SC("tostring"), &SLongInt::ToSqString)
        .Func(_SC("tobool"), &SLongInt::ToSqBool)
        .Func(_SC("tochar"), &SLongInt::ToSqChar)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, SLongInt, ULongInt >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, SLongInt, ULongInt >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, SLongInt, ULongInt >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, SLongInt, ULongInt >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< SLongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, SLongInt, ULongInt >)
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
        .Ctor< const char *, SQInteger >()
        // Properties
        .Prop(_SC("Str"), &ULongInt::GetCStr, &ULongInt::SetStr)
        .Prop(_SC("Num"), &ULongInt::GetSNum, &ULongInt::SetNum)
        // Core Meta-methods
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, ULongInt, SLongInt >)
        .SquirrelFunc(_SC("_typename"), &TypenameU::Fn)
        .Func(_SC("_tostring"), &ULongInt::ToString)
        // Core Functions
        .Func(_SC("tointeger"), &ULongInt::ToSqInteger)
        .Func(_SC("tofloat"), &ULongInt::ToSqFloat)
        .Func(_SC("tostring"), &ULongInt::ToSqString)
        .Func(_SC("tobool"), &ULongInt::ToSqBool)
        .Func(_SC("tochar"), &ULongInt::ToSqChar)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, ULongInt, SLongInt >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, ULongInt, SLongInt >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, ULongInt, SLongInt >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, ULongInt, SLongInt >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< ULongInt >, SQInteger, SQFloat, bool, std::nullptr_t, const SQChar *, ULongInt, SLongInt >)
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
