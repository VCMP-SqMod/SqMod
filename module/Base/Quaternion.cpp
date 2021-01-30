// ------------------------------------------------------------------------------------------------
#include "Base/Quaternion.hpp"
#include "Base/Vector3.hpp"
#include "Base/Vector4.hpp"
#include "Base/DynArg.hpp"
#include "Core/Buffer.hpp"
#include "Core/Utility.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
#define STOVAL(v) static_cast< Quaternion::Value >(v)

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("Quaternion"))

// ------------------------------------------------------------------------------------------------
const Quaternion Quaternion::NIL(0);
const Quaternion Quaternion::MIN(std::numeric_limits< Quaternion::Value >::min());
const Quaternion Quaternion::MAX(std::numeric_limits< Quaternion::Value >::max());
const Quaternion Quaternion::IDENTITY(1.0, 0.0, 0.0, 0.0);

// ------------------------------------------------------------------------------------------------
SQChar Quaternion::Delim = ',';

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion(Value sv) noexcept
    : x(sv), y(sv), z(sv), w(sv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion(Value xv, Value yv, Value zv) noexcept
    : x(xv), y(yv), z(zv), w(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion(Value xv, Value yv, Value zv, Value wv) noexcept
    : x(xv), y(yv), z(zv), w(wv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator = (Value s)
{
    x = s;
    y = s;
    z = s;
    w = s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator = (const Vector3 & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = 0.0;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator = (const Vector4 & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator += (const Quaternion & q)
{
    x += q.x;
    y += q.y;
    z += q.z;
    w += q.w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator -= (const Quaternion & q)
{
    x -= q.x;
    y -= q.y;
    z -= q.z;
    w -= q.w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator *= (const Quaternion & q)
{
    x *= q.x;
    y *= q.y;
    z *= q.z;
    w *= q.w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator /= (const Quaternion & q)
{
    x /= q.x;
    y /= q.y;
    z /= q.z;
    w /= q.w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator %= (const Quaternion & q)
{
    x = fmodf(x, q.x);
    y = fmodf(y, q.y);
    z = fmodf(z, q.z);
    w = fmodf(w, q.w);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator += (Value s)
{
    x += s;
    y += s;
    z += s;
    w += s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator -= (Value s)
{
    x -= s;
    y -= s;
    z -= s;
    w -= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator *= (Value s)
{
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator /= (Value s)
{
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator %= (Value s)
{
    x = fmodf(x, s);
    y = fmodf(y, s);
    z = fmodf(z, s);
    w = fmodf(w, s);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator ++ ()
{
    ++x;
    ++y;
    ++z;
    ++w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator -- ()
{
    --x;
    --y;
    --z;
    --w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator ++ (int) // NOLINT(cert-dcl21-cpp)
{
    Quaternion state(*this);
    ++x;
    ++y;
    ++z;
    ++w;
    return state;
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator -- (int) // NOLINT(cert-dcl21-cpp)
{
    Quaternion state(*this);
    --x;
    --y;
    --z;
    --w;
    return state;
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator + (const Quaternion & q) const
{
    return {x + q.x, y + q.y, z + q.z, w + q.w};
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator + (Value s) const
{
    return {x + s, y + s, z + s, w + s};
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator - (const Quaternion & q) const
{
    return {x - q.x, y - q.y, z - q.z, w - q.w};
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator - (Value s) const
{
    return {x - s, y - s, z - s, w - s};
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator * (const Quaternion & q) const
{
    return {x * q.x, y * q.y, z * q.z, w * q.w};
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator * (Value s) const
{
    return {x * s, y * s, z * s, w * s};
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator / (const Quaternion & q) const
{
    return {x / q.x, y / q.y, z / q.z, w / q.w};
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator / (Value s) const
{
    return {x / s, y / s, z / s, w / s};
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator % (const Quaternion & q) const
{
    return {fmodf(x, q.x), fmodf(y, q.y), fmodf(z, q.z), fmodf(w, q.w)};
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator % (Value s) const
{
    return {fmodf(x, s), fmodf(y, s), fmodf(z, s), fmodf(w, s)};
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator + () const
{
    return {fabsf(x), fabsf(y), fabsf(z), fabsf(w)};
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator - () const
{
    return {-x, -y, -z, -w};
}

// ------------------------------------------------------------------------------------------------
bool Quaternion::operator == (const Quaternion & q) const
{
    return EpsEq(x, q.x) && EpsEq(y, q.y) && EpsEq(z, q.z) && EpsEq(w, q.w);
}

// ------------------------------------------------------------------------------------------------
bool Quaternion::operator != (const Quaternion & q) const
{
    return !EpsEq(x, q.x) || !EpsEq(y, q.y) || !EpsEq(z, q.z) || !EpsEq(w, q.w);
}

// ------------------------------------------------------------------------------------------------
bool Quaternion::operator < (const Quaternion & q) const
{
    return EpsLt(x, q.x) && EpsLt(y, q.y) && EpsLt(z, q.z) && EpsLt(w, q.w);
}

// ------------------------------------------------------------------------------------------------
bool Quaternion::operator > (const Quaternion & q) const
{
    return EpsGt(x, q.x) && EpsGt(y, q.y) && EpsGt(z, q.z) && EpsGt(w, q.w);
}

// ------------------------------------------------------------------------------------------------
bool Quaternion::operator <= (const Quaternion & q) const
{
    return EpsLtEq(x, q.x) && EpsLtEq(y, q.y) && EpsLtEq(z, q.z) && EpsLtEq(w, q.w);
}

// ------------------------------------------------------------------------------------------------
bool Quaternion::operator >= (const Quaternion & q) const
{
    return EpsGtEq(x, q.x) && EpsGtEq(y, q.y) && EpsGtEq(z, q.z) && EpsGtEq(w, q.w);
}

// ------------------------------------------------------------------------------------------------
int32_t Quaternion::Cmp(const Quaternion & o) const
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
String Quaternion::ToString() const
{
    return fmt::format("{},{},{},{}", x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
void Quaternion::SetScalar(Value ns)
{
    x = ns;
    y = ns;
    z = ns;
    w = ns;
}

// ------------------------------------------------------------------------------------------------
void Quaternion::SetQuaternion(const Quaternion & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
}

// ------------------------------------------------------------------------------------------------
void Quaternion::SetQuaternionEx(Value nx, Value ny, Value nz, Value nw)
{
    x = nx;
    y = ny;
    z = nz;
    w = nw;
}

// ------------------------------------------------------------------------------------------------
void Quaternion::SetVector3(const Vector3 & v)
{
    SetVector3Ex(v.x, v.y, v.z);
}

// ------------------------------------------------------------------------------------------------
void Quaternion::SetVector3Ex(Value nx, Value ny, Value nz)
{
    double angle;

    angle = (nx * 0.5);
    const double sr = sin(angle);
    const double cr = cos(angle);

    angle = (ny * 0.5);
    const double sp = sin(angle);
    const double cp = cos(angle);

    angle = (nz * 0.5);
    const double sy = sin(angle);
    const double cy = cos(angle);

    const double cpcy = (cp * cy);
    const double spcy = (sp * cy);
    const double cpsy = (cp * sy);
    const double spsy = (sp * sy);

    x = STOVAL(sr * cpcy - cr * spsy);
    y = STOVAL(cr * spcy + sr * cpsy);
    z = STOVAL(cr * cpsy - sr * spcy);
    w = STOVAL(cr * cpcy + sr * spsy);

    Normalize();
}

// ------------------------------------------------------------------------------------------------
void Quaternion::SetVector4(const Vector4 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

// ------------------------------------------------------------------------------------------------
void Quaternion::SetStr(SQChar delim, StackStrF & values)
{
    SetQuaternion(Quaternion::GetEx(delim, values));
}

// ------------------------------------------------------------------------------------------------
void Quaternion::Generate()
{
    x = GetRandomFloat32();
    y = GetRandomFloat32();
    z = GetRandomFloat32();
    w = GetRandomFloat32();
}

// ------------------------------------------------------------------------------------------------
void Quaternion::Generate(Value min, Value max)
{
    if (EpsLt(max, min))
    {
        STHROWF("max value is lower than min value");
    }

    x = GetRandomFloat32(min, max);
    y = GetRandomFloat32(min, max);
    z = GetRandomFloat32(min, max);
    y = GetRandomFloat32(min, max);
}

// ------------------------------------------------------------------------------------------------
void Quaternion::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value wmin, Value wmax)
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
Quaternion Quaternion::Abs() const
{
    return {fabsf(x), fabsf(y), fabsf(z), fabsf(w)};
}

// ------------------------------------------------------------------------------------------------
bool Quaternion::IsNaN() const
{
    return isnanf(w) || isnanf(x) || isnanf(y) || isnanf(z);
}

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion::Value Quaternion::LengthSquared() const
{
    return (x * x) + (y * y) + (z * z) + (w * w);
}

// ------------------------------------------------------------------------------------------------
Quaternion::Value Quaternion::DotProduct(const Quaternion & quat) const
{
    return (x * quat.x) + (y * quat.y) + (z * quat.z) + (w * quat.w);
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::Conjugate() const
{
    return {-x, -y, -z, w};
}

// ------------------------------------------------------------------------------------------------
void Quaternion::Normalize()
{
    const Value len_squared = LengthSquared();

    if (!EpsEq(len_squared, STOVAL(1.0)) && EpsGt(len_squared, STOVAL(0.0)))
    {
        const Value inv_len = STOVAL(1.0) / sqrtf(len_squared);

        x *= inv_len;
        y *= inv_len;
        z *= inv_len;
        w *= inv_len;
    }
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::Normalized() const
{
    const Value len_squared = LengthSquared();

    if (!EpsEq(len_squared, STOVAL(1.0)) && EpsGt(len_squared, STOVAL(0.0)))
    {
        Value inv_len = 1.0f / sqrtf(len_squared);

        return ((*this) * inv_len);
    }
    else
    {
        return (*this);
    }
}

// ------------------------------------------------------------------------------------------------
void Quaternion::FromAngleAxis(Value angle, const Vector3 & axis)
{
    const Vector3 norm_axis = axis.Normalized();
    angle *= SQMOD_DEGTORAD_2;
    const Value sin_angle = sinf(angle);
    x = norm_axis.x * sin_angle;
    y = norm_axis.y * sin_angle;
    z = norm_axis.z * sin_angle;
    w = cosf(angle);
}

// ------------------------------------------------------------------------------------------------
void Quaternion::FromRotationTo(const Vector3 & start, const Vector3 & end)
{
    const Vector3 norm_start = start.Normalized();
    const Vector3 norm_end = end.Normalized();
    const Value d = norm_start.DotProduct(norm_end);

    if (EpsGt(d, STOVAL(-1.0)))
    {
        const Vector3 c = norm_start.CrossProduct(norm_end);
        const Value s = sqrtf((STOVAL(1.0) + d) * STOVAL(2.0));
        const Value inv_s = STOVAL(1.0) / s;

        x = c.x * inv_s;
        y = c.y * inv_s;
        z = c.z * inv_s;
        w = STOVAL(0.5) * s;
    }
    else
    {
        Vector3 axis = Vector3::RIGHT.CrossProduct(norm_start);

        if (EpsLt(axis.GetLength(), STOVAL(0.0)))
        {
            FromAngleAxis(STOVAL(180), Vector3::UP.CrossProduct(norm_start));
        }
        else
        {
            FromAngleAxis(STOVAL(180), axis);
        }
    }
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::Inverse() const
{
    const Value len_squared = LengthSquared();
    if (EpsEq(len_squared, STOVAL(1.0)))
    {
        return Conjugate();
    }
    else if (EpsGtEq(len_squared, STOVAL(0.0)))
    {
        return Conjugate() * (STOVAL(1.0) / len_squared);
    }
    else
    {
        return IDENTITY;
    }
}

// ------------------------------------------------------------------------------------------------
Vector3 Quaternion::ToEuler() const
{
    const double sqw = (w * w);
    const double sqx = (x * x);
    const double sqy = (y * y);
    const double sqz = (z * z);
    const double test = 2.0 * ((y * w) - (x * z));

    if (EpsEq(test, 1.0))
    {
        return {
            // bank = rotation about x-axis
            STOVAL(0.0),
            // attitude = rotation about y-axis
            STOVAL(SQMOD_PI64 / 2.0),
            // heading = rotation about z-axis
            STOVAL(-2.0 * atan2f(x, w))
        };
    }
    else if (EpsEq(test, -1.0))
    {
        return {
            // bank = rotation about x-axis
            STOVAL(0.0),
            // attitude = rotation about y-axis
            STOVAL(SQMOD_PI64 / -2.0),
            // heading = rotation about z-axis
            STOVAL(2.0 * atan2f(x, w))
        };
    }

    return {
        // bank = rotation about x-axis
        STOVAL(atan2(2.0 * ((y * z) + (x * w)), (-sqx - sqy + sqz + sqw))),
        // attitude = rotation about y-axis
        STOVAL(asin(Clamp(test, -1.0, 1.0))),
        // heading = rotation about z-axis
        STOVAL(atan2(2.0 * ((x * y) + (z * w)), (sqx - sqy - sqz + sqw)))
    };
}

// ------------------------------------------------------------------------------------------------
Quaternion::Value Quaternion::YawAngle() const
{
    return ToEuler().y;
}

// ------------------------------------------------------------------------------------------------
Quaternion::Value Quaternion::PitchAngle() const
{
    return ToEuler().x;
}

// ------------------------------------------------------------------------------------------------
Quaternion::Value Quaternion::RollAngle() const
{
    return ToEuler().z;
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::Slerp(Quaternion quat, Value t) const
{
    // Favor accuracy for native code builds
    Value cos_angle = DotProduct(quat);
    // Enable shortest path rotation
    if (EpsLt(cos_angle, STOVAL(0.0)))
    {
        cos_angle = -cos_angle;
        quat = -quat;
    }

    const Value angle = acosf(cos_angle);
    Value sin_angle = sinf(angle);
    Value t1, t2;

    if (sin_angle > STOVAL(0.001))
    {
        Value inv_sin_angle = STOVAL(1.0) / sin_angle;
        t1 = sinf((STOVAL(1.0) - t) * angle) * inv_sin_angle;
        t2 = sinf(t * angle) * inv_sin_angle;
    }
    else
    {
        t1 = STOVAL(1.0) - t;
        t2 = t;
    }

    return ((*this) * t1 + quat * t2);
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::Nlerp(const Quaternion & quat, Value t) const
{
    return NlerpEx(quat, t, false);
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::NlerpEx(const Quaternion & quat, Value t, bool shortest_path) const
{
    Quaternion result;
    const Value fcos = DotProduct(quat);
    if (EpsLt(fcos, STOVAL(0.0)) && shortest_path)
    {
        result = (*this) + (((-quat) - (*this)) * t);
    }
    else
    {
        result = (*this) + ((quat - (*this)) * t);
    }
    result.Normalize();
    return result;
}

// ------------------------------------------------------------------------------------------------
const Quaternion & Quaternion::Get(StackStrF & str)
{
    return Quaternion::GetEx(Quaternion::Delim, str);
}

// ------------------------------------------------------------------------------------------------
String Quaternion::Format(StackStrF & str) const
{
    return fmt::format(str.ToStr()
        , fmt::arg("x", x)
        , fmt::arg("y", y)
        , fmt::arg("z", z)
        , fmt::arg("w", w)
    );
}

// ------------------------------------------------------------------------------------------------
const Quaternion & Quaternion::GetEx(SQChar delim, StackStrF & str)
{
    static Quaternion quat;
    // Clear previous values, if any
    quat.Clear();
    // Is the specified string empty?
    if (str.mLen <= 0)
    {
        return quat; // Return the value as is!
    }
    // The format specifications that will be used to scan the string
    SQChar fs[] = _SC(" %f , %f , %f , %f ");
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    fs[14] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str.mPtr, fs, &quat.x, &quat.y, &quat.z, &quat.w);
    // Return the resulted value
    return quat;
}

// ================================================================================================
void Register_Quaternion(HSQUIRRELVM vm)
{
    typedef Quaternion::Value Val;

    RootTable(vm).Bind(Typename::Str,
        Class< Quaternion >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val, Val >()
        .Ctor< Val, Val, Val, Val >()
        // Member Variables
        .Var(_SC("x"), &Quaternion::x)
        .Var(_SC("y"), &Quaternion::y)
        .Var(_SC("z"), &Quaternion::z)
        .Var(_SC("w"), &Quaternion::w)
        .Var(_SC("X"), &Quaternion::x)
        .Var(_SC("Y"), &Quaternion::y)
        .Var(_SC("Z"), &Quaternion::z)
        .Var(_SC("W"), &Quaternion::w)
        // Core Meta-methods
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< Quaternion >, SQFloat, SQInteger, bool, std::nullptr_t, Quaternion >)
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Quaternion::ToString)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< Quaternion >, SQFloat, SQInteger, bool, std::nullptr_t, Quaternion >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< Quaternion >, SQFloat, SQInteger, bool, std::nullptr_t, Quaternion >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< Quaternion >, SQFloat, SQInteger, bool, std::nullptr_t, Quaternion >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< Quaternion >, SQFloat, SQInteger, bool, std::nullptr_t, Quaternion >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< Quaternion >, SQFloat, SQInteger, bool, std::nullptr_t, Quaternion >)
        .Func< Quaternion (Quaternion::*)(void) const >(_SC("_unm"), &Quaternion::operator -)
        // Properties
        .Prop(_SC("Abs"), &Quaternion::Abs)
        .Prop(_SC("NaN"), &Quaternion::IsNaN)
        .Prop(_SC("LengthSq"), &Quaternion::LengthSquared)
        .Prop(_SC("Conjugate"), &Quaternion::Conjugate)
        .Prop(_SC("Normalized"), &Quaternion::Normalized)
        .Prop(_SC("Inverse"), &Quaternion::Inverse)
        .Prop(_SC("Euler"), &Quaternion::ToEuler, &Quaternion::SetVector3)
        .Prop(_SC("YawAngle"), &Quaternion::YawAngle)
        .Prop(_SC("PitchAngle"), &Quaternion::PitchAngle)
        .Prop(_SC("RollAngle"), &Quaternion::RollAngle)
        // Member Methods
        .Func(_SC("SetScalar"), &Quaternion::SetScalar)
        .Func(_SC("SetQuaternion"), &Quaternion::SetQuaternion)
        .Func(_SC("SetQuaternionEx"), &Quaternion::SetQuaternionEx)
        .Func(_SC("SetVector3"), &Quaternion::SetVector3)
        .Func(_SC("SetVector3Ex"), &Quaternion::SetVector3Ex)
        .Func(_SC("SetVector4"), &Quaternion::SetVector4)
        .FmtFunc(_SC("SetStr"), &Quaternion::SetStr)
        .Func(_SC("Clear"), &Quaternion::Clear)
        .FmtFunc(_SC("Format"), &Quaternion::Format)
        .Func(_SC("DotProduct"), &Quaternion::DotProduct)
        .Func(_SC("Normalize"), &Quaternion::Normalize)
        .Func(_SC("FromAngleAxis"), &Quaternion::FromAngleAxis)
        .Func(_SC("FromRotationTo"), &Quaternion::FromRotationTo)
        .Func(_SC("Slerp"), &Quaternion::Slerp)
        .Func(_SC("Nlerp"), &Quaternion::Nlerp)
        .Func(_SC("NlerpEx"), &Quaternion::NlerpEx)
        // Member Overloads
        .Overload< void (Quaternion::*)(void) >(_SC("Generate"), &Quaternion::Generate)
        .Overload< void (Quaternion::*)(Val, Val) >(_SC("Generate"), &Quaternion::Generate)
        .Overload< void (Quaternion::*)(Val, Val, Val, Val, Val, Val, Val, Val) >(_SC("Generate"), &Quaternion::Generate)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Quaternion >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Quaternion >)
        .StaticFmtFunc(_SC("FromStr"), &Quaternion::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Quaternion::GetEx)
    );
}

} // Namespace:: SqMod
