// ------------------------------------------------------------------------------------------------
#include "Library/Numeric.hpp"
#include "Library/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
LongInt< Int64 >::LongInt(CSStr text) : m_Data(0), m_Text()
{ m_Data = strtoll(text, NULL, 10); }
LongInt< Int64 >::LongInt(CSStr text, Object & /* null */) : m_Data(0), m_Text()
{ m_Data = strtoll(text, NULL, 10); }

// ------------------------------------------------------------------------------------------------
LongInt< Int64 > & LongInt< Int64 >::operator = (CSStr text)
{
    m_Data = strtoll(text, NULL, 10);
    return *this;
}

// ------------------------------------------------------------------------------------------------
CSStr LongInt< Int64 >::ToString()
{
    if (snprintf(m_Text, sizeof(m_Text), "%llu", m_Data) < 0)
    {
        m_Text[0] = 0;
    }

    return m_Text;
}

// ------------------------------------------------------------------------------------------------
void LongInt< Int64 >::Random() { m_Data = GetRandomInt64(); }
void LongInt< Int64 >::Random(Type n) { m_Data = GetRandomInt64(n); }
void LongInt< Int64 >::Random(Type m, Type n) { m_Data = GetRandomInt64(m, n); }

// ------------------------------------------------------------------------------------------------
LongInt< Uint64 >::LongInt(CSStr text) : m_Data(0), m_Text()
{ m_Data = strtoll(text, NULL, 10); }
LongInt< Uint64 >::LongInt(CSStr text, Object & /* null */) : m_Data(0), m_Text()
{ m_Data = strtoll(text, NULL, 10); }

// ------------------------------------------------------------------------------------------------
LongInt< Uint64 > & LongInt< Uint64 >::operator = (CSStr text)
{
    m_Data = strtoll(text, NULL, 10);
    return *this;
}

// ------------------------------------------------------------------------------------------------
CSStr LongInt< Uint64 >::ToString()
{
    if (snprintf(m_Text, sizeof(m_Text), "%llu", m_Data) < 0)
    {
        m_Text[0] = 0;
    }

    return m_Text;
}

// ------------------------------------------------------------------------------------------------
void LongInt< Uint64 >::Random() { m_Data = GetRandomUint64(); }
void LongInt< Uint64 >::Random(Type n) { m_Data = GetRandomUint64(n); }
void LongInt< Uint64 >::Random(Type m, Type n) { m_Data = GetRandomUint64(m, n); }

// ================================================================================================
void Register_Numeric(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SLongInt"), Class< SLongInt >(vm, _SC("SLongInt"))
        /* Constructors */
        .Ctor()
        .Ctor< SLongInt::Type >()
        .template Ctor< CCStr, Object & >()
        /* Properties */
        .Prop(_SC("Str"), &SLongInt::GetCStr, &SLongInt::SetStr)
        .Prop(_SC("Num"), &SLongInt::GetSNum, &SLongInt::SetNum)
        /* Metamethods */
        .Func(_SC("_tostring"), &SLongInt::ToString)
        .Func(_SC("_cmp"), &SLongInt::Cmp)
        .Func< SLongInt (SLongInt::*)(const SLongInt &) const >(_SC("_add"), &SLongInt::operator +)
        .Func< SLongInt (SLongInt::*)(const SLongInt &) const >(_SC("_sub"), &SLongInt::operator -)
        .Func< SLongInt (SLongInt::*)(const SLongInt &) const >(_SC("_mul"), &SLongInt::operator *)
        .Func< SLongInt (SLongInt::*)(const SLongInt &) const >(_SC("_div"), &SLongInt::operator /)
        .Func< SLongInt (SLongInt::*)(const SLongInt &) const >(_SC("_modulo"), &SLongInt::operator %)
        .Func< SLongInt (SLongInt::*)(void) const >(_SC("_unm"), &SLongInt::operator -)
        /* Overloads */
        .Overload< void (SLongInt::*)(void) >(_SC("Random"), &SLongInt::Random)
        .Overload< void (SLongInt::*)(SLongInt::Type) >(_SC("Random"), &SLongInt::Random)
        .Overload< void (SLongInt::*)(SLongInt::Type, SLongInt::Type) >(_SC("Random"), &SLongInt::Random)
    );

    RootTable(vm).Bind(_SC("ULongInt"), Class< ULongInt >(vm, _SC("ULongInt"))
        /* Constructors */
        .Ctor()
        .Ctor< ULongInt::Type >()
        .Ctor< CCStr, Object & >()
        /* Properties */
        .Prop(_SC("Str"), &ULongInt::GetCStr, &ULongInt::SetStr)
        .Prop(_SC("Num"), &ULongInt::GetSNum, &ULongInt::SetNum)
        /* Metamethods */
        .Func(_SC("_tostring"), &ULongInt::ToString)
        .Func(_SC("_cmp"), &ULongInt::Cmp)
        .Func< ULongInt (ULongInt::*)(const ULongInt &) const >(_SC("_add"), &ULongInt::operator +)
        .Func< ULongInt (ULongInt::*)(const ULongInt &) const >(_SC("_sub"), &ULongInt::operator -)
        .Func< ULongInt (ULongInt::*)(const ULongInt &) const >(_SC("_mul"), &ULongInt::operator *)
        .Func< ULongInt (ULongInt::*)(const ULongInt &) const >(_SC("_div"), &ULongInt::operator /)
        .Func< ULongInt (ULongInt::*)(const ULongInt &) const >(_SC("_modulo"), &ULongInt::operator %)
        .Func< ULongInt (ULongInt::*)(void) const >(_SC("_unm"), &ULongInt::operator -)
        /* Overloads */
        .Overload< void (ULongInt::*)(void) >(_SC("Random"), &ULongInt::Random)
        .Overload< void (ULongInt::*)(ULongInt::Type) >(_SC("Random"), &ULongInt::Random)
        .Overload< void (ULongInt::*)(ULongInt::Type, ULongInt::Type) >(_SC("Random"), &ULongInt::Random)
    );
}

} // Namespace:: SqMod
