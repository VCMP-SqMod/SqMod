#include "Base/Vector2i.hpp"
#include "Base/Vector2u.hpp"
#include "Base/Vector2f.hpp"
#include "Base/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Vector2i Vector2i::NIL = Vector2i(0);
const Vector2i Vector2i::MIN = Vector2i(std::numeric_limits<Vector2i::Value>::min());
const Vector2i Vector2i::MAX = Vector2i(std::numeric_limits<Vector2i::Value>::max());

// ------------------------------------------------------------------------------------------------
SQChar Vector2i::Delim = ',';

// ------------------------------------------------------------------------------------------------
Vector2i::Vector2i() noexcept
    : x(0), y(0)
{

}

Vector2i::Vector2i(Value s) noexcept
    : x(s), y(s)
{

}

Vector2i::Vector2i(Value xv, Value yv) noexcept
    : x(xv), y(yv)
{

}

// ------------------------------------------------------------------------------------------------
Vector2i::Vector2i(const Vector2u & v) noexcept
    : x(static_cast<Value>(v.x)), y(static_cast<Value>(v.y))
{

}

Vector2i::Vector2i(const Vector2f & v) noexcept
    : x(static_cast<Value>(v.x)), y(static_cast<Value>(v.y))
{

}

// ------------------------------------------------------------------------------------------------
Vector2i::Vector2i(const SQChar * values, SQChar delim) noexcept
    : Vector2i(GetVector2i(values, delim))
{

}

// ------------------------------------------------------------------------------------------------
Vector2i::Vector2i(const Vector2i & v) noexcept
    : x(v.x), y(v.y)
{

}

Vector2i::Vector2i(Vector2i && v) noexcept
    : x(v.x), y(v.y)
{

}

// ------------------------------------------------------------------------------------------------
Vector2i::~Vector2i()
{

}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator = (const Vector2i & v) noexcept
{
    x = v.x;
    y = v.y;
    return *this;
}

