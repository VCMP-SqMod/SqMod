#include "Base/Vector2u.hpp"
#include "Base/Vector2f.hpp"
#include "Base/Vector2i.hpp"
#include "Base/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Vector2u Vector2u::NIL = Vector2u(0);
const Vector2u Vector2u::MIN = Vector2u(std::numeric_limits<Vector2u::Value>::min());
const Vector2u Vector2u::MAX = Vector2u(std::numeric_limits<Vector2u::Value>::max());

// ------------------------------------------------------------------------------------------------
SQChar Vector2u::Delim = ',';

// ------------------------------------------------------------------------------------------------
Vector2u::Vector2u()
    : x(0), y(0)
{

}

Vector2u::Vector2u(Value s)
    : x(s), y(s)
{

}

Vector2u::Vector2u(Value xv, Value yv)
    : x(xv), y(yv)
{

}

// ------------------------------------------------------------------------------------------------
Vector2u::Vector2u(const Vector2i & v)
    : x(static_cast<Value>(v.x)), y(static_cast<Value>(v.y))
{

}

Vector2u::Vector2u(const Vector2f & v)
    : x(static_cast<Value>(v.x)), y(static_cast<Value>(v.y))
{

}

// ------------------------------------------------------------------------------------------------
Vector2u::Vector2u(const SQChar * values, SQChar delim)
    : Vector2u(GetVector2u(values, delim))
{

}

// ------------------------------------------------------------------------------------------------
Vector2u::Vector2u(const Vector2u & v)
    : x(v.x), y(v.y)
{

}

Vector2u::Vector2u(Vector2u && v)
    : x(v.x), y(v.y)
{

}

// ------------------------------------------------------------------------------------------------
Vector2u::~Vector2u()
{

}

// ------------------------------------------------------------------------------------------------
Vector2u & Vector2u::operator = (const Vector2u & v)
{
    x = v.x;
    y = v.y;
    return *this;
}

