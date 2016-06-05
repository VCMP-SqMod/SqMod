// ------------------------------------------------------------------------------------------------
#include "Base/Sphere.hpp"
#include "Base/Shared.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Sphere Sphere::NIL = Sphere();
const Sphere Sphere::MIN = Sphere(0.0);
const Sphere Sphere::MAX = Sphere(std::numeric_limits< Sphere::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Sphere::Delim = ',';

// ------------------------------------------------------------------------------------------------
SQInteger Sphere::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("Sphere");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Sphere::Sphere()
    : pos(0.0), rad(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Sphere::Sphere(Value rv)
    : pos(0.0), rad(rv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Sphere::Sphere(const Vector3 & pv, Value rv)
    : pos(pv), rad(rv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Sphere::Sphere(Value xv, Value yv, Value zv, Value rv)
    : pos(xv, yv, zv), rad(rv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator = (Value r)
{
    rad = r;
    return *this;
}

Sphere & Sphere::operator = (const Vector3 & p)
{
    pos = p;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator += (const Sphere & s)
{
    pos += s.pos;
    rad += s.rad;
    return *this;
}

Sphere & Sphere::operator -= (const Sphere & s)
{
    pos -= s.pos;
    rad -= s.rad;
    return *this;
}

Sphere & Sphere::operator *= (const Sphere & s)
{
    pos *= s.pos;
    rad *= s.rad;
    return *this;
}

Sphere & Sphere::operator /= (const Sphere & s)
{
    pos /= s.pos;
    rad /= s.rad;
    return *this;
}

Sphere & Sphere::operator %= (const Sphere & s)
{
    pos %= s.pos;
    rad = std::fmod(rad, s.rad);

    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator += (Value r)
{
    rad += r;
    return *this;
}

Sphere & Sphere::operator -= (Value r)
{
    rad -= r;
    return *this;
}

Sphere & Sphere::operator *= (Value r)
{
    rad *= r;
    return *this;
}

Sphere & Sphere::operator /= (Value r)
{
    rad /= r;
    return *this;
}

Sphere & Sphere::operator %= (Value r)
{
    rad = std::fmod(rad, r);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator += (const Vector3 & p)
{
    pos += p;
    return *this;
}

Sphere & Sphere::operator -= (const Vector3 & p)
{
    pos -= p;
    return *this;
}

Sphere & Sphere::operator *= (const Vector3 & p)
{
    pos *= p;
    return *this;
}

Sphere & Sphere::operator /= (const Vector3 & p)
{
    pos /= p;
    return *this;
}

Sphere & Sphere::operator %= (const Vector3 & p)
{
    pos %= p;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator ++ ()
{
    ++pos;
    ++rad;
    return *this;
}

Sphere & Sphere::operator -- ()
{
    --pos;
    --rad;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator ++ (int)
{
    Sphere state(*this);
    ++pos;
    ++rad;
    return state;
}

Sphere Sphere::operator -- (int)
{
    Sphere state(*this);
    --pos;
    --rad;
    return state;
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator + (const Sphere & s) const
{
    return Sphere(pos + s.pos, rad + s.rad);
}

Sphere Sphere::operator - (const Sphere & s) const
{
    return Sphere(pos - s.pos, rad - s.rad);
}

Sphere Sphere::operator * (const Sphere & s) const
{
    return Sphere(pos * s.pos, rad * s.rad);
}

Sphere Sphere::operator / (const Sphere & s) const
{
    return Sphere(pos / s.pos, rad / s.rad);
}

Sphere Sphere::operator % (const Sphere & s) const
{
    return Sphere(pos % s.pos, std::fmod(rad, s.rad));
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator + (Value r) const
{
    return Sphere(rad + r);
}

Sphere Sphere::operator - (Value r) const
{
    return Sphere(rad - r);
}

Sphere Sphere::operator * (Value r) const
{
    return Sphere(rad * r);
}

Sphere Sphere::operator / (Value r) const
{
    return Sphere(rad / r);
}

Sphere Sphere::operator % (Value r) const
{
    return Sphere(std::fmod(rad, r));
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator + (const Vector3 & p) const
{
    return Sphere(pos + p, rad);
}

Sphere Sphere::operator - (const Vector3 & p) const
{
    return Sphere(pos - p, rad);
}

Sphere Sphere::operator * (const Vector3 & p) const
{
    return Sphere(pos * p, rad);
}

Sphere Sphere::operator / (const Vector3 & p) const
{
    return Sphere(pos / p, rad);
}

Sphere Sphere::operator % (const Vector3 & p) const
{
    return Sphere(pos % p, rad);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator + () const
{
    return Sphere(pos.Abs(), std::fabs(rad));
}

Sphere Sphere::operator - () const
{
    return Sphere(-pos, -rad);
}

// ------------------------------------------------------------------------------------------------
bool Sphere::operator == (const Sphere & s) const
{
    return EpsEq(rad, s.rad) && (pos == s.pos);
}

bool Sphere::operator != (const Sphere & s) const
{
    return !EpsEq(rad, s.rad) && (pos != s.pos);
}

bool Sphere::operator < (const Sphere & s) const
{
    return EpsLt(rad, s.rad) && (pos < s.pos);
}

bool Sphere::operator > (const Sphere & s) const
{
    return EpsGt(rad, s.rad) && (pos > s.pos);
}

bool Sphere::operator <= (const Sphere & s) const
{
    return EpsLtEq(rad, s.rad) && (pos <= s.pos);
}

bool Sphere::operator >= (const Sphere & s) const
{
    return EpsGtEq(rad, s.rad) && (pos >= s.pos);
}

// ------------------------------------------------------------------------------------------------
Int32 Sphere::Cmp(const Sphere & o) const
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
CSStr Sphere::ToString() const
{
    return ToStrF("%f,%f,%f,%f", pos.x, pos.y, pos.z, rad);
}

// ------------------------------------------------------------------------------------------------
void Sphere::Set(Value nr)
{
    rad = nr;
}

void Sphere::Set(const Sphere & ns)
{
    pos = ns.pos;
    rad = ns.rad;
}

void Sphere::Set(const Vector3 & np)
{
    pos = np;
}

void Sphere::Set(const Vector3 & np, Value nr)
{
    pos = np;
    rad = nr;
}

// ------------------------------------------------------------------------------------------------
void Sphere::Set(Value nx, Value ny, Value nz)
{
    pos.Set(nx, ny, nz);
}

void Sphere::Set(Value nx, Value ny, Value nz, Value nr)
{
    pos.Set(nx, ny, nz);
    rad = nr;
}

// ------------------------------------------------------------------------------------------------
void Sphere::Set(CSStr values, SQChar delim)
{
    Set(Sphere::Get(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Sphere::Generate()
{
    pos.Generate();
    rad = GetRandomFloat32();
}

void Sphere::Generate(Value min, Value max, bool r)
{
    if (EpsLt(max, min))
    {
        STHROWF("max value is lower than min value");
    }
    else if (r)
    {
        rad = GetRandomFloat32(min, max);
    }
    else
    {
        pos.Generate(min, max);
    }
}

void Sphere::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin) || EpsLt(zmax, zmin))
    {
        STHROWF("max value is lower than min value");
    }

    pos.Generate(xmin, xmax, ymin, ymax, zmin, zmax);
}

void Sphere::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value rmin, Value rmax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin) || EpsLt(zmax, zmin) || EpsLt(rmax, rmin))
    {
        STHROWF("max value is lower than min value");
    }

    pos.Generate(xmin, xmax, ymin, ymax, zmin, zmax);
    rad = GetRandomFloat32(rmin, rmax);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::Abs() const
{
    return Sphere(pos.Abs(), std::fabs(rad));
}

// ------------------------------------------------------------------------------------------------
const Sphere & Sphere::Get(CSStr str)
{
    return Sphere::Get(str, Sphere::Delim);
}

// ------------------------------------------------------------------------------------------------
const Sphere & Sphere::Get(CSStr str, SQChar delim)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %f , %f , %f , %f ");
    static Sphere sphere;
    // Clear previous values, if any
    sphere.Clear();
    // Is the specified string empty?
    if (!str || *str == '\0')
    {
        return sphere; // Return the value as is!
    }
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    fs[14] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str, fs, &sphere.pos.x, &sphere.pos.y, &sphere.pos.z, &sphere.rad);
    // Return the resulted value
    return sphere;
}

// ------------------------------------------------------------------------------------------------
const Sphere & GetSphere()
{
    static Sphere sphere;
    sphere.Clear();
    return sphere;
}

const Sphere & GetSphere(Float32 rv)
{
    static Sphere sphere;
    sphere.Set(rv);
    return sphere;
}

const Sphere & GetSphere(const Vector3 & pv, Float32 rv)
{
    static Sphere sphere;
    sphere.Set(pv, rv);
    return sphere;
}

const Sphere & GetSphere(Float32 xv, Float32 yv, Float32 zv, Float32 rv)
{
    static Sphere sphere;
    sphere.Set(xv, yv, zv, rv);
    return sphere;
}

const Sphere & GetSphere(const Sphere & o)
{
    static Sphere sphere;
    sphere.Set(o);
    return sphere;
}

// ================================================================================================
void Register_Sphere(HSQUIRRELVM vm)
{
    typedef Sphere::Value Val;

    RootTable(vm).Bind(_SC("Sphere"), Class< Sphere >(vm, _SC("Sphere"))
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< const Vector3 &, Val >()
        .Ctor< Val, Val, Val, Val >()
        // Static Members
        .SetStaticValue(_SC("Delim"), &Sphere::Delim)
        // Member Variables
        .Var(_SC("Pos"), &Sphere::pos)
        .Var(_SC("Rad"), &Sphere::rad)
        // Properties
        .Prop(_SC("Abs"), &Sphere::Abs)
        // Core Meta-methods
        .Func(_SC("_tostring"), &Sphere::ToString)
        .SquirrelFunc(_SC("_typename"), &Sphere::Typename)
        .Func(_SC("_cmp"), &Sphere::Cmp)
        // Meta-methods
        .Func< Sphere (Sphere::*)(const Sphere &) const >(_SC("_add"), &Sphere::operator +)
        .Func< Sphere (Sphere::*)(const Sphere &) const >(_SC("_sub"), &Sphere::operator -)
        .Func< Sphere (Sphere::*)(const Sphere &) const >(_SC("_mul"), &Sphere::operator *)
        .Func< Sphere (Sphere::*)(const Sphere &) const >(_SC("_div"), &Sphere::operator /)
        .Func< Sphere (Sphere::*)(const Sphere &) const >(_SC("_modulo"), &Sphere::operator %)
        .Func< Sphere (Sphere::*)(void) const >(_SC("_unm"), &Sphere::operator -)
        // Setters
        .Overload< void (Sphere::*)(const Sphere &) >(_SC("Set"), &Sphere::Set)
        .Overload< void (Sphere::*)(const Vector3 &, Val) >(_SC("Set"), &Sphere::Set)
        .Overload< void (Sphere::*)(Val, Val, Val, Val) >(_SC("Set"), &Sphere::Set)
        .Overload< void (Sphere::*)(Val) >(_SC("SetRad"), &Sphere::Set)
        .Overload< void (Sphere::*)(const Vector3 &) >(_SC("SetVec3"), &Sphere::Set)
        .Overload< void (Sphere::*)(Val, Val, Val) >(_SC("SetVec3"), &Sphere::Set)
        .Overload< void (Sphere::*)(CSStr, SQChar) >(_SC("SetStr"), &Sphere::Set)
        // Utility Methods
        .Func(_SC("Clear"), &Sphere::Clear)
        // Static Overloads
        .StaticOverload< const Sphere & (*)(CSStr) >(_SC("FromStr"), &Sphere::Get)
        .StaticOverload< const Sphere & (*)(CSStr, SQChar) >(_SC("FromStr"), &Sphere::Get)
        // Operator Exposure
        .Func< Sphere & (Sphere::*)(const Sphere &) >(_SC("opAddAssign"), &Sphere::operator +=)
        .Func< Sphere & (Sphere::*)(const Sphere &) >(_SC("opSubAssign"), &Sphere::operator -=)
        .Func< Sphere & (Sphere::*)(const Sphere &) >(_SC("opMulAssign"), &Sphere::operator *=)
        .Func< Sphere & (Sphere::*)(const Sphere &) >(_SC("opDivAssign"), &Sphere::operator /=)
        .Func< Sphere & (Sphere::*)(const Sphere &) >(_SC("opModAssign"), &Sphere::operator %=)

        .Func< Sphere & (Sphere::*)(Sphere::Value) >(_SC("opAddAssignR"), &Sphere::operator +=)
        .Func< Sphere & (Sphere::*)(Sphere::Value) >(_SC("opSubAssignR"), &Sphere::operator -=)
        .Func< Sphere & (Sphere::*)(Sphere::Value) >(_SC("opMulAssignR"), &Sphere::operator *=)
        .Func< Sphere & (Sphere::*)(Sphere::Value) >(_SC("opDivAssignR"), &Sphere::operator /=)
        .Func< Sphere & (Sphere::*)(Sphere::Value) >(_SC("opModAssignR"), &Sphere::operator %=)

        .Func< Sphere & (Sphere::*)(const Vector3 &) >(_SC("opAddAssignP"), &Sphere::operator +=)
        .Func< Sphere & (Sphere::*)(const Vector3 &) >(_SC("opSubAssignP"), &Sphere::operator -=)
        .Func< Sphere & (Sphere::*)(const Vector3 &) >(_SC("opMulAssignP"), &Sphere::operator *=)
        .Func< Sphere & (Sphere::*)(const Vector3 &) >(_SC("opDivAssignP"), &Sphere::operator /=)
        .Func< Sphere & (Sphere::*)(const Vector3 &) >(_SC("opModAssignP"), &Sphere::operator %=)

        .Func< Sphere & (Sphere::*)(void) >(_SC("opPreInc"), &Sphere::operator ++)
        .Func< Sphere & (Sphere::*)(void) >(_SC("opPreDec"), &Sphere::operator --)
        .Func< Sphere (Sphere::*)(int) >(_SC("opPostInc"), &Sphere::operator ++)
        .Func< Sphere (Sphere::*)(int) >(_SC("opPostDec"), &Sphere::operator --)

        .Func< Sphere (Sphere::*)(const Sphere &) const >(_SC("opAdd"), &Sphere::operator +)
        .Func< Sphere (Sphere::*)(const Sphere &) const >(_SC("opSub"), &Sphere::operator -)
        .Func< Sphere (Sphere::*)(const Sphere &) const >(_SC("opMul"), &Sphere::operator *)
        .Func< Sphere (Sphere::*)(const Sphere &) const >(_SC("opDiv"), &Sphere::operator /)
        .Func< Sphere (Sphere::*)(const Sphere &) const >(_SC("opMod"), &Sphere::operator %)

        .Func< Sphere (Sphere::*)(Sphere::Value) const >(_SC("opAddR"), &Sphere::operator +)
        .Func< Sphere (Sphere::*)(Sphere::Value) const >(_SC("opSubR"), &Sphere::operator -)
        .Func< Sphere (Sphere::*)(Sphere::Value) const >(_SC("opMulR"), &Sphere::operator *)
        .Func< Sphere (Sphere::*)(Sphere::Value) const >(_SC("opDivR"), &Sphere::operator /)
        .Func< Sphere (Sphere::*)(Sphere::Value) const >(_SC("opModR"), &Sphere::operator %)

        .Func< Sphere (Sphere::*)(const Vector3 &) const >(_SC("opAddP"), &Sphere::operator +)
        .Func< Sphere (Sphere::*)(const Vector3 &) const >(_SC("opSubP"), &Sphere::operator -)
        .Func< Sphere (Sphere::*)(const Vector3 &) const >(_SC("opMulP"), &Sphere::operator *)
        .Func< Sphere (Sphere::*)(const Vector3 &) const >(_SC("opDivP"), &Sphere::operator /)
        .Func< Sphere (Sphere::*)(const Vector3 &) const >(_SC("opModP"), &Sphere::operator %)

        .Func< Sphere (Sphere::*)(void) const >(_SC("opUnPlus"), &Sphere::operator +)
        .Func< Sphere (Sphere::*)(void) const >(_SC("opUnMinus"), &Sphere::operator -)

        .Func< bool (Sphere::*)(const Sphere &) const >(_SC("opEqual"), &Sphere::operator ==)
        .Func< bool (Sphere::*)(const Sphere &) const >(_SC("opNotEqual"), &Sphere::operator !=)
        .Func< bool (Sphere::*)(const Sphere &) const >(_SC("opLessThan"), &Sphere::operator <)
        .Func< bool (Sphere::*)(const Sphere &) const >(_SC("opGreaterThan"), &Sphere::operator >)
        .Func< bool (Sphere::*)(const Sphere &) const >(_SC("opLessEqual"), &Sphere::operator <=)
        .Func< bool (Sphere::*)(const Sphere &) const >(_SC("opGreaterEqual"), &Sphere::operator >=)
    );
}

} // Namespace:: SqMod
