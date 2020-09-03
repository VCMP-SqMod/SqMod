// ------------------------------------------------------------------------------------------------
#include "Base/Vector3.hpp"
#include "Base/Vector4.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Shared.hpp"
#include "Base/DynArg.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
#define STOVAL(v) static_cast< Vector3::Value >(v)

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("Vector3"))

// ------------------------------------------------------------------------------------------------
const Vector3 Vector3::NIL(STOVAL(0.0));
const Vector3 Vector3::MIN(std::numeric_limits< Vector3::Value >::min());
const Vector3 Vector3::MAX(std::numeric_limits< Vector3::Value >::max());
const Vector3 Vector3::LEFT(STOVAL(-1.0),   STOVAL(0.0),    STOVAL(0.0));
const Vector3 Vector3::RIGHT(STOVAL(1.0),   STOVAL(0.0),    STOVAL(0.0));
const Vector3 Vector3::UP(STOVAL(0.0),      STOVAL(1.0),    STOVAL(0.0));
const Vector3 Vector3::DOWN(STOVAL(0.0),    STOVAL(-1.0),   STOVAL(0.0));
const Vector3 Vector3::FORWARD(STOVAL(0.0), STOVAL(0.0),    STOVAL(1.0));
const Vector3 Vector3::BACK(STOVAL(0.0),    STOVAL(0.0),    STOVAL(-1.0));
const Vector3 Vector3::ONE(STOVAL(1.0),     STOVAL(1.0),    STOVAL(1.0));

// ------------------------------------------------------------------------------------------------
SQChar Vector3::Delim = ',';

