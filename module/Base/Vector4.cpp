// ------------------------------------------------------------------------------------------------
#include "Base/Vector4.hpp"
#include "Base/Vector3.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Shared.hpp"
#include "Base/DynArg.hpp"
#include "Base/Buffer.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("Vector4"))

// ------------------------------------------------------------------------------------------------
const Vector4 Vector4::NIL = Vector4(0);
const Vector4 Vector4::MIN = Vector4(std::numeric_limits< Vector4::Value >::min());
const Vector4 Vector4::MAX = Vector4(std::numeric_limits< Vector4::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Vector4::Delim = ',';

// ------------------------------------------------------------------------------------------------
Vector4::Vector4() noexcept
    : x(0.0), y(0.0), z(0.0), w(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector4::Vector4(Value sv) noexcept
    : x(sv), y(sv), z(sv), w(sv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector4::Vector4(Value xv, Value yv, Value zv) noexcept
    : x(xv), y(yv), z(zv), w(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector4::Vector4(Value xv, Value yv, Value zv, Value wv) noexcept
    : x(xv), y(yv), z(zv), w(wv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator = (Value s)
{
    x = s;
    y = s;
    z = s;
    w = s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator = (const Vector3 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 0.0;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator = (const Quaternion & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator += (const Vector4 & v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator -= (const Vector4 & v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator *= (const Vector4 & v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator /= (const Vector4 & v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator %= (const Vector4 & v)
{
    x = std::fmod(x, v.x);
    y = std::fmod(y, v.y);
    z = std::fmod(z, v.z);
    w = std::fmod(w, v.w);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator += (Value s)
{
    x += s;
    y += s;
    z += s;
    w += s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator -= (Value s)
{
    x -= s;
    y -= s;
    z -= s;
    w -= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator *= (Value s)
{
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}

Vector4 & Vector4::operator /= (Value s)
{
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator %= (Value s)
{
    x = std::fmod(x, s);
    y = std::fmod(y, s);
    z = std::fmod(z, s);
    w = std::fmod(w, s);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator ++ ()
{
    ++x;
    ++y;
    ++z;
    ++w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator -- ()
{
    --x;
    --y;
    --z;
    --w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator ++ (int) // NOLINT(cert-dcl21-cpp)
{
    Vector4 state(*this);
    ++x;
    ++y;
    ++z;
    ++w;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator -- (int) // NOLINT(cert-dcl21-cpp)
{
    Vector4 state(*this);
    --x;
    --y;
    --z;
    --w;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator + (const Vector4 & v) const
{
    return {x + v.x, y + v.y, z + v.z, w + v.w};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator - (const Vector4 & v) const
{
    return {x - v.x, y - v.y, z - v.z, w - v.w};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator * (const Vector4 & v) const
{
    return {x * v.x, y * v.y, z * v.z, w * v.w};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator / (const Vector4 & v) const
{
    return {x / v.x, y / v.y, z / v.z, w / v.w};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator % (const Vector4 & v) const
{
    return {std::fmod(x, v.x), std::fmod(y, v.y), std::fmod(z, v.z), std::fmod(w, v.w)};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator + (Value s) const
{
    return {x + s, y + s, z + s, w + s};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator - (Value s) const
{
    return {x - s, y - s, z - s, w - s};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator * (Value s) const
{
    return {x * s, y * s, z * s, w * s};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator / (Value s) const
{
    return {x / s, y / s, z / s, w / s};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator % (Value s) const
{
    return {std::fmod(x, s), std::fmod(y, s), std::fmod(z, s), std::fmod(w, s)};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator + () const
{
    return {std::fabs(x), std::fabs(y), std::fabs(z), std::fabs(w)};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator - () const
{
    return {-x, -y, -z, -w};
}

// ------------------------------------------------------------------------------------------------
bool Vector4::operator == (const Vector4 & v) const
{
    return EpsEq(x, v.x) && EpsEq(y, v.y) && EpsEq(z, v.z) && EpsEq(w, v.w);
}

// ------------------------------------------------------------------------------------------------
bool Vector4::operator != (const Vector4 & v) const
{
    return !EpsEq(x, v.x) || !EpsEq(y, v.y) || !EpsEq(z, v.z) || !EpsEq(w, v.w);
}

// ------------------------------------------------------------------------------------------------
bool Vector4::operator < (const Vector4 & v) const
{
    return EpsLt(x, v.x) && EpsLt(y, v.y) && EpsLt(z, v.z) && EpsLt(w, v.w);
}

// ------------------------------------------------------------------------------------------------
bool Vector4::operator > (const Vector4 & v) const
{
    return EpsGt(x, v.x) && EpsGt(y, v.y) && EpsGt(z, v.z) && EpsGt(w, v.w);
}

// ------------------------------------------------------------------------------------------------
bool Vector4::operator <= (const Vector4 & v) const
{
    return EpsLtEq(x, v.x) && EpsLtEq(y, v.y) && EpsLtEq(z, v.z) && EpsLtEq(w, v.w);
}

// ------------------------------------------------------------------------------------------------
bool Vector4::operator >= (const Vector4 & v) const
{
    return EpsGtEq(x, v.x) && EpsGtEq(y, v.y) && EpsGtEq(z, v.z) && EpsGtEq(w, v.w);
}

// ------------------------------------------------------------------------------------------------
Int32 Vector4::Cmp(const Vector4 & o) const
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
CSStr Vector4::ToString() const
{
    return ToStrF("%f,%f,%f,%f", x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
void Vector4::SetScalar(Value ns)
{
    x = ns;
    y = ns;
    z = ns;
    w = ns;
}

// ------------------------------------------------------------------------------------------------
void Vector4::SetVector4(const Vector4 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

// ------------------------------------------------------------------------------------------------
void Vector4::SetVector4Ex(Value nx, Value ny, Value nz, Value nw)
{
    x = nx;
    y = ny;
    z = nz;
    w = nw;
}

// ------------------------------------------------------------------------------------------------
void Vector4::SetVector3(const Vector3 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 0.0;
}

// ------------------------------------------------------------------------------------------------
void Vector4::SetVector3Ex(Value nx, Value ny, Value nz)
{
    x = nx;
    y = ny;
    z = nz;
}

// ------------------------------------------------------------------------------------------------
void Vector4::SetQuaternion(const Quaternion & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
}

// ------------------------------------------------------------------------------------------------
void Vector4::SetQuaternionEx(Value nx, Value ny, Value nz, Value nw)
{
    x = nx;
    y = ny;
    z = nz;
    w = nw;
}

// ------------------------------------------------------------------------------------------------
void Vector4::SetStr(SQChar delim, StackStrF & values)
{
    SetVector4(Vector4::GetEx(delim, values));
}

// ------------------------------------------------------------------------------------------------
void Vector4::Generate()
{
    x = GetRandomFloat32();
    y = GetRandomFloat32();
    z = GetRandomFloat32();
    w = GetRandomFloat32();
}

// ------------------------------------------------------------------------------------------------
void Vector4::Generate(Value min, Value max)
{
    if (max < min)
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomFloat32(min, max);
    y = GetRandomFloat32(min, max);
    z = GetRandomFloat32(min, max);
    y = GetRandomFloat32(min, max);
}

// ------------------------------------------------------------------------------------------------
void Vector4::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value wmin, Value wmax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin) || EpsLt(zmax, zmin) || EpsLt(wmax, wmin))
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomFloat32(xmin, xmax);
    y = GetRandomFloat32(ymin, ymax);
    z = GetRandomFloat32(zmin, zmax);
    y = GetRandomFloat32(ymin, ymax);
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::Abs() const
{
    return {std::fabs(x), std::fabs(y), std::fabs(z), std::fabs(w)};
}

// ------------------------------------------------------------------------------------------------
const Vector4 & Vector4::Get(StackStrF & str)
{
    return Vector4::GetEx(Vector4::Delim, str);
}

// ------------------------------------------------------------------------------------------------
LightObj Vector4::Format(const String & spec, StackStrF & fmt) const
{
    String out;
    // Attempt to build the format string
    if (!BuildFormatString(out, fmt, 4, spec))
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
    Buffer::SzType n = buff.WriteF(0, out.c_str(), x, y, z, w);
    // Did the format failed?
    if (!n && !out.empty())
    {
        STHROWF("Format failed. Please check format specifier and parameter count.");
    }
    // Return the resulted string
    return LightObj{buff.Begin< SQChar >(), static_cast< SQInteger >(n)};
}

// ------------------------------------------------------------------------------------------------
const Vector4 & Vector4::GetEx(SQChar delim, StackStrF & str)
{
    static Vector4 vec;
    // Clear previous values, if any
    vec.Clear();
    // Is the specified string empty?
    if (str.mLen <= 0)
    {
        return vec; // Return the value as is!
    }
    // The format specifications that will be used to scan the string
    SQChar fs[] = _SC(" %f , %f , %f , %f ");
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    fs[14] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str.mPtr, &fs[0], &vec.x, &vec.y, &vec.z, &vec.w);
    // Return the resulted value
    return vec;
}

// ================================================================================================
void Register_Vector4(HSQUIRRELVM vm)
{
    typedef Vector4::Value Val;

    RootTable(vm).Bind(Typename::Str,
        Class< Vector4 >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val, Val >()
        .Ctor< Val, Val, Val, Val >()
        // Member Variables
        .Var(_SC("x"), &Vector4::x)
        .Var(_SC("y"), &Vector4::y)
        .Var(_SC("z"), &Vector4::z)
        .Var(_SC("w"), &Vector4::w)
        .Var(_SC("X"), &Vector4::x)
        .Var(_SC("Y"), &Vector4::y)
        .Var(_SC("Z"), &Vector4::z)
        .Var(_SC("W"), &Vector4::w)
        // Core Meta-methods
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< Vector4 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector4 >)
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Vector4::ToString)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< Vector4 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector4 >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< Vector4 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector4 >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< Vector4 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector4 >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< Vector4 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector4 >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< Vector4 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector4 >)
        .Func< Vector4 (Vector4::*)(void) const >(_SC("_unm"), &Vector4::operator -)
        // Properties
        .Prop(_SC("Abs"), &Vector4::Abs)
        // Member Methods
        .Func(_SC("SetScalar"), &Vector4::SetScalar)
        .Func(_SC("SetVector4"), &Vector4::SetVector4)
        .Func(_SC("SetVector4Ex"), &Vector4::SetVector4Ex)
        .Func(_SC("SetVector3"), &Vector4::SetVector3)
        .Func(_SC("SetVector3Ex"), &Vector4::SetVector3Ex)
        .Func(_SC("SetQuaternion"), &Vector4::SetQuaternion)
        .Func(_SC("SetQuaternionEx"), &Vector4::SetQuaternionEx)
        .FmtFunc(_SC("SetStr"), &Vector4::SetStr)
        .Func(_SC("Clear"), &Vector4::Clear)
        .FmtFunc(_SC("Format"), &Vector4::Format)
        // Member Overloads
        .Overload< void (Vector4::*)(void) >(_SC("Generate"), &Vector4::Generate)
        .Overload< void (Vector4::*)(Val, Val) >(_SC("Generate"), &Vector4::Generate)
        .Overload< void (Vector4::*)(Val, Val, Val, Val, Val, Val, Val, Val) >(_SC("Generate"), &Vector4::Generate)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Vector4 >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Vector4 >)
        .StaticFmtFunc(_SC("FromStr"), &Vector4::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Vector4::GetEx)
        // Operator Exposure
        .Func< Vector4 & (Vector4::*)(const Vector4 &) >(_SC("opAddAssign"), &Vector4::operator +=)
        .Func< Vector4 & (Vector4::*)(const Vector4 &) >(_SC("opSubAssign"), &Vector4::operator -=)
        .Func< Vector4 & (Vector4::*)(const Vector4 &) >(_SC("opMulAssign"), &Vector4::operator *=)
        .Func< Vector4 & (Vector4::*)(const Vector4 &) >(_SC("opDivAssign"), &Vector4::operator /=)
        .Func< Vector4 & (Vector4::*)(const Vector4 &) >(_SC("opModAssign"), &Vector4::operator %=)
        .Func< Vector4 & (Vector4::*)(Vector4::Value) >(_SC("opAddAssignS"), &Vector4::operator +=)
        .Func< Vector4 & (Vector4::*)(Vector4::Value) >(_SC("opSubAssignS"), &Vector4::operator -=)
        .Func< Vector4 & (Vector4::*)(Vector4::Value) >(_SC("opMulAssignS"), &Vector4::operator *=)
        .Func< Vector4 & (Vector4::*)(Vector4::Value) >(_SC("opDivAssignS"), &Vector4::operator /=)
        .Func< Vector4 & (Vector4::*)(Vector4::Value) >(_SC("opModAssignS"), &Vector4::operator %=)
        .Func< Vector4 & (Vector4::*)(void) >(_SC("opPreInc"), &Vector4::operator ++)
        .Func< Vector4 & (Vector4::*)(void) >(_SC("opPreDec"), &Vector4::operator --)
        .Func< Vector4 (Vector4::*)(int) >(_SC("opPostInc"), &Vector4::operator ++)
        .Func< Vector4 (Vector4::*)(int) >(_SC("opPostDec"), &Vector4::operator --)
        .Func< Vector4 (Vector4::*)(const Vector4 &) const >(_SC("opAdd"), &Vector4::operator +)
        .Func< Vector4 (Vector4::*)(const Vector4 &) const >(_SC("opSub"), &Vector4::operator -)
        .Func< Vector4 (Vector4::*)(const Vector4 &) const >(_SC("opMul"), &Vector4::operator *)
        .Func< Vector4 (Vector4::*)(const Vector4 &) const >(_SC("opDiv"), &Vector4::operator /)
        .Func< Vector4 (Vector4::*)(const Vector4 &) const >(_SC("opMod"), &Vector4::operator %)
        .Func< Vector4 (Vector4::*)(Vector4::Value) const >(_SC("opAddS"), &Vector4::operator +)
        .Func< Vector4 (Vector4::*)(Vector4::Value) const >(_SC("opSubS"), &Vector4::operator -)
        .Func< Vector4 (Vector4::*)(Vector4::Value) const >(_SC("opMulS"), &Vector4::operator *)
        .Func< Vector4 (Vector4::*)(Vector4::Value) const >(_SC("opDivS"), &Vector4::operator /)
        .Func< Vector4 (Vector4::*)(Vector4::Value) const >(_SC("opModS"), &Vector4::operator %)
        .Func< Vector4 (Vector4::*)(void) const >(_SC("opUnPlus"), &Vector4::operator +)
        .Func< Vector4 (Vector4::*)(void) const >(_SC("opUnMinus"), &Vector4::operator -)
        .Func< bool (Vector4::*)(const Vector4 &) const >(_SC("opEqual"), &Vector4::operator ==)
        .Func< bool (Vector4::*)(const Vector4 &) const >(_SC("opNotEqual"), &Vector4::operator !=)
        .Func< bool (Vector4::*)(const Vector4 &) const >(_SC("opLessThan"), &Vector4::operator <)
        .Func< bool (Vector4::*)(const Vector4 &) const >(_SC("opGreaterThan"), &Vector4::operator >)
        .Func< bool (Vector4::*)(const Vector4 &) const >(_SC("opLessEqual"), &Vector4::operator <=)
        .Func< bool (Vector4::*)(const Vector4 &) const >(_SC("opGreaterEqual"), &Vector4::operator >=)
    );
}

} // Namespace:: SqMod