Vector2i & Vector2i::operator = (Vector2i && v) noexcept
{
    x = v.x;
    y = v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator = (Value s) noexcept
{
    x = s;
    y = s;
    return *this;
}

Vector2i & Vector2i::operator = (const SQChar * values) noexcept
{
    Set(GetVector2i(values, Delim));
    return *this;
}

Vector2i & Vector2i::operator = (const Vector2u & v) noexcept
{
    x = static_cast<Value>(v.x);
    y = static_cast<Value>(v.y);
    return *this;
}

Vector2i & Vector2i::operator = (const Vector2f & v) noexcept
{
    x = static_cast<Value>(v.x);
    y = static_cast<Value>(v.y);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator += (const Vector2i & v) noexcept
{
    x += v.x;
    y += v.y;
    return *this;
}

Vector2i & Vector2i::operator -= (const Vector2i & v) noexcept
{
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2i & Vector2i::operator *= (const Vector2i & v) noexcept
{
    x *= v.x;
    y *= v.y;
    return *this;
}

Vector2i & Vector2i::operator /= (const Vector2i & v) noexcept
{
    x /= v.x;
    y /= v.y;
    return *this;
}

Vector2i & Vector2i::operator %= (const Vector2i & v) noexcept
{
    x %= v.x;
    y %= v.y;
    return *this;
}

Vector2i & Vector2i::operator &= (const Vector2i & v) noexcept
{
    x &= v.x;
    y &= v.y;
    return *this;
}

Vector2i & Vector2i::operator |= (const Vector2i & v) noexcept
{
    x |= v.x;
    y |= v.y;
    return *this;
}

Vector2i & Vector2i::operator ^= (const Vector2i & v) noexcept
{
    x ^= v.x;
    y ^= v.y;
    return *this;
}

Vector2i & Vector2i::operator <<= (const Vector2i & v) noexcept
{
    x <<= v.x;
    y <<= v.y;
    return *this;
}

Vector2i & Vector2i::operator >>= (const Vector2i & v) noexcept
{
    x >>= v.x;
    y >>= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator += (Value s) noexcept
{
    x += s;
    y += s;
    return *this;
}

Vector2i & Vector2i::operator -= (Value s) noexcept
{
    x -= s;
    y -= s;
    return *this;
}

Vector2i & Vector2i::operator *= (Value s) noexcept
{
    x *= s;
    y *= s;
    return *this;
}

Vector2i & Vector2i::operator /= (Value s) noexcept
{
    x /= s;
    y /= s;
    return *this;
}

Vector2i & Vector2i::operator %= (Value s) noexcept
{
    x %= s;
    y %= s;
    return *this;
}

Vector2i & Vector2i::operator &= (Value s) noexcept
{
    x &= s;
    y &= s;
    return *this;
}

Vector2i & Vector2i::operator |= (Value s) noexcept
{
    x |= s;
    y |= s;
    return *this;
}

Vector2i & Vector2i::operator ^= (Value s) noexcept
{
    x += s;
    y += s;
    return *this;
}

Vector2i & Vector2i::operator <<= (Value s) noexcept
{
    x <<= s;
    y <<= s;
    return *this;
}

Vector2i & Vector2i::operator >>= (Value s) noexcept
{
    x >>= s;
    y >>= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator ++ () noexcept
{
    ++x;
    ++y;
    return *this;
}

Vector2i & Vector2i::operator -- () noexcept
{
    --x;
    --y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator ++ (int) noexcept
{
    Vector2i state(*this);
    ++x;
    ++y;
    return state;
}

Vector2i Vector2i::operator -- (int) noexcept
{
    Vector2i state(*this);
    --x;
    --y;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator + (const Vector2i & v) const noexcept
{
    return Vector2i(x + v.x, y + v.y);
}

Vector2i Vector2i::operator - (const Vector2i & v) const noexcept
{
    return Vector2i(x - v.x, y - v.y);
}

Vector2i Vector2i::operator * (const Vector2i & v) const noexcept
{
    return Vector2i(x * v.x, y * v.y);
}

Vector2i Vector2i::operator / (const Vector2i & v) const noexcept
{
    return Vector2i(x / v.x, y / v.y);
}

Vector2i Vector2i::operator % (const Vector2i & v) const noexcept
{
    return Vector2i(x % v.x, y % v.y);
}

Vector2i Vector2i::operator & (const Vector2i & v) const noexcept
{
    return Vector2i(x & v.x, y & v.y);
}

Vector2i Vector2i::operator | (const Vector2i & v) const noexcept
{
    return Vector2i(x | v.x, y | v.y);
}

Vector2i Vector2i::operator ^ (const Vector2i & v) const noexcept
{
    return Vector2i(x ^ v.x, y ^ v.y);
}

Vector2i Vector2i::operator << (const Vector2i & v) const noexcept
{
    return Vector2i(x << v.x, y << v.y);
}

Vector2i Vector2i::operator >> (const Vector2i & v) const noexcept
{
    return Vector2i(x >> v.x, y >> v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator + (Value s) const noexcept
{
    return Vector2i(x + s, y + s);
}

Vector2i Vector2i::operator - (Value s) const noexcept
{
    return Vector2i(x - s, y - s);
}

Vector2i Vector2i::operator * (Value s) const noexcept
{
    return Vector2i(x * s, y * s);
}

Vector2i Vector2i::operator / (Value s) const noexcept
{
    return Vector2i(x / s, y / s);
}

Vector2i Vector2i::operator % (Value s) const noexcept
{
    return Vector2i(x % s, y % s);
}

Vector2i Vector2i::operator & (Value s) const noexcept
{
    return Vector2i(x & s, y & s);
}

Vector2i Vector2i::operator | (Value s) const noexcept
{
    return Vector2i(x | s, y | s);
}

Vector2i Vector2i::operator ^ (Value s) const noexcept
{
    return Vector2i(x ^ s, y ^ s);
}

Vector2i Vector2i::operator << (Value s) const noexcept
{
    return Vector2i(x < s, y < s);
}

Vector2i Vector2i::operator >> (Value s) const noexcept
{
    return Vector2i(x >> s, y >> s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator + () const noexcept
{
    return Vector2i(std::abs(x), std::abs(y));
}

Vector2i Vector2i::operator - () const noexcept
{
    return Vector2i(-x, -y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator ~ () const noexcept
{
    return Vector2i(~x, ~y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2i::operator == (const Vector2i & v) const noexcept
{
    return (x == v.x) && (y == v.y);
}

bool Vector2i::operator != (const Vector2i & v) const noexcept
{
    return (x != v.x) && (y != v.y);
}

bool Vector2i::operator < (const Vector2i & v) const noexcept
{
    return (x < v.x) && (y < v.y);
}

bool Vector2i::operator > (const Vector2i & v) const noexcept
{
    return (x > v.x) && (y > v.y);
}

bool Vector2i::operator <= (const Vector2i & v) const noexcept
{
    return (x <= v.x) && (y <= v.y);
}

bool Vector2i::operator >= (const Vector2i & v) const noexcept
{
        return (x >= v.x) && (y >= v.y);
}

// ------------------------------------------------------------------------------------------------
SQInteger Vector2i::Cmp(const Vector2i & v) const noexcept
{
    return *this == v ? 0 : (*this > v ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Vector2i::ToString() const noexcept
{
    return ToStringF("%d,%d", x, y);
}

// ------------------------------------------------------------------------------------------------
void Vector2i::Set(Value ns) noexcept
{
    x = ns;
    y = ns;
}

void Vector2i::Set(Value nx, Value ny) noexcept
{
    x = nx;
    y = ny;
}

// ------------------------------------------------------------------------------------------------
void Vector2i::Set(const Vector2i & v) noexcept
{
    x = v.x;
    y = v.y;
}

void Vector2i::Set(const Vector2u & v) noexcept
{
    x = static_cast<Value>(v.x);
    y = static_cast<Value>(v.y);
}

void Vector2i::Set(const Vector2f & v) noexcept
{
    x = static_cast<Value>(v.x);
    y = static_cast<Value>(v.y);
}

// ------------------------------------------------------------------------------------------------
void Vector2i::Set(const SQChar * values, SQChar delim) noexcept
{
    Set(GetVector2i(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Vector2i::Generate() noexcept
{
    x = RandomVal<Value>::Get();
    y = RandomVal<Value>::Get();
}

void Vector2i::Generate(Value min, Value max) noexcept
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

void Vector2i::Generate(Value xmin, Value xmax, Value ymin, Value ymax) noexcept
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
Vector2i Vector2i::Abs() const noexcept
{
    return Vector2i(std::abs(x), std::abs(y));
}

// ================================================================================================
bool Register_Vector2i(HSQUIRRELVM vm)
{
    LogDbg("Beginning registration of <Vector2i> type");

    typedef Vector2i::Value Val;

    Sqrat::RootTable(vm).Bind(_SC("Vector2i"), Sqrat::Class<Vector2i>(vm, _SC("Vector2i"))
        .Ctor()
        .Ctor<Val>()
        .Ctor<Val, Val>()

        .SetStaticValue(_SC("delim"), &Vector2i::Delim)

        .Var(_SC("x"), &Vector2i::x)
        .Var(_SC("y"), &Vector2i::y)

        .Prop(_SC("abs"), &Vector2i::Abs)

        .Func(_SC("_tostring"), &Vector2i::ToString)
        .Func(_SC("_cmp"), &Vector2i::Cmp)

        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("_add"), &Vector2i::operator +)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("_sub"), &Vector2i::operator -)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("_mul"), &Vector2i::operator *)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("_div"), &Vector2i::operator /)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("_modulo"), &Vector2i::operator %)
        .Func<Vector2i (Vector2i::*)(void) const>(_SC("_unm"), &Vector2i::operator -)

        .Overload<void (Vector2i::*)(Val)>(_SC("set"), &Vector2i::Set)
        .Overload<void (Vector2i::*)(Val, Val)>(_SC("set"), &Vector2i::Set)
        .Overload<void (Vector2i::*)(const Vector2i &)>(_SC("set_vec2i"), &Vector2i::Set)
        .Overload<void (Vector2i::*)(const Vector2u &)>(_SC("set_vec2u"), &Vector2i::Set)
        .Overload<void (Vector2i::*)(const Vector2f &)>(_SC("set_vec2f"), &Vector2i::Set)
        .Overload<void (Vector2i::*)(const SQChar *, SQChar)>(_SC("set_str"), &Vector2i::Set)

        .Overload<void (Vector2i::*)(void)>(_SC("generate"), &Vector2i::Generate)
        .Overload<void (Vector2i::*)(Val, Val)>(_SC("generate"), &Vector2i::Generate)
        .Overload<void (Vector2i::*)(Val, Val, Val, Val)>(_SC("generate"), &Vector2i::Generate)

        .Func(_SC("clear"), &Vector2i::Clear)

        .Func<Vector2i & (Vector2i::*)(const Vector2i &)>(_SC("opAddAssign"), &Vector2i::operator +=)
        .Func<Vector2i & (Vector2i::*)(const Vector2i &)>(_SC("opSubAssign"), &Vector2i::operator -=)
        .Func<Vector2i & (Vector2i::*)(const Vector2i &)>(_SC("opMulAssign"), &Vector2i::operator *=)
        .Func<Vector2i & (Vector2i::*)(const Vector2i &)>(_SC("opDivAssign"), &Vector2i::operator /=)
        .Func<Vector2i & (Vector2i::*)(const Vector2i &)>(_SC("opModAssign"), &Vector2i::operator %=)
        .Func<Vector2i & (Vector2i::*)(const Vector2i &)>(_SC("opAndAssign"), &Vector2i::operator &=)
        .Func<Vector2i & (Vector2i::*)(const Vector2i &)>(_SC("opOrAssign"), &Vector2i::operator |=)
        .Func<Vector2i & (Vector2i::*)(const Vector2i &)>(_SC("opXorAssign"), &Vector2i::operator ^=)
        .Func<Vector2i & (Vector2i::*)(const Vector2i &)>(_SC("opShlAssign"), &Vector2i::operator <<=)
        .Func<Vector2i & (Vector2i::*)(const Vector2i &)>(_SC("opShrAssign"), &Vector2i::operator >>=)

        .Func<Vector2i & (Vector2i::*)(Vector2i::Value)>(_SC("opAddAssignS"), &Vector2i::operator +=)
        .Func<Vector2i & (Vector2i::*)(Vector2i::Value)>(_SC("opSubAssignS"), &Vector2i::operator -=)
        .Func<Vector2i & (Vector2i::*)(Vector2i::Value)>(_SC("opMulAssignS"), &Vector2i::operator *=)
        .Func<Vector2i & (Vector2i::*)(Vector2i::Value)>(_SC("opDivAssignS"), &Vector2i::operator /=)
        .Func<Vector2i & (Vector2i::*)(Vector2i::Value)>(_SC("opModAssignS"), &Vector2i::operator %=)
        .Func<Vector2i & (Vector2i::*)(Vector2i::Value)>(_SC("opAndAssignS"), &Vector2i::operator &=)
        .Func<Vector2i & (Vector2i::*)(Vector2i::Value)>(_SC("opOrAssignS"), &Vector2i::operator |=)
        .Func<Vector2i & (Vector2i::*)(Vector2i::Value)>(_SC("opXorAssignS"), &Vector2i::operator ^=)
        .Func<Vector2i & (Vector2i::*)(Vector2i::Value)>(_SC("opShlAssignS"), &Vector2i::operator <<=)
        .Func<Vector2i & (Vector2i::*)(Vector2i::Value)>(_SC("opShrAssignS"), &Vector2i::operator >>=)

        .Func<Vector2i & (Vector2i::*)(void)>(_SC("opPreInc"), &Vector2i::operator ++)
        .Func<Vector2i & (Vector2i::*)(void)>(_SC("opPreDec"), &Vector2i::operator --)
        .Func<Vector2i (Vector2i::*)(int)>(_SC("opPostInc"), &Vector2i::operator ++)
        .Func<Vector2i (Vector2i::*)(int)>(_SC("opPostDec"), &Vector2i::operator --)

        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("opAdd"), &Vector2i::operator +)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("opSub"), &Vector2i::operator -)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("opMul"), &Vector2i::operator *)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("opDiv"), &Vector2i::operator /)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("opMod"), &Vector2i::operator %)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("opAnd"), &Vector2i::operator &)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("opOr"), &Vector2i::operator |)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("opShl"), &Vector2i::operator ^)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("opShl"), &Vector2i::operator <<)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("opShr"), &Vector2i::operator >>)

        .Func<Vector2i (Vector2i::*)(Vector2i::Value) const>(_SC("opAddS"), &Vector2i::operator +)
        .Func<Vector2i (Vector2i::*)(Vector2i::Value) const>(_SC("opSubS"), &Vector2i::operator -)
        .Func<Vector2i (Vector2i::*)(Vector2i::Value) const>(_SC("opMulS"), &Vector2i::operator *)
        .Func<Vector2i (Vector2i::*)(Vector2i::Value) const>(_SC("opDivS"), &Vector2i::operator /)
        .Func<Vector2i (Vector2i::*)(Vector2i::Value) const>(_SC("opModS"), &Vector2i::operator %)
        .Func<Vector2i (Vector2i::*)(Vector2i::Value) const>(_SC("opAndS"), &Vector2i::operator &)
        .Func<Vector2i (Vector2i::*)(Vector2i::Value) const>(_SC("opOrS"), &Vector2i::operator |)
        .Func<Vector2i (Vector2i::*)(Vector2i::Value) const>(_SC("opShlS"), &Vector2i::operator ^)
        .Func<Vector2i (Vector2i::*)(Vector2i::Value) const>(_SC("opShlS"), &Vector2i::operator <<)
        .Func<Vector2i (Vector2i::*)(Vector2i::Value) const>(_SC("opShrS"), &Vector2i::operator >>)

        .Func<Vector2i (Vector2i::*)(void) const>(_SC("opUnPlus"), &Vector2i::operator +)
        .Func<Vector2i (Vector2i::*)(void) const>(_SC("opUnMinus"), &Vector2i::operator -)
        .Func<Vector2i (Vector2i::*)(void) const>(_SC("opCom"), &Vector2i::operator ~)

        .Func<bool (Vector2i::*)(const Vector2i &) const>(_SC("opEqual"), &Vector2i::operator ==)
        .Func<bool (Vector2i::*)(const Vector2i &) const>(_SC("opNotEqual"), &Vector2i::operator !=)
        .Func<bool (Vector2i::*)(const Vector2i &) const>(_SC("opLessThan"), &Vector2i::operator <)
        .Func<bool (Vector2i::*)(const Vector2i &) const>(_SC("opGreaterThan"), &Vector2i::operator >)
        .Func<bool (Vector2i::*)(const Vector2i &) const>(_SC("opLessEqual"), &Vector2i::operator <=)
        .Func<bool (Vector2i::*)(const Vector2i &) const>(_SC("opGreaterEqual"), &Vector2i::operator >=)
    );

    LogDbg("Registration of <Vector2i> type was successful");

    return true;
}

} // Namespace:: SqMod
