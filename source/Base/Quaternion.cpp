#include "Base/Quaternion.hpp"
#include "Base/Vector3.hpp"
#include "Base/Vector4.hpp"
#include "Base/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Quaternion Quaternion::NIL = Quaternion(0);
const Quaternion Quaternion::MIN = Quaternion(std::numeric_limits<Quaternion::Value>::min());
const Quaternion Quaternion::MAX = Quaternion(std::numeric_limits<Quaternion::Value>::max());

// ------------------------------------------------------------------------------------------------
SQChar Quaternion::Delim = ',';

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion()
    : x(0.0), y(0.0), z(0.0), w(0.0)
{

}

Quaternion::Quaternion(Value s)
    : x(s), y(s), z(s), w(s)
{

}

Quaternion::Quaternion(Value xv, Value yv, Value zv)
    : x(xv), y(yv), z(zv), w(0.0)
{

}

Quaternion::Quaternion(Value xv, Value yv, Value zv, Value wv)
    : x(xv), y(yv), z(zv), w(wv)
{

}

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion(const Vector3 & v)
    : x(v.x), y(v.y), z(v.z), w(0.0)
{

}

Quaternion::Quaternion(const Vector4 & v)
    : x(v.x), y(v.y), z(v.z), w(v.w)
{

}

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion(const SQChar * values, SQChar delim)
    : Quaternion(GetQuaternion(values, delim))
{

}

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion(const Quaternion & q)
    : x(q.x), y(q.y), z(q.z), w(q.w)
{

}

Quaternion::Quaternion(Quaternion && q)
    : x(q.x), y(q.y), z(q.z), w(q.w)
{

}

// ------------------------------------------------------------------------------------------------
Quaternion::~Quaternion()
{

}

// ------------------------------------------------------------------------------------------------
Quaternion & Quaternion::operator = (const Quaternion & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
    return *this;
}

