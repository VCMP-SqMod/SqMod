// ------------------------------------------------------------------------------------------------
#include "Base/Vector4.hpp"
#include "Base/Vector3.hpp"
#include "Base/Quaternion.hpp"
#include "Base/DynArg.hpp"
#include "Core/Buffer.hpp"
#include "Core/Utility.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("Vector4"))

// ------------------------------------------------------------------------------------------------
const Vector4 Vector4::NIL = Vector4(0);
const Vector4 Vector4::MIN = Vector4(std::numeric_limits< Vector4::Value >::min());
const Vector4 Vector4::MAX = Vector4(std::numeric_limits< Vector4::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Vector4::Delim = ',';

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
    x = fmodf(x, v.x);
    y = fmodf(y, v.y);
    z = fmodf(z, v.z);
    w = fmodf(w, v.w);
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
    x = fmodf(x, s);
    y = fmodf(y, s);
    z = fmodf(z, s);
    w = fmodf(w, s);
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
    return {fmodf(x, v.x), fmodf(y, v.y), fmodf(z, v.z), fmodf(w, v.w)};
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
    return {fmodf(x, s), fmodf(y, s), fmodf(z, s), fmodf(w, s)};
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator + () const
{
    return {fabsf(x), fabsf(y), fabsf(z), fabsf(w)};
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
int32_t Vector4::Cmp(const Vector4 & o) const
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
String Vector4::ToString() const
{
    return fmt::format("{},{},{},{}", x, y, z, w);
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
Vector4 & Vector4::Generate()
{
    x = GetRandomFloat32();
    y = GetRandomFloat32();
    z = GetRandomFloat32();
    w = GetRandomFloat32();
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::GenerateB(Value min, Value max)
{
    if (max < min)
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomFloat32(min, max);
    y = GetRandomFloat32(min, max);
    z = GetRandomFloat32(min, max);
    y = GetRandomFloat32(min, max);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::GenerateR(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value wmin, Value wmax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin) || EpsLt(zmax, zmin) || EpsLt(wmax, wmin))
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomFloat32(xmin, xmax);
    y = GetRandomFloat32(ymin, ymax);
    z = GetRandomFloat32(zmin, zmax);
    y = GetRandomFloat32(ymin, ymax);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::Abs() const
{
    return {fabsf(x), fabsf(y), fabsf(z), fabsf(w)};
}

// ------------------------------------------------------------------------------------------------
const Vector4 & Vector4::Get(StackStrF & str)
{
    return Vector4::GetEx(Vector4::Delim, str);
}

// ------------------------------------------------------------------------------------------------
String Vector4::Format(StackStrF & str) const
{
    return fmt::format(fmt::runtime(str.ToStr())
        , fmt::arg("x", x)
        , fmt::arg("y", y)
        , fmt::arg("z", z)
        , fmt::arg("w", w)
    );
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
        .Overload(_SC("Generate"), &Vector4::Generate)
        .Overload(_SC("Generate"), &Vector4::GenerateB)
        .Overload(_SC("Generate"), &Vector4::GenerateR)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Vector4 >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Vector4 >)
        .StaticFmtFunc(_SC("FromStr"), &Vector4::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Vector4::GetEx)
    );
}

} // Namespace:: SqMod
