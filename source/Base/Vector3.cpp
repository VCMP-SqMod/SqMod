#include "Base/Vector3.hpp"
#include "Base/Vector4.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Vector3 Vector3::NIL = Vector3(0);
const Vector3 Vector3::MIN = Vector3(std::numeric_limits<Vector3::Value>::min());
const Vector3 Vector3::MAX = Vector3(std::numeric_limits<Vector3::Value>::max());

// ------------------------------------------------------------------------------------------------
SQChar Vector3::Delim = ',';

// ------------------------------------------------------------------------------------------------
Vector3::Vector3()
    : x(0.0), y(0.0), z(0.0)
{

}

Vector3::Vector3(Value s)
    : x(s), y(s), z(s)
{

}

Vector3::Vector3(Value xv, Value yv, Value zv)
    : x(xv), y(yv), z(zv)
{

}

// ------------------------------------------------------------------------------------------------
Vector3::Vector3(const Vector4 & v)
    : x(v.x), y(v.y), z(v.z)
{

}

Vector3::Vector3(const Quaternion & q)
    : x(q.x), y(q.y), z(q.z)
{

}

// ------------------------------------------------------------------------------------------------
Vector3::Vector3(const SQChar * values, char delim)
    : Vector3(GetVector3(values, delim))
{

}

// ------------------------------------------------------------------------------------------------
Vector3::Vector3(const Vector3 & v)
    : x(v.x), y(v.y), z(v.z)
{

}

Vector3::Vector3(Vector3 && v)
    : x(v.x), y(v.y), z(v.z)
{

}

// ------------------------------------------------------------------------------------------------
Vector3::~Vector3()
{

}

