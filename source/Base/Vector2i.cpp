// ------------------------------------------------------------------------------------------------
#include "Base/Vector2i.hpp"
#include "Base/Vector2.hpp"
#include "Base/Shared.hpp"
#include "Base/DynArg.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("Vector2i"))

// ------------------------------------------------------------------------------------------------
const Vector2i Vector2i::NIL = Vector2i(0);
const Vector2i Vector2i::MIN = Vector2i(std::numeric_limits< Vector2i::Value >::min());
const Vector2i Vector2i::MAX = Vector2i(std::numeric_limits< Vector2i::Value >::max());

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
Vector2i & Vector2i::operator = (Value s)
{
    x = s;
    y = s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator = (const Vector2 & v)
{
    x = ConvTo< Value >::From(v.x);
    y = ConvTo< Value >::From(v.y);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator += (const Vector2i & v)
{
    x += v.x;
    y += v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator -= (const Vector2i & v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator *= (const Vector2i & v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator /= (const Vector2i & v)
{
    x /= v.x;
    y /= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator %= (const Vector2i & v)
{
    x %= v.x;
    y %= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator &= (const Vector2i & v)
{
    x &= v.x;
    y &= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator |= (const Vector2i & v)
{
    x |= v.x;
    y |= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator ^= (const Vector2i & v)
{
    x ^= v.x;
    y ^= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator <<= (const Vector2i & v)
{
    x <<= v.x;
    y <<= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator -= (Value s)
{
    x -= s;
    y -= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator *= (Value s)
{
    x *= s;
    y *= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator /= (Value s)
{
    x /= s;
    y /= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator %= (Value s)
{
    x %= s;
    y %= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator &= (Value s)
{
    x &= s;
    y &= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator |= (Value s)
{
    x |= s;
    y |= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator ^= (Value s)
{
    x += s;
    y += s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator <<= (Value s)
{
    x <<= s;
    y <<= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator - (const Vector2i & v) const
{
    return Vector2i(x - v.x, y - v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator * (const Vector2i & v) const
{
    return Vector2i(x * v.x, y * v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator / (const Vector2i & v) const
{
    return Vector2i(x / v.x, y / v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator % (const Vector2i & v) const
{
    return Vector2i(x % v.x, y % v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator & (const Vector2i & v) const
{
    return Vector2i(x & v.x, y & v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator | (const Vector2i & v) const
{
    return Vector2i(x | v.x, y | v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator ^ (const Vector2i & v) const
{
    return Vector2i(x ^ v.x, y ^ v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator << (const Vector2i & v) const
{
    return Vector2i(x << v.x, y << v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator >> (const Vector2i & v) const
{
    return Vector2i(x >> v.x, y >> v.y);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator + (Value s) const
{
    return Vector2i(x + s, y + s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator - (Value s) const
{
    return Vector2i(x - s, y - s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator * (Value s) const
{
    return Vector2i(x * s, y * s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator / (Value s) const
{
    return Vector2i(x / s, y / s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator % (Value s) const
{
    return Vector2i(x % s, y % s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator & (Value s) const
{
    return Vector2i(x & s, y & s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator | (Value s) const
{
    return Vector2i(x | s, y | s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator ^ (Value s) const
{
    return Vector2i(x ^ s, y ^ s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator << (Value s) const
{
    return Vector2i(x < s, y < s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator >> (Value s) const
{
    return Vector2i(x >> s, y >> s);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator + () const
{
    return Vector2i(std::abs(x), std::abs(y));
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
bool Vector2i::operator != (const Vector2i & v) const
{
    return (x != v.x) || (y != v.y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2i::operator < (const Vector2i & v) const
{
    return (x < v.x) && (y < v.y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2i::operator > (const Vector2i & v) const
{
    return (x > v.x) && (y > v.y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2i::operator <= (const Vector2i & v) const
{
    return (x <= v.x) && (y <= v.y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2i::operator >= (const Vector2i & v) const
{
        return (x >= v.x) && (y >= v.y);
}

// ------------------------------------------------------------------------------------------------
Int32 Vector2i::Cmp(const Vector2i & o) const
{
    if (*this == o)
    {
        return 0;
    }
    else if (*this > o)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
CSStr Vector2i::ToString() const
{
    return ToStrF("%d,%d", x, y);
}

// ------------------------------------------------------------------------------------------------
void Vector2i::SetScalar(Value ns)
{
    x = ns;
    y = ns;
}

// ------------------------------------------------------------------------------------------------
void Vector2i::SetVector2i(const Vector2i & v)
{
    x = v.x;
    y = v.y;
}

// ------------------------------------------------------------------------------------------------
void Vector2i::SetVector2iEx(Value nx, Value ny)
{
    x = nx;
    y = ny;
}

// ------------------------------------------------------------------------------------------------
void Vector2i::SetVector2(const Vector2 & v)
{
    x = ConvTo< Value >::From(v.x);
    y = ConvTo< Value >::From(v.y);
}

// ------------------------------------------------------------------------------------------------
void Vector2i::SetStr(SQChar delim, const StackStrF & values)
{
    SetVector2i(Vector2i::GetEx(delim, values));
}

// ------------------------------------------------------------------------------------------------
void Vector2i::Generate()
{
    x = GetRandomInt32();
    y = GetRandomInt32();
}

// ------------------------------------------------------------------------------------------------
void Vector2i::Generate(Value min, Value max)
{
    if (max < min)
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomInt32(min, max);
    y = GetRandomInt32(min, max);
}

// ------------------------------------------------------------------------------------------------
void Vector2i::Generate(Value xmin, Value xmax, Value ymin, Value ymax)
{
    if (xmax < xmin || ymax < ymin)
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomInt32(ymin, ymax);
    y = GetRandomInt32(xmin, xmax);
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::Abs() const
{
    return Vector2i(std::abs(x), std::abs(y));
}

// ------------------------------------------------------------------------------------------------
const Vector2i & Vector2i::Get(const StackStrF & str)
{
    return Vector2i::GetEx(Vector2i::Delim, str);
}

// ------------------------------------------------------------------------------------------------
const Vector2i & Vector2i::GetEx(SQChar delim, const StackStrF & str)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %d , %d ");
    static Vector2i vec;
    // Clear previous values, if any
    vec.Clear();
    // Is the specified string empty?
    if (str.mLen <= 0)
    {
        return vec; // Return the value as is!
    }
    // Assign the specified delimiter
    fs[4] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str.mPtr, &fs[0], &vec.x, &vec.y);
    // Return the resulted value
    return vec;
}

// ------------------------------------------------------------------------------------------------
const Vector2i & GetVector2i()
{
    static Vector2i vec;
    vec.Clear();
    return vec;
}

// ------------------------------------------------------------------------------------------------
const Vector2i & GetVector2i(Int32 sv)
{
    static Vector2i vec;
    vec.SetScalar(sv);
    return vec;
}

// ------------------------------------------------------------------------------------------------
const Vector2i & GetVector2i(Int32 xv, Int32 yv)
{
    static Vector2i vec;
    vec.SetVector2iEx(xv, yv);
    return vec;
}

// ------------------------------------------------------------------------------------------------
const Vector2i & GetVector2i(const Vector2i & o)
{
    static Vector2i vec;
    vec.SetVector2i(o);
    return vec;
}

// ================================================================================================
void Register_Vector2i(HSQUIRRELVM vm)
{
    typedef Vector2i::Value Val;

    RootTable(vm).Bind(Typename::Str,
        Class< Vector2i >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val >()
        // Member Variables
        .Var(_SC("x"), &Vector2i::x)
        .Var(_SC("y"), &Vector2i::y)
        .Var(_SC("X"), &Vector2i::x)
        .Var(_SC("Y"), &Vector2i::y)
        // Core Meta-methods
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< Vector2i >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2i >)
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Vector2i::ToString)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< Vector2i >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2i >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< Vector2i >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2i >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< Vector2i >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2i >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< Vector2i >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2i >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< Vector2i >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2i >)
        .Func< Vector2i (Vector2i::*)(void) const >(_SC("_unm"), &Vector2i::operator -)
        // Properties
        .Prop(_SC("Abs"), &Vector2i::Abs)
        // Member Methods
        .Func(_SC("SetScalar"), &Vector2i::SetScalar)
        .Func(_SC("SetVector2i"), &Vector2i::SetVector2i)
        .Func(_SC("SetVector2iEx"), &Vector2i::SetVector2iEx)
        .Func(_SC("SetVector2"), &Vector2i::SetVector2)
        .FmtFunc(_SC("SetStr"), &Vector2i::SetStr)
        .Func(_SC("Clear"), &Vector2i::Clear)
        // Member Overloads
        .Overload< void (Vector2i::*)(void) >(_SC("Generate"), &Vector2i::Generate)
        .Overload< void (Vector2i::*)(Val, Val) >(_SC("Generate"), &Vector2i::Generate)
        .Overload< void (Vector2i::*)(Val, Val, Val, Val) >(_SC("Generate"), &Vector2i::Generate)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Vector2i >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Vector2i >)
        .StaticFmtFunc(_SC("FromStr"), &Vector2i::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Vector2i::GetEx)
        // Operator Exposure
        .Func< Vector2i & (Vector2i::*)(const Vector2i &) >(_SC("opAddAssign"), &Vector2i::operator +=)
        .Func< Vector2i & (Vector2i::*)(const Vector2i &) >(_SC("opSubAssign"), &Vector2i::operator -=)
        .Func< Vector2i & (Vector2i::*)(const Vector2i &) >(_SC("opMulAssign"), &Vector2i::operator *=)
        .Func< Vector2i & (Vector2i::*)(const Vector2i &) >(_SC("opDivAssign"), &Vector2i::operator /=)
        .Func< Vector2i & (Vector2i::*)(const Vector2i &) >(_SC("opModAssign"), &Vector2i::operator %=)
        .Func< Vector2i & (Vector2i::*)(const Vector2i &) >(_SC("opAndAssign"), &Vector2i::operator &=)
        .Func< Vector2i & (Vector2i::*)(const Vector2i &) >(_SC("opOrAssign"), &Vector2i::operator |=)
        .Func< Vector2i & (Vector2i::*)(const Vector2i &) >(_SC("opXorAssign"), &Vector2i::operator ^=)
        .Func< Vector2i & (Vector2i::*)(const Vector2i &) >(_SC("opShlAssign"), &Vector2i::operator <<=)
        .Func< Vector2i & (Vector2i::*)(const Vector2i &) >(_SC("opShrAssign"), &Vector2i::operator >>=)
        .Func< Vector2i & (Vector2i::*)(Vector2i::Value) >(_SC("opAddAssignS"), &Vector2i::operator +=)
        .Func< Vector2i & (Vector2i::*)(Vector2i::Value) >(_SC("opSubAssignS"), &Vector2i::operator -=)
        .Func< Vector2i & (Vector2i::*)(Vector2i::Value) >(_SC("opMulAssignS"), &Vector2i::operator *=)
        .Func< Vector2i & (Vector2i::*)(Vector2i::Value) >(_SC("opDivAssignS"), &Vector2i::operator /=)
        .Func< Vector2i & (Vector2i::*)(Vector2i::Value) >(_SC("opModAssignS"), &Vector2i::operator %=)
        .Func< Vector2i & (Vector2i::*)(Vector2i::Value) >(_SC("opAndAssignS"), &Vector2i::operator &=)
        .Func< Vector2i & (Vector2i::*)(Vector2i::Value) >(_SC("opOrAssignS"), &Vector2i::operator |=)
        .Func< Vector2i & (Vector2i::*)(Vector2i::Value) >(_SC("opXorAssignS"), &Vector2i::operator ^=)
        .Func< Vector2i & (Vector2i::*)(Vector2i::Value) >(_SC("opShlAssignS"), &Vector2i::operator <<=)
        .Func< Vector2i & (Vector2i::*)(Vector2i::Value) >(_SC("opShrAssignS"), &Vector2i::operator >>=)
        .Func< Vector2i & (Vector2i::*)(void) >(_SC("opPreInc"), &Vector2i::operator ++)
        .Func< Vector2i & (Vector2i::*)(void) >(_SC("opPreDec"), &Vector2i::operator --)
        .Func< Vector2i (Vector2i::*)(int) >(_SC("opPostInc"), &Vector2i::operator ++)
        .Func< Vector2i (Vector2i::*)(int) >(_SC("opPostDec"), &Vector2i::operator --)
        .Func< Vector2i (Vector2i::*)(const Vector2i &) const >(_SC("opAdd"), &Vector2i::operator +)
        .Func< Vector2i (Vector2i::*)(const Vector2i &) const >(_SC("opSub"), &Vector2i::operator -)
        .Func< Vector2i (Vector2i::*)(const Vector2i &) const >(_SC("opMul"), &Vector2i::operator *)
        .Func< Vector2i (Vector2i::*)(const Vector2i &) const >(_SC("opDiv"), &Vector2i::operator /)
        .Func< Vector2i (Vector2i::*)(const Vector2i &) const >(_SC("opMod"), &Vector2i::operator %)
        .Func< Vector2i (Vector2i::*)(const Vector2i &) const >(_SC("opAnd"), &Vector2i::operator &)
        .Func< Vector2i (Vector2i::*)(const Vector2i &) const >(_SC("opOr"), &Vector2i::operator |)
        .Func< Vector2i (Vector2i::*)(const Vector2i &) const >(_SC("opShl"), &Vector2i::operator ^)
        .Func< Vector2i (Vector2i::*)(const Vector2i &) const >(_SC("opShl"), &Vector2i::operator <<)
        .Func< Vector2i (Vector2i::*)(const Vector2i &) const >(_SC("opShr"), &Vector2i::operator >>)
        .Func< Vector2i (Vector2i::*)(Vector2i::Value) const >(_SC("opAddS"), &Vector2i::operator +)
        .Func< Vector2i (Vector2i::*)(Vector2i::Value) const >(_SC("opSubS"), &Vector2i::operator -)
        .Func< Vector2i (Vector2i::*)(Vector2i::Value) const >(_SC("opMulS"), &Vector2i::operator *)
        .Func< Vector2i (Vector2i::*)(Vector2i::Value) const >(_SC("opDivS"), &Vector2i::operator /)
        .Func< Vector2i (Vector2i::*)(Vector2i::Value) const >(_SC("opModS"), &Vector2i::operator %)
        .Func< Vector2i (Vector2i::*)(Vector2i::Value) const >(_SC("opAndS"), &Vector2i::operator &)
        .Func< Vector2i (Vector2i::*)(Vector2i::Value) const >(_SC("opOrS"), &Vector2i::operator |)
        .Func< Vector2i (Vector2i::*)(Vector2i::Value) const >(_SC("opShlS"), &Vector2i::operator ^)
        .Func< Vector2i (Vector2i::*)(Vector2i::Value) const >(_SC("opShlS"), &Vector2i::operator <<)
        .Func< Vector2i (Vector2i::*)(Vector2i::Value) const >(_SC("opShrS"), &Vector2i::operator >>)
        .Func< Vector2i (Vector2i::*)(void) const >(_SC("opUnPlus"), &Vector2i::operator +)
        .Func< Vector2i (Vector2i::*)(void) const >(_SC("opUnMinus"), &Vector2i::operator -)
        .Func< Vector2i (Vector2i::*)(void) const >(_SC("opCom"), &Vector2i::operator ~)
        .Func< bool (Vector2i::*)(const Vector2i &) const >(_SC("opEqual"), &Vector2i::operator ==)
        .Func< bool (Vector2i::*)(const Vector2i &) const >(_SC("opNotEqual"), &Vector2i::operator !=)
        .Func< bool (Vector2i::*)(const Vector2i &) const >(_SC("opLessThan"), &Vector2i::operator <)
        .Func< bool (Vector2i::*)(const Vector2i &) const >(_SC("opGreaterThan"), &Vector2i::operator >)
        .Func< bool (Vector2i::*)(const Vector2i &) const >(_SC("opLessEqual"), &Vector2i::operator <=)
        .Func< bool (Vector2i::*)(const Vector2i &) const >(_SC("opGreaterEqual"), &Vector2i::operator >=)
    );
}

} // Namespace:: SqMod
