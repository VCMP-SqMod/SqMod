// ------------------------------------------------------------------------------------------------
#include "Base/Circle.hpp"
#include "Base/Shared.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Circle Circle::NIL = Circle();
const Circle Circle::MIN = Circle(0.0);
const Circle Circle::MAX = Circle(std::numeric_limits< Circle::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Circle::Delim = ',';

// ------------------------------------------------------------------------------------------------
SQInteger Circle::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("Circle");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Circle::Circle()
    : pos(0.0, 0.0), rad(0.0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Circle::Circle(Value rv)
    : pos(0.0, 0.0), rad(rv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Circle::Circle(const Vector2 & pv, Value rv)
    : pos(pv), rad(rv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Circle::Circle(Value xv, Value yv, Value rv)
    : pos(xv, yv), rad(rv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator = (Value r)
{
    rad = r;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator = (const Vector2 & p)
{
    pos = p;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator += (const Circle & c)
{
    pos += c.pos;
    rad += c.rad;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator -= (const Circle & c)
{
    pos -= c.pos;
    rad -= c.rad;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator *= (const Circle & c)
{
    pos *= c.pos;
    rad *= c.rad;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator /= (const Circle & c)
{
    pos /= c.pos;
    rad /= c.rad;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator %= (const Circle & c)
{
    pos %= c.pos;
    rad = std::fmod(rad, c.rad);

    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator += (Value r)
{
    rad += r;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator -= (Value r)
{
    rad -= r;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator *= (Value r)
{
    rad *= r;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator /= (Value r)
{
    rad /= r;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator %= (Value r)
{
    rad = std::fmod(rad, r);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator += (const Vector2 & p)
{
    pos += p;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator -= (const Vector2 & p)
{
    pos -= p;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator *= (const Vector2 & p)
{
    pos *= p;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator /= (const Vector2 & p)
{
    pos /= p;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator %= (const Vector2 & p)
{
    pos %= p;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator ++ ()
{
    ++pos;
    ++rad;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator -- ()
{
    --pos;
    --rad;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator ++ (int)
{
    Circle state(*this);
    ++pos;
    ++rad;
    return state;
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator -- (int)
{
    Circle state(*this);
    --pos;
    --rad;
    return state;
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator + (const Circle & c) const
{
    return Circle(pos + c.pos, rad + c.rad);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator - (const Circle & c) const
{
    return Circle(pos - c.pos, rad - c.rad);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator * (const Circle & c) const
{
    return Circle(pos * c.pos, rad * c.rad);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator / (const Circle & c) const
{
    return Circle(pos / c.pos, rad / c.rad);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator % (const Circle & c) const
{
    return Circle(pos % c.pos, std::fmod(rad, c.rad));
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator + (Value r) const
{
    return Circle(rad + r);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator - (Value r) const
{
    return Circle(rad - r);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator * (Value r) const
{
    return Circle(rad * r);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator / (Value r) const
{
    return Circle(rad / r);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator % (Value r) const
{
    return Circle(std::fmod(rad, r));
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator + (const Vector2 & p) const
{
    return Circle(pos + p, rad);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator - (const Vector2 & p) const
{
    return Circle(pos - p, rad);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator * (const Vector2 & p) const
{
    return Circle(pos * p, rad);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator / (const Vector2 & p) const
{
    return Circle(pos / p, rad);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator % (const Vector2 & p) const
{
    return Circle(pos % p, rad);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator + () const
{
    return Circle(pos.Abs(), std::fabs(rad));
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator - () const
{
    return Circle(-pos, -rad);
}

// ------------------------------------------------------------------------------------------------
bool Circle::operator == (const Circle & c) const
{
    return EpsEq(rad, c.rad) && (pos == c.pos);
}

// ------------------------------------------------------------------------------------------------
bool Circle::operator != (const Circle & c) const
{
    return !EpsEq(rad, c.rad) && (pos != c.pos);
}

// ------------------------------------------------------------------------------------------------
bool Circle::operator < (const Circle & c) const
{
    return EpsLt(rad, c.rad) && (pos < c.pos);
}

// ------------------------------------------------------------------------------------------------
bool Circle::operator > (const Circle & c) const
{
    return EpsGt(rad, c.rad) && (pos > c.pos);
}

// ------------------------------------------------------------------------------------------------
bool Circle::operator <= (const Circle & c) const
{
    return EpsLtEq(rad, c.rad) && (pos <= c.pos);
}

// ------------------------------------------------------------------------------------------------
bool Circle::operator >= (const Circle & c) const
{
    return EpsGtEq(rad, c.rad) && (pos >= c.pos);
}

// ------------------------------------------------------------------------------------------------
Int32 Circle::Cmp(const Circle & o) const
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
CSStr Circle::ToString() const
{
    return ToStrF("%f,%f,%f", pos.x, pos.y, rad);
}

// ------------------------------------------------------------------------------------------------
void Circle::SetRadius(Value nr)
{
    rad = nr;
}

// ------------------------------------------------------------------------------------------------
void Circle::SetCircle(const Circle & nc)
{
    pos = nc.pos;
    rad = nc.rad;
}

// ------------------------------------------------------------------------------------------------
void Circle::SetCircleEx(Value nx, Value ny, Value nr)
{
    pos.SetVector2Ex(nx, ny);
    rad = nr;
}

// ------------------------------------------------------------------------------------------------
void Circle::SetValues(const Vector2 & np, Value nr)
{
    pos = np;
    rad = nr;
}

// ------------------------------------------------------------------------------------------------
void Circle::SetPosition(const Vector2 & np)
{
    pos = np;
}

// ------------------------------------------------------------------------------------------------
void Circle::SetPositionEx(Value nx, Value ny)
{
    pos.SetVector2Ex(nx, ny);
}

// ------------------------------------------------------------------------------------------------
void Circle::SetStr(CSStr values, SQChar delim)
{
    SetCircle(Circle::Get(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Circle::Generate()
{
    pos.Generate();
    rad = GetRandomFloat32();
}

// ------------------------------------------------------------------------------------------------
void Circle::Generate(Value min, Value max, bool r)
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
void Circle::Generate(Value xmin, Value xmax, Value ymin, Value ymax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin))
    {
        STHROWF("max value is lower than min value");
    }

    pos.Generate(xmin, xmax, ymin, ymax);
}

// ------------------------------------------------------------------------------------------------
void Circle::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value rmin, Value rmax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin) || EpsLt(rmax, rmin))
    {
        STHROWF("max value is lower than min value");
    }

    pos.Generate(xmin, xmax, ymin, ymax);
    rad = GetRandomFloat32(rmin, rmax);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::Abs() const
{
    return Circle(pos.Abs(), std::fabs(rad));
}

// ------------------------------------------------------------------------------------------------
const Circle & Circle::Get(CSStr str)
{
    return Circle::Get(str, Circle::Delim);
}

// ------------------------------------------------------------------------------------------------
const Circle & Circle::Get(CSStr str, SQChar delim)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %f , %f , %f ");
    static Circle circle;
    // Clear previous values, if any
    circle.Clear();
    // Is the specified string empty?
    if (!str || *str == '\0')
    {
        return circle; // Return the value as is!
    }
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str, fs, &circle.pos.x, &circle.pos.y, &circle.rad);
    // Return the resulted value
    return circle;
}

// ------------------------------------------------------------------------------------------------
const Circle & GetCircle()
{
    static Circle circle;
    circle.Clear();
    return circle;
}

// ------------------------------------------------------------------------------------------------
const Circle & GetCircle(Float32 rv)
{
    static Circle circle;
    circle.SetRadius(rv);
    return circle;
}

// ------------------------------------------------------------------------------------------------
const Circle & GetCircle(const Vector2 & pv, Float32 rv)
{
    static Circle circle;
    circle.SetValues(pv, rv);
    return circle;
}

// ------------------------------------------------------------------------------------------------
const Circle & GetCircle(Float32 xv, Float32 yv, Float32 rv)
{
    static Circle circle;
    circle.SetCircleEx(xv, yv, rv);
    return circle;
}

// ------------------------------------------------------------------------------------------------
const Circle & GetCircle(const Circle & o)
{
    static Circle circle;
    circle.SetCircle(o);
    return circle;
}

// ================================================================================================
void Register_Circle(HSQUIRRELVM vm)
{
    typedef Circle::Value Val;

    RootTable(vm).Bind(_SC("Circle"), Class< Circle >(vm, _SC("Circle"))
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< const Vector2 &, Val >()
        .Ctor< Val, Val, Val >()
        // Member Variables
        .Var(_SC("pos"), &Circle::pos)
        .Var(_SC("rad"), &Circle::rad)
        .Var(_SC("Pos"), &Circle::pos)
        .Var(_SC("Rad"), &Circle::rad)
        // Core Meta-methods
        .Func(_SC("_tostring"), &Circle::ToString)
        .SquirrelFunc(_SC("_typename"), &Circle::Typename)
        .Func(_SC("_cmp"), &Circle::Cmp)
        // Meta-methods
        .Func< Circle (Circle::*)(const Circle &) const >(_SC("_add"), &Circle::operator +)
        .Func< Circle (Circle::*)(const Circle &) const >(_SC("_sub"), &Circle::operator -)
        .Func< Circle (Circle::*)(const Circle &) const >(_SC("_mul"), &Circle::operator *)
        .Func< Circle (Circle::*)(const Circle &) const >(_SC("_div"), &Circle::operator /)
        .Func< Circle (Circle::*)(const Circle &) const >(_SC("_modulo"), &Circle::operator %)
        .Func< Circle (Circle::*)(void) const >(_SC("_unm"), &Circle::operator -)
        // Properties
        .Prop(_SC("Abs"), &Circle::Abs)
        // Member Methods
        .Func(_SC("SetRadius"), &Circle::SetRadius)
        .Func(_SC("SetCircle"), &Circle::SetCircle)
        .Func(_SC("SetCircleEx"), &Circle::SetCircleEx)
        .Func(_SC("SetValues"), &Circle::SetValues)
        .Func(_SC("SetPos"), &Circle::SetPosition)
        .Func(_SC("SetPosition"), &Circle::SetPosition)
        .Func(_SC("SetPosEx"), &Circle::SetPositionEx)
        .Func(_SC("SetPositionEx"), &Circle::SetPositionEx)
        .Func(_SC("SetStr"), &Circle::SetStr)
        .Func(_SC("Clear"), &Circle::Clear)
        // Member Overloads
        .Overload< void (Circle::*)(void) >(_SC("Generate"), &Circle::Generate)
        .Overload< void (Circle::*)(Val, Val, bool) >(_SC("Generate"), &Circle::Generate)
        .Overload< void (Circle::*)(Val, Val, Val, Val) >(_SC("Generate"), &Circle::Generate)
        .Overload< void (Circle::*)(Val, Val, Val, Val, Val, Val) >(_SC("Generate"), &Circle::Generate)
        // Static Overloads
        .StaticOverload< const Circle & (*)(CSStr) >(_SC("FromStr"), &Circle::Get)
        .StaticOverload< const Circle & (*)(CSStr, SQChar) >(_SC("FromStr"), &Circle::Get)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Circle >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Circle >)
        // Operator Exposure
        .Func< Circle & (Circle::*)(const Circle &) >(_SC("opAddAssign"), &Circle::operator +=)
        .Func< Circle & (Circle::*)(const Circle &) >(_SC("opSubAssign"), &Circle::operator -=)
        .Func< Circle & (Circle::*)(const Circle &) >(_SC("opMulAssign"), &Circle::operator *=)
        .Func< Circle & (Circle::*)(const Circle &) >(_SC("opDivAssign"), &Circle::operator /=)
        .Func< Circle & (Circle::*)(const Circle &) >(_SC("opModAssign"), &Circle::operator %=)
        .Func< Circle & (Circle::*)(Circle::Value) >(_SC("opAddAssignR"), &Circle::operator +=)
        .Func< Circle & (Circle::*)(Circle::Value) >(_SC("opSubAssignR"), &Circle::operator -=)
        .Func< Circle & (Circle::*)(Circle::Value) >(_SC("opMulAssignR"), &Circle::operator *=)
        .Func< Circle & (Circle::*)(Circle::Value) >(_SC("opDivAssignR"), &Circle::operator /=)
        .Func< Circle & (Circle::*)(Circle::Value) >(_SC("opModAssignR"), &Circle::operator %=)
        .Func< Circle & (Circle::*)(const Vector2 &) >(_SC("opAddAssignP"), &Circle::operator +=)
        .Func< Circle & (Circle::*)(const Vector2 &) >(_SC("opSubAssignP"), &Circle::operator -=)
        .Func< Circle & (Circle::*)(const Vector2 &) >(_SC("opMulAssignP"), &Circle::operator *=)
        .Func< Circle & (Circle::*)(const Vector2 &) >(_SC("opDivAssignP"), &Circle::operator /=)
        .Func< Circle & (Circle::*)(const Vector2 &) >(_SC("opModAssignP"), &Circle::operator %=)
        .Func< Circle & (Circle::*)(void) >(_SC("opPreInc"), &Circle::operator ++)
        .Func< Circle & (Circle::*)(void) >(_SC("opPreDec"), &Circle::operator --)
        .Func< Circle (Circle::*)(int) >(_SC("opPostInc"), &Circle::operator ++)
        .Func< Circle (Circle::*)(int) >(_SC("opPostDec"), &Circle::operator --)
        .Func< Circle (Circle::*)(const Circle &) const >(_SC("opAdd"), &Circle::operator +)
        .Func< Circle (Circle::*)(const Circle &) const >(_SC("opSub"), &Circle::operator -)
        .Func< Circle (Circle::*)(const Circle &) const >(_SC("opMul"), &Circle::operator *)
        .Func< Circle (Circle::*)(const Circle &) const >(_SC("opDiv"), &Circle::operator /)
        .Func< Circle (Circle::*)(const Circle &) const >(_SC("opMod"), &Circle::operator %)
        .Func< Circle (Circle::*)(Circle::Value) const >(_SC("opAddR"), &Circle::operator +)
        .Func< Circle (Circle::*)(Circle::Value) const >(_SC("opSubR"), &Circle::operator -)
        .Func< Circle (Circle::*)(Circle::Value) const >(_SC("opMulR"), &Circle::operator *)
        .Func< Circle (Circle::*)(Circle::Value) const >(_SC("opDivR"), &Circle::operator /)
        .Func< Circle (Circle::*)(Circle::Value) const >(_SC("opModR"), &Circle::operator %)
        .Func< Circle (Circle::*)(const Vector2 &) const >(_SC("opAddP"), &Circle::operator +)
        .Func< Circle (Circle::*)(const Vector2 &) const >(_SC("opSubP"), &Circle::operator -)
        .Func< Circle (Circle::*)(const Vector2 &) const >(_SC("opMulP"), &Circle::operator *)
        .Func< Circle (Circle::*)(const Vector2 &) const >(_SC("opDivP"), &Circle::operator /)
        .Func< Circle (Circle::*)(const Vector2 &) const >(_SC("opModP"), &Circle::operator %)
        .Func< Circle (Circle::*)(void) const >(_SC("opUnPlus"), &Circle::operator +)
        .Func< Circle (Circle::*)(void) const >(_SC("opUnMinus"), &Circle::operator -)
        .Func< bool (Circle::*)(const Circle &) const >(_SC("opEqual"), &Circle::operator ==)
        .Func< bool (Circle::*)(const Circle &) const >(_SC("opNotEqual"), &Circle::operator !=)
        .Func< bool (Circle::*)(const Circle &) const >(_SC("opLessThan"), &Circle::operator <)
        .Func< bool (Circle::*)(const Circle &) const >(_SC("opGreaterThan"), &Circle::operator >)
        .Func< bool (Circle::*)(const Circle &) const >(_SC("opLessEqual"), &Circle::operator <=)
        .Func< bool (Circle::*)(const Circle &) const >(_SC("opGreaterEqual"), &Circle::operator >=)
    );
}

} // Namespace:: SqMod
