#include "Base/AABB.hpp"
#include "Base/Vector4.hpp"
#include "Base/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const AABB AABB::NIL = AABB(0);
const AABB AABB::MIN = AABB(-1, -1, -1, 1, 1, 1);
const AABB AABB::MAX = AABB(Vector3::MIN, Vector3::MAX);

// ------------------------------------------------------------------------------------------------
SQChar AABB::Delim = ',';

// ------------------------------------------------------------------------------------------------
AABB::AABB() noexcept
    : min(-1), max(1)
{

}

AABB::AABB(Value s) noexcept
    : min(-s), max(std::fabs(s))
{

}

AABB::AABB(Value xv, Value yv, Value zv) noexcept
    : min(-xv, -yv, -zv), max(std::fabs(xv), std::fabs(yv), std::fabs(zv))
{

}

AABB::AABB(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax) noexcept
    : min(xmin, ymin, zmin), max(xmax, ymax, zmax)
{

}

// ------------------------------------------------------------------------------------------------
AABB::AABB(const Vector3 & v) noexcept
    : min(-v), max(v.Abs())
{

}

AABB::AABB(const Vector3 & vmin, const Vector3 & vmax) noexcept
    : min(vmin), max(vmax)
{

}

// ------------------------------------------------------------------------------------------------
AABB::AABB(const Vector4 & v) noexcept
    : min(-v), max(v.Abs())
{

}

AABB::AABB(const Vector4 & vmin, const Vector4 & vmax) noexcept
    : min(vmin), max(vmax)
{

}

// ------------------------------------------------------------------------------------------------
AABB::AABB(const SQChar * values, SQChar delim) noexcept
    : AABB(GetAABB(values, delim))
{

}

// ------------------------------------------------------------------------------------------------
AABB::AABB(const AABB & b) noexcept
    : min(b.min), max(b.max)
{

}

AABB::AABB(AABB && b) noexcept
    : min(b.min), max(b.max)
{

}

// ------------------------------------------------------------------------------------------------
AABB::~AABB()
{

}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator = (const AABB & b) noexcept
{
    min = b.min;
    max = b.max;
    return *this;
}

