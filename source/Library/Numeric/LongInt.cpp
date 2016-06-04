// ------------------------------------------------------------------------------------------------
#include "Library/Numeric/LongInt.hpp"
#include "Library/Numeric/Random.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cerrno>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
LongInt< Int64 >::LongInt(CSStr text) : m_Data(0), m_Text()
{
    m_Data = std::strtoll(text, nullptr, 10);
}

// ------------------------------------------------------------------------------------------------
LongInt< Int64 >::LongInt(CSStr text, SQInteger fall) : m_Data(0), m_Text()
{
    m_Data = std::strtoll(text, nullptr, 10);
    // Simple, check for conversion errors
    if (errno == ERANGE)
    {
        m_Data = ConvTo< Type >::From(fall);
    }
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
LongInt< Uint64 >::LongInt(CSStr text) : m_Data(0), m_Text()
{
    m_Data = std::strtoll(text, nullptr, 10);
}

// ------------------------------------------------------------------------------------------------
LongInt< Uint64 >::LongInt(CSStr text, SQInteger fall) : m_Data(0), m_Text()
{
    m_Data = std::strtoull(text, nullptr, 10);
    // Simple, check for conversion errors
    if (errno == ERANGE)
    {
        m_Data = ConvTo< Type >::From(fall);
    }
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

// ================================================================================================
void Register_LongInt(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SLongInt"), Class< SLongInt >(vm, _SC("SLongInt"))
        /* Constructors */
        .Ctor()
        .Ctor< SLongInt::Type >()
        .template Ctor< CCStr, SQInteger >()
        /* Properties */
        .Prop(_SC("Str"), &SLongInt::GetCStr, &SLongInt::SetStr)
        .Prop(_SC("Num"), &SLongInt::GetSNum, &SLongInt::SetNum)
        /* Core Meta-methods */
        .Func(_SC("_tostring"), &SLongInt::ToString)
        .Func(_SC("_typename"), &SLongInt::Typename)
        .Func(_SC("_cmp"), &SLongInt::Cmp)
        /* Core Functions */
        .Func(_SC("tointeger"), &SLongInt::ToSqInteger)
        .Func(_SC("tofloat"), &SLongInt::ToSqFloat)
        .Func(_SC("tostring"), &SLongInt::ToSqString)
        .Func(_SC("tobool"), &SLongInt::ToSqBool)
        .Func(_SC("tochar"), &SLongInt::ToSqChar)
        /* Meta-methods */
        .Func< SLongInt (SLongInt::*)(const SLongInt &) const >(_SC("_add"), &SLongInt::operator +)
        .Func< SLongInt (SLongInt::*)(const SLongInt &) const >(_SC("_sub"), &SLongInt::operator -)
        .Func< SLongInt (SLongInt::*)(const SLongInt &) const >(_SC("_mul"), &SLongInt::operator *)
        .Func< SLongInt (SLongInt::*)(const SLongInt &) const >(_SC("_div"), &SLongInt::operator /)
        .Func< SLongInt (SLongInt::*)(const SLongInt &) const >(_SC("_modulo"), &SLongInt::operator %)
        .Func< SLongInt (SLongInt::*)(void) const >(_SC("_unm"), &SLongInt::operator -)
        /* Functions */
        .Func(_SC("GetStr"), &SLongInt::GetCStr)
        .Func(_SC("SetStr"), &SLongInt::SetStr)
        .Func(_SC("GetNum"), &SLongInt::GetSNum)
        .Func(_SC("SetNum"), &SLongInt::SetNum)
        /* Overloads */
        .Overload< void (SLongInt::*)(void) >(_SC("Random"), &SLongInt::Random)
        .Overload< void (SLongInt::*)(SLongInt::Type) >(_SC("Random"), &SLongInt::Random)
        .Overload< void (SLongInt::*)(SLongInt::Type, SLongInt::Type) >(_SC("Random"), &SLongInt::Random)
    );

    RootTable(vm).Bind(_SC("ULongInt"), Class< ULongInt >(vm, _SC("ULongInt"))
        /* Constructors */
        .Ctor()
        .Ctor< ULongInt::Type >()
        .Ctor< CCStr, SQInteger >()
        /* Properties */
        .Prop(_SC("Str"), &ULongInt::GetCStr, &ULongInt::SetStr)
        .Prop(_SC("Num"), &ULongInt::GetSNum, &ULongInt::SetNum)
        /* Core Meta-methods */
        .Func(_SC("_tostring"), &ULongInt::ToString)
        .Func(_SC("_typename"), &ULongInt::Typename)
        .Func(_SC("_cmp"), &ULongInt::Cmp)
        /* Core Functions */
        .Func(_SC("tointeger"), &ULongInt::ToSqInteger)
        .Func(_SC("tofloat"), &ULongInt::ToSqFloat)
        .Func(_SC("tostring"), &ULongInt::ToSqString)
        .Func(_SC("tobool"), &ULongInt::ToSqBool)
        .Func(_SC("tochar"), &ULongInt::ToSqChar)
        /* Meta-methods */
        .Func< ULongInt (ULongInt::*)(const ULongInt &) const >(_SC("_add"), &ULongInt::operator +)
        .Func< ULongInt (ULongInt::*)(const ULongInt &) const >(_SC("_sub"), &ULongInt::operator -)
        .Func< ULongInt (ULongInt::*)(const ULongInt &) const >(_SC("_mul"), &ULongInt::operator *)
        .Func< ULongInt (ULongInt::*)(const ULongInt &) const >(_SC("_div"), &ULongInt::operator /)
        .Func< ULongInt (ULongInt::*)(const ULongInt &) const >(_SC("_modulo"), &ULongInt::operator %)
        .Func< ULongInt (ULongInt::*)(void) const >(_SC("_unm"), &ULongInt::operator -)
        /* Functions */
        .Func(_SC("GetStr"), &ULongInt::GetCStr)
        .Func(_SC("SetStr"), &ULongInt::SetStr)
        .Func(_SC("GetNum"), &ULongInt::GetSNum)
        .Func(_SC("SetNum"), &ULongInt::SetNum)
        /* Overloads */
        .Overload< void (ULongInt::*)(void) >(_SC("Random"), &ULongInt::Random)
        .Overload< void (ULongInt::*)(ULongInt::Type) >(_SC("Random"), &ULongInt::Random)
        .Overload< void (ULongInt::*)(ULongInt::Type, ULongInt::Type) >(_SC("Random"), &ULongInt::Random)
    );
}

} // Namespace:: SqMod
