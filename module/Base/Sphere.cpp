// ------------------------------------------------------------------------------------------------
#include "Base/Sphere.hpp"
#include "Base/DynArg.hpp"
#include "Core/Buffer.hpp"
#include "Core/Utility.hpp"
#include "Library/JSON.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("Sphere"))

// ------------------------------------------------------------------------------------------------
const Sphere Sphere::NIL = Sphere();
const Sphere Sphere::MIN = Sphere(0.0);
const Sphere Sphere::MAX = Sphere(std::numeric_limits< Sphere::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Sphere::Delim = ',';

// ------------------------------------------------------------------------------------------------
Sphere::Sphere(Value rv) noexcept
    : pos(0.0), rad(rv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Sphere::Sphere(const Vector3 & pv, Value rv) noexcept
    : pos(pv), rad(rv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Sphere::Sphere(Value xv, Value yv, Value zv, Value rv) noexcept
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
    rad = fmodf(rad, s.rad);

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
    rad = fmodf(rad, r);
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
Sphere Sphere::operator ++ (int) // NOLINT(cert-dcl21-cpp)
{
    Sphere state(*this);
    ++pos;
    ++rad;
    return state;
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator -- (int) // NOLINT(cert-dcl21-cpp)
{
    Sphere state(*this);
    --pos;
    --rad;
    return state;
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator + (const Sphere & s) const
{
    return {pos + s.pos, rad + s.rad};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator - (const Sphere & s) const
{
    return {pos - s.pos, rad - s.rad};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator * (const Sphere & s) const
{
    return {pos * s.pos, rad * s.rad};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator / (const Sphere & s) const
{
    return {pos / s.pos, rad / s.rad};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator % (const Sphere & s) const
{
    return {pos % s.pos, fmodf(rad, s.rad)};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator + (Value r) const
{
    return {rad + r};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator - (Value r) const
{
    return {rad - r};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator * (Value r) const
{
    return {rad * r};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator / (Value r) const
{
    return {rad / r};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator % (Value r) const
{
    return {fmodf(rad, r)};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator + (const Vector3 & p) const
{
    return {pos + p, rad};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator - (const Vector3 & p) const
{
    return {pos - p, rad};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator * (const Vector3 & p) const
{
    return {pos * p, rad};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator / (const Vector3 & p) const
{
    return {pos / p, rad};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator % (const Vector3 & p) const
{
    return {pos % p, rad};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator + () const
{
    return {pos.Abs(), fabsf(rad)};
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::operator - () const
{
    return {-pos, -rad};
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
int32_t Sphere::Cmp(const Sphere & o) const
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
String Sphere::ToString() const
{
    return fmt::format("{},{},{},{}", pos.x, pos.y, pos.z, rad);
}

// ------------------------------------------------------------------------------------------------
void Sphere::ToJSON(CtxJSON & ctx) const
{
    if (ctx.mObjectOverArray)
    {
        fmt::format_to(std::back_inserter(ctx.mOutput), "{{x:{},y:{},z:{},r:{}}},", pos.x, pos.y, pos.z, rad);
    }
    else
    {
        fmt::format_to(std::back_inserter(ctx.mOutput), "[{},{},{},{}],", pos.x, pos.y, pos.z, rad);
    }
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
void Sphere::SetStr(SQChar delim, StackStrF & values)
{
    SetSphere(Sphere::GetEx(delim, values));
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::Generate()
{
    pos.Generate();
    rad = GetRandomFloat32();
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::GenerateB(Value min, Value max, bool r)
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
        pos.GenerateB(min, max);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::GenerateR(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin) || EpsLt(zmax, zmin))
    {
        STHROWF("max value is lower than min value");
    }

    pos.GenerateR(xmin, xmax, ymin, ymax, zmin, zmax);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::GenerateR2(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value rmin, Value rmax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin) || EpsLt(zmax, zmin) || EpsLt(rmax, rmin))
    {
        STHROWF("max value is lower than min value");
    }

    pos.GenerateR(xmin, xmax, ymin, ymax, zmin, zmax);
    rad = GetRandomFloat32(rmin, rmax);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::Abs() const
{
    return {pos.Abs(), fabsf(rad)};
}

// ------------------------------------------------------------------------------------------------
String Sphere::Format(StackStrF & str) const
{
    return fmt::format(fmt::runtime(str.ToStr())
        , fmt::arg("x", pos.x)
        , fmt::arg("y", pos.y)
        , fmt::arg("z", pos.z)
        , fmt::arg("r", rad)
    );
}

// ------------------------------------------------------------------------------------------------
const Sphere & Sphere::Get(StackStrF & str)
{
    return Sphere::GetEx(Sphere::Delim, str);
}

// ------------------------------------------------------------------------------------------------
const Sphere & Sphere::GetEx(SQChar delim, StackStrF & str)
{
    static Sphere sphere;
    // Clear previous values, if any
    sphere.Clear();
    // Is the specified string empty?
    if (str.mLen <= 0)
    {
        return sphere; // Return the value as is!
    }
    // The format specifications that will be used to scan the string
    SQChar fs[] = _SC(" %f , %f , %f , %f ");
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    fs[14] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str.mPtr, fs, &sphere.pos.x, &sphere.pos.y, &sphere.pos.z, &sphere.rad);
    // Return the resulted value
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
        .Func(_SC("_tojson"), &Sphere::ToJSON)
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
        .FmtFunc(_SC("Format"), &Sphere::Format)
        // Member Overloads
        .Overload(_SC("Generate"), &Sphere::Generate)
        .Overload(_SC("Generate"), &Sphere::GenerateB)
        .Overload(_SC("Generate"), &Sphere::GenerateR)
        .Overload(_SC("Generate"), &Sphere::GenerateR2)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Sphere >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Sphere >)
        .StaticFmtFunc(_SC("FromStr"), &Sphere::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Sphere::GetEx)
    );
}

} // Namespace:: SqMod