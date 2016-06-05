// ------------------------------------------------------------------------------------------------
#include "Base/Vector3.hpp"
#include "Base/Vector4.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Shared.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Vector3 Vector3::NIL = Vector3(0);
const Vector3 Vector3::MIN = Vector3(std::numeric_limits< Vector3::Value >::min());
const Vector3 Vector3::MAX = Vector3(std::numeric_limits< Vector3::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Vector3::Delim = ',';

// ------------------------------------------------------------------------------------------------
SQInteger Vector3::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("Vector3");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Vector3::Vector3()
    : x(0.0), y(0.0), z(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector3::Vector3(Value sv)
    : x(sv), y(sv), z(sv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Vector3::Vector3(Value xv, Value yv, Value zv)
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

Vector3 & Vector3::operator = (const Vector4 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

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

Vector3 & Vector3::operator -= (const Vector3 & v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3 & Vector3::operator *= (const Vector3 & v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

Vector3 & Vector3::operator /= (const Vector3 & v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

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

Vector3 & Vector3::operator -= (Value s)
{
    x -= s;
    y -= s;
    z -= s;
    return *this;
}

Vector3 & Vector3::operator *= (Value s)
{
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

Vector3 & Vector3::operator /= (Value s)
{
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

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

Vector3 & Vector3::operator -- ()
{
    --x;
    --y;
    --z;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator ++ (int)
{
    Vector3 state(*this);
    ++x;
    ++y;
    ++z;
    return state;
}

Vector3 Vector3::operator -- (int)
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
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator - (const Vector3 & v) const
{
    return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator * (const Vector3 & v) const
{
    return Vector3(x * v.x, y * v.y, z * v.z);
}

Vector3 Vector3::operator / (const Vector3 & v) const
{
    return Vector3(x / v.x, y / v.y, z / v.z);
}

Vector3 Vector3::operator % (const Vector3 & v) const
{
    return Vector3(std::fmod(x, v.x), std::fmod(y, v.y), std::fmod(z, v.z));
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator + (Value s) const
{
    return Vector3(x + s, y + s, z + s);
}

Vector3 Vector3::operator - (Value s) const
{
    return Vector3(x - s, y - s, z - s);
}

Vector3 Vector3::operator * (Value s) const
{
    return Vector3(x * s, y * s, z * s);
}

Vector3 Vector3::operator / (Value s) const
{
    return Vector3(x / s, y / s, z / s);
}

Vector3 Vector3::operator % (Value s) const
{
    return Vector3(std::fmod(x, s), std::fmod(y, s), std::fmod(z, s));
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::operator + () const
{
    return Vector3(std::fabs(x), std::fabs(y), std::fabs(z));
}

Vector3 Vector3::operator - () const
{
    return Vector3(-x, -y, -z);
}

// ------------------------------------------------------------------------------------------------
bool Vector3::operator == (const Vector3 & v) const
{
    return EpsEq(x, v.x) && EpsEq(y, v.y) && EpsEq(z, v.z);
}

bool Vector3::operator != (const Vector3 & v) const
{
    return !EpsEq(x, v.x) && !EpsEq(y, v.y) && !EpsEq(z, v.z);
}

bool Vector3::operator < (const Vector3 & v) const
{
    return EpsLt(x, v.x) && EpsLt(y, v.y) && EpsLt(z, v.z);
}

bool Vector3::operator > (const Vector3 & v) const
{
    return EpsGt(x, v.x) && EpsGt(y, v.y) && EpsGt(z, v.z);
}

bool Vector3::operator <= (const Vector3 & v) const
{
    return EpsLtEq(x, v.x) && EpsLtEq(y, v.y) && EpsLtEq(z, v.z);
}

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
void Vector3::Set(Value ns)
{
    x = ns;
    y = ns;
    z = ns;
}

void Vector3::Set(Value nx, Value ny, Value nz)
{
    x = nx;
    y = ny;
    z = nz;
}

// ------------------------------------------------------------------------------------------------
void Vector3::Set(const Vector3 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

void Vector3::Set(const Vector4 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

void Vector3::Set(const Quaternion & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
}

// ------------------------------------------------------------------------------------------------
void Vector3::Set(CSStr values, SQChar delim)
{
    Set(Vector3::Get(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Vector3::Generate()
{
    x = GetRandomFloat32();
    y = GetRandomFloat32();
    z = GetRandomFloat32();
}

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
    return Vector3(std::fabs(x), std::fabs(y), std::fabs(z));
}

// ------------------------------------------------------------------------------------------------
const Vector3 & Vector3::Get(CSStr str)
{
    return Vector3::Get(str, Vector3::Delim);
}

// ------------------------------------------------------------------------------------------------
const Vector3 & Vector3::Get(CSStr str, SQChar delim)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %f , %f , %f ");
    static Vector3 vec;
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
    // Attempt to extract the component values from the specified string
    std::sscanf(str, &fs[0], &vec.x, &vec.y, &vec.z);
    // Return the resulted value
    return vec;
}

// ------------------------------------------------------------------------------------------------
const Vector3 & GetVector3()
{
    static Vector3 vec;
    vec.Clear();
    return vec;
}

const Vector3 & GetVector3(Float32 sv)
{
    static Vector3 vec;
    vec.Set(sv);
    return vec;
}

const Vector3 & GetVector3(Float32 xv, Float32 yv, Float32 zv)
{
    static Vector3 vec;
    vec.Set(xv, yv, zv);
    return vec;
}

const Vector3 & GetVector3(const Vector3 & o)
{
    static Vector3 vec;
    vec.Set(o);
    return vec;
}

// ================================================================================================
void Register_Vector3(HSQUIRRELVM vm)
{
    typedef Vector3::Value Val;

    RootTable(vm).Bind(_SC("Vector3"), Class< Vector3 >(vm, _SC("Vector3"))
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val, Val >()
        // Static Members
        .SetStaticValue(_SC("Delim"), &Vector3::Delim)
        // Member Variables
        .Var(_SC("X"), &Vector3::x)
        .Var(_SC("Y"), &Vector3::y)
        .Var(_SC("Z"), &Vector3::z)
        // Properties
        .Prop(_SC("Abs"), &Vector3::Abs)
        // Core Meta-methods
        .Func(_SC("_tostring"), &Vector3::ToString)
        .SquirrelFunc(_SC("_typename"), &Vector3::Typename)
        .Func(_SC("_cmp"), &Vector3::Cmp)
        // Meta-methods
        .Func< Vector3 (Vector3::*)(const Vector3 &) const >(_SC("_add"), &Vector3::operator +)
        .Func< Vector3 (Vector3::*)(const Vector3 &) const >(_SC("_sub"), &Vector3::operator -)
        .Func< Vector3 (Vector3::*)(const Vector3 &) const >(_SC("_mul"), &Vector3::operator *)
        .Func< Vector3 (Vector3::*)(const Vector3 &) const >(_SC("_div"), &Vector3::operator /)
        .Func< Vector3 (Vector3::*)(const Vector3 &) const >(_SC("_modulo"), &Vector3::operator %)
        .Func< Vector3 (Vector3::*)(void) const >(_SC("_unm"), &Vector3::operator -)
        // Setters
        .Overload< void (Vector3::*)(Val) >(_SC("Set"), &Vector3::Set)
        .Overload< void (Vector3::*)(Val, Val, Val) >(_SC("Set"), &Vector3::Set)
        .Overload< void (Vector3::*)(const Vector3 &) >(_SC("SetVec3"), &Vector3::Set)
        .Overload< void (Vector3::*)(const Vector4 &) >(_SC("SetVec4"), &Vector3::Set)
        .Overload< void (Vector3::*)(const Quaternion &) >(_SC("SetQuat"), &Vector3::Set)
        .Overload< void (Vector3::*)(CSStr, SQChar) >(_SC("SetStr"), &Vector3::Set)
        // Random Generators
        .Overload< void (Vector3::*)(void) >(_SC("Generate"), &Vector3::Generate)
        .Overload< void (Vector3::*)(Val, Val) >(_SC("Generate"), &Vector3::Generate)
        .Overload< void (Vector3::*)(Val, Val, Val, Val, Val, Val) >(_SC("Generate"), &Vector3::Generate)
        // Utility Methods
        .Func(_SC("Clear"), &Vector3::Clear)
        // Static Overloads
        .StaticOverload< const Vector3 & (*)(CSStr) >(_SC("FromStr"), &Vector3::Get)
        .StaticOverload< const Vector3 & (*)(CSStr, SQChar) >(_SC("FromStr"), &Vector3::Get)
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
