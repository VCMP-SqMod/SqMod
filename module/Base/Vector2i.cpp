// ------------------------------------------------------------------------------------------------
#include "Base/Vector2i.hpp"
#include "Base/Vector2.hpp"
#include "Base/DynArg.hpp"
#include "Core/Buffer.hpp"
#include "Core/Utility.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("Vector2i"))

// ------------------------------------------------------------------------------------------------
const Vector2i Vector2i::NIL = Vector2i(0);
const Vector2i Vector2i::MIN = Vector2i(std::numeric_limits< Vector2i::Value >::min());
const Vector2i Vector2i::MAX = Vector2i(std::numeric_limits< Vector2i::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Vector2i::Delim = ',';

// ------------------------------------------------------------------------------------------------
Vector2i::Vector2i(Value sv) noexcept
    : x(sv), y(sv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector2i::Vector2i(Value xv, Value yv) noexcept
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
    x &= v.x; // NOLINT(hicpp-signed-bitwise)
    y &= v.y; // NOLINT(hicpp-signed-bitwise)
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator |= (const Vector2i & v)
{
    x |= v.x; // NOLINT(hicpp-signed-bitwise)
    y |= v.y; // NOLINT(hicpp-signed-bitwise)
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator ^= (const Vector2i & v)
{
    x ^= v.x; // NOLINT(hicpp-signed-bitwise)
    y ^= v.y; // NOLINT(hicpp-signed-bitwise)
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator <<= (const Vector2i & v)
{
    x <<= v.x; // NOLINT(hicpp-signed-bitwise)
    y <<= v.y; // NOLINT(hicpp-signed-bitwise)
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator >>= (const Vector2i & v)
{
    x >>= v.x; // NOLINT(hicpp-signed-bitwise)
    y >>= v.y; // NOLINT(hicpp-signed-bitwise)
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
    x &= s; // NOLINT(hicpp-signed-bitwise)
    y &= s; // NOLINT(hicpp-signed-bitwise)
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator |= (Value s)
{
    x |= s; // NOLINT(hicpp-signed-bitwise)
    y |= s; // NOLINT(hicpp-signed-bitwise)
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
    x <<= s; // NOLINT(hicpp-signed-bitwise)
    y <<= s; // NOLINT(hicpp-signed-bitwise)
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::operator >>= (Value s)
{
    x >>= s; // NOLINT(hicpp-signed-bitwise)
    y >>= s; // NOLINT(hicpp-signed-bitwise)
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
Vector2i Vector2i::operator ++ (int) // NOLINT(cert-dcl21-cpp)
{
    Vector2i state(*this);
    ++x;
    ++y;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator -- (int) // NOLINT(cert-dcl21-cpp)
{
    Vector2i state(*this);
    --x;
    --y;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator + (const Vector2i & v) const
{
    return {x + v.x, y + v.y};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator - (const Vector2i & v) const
{
    return {x - v.x, y - v.y};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator * (const Vector2i & v) const
{
    return {x * v.x, y * v.y};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator / (const Vector2i & v) const
{
    return {x / v.x, y / v.y};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator % (const Vector2i & v) const
{
    return {x % v.x, y % v.y};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator & (const Vector2i & v) const
{
    return {x & v.x, y & v.y}; // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator | (const Vector2i & v) const
{
    return {x | v.x, y | v.y}; // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator ^ (const Vector2i & v) const
{
    return {x ^ v.x, y ^ v.y}; // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator << (const Vector2i & v) const
{
    return {x << v.x, y << v.y}; // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator >> (const Vector2i & v) const
{
    return {x >> v.x, y >> v.y}; // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator + (Value s) const
{
    return {x + s, y + s};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator - (Value s) const
{
    return {x - s, y - s};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator * (Value s) const
{
    return {x * s, y * s};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator / (Value s) const
{
    return {x / s, y / s};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator % (Value s) const
{
    return {x % s, y % s};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator & (Value s) const
{
    return {x & s, y & s}; // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator | (Value s) const
{
    return {x | s, y | s}; // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator ^ (Value s) const
{
    return {x ^ s, y ^ s}; // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator << (Value s) const
{
    return {x << s, y << s}; // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator >> (Value s) const
{
    return {x >> s, y >> s}; // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator + () const
{
    return {std::abs(x), std::abs(y)};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator - () const
{
    return {-x, -y};
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::operator ~ () const
{
    return {~x, ~y}; // NOLINT(hicpp-signed-bitwise)
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
int32_t Vector2i::Cmp(const Vector2i & o) const
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
String Vector2i::ToString() const
{
    return fmt::format("{},{}", x, y);
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
void Vector2i::SetStr(SQChar delim, StackStrF & values)
{
    SetVector2i(Vector2i::GetEx(delim, values));
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::Generate()
{
    x = GetRandomInt32();
    y = GetRandomInt32();
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::GenerateB(Value min, Value max)
{
    if (max < min)
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomInt32(min, max);
    y = GetRandomInt32(min, max);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i & Vector2i::GenerateR(Value xmin, Value xmax, Value ymin, Value ymax)
{
    if (xmax < xmin || ymax < ymin)
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomInt32(ymin, ymax);
    y = GetRandomInt32(xmin, xmax);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2i Vector2i::Abs() const
{
    return {std::abs(x), std::abs(y)};
}

// ------------------------------------------------------------------------------------------------
String Vector2i::Format(StackStrF & str) const
{
    return fmt::format(fmt::runtime(str.ToStr())
        , fmt::arg("x", x)
        , fmt::arg("y", y)
    );
}

// ------------------------------------------------------------------------------------------------
const Vector2i & Vector2i::Get(StackStrF & str)
{
    return Vector2i::GetEx(Vector2i::Delim, str);
}

// ------------------------------------------------------------------------------------------------
const Vector2i & Vector2i::GetEx(SQChar delim, StackStrF & str)
{
    static Vector2i vec;
    // Clear previous values, if any
    vec.Clear();
    // Is the specified string empty?
    if (str.mLen <= 0)
    {
        return vec; // Return the value as is!
    }
    // The format specifications that will be used to scan the string
    SQChar fs[] = _SC(" %d , %d ");
    // Assign the specified delimiter
    fs[4] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str.mPtr, &fs[0], &vec.x, &vec.y);
    // Return the resulted value
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
        .FmtFunc(_SC("Format"), &Vector2i::Format)
        // Member Overloads
        .Overload(_SC("Generate"), &Vector2i::Generate)
        .Overload(_SC("Generate"), &Vector2i::GenerateB)
        .Overload(_SC("Generate"), &Vector2i::GenerateR)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Vector2i >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Vector2i >)
        .StaticFmtFunc(_SC("FromStr"), &Vector2i::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Vector2i::GetEx)
    );
}

} // Namespace:: SqMod