// ------------------------------------------------------------------------------------------------
Vector3::Vector3() noexcept
    : x(0.0), y(0.0), z(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector3::Vector3(Value sv) noexcept
    : x(sv), y(sv), z(sv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector3::Vector3(Value xv, Value yv, Value zv) noexcept
    : x(xv), y(yv), z(zv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator = (Value s)
{
    x = s;
    y = s;
    z = s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator = (const Vector4 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator = (const Quaternion & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator += (const Vector3 & v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator -= (const Vector3 & v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator *= (const Vector3 & v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator /= (const Vector3 & v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator %= (const Vector3 & v)
{
    x = std::fmod(x, v.x);
    y = std::fmod(y, v.y);
    z = std::fmod(z, v.z);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator += (Value s)
{
    x += s;
    y += s;
    z += s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator -= (Value s)
{
    x -= s;
    y -= s;
    z -= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator *= (Value s)
{
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator /= (Value s)
{
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator %= (Value s)
{
    x = std::fmod(x, s);
    y = std::fmod(y, s);
    z = std::fmod(z, s);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator ++ ()
{
    ++x;
    ++y;
    ++z;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator -- ()
{
    --x;
    --y;
    --z;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator ++ (int) // NOLINT(cert-dcl21-cpp)
{
    Vector3 state(*this);
    ++x;
    ++y;
    ++z;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator -- (int) // NOLINT(cert-dcl21-cpp)
{
    Vector3 state(*this);
    --x;
    --y;
    --z;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator + (const Vector3 & v) const
{
    return {x + v.x, y + v.y, z + v.z};
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator - (const Vector3 & v) const
{
    return {x - v.x, y - v.y, z - v.z};
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator * (const Vector3 & v) const
{
    return {x * v.x, y * v.y, z * v.z};
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator / (const Vector3 & v) const
{
    return {x / v.x, y / v.y, z / v.z};
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator % (const Vector3 & v) const
{
    return {std::fmod(x, v.x), std::fmod(y, v.y), std::fmod(z, v.z)};
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator + (Value s) const
{
    return {x + s, y + s, z + s};
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator - (Value s) const
{
    return {x - s, y - s, z - s};
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator * (Value s) const
{
    return {x * s, y * s, z * s};
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator / (Value s) const
{
    return {x / s, y / s, z / s};
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator % (Value s) const
{
    return {std::fmod(x, s), std::fmod(y, s), std::fmod(z, s)};
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator + () const
{
    return {std::fabs(x), std::fabs(y), std::fabs(z)};
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator - () const
{
    return {-x, -y, -z};
}

// ------------------------------------------------------------------------------------------------
bool Vector3::operator == (const Vector3 & v) const
{
    return EpsEq(x, v.x) && EpsEq(y, v.y) && EpsEq(z, v.z);
}

// ------------------------------------------------------------------------------------------------
bool Vector3::operator != (const Vector3 & v) const
{
    return !EpsEq(x, v.x) || !EpsEq(y, v.y) || !EpsEq(z, v.z);
}

// ------------------------------------------------------------------------------------------------
bool Vector3::operator < (const Vector3 & v) const
{
    return EpsLt(x, v.x) && EpsLt(y, v.y) && EpsLt(z, v.z);
}

// ------------------------------------------------------------------------------------------------
bool Vector3::operator > (const Vector3 & v) const
{
    return EpsGt(x, v.x) && EpsGt(y, v.y) && EpsGt(z, v.z);
}

// ------------------------------------------------------------------------------------------------
bool Vector3::operator <= (const Vector3 & v) const
{
    return EpsLtEq(x, v.x) && EpsLtEq(y, v.y) && EpsLtEq(z, v.z);
}

// ------------------------------------------------------------------------------------------------
bool Vector3::operator >= (const Vector3 & v) const
{
    return EpsGtEq(x, v.x) && EpsGtEq(y, v.y) && EpsGtEq(z, v.z);
}

// ------------------------------------------------------------------------------------------------
Int32 Vector3::Cmp(const Vector3 & o) const
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
CSStr Vector3::ToString() const
{
    return ToStrF("%f,%f,%f", x, y, z);
}

// ------------------------------------------------------------------------------------------------
void Vector3::SetScalar(Value ns)
{
    x = ns;
    y = ns;
    z = ns;
}

// ------------------------------------------------------------------------------------------------
void Vector3::SetVector3(const Vector3 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

// ------------------------------------------------------------------------------------------------
void Vector3::SetVector3Ex(Value nx, Value ny, Value nz)
{
    x = nx;
    y = ny;
    z = nz;
}

// ------------------------------------------------------------------------------------------------
void Vector3::SetVector4(const Vector4 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

// ------------------------------------------------------------------------------------------------
void Vector3::SetVector4Ex(Value nx, Value ny, Value nz, Value /*nw*/)
{
    x = nx;
    y = ny;
    z = nz;
}

// ------------------------------------------------------------------------------------------------
void Vector3::SetQuaternion(const Quaternion & q)
{
    SetQuaternionEx(q.x, q.y, q.z, q.w);
}

// ------------------------------------------------------------------------------------------------
void Vector3::SetQuaternionEx(Value qx, Value qy, Value qz, Value qw)
{
    // Quick conversion to Euler angles to give tilt to user
    const Value sqx = (qx * qx), sqy = (qy * qy), sqz = (qz * qz), sqw = (qw * qw);

    y = std::asin(STOVAL(2.0) * ((qw * qy) - (qx * qz)));

    if (EpsGt((SQMOD_PI * STOVAL(0.5)) - std::abs(y), STOVAL(1e-10)))
    {
        z = std::atan2(STOVAL(2.0) * ((qx * qy) + (qw * qz)), sqx - sqy - sqz + sqw);
        x = std::atan2(STOVAL(2.0) * ((qw * qx) + (qy * qz)), sqw - sqx - sqy + sqz);
    }
    else
    {
        // Compute heading from local 'down' vector
        z = std::atan2((STOVAL(2.0) * qy * qz) - (STOVAL(2.0) * qx * qw),
                       (STOVAL(2.0) * qx * qz) + (STOVAL(2.0) * qy * qw));
        x = STOVAL(0.0);

        // If facing down, reverse yaw
        if (EpsLt(y, STOVAL(0.0)))
        {
            z -= SQMOD_PI;
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Vector3::SetStr(SQChar delim, StackStrF & values)
{
    SetVector3(Vector3::GetEx(delim, values));
}

// ------------------------------------------------------------------------------------------------
void Vector3::Generate()
{
    x = GetRandomFloat32();
    y = GetRandomFloat32();
    z = GetRandomFloat32();
}

// ------------------------------------------------------------------------------------------------
void Vector3::Generate(Value min, Value max)
{
    if (EpsLt(max, min))
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomFloat32(min, max);
    y = GetRandomFloat32(min, max);
    z = GetRandomFloat32(min, max);
}

// ------------------------------------------------------------------------------------------------
void Vector3::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin) || EpsLt(zmax, zmin))
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomFloat32(xmin, xmax);
    y = GetRandomFloat32(ymin, ymax);
    z = GetRandomFloat32(zmin, zmax);
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::Abs() const
{
    return {std::fabs(x), std::fabs(y), std::fabs(z)};
}

// ------------------------------------------------------------------------------------------------
bool Vector3::IsNaN() const
{
    return std::isnan(x) || std::isnan(y) || std::isnan(z);
}

// ------------------------------------------------------------------------------------------------
Vector3::Value Vector3::GetLength() const
{
    return std::sqrt((x * x) + (y * y) + (z * z));
}

// ------------------------------------------------------------------------------------------------
void Vector3::SetLength(Value length)
{
    Normalize();
    // Assign the specified length
    *this *= length;
}

// ------------------------------------------------------------------------------------------------
Vector3::Value Vector3::GetLengthSquared() const
{
    return ((x * x) + (y * y) + (z * z));
}

// ------------------------------------------------------------------------------------------------
void Vector3::SetLengthSquared(Value length)
{
    Normalize();
    // Assign the specified length
    *this *= std::sqrt(length);
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::Normalized() const
{
    const Value len_squared = GetLengthSquared();

    if (!EpsEq(len_squared, STOVAL(1.0)) && EpsLt(len_squared, STOVAL(0.0)))
    {
        return (*this * (STOVAL(1.0) / std::sqrt(len_squared)));
    }
    else
    {
        return *this;
    }
}

// ------------------------------------------------------------------------------------------------
void Vector3::Normalize()
{
    const Value len_squared = GetLengthSquared();

    if (!EpsEq(len_squared, STOVAL(1.0)) && EpsGt(len_squared, STOVAL(0.0)))
    {
        const Value inv_len = STOVAL(1.0) / std::sqrt(len_squared);
        x *= inv_len;
        y *= inv_len;
        z *= inv_len;
    }
}

// ------------------------------------------------------------------------------------------------
Vector3::Value Vector3::DotProduct(const Vector3 & vec) const
{
    return ((x * vec.x) + (y * vec.y) + (z * vec.z));
}

// ------------------------------------------------------------------------------------------------
Vector3::Value Vector3::AbsDotProduct(const Vector3 & vec) const
{
    return (std::abs(x * vec.x) + std::abs(y * vec.y) + std::abs(z * vec.z));
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::CrossProduct(const Vector3 & vec) const
{
    return {(y * vec.z) - (z * vec.y), (z * vec.x) - (x * vec.z), (x * vec.y) - (y * vec.x)};
}

// ------------------------------------------------------------------------------------------------
Vector3::Value Vector3::Angle(const Vector3 & vec) const
{
    return std::acos(DotProduct(vec) / (GetLength() * vec.GetLength()));
}

// ------------------------------------------------------------------------------------------------
Vector3::Value Vector3::GetDistanceTo(const Vector3 & vec) const
{
    return std::sqrt(std::pow(x - vec.x, 2) + std::pow(y - vec.y, 2) + std::pow(z - vec.z, 2));
}

// ------------------------------------------------------------------------------------------------
Vector3::Value Vector3::GetSquaredDistanceTo(const Vector3 & vec) const
{
    return (std::pow(x - vec.x, 2) + std::pow(y - vec.y, 2) + std::pow(z - vec.z, 2));
}

// ------------------------------------------------------------------------------------------------
bool Vector3::IsBetweenPoints(const Vector3 & begin, const Vector3 & end) const
{
    const Value length = (end - begin).GetLengthSquared();
    return EpsLtEq(GetSquaredDistanceTo(begin), length) && EpsLtEq(GetSquaredDistanceTo(end), length);
}

// ------------------------------------------------------------------------------------------------
void Vector3::Interpolate(const Vector3 & a, const Vector3 & b, Value d)
{
    x = STOVAL(static_cast< Float64 >(b.x) + ((a.x - b.x) * d));
    y = STOVAL(static_cast< Float64 >(b.y) + ((a.y - b.y) * d));
    z = STOVAL(static_cast< Float64 >(b.z) + ((a.z - b.z) * d));
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::Interpolated(const Vector3 & vec, Value d) const
{
    const Float64 inv = 1.0 - d;
    return {
        STOVAL((vec.x * inv) + (x * d)),
        STOVAL((vec.y * inv) + (y * d)),
        STOVAL((vec.z * inv) + (z * d))
    };
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::Rotated(const Vector3 & axis, Value angle) const
{
    const Vector3 o(axis * axis.DotProduct(*this));
    return (o + ((*this - o) * std::cos(angle)) + (axis.CrossProduct(*this) * std::sin(angle)));
}

// ------------------------------------------------------------------------------------------------
void Vector3::CenterRotateXZBy(Value degrees, const Vector3 & center)
{
    degrees *= SQMOD_DEGTORAD;
    const Value cs = std::cos(degrees);
    const Value sn = std::sin(degrees);
    x -= center.x;
    z -= center.z;
    x = static_cast< Value >((x * cs) - (z * sn)) + center.x;
    z = static_cast< Value >((x * sn) + (z * cs)) + center.z;
}

// ------------------------------------------------------------------------------------------------
void Vector3::CenterRotateXYBy(Value degrees, const Vector3 & center)
{
    degrees *= SQMOD_DEGTORAD;
    const Value cs = std::cos(degrees);
    const Value sn = std::sin(degrees);
    x -= center.x;
    y -= center.y;
    x = static_cast< Value >((x * cs) - (y * sn)) + center.x;
    y = static_cast< Value >((x * sn) + (y * cs)) + center.y;
}

// ------------------------------------------------------------------------------------------------
void Vector3::CenterRotateYZBy(Value degrees, const Vector3 & center)
{
    degrees *= SQMOD_DEGTORAD;
    const Value cs = std::cos(degrees);
    const Value sn = std::sin(degrees);
    z -= center.z;
    y -= center.y;
    y = static_cast< Value >((y * cs) - (z * sn)) + center.z;
    z = static_cast< Value >((y * sn) + (z * cs)) + center.y;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & Vector3::Get(StackStrF & str)
{
    return Vector3::GetEx(Vector3::Delim, str);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & Vector3::GetEx(SQChar delim, StackStrF & str)
{
    static Vector3 vec;
    // Clear previous values, if any
    vec.Clear();
    // Is the specified string empty?
    if (str.mLen <= 0)
    {
        return vec; // Return the value as is!
    }
    // The format specifications that will be used to scan the string
    SQChar fs[] = _SC(" %f , %f , %f ");
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str.mPtr, &fs[0], &vec.x, &vec.y, &vec.z);
    // Return the resulted value
    return vec;
}

// ================================================================================================
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void Register_Vector3(HSQUIRRELVM vm)
#pragma clang diagnostic pop
{
    typedef Vector3::Value Val;

    RootTable(vm).Bind(Typename::Str,
        Class< Vector3 >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val, Val >()
        // Static variables
        .SetStaticValue(_SC("NIL"), &Vector3::NIL)
        .SetStaticValue(_SC("MIN"), &Vector3::MIN)
        .SetStaticValue(_SC("MAX"), &Vector3::MAX)
        .SetStaticValue(_SC("LEFT"), &Vector3::LEFT)
        .SetStaticValue(_SC("RIGHT"), &Vector3::RIGHT)
        .SetStaticValue(_SC("UP"), &Vector3::UP)
        .SetStaticValue(_SC("DOWN"), &Vector3::DOWN)
        .SetStaticValue(_SC("FORWARD"), &Vector3::FORWARD)
        .SetStaticValue(_SC("BACK"), &Vector3::BACK)
        .SetStaticValue(_SC("ONE"), &Vector3::ONE)
        // Member Variables
        .Var(_SC("x"), &Vector3::x)
        .Var(_SC("y"), &Vector3::y)
        .Var(_SC("z"), &Vector3::z)
        .Var(_SC("X"), &Vector3::x)
        .Var(_SC("Y"), &Vector3::y)
        .Var(_SC("Z"), &Vector3::z)
        // Core Meta-methods
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< Vector3 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector3 >)
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Vector3::ToString)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< Vector3 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector3 >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< Vector3 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector3 >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< Vector3 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector3 >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< Vector3 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector3 >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< Vector3 >, SQFloat, SQInteger, bool, std::nullptr_t, Vector3 >)
        .Func< Vector3 (Vector3::*)(void) const >(_SC("_unm"), &Vector3::operator -)
        // Properties
        .Prop(_SC("Abs"), &Vector3::Abs)
        .Prop(_SC("NaN"), &Vector3::IsNaN)
        .Prop(_SC("Length"), &Vector3::GetLength, &Vector3::SetLength)
        .Prop(_SC("LengthSq"), &Vector3::GetLengthSquared, &Vector3::SetLengthSquared)
        .Prop(_SC("Normalized"), &Vector3::Normalized)
        // Member Methods
        .Func(_SC("SetScalar"), &Vector3::SetScalar)
        .Func(_SC("SetVector3"), &Vector3::SetVector3)
        .Func(_SC("SetVector3Ex"), &Vector3::SetVector3Ex)
        .Func(_SC("SetVector4"), &Vector3::SetVector4)
        .Func(_SC("SetVector4Ex"), &Vector3::SetVector4Ex)
        .Func(_SC("SetQuaternion"), &Vector3::SetQuaternion)
        .Func(_SC("SetQuaternionEx"), &Vector3::SetQuaternionEx)
        .FmtFunc(_SC("SetStr"), &Vector3::SetStr)
        .Func(_SC("Clear"), &Vector3::Clear)
        .Func(_SC("Normalize"), &Vector3::Normalize)
        .Func(_SC("Dot"), &Vector3::DotProduct)
        .Func(_SC("AbsDot"), &Vector3::AbsDotProduct)
        .Func(_SC("Cross"), &Vector3::CrossProduct)
        .Func(_SC("Angle"), &Vector3::Angle)
        .Func(_SC("DistanceTo"), &Vector3::GetDistanceTo)
        .Func(_SC("SqDistanceTo"), &Vector3::GetSquaredDistanceTo)
        .Func(_SC("IsBetweenPoints"), &Vector3::IsBetweenPoints)
        .Func(_SC("Interpolate"), &Vector3::Interpolate)
        .Func(_SC("Interpolated"), &Vector3::Interpolated)
        .Func(_SC("Rotated"), &Vector3::Rotated)
        .Func(_SC("RotateXZBy"), &Vector3::RotateXZBy)
        .Func(_SC("CenterRotateXZBy"), &Vector3::CenterRotateXZBy)
        .Func(_SC("RotateXYBy"), &Vector3::RotateXYBy)
        .Func(_SC("CenterRotateXYBy"), &Vector3::CenterRotateXYBy)
        .Func(_SC("RotateYZBy"), &Vector3::RotateYZBy)
        .Func(_SC("CenterRotateYZBy"), &Vector3::CenterRotateYZBy)
        // Member Overloads
        .Overload< void (Vector3::*)(void) >(_SC("Generate"), &Vector3::Generate)
        .Overload< void (Vector3::*)(Val, Val) >(_SC("Generate"), &Vector3::Generate)
        .Overload< void (Vector3::*)(Val, Val, Val, Val, Val, Val) >(_SC("Generate"), &Vector3::Generate)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Vector3 >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Vector3 >)
        .StaticFmtFunc(_SC("FromStr"), &Vector3::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Vector3::GetEx)
        // Operator Exposure
        .Func< Vector3 & (Vector3::*)(const Vector3 &) >(_SC("opAddAssign"), &Vector3::operator +=)
        .Func< Vector3 & (Vector3::*)(const Vector3 &) >(_SC("opSubAssign"), &Vector3::operator -=)
        .Func< Vector3 & (Vector3::*)(const Vector3 &) >(_SC("opMulAssign"), &Vector3::operator *=)
        .Func< Vector3 & (Vector3::*)(const Vector3 &) >(_SC("opDivAssign"), &Vector3::operator /=)
        .Func< Vector3 & (Vector3::*)(const Vector3 &) >(_SC("opModAssign"), &Vector3::operator %=)
        .Func< Vector3 & (Vector3::*)(Vector3::Value) >(_SC("opAddAssignS"), &Vector3::operator +=)
        .Func< Vector3 & (Vector3::*)(Vector3::Value) >(_SC("opSubAssignS"), &Vector3::operator -=)
        .Func< Vector3 & (Vector3::*)(Vector3::Value) >(_SC("opMulAssignS"), &Vector3::operator *=)
        .Func< Vector3 & (Vector3::*)(Vector3::Value) >(_SC("opDivAssignS"), &Vector3::operator /=)
        .Func< Vector3 & (Vector3::*)(Vector3::Value) >(_SC("opModAssignS"), &Vector3::operator %=)
        .Func< Vector3 & (Vector3::*)(void) >(_SC("opPreInc"), &Vector3::operator ++)
        .Func< Vector3 & (Vector3::*)(void) >(_SC("opPreDec"), &Vector3::operator --)
        .Func< Vector3 (Vector3::*)(int) >(_SC("opPostInc"), &Vector3::operator ++)
        .Func< Vector3 (Vector3::*)(int) >(_SC("opPostDec"), &Vector3::operator --)
        .Func< Vector3 (Vector3::*)(const Vector3 &) const >(_SC("opAdd"), &Vector3::operator +)
        .Func< Vector3 (Vector3::*)(const Vector3 &) const >(_SC("opSub"), &Vector3::operator -)
        .Func< Vector3 (Vector3::*)(const Vector3 &) const >(_SC("opMul"), &Vector3::operator *)
        .Func< Vector3 (Vector3::*)(const Vector3 &) const >(_SC("opDiv"), &Vector3::operator /)
        .Func< Vector3 (Vector3::*)(const Vector3 &) const >(_SC("opMod"), &Vector3::operator %)
        .Func< Vector3 (Vector3::*)(Vector3::Value) const >(_SC("opAddS"), &Vector3::operator +)
        .Func< Vector3 (Vector3::*)(Vector3::Value) const >(_SC("opSubS"), &Vector3::operator -)
        .Func< Vector3 (Vector3::*)(Vector3::Value) const >(_SC("opMulS"), &Vector3::operator *)
        .Func< Vector3 (Vector3::*)(Vector3::Value) const >(_SC("opDivS"), &Vector3::operator /)
        .Func< Vector3 (Vector3::*)(Vector3::Value) const >(_SC("opModS"), &Vector3::operator %)
        .Func< Vector3 (Vector3::*)(void) const >(_SC("opUnPlus"), &Vector3::operator +)
        .Func< Vector3 (Vector3::*)(void) const >(_SC("opUnMinus"), &Vector3::operator -)
        .Func< bool (Vector3::*)(const Vector3 &) const >(_SC("opEqual"), &Vector3::operator ==)
        .Func< bool (Vector3::*)(const Vector3 &) const >(_SC("opNotEqual"), &Vector3::operator !=)
        .Func< bool (Vector3::*)(const Vector3 &) const >(_SC("opLessThan"), &Vector3::operator <)
        .Func< bool (Vector3::*)(const Vector3 &) const >(_SC("opGreaterThan"), &Vector3::operator >)
        .Func< bool (Vector3::*)(const Vector3 &) const >(_SC("opLessEqual"), &Vector3::operator <=)
        .Func< bool (Vector3::*)(const Vector3 &) const >(_SC("opGreaterEqual"), &Vector3::operator >=)
    );
}

} // Namespace:: SqMod
