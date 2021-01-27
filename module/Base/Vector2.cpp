// ------------------------------------------------------------------------------------------------
#include "Base/Vector2.hpp"
#include "Base/Vector2i.hpp"
#include "Base/Shared.hpp"
#include "Base/DynArg.hpp"
#include "Base/Buffer.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("Vector2"))

// ------------------------------------------------------------------------------------------------
const Vector2 Vector2::NIL = Vector2(0);
const Vector2 Vector2::MIN = Vector2(std::numeric_limits< Vector2::Value >::min());
const Vector2 Vector2::MAX = Vector2(std::numeric_limits< Vector2::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Vector2::Delim = ',';

// ------------------------------------------------------------------------------------------------
Vector2::Vector2() noexcept
    : x(0.0), y(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector2::Vector2(Value sv) noexcept
    : x(sv), y(sv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector2::Vector2(Value xv, Value yv) noexcept
    : x(xv), y(yv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator = (Value s)
{
    x = s;
    y = s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator = (const Vector2i & v)
{
    x = ConvTo< Value >::From(v.x);
    y = ConvTo< Value >::From(v.y);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator += (const Vector2 & v)
{
    x += v.x;
    y += v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator -= (const Vector2 & v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator *= (const Vector2 & v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator /= (const Vector2 & v)
{
    x /= v.x;
    y /= v.y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator %= (const Vector2 & v)
{
    x = std::fmod(x, v.x);
    y = std::fmod(y, v.y);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator += (Value s)
{
    x += s;
    y += s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator -= (Value s)
{
    x -= s;
    y -= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator *= (Value s)
{
    x *= s;
    y *= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator /= (Value s)
{
    x /= s;
    y /= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator %= (Value s)
{
    x = std::fmod(x, s);
    y = std::fmod(y, s);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator ++ ()
{
    ++x;
    ++y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 & Vector2::operator -- ()
{
    --x;
    --y;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator ++ (int) // NOLINT(cert-dcl21-cpp)
{
    Vector2 state(*this);
    ++x;
    ++y;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator -- (int) // NOLINT(cert-dcl21-cpp)
{
    Vector2 state(*this);
    --x;
    --y;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator + (const Vector2 & v) const
{
    return {x + v.x, y + v.y};
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator - (const Vector2 & v) const
{
    return {x - v.x, y - v.y};
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator * (const Vector2 & v) const
{
    return {x * v.x, y * v.y};
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator / (const Vector2 & v) const
{
    return {x / v.x, y / v.y};
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator % (const Vector2 & v) const
{
    return {std::fmod(x, v.x), std::fmod(y, v.y)};
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator + (Value s) const
{
    return {x + s, y + s};
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator - (Value s) const
{
    return {x - s, y - s};
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator * (Value s) const
{
    return {x * s, y * s};
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator / (Value s) const
{
    return {x / s, y / s};
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator % (Value s) const
{
    return {std::fmod(x, s), std::fmod(y, s)};
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator + () const
{
    return {std::fabs(x), std::fabs(y)};
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::operator - () const
{
    return {-x, -y};
}

// ------------------------------------------------------------------------------------------------
bool Vector2::operator == (const Vector2 & v) const
{
    return EpsEq(x, v.x) && EpsEq(y, v.y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2::operator != (const Vector2 & v) const
{
    return !EpsEq(x, v.x) || !EpsEq(y, v.y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2::operator < (const Vector2 & v) const
{
    return EpsLt(x, v.x) && EpsLt(y, v.y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2::operator > (const Vector2 & v) const
{
    return EpsGt(x, v.x) && EpsGt(y, v.y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2::operator <= (const Vector2 & v) const
{
    return EpsLtEq(x, v.x) && EpsLtEq(y, v.y);
}

// ------------------------------------------------------------------------------------------------
bool Vector2::operator >= (const Vector2 & v) const
{
    return EpsGtEq(x, v.x) && EpsGtEq(y, v.y);
}

// ------------------------------------------------------------------------------------------------
Int32 Vector2::Cmp(const Vector2 & o) const
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
CSStr Vector2::ToString() const
{
    return ToStrF("%f,%f", x, y);
}

// ------------------------------------------------------------------------------------------------
void Vector2::SetScalar(Value ns)
{
    x = ns;
    y = ns;
}

// ------------------------------------------------------------------------------------------------
void Vector2::SetVector2(const Vector2 & v)
{
    x = v.x;
    y = v.y;
}

// ------------------------------------------------------------------------------------------------
void Vector2::SetVector2Ex(Value nx, Value ny)
{
    x = nx;
    y = ny;
}

// ------------------------------------------------------------------------------------------------
void Vector2::SetVector2i(const Vector2i & v)
{
    x = ConvTo< Value >::From(v.x);
    y = ConvTo< Value >::From(v.y);
}

// ------------------------------------------------------------------------------------------------
void Vector2::SetStr(SQChar delim, StackStrF & values)
{
    SetVector2(Vector2::GetEx(delim, values));
}

// ------------------------------------------------------------------------------------------------
void Vector2::Generate()
{
    x = GetRandomFloat32();
    y = GetRandomFloat32();
}

// ------------------------------------------------------------------------------------------------
void Vector2::Generate(Value min, Value max)
{
    if (EpsLt(max, min))
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomFloat32(min, max);
    y = GetRandomFloat32(min, max);
}

// ------------------------------------------------------------------------------------------------
void Vector2::Generate(Value xmin, Value xmax, Value ymin, Value ymax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin))
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomFloat32(ymin, ymax);
    y = GetRandomFloat32(xmin, xmax);
}

// ------------------------------------------------------------------------------------------------
Vector2 Vector2::Abs() const
{
    return {std::fabs(x), std::fabs(y)};
}

// ------------------------------------------------------------------------------------------------
LightObj Vector2::Format(const String & spec, StackStrF & fmt) const
{
    String out;
    // Attempt to build the format string
    if (!BuildFormatString(out, fmt, 2, spec))
    {
        return LightObj{}; // Default to null
    }
    // Empty string is unacceptable
    else if (out.empty())
    {
        STHROWF("Unable to build a valid format string.");
    }
    // Grab a temporary buffer
    Buffer buff(out.size());
    // Generate the string
    Buffer::SzType n = buff.WriteF(0, out.c_str(), x, y);
    // Did the format failed?
    if (!n && !out.empty())
    {
        STHROWF("Format failed. Please check format specifier and parameter count.");
    }
    // Return the resulted string
    return LightObj{buff.Begin< SQChar >(), static_cast< SQInteger >(n)};
}

// ------------------------------------------------------------------------------------------------
const Vector2 & Vector2::Get(StackStrF & str)
{
    return Vector2::GetEx(Vector2::Delim, str);
}

// ------------------------------------------------------------------------------------------------
const Vector2 & Vector2::GetEx(SQChar delim, StackStrF & str)
{
    static Vector2 vec;
    // Clear previous values, if any
    vec.Clear();
    // Is the specified string empty?
    if (str.mLen <= 0)
    {
        return vec; // Return the value as is!
    }
    // The format specifications that will be used to scan the string
    SQChar fs[] = _SC(" %f , %f ");
    // Assign the specified delimiter
    fs[4] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str.mPtr, fs, &vec.x, &vec.y);
    // Return the resulted value
    return vec;
}

// ================================================================================================
void Register_Vector2(HSQUIRRELVM vm)
{
    typedef Vector2::Value Val;

    RootTable(vm).Bind(Typename::Str,
        Class< Vector2 >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val >()
        // Member Variables
        .Var(_SC("x"), &Vector2::x)
        .Var(_SC("y"), &Vector2::y)
        .Var(_SC("X"), &Vector2::x)
        .Var(_SC("Y"), &Vector2::y)
        // Core Meta-methods
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< Vector2 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2 >)
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Vector2::ToString)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< Vector2 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2 >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< Vector2 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2 >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< Vector2 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2 >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< Vector2 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2 >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< Vector2 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector2 >)
        .Func< Vector2 (Vector2::*)(void) const >(_SC("_unm"), &Vector2::operator -)
        // Properties
        .Prop(_SC("Abs"), &Vector2::Abs)
        // Member Methods
        .Func(_SC("SetScalar"), &Vector2::SetScalar)
        .Func(_SC("SetVector2"), &Vector2::SetVector2)
        .Func(_SC("SetVector2Ex"), &Vector2::SetVector2Ex)
        .Func(_SC("SetVector2i"), &Vector2::SetVector2i)
        .FmtFunc(_SC("SetStr"), &Vector2::SetStr)
        .Func(_SC("Clear"), &Vector2::Clear)
        .FmtFunc(_SC("Format"), &Vector2::Format)
        // Member Overloads
        .Overload< void (Vector2::*)(void) >(_SC("Generate"), &Vector2::Generate)
        .Overload< void (Vector2::*)(Val, Val) >(_SC("Generate"), &Vector2::Generate)
        .Overload< void (Vector2::*)(Val, Val, Val, Val) >(_SC("Generate"), &Vector2::Generate)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Vector2 >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Vector2 >)
        .StaticFmtFunc(_SC("FromStr"), &Vector2::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Vector2::GetEx)
        // Operator Exposure
        .Func< Vector2 & (Vector2::*)(const Vector2 &) >(_SC("opAddAssign"), &Vector2::operator +=)
        .Func< Vector2 & (Vector2::*)(const Vector2 &) >(_SC("opSubAssign"), &Vector2::operator -=)
        .Func< Vector2 & (Vector2::*)(const Vector2 &) >(_SC("opMulAssign"), &Vector2::operator *=)
        .Func< Vector2 & (Vector2::*)(const Vector2 &) >(_SC("opDivAssign"), &Vector2::operator /=)
        .Func< Vector2 & (Vector2::*)(const Vector2 &) >(_SC("opModAssign"), &Vector2::operator %=)
        .Func< Vector2 & (Vector2::*)(Vector2::Value) >(_SC("opAddAssignS"), &Vector2::operator +=)
        .Func< Vector2 & (Vector2::*)(Vector2::Value) >(_SC("opSubAssignS"), &Vector2::operator -=)
        .Func< Vector2 & (Vector2::*)(Vector2::Value) >(_SC("opMulAssignS"), &Vector2::operator *=)
        .Func< Vector2 & (Vector2::*)(Vector2::Value) >(_SC("opDivAssignS"), &Vector2::operator /=)
        .Func< Vector2 & (Vector2::*)(Vector2::Value) >(_SC("opModAssignS"), &Vector2::operator %=)
        .Func< Vector2 & (Vector2::*)(void) >(_SC("opPreInc"), &Vector2::operator ++)
        .Func< Vector2 & (Vector2::*)(void) >(_SC("opPreDec"), &Vector2::operator --)
        .Func< Vector2 (Vector2::*)(int) >(_SC("opPostInc"), &Vector2::operator ++)
        .Func< Vector2 (Vector2::*)(int) >(_SC("opPostDec"), &Vector2::operator --)
        .Func< Vector2 (Vector2::*)(const Vector2 &) const >(_SC("opAdd"), &Vector2::operator +)
        .Func< Vector2 (Vector2::*)(const Vector2 &) const >(_SC("opSub"), &Vector2::operator -)
        .Func< Vector2 (Vector2::*)(const Vector2 &) const >(_SC("opMul"), &Vector2::operator *)
        .Func< Vector2 (Vector2::*)(const Vector2 &) const >(_SC("opDiv"), &Vector2::operator /)
        .Func< Vector2 (Vector2::*)(const Vector2 &) const >(_SC("opMod"), &Vector2::operator %)
        .Func< Vector2 (Vector2::*)(Vector2::Value) const >(_SC("opAddS"), &Vector2::operator +)
        .Func< Vector2 (Vector2::*)(Vector2::Value) const >(_SC("opSubS"), &Vector2::operator -)
        .Func< Vector2 (Vector2::*)(Vector2::Value) const >(_SC("opMulS"), &Vector2::operator *)
        .Func< Vector2 (Vector2::*)(Vector2::Value) const >(_SC("opDivS"), &Vector2::operator /)
        .Func< Vector2 (Vector2::*)(Vector2::Value) const >(_SC("opModS"), &Vector2::operator %)
        .Func< Vector2 (Vector2::*)(void) const >(_SC("opUnPlus"), &Vector2::operator +)
        .Func< Vector2 (Vector2::*)(void) const >(_SC("opUnMinus"), &Vector2::operator -)
        .Func< bool (Vector2::*)(const Vector2 &) const >(_SC("opEqual"), &Vector2::operator ==)
        .Func< bool (Vector2::*)(const Vector2 &) const >(_SC("opNotEqual"), &Vector2::operator !=)
        .Func< bool (Vector2::*)(const Vector2 &) const >(_SC("opLessThan"), &Vector2::operator <)
        .Func< bool (Vector2::*)(const Vector2 &) const >(_SC("opGreaterThan"), &Vector2::operator >)
        .Func< bool (Vector2::*)(const Vector2 &) const >(_SC("opLessEqual"), &Vector2::operator <=)
        .Func< bool (Vector2::*)(const Vector2 &) const >(_SC("opGreaterEqual"), &Vector2::operator >=)
    );
}

} // Namespace:: SqMod