// ------------------------------------------------------------------------------------------------
Vector3 & Vector3::operator = (const Vector3 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

Vector3 & Vector3::operator = (Vector3 && v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
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
    return std::isless(x, v.x) && std::isless(y, v.y) && std::isless(z, v.z);
}

bool Vector3::operator > (const Vector3 & v) const
{
    return std::isgreater(x, v.x) && std::isgreater(y, v.y) && std::isgreater(z, v.z);
}

bool Vector3::operator <= (const Vector3 & v) const
{
    return std::islessequal(x, v.x) && std::islessequal(y, v.y) && std::islessequal(z, v.z);
}

bool Vector3::operator >= (const Vector3 & v) const
{
    return std::isgreaterequal(x, v.x) && std::isgreaterequal(y, v.y) && std::isgreaterequal(z, v.z);
}

// ------------------------------------------------------------------------------------------------
SQInteger Vector3::Cmp(const Vector3 & v) const
{
    return *this == v ? 0 : (*this > v ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Vector3::ToString() const
{
    return ToStringF("%f,%f,%f", x, y, z);
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
void Vector3::Set(const SQChar * values, char delim)
{
    Set(GetVector3(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Vector3::Generate()
{
    x = RandomVal<Value>::Get();
    y = RandomVal<Value>::Get();
    z = RandomVal<Value>::Get();
}

void Vector3::Generate(Value min, Value max)
{
    if (max < min)
    {
        LogErr("max value is lower than min value");
    }
    else
    {
        x = RandomVal<Value>::Get(min, max);
        y = RandomVal<Value>::Get(min, max);
        z = RandomVal<Value>::Get(min, max);
    }
}

void Vector3::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax)
{
    if (std::isless(xmax, xmin) || std::isless(ymax, ymin) || std::isless(zmax, zmin))
    {
        LogErr("max value is lower than min value");
    }
    else
    {
        x = RandomVal<Value>::Get(xmin, xmax);
        y = RandomVal<Value>::Get(ymin, ymax);
        z = RandomVal<Value>::Get(zmin, zmax);
    }
}

// ------------------------------------------------------------------------------------------------
Vector3 Vector3::Abs() const
{
    return Vector3(std::fabs(x), std::fabs(y), std::fabs(z));
}

// ================================================================================================
bool Register_Vector3(HSQUIRRELVM vm)
{
    LogDbg("Beginning registration of <Vector3> type");

    typedef Vector3::Value Val;

    Sqrat::RootTable(vm).Bind(_SC("Vector3"), Sqrat::Class<Vector3>(vm, _SC("Vector3"))
        .Ctor()
        .Ctor<Val>()
        .Ctor<Val, Val, Val>()
        .Ctor<const SQChar *, SQChar>()

        .SetStaticValue(_SC("delim"), &Vector3::Delim)

        .Var(_SC("x"), &Vector3::x)
        .Var(_SC("y"), &Vector3::y)
        .Var(_SC("z"), &Vector3::z)

        .Prop(_SC("abs"), &Vector3::Abs)

        .Func(_SC("_tostring"), &Vector3::ToString)
        .Func(_SC("_cmp"), &Vector3::Cmp)

        .Func<Vector3 (Vector3::*)(const Vector3 &) const>(_SC("_add"), &Vector3::operator +)
        .Func<Vector3 (Vector3::*)(const Vector3 &) const>(_SC("_sub"), &Vector3::operator -)
        .Func<Vector3 (Vector3::*)(const Vector3 &) const>(_SC("_mul"), &Vector3::operator *)
        .Func<Vector3 (Vector3::*)(const Vector3 &) const>(_SC("_div"), &Vector3::operator /)
        .Func<Vector3 (Vector3::*)(const Vector3 &) const>(_SC("_modulo"), &Vector3::operator %)
        .Func<Vector3 (Vector3::*)(void) const>(_SC("_unm"), &Vector3::operator -)

        .Overload<void (Vector3::*)(Val)>(_SC("set"), &Vector3::Set)
        .Overload<void (Vector3::*)(Val, Val, Val)>(_SC("set"), &Vector3::Set)
        .Overload<void (Vector3::*)(const Vector3 &)>(_SC("set_vec3"), &Vector3::Set)
        .Overload<void (Vector3::*)(const Vector4 &)>(_SC("set_vec4"), &Vector3::Set)
        .Overload<void (Vector3::*)(const Quaternion &)>(_SC("set_quat"), &Vector3::Set)
        .Overload<void (Vector3::*)(const SQChar *, SQChar)>(_SC("set_str"), &Vector3::Set)

        .Overload<void (Vector3::*)(void)>(_SC("generate"), &Vector3::Generate)
        .Overload<void (Vector3::*)(Val, Val)>(_SC("generate"), &Vector3::Generate)
        .Overload<void (Vector3::*)(Val, Val, Val, Val, Val, Val)>(_SC("generate"), &Vector3::Generate)

        .Func(_SC("clear"), &Vector3::Clear)

        .Func<Vector3 & (Vector3::*)(const Vector3 &)>(_SC("opAddAssign"), &Vector3::operator +=)
        .Func<Vector3 & (Vector3::*)(const Vector3 &)>(_SC("opSubAssign"), &Vector3::operator -=)
        .Func<Vector3 & (Vector3::*)(const Vector3 &)>(_SC("opMulAssign"), &Vector3::operator *=)
        .Func<Vector3 & (Vector3::*)(const Vector3 &)>(_SC("opDivAssign"), &Vector3::operator /=)
        .Func<Vector3 & (Vector3::*)(const Vector3 &)>(_SC("opModAssign"), &Vector3::operator %=)

        .Func<Vector3 & (Vector3::*)(Vector3::Value)>(_SC("opAddAssignS"), &Vector3::operator +=)
        .Func<Vector3 & (Vector3::*)(Vector3::Value)>(_SC("opSubAssignS"), &Vector3::operator -=)
        .Func<Vector3 & (Vector3::*)(Vector3::Value)>(_SC("opMulAssignS"), &Vector3::operator *=)
        .Func<Vector3 & (Vector3::*)(Vector3::Value)>(_SC("opDivAssignS"), &Vector3::operator /=)
        .Func<Vector3 & (Vector3::*)(Vector3::Value)>(_SC("opModAssignS"), &Vector3::operator %=)

        .Func<Vector3 & (Vector3::*)(void)>(_SC("opPreInc"), &Vector3::operator ++)
        .Func<Vector3 & (Vector3::*)(void)>(_SC("opPreDec"), &Vector3::operator --)
        .Func<Vector3 (Vector3::*)(int)>(_SC("opPostInc"), &Vector3::operator ++)
        .Func<Vector3 (Vector3::*)(int)>(_SC("opPostDec"), &Vector3::operator --)

        .Func<Vector3 (Vector3::*)(const Vector3 &) const>(_SC("opAdd"), &Vector3::operator +)
        .Func<Vector3 (Vector3::*)(const Vector3 &) const>(_SC("opSub"), &Vector3::operator -)
        .Func<Vector3 (Vector3::*)(const Vector3 &) const>(_SC("opMul"), &Vector3::operator *)
        .Func<Vector3 (Vector3::*)(const Vector3 &) const>(_SC("opDiv"), &Vector3::operator /)
        .Func<Vector3 (Vector3::*)(const Vector3 &) const>(_SC("opMod"), &Vector3::operator %)

        .Func<Vector3 (Vector3::*)(Vector3::Value) const>(_SC("opAddS"), &Vector3::operator +)
        .Func<Vector3 (Vector3::*)(Vector3::Value) const>(_SC("opSubS"), &Vector3::operator -)
        .Func<Vector3 (Vector3::*)(Vector3::Value) const>(_SC("opMulS"), &Vector3::operator *)
        .Func<Vector3 (Vector3::*)(Vector3::Value) const>(_SC("opDivS"), &Vector3::operator /)
        .Func<Vector3 (Vector3::*)(Vector3::Value) const>(_SC("opModS"), &Vector3::operator %)

        .Func<Vector3 (Vector3::*)(void) const>(_SC("opUnPlus"), &Vector3::operator +)
        .Func<Vector3 (Vector3::*)(void) const>(_SC("opUnMinus"), &Vector3::operator -)

        .Func<bool (Vector3::*)(const Vector3 &) const>(_SC("opEqual"), &Vector3::operator ==)
        .Func<bool (Vector3::*)(const Vector3 &) const>(_SC("opNotEqual"), &Vector3::operator !=)
        .Func<bool (Vector3::*)(const Vector3 &) const>(_SC("opLessThan"), &Vector3::operator <)
        .Func<bool (Vector3::*)(const Vector3 &) const>(_SC("opGreaterThan"), &Vector3::operator >)
        .Func<bool (Vector3::*)(const Vector3 &) const>(_SC("opLessEqual"), &Vector3::operator <=)
        .Func<bool (Vector3::*)(const Vector3 &) const>(_SC("opGreaterEqual"), &Vector3::operator >=)
    );

    LogDbg("Registration of <Vector3> type was successful");

    return true;
}

} // Namespace:: SqMod
