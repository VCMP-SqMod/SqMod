#include "Library/LongInt.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_LongInt(HSQUIRRELVM vm)
{
    // Output debugging information
    LogDbg("Beginning registration of <Long integer> type");
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("SLongInt"), Sqrat::Class< SLongInt >(vm, _SC("SLongInt"))
        /* Constructors */
        .Ctor()
        .Ctor< SLongInt::Type >()
        .template Ctor< const SQChar *, SQInteger >()
        /* Properties */
        .Prop(_SC("str"), &SLongInt::GetCStr, &SLongInt::SetStr)
        .Prop(_SC("num"), &SLongInt::GetSNum, &SLongInt::SetNum)
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
        .Overload< void (SLongInt::*)(void) >(_SC("random"), &SLongInt::Random)
        .Overload< void (SLongInt::*)(SLongInt::Type, SLongInt::Type) >(_SC("random"), &SLongInt::Random)
    );
    // Attempt to register the specified type
    Sqrat::RootTable(vm).Bind(_SC("ULongInt"), Sqrat::Class< ULongInt >(vm, _SC("ULongInt"))
        /* Constructors */
        .Ctor()
        .Ctor< ULongInt::Type >()
        .Ctor< const SQChar *, SQInteger >()
        /* Properties */
        .Prop(_SC("str"), &ULongInt::GetCStr, &ULongInt::SetStr)
        .Prop(_SC("num"), &ULongInt::GetSNum, &ULongInt::SetNum)
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
        .Overload< void (ULongInt::*)(void) >(_SC("random"), &ULongInt::Random)
        .Overload< void (ULongInt::*)(ULongInt::Type, ULongInt::Type) >(_SC("random"), &ULongInt::Random)
    );
    // Output debugging information
    LogDbg("Registration of <IRCSession> type was successful");
    // Registration succeeded
    return true;
}

} // Namespace:: SqMod
