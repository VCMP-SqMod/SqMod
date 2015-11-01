#include "Base/Vector4.hpp"
#include "Base/Vector3.hpp"
#include "Base/Quaternion.hpp"
#include "Base/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Vector4 Vector4::NIL = Vector4(0);
const Vector4 Vector4::MIN = Vector4(std::numeric_limits<Vector4::Value>::min());
const Vector4 Vector4::MAX = Vector4(std::numeric_limits<Vector4::Value>::max());

// ------------------------------------------------------------------------------------------------
SQChar Vector4::Delim = ',';

// ------------------------------------------------------------------------------------------------
Vector4::Vector4()
    : x(0.0), y(0.0), z(0.0), w(0.0)
{

}

Vector4::Vector4(Value s)
    : x(s), y(s), z(s), w(s)
{

}

Vector4::Vector4(Value xv, Value yv, Value zv)
    : x(xv), y(yv), z(zv), w(0.0)
{

}

Vector4::Vector4(Value xv, Value yv, Value zv, Value wv)
    : x(xv), y(yv), z(zv), w(wv)
{

}

// ------------------------------------------------------------------------------------------------
Vector4::Vector4(const Vector3 & v)
    : x(v.x), y(v.y), z(v.z), w(0.0)
{

}

Vector4::Vector4(const Quaternion & q)
    : x(q.x), y(q.y), z(q.z), w(q.w)
{

}

// ------------------------------------------------------------------------------------------------
Vector4::Vector4(const SQChar * values, SQChar delim)
    : Vector4(GetVector4(values, delim))
{

}

// ------------------------------------------------------------------------------------------------
Vector4::Vector4(const Vector4 & v)
    : x(v.x), y(v.y), z(v.z), w(v.w)
{

}

Vector4::Vector4(Vector4 && v)
    : x(v.x), y(v.y), z(v.z), w(v.w)
{

}

// ------------------------------------------------------------------------------------------------
Vector4::~Vector4()
{

}