Quaternion & Quaternion::operator = (Quaternion && q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
    return *this;
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

Quaternion & Quaternion::operator = (const Vector3 & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = 0.0;
    return *this;
}

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

Quaternion & Quaternion::operator -= (const Quaternion & q)
{
    x -= q.x;
    y -= q.y;
    z -= q.z;
    w -= q.w;
    return *this;
}

Quaternion & Quaternion::operator *= (const Quaternion & q)
{
    x *= q.x;
    y *= q.y;
    z *= q.z;
    w *= q.w;
    return *this;
}

Quaternion & Quaternion::operator /= (const Quaternion & q)
{
    x /= q.x;
    y /= q.y;
    z /= q.z;
    w /= q.w;
    return *this;
}

Quaternion & Quaternion::operator %= (const Quaternion & q)
{
    x = std::fmod(x, q.x);
    y = std::fmod(y, q.y);
    z = std::fmod(z, q.z);
    w = std::fmod(w, q.w);
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

Quaternion & Quaternion::operator -= (Value s)
{
    x -= s;
    y -= s;
    z -= s;
    w -= s;
    return *this;
}

Quaternion & Quaternion::operator *= (Value s)
{
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}

Quaternion & Quaternion::operator /= (Value s)
{
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
}

Quaternion & Quaternion::operator %= (Value s)
{
    x = std::fmod(x, s);
    y = std::fmod(y, s);
    z = std::fmod(z, s);
    w = std::fmod(w, s);
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

Quaternion & Quaternion::operator -- ()
{
    --x;
    --y;
    --z;
    --w;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator ++ (int)
{
    Quaternion state(*this);
    ++x;
    ++y;
    ++z;
    ++w;
    return state;
}

Quaternion Quaternion::operator -- (int)
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
    return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

Quaternion Quaternion::operator + (Value s) const
{
    return Quaternion(x + s, y + s, z + s, w + s);
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator - (const Quaternion & q) const
{
    return Quaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

Quaternion Quaternion::operator - (Value s) const
{
    return Quaternion(x - s, y - s, z - s, w - s);
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator * (const Quaternion & q) const
{
    return Quaternion(x * q.x, y * q.y, z * q.z, w * q.w);
}

Quaternion Quaternion::operator * (Value s) const
{
    return Quaternion(x * s, y * s, z * s, w * s);
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator / (const Quaternion & q) const
{
    return Quaternion(x / q.x, y / q.y, z / q.z, w / q.w);
}

Quaternion Quaternion::operator / (Value s) const
{
    return Quaternion(x / s, y / s, z / s, w / s);
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator % (const Quaternion & q) const
{
    return Quaternion(std::fmod(x, q.x), std::fmod(y, q.y), std::fmod(z, q.z), std::fmod(w, q.w));
}

Quaternion Quaternion::operator % (Value s) const
{
    return Quaternion(std::fmod(x, s), std::fmod(y, s), std::fmod(z, s), std::fmod(w, s));
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator + () const
{
    return Quaternion(std::fabs(x), std::fabs(y), std::fabs(z), std::fabs(w));
}

Quaternion Quaternion::operator - () const
{
    return Quaternion(-x, -y, -z, -w);
}

// ------------------------------------------------------------------------------------------------
bool Quaternion::operator == (const Quaternion & q) const
{
    return EpsEq(x, q.x) && EpsEq(y, q.y) && EpsEq(z, q.z) && EpsEq(w, q.w);
}

bool Quaternion::operator != (const Quaternion & q) const
{
    return !EpsEq(x, q.x) && !EpsEq(y, q.y) && !EpsEq(z, q.z) && !EpsEq(w, q.w);
}

bool Quaternion::operator < (const Quaternion & q) const
{
    return std::isless(x, q.x) && std::isless(y, q.y) && std::isless(z, q.z) && std::isless(w, q.w);
}

bool Quaternion::operator > (const Quaternion & q) const
{
    return std::isgreater(x, q.x) && std::isgreater(y, q.y) && std::isgreater(z, q.z) && std::isgreater(w, q.w);
}

bool Quaternion::operator <= (const Quaternion & q) const
{
    return std::islessequal(x, q.x) && std::islessequal(y, q.y) && std::islessequal(z, q.z) && std::islessequal(w, q.w);
}

bool Quaternion::operator >= (const Quaternion & q) const
{
    return std::isgreaterequal(x, q.x) && std::isgreaterequal(y, q.y) && std::isgreaterequal(z, q.z) && std::isgreaterequal(w, q.w);
}

// ------------------------------------------------------------------------------------------------
SQInteger Quaternion::Cmp(const Quaternion & q) const
{
    return *this == q ? 0 : (*this > q ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Quaternion::ToString() const
{
    return ToStringF("%f,%f,%f,%f", x, y, z, w);
}

// ------------------------------------------------------------------------------------------------
void Quaternion::Set(Value ns)
{
    x = ns;
    y = ns;
    z = ns;
    w = ns;
}

void Quaternion::Set(Value nx, Value ny, Value nz)
{
    x = nx;
    y = ny;
    z = nz;
}

void Quaternion::Set(Value nx, Value ny, Value nz, Value nw)
{
    x = nx;
    y = ny;
    z = nz;
    w = nw;
}

// ------------------------------------------------------------------------------------------------
void Quaternion::Set(const Quaternion & q)
{
    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;
}

void Quaternion::Set(const Vector3 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 0.0;
}

void Quaternion::Set(const Vector4 & v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

// ------------------------------------------------------------------------------------------------
void Quaternion::Set(const SQChar * values, SQChar delim)
{
    Set(GetQuaternion(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Quaternion::Generate()
{
    x = RandomVal<Value>::Get();
    y = RandomVal<Value>::Get();
    z = RandomVal<Value>::Get();
    w = RandomVal<Value>::Get();
}

void Quaternion::Generate(Value min, Value max)
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

void Quaternion::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value wmin, Value wmax)
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
Quaternion Quaternion::Abs() const
{
    return Quaternion(std::fabs(x), std::fabs(y), std::fabs(z), std::fabs(w));
}

// ================================================================================================
bool Register_Quaternion(HSQUIRRELVM vm)
{
    LogDbg("Beginning registration of <Quaternion> type");

    typedef Quaternion::Value Val;

    Sqrat::RootTable(vm).Bind(_SC("Quaternion"), Sqrat::Class<Quaternion>(vm, _SC("Quaternion"))
        .Ctor()
        .Ctor<Val>()
        .Ctor<Val, Val, Val>()
        .Ctor<Val, Val, Val, Val>()
        .Ctor<const SQChar *, SQChar>()

        .SetStaticValue(_SC("delim"), &Quaternion::Delim)

        .Var(_SC("x"), &Quaternion::x)
        .Var(_SC("y"), &Quaternion::y)
        .Var(_SC("z"), &Quaternion::z)
        .Var(_SC("w"), &Quaternion::w)

        .Prop(_SC("abs"), &Quaternion::Abs)

        .Func(_SC("_tostring"), &Quaternion::ToString)
        .Func(_SC("_cmp"), &Quaternion::Cmp)

        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("_add"), &Quaternion::operator +)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("_sub"), &Quaternion::operator -)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("_mul"), &Quaternion::operator *)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("_div"), &Quaternion::operator /)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("_modulo"), &Quaternion::operator %)
        .Func<Quaternion (Quaternion::*)(void) const>(_SC("_unm"), &Quaternion::operator -)

        .Overload<void (Quaternion::*)(Val)>(_SC("set"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(Val, Val, Val)>(_SC("set"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(Val, Val, Val, Val)>(_SC("set"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(const Quaternion &)>(_SC("set_quat"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(const Vector3 &)>(_SC("set_vec3"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(const Vector4 &)>(_SC("set_vec4"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(const SQChar *, SQChar)>(_SC("set_str"), &Quaternion::Set)

        .Overload<void (Quaternion::*)(void)>(_SC("generate"), &Quaternion::Generate)
        .Overload<void (Quaternion::*)(Val, Val)>(_SC("generate"), &Quaternion::Generate)
        .Overload<void (Quaternion::*)(Val, Val, Val, Val, Val, Val, Val, Val)>(_SC("generate"), &Quaternion::Generate)

        .Func(_SC("clear"), &Quaternion::Clear)

        .Func<Quaternion & (Quaternion::*)(const Quaternion &)>(_SC("opAddAssign"), &Quaternion::operator +=)
        .Func<Quaternion & (Quaternion::*)(const Quaternion &)>(_SC("opSubAssign"), &Quaternion::operator -=)
        .Func<Quaternion & (Quaternion::*)(const Quaternion &)>(_SC("opMulAssign"), &Quaternion::operator *=)
        .Func<Quaternion & (Quaternion::*)(const Quaternion &)>(_SC("opDivAssign"), &Quaternion::operator /=)
        .Func<Quaternion & (Quaternion::*)(const Quaternion &)>(_SC("opModAssign"), &Quaternion::operator %=)

        .Func<Quaternion & (Quaternion::*)(Quaternion::Value)>(_SC("opAddAssignS"), &Quaternion::operator +=)
        .Func<Quaternion & (Quaternion::*)(Quaternion::Value)>(_SC("opSubAssignS"), &Quaternion::operator -=)
        .Func<Quaternion & (Quaternion::*)(Quaternion::Value)>(_SC("opMulAssignS"), &Quaternion::operator *=)
        .Func<Quaternion & (Quaternion::*)(Quaternion::Value)>(_SC("opDivAssignS"), &Quaternion::operator /=)
        .Func<Quaternion & (Quaternion::*)(Quaternion::Value)>(_SC("opModAssignS"), &Quaternion::operator %=)

        .Func<Quaternion & (Quaternion::*)(void)>(_SC("opPreInc"), &Quaternion::operator ++)
        .Func<Quaternion & (Quaternion::*)(void)>(_SC("opPreDec"), &Quaternion::operator --)
        .Func<Quaternion (Quaternion::*)(int)>(_SC("opPostInc"), &Quaternion::operator ++)
        .Func<Quaternion (Quaternion::*)(int)>(_SC("opPostDec"), &Quaternion::operator --)

        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("opAdd"), &Quaternion::operator +)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("opSub"), &Quaternion::operator -)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("opMul"), &Quaternion::operator *)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("opDiv"), &Quaternion::operator /)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("opMod"), &Quaternion::operator %)

        .Func<Quaternion (Quaternion::*)(Quaternion::Value) const>(_SC("opAddS"), &Quaternion::operator +)
        .Func<Quaternion (Quaternion::*)(Quaternion::Value) const>(_SC("opSubS"), &Quaternion::operator -)
        .Func<Quaternion (Quaternion::*)(Quaternion::Value) const>(_SC("opMulS"), &Quaternion::operator *)
        .Func<Quaternion (Quaternion::*)(Quaternion::Value) const>(_SC("opDivS"), &Quaternion::operator /)
        .Func<Quaternion (Quaternion::*)(Quaternion::Value) const>(_SC("opModS"), &Quaternion::operator %)

        .Func<Quaternion (Quaternion::*)(void) const>(_SC("opUnPlus"), &Quaternion::operator +)
        .Func<Quaternion (Quaternion::*)(void) const>(_SC("opUnMinus"), &Quaternion::operator -)

        .Func<bool (Quaternion::*)(const Quaternion &) const>(_SC("opEqual"), &Quaternion::operator ==)
        .Func<bool (Quaternion::*)(const Quaternion &) const>(_SC("opNotEqual"), &Quaternion::operator !=)
        .Func<bool (Quaternion::*)(const Quaternion &) const>(_SC("opLessThan"), &Quaternion::operator <)
        .Func<bool (Quaternion::*)(const Quaternion &) const>(_SC("opGreaterThan"), &Quaternion::operator >)
        .Func<bool (Quaternion::*)(const Quaternion &) const>(_SC("opLessEqual"), &Quaternion::operator <=)
        .Func<bool (Quaternion::*)(const Quaternion &) const>(_SC("opGreaterEqual"), &Quaternion::operator >=)
    );

    LogDbg("Registration of <Quaternion> type was successful");

    return true;
}

} // Namespace:: SqMod
