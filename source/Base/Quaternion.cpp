// ------------------------------------------------------------------------------------------------
#include "Base/Quaternion.hpp"
#include "Base/Vector3.hpp"
#include "Base/Vector4.hpp"
#include "Base/Shared.hpp"
#include "Library/Random.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Quaternion Quaternion::NIL = Quaternion(0);
const Quaternion Quaternion::MIN = Quaternion(NumLimit< Quaternion::Value >::Min);
const Quaternion Quaternion::MAX = Quaternion(NumLimit< Quaternion::Value >::Max);

// ------------------------------------------------------------------------------------------------
SQChar Quaternion::Delim = ',';

// ------------------------------------------------------------------------------------------------
SQInteger Quaternion::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("Quaternion");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion()
    : x(0.0), y(0.0), z(0.0), w(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion(Value sv)
    : x(sv), y(sv), z(sv), w(sv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion(Value xv, Value yv, Value zv)
    : x(xv), y(yv), z(zv), w(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Quaternion::Quaternion(Value xv, Value yv, Value zv, Value wv)
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
    x = fmod(x, q.x);
    y = fmod(y, q.y);
    z = fmod(z, q.z);
    w = fmod(w, q.w);
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
    x = fmod(x, s);
    y = fmod(y, s);
    z = fmod(z, s);
    w = fmod(w, s);
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
    return Quaternion(fmod(x, q.x), fmod(y, q.y), fmod(z, q.z), fmod(w, q.w));
}

Quaternion Quaternion::operator % (Value s) const
{
    return Quaternion(fmod(x, s), fmod(y, s), fmod(z, s), fmod(w, s));
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::operator + () const
{
    return Quaternion(fabs(x), fabs(y), fabs(z), fabs(w));
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
    return EpsLt(x, q.x) && EpsLt(y, q.y) && EpsLt(z, q.z) && EpsLt(w, q.w);
}

bool Quaternion::operator > (const Quaternion & q) const
{
    return EpsGt(x, q.x) && EpsGt(y, q.y) && EpsGt(z, q.z) && EpsGt(w, q.w);
}

bool Quaternion::operator <= (const Quaternion & q) const
{
    return EpsLtEq(x, q.x) && EpsLtEq(y, q.y) && EpsLtEq(z, q.z) && EpsLtEq(w, q.w);
}

bool Quaternion::operator >= (const Quaternion & q) const
{
    return EpsGtEq(x, q.x) && EpsGtEq(y, q.y) && EpsGtEq(z, q.z) && EpsGtEq(w, q.w);
}

// ------------------------------------------------------------------------------------------------
Int32 Quaternion::Cmp(const Quaternion & o) const
{
    if (*this == o)
        return 0;
    else if (*this > o)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr Quaternion::ToString() const
{
    return ToStrF("%f,%f,%f,%f", x, y, z, w);
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
void Quaternion::Set(CSStr values, SQChar delim)
{
    Set(GetQuaternion(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Quaternion::Generate()
{
    x = GetRandomFloat32();
    y = GetRandomFloat32();
    z = GetRandomFloat32();
    w = GetRandomFloat32();
}

void Quaternion::Generate(Value min, Value max)
{
    if (EpsLt(max, min))
        SqThrowF("max value is lower than min value");

    x = GetRandomFloat32(min, max);
    y = GetRandomFloat32(min, max);
    z = GetRandomFloat32(min, max);
    y = GetRandomFloat32(min, max);
}

void Quaternion::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value wmin, Value wmax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin) || EpsLt(zmax, zmin) || EpsLt(wmax, wmin))
        SqThrowF("max value is lower than min value");

    x = GetRandomFloat32(xmin, xmax);
    y = GetRandomFloat32(ymin, ymax);
    z = GetRandomFloat32(zmin, zmax);
    y = GetRandomFloat32(ymin, ymax);
}

// ------------------------------------------------------------------------------------------------
Quaternion Quaternion::Abs() const
{
    return Quaternion(fabs(x), fabs(y), fabs(z), fabs(w));
}

// ================================================================================================
void Register_Quaternion(HSQUIRRELVM vm)
{
    typedef Quaternion::Value Val;

    RootTable(vm).Bind(_SC("Quaternion"), Class< Quaternion >(vm, _SC("Quaternion"))
        /* Constructors */
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val, Val >()
        .Ctor< Val, Val, Val, Val >()
        /* Static Members */
        .SetStaticValue(_SC("Delim"), &Quaternion::Delim)
        /* Member Variables */
        .Var(_SC("x"), &Quaternion::x)
        .Var(_SC("y"), &Quaternion::y)
        .Var(_SC("z"), &Quaternion::z)
        .Var(_SC("w"), &Quaternion::w)
        /* Properties */
        .Prop(_SC("abs"), &Quaternion::Abs)
        /* Core Metamethods */
        .Func(_SC("_tostring"), &Quaternion::ToString)
        .SquirrelFunc(_SC("_typename"), &Quaternion::Typename)
        .Func(_SC("_cmp"), &Quaternion::Cmp)
        /* Metamethods */
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("_add"), &Quaternion::operator +)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("_sub"), &Quaternion::operator -)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("_mul"), &Quaternion::operator *)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("_div"), &Quaternion::operator /)
        .Func<Quaternion (Quaternion::*)(const Quaternion &) const>(_SC("_modulo"), &Quaternion::operator %)
        .Func<Quaternion (Quaternion::*)(void) const>(_SC("_unm"), &Quaternion::operator -)
        /* Setters */
        .Overload<void (Quaternion::*)(Val)>(_SC("Set"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(Val, Val, Val)>(_SC("Set"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(Val, Val, Val, Val)>(_SC("Set"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(const Quaternion &)>(_SC("SetQuat"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(const Vector3 &)>(_SC("SetVec3"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(const Vector4 &)>(_SC("SetVec4"), &Quaternion::Set)
        .Overload<void (Quaternion::*)(CSStr, SQChar)>(_SC("SetStr"), &Quaternion::Set)
        /* Random Generators */
        .Overload<void (Quaternion::*)(void)>(_SC("Generate"), &Quaternion::Generate)
        .Overload<void (Quaternion::*)(Val, Val)>(_SC("Generate"), &Quaternion::Generate)
        .Overload<void (Quaternion::*)(Val, Val, Val, Val, Val, Val, Val, Val)>(_SC("Generate"), &Quaternion::Generate)
        /* Utility Methods */
        .Func(_SC("Clear"), &Quaternion::Clear)
        /* Operator Exposure */
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
}

} // Namespace:: SqMod