AABB & AABB::operator = (AABB && b) noexcept
{
    min = b.min;
    max = b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator = (Value s) noexcept
{
    min.Set(-s);
    max.Set(std::fabs(s));
    return *this;
}

AABB & AABB::operator = (const Vector3 & v) noexcept
{
    min.Set(-v);
    max.Set(v.Abs());
    return *this;
}

AABB & AABB::operator = (const Vector4 & v) noexcept
{
    min.Set(-v);
    max.Set(v.Abs());
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator += (const AABB & b) noexcept
{
    min += b.min;
    max += b.max;
    return *this;
}

AABB & AABB::operator -= (const AABB & b) noexcept
{
    min -= b.min;
    max -= b.max;
    return *this;
}

AABB & AABB::operator *= (const AABB & b) noexcept
{
    min *= b.min;
    max *= b.max;
    return *this;
}

AABB & AABB::operator /= (const AABB & b) noexcept
{
    min /= b.min;
    max /= b.max;
    return *this;
}

AABB & AABB::operator %= (const AABB & b) noexcept
{
    min %= b.min;
    max %= b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator += (Value s) noexcept
{
    min += s;
    max += s;
    return *this;
}

AABB & AABB::operator -= (Value s) noexcept
{
    min -= s;
    max -= s;
    return *this;
}

AABB & AABB::operator *= (Value s) noexcept
{
    min *= s;
    max *= s;
    return *this;
}

AABB & AABB::operator /= (Value s) noexcept
{
    min /= s;
    max /= s;
    return *this;
}

AABB & AABB::operator %= (Value s) noexcept
{
    min %= s;
    max %= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator ++ () noexcept
{
    ++min;
    ++max;
    return *this;
}

AABB & AABB::operator -- () noexcept
{
    --min;
    --max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator ++ (int) noexcept
{
    AABB state(*this);
    ++min;
    ++max;
    return state;
}

AABB AABB::operator -- (int) noexcept
{
    AABB state(*this);
    --min;
    --max;
    return state;
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator + (const AABB & b) const noexcept
{
    return AABB(min + b.min, max + b.max);
}

AABB AABB::operator - (const AABB & b) const noexcept
{
    return AABB(min - b.min, max - b.max);
}

AABB AABB::operator * (const AABB & b) const noexcept
{
    return AABB(min * b.min, max * b.max);
}

AABB AABB::operator / (const AABB & b) const noexcept
{
    return AABB(min / b.min, max / b.max);
}

AABB AABB::operator % (const AABB & b) const noexcept
{
    return AABB(min % b.min, max % b.max);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator + (Value s) const noexcept
{
    return AABB(min + s, max + s);
}

AABB AABB::operator - (Value s) const noexcept
{
    return AABB(min - s, max - s);
}

AABB AABB::operator * (Value s) const noexcept
{
    return AABB(min * s, max * s);
}

AABB AABB::operator / (Value s) const noexcept
{
    return AABB(min / s, max / s);
}

AABB AABB::operator % (Value s) const noexcept
{
    return AABB(min % s, max % s);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator + () const noexcept
{
    return AABB(min.Abs(), max.Abs());
}

AABB AABB::operator - () const noexcept
{
    return AABB(-min, -max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator == (const AABB & b) const noexcept
{
    return (min == b.min) && (max == b.max);
}

bool AABB::operator != (const AABB & b) const noexcept
{
    return (min != b.min) && (max != b.max);
}

bool AABB::operator < (const AABB & b) const noexcept
{
    return (min < b.min) && (max < b.max);
}

bool AABB::operator > (const AABB & b) const noexcept
{
    return (min > b.min) && (max > b.max);
}

bool AABB::operator <= (const AABB & b) const noexcept
{
    return (min <= b.min) && (max <= b.max);
}

bool AABB::operator >= (const AABB & b) const noexcept
{
    return (min >= b.min) && (max >= b.max);
}

// ------------------------------------------------------------------------------------------------
SQInteger AABB::Cmp(const AABB & b) const noexcept
{
    return *this == b ? 0 : (*this > b ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * AABB::ToString() const noexcept
{
    return ToStringF("%f,%f,%f,%f,%f,%f", min.x, min.y, min.z, max.x, max.y, max.z);
}

// ------------------------------------------------------------------------------------------------
void AABB::Set(Value ns) noexcept
{
    min = -ns;
    max = std::fabs(ns);
}

void AABB::Set(Value nx, Value ny, Value nz) noexcept
{
    min.Set(-nx, -ny, -nz);
    max.Set(std::fabs(nx), std::fabs(ny), std::fabs(nz));
}

void AABB::Set(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax) noexcept
{
    min.Set(xmin, ymin, zmin);
    max.Set(xmax, ymax, zmax);
}

// ------------------------------------------------------------------------------------------------
void AABB::Set(const AABB & b) noexcept
{
    min = b.min;
    max = b.max;
}

// ------------------------------------------------------------------------------------------------
void AABB::Set(const Vector3 & v) noexcept
{
    min = -v;
    max = v.Abs();
}

void AABB::Set(const Vector3 & nmin, const Vector3 & nmax) noexcept
{
    min = nmin;
    max = nmax;
}

// ------------------------------------------------------------------------------------------------
void AABB::Set(const Vector4 & v) noexcept
{
    min = -v;
    max = v.Abs();
}

void AABB::Set(const Vector4 & nmin, const Vector4 & nmax) noexcept
{
    min = nmin;
    max = nmax;
}

// ------------------------------------------------------------------------------------------------
void AABB::Set(const SQChar * values, SQChar delim) noexcept
{
    Set(GetAABB(values, delim));
}

// ------------------------------------------------------------------------------------------------
AABB AABB::Abs() const noexcept
{
    return AABB(min.Abs(), max.Abs());
}

// ================================================================================================
bool Register_AABB(HSQUIRRELVM vm)
{
    LogDbg("Beginning registration of <AABB> type");

    typedef AABB::Value Val;

    Sqrat::RootTable(vm).Bind(_SC("AABB"), Sqrat::Class<AABB>(vm, _SC("AABB"))
        .Ctor()
        .Ctor<Val>()
        .Ctor<Val, Val, Val>()
        .Ctor<Val, Val, Val, Val, Val, Val>()
        .Ctor<const Vector3 &, const Vector3 &>()

        .SetStaticValue(_SC("delim"), &AABB::Delim)

        .Var(_SC("min"), &AABB::min)
        .Var(_SC("max"), &AABB::max)

        .Prop(_SC("abs"), &AABB::Abs)

        .Func(_SC("_tostring"), &AABB::ToString)
        .Func(_SC("_cmp"), &AABB::Cmp)

        .Func<AABB (AABB::*)(const AABB &) const>(_SC("_add"), &AABB::operator +)
        .Func<AABB (AABB::*)(const AABB &) const>(_SC("_sub"), &AABB::operator -)
        .Func<AABB (AABB::*)(const AABB &) const>(_SC("_mul"), &AABB::operator *)
        .Func<AABB (AABB::*)(const AABB &) const>(_SC("_div"), &AABB::operator /)
        .Func<AABB (AABB::*)(const AABB &) const>(_SC("_modulo"), &AABB::operator %)
        .Func<AABB (AABB::*)(void) const>(_SC("_unm"), &AABB::operator -)

        .Overload<void (AABB::*)(Val)>(_SC("set"), &AABB::Set)
        .Overload<void (AABB::*)(Val, Val, Val)>(_SC("set"), &AABB::Set)
        .Overload<void (AABB::*)(Val, Val, Val, Val, Val, Val)>(_SC("set"), &AABB::Set)
        .Overload<void (AABB::*)(const AABB &)>(_SC("set_box"), &AABB::Set)
        .Overload<void (AABB::*)(const Vector3 &)>(_SC("set_vec3"), &AABB::Set)
        .Overload<void (AABB::*)(const Vector3 &, const Vector3 &)>(_SC("set_vec3"), &AABB::Set)
        .Overload<void (AABB::*)(const Vector4 &)>(_SC("set_vec4"), &AABB::Set)
        .Overload<void (AABB::*)(const Vector4 &, const Vector4 &)>(_SC("set_vec4"), &AABB::Set)
        .Overload<void (AABB::*)(const SQChar *, SQChar)>(_SC("set_str"), &AABB::Set)

        .Func(_SC("clear"), &AABB::Clear)

        .Func<AABB & (AABB::*)(const AABB &)>(_SC("opAddAssign"), &AABB::operator +=)
        .Func<AABB & (AABB::*)(const AABB &)>(_SC("opSubAssign"), &AABB::operator -=)
        .Func<AABB & (AABB::*)(const AABB &)>(_SC("opMulAssign"), &AABB::operator *=)
        .Func<AABB & (AABB::*)(const AABB &)>(_SC("opDivAssign"), &AABB::operator /=)
        .Func<AABB & (AABB::*)(const AABB &)>(_SC("opModAssign"), &AABB::operator %=)

        .Func<AABB & (AABB::*)(AABB::Value)>(_SC("opAddAssignS"), &AABB::operator +=)
        .Func<AABB & (AABB::*)(AABB::Value)>(_SC("opSubAssignS"), &AABB::operator -=)
        .Func<AABB & (AABB::*)(AABB::Value)>(_SC("opMulAssignS"), &AABB::operator *=)
        .Func<AABB & (AABB::*)(AABB::Value)>(_SC("opDivAssignS"), &AABB::operator /=)
        .Func<AABB & (AABB::*)(AABB::Value)>(_SC("opModAssignS"), &AABB::operator %=)

        .Func<AABB & (AABB::*)(void)>(_SC("opPreInc"), &AABB::operator ++)
        .Func<AABB & (AABB::*)(void)>(_SC("opPreDec"), &AABB::operator --)
        .Func<AABB (AABB::*)(int)>(_SC("opPostInc"), &AABB::operator ++)
        .Func<AABB (AABB::*)(int)>(_SC("opPostDec"), &AABB::operator --)

        .Func<AABB (AABB::*)(const AABB &) const>(_SC("opAdd"), &AABB::operator +)
        .Func<AABB (AABB::*)(AABB::Value) const>(_SC("opAddS"), &AABB::operator +)
        .Func<AABB (AABB::*)(const AABB &) const>(_SC("opSub"), &AABB::operator -)
        .Func<AABB (AABB::*)(AABB::Value) const>(_SC("opSubS"), &AABB::operator -)
        .Func<AABB (AABB::*)(const AABB &) const>(_SC("opMul"), &AABB::operator *)
        .Func<AABB (AABB::*)(AABB::Value) const>(_SC("opMulS"), &AABB::operator *)
        .Func<AABB (AABB::*)(const AABB &) const>(_SC("opDiv"), &AABB::operator /)
        .Func<AABB (AABB::*)(AABB::Value) const>(_SC("opDivS"), &AABB::operator /)
        .Func<AABB (AABB::*)(const AABB &) const>(_SC("opMod"), &AABB::operator %)
        .Func<AABB (AABB::*)(AABB::Value) const>(_SC("opModS"), &AABB::operator %)

        .Func<AABB (AABB::*)(void) const>(_SC("opUnPlus"), &AABB::operator +)
        .Func<AABB (AABB::*)(void) const>(_SC("opUnMinus"), &AABB::operator -)

        .Func<bool (AABB::*)(const AABB &) const>(_SC("opEqual"), &AABB::operator ==)
        .Func<bool (AABB::*)(const AABB &) const>(_SC("opNotEqual"), &AABB::operator !=)
        .Func<bool (AABB::*)(const AABB &) const>(_SC("opLessThan"), &AABB::operator <)
        .Func<bool (AABB::*)(const AABB &) const>(_SC("opGreaterThan"), &AABB::operator >)
        .Func<bool (AABB::*)(const AABB &) const>(_SC("opLessEqual"), &AABB::operator <=)
        .Func<bool (AABB::*)(const AABB &) const>(_SC("opGreaterEqual"), &AABB::operator >=)
    );

    LogDbg("Registration of <AABB> type was successful");

    return true;
}

} // Namespace:: SqMod
