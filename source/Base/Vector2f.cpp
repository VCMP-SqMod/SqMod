#include "Base/Vector2f.hpp"
#include "Base/Vector2i.hpp"
#include "Base/Vector2u.hpp"
#include "Base/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Vector2f Vector2f::NIL = Vector2f(0);
const Vector2f Vector2f::MIN = Vector2f(std::numeric_limits<Vector2f::Value>::min());
const Vector2f Vector2f::MAX = Vector2f(std::numeric_limits<Vector2f::Value>::max());

// ------------------------------------------------------------------------------------------------
SQChar Vector2f::Delim = ',';

// ------------------------------------------------------------------------------------------------
Vector2f::Vector2f() noexcept
    : x(0.0), y(0.0)
{

}

Vector2f::Vector2f(Value s) noexcept
    : x(s), y(s)
{

}

Vector2f::Vector2f(Value xv, Value yv) noexcept
    : x(xv), y(yv)
{

}

// ------------------------------------------------------------------------------------------------
Vector2f::Vector2f(const Vector2i & v) noexcept
    : x(static_cast<Value>(v.x)), y(static_cast<Value>(v.y))
{

}

Vector2f::Vector2f(const Vector2u & v) noexcept
    : x(static_cast<Value>(v.x)), y(static_cast<Value>(v.y))
{

}

// ------------------------------------------------------------------------------------------------
Vector2f::Vector2f(const SQChar * values, SQChar delim) noexcept
    : Vector2f(GetVector2f(values, delim))
{

}

// ------------------------------------------------------------------------------------------------
Vector2f::Vector2f(const Vector2f & v) noexcept
    : x(v.x), y(v.y)
{

}

Vector2f::Vector2f(Vector2f && v) noexcept
    : x(v.x), y(v.y)
{

}

// ------------------------------------------------------------------------------------------------
Vector2f::~Vector2f()
{

}

// ------------------------------------------------------------------------------------------------
Vector2f & Vector2f::operator = (const Vector2f & v) noexcept
{
    x = v.x;
    y = v.y;
    return *this;
}

