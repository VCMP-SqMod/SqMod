// ------------------------------------------------------------------------------------------------
#include "Base/Sphere.hpp"
#include "Base/Shared.hpp"
#include "Base/DynArg.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("Sphere"))

// ------------------------------------------------------------------------------------------------
const Sphere Sphere::NIL = Sphere();
const Sphere Sphere::MIN = Sphere(0.0);
const Sphere Sphere::MAX = Sphere(std::numeric_limits< Sphere::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Sphere::Delim = ',';

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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator -= (const Sphere & s)
{
    pos -= s.pos;
    rad -= s.rad;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator *= (const Sphere & s)
{
    pos *= s.pos;
    rad *= s.rad;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator /= (const Sphere & s)
{
    pos /= s.pos;
    rad /= s.rad;
    return *this;
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator -= (Value r)
{
    rad -= r;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator *= (Value r)
{
    rad *= r;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator /= (Value r)
{
    rad /= r;
    return *this;
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator -= (const Vector3 & p)
{
    pos -= p;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator *= (const Vector3 & p)
{
    pos *= p;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator /= (const Vector3 & p)
{
    pos /= p;
    return *this;
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator - (const Sphere & s) const
{
    return Sphere(pos - s.pos, rad - s.rad);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator * (const Sphere & s) const
{
    return Sphere(pos * s.pos, rad * s.rad);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator / (const Sphere & s) const
{
    return Sphere(pos / s.pos, rad / s.rad);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator % (const Sphere & s) const
{
    return Sphere(pos % s.pos, std::fmod(rad, s.rad));
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator + (Value r) const
{
    return Sphere(rad + r);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator - (Value r) const
{
    return Sphere(rad - r);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator * (Value r) const
{
    return Sphere(rad * r);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator / (Value r) const
{
    return Sphere(rad / r);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator % (Value r) const
{
    return Sphere(std::fmod(rad, r));
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator + (const Vector3 & p) const
{
    return Sphere(pos + p, rad);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator - (const Vector3 & p) const
{
    return Sphere(pos - p, rad);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator * (const Vector3 & p) const
{
    return Sphere(pos * p, rad);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator / (const Vector3 & p) const
{
    return Sphere(pos / p, rad);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator % (const Vector3 & p) const
{
    return Sphere(pos % p, rad);
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator + () const
{
    return Sphere(pos.Abs(), std::fabs(rad));
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator - () const
{
    return Sphere(-pos, -rad);
}

// ------------------------------------------------------------------------------------------------
bool Sphere::operator == (const Sphere & s) const
{
    return EpsEq(rad, s.rad) && (pos == s.pos);
}

// ------------------------------------------------------------------------------------------------
bool Sphere::operator != (const Sphere & s) const
{
    return !EpsEq(rad, s.rad) || (pos != s.pos);
}

// ------------------------------------------------------------------------------------------------
bool Sphere::operator < (const Sphere & s) const
{
    return EpsLt(rad, s.rad) && (pos < s.pos);
}

// ------------------------------------------------------------------------------------------------
bool Sphere::operator > (const Sphere & s) const
{
    return EpsGt(rad, s.rad) && (pos > s.pos);
}

// ------------------------------------------------------------------------------------------------
bool Sphere::operator <= (const Sphere & s) const
{
    return EpsLtEq(rad, s.rad) && (pos <= s.pos);
}

// ------------------------------------------------------------------------------------------------
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
void Sphere::SetRadius(Value nr)
{
    rad = nr;
}

// ------------------------------------------------------------------------------------------------
void Sphere::SetSphere(const Sphere & ns)
{
    pos = ns.pos;
    rad = ns.rad;
}

// ------------------------------------------------------------------------------------------------
void Sphere::SetSphereEx(Value nx, Value ny, Value nz, Value nr)
{
    pos.SetVector3Ex(nx, ny, nz);
    rad = nr;
}

// ------------------------------------------------------------------------------------------------
void Sphere::SetValues(const Vector3 & np, Value nr)
{
    pos = np;
    rad = nr;
}

// ------------------------------------------------------------------------------------------------
void Sphere::SetPosition(const Vector3 & np)
{
    pos = np;
}

// ------------------------------------------------------------------------------------------------
void Sphere::SetPositionEx(Value nx, Value ny, Value nz)
{
    pos.SetVector3Ex(nx, ny, nz);
}

// ------------------------------------------------------------------------------------------------
void Sphere::SetStr(SQChar delim, const StackStrF & values)
{
    SetSphere(Sphere::GetEx(delim, values));
}

// ------------------------------------------------------------------------------------------------
void Sphere::Generate()
{
    pos.Generate();
    rad = GetRandomFloat32();
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
void Sphere::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin) || EpsLt(zmax, zmin))
    {
        STHROWF("max value is lower than min value");
    }

    pos.Generate(xmin, xmax, ymin, ymax, zmin, zmax);
}

// ------------------------------------------------------------------------------------------------
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
const Sphere & Sphere::Get(const StackStrF & str)
{
    return Sphere::GetEx(Sphere::Delim, str);
}

// ------------------------------------------------------------------------------------------------
const Sphere & Sphere::GetEx(SQChar delim, const StackStrF & str)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %f , %f , %f , %f ");
    static Sphere sphere;
    // Clear previous values, if any
    sphere.Clear();
    // Is the specified string empty?
    if (str.mLen <= 0)
    {
        return sphere; // Return the value as is!
    }
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    fs[14] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str.mPtr, fs, &sphere.pos.x, &sphere.pos.y, &sphere.pos.z, &sphere.rad);
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

// ------------------------------------------------------------------------------------------------
const Sphere & GetSphere(Float32 rv)
{
    static Sphere sphere;
    sphere.SetRadius(rv);
    return sphere;
}

// ------------------------------------------------------------------------------------------------
const Sphere & GetSphere(const Vector3 & pv, Float32 rv)
{
    static Sphere sphere;
    sphere.SetValues(pv, rv);
    return sphere;
}

// ------------------------------------------------------------------------------------------------
const Sphere & GetSphere(Float32 xv, Float32 yv, Float32 zv, Float32 rv)
{
    static Sphere sphere;
    sphere.SetSphereEx(xv, yv, zv, rv);
    return sphere;
}

// ------------------------------------------------------------------------------------------------
const Sphere & GetSphere(const Sphere & o)
{
    static Sphere sphere;
    sphere.SetSphere(o);
    return sphere;
}

// ================================================================================================
void Register_Sphere(HSQUIRRELVM vm)
{
    typedef Sphere::Value Val;

    RootTable(vm).Bind(Typename::Str,
        Class< Sphere >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< const Vector3 &, Val >()
        .Ctor< Val, Val, Val, Val >()
        // Member Variables
        .Var(_SC("pos"), &Sphere::pos)
        .Var(_SC("rad"), &Sphere::rad)
        .Var(_SC("Pos"), &Sphere::pos)
        .Var(_SC("Rad"), &Sphere::rad)
        // Core Meta-methods
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< Sphere >, SQFloat, SQInteger, bool, std::nullptr_t, Sphere >)
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Sphere::ToString)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< Sphere >, SQFloat, SQInteger, bool, std::nullptr_t, Sphere >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< Sphere >, SQFloat, SQInteger, bool, std::nullptr_t, Sphere >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< Sphere >, SQFloat, SQInteger, bool, std::nullptr_t, Sphere >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< Sphere >, SQFloat, SQInteger, bool, std::nullptr_t, Sphere >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< Sphere >, SQFloat, SQInteger, bool, std::nullptr_t, Sphere >)
        .Func< Sphere (Sphere::*)(void) const >(_SC("_unm"), &Sphere::operator -)
        // Properties
        .Prop(_SC("Abs"), &Sphere::Abs)
        // Member Methods
        .Func(_SC("SetRadius"), &Sphere::SetRadius)
        .Func(_SC("SetSphere"), &Sphere::SetSphere)
        .Func(_SC("SetSphereEx"), &Sphere::SetSphereEx)
        .Func(_SC("SetValues"), &Sphere::SetValues)
        .Func(_SC("SetPosition"), &Sphere::SetPosition)
        .Func(_SC("SetPositionEx"), &Sphere::SetPositionEx)
        .FmtFunc(_SC("SetStr"), &Sphere::SetStr)
        .Func(_SC("Clear"), &Sphere::Clear)
        // Member Overloads
        .Overload< void (Sphere::*)(void) >(_SC("Generate"), &Sphere::Generate)
        .Overload< void (Sphere::*)(Val, Val, bool) >(_SC("Generate"), &Sphere::Generate)
        .Overload< void (Sphere::*)(Val, Val, Val, Val, Val, Val) >(_SC("Generate"), &Sphere::Generate)
        .Overload< void (Sphere::*)(Val, Val, Val, Val, Val, Val, Val, Val) >(_SC("Generate"), &Sphere::Generate)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Sphere >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Sphere >)
        .StaticFmtFunc(_SC("FromStr"), &Sphere::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Sphere::GetEx)
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