// ------------------------------------------------------------------------------------------------
Vector4 & Vector4::operator = (const Vector4 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

Vector4 & Vector4::operator = (Vector4 && v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
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

Vector4 & Vector4::operator = (const Vector3 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 0.0;
    return *this;
}

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

Vector4 & Vector4::operator -= (const Vector4 & v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

Vector4 & Vector4::operator *= (const Vector4 & v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}

Vector4 & Vector4::operator /= (const Vector4 & v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}

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

Vector4 & Vector4::operator -= (Value s)
{
    x -= s;
    y -= s;
    z -= s;
    w -= s;
    return *this;
}

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

Vector4 Vector4::operator - (const Vector4 & v) const
{
    return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator * (const Vector4 & v) const
{
    return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4 Vector4::operator / (const Vector4 & v) const
{
    return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

Vector4 Vector4::operator % (const Vector4 & v) const
{
    return Vector4(std::fmod(x, v.x), std::fmod(y, v.y), std::fmod(z, v.z), std::fmod(w, v.w));
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator + (Value s) const
{
    return Vector4(x + s, y + s, z + s, w + s);
}

Vector4 Vector4::operator - (Value s) const
{
    return Vector4(x - s, y - s, z - s, w - s);
}

Vector4 Vector4::operator * (Value s) const
{
    return Vector4(x * s, y * s, z * s, w * s);
}

Vector4 Vector4::operator / (Value s) const
{
    return Vector4(x / s, y / s, z / s, w / s);
}

Vector4 Vector4::operator % (Value s) const
{
    return Vector4(std::fmod(x, s), std::fmod(y, s), std::fmod(z, s), std::fmod(w, s));
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::operator + () const
{
    return Vector4(std::fabs(x), std::fabs(y), std::fabs(z), std::fabs(w));
}

Vector4 Vector4::operator - () const
{
    return Vector4(-x, -y, -z, -w);
}

// ------------------------------------------------------------------------------------------------
bool Vector4::operator == (const Vector4 & v) const
{
    return EpsEq(x, v.x) && EpsEq(y, v.y) && EpsEq(z, v.z) && EpsEq(w, v.w);
}

bool Vector4::operator != (const Vector4 & v) const
{
    return !EpsEq(x, v.x) && !EpsEq(y, v.y) && !EpsEq(z, v.z) && !EpsEq(w, v.w);
}

bool Vector4::operator < (const Vector4 & v) const
{
    return std::isless(x, v.x) && std::isless(y, v.y) && std::isless(z, v.z) && std::isless(w, v.w);
}

bool Vector4::operator > (const Vector4 & v) const
{
    return std::isgreater(x, v.x) && std::isgreater(y, v.y) && std::isgreater(z, v.z) && std::isgreater(w, v.w);
}

bool Vector4::operator <= (const Vector4 & v) const
{
    return std::islessequal(x, v.x) && std::islessequal(y, v.y) && std::islessequal(z, v.z) && std::islessequal(w, v.w);
}

bool Vector4::operator >= (const Vector4 & v) const
{
    return std::isgreaterequal(x, v.x) && std::isgreaterequal(y, v.y) && std::isgreaterequal(z, v.z) && std::isgreaterequal(w, v.w);
}

// ------------------------------------------------------------------------------------------------
SQInteger Vector4::Cmp(const Vector4 & v) const
{
    return *this == v ? 0 : (*this > v ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Vector4::ToString() const
{
    return ToStringF("%f,%f,%f,%f", x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
void Vector4::Set(Value ns)
{
    x = ns;
    y = ns;
    z = ns;
    w = ns;
}

void Vector4::Set(Value nx, Value ny, Value nz)
{
    x = nx;
    y = ny;
    z = nz;
}

void Vector4::Set(Value nx, Value ny, Value nz, Value nw)
{
    x = nx;
    y = ny;
    z = nz;
    w = nw;
}

// ------------------------------------------------------------------------------------------------
void Vector4::Set(const Vector4 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

void Vector4::Set(const Vector3 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 0.0;
}

void Vector4::Set(const Quaternion & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
}

// ------------------------------------------------------------------------------------------------
void Vector4::Set(const SQChar * values, SQChar delim)
{
    Set(GetVector4(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Vector4::Generate()
{
    x = RandomVal<Value>::Get();
    y = RandomVal<Value>::Get();
    z = RandomVal<Value>::Get();
    w = RandomVal<Value>::Get();
}

void Vector4::Generate(Value min, Value max)
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
        y = RandomVal<Value>::Get(min, max);
    }
}

void Vector4::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value wmin, Value wmax)
{
    if (std::isless(xmax, xmin) || std::isless(ymax, ymin) || std::isless(zmax, zmin) || std::isless(wmax, wmin))
    {
        LogErr("max value is lower than min value");
    }
    else
    {
        x = RandomVal<Value>::Get(xmin, xmax);
        y = RandomVal<Value>::Get(ymin, ymax);
        z = RandomVal<Value>::Get(zmin, zmax);
        y = RandomVal<Value>::Get(ymin, ymax);
    }
}

// ------------------------------------------------------------------------------------------------
Vector4 Vector4::Abs() const
{
    return Vector4(std::fabs(x), std::fabs(y), std::fabs(z), std::fabs(w));
}

// ================================================================================================
bool Register_Vector4(HSQUIRRELVM vm)
{
    LogDbg("Beginning registration of <Vector4> type");

    typedef Vector4::Value Val;

    Sqrat::RootTable(vm).Bind(_SC("Vector4"), Sqrat::Class<Vector4>(vm, _SC("Vector4"))
        .Ctor()
        .Ctor<Val>()
        .Ctor<Val, Val, Val>()
        .Ctor<Val, Val, Val, Val>()
        .Ctor<const SQChar *, SQChar>()

        .SetStaticValue(_SC("delim"), &Vector4::Delim)

        .Var(_SC("x"), &Vector4::x)
        .Var(_SC("y"), &Vector4::y)
        .Var(_SC("z"), &Vector4::z)
        .Var(_SC("w"), &Vector4::w)

        .Prop(_SC("abs"), &Vector4::Abs)

        .Func(_SC("_tostring"), &Vector4::ToString)
        .Func(_SC("_cmp"), &Vector4::Cmp)

        .Func<Vector4 (Vector4::*)(const Vector4 &) const>(_SC("_add"), &Vector4::operator +)
        .Func<Vector4 (Vector4::*)(const Vector4 &) const>(_SC("_sub"), &Vector4::operator -)
        .Func<Vector4 (Vector4::*)(const Vector4 &) const>(_SC("_mul"), &Vector4::operator *)
        .Func<Vector4 (Vector4::*)(const Vector4 &) const>(_SC("_div"), &Vector4::operator /)
        .Func<Vector4 (Vector4::*)(const Vector4 &) const>(_SC("_modulo"), &Vector4::operator %)
        .Func<Vector4 (Vector4::*)(void) const>(_SC("_unm"), &Vector4::operator -)

        .Overload<void (Vector4::*)(Val)>(_SC("set"), &Vector4::Set)
        .Overload<void (Vector4::*)(Val, Val, Val)>(_SC("set"), &Vector4::Set)
        .Overload<void (Vector4::*)(Val, Val, Val, Val)>(_SC("set"), &Vector4::Set)
        .Overload<void (Vector4::*)(const Vector4 &)>(_SC("set_vec4"), &Vector4::Set)
        .Overload<void (Vector4::*)(const Vector3 &)>(_SC("set_vec3"), &Vector4::Set)
        .Overload<void (Vector4::*)(const Quaternion &)>(_SC("set_quat"), &Vector4::Set)
        .Overload<void (Vector4::*)(const SQChar *, SQChar)>(_SC("set_str"), &Vector4::Set)

        .Overload<void (Vector4::*)(void)>(_SC("generate"), &Vector4::Generate)
        .Overload<void (Vector4::*)(Val, Val)>(_SC("generate"), &Vector4::Generate)
        .Overload<void (Vector4::*)(Val, Val, Val, Val, Val, Val, Val, Val)>(_SC("generate"), &Vector4::Generate)

        .Func(_SC("clear"), &Vector4::Clear)

        .Func<Vector4 & (Vector4::*)(const Vector4 &)>(_SC("opAddAssign"), &Vector4::operator +=)
        .Func<Vector4 & (Vector4::*)(const Vector4 &)>(_SC("opSubAssign"), &Vector4::operator -=)
        .Func<Vector4 & (Vector4::*)(const Vector4 &)>(_SC("opMulAssign"), &Vector4::operator *=)
        .Func<Vector4 & (Vector4::*)(const Vector4 &)>(_SC("opDivAssign"), &Vector4::operator /=)
        .Func<Vector4 & (Vector4::*)(const Vector4 &)>(_SC("opModAssign"), &Vector4::operator %=)

        .Func<Vector4 & (Vector4::*)(Vector4::Value)>(_SC("opAddAssignS"), &Vector4::operator +=)
        .Func<Vector4 & (Vector4::*)(Vector4::Value)>(_SC("opSubAssignS"), &Vector4::operator -=)
        .Func<Vector4 & (Vector4::*)(Vector4::Value)>(_SC("opMulAssignS"), &Vector4::operator *=)
        .Func<Vector4 & (Vector4::*)(Vector4::Value)>(_SC("opDivAssignS"), &Vector4::operator /=)
        .Func<Vector4 & (Vector4::*)(Vector4::Value)>(_SC("opModAssignS"), &Vector4::operator %=)

        .Func<Vector4 & (Vector4::*)(void)>(_SC("opPreInc"), &Vector4::operator ++)
        .Func<Vector4 & (Vector4::*)(void)>(_SC("opPreDec"), &Vector4::operator --)
        .Func<Vector4 (Vector4::*)(int)>(_SC("opPostInc"), &Vector4::operator ++)
        .Func<Vector4 (Vector4::*)(int)>(_SC("opPostDec"), &Vector4::operator --)

        .Func<Vector4 (Vector4::*)(const Vector4 &) const>(_SC("opAdd"), &Vector4::operator +)
        .Func<Vector4 (Vector4::*)(const Vector4 &) const>(_SC("opSub"), &Vector4::operator -)
        .Func<Vector4 (Vector4::*)(const Vector4 &) const>(_SC("opMul"), &Vector4::operator *)
        .Func<Vector4 (Vector4::*)(const Vector4 &) const>(_SC("opDiv"), &Vector4::operator /)
        .Func<Vector4 (Vector4::*)(const Vector4 &) const>(_SC("opMod"), &Vector4::operator %)

        .Func<Vector4 (Vector4::*)(Vector4::Value) const>(_SC("opAddS"), &Vector4::operator +)
        .Func<Vector4 (Vector4::*)(Vector4::Value) const>(_SC("opSubS"), &Vector4::operator -)
        .Func<Vector4 (Vector4::*)(Vector4::Value) const>(_SC("opMulS"), &Vector4::operator *)
        .Func<Vector4 (Vector4::*)(Vector4::Value) const>(_SC("opDivS"), &Vector4::operator /)
        .Func<Vector4 (Vector4::*)(Vector4::Value) const>(_SC("opModS"), &Vector4::operator %)

        .Func<Vector4 (Vector4::*)(void) const>(_SC("opUnPlus"), &Vector4::operator +)
        .Func<Vector4 (Vector4::*)(void) const>(_SC("opUnMinus"), &Vector4::operator -)

        .Func<bool (Vector4::*)(const Vector4 &) const>(_SC("opEqual"), &Vector4::operator ==)
        .Func<bool (Vector4::*)(const Vector4 &) const>(_SC("opNotEqual"), &Vector4::operator !=)
        .Func<bool (Vector4::*)(const Vector4 &) const>(_SC("opLessThan"), &Vector4::operator <)
        .Func<bool (Vector4::*)(const Vector4 &) const>(_SC("opGreaterThan"), &Vector4::operator >)
        .Func<bool (Vector4::*)(const Vector4 &) const>(_SC("opLessEqual"), &Vector4::operator <=)
        .Func<bool (Vector4::*)(const Vector4 &) const>(_SC("opGreaterEqual"), &Vector4::operator >=)
    );

    LogDbg("Registration of <Vector4> type was successful");

    return true;
}

} // Namespace:: SqMod
