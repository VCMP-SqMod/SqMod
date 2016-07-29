// ------------------------------------------------------------------------------------------------
#include "Base/AABB.hpp"
#include "Base/Vector4.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const AABB AABB::NIL = AABB(0);
const AABB AABB::MIN = AABB(-1, -1, -1, 1, 1, 1);
const AABB AABB::MAX = AABB(Vector3::MIN, Vector3::MAX);

// ------------------------------------------------------------------------------------------------
SQChar AABB::Delim = ',';

// ------------------------------------------------------------------------------------------------
SQInteger AABB::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("AABB");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
AABB::AABB()
    : min(-1.0), max(1.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
AABB::AABB(Value sv)
    : min(-sv), max(std::fabs(sv))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
AABB::AABB(Value xv, Value yv, Value zv)
    : min(-xv, -yv, -zv), max(std::fabs(xv), std::fabs(yv), std::fabs(zv))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
AABB::AABB(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax)
    : min(xmin, ymin, zmin), max(xmax, ymax, zmax)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
AABB::AABB(const Vector3 & vmin, const Vector3 & vmax)
    : min(vmin), max(vmax)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator = (Value s)
{
    min.SetScalar(-s);
    max.SetScalar(std::fabs(s));
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator = (const Vector3 & v)
{
    min.SetVector3(-v);
    max.SetVector3(v.Abs());
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator = (const Vector4 & v)
{
    min.SetVector4(-v);
    max.SetVector4(v.Abs());
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator += (const AABB & b)
{
    min += b.min;
    max += b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator -= (const AABB & b)
{
    min -= b.min;
    max -= b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator *= (const AABB & b)
{
    min *= b.min;
    max *= b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator /= (const AABB & b)
{
    min /= b.min;
    max /= b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator %= (const AABB & b)
{
    min %= b.min;
    max %= b.max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator += (Value s)
{
    min += s;
    max += s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator -= (Value s)
{
    min -= s;
    max -= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator *= (Value s)
{
    min *= s;
    max *= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator /= (Value s)
{
    min /= s;
    max /= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator %= (Value s)
{
    min %= s;
    max %= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator ++ ()
{
    ++min;
    ++max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB & AABB::operator -- ()
{
    --min;
    --max;
    return *this;
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator ++ (int)
{
    AABB state(*this);
    ++min;
    ++max;
    return state;
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator -- (int)
{
    AABB state(*this);
    --min;
    --max;
    return state;
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator + (const AABB & b) const
{
    return AABB(min + b.min, max + b.max);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator - (const AABB & b) const
{
    return AABB(min - b.min, max - b.max);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator * (const AABB & b) const
{
    return AABB(min * b.min, max * b.max);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator / (const AABB & b) const
{
    return AABB(min / b.min, max / b.max);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator % (const AABB & b) const
{
    return AABB(min % b.min, max % b.max);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator + (Value s) const
{
    return AABB(min + s, max + s);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator - (Value s) const
{
    return AABB(min - s, max - s);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator * (Value s) const
{
    return AABB(min * s, max * s);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator / (Value s) const
{
    return AABB(min / s, max / s);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator % (Value s) const
{
    return AABB(min % s, max % s);
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator + () const
{
    return AABB(min.Abs(), max.Abs());
}

// ------------------------------------------------------------------------------------------------
AABB AABB::operator - () const
{
    return AABB(-min, -max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator == (const AABB & b) const
{
    return (min == b.min) && (max == b.max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator != (const AABB & b) const
{
    return (min != b.min) || (max != b.max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator < (const AABB & b) const
{
    return (min < b.min) && (max < b.max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator > (const AABB & b) const
{
    return (min > b.min) && (max > b.max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator <= (const AABB & b) const
{
    return (min <= b.min) && (max <= b.max);
}

// ------------------------------------------------------------------------------------------------
bool AABB::operator >= (const AABB & b) const
{
    return (min >= b.min) && (max >= b.max);
}

// ------------------------------------------------------------------------------------------------
Int32 AABB::Cmp(const AABB & o) const
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
CSStr AABB::ToString() const
{
    return ToStrF("%f,%f,%f,%f,%f,%f", min.x, min.y, min.z, max.x, max.y, max.z);
}

// ------------------------------------------------------------------------------------------------
void AABB::Set(Value ns)
{
    min = -ns;
    max = std::fabs(ns);
}

// ------------------------------------------------------------------------------------------------
void AABB::Set(Value nx, Value ny, Value nz)
{
    min.SetVector3Ex(-nx, -ny, -nz);
    max.SetVector3Ex(std::fabs(nx), std::fabs(ny), std::fabs(nz));
}

// ------------------------------------------------------------------------------------------------
void AABB::Set(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax)
{
    min.SetVector3Ex(xmin, ymin, zmin);
    max.SetVector3Ex(xmax, ymax, zmax);
}

// ------------------------------------------------------------------------------------------------
void AABB::SetAABB(const AABB & b)
{
    min = b.min;
    max = b.max;
}

// ------------------------------------------------------------------------------------------------
void AABB::SetVector3(const Vector3 & v)
{
    min = -v;
    max = v.Abs();
}

// ------------------------------------------------------------------------------------------------
void AABB::SetVector3(const Vector3 & nmin, const Vector3 & nmax)
{
    min = nmin;
    max = nmax;
}

// ------------------------------------------------------------------------------------------------
void AABB::SetVector4(const Vector4 & v)
{
    min = -v;
    max = v.Abs();
}

// ------------------------------------------------------------------------------------------------
void AABB::SetVector4(const Vector4 & nmin, const Vector4 & nmax)
{
    min = nmin;
    max = nmax;
}

// ------------------------------------------------------------------------------------------------
void AABB::SetStr(CSStr values, SQChar delim)
{
    SetAABB(AABB::Get(values, delim));
}

// ------------------------------------------------------------------------------------------------
AABB AABB::Abs() const
{
    return AABB(min.Abs(), max.Abs());
}

// ------------------------------------------------------------------------------------------------
const AABB & AABB::Get(CSStr str)
{
    return AABB::Get(str, AABB::Delim);
}

// ------------------------------------------------------------------------------------------------
const AABB & AABB::Get(CSStr str, SQChar delim)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %f , %f , %f , %f , %f , %f ");
    static AABB box;
    // Clear previous values, if any
    box.Clear();
    // Is the specified string empty?
    if (!str || *str == '\0')
    {
        return box; // Return the value as is!
    }
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    fs[14] = delim;
    fs[19] = delim;
    fs[24] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str, fs, &box.min.x, &box.min.y, &box.min.z, &box.max.x, &box.max.y, &box.max.z);
    // Return the resulted value
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB()
{
    static AABB box;
    box.Clear();
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB(Float32 sv)
{
    static AABB box;
    box.Set(sv);
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB(Float32 xv, Float32 yv, Float32 zv)
{
    static AABB box;
    box.Set(xv, yv, zv);
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB(Float32 xmin, Float32 ymin, Float32 zmin, Float32 xmax, Float32 ymax, Float32 zmax)
{
    static AABB box;
    box.Set(xmin, ymin, zmin, xmax, ymax, zmax);
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB(const Vector3 & vmin, const Vector3 & vmax)
{
    static AABB box;
    box.SetVector3(vmin, vmax);
    return box;
}

// ------------------------------------------------------------------------------------------------
const AABB & GetAABB(const AABB & o)
{
    static AABB box;
    box.SetAABB(o);
    return box;
}

// ================================================================================================
void Register_AABB(HSQUIRRELVM vm)
{
    typedef AABB::Value Val;

    RootTable(vm).Bind(_SC("AABB"), Class< AABB >(vm, _SC("AABB"))
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val, Val >()
        .Ctor< Val, Val, Val, Val, Val, Val >()
        .Ctor< const Vector3 &, const Vector3 & >()
        // Member Variables
        .Var(_SC("min"), &AABB::min)
        .Var(_SC("max"), &AABB::max)
        .Var(_SC("Min"), &AABB::min)
        .Var(_SC("Max"), &AABB::max)
        // Core Meta-methods
        .Func(_SC("_tostring"), &AABB::ToString)
        .SquirrelFunc(_SC("_typename"), &AABB::Typename)
        .Func(_SC("_cmp"), &AABB::Cmp)
        // Meta-methods
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("_add"), &AABB::operator +)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("_sub"), &AABB::operator -)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("_mul"), &AABB::operator *)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("_div"), &AABB::operator /)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("_modulo"), &AABB::operator %)
        .Func< AABB (AABB::*)(void) const >(_SC("_unm"), &AABB::operator -)
        // Properties
        .Prop(_SC("Abs"), &AABB::Abs)
        // Member Methods
        .Func(_SC("SetAABB"), &AABB::SetAABB)
        .Func(_SC("SetStr"), &AABB::SetStr)
        .Func(_SC("Clear"), &AABB::Clear)
        // Member Overloads
        .Overload< void (AABB::*)(Val) >(_SC("Set"), &AABB::Set)
        .Overload< void (AABB::*)(Val, Val, Val) >(_SC("Set"), &AABB::Set)
        .Overload< void (AABB::*)(Val, Val, Val, Val, Val, Val) >(_SC("Set"), &AABB::Set)
        .Overload< void (AABB::*)(const Vector3 &) >(_SC("SetVector3"), &AABB::SetVector3)
        .Overload< void (AABB::*)(const Vector3 &, const Vector3 &) >(_SC("SetVector3"), &AABB::SetVector3)
        .Overload< void (AABB::*)(const Vector4 &) >(_SC("SetVector4"), &AABB::SetVector4)
        .Overload< void (AABB::*)(const Vector4 &, const Vector4 &) >(_SC("SetVector4"), &AABB::SetVector4)
        // Static Overloads
        .StaticOverload< const AABB & (*)(CSStr) >(_SC("FromStr"), &AABB::Get)
        .StaticOverload< const AABB & (*)(CSStr, SQChar) >(_SC("FromStr"), &AABB::Get)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< AABB >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< AABB >)
        // Operator Exposure
        .Func< AABB & (AABB::*)(const AABB &) >(_SC("opAddAssign"), &AABB::operator +=)
        .Func< AABB & (AABB::*)(const AABB &) >(_SC("opSubAssign"), &AABB::operator -=)
        .Func< AABB & (AABB::*)(const AABB &) >(_SC("opMulAssign"), &AABB::operator *=)
        .Func< AABB & (AABB::*)(const AABB &) >(_SC("opDivAssign"), &AABB::operator /=)
        .Func< AABB & (AABB::*)(const AABB &) >(_SC("opModAssign"), &AABB::operator %=)
        .Func< AABB & (AABB::*)(AABB::Value) >(_SC("opAddAssignS"), &AABB::operator +=)
        .Func< AABB & (AABB::*)(AABB::Value) >(_SC("opSubAssignS"), &AABB::operator -=)
        .Func< AABB & (AABB::*)(AABB::Value) >(_SC("opMulAssignS"), &AABB::operator *=)
        .Func< AABB & (AABB::*)(AABB::Value) >(_SC("opDivAssignS"), &AABB::operator /=)
        .Func< AABB & (AABB::*)(AABB::Value) >(_SC("opModAssignS"), &AABB::operator %=)
        .Func< AABB & (AABB::*)(void) >(_SC("opPreInc"), &AABB::operator ++)
        .Func< AABB & (AABB::*)(void) >(_SC("opPreDec"), &AABB::operator --)
        .Func< AABB (AABB::*)(int) >(_SC("opPostInc"), &AABB::operator ++)
        .Func< AABB (AABB::*)(int) >(_SC("opPostDec"), &AABB::operator --)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("opAdd"), &AABB::operator +)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("opSub"), &AABB::operator -)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("opMul"), &AABB::operator *)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("opDiv"), &AABB::operator /)
        .Func< AABB (AABB::*)(const AABB &) const >(_SC("opMod"), &AABB::operator %)
        .Func< AABB (AABB::*)(AABB::Value) const >(_SC("opAddS"), &AABB::operator +)
        .Func< AABB (AABB::*)(AABB::Value) const >(_SC("opSubS"), &AABB::operator -)
        .Func< AABB (AABB::*)(AABB::Value) const >(_SC("opMulS"), &AABB::operator *)
        .Func< AABB (AABB::*)(AABB::Value) const >(_SC("opDivS"), &AABB::operator /)
        .Func< AABB (AABB::*)(AABB::Value) const >(_SC("opModS"), &AABB::operator %)
        .Func< AABB (AABB::*)(void) const >(_SC("opUnPlus"), &AABB::operator +)
        .Func< AABB (AABB::*)(void) const >(_SC("opUnMinus"), &AABB::operator -)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opEqual"), &AABB::operator ==)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opNotEqual"), &AABB::operator !=)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opLessThan"), &AABB::operator <)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opGreaterThan"), &AABB::operator >)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opLessEqual"), &AABB::operator <=)
        .Func< bool (AABB::*)(const AABB &) const >(_SC("opGreaterEqual"), &AABB::operator >=)
    );
}

} // Namespace:: SqMod
