#include "Library/LongInt.hpp"
//#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Register_LongInt(HSQUIRRELVM vm)
{
/*
    Sqrat::RootTable(vm).Bind(_SC("SLongInt"), Sqrat::Class<SLongInt>(vm, _SC("SLongInt"))
        .Ctor()
        .Ctor(SLongInt::Value)
        .Ctor(const SQChar *, SQInteger)

        .Prop(_SC("str"), &SLongInt::GetCStr, &SLongInt::SetStr)
        .Prop(_SC("num"), &SLongInt::GetSNum, &SLongInt::SetNum)

        .Func(_SC("_tostring"), &SLongInt::ToString)
        .Func(_SC("_cmp"), &SLongInt::Cmp)

        .Func<SLongInt (SLongInt::*)(const SLongInt &) const>(_SC("_add"), &SLongInt::operator +)
        .Func<SLongInt (SLongInt::*)(const SLongInt &) const>(_SC("_sub"), &SLongInt::operator -)
        .Func<SLongInt (SLongInt::*)(const SLongInt &) const>(_SC("_mul"), &SLongInt::operator *)
        .Func<SLongInt (SLongInt::*)(const SLongInt &) const>(_SC("_div"), &SLongInt::operator /)
        .Func<SLongInt (SLongInt::*)(const SLongInt &) const>(_SC("_modulo"), &SLongInt::operator %)

        .Func<SLongInt (SLongInt::*)(void) const>(_SC("_unm"), &SLongInt::operator -)

        .Overload<void (SLongInt::*)(void)>(_SC("random"), &SLongInt::Random)
        .Overload<void (SLongInt::*)(SLongInt::Value, SLongInt::Value)>(_SC("random"), &SLongInt::Random)
    );

    Sqrat::RootTable(vm).Bind(_SC("ULongInt"), Sqrat::Class<ULongInt>(vm, _SC("ULongInt"))
        .Ctor()
        .Ctor(ULongInt::Value)
        .Ctor(const SQChar *, SQInteger)

        .Prop(_SC("str"), &ULongInt::GetCStr, &ULongInt::SetStr)
        .Prop(_SC("num"), &ULongInt::GetSNum, &ULongInt::SetNum)

        .Func(_SC("_tostring"), &ULongInt::ToString)
        .Func(_SC("_cmp"), &ULongInt::Cmp)

        .Func<ULongInt (ULongInt::*)(const ULongInt &) const>(_SC("_add"), &ULongInt::operator +)
        .Func<ULongInt (ULongInt::*)(const ULongInt &) const>(_SC("_sub"), &ULongInt::operator -)
        .Func<ULongInt (ULongInt::*)(const ULongInt &) const>(_SC("_mul"), &ULongInt::operator *)
        .Func<ULongInt (ULongInt::*)(const ULongInt &) const>(_SC("_div"), &ULongInt::operator /)
        .Func<ULongInt (ULongInt::*)(const ULongInt &) const>(_SC("_modulo"), &ULongInt::operator %)

        .Func<ULongInt (ULongInt::*)(void) const>(_SC("_unm"), &ULongInt::operator -)

        .Overload<void (ULongInt::*)(void)>(_SC("random"), &ULongInt::Random)
        .Overload<void (ULongInt::*)(ULongInt::Value, ULongInt::Value)>(_SC("random"), &ULongInt::Random)
    );
*/
    return true;
}

} // Namespace:: SqMod