Vector2u & Vector2u::operator = (Vector2u && v)
{
    x = v.x;
    y = v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2u & Vector2u::operator = (Value s)
{
    x = s;
    y = s;
    return *this;
}

Vector2u & Vector2u::operator = (const SQChar * values)
{
    Set(GetVector2i(values, Delim));
    return *this;
}

Vector2u & Vector2u::operator = (const Vector2i & v)
{
    x = static_cast<Value>(v.x);
    y = static_cast<Value>(v.y);
    return *this;
}

Vector2u & Vector2u::operator = (const Vector2f & v)
{
    x = static_cast<Value>(v.x);
    y = static_cast<Value>(v.y);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2u & Vector2u::operator += (const Vector2u & v)
{
    x += v.x;
    y += v.y;
    return *this;
}

Vector2u & Vector2u::operator -= (const Vector2u & v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2u & Vector2u::operator *= (const Vector2u & v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

Vector2u & Vector2u::operator /= (const Vector2u & v)
{
    x /= v.x;
    y /= v.y;
    return *this;
}

Vector2u & Vector2u::operator %= (const Vector2u & v)
{
    x %= v.x;
    y %= v.y;
    return *this;
}

Vector2u & Vector2u::operator &= (const Vector2u & v)
{
    x &= v.x;
    y &= v.y;
    return *this;
}

Vector2u & Vector2u::operator |= (const Vector2u & v)
{
    x |= v.x;
    y |= v.y;
    return *this;
}

Vector2u & Vector2u::operator ^= (const Vector2u & v)
{
    x ^= v.x;
    y ^= v.y;
    return *this;
}

Vector2u & Vector2u::operator <<= (const Vector2u & v)
{
    x <<= v.x;
    y <<= v.y;
    return *this;
}

Vector2u & Vector2u::operator >>= (const Vector2u & v)
{
    x >>= v.x;
    y >>= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2u & Vector2u::operator += (Value s)
{
    x += s;
    y += s;
    return *this;
}

Vector2u & Vector2u::operator -= (Value s)
{
    x -= s;
    y -= s;
    return *this;
}

Vector2u & Vector2u::operator *= (Value s)
{
    x *= s;
    y *= s;
    return *this;
}

Vector2u & Vector2u::operator /= (Value s)
{
    x /= s;
    y /= s;
    return *this;
}

Vector2u & Vector2u::operator %= (Value s)
{
    x %= s;
    y %= s;
    return *this;
}

Vector2u & Vector2u::operator &= (Value s)
{
    x &= s;
    y &= s;
    return *this;
}

Vector2u & Vector2u::operator |= (Value s)
{
    x |= s;
    y |= s;
    return *this;
}

Vector2u & Vector2u::operator ^= (Value s)
{
    x ^= s;
    y ^= s;
    return *this;
}

Vector2u & Vector2u::operator <<= (Value s)
{
    x <<= s;
    y <<= s;
    return *this;
}

Vector2u & Vector2u::operator >>= (Value s)
{
    x >>= s;
    y >>= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2u & Vector2u::operator ++ ()
{
    ++x;
    ++y;
    return *this;
}

Vector2u & Vector2u::operator -- ()
{
    --x;
    --y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2u Vector2u::operator ++ (int)
{
    Vector2i state(*this);
    ++x;
    ++y;
    return state;
}

Vector2u Vector2u::operator -- (int)
{
    Vector2i state(*this);
    --x;
    --y;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector2u Vector2u::operator + (const Vector2u & v) const
{
    return Vector2i(x + v.x, y + v.y);
}

Vector2u Vector2u::operator - (const Vector2u & v) const
{
    return Vector2i(x - v.x, y - v.y);
}

Vector2u Vector2u::operator * (const Vector2u & v) const
{
    return Vector2i(x * v.x, y * v.y);
}

Vector2u Vector2u::operator / (const Vector2u & v) const
{
    return Vector2i(x / v.x, y / v.y);
}

Vector2u Vector2u::operator % (const Vector2u & v) const
{
    return Vector2i(x % v.x, y % v.y);
}

Vector2u Vector2u::operator & (const Vector2u & v) const
{
    return Vector2i(x & v.x, y & v.y);
}

Vector2u Vector2u::operator | (const Vector2u & v) const
{
    return Vector2i(x | v.x, y | v.y);
}

Vector2u Vector2u::operator ^ (const Vector2u & v) const
{
    return Vector2i(x ^ v.x, y ^ v.y);
}

Vector2u Vector2u::operator << (const Vector2u & v) const
{
    return Vector2i(x << v.x, y << v.y);
}

Vector2u Vector2u::operator >> (const Vector2u & v) const
{
    return Vector2i(x >> v.x, y >> v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2u Vector2u::operator + (Value s) const
{
    return Vector2i(x + s, y + s);
}

Vector2u Vector2u::operator - (Value s) const
{
    return Vector2i(x - s, y - s);
}

Vector2u Vector2u::operator * (Value s) const
{
    return Vector2i(x - s, y - s);
}

Vector2u Vector2u::operator / (Value s) const
{
    return Vector2i(x / s, y / s);
}

Vector2u Vector2u::operator % (Value s) const
{
    return Vector2i(x % s, y % s);
}

Vector2u Vector2u::operator & (Value s) const
{
    return Vector2i(x & s, y & s);
}

Vector2u Vector2u::operator | (Value s) const
{
    return Vector2i(x | s, y | s);
}

Vector2u Vector2u::operator ^ (Value s) const
{
    return Vector2i(x ^ s, y ^ s);
}

Vector2u Vector2u::operator << (Value s) const
{
    return Vector2i(x << s, y << s);
}

Vector2u Vector2u::operator >> (Value s) const
{
    return Vector2i(x >> s, y >> s);
}

// ------------------------------------------------------------------------------------------------
Vector2u Vector2u::operator + () const
{
    return Vector2i(x, y);
}

Vector2u Vector2u::operator - () const
{
    return Vector2i(0, 0);
}

// ------------------------------------------------------------------------------------------------
Vector2u Vector2u::operator ~ () const
{
    return Vector2i(~x, ~y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2u::operator == (const Vector2u & v) const
{
    return (x == v.x) && (y == v.y);
}

bool Vector2u::operator != (const Vector2u & v) const
{
    return (x != v.x) && (y != v.y);
}

bool Vector2u::operator < (const Vector2u & v) const
{
    return (x < v.x) && (y < v.y);
}

bool Vector2u::operator > (const Vector2u & v) const
{
    return (x > v.x) && (y > v.y);
}

bool Vector2u::operator <= (const Vector2u & v) const
{
    return (x <= v.x) && (y <= v.y);
}

bool Vector2u::operator >= (const Vector2u & v) const
{
    return (x >= v.x) && (y >= v.y);
}

// ------------------------------------------------------------------------------------------------
SQInteger Vector2u::Cmp(const Vector2u & v) const
{
    return *this == v ? 0 : (*this > v ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Vector2u::ToString() const
{
    return ToStringF("%u,%u", x, y);
}

// ------------------------------------------------------------------------------------------------
void Vector2u::Set(Value ns)
{
    x = ns;
    y = ns;
}

void Vector2u::Set(Value nx, Value ny)
{
    x = nx;
    y = ny;
}

// ------------------------------------------------------------------------------------------------
void Vector2u::Set(const Vector2u & v)
{
    x = v.x;
    y = v.y;
}

void Vector2u::Set(const Vector2i & v)
{
    x = static_cast<SQInt32>(v.x);
    y = static_cast<SQInt32>(v.y);
}

void Vector2u::Set(const Vector2f & v)
{
    x = static_cast<SQInt32>(v.x);
    y = static_cast<SQInt32>(v.y);
}

// ------------------------------------------------------------------------------------------------
void Vector2u::Set(const SQChar * values, SQChar delim)
{
    Set(GetVector2i(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Vector2u::Generate()
{
    x = RandomVal<Value>::Get();
    y = RandomVal<Value>::Get();
}

void Vector2u::Generate(Value min, Value max)
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

void Vector2u::Generate(Value xmin, Value xmax, Value ymin, Value ymax)
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
Vector2u Vector2u::Abs() const
{
    return Vector2i(x, y);
}

// ================================================================================================
bool Register_Vector2u(HSQUIRRELVM vm)
{
    LogDbg("Beginning registration of <Vector2u> type");

    typedef Vector2u::Value Val;

    Sqrat::RootTable(vm).Bind(_SC("Vector2u"), Sqrat::Class<Vector2u>(vm, _SC("Vector2u"))
        .Ctor()
        .Ctor<Val>()
        .Ctor<Val, Val>()

        .SetStaticValue(_SC("delim"), &Vector2u::Delim)

        .Var(_SC("x"), &Vector2u::x)
        .Var(_SC("y"), &Vector2u::y)

        .Prop(_SC("abs"), &Vector2u::Abs)

        .Func(_SC("_tostring"), &Vector2u::ToString)
        .Func(_SC("_cmp"), &Vector2u::Cmp)

        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("_add"), &Vector2u::operator +)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("_sub"), &Vector2u::operator -)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("_mul"), &Vector2u::operator *)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("_div"), &Vector2u::operator /)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("_modulo"), &Vector2u::operator %)
        .Func<Vector2u (Vector2u::*)(void) const>(_SC("_unm"), &Vector2u::operator -)

        .Overload<void (Vector2u::*)(Val)>(_SC("set"), &Vector2u::Set)
        .Overload<void (Vector2u::*)(Val, Val)>(_SC("set"), &Vector2u::Set)
        .Overload<void (Vector2u::*)(const Vector2u &)>(_SC("set_vec2u"), &Vector2u::Set)
        .Overload<void (Vector2u::*)(const Vector2i &)>(_SC("set_vec2i"), &Vector2u::Set)
        .Overload<void (Vector2u::*)(const Vector2f &)>(_SC("set_vec2f"), &Vector2u::Set)
        .Overload<void (Vector2u::*)(const SQChar *, SQChar)>(_SC("set_str"), &Vector2u::Set)

        .Overload<void (Vector2u::*)(void)>(_SC("generate"), &Vector2u::Generate)
        .Overload<void (Vector2u::*)(Val, Val)>(_SC("generate"), &Vector2u::Generate)
        .Overload<void (Vector2u::*)(Val, Val, Val, Val)>(_SC("generate"), &Vector2u::Generate)

        .Func(_SC("clear"), &Vector2u::Clear)

        .Func<Vector2u & (Vector2u::*)(const Vector2u &)>(_SC("opAddAssign"), &Vector2u::operator +=)
        .Func<Vector2u & (Vector2u::*)(const Vector2u &)>(_SC("opSubAssign"), &Vector2u::operator -=)
        .Func<Vector2u & (Vector2u::*)(const Vector2u &)>(_SC("opMulAssign"), &Vector2u::operator *=)
        .Func<Vector2u & (Vector2u::*)(const Vector2u &)>(_SC("opDivAssign"), &Vector2u::operator /=)
        .Func<Vector2u & (Vector2u::*)(const Vector2u &)>(_SC("opModAssign"), &Vector2u::operator %=)
        .Func<Vector2u & (Vector2u::*)(const Vector2u &)>(_SC("opAndAssign"), &Vector2u::operator &=)
        .Func<Vector2u & (Vector2u::*)(const Vector2u &)>(_SC("opOrAssign"), &Vector2u::operator |=)
        .Func<Vector2u & (Vector2u::*)(const Vector2u &)>(_SC("opXorAssign"), &Vector2u::operator ^=)
        .Func<Vector2u & (Vector2u::*)(const Vector2u &)>(_SC("opShlAssign"), &Vector2u::operator <<=)
        .Func<Vector2u & (Vector2u::*)(const Vector2u &)>(_SC("opShrAssign"), &Vector2u::operator >>=)

        .Func<Vector2u & (Vector2u::*)(Vector2u::Value)>(_SC("opAddAssignS"), &Vector2u::operator +=)
        .Func<Vector2u & (Vector2u::*)(Vector2u::Value)>(_SC("opSubAssignS"), &Vector2u::operator -=)
        .Func<Vector2u & (Vector2u::*)(Vector2u::Value)>(_SC("opMulAssignS"), &Vector2u::operator *=)
        .Func<Vector2u & (Vector2u::*)(Vector2u::Value)>(_SC("opDivAssignS"), &Vector2u::operator /=)
        .Func<Vector2u & (Vector2u::*)(Vector2u::Value)>(_SC("opModAssignS"), &Vector2u::operator %=)
        .Func<Vector2u & (Vector2u::*)(Vector2u::Value)>(_SC("opAndAssignS"), &Vector2u::operator &=)
        .Func<Vector2u & (Vector2u::*)(Vector2u::Value)>(_SC("opOrAssignS"), &Vector2u::operator |=)
        .Func<Vector2u & (Vector2u::*)(Vector2u::Value)>(_SC("opXorAssignS"), &Vector2u::operator ^=)
        .Func<Vector2u & (Vector2u::*)(Vector2u::Value)>(_SC("opShlAssignS"), &Vector2u::operator <<=)
        .Func<Vector2u & (Vector2u::*)(Vector2u::Value)>(_SC("opShrAssignS"), &Vector2u::operator >>=)

        .Func<Vector2u & (Vector2u::*)(void)>(_SC("opPreInc"), &Vector2u::operator ++)
        .Func<Vector2u & (Vector2u::*)(void)>(_SC("opPreDec"), &Vector2u::operator --)
        .Func<Vector2u (Vector2u::*)(int)>(_SC("opPostInc"), &Vector2u::operator ++)
        .Func<Vector2u (Vector2u::*)(int)>(_SC("opPostDec"), &Vector2u::operator --)

        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("opAdd"), &Vector2u::operator +)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("opSub"), &Vector2u::operator -)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("opMul"), &Vector2u::operator *)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("opDiv"), &Vector2u::operator /)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("opMod"), &Vector2u::operator %)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("opAnd"), &Vector2u::operator &)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("opOr"), &Vector2u::operator |)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("opShl"), &Vector2u::operator ^)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("opShl"), &Vector2u::operator <<)
        .Func<Vector2u (Vector2u::*)(const Vector2u &) const>(_SC("opShr"), &Vector2u::operator >>)

        .Func<Vector2u (Vector2u::*)(Vector2u::Value) const>(_SC("opAddS"), &Vector2u::operator +)
        .Func<Vector2u (Vector2u::*)(Vector2u::Value) const>(_SC("opSubS"), &Vector2u::operator -)
        .Func<Vector2u (Vector2u::*)(Vector2u::Value) const>(_SC("opMulS"), &Vector2u::operator *)
        .Func<Vector2u (Vector2u::*)(Vector2u::Value) const>(_SC("opDivS"), &Vector2u::operator /)
        .Func<Vector2u (Vector2u::*)(Vector2u::Value) const>(_SC("opModS"), &Vector2u::operator %)
        .Func<Vector2u (Vector2u::*)(Vector2u::Value) const>(_SC("opAndS"), &Vector2u::operator &)
        .Func<Vector2u (Vector2u::*)(Vector2u::Value) const>(_SC("opOrS"), &Vector2u::operator |)
        .Func<Vector2u (Vector2u::*)(Vector2u::Value) const>(_SC("opShlS"), &Vector2u::operator ^)
        .Func<Vector2u (Vector2u::*)(Vector2u::Value) const>(_SC("opShlS"), &Vector2u::operator <<)
        .Func<Vector2u (Vector2u::*)(Vector2u::Value) const>(_SC("opShrS"), &Vector2u::operator >>)

        .Func<Vector2u (Vector2u::*)(void) const>(_SC("opUnPlus"), &Vector2u::operator +)
        .Func<Vector2u (Vector2u::*)(void) const>(_SC("opUnMinus"), &Vector2u::operator -)
        .Func<Vector2u (Vector2u::*)(void) const>(_SC("opCom"), &Vector2u::operator ~)

        .Func<bool (Vector2u::*)(const Vector2u &) const>(_SC("opEqual"), &Vector2u::operator ==)
        .Func<bool (Vector2u::*)(const Vector2u &) const>(_SC("opNotEqual"), &Vector2u::operator !=)
        .Func<bool (Vector2u::*)(const Vector2u &) const>(_SC("opLessThan"), &Vector2u::operator <)
        .Func<bool (Vector2u::*)(const Vector2u &) const>(_SC("opGreaterThan"), &Vector2u::operator >)
        .Func<bool (Vector2u::*)(const Vector2u &) const>(_SC("opLessEqual"), &Vector2u::operator <=)
        .Func<bool (Vector2u::*)(const Vector2u &) const>(_SC("opGreaterEqual"), &Vector2u::operator >=)
    );

    LogDbg("Registration of <Vector2u> type was successful");

    return true;
}

} // Namespace:: SqMod
