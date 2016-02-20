// ------------------------------------------------------------------------------------------------
#include "Base/Vector2i.hpp"
#include "Base/Vector2.hpp"
#include "Base/Shared.hpp"
#include "Library/Random.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Vector2i Vector2i::NIL = Vector2i(0);
const Vector2i Vector2i::MIN = Vector2i(NumLimit< Vector2i::Value >::Min);
const Vector2i Vector2i::MAX = Vector2i(NumLimit< Vector2i::Value >::Max);

// ------------------------------------------------------------------------------------------------
SQChar Vector2i::Delim = ',';

// ------------------------------------------------------------------------------------------------
Vector2i::Vector2i()
    : x(0), y(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector2i::Vector2i(Value sv)
    : x(sv), y(sv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector2i::Vector2i(Value xv, Value yv)
    : x(xv), y(yv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector2i::Vector2i(const Vector2i & o)
    : x(o.x), y(o.y)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector2i::~Vector2i()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator = (const Vector2i & o)
{
    x = o.x;
    y = o.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator = (Value s)
{
    x = s;
    y = s;
    return *this;
}

Vector2i & Vector2i::operator = (CSStr values)
{
    Set(GetVector2i(values, Delim));
    return *this;
}

Vector2i & Vector2i::operator = (const Vector2 & v)
{
    x = Value(v.x);
    y = Value(v.y);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator += (const Vector2i & v)
{
    x += v.x;
    y += v.y;
    return *this;
}

Vector2i & Vector2i::operator -= (const Vector2i & v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2i & Vector2i::operator *= (const Vector2i & v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

Vector2i & Vector2i::operator /= (const Vector2i & v)
{
    x /= v.x;
    y /= v.y;
    return *this;
}

Vector2i & Vector2i::operator %= (const Vector2i & v)
{
    x %= v.x;
    y %= v.y;
    return *this;
}

Vector2i & Vector2i::operator &= (const Vector2i & v)
{
    x &= v.x;
    y &= v.y;
    return *this;
}

Vector2i & Vector2i::operator |= (const Vector2i & v)
{
    x |= v.x;
    y |= v.y;
    return *this;
}

Vector2i & Vector2i::operator ^= (const Vector2i & v)
{
    x ^= v.x;
    y ^= v.y;
    return *this;
}

Vector2i & Vector2i::operator <<= (const Vector2i & v)
{
    x <<= v.x;
    y <<= v.y;
    return *this;
}

Vector2i & Vector2i::operator >>= (const Vector2i & v)
{
    x >>= v.x;
    y >>= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator += (Value s)
{
    x += s;
    y += s;
    return *this;
}

Vector2i & Vector2i::operator -= (Value s)
{
    x -= s;
    y -= s;
    return *this;
}

Vector2i & Vector2i::operator *= (Value s)
{
    x *= s;
    y *= s;
    return *this;
}

Vector2i & Vector2i::operator /= (Value s)
{
    x /= s;
    y /= s;
    return *this;
}

Vector2i & Vector2i::operator %= (Value s)
{
    x %= s;
    y %= s;
    return *this;
}

Vector2i & Vector2i::operator &= (Value s)
{
    x &= s;
    y &= s;
    return *this;
}

Vector2i & Vector2i::operator |= (Value s)
{
    x |= s;
    y |= s;
    return *this;
}

Vector2i & Vector2i::operator ^= (Value s)
{
    x += s;
    y += s;
    return *this;
}

Vector2i & Vector2i::operator <<= (Value s)
{
    x <<= s;
    y <<= s;
    return *this;
}

Vector2i & Vector2i::operator >>= (Value s)
{
    x >>= s;
    y >>= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator ++ ()
{
    ++x;
    ++y;
    return *this;
}

Vector2i & Vector2i::operator -- ()
{
    --x;
    --y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator ++ (int)
{
    Vector2i state(*this);
    ++x;
    ++y;
    return state;
}

Vector2i Vector2i::operator -- (int)
{
    Vector2i state(*this);
    --x;
    --y;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator + (const Vector2i & v) const
{
    return Vector2i(x + v.x, y + v.y);
}

Vector2i Vector2i::operator - (const Vector2i & v) const
{
    return Vector2i(x - v.x, y - v.y);
}

Vector2i Vector2i::operator * (const Vector2i & v) const
{
    return Vector2i(x * v.x, y * v.y);
}

Vector2i Vector2i::operator / (const Vector2i & v) const
{
    return Vector2i(x / v.x, y / v.y);
}

Vector2i Vector2i::operator % (const Vector2i & v) const
{
    return Vector2i(x % v.x, y % v.y);
}

Vector2i Vector2i::operator & (const Vector2i & v) const
{
    return Vector2i(x & v.x, y & v.y);
}

Vector2i Vector2i::operator | (const Vector2i & v) const
{
    return Vector2i(x | v.x, y | v.y);
}

Vector2i Vector2i::operator ^ (const Vector2i & v) const
{
    return Vector2i(x ^ v.x, y ^ v.y);
}

Vector2i Vector2i::operator << (const Vector2i & v) const
{
    return Vector2i(x << v.x, y << v.y);
}

Vector2i Vector2i::operator >> (const Vector2i & v) const
{
    return Vector2i(x >> v.x, y >> v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator + (Value s) const
{
    return Vector2i(x + s, y + s);
}

Vector2i Vector2i::operator - (Value s) const
{
    return Vector2i(x - s, y - s);
}

Vector2i Vector2i::operator * (Value s) const
{
    return Vector2i(x * s, y * s);
}

Vector2i Vector2i::operator / (Value s) const
{
    return Vector2i(x / s, y / s);
}

Vector2i Vector2i::operator % (Value s) const
{
    return Vector2i(x % s, y % s);
}

Vector2i Vector2i::operator & (Value s) const
{
    return Vector2i(x & s, y & s);
}

Vector2i Vector2i::operator | (Value s) const
{
    return Vector2i(x | s, y | s);
}

Vector2i Vector2i::operator ^ (Value s) const
{
    return Vector2i(x ^ s, y ^ s);
}

Vector2i Vector2i::operator << (Value s) const
{
    return Vector2i(x < s, y < s);
}

Vector2i Vector2i::operator >> (Value s) const
{
    return Vector2i(x >> s, y >> s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator + () const
{
    return Vector2i(abs(x), abs(y));
}

Vector2i Vector2i::operator - () const
{
    return Vector2i(-x, -y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator ~ () const
{
    return Vector2i(~x, ~y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2i::operator == (const Vector2i & v) const
{
    return (x == v.x) && (y == v.y);
}

bool Vector2i::operator != (const Vector2i & v) const
{
    return (x != v.x) && (y != v.y);
}

bool Vector2i::operator < (const Vector2i & v) const
{
    return (x < v.x) && (y < v.y);
}

bool Vector2i::operator > (const Vector2i & v) const
{
    return (x > v.x) && (y > v.y);
}

bool Vector2i::operator <= (const Vector2i & v) const
{
    return (x <= v.x) && (y <= v.y);
}

bool Vector2i::operator >= (const Vector2i & v) const
{
        return (x >= v.x) && (y >= v.y);
}

// ------------------------------------------------------------------------------------------------
Int32 Vector2i::Cmp(const Vector2i & o) const
{
    if (*this == o)
        return 0;
    else if (*this > o)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr Vector2i::ToString() const
{
    return ToStringF("%d,%d", x, y);
}

// ------------------------------------------------------------------------------------------------
void Vector2i::Set(Value ns)
{
    x = ns;
    y = ns;
}

void Vector2i::Set(Value nx, Value ny)
{
    x = nx;
    y = ny;
}

// ------------------------------------------------------------------------------------------------
void Vector2i::Set(const Vector2i & v)
{
    x = v.x;
    y = v.y;
}

void Vector2i::Set(const Vector2 & v)
{
    x = Value(v.x);
    y = Value(v.y);
}

// ------------------------------------------------------------------------------------------------
void Vector2i::Set(CSStr values, SQChar delim)
{
    Set(GetVector2i(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Vector2i::Generate()
{
    x = GetRandomInt32();
    y = GetRandomInt32();
}

void Vector2i::Generate(Value min, Value max)
{
    if (max < min)
    {
        SqThrow("max value is lower than min value");
    }
    else
    {
        x = GetRandomInt32(min, max);
        y = GetRandomInt32(min, max);
    }
}

void Vector2i::Generate(Value xmin, Value xmax, Value ymin, Value ymax)
{
    if (xmax < xmin || ymax < ymin)
    {
        SqThrow("max value is lower than min value");
    }
    else
    {
        x = GetRandomInt32(ymin, ymax);
        y = GetRandomInt32(xmin, xmax);
    }
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::Abs() const
{
    return Vector2i(abs(x), abs(y));
}

// ================================================================================================
void Register_Vector2i(HSQUIRRELVM vm)
{
    typedef Vector2i::Value Val;

    RootTable(vm).Bind(_SC("Vector2i"), Class< Vector2i >(vm, _SC("Vector2i"))
        /* Constructors */
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val >()
        /* Static Members */
        .SetStaticValue(_SC("Delim"), &Vector2i::Delim)
        /* Member Variables */
        .Var(_SC("x"), &Vector2i::x)
        .Var(_SC("y"), &Vector2i::y)
        /* Properties */
        .Prop(_SC("abs"), &Vector2i::Abs)
        /* Core Metamethods */
        .Func(_SC("_tostring"), &Vector2i::ToString)
        .Func(_SC("_cmp"), &Vector2i::Cmp)
        /* Metamethods */
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("_add"), &Vector2i::operator +)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("_sub"), &Vector2i::operator -)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("_mul"), &Vector2i::operator *)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("_div"), &Vector2i::operator /)
        .Func<Vector2i (Vector2i::*)(const Vector2i &) const>(_SC("_modulo"), &Vector2i::operator %)
        .Func<Vector2i (Vector2i::*)(void) const>(_SC("_unm"), &Vector2i::operator -)
        /* Setters */
        .Overload<void (Vector2i::*)(Val)>(_SC("Set"), &Vector2i::Set)
        .Overload<void (Vector2i::*)(Val, Val)>(_SC("Set"), &Vector2i::Set)
        .Overload<void (Vector2i::*)(const Vector2i &)>(_SC("SetVec2i"), &Vector2i::Set)
        .Overload<void (Vector2i::*)(const Vector2 &)>(_SC("SetVec2"), &Vector2i::Set)
        .Overload<void (Vector2i::*)(CSStr, SQChar)>(_SC("SetStr"), &Vector2i::Set)
        /* Random Generators */
        .Overload<void (Vector2i::*)(void)>(_SC("Generate"), &Vector2i::Generate)
        .Overload<void (Vector2i::*)(Val, Val)>(_SC("Generate"), &Vector2i::Generate)
        .Overload<void (Vector2i::*)(Val, Val, Val, Val)>(_SC("Generate"), &Vector2i::Generate)
        /* Utility Methods */
        .Func(_SC("Clear"), &Vector2i::Clear)
        /* Operator Exposure */
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
}

} // Namespace:: SqMod