Vector2f & Vector2f::operator = (Vector2f && v) noexcept
{
    x = v.x;
    y = v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2f & Vector2f::operator = (Value s) noexcept
{
    x = s;
    y = s;
    return *this;
}

Vector2f & Vector2f::operator = (const SQChar * values) noexcept
{
    Set(GetVector2f(values, Delim));
    return *this;
}

Vector2f & Vector2f::operator = (const Vector2i & v) noexcept
{
    x = static_cast<Value>(v.x);
    y = static_cast<Value>(v.y);
    return *this;
}

Vector2f & Vector2f::operator = (const Vector2u & v) noexcept
{
    x = static_cast<Value>(v.x);
    y = static_cast<Value>(v.y);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2f & Vector2f::operator += (const Vector2f & v) noexcept
{
    x += v.x;
    y += v.y;
    return *this;
}

Vector2f & Vector2f::operator -= (const Vector2f & v) noexcept
{
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2f & Vector2f::operator *= (const Vector2f & v) noexcept
{
    x *= v.x;
    y *= v.y;
    return *this;
}

Vector2f & Vector2f::operator /= (const Vector2f & v) noexcept
{
    x /= v.x;
    y /= v.y;
    return *this;
}

Vector2f & Vector2f::operator %= (const Vector2f & v) noexcept
{
    x = std::fmod(x, v.x);
    y = std::fmod(y, v.y);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2f & Vector2f::operator += (Value s) noexcept
{
    x += s;
    y += s;
    return *this;
}

Vector2f & Vector2f::operator -= (Value s) noexcept
{
    x -= s;
    y -= s;
    return *this;
}

Vector2f & Vector2f::operator *= (Value s) noexcept
{
    x *= s;
    y *= s;
    return *this;
}

Vector2f & Vector2f::operator /= (Value s) noexcept
{
    x /= s;
    y /= s;
    return *this;
}

Vector2f & Vector2f::operator %= (Value s) noexcept
{
    x = std::fmod(x, s);
    y = std::fmod(y, s);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2f & Vector2f::operator ++ () noexcept
{
    ++x;
    ++y;
    return *this;
}

Vector2f & Vector2f::operator -- () noexcept
{
    --x;
    --y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2f Vector2f::operator ++ (int) noexcept
{
    Vector2f state(*this);
    ++x;
    ++y;
    return state;
}

Vector2f Vector2f::operator -- (int) noexcept
{
    Vector2f state(*this);
    --x;
    --y;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector2f Vector2f::operator + (const Vector2f & v) const noexcept
{
    return Vector2f(x + v.x, y + v.y);
}

Vector2f Vector2f::operator - (const Vector2f & v) const noexcept
{
    return Vector2f(x - v.x, y - v.y);
}

Vector2f Vector2f::operator * (const Vector2f & v) const noexcept
{
    return Vector2f(x * v.x, y * v.y);
}

Vector2f Vector2f::operator / (const Vector2f & v) const noexcept
{
    return Vector2f(x / v.x, y / v.y);
}

Vector2f Vector2f::operator % (const Vector2f & v) const noexcept
{
    return Vector2f(std::fmod(x, v.x), std::fmod(y, v.y));
}

// ------------------------------------------------------------------------------------------------
Vector2f Vector2f::operator + (Value s) const noexcept
{
    return Vector2f(x + s, y + s);
}

Vector2f Vector2f::operator - (Value s) const noexcept
{
    return Vector2f(x - s, y - s);
}

Vector2f Vector2f::operator * (Value s) const noexcept
{
    return Vector2f(x * s, y * s);
}

Vector2f Vector2f::operator / (Value s) const noexcept
{
    return Vector2f(x / s, y / s);
}

Vector2f Vector2f::operator % (Value s) const noexcept
{
    return Vector2f(std::fmod(x, s), std::fmod(y, s));
}

// ------------------------------------------------------------------------------------------------
Vector2f Vector2f::operator + () const noexcept
{
    return Vector2f(std::fabs(x), std::fabs(y));
}

Vector2f Vector2f::operator - () const noexcept
{
    return Vector2f(-x, -y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2f::operator == (const Vector2f & v) const noexcept
{
    return EpsEq(x, v.x) && EpsEq(y, v.y);
}

bool Vector2f::operator != (const Vector2f & v) const noexcept
{
    return !EpsEq(x, v.x) && !EpsEq(y, v.y);
}

bool Vector2f::operator < (const Vector2f & v) const noexcept
{
    return std::isless(x, v.x) && std::isless(y, v.y);
}

bool Vector2f::operator > (const Vector2f & v) const noexcept
{
    return std::isgreater(x, v.x) && std::isgreater(y, v.y);
}

bool Vector2f::operator <= (const Vector2f & v) const noexcept
{
    return std::islessequal(x, v.x) && std::islessequal(y, v.y);
}

bool Vector2f::operator >= (const Vector2f & v) const noexcept
{
    return std::isgreaterequal(x, v.x) && std::isgreaterequal(y, v.y);
}

// ------------------------------------------------------------------------------------------------
SQInteger Vector2f::Cmp(const Vector2f & v) const noexcept
{
    return *this == v ? 0 : (*this > v ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Vector2f::ToString() const noexcept
{
    return ToStringF("%f,%f", x, y);
}

// ------------------------------------------------------------------------------------------------
void Vector2f::Set(Value ns) noexcept
{
    x = ns;
    y = ns;
}

void Vector2f::Set(Value nx, Value ny) noexcept
{
    x = nx;
    y = ny;
}

// ------------------------------------------------------------------------------------------------
void Vector2f::Set(const Vector2f & v) noexcept
{
    x = v.x;
    y = v.y;
}

void Vector2f::Set(const Vector2i & v) noexcept
{
    x = static_cast<Value>(v.x);
    y = static_cast<Value>(v.y);
}

void Vector2f::Set(const Vector2u & v) noexcept
{
    x = static_cast<Value>(v.x);
    y = static_cast<Value>(v.y);
}

// ------------------------------------------------------------------------------------------------
void Vector2f::Set(const SQChar * values, SQChar delim) noexcept
{
    Set(GetVector2f(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Vector2f::Generate() noexcept
{
    x = RandomVal<Value>::Get();
    y = RandomVal<Value>::Get();
}

void Vector2f::Generate(Value min, Value max) noexcept
{
    if (max < min)
    {
        LogErr("max value is lower than min value");
    }
    else
    {
        x = RandomVal<Value>::Get(min, max);
        y = RandomVal<Value>::Get(min, max);
    }
}

void Vector2f::Generate(Value xmin, Value xmax, Value ymin, Value ymax) noexcept
{
    if (xmax < xmin || ymax < ymin)
    {
        LogErr("max value is lower than min value");
    }
    else
    {
        x = RandomVal<Value>::Get(ymin, ymax);
        y = RandomVal<Value>::Get(xmin, xmax);
    }
}

// ------------------------------------------------------------------------------------------------
Vector2f Vector2f::Abs() const noexcept
{
    return Vector2f(std::fabs(x), std::fabs(y));
}

// ================================================================================================
bool Register_Vector2f(HSQUIRRELVM vm)
{
    LogDbg("Beginning registration of <Vector2f> type");

    typedef Vector2f::Value Val;

    Sqrat::RootTable(vm).Bind(_SC("Vector2f"), Sqrat::Class<Vector2f>(vm, _SC("Vector2f"))
        .Ctor()
        .Ctor<Val>()
        .Ctor<Val, Val>()

        .SetStaticValue(_SC("delim"), &Vector2f::Delim)

        .Var(_SC("x"), &Vector2f::x)
        .Var(_SC("y"), &Vector2f::y)

        .Prop(_SC("abs"), &Vector2f::Abs)

        .Func(_SC("_tostring"), &Vector2f::ToString)
        .Func(_SC("_cmp"), &Vector2f::Cmp)

        .Func<Vector2f (Vector2f::*)(const Vector2f &) const>(_SC("_add"), &Vector2f::operator +)
        .Func<Vector2f (Vector2f::*)(const Vector2f &) const>(_SC("_sub"), &Vector2f::operator -)
        .Func<Vector2f (Vector2f::*)(const Vector2f &) const>(_SC("_mul"), &Vector2f::operator *)
        .Func<Vector2f (Vector2f::*)(const Vector2f &) const>(_SC("_div"), &Vector2f::operator /)
        .Func<Vector2f (Vector2f::*)(const Vector2f &) const>(_SC("_modulo"), &Vector2f::operator %)
        .Func<Vector2f (Vector2f::*)(void) const>(_SC("_unm"), &Vector2f::operator -)

        .Overload<void (Vector2f::*)(Val)>(_SC("set"), &Vector2f::Set)
        .Overload<void (Vector2f::*)(Val, Val)>(_SC("set"), &Vector2f::Set)
        .Overload<void (Vector2f::*)(const Vector2f &)>(_SC("set_vec2f"), &Vector2f::Set)
        .Overload<void (Vector2f::*)(const Vector2i &)>(_SC("set_vec2i"), &Vector2f::Set)
        .Overload<void (Vector2f::*)(const Vector2u &)>(_SC("set_vec2u"), &Vector2f::Set)
        .Overload<void (Vector2f::*)(const SQChar *, SQChar)>(_SC("set_str"), &Vector2f::Set)

        .Overload<void (Vector2f::*)(void)>(_SC("generate"), &Vector2f::Generate)
        .Overload<void (Vector2f::*)(Val, Val)>(_SC("generate"), &Vector2f::Generate)
        .Overload<void (Vector2f::*)(Val, Val, Val, Val)>(_SC("generate"), &Vector2f::Generate)

        .Func(_SC("clear"), &Vector2f::Clear)

        .Func<Vector2f & (Vector2f::*)(const Vector2f &)>(_SC("opAddAssign"), &Vector2f::operator +=)
        .Func<Vector2f & (Vector2f::*)(const Vector2f &)>(_SC("opSubAssign"), &Vector2f::operator -=)
        .Func<Vector2f & (Vector2f::*)(const Vector2f &)>(_SC("opMulAssign"), &Vector2f::operator *=)
        .Func<Vector2f & (Vector2f::*)(const Vector2f &)>(_SC("opDivAssign"), &Vector2f::operator /=)
        .Func<Vector2f & (Vector2f::*)(const Vector2f &)>(_SC("opModAssign"), &Vector2f::operator %=)

        .Func<Vector2f & (Vector2f::*)(Vector2f::Value)>(_SC("opAddAssignS"), &Vector2f::operator +=)
        .Func<Vector2f & (Vector2f::*)(Vector2f::Value)>(_SC("opSubAssignS"), &Vector2f::operator -=)
        .Func<Vector2f & (Vector2f::*)(Vector2f::Value)>(_SC("opMulAssignS"), &Vector2f::operator *=)
        .Func<Vector2f & (Vector2f::*)(Vector2f::Value)>(_SC("opDivAssignS"), &Vector2f::operator /=)
        .Func<Vector2f & (Vector2f::*)(Vector2f::Value)>(_SC("opModAssignS"), &Vector2f::operator %=)

        .Func<Vector2f & (Vector2f::*)(void)>(_SC("opPreInc"), &Vector2f::operator ++)
        .Func<Vector2f & (Vector2f::*)(void)>(_SC("opPreDec"), &Vector2f::operator --)
        .Func<Vector2f (Vector2f::*)(int)>(_SC("opPostInc"), &Vector2f::operator ++)
        .Func<Vector2f (Vector2f::*)(int)>(_SC("opPostDec"), &Vector2f::operator --)

        .Func<Vector2f (Vector2f::*)(const Vector2f &) const>(_SC("opAdd"), &Vector2f::operator +)
        .Func<Vector2f (Vector2f::*)(const Vector2f &) const>(_SC("opSub"), &Vector2f::operator -)
        .Func<Vector2f (Vector2f::*)(const Vector2f &) const>(_SC("opMul"), &Vector2f::operator *)
        .Func<Vector2f (Vector2f::*)(const Vector2f &) const>(_SC("opDiv"), &Vector2f::operator /)
        .Func<Vector2f (Vector2f::*)(const Vector2f &) const>(_SC("opMod"), &Vector2f::operator %)

        .Func<Vector2f (Vector2f::*)(Vector2f::Value) const>(_SC("opAddS"), &Vector2f::operator +)
        .Func<Vector2f (Vector2f::*)(Vector2f::Value) const>(_SC("opSubS"), &Vector2f::operator -)
        .Func<Vector2f (Vector2f::*)(Vector2f::Value) const>(_SC("opMulS"), &Vector2f::operator *)
        .Func<Vector2f (Vector2f::*)(Vector2f::Value) const>(_SC("opDivS"), &Vector2f::operator /)
        .Func<Vector2f (Vector2f::*)(Vector2f::Value) const>(_SC("opModS"), &Vector2f::operator %)

        .Func<Vector2f (Vector2f::*)(void) const>(_SC("opUnPlus"), &Vector2f::operator +)
        .Func<Vector2f (Vector2f::*)(void) const>(_SC("opUnMinus"), &Vector2f::operator -)

        .Func<bool (Vector2f::*)(const Vector2f &) const>(_SC("opEqual"), &Vector2f::operator ==)
        .Func<bool (Vector2f::*)(const Vector2f &) const>(_SC("opNotEqual"), &Vector2f::operator !=)
        .Func<bool (Vector2f::*)(const Vector2f &) const>(_SC("opLessThan"), &Vector2f::operator <)
        .Func<bool (Vector2f::*)(const Vector2f &) const>(_SC("opGreaterThan"), &Vector2f::operator >)
        .Func<bool (Vector2f::*)(const Vector2f &) const>(_SC("opLessEqual"), &Vector2f::operator <=)
        .Func<bool (Vector2f::*)(const Vector2f &) const>(_SC("opGreaterEqual"), &Vector2f::operator >=)
    );

    LogDbg("Registration of <Vector2f> type was successful");

    return true;
}

} // Namespace:: SqMod
