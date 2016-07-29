// ------------------------------------------------------------------------------------------------
#include "Base/Vector4.hpp"
#include "Base/Vector3.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Shared.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Vector4 Vector4::NIL = Vector4(0);
const Vector4 Vector4::MIN = Vector4(std::numeric_limits< Vector4::Value >::min());
const Vector4 Vector4::MAX = Vector4(std::numeric_limits< Vector4::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Vector4::Delim = ',';

// ------------------------------------------------------------------------------------------------
SQInteger Vector4::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("Vector4");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Vector4::Vector4()
    : x(0.0), y(0.0), z(0.0), w(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector4::Vector4(Value sv)
    : x(sv), y(sv), z(sv), w(sv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector4::Vector4(Value xv, Value yv, Value zv)
    : x(xv), y(yv), z(zv), w(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector4::Vector4(Value xv, Value yv, Value zv, Value wv)
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
Vector4 Vector4::operator ++ (int)
{
    Vector4 state(*this);
    ++x;
    ++y;
    ++z;
    ++w;
    return state;
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator -- (int)
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
    return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator - (const Vector4 & v) const
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator * (const Vector4 & v) const
{
    return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator / (const Vector4 & v) const
{
    return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator % (const Vector4 & v) const
{
    return Vector4(std::fmod(x, v.x), std::fmod(y, v.y), std::fmod(z, v.z), std::fmod(w, v.w));
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator + (Value s) const
{
    return Vector4(x + s, y + s, z + s, w + s);
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator - (Value s) const
{
    return Vector4(x - s, y - s, z - s, w - s);
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator * (Value s) const
{
    return Vector4(x * s, y * s, z * s, w * s);
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator / (Value s) const
{
    return Vector4(x / s, y / s, z / s, w / s);
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator % (Value s) const
{
    return Vector4(std::fmod(x, s), std::fmod(y, s), std::fmod(z, s), std::fmod(w, s));
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator + () const
{
    return Vector4(std::fabs(x), std::fabs(y), std::fabs(z), std::fabs(w));
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator - () const
{
    return Vector4(-x, -y, -z, -w);
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
void Vector4::SetStr(CSStr values, SQChar delim)
{
    SetVector4(Vector4::Get(values, delim));
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
    return Vector4(std::fabs(x), std::fabs(y), std::fabs(z), std::fabs(w));
}

// ------------------------------------------------------------------------------------------------
const Vector4 & Vector4::Get(CSStr str)
{
    return Vector4::Get(str, Vector4::Delim);
}

// ------------------------------------------------------------------------------------------------
const Vector4 & Vector4::Get(CSStr str, SQChar delim)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %f , %f , %f , %f ");
    static Vector4 vec;
    // Clear previous values, if any
    vec.Clear();
    // Is the specified string empty?
    if (!str || *str == '\0')
    {
        return vec; // Return the value as is!
    }
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    fs[14] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str, &fs[0], &vec.x, &vec.y, &vec.z, &vec.w);
    // Return the resulted value
    return vec;
}

// ------------------------------------------------------------------------------------------------
const Vector4 & GetVector4()
{
    static Vector4 vec;
    vec.Clear();
    return vec;
}

// ------------------------------------------------------------------------------------------------
const Vector4 & GetVector4(Float32 sv)
{
    static Vector4 vec;
    vec.SetScalar(sv);
    return vec;
}

// ------------------------------------------------------------------------------------------------
const Vector4 & GetVector4(Float32 xv, Float32 yv, Float32 zv)
{
    static Vector4 vec;
    vec.SetVector3Ex(xv, yv, zv);
    return vec;
}

// ------------------------------------------------------------------------------------------------
const Vector4 & GetVector4(Float32 xv, Float32 yv, Float32 zv, Float32 wv)
{
    static Vector4 vec;
    vec.SetVector4Ex(xv, yv, zv, wv);
    return vec;
}

// ------------------------------------------------------------------------------------------------
const Vector4 & GetVector4(const Vector4 & o)
{
    static Vector4 vec;
    vec.SetVector4(o);
    return vec;
}

// ================================================================================================
void Register_Vector4(HSQUIRRELVM vm)
{
    typedef Vector4::Value Val;

    RootTable(vm).Bind(_SC("Vector4"), Class< Vector4 >(vm, _SC("Vector4"))
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
        .Func(_SC("_tostring"), &Vector4::ToString)
        .SquirrelFunc(_SC("_typename"), &Vector4::Typename)
        .Func(_SC("_cmp"), &Vector4::Cmp)
        // Meta-methods
        .Func< Vector4 (Vector4::*)(const Vector4 &) const >(_SC("_add"), &Vector4::operator +)
        .Func< Vector4 (Vector4::*)(const Vector4 &) const >(_SC("_sub"), &Vector4::operator -)
        .Func< Vector4 (Vector4::*)(const Vector4 &) const >(_SC("_mul"), &Vector4::operator *)
        .Func< Vector4 (Vector4::*)(const Vector4 &) const >(_SC("_div"), &Vector4::operator /)
        .Func< Vector4 (Vector4::*)(const Vector4 &) const >(_SC("_modulo"), &Vector4::operator %)
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
        .Func(_SC("SetStr"), &Vector4::SetStr)
        .Func(_SC("Clear"), &Vector4::Clear)
        // Member Overloads
        .Overload< void (Vector4::*)(void) >(_SC("Generate"), &Vector4::Generate)
        .Overload< void (Vector4::*)(Val, Val) >(_SC("Generate"), &Vector4::Generate)
        .Overload< void (Vector4::*)(Val, Val, Val, Val, Val, Val, Val, Val) >(_SC("Generate"), &Vector4::Generate)
        // Static Overloads
        .StaticOverload< const Vector4 & (*)(CSStr) >(_SC("FromStr"), &Vector4::Get)
        .StaticOverload< const Vector4 & (*)(CSStr, SQChar) >(_SC("FromStr"), &Vector4::Get)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Vector4 >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Vector4 >)
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
