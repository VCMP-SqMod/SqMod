// ------------------------------------------------------------------------------------------------
#include "Base/Circle.hpp"
#include "Base/DynArg.hpp"
#include "Core/Buffer.hpp"
#include "Core/Utility.hpp"
#include "Library/JSON.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("Circle"))

// ------------------------------------------------------------------------------------------------
const Circle Circle::NIL = Circle();
const Circle Circle::MIN = Circle(0.0);
const Circle Circle::MAX = Circle(std::numeric_limits< Circle::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Circle::Delim = ',';

// ------------------------------------------------------------------------------------------------
Circle::Circle(Value rv) noexcept
    : pos(0.0, 0.0), rad(rv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Circle::Circle(const Vector2 & pv, Value rv) noexcept
    : pos(pv), rad(rv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Circle::Circle(Value xv, Value yv, Value rv) noexcept
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
    rad = fmodf(rad, c.rad);

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
    rad = fmodf(rad, r);
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
Circle Circle::operator ++ (int) // NOLINT(cert-dcl21-cpp)
{
    Circle state(*this);
    ++pos;
    ++rad;
    return state;
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator -- (int) // NOLINT(cert-dcl21-cpp)
{
    Circle state(*this);
    --pos;
    --rad;
    return state;
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator + (const Circle & c) const
{
    return {pos + c.pos, rad + c.rad};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator - (const Circle & c) const
{
    return {pos - c.pos, rad - c.rad};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator * (const Circle & c) const
{
    return {pos * c.pos, rad * c.rad};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator / (const Circle & c) const
{
    return {pos / c.pos, rad / c.rad};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator % (const Circle & c) const
{
    return {pos % c.pos, fmodf(rad, c.rad)};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator + (Value r) const
{
    return {rad + r};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator - (Value r) const
{
    return {rad - r};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator * (Value r) const
{
    return {rad * r};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator / (Value r) const
{
    return {rad / r};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator % (Value r) const
{
    return {fmodf(rad, r)};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator + (const Vector2 & p) const
{
    return {pos + p, rad};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator - (const Vector2 & p) const
{
    return {pos - p, rad};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator * (const Vector2 & p) const
{
    return {pos * p, rad};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator / (const Vector2 & p) const
{
    return {pos / p, rad};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator % (const Vector2 & p) const
{
    return {pos % p, rad};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator + () const
{
    return {pos.Abs(), fabsf(rad)};
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator - () const
{
    return {-pos, -rad};
}

// ------------------------------------------------------------------------------------------------
bool Circle::operator == (const Circle & c) const
{
    return EpsEq(rad, c.rad) && (pos == c.pos);
}

// ------------------------------------------------------------------------------------------------
bool Circle::operator != (const Circle & c) const
{
    return !EpsEq(rad, c.rad) || (pos != c.pos);
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
int32_t Circle::Cmp(const Circle & o) const
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
String Circle::ToString() const
{
    return fmt::format("{},{},{}", pos.x, pos.y, rad);
}

// ------------------------------------------------------------------------------------------------
void Circle::ToJSON(CtxJSON & ctx) const
{
    if (ctx.mObjectOverArray)
    {
        fmt::format_to(std::back_inserter(ctx.mOutput), "{{x:{},y:{},r:{}}},", pos.x, pos.y, rad);
    }
    else
    {
        fmt::format_to(std::back_inserter(ctx.mOutput), "[{},{},{}],", pos.x, pos.y, rad);
    }
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
void Circle::SetStr(SQChar delim, StackStrF & values)
{
    SetCircle(Circle::GetEx(delim, values));
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::Generate()
{
    pos.Generate();
    rad = GetRandomFloat32();
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::GenerateB(Value min, Value max, bool r)
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
Circle & Circle::GenerateR(Value xmin, Value xmax, Value ymin, Value ymax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin))
    {
        STHROWF("max value is lower than min value");
    }

    pos.GenerateR(xmin, xmax, ymin, ymax);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::GenerateR2(Value xmin, Value xmax, Value ymin, Value ymax, Value rmin, Value rmax)
{
    if (EpsLt(xmax, xmin) || EpsLt(ymax, ymin) || EpsLt(rmax, rmin))
    {
        STHROWF("max value is lower than min value");
    }

    pos.GenerateR(xmin, xmax, ymin, ymax);
    rad = GetRandomFloat32(rmin, rmax);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle Circle::Abs() const
{
    return {pos.Abs(), fabsf(rad)};
}

// ------------------------------------------------------------------------------------------------
Array Circle::ToPointsArray(SQInteger num_segments) const
{
    // Allocate an array with the same amount of elements as the number of segments
    Array arr(SqVM());
    // Iterate the specified segments array
    arr.AppendFromCounted([this](HSQUIRRELVM vm, SQInteger i, SQInteger num_segments) -> bool {
        if (i >= num_segments) return false;
        // Get the current angle
#ifdef SQUSEDOUBLE
        SQFloat theta = 2.0 * SQMOD_PI64 * static_cast< SQFloat >(i) / static_cast< SQFloat >(num_segments);
#else
        SQFloat theta = 2.0f * SQMOD_PI * static_cast< SQFloat >(i) / static_cast< SQFloat >(num_segments);
#endif // SQUSEDOUBLE
        // Calculate the x component
        SQFloat x = (rad * std::cos(theta)) + pos.x;
        // Calculate the y component
        SQFloat y = (rad * std::sin(theta)) + pos.y;
        // Push the Vector2 instance on the stack
        Var< Vector2 >::push(vm, Vector2{static_cast< Vector2::Value >(x), static_cast< Vector2::Value >(y)});
        // Insert the element on the stack into the array
        return true;
    }, num_segments);
    // Return the resulted array
    return arr;
}

// ------------------------------------------------------------------------------------------------
String Circle::Format(StackStrF & str) const
{
    return fmt::format(fmt::runtime(str.ToStr())
        , fmt::arg("x", pos.x)
        , fmt::arg("y", pos.y)
        , fmt::arg("r", rad)
    );
}

// ------------------------------------------------------------------------------------------------
const Circle & Circle::Get(StackStrF & str)
{
    return Circle::GetEx(Circle::Delim, str);
}

// ------------------------------------------------------------------------------------------------
const Circle & Circle::GetEx(SQChar delim, StackStrF & str)
{
    static Circle circle;
    // Clear previous values, if any
    circle.Clear();
    // Is the specified string empty?
    if (str.mLen <= 0)
    {
        return circle; // Return the value as is!
    }
    // The format specifications that will be used to scan the string
    SQChar fs[] = _SC(" %f , %f , %f ");
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    // Attempt to extract the component values from the specified string
    std::sscanf(str.mPtr, fs, &circle.pos.x, &circle.pos.y, &circle.rad);
    // Return the resulted value
    return circle;
}

// ================================================================================================
void Register_Circle(HSQUIRRELVM vm)
{
    typedef Circle::Value Val;

    RootTable(vm).Bind(Typename::Str,
        Class< Circle >(vm, Typename::Str)
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
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< Circle >, SQFloat, SQInteger, bool, std::nullptr_t, Circle >)
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Circle::ToString)
        .Func(_SC("_toJSON"), &Circle::ToJSON)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< Circle >, SQFloat, SQInteger, bool, std::nullptr_t, Circle >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< Circle >, SQFloat, SQInteger, bool, std::nullptr_t, Circle >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< Circle >, SQFloat, SQInteger, bool, std::nullptr_t, Circle >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< Circle >, SQFloat, SQInteger, bool, std::nullptr_t, Circle >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< Circle >, SQFloat, SQInteger, bool, std::nullptr_t, Circle >)
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
        .FmtFunc(_SC("SetStr"), &Circle::SetStr)
        .Func(_SC("Clear"), &Circle::Clear)
        .FmtFunc(_SC("Format"), &Circle::Format)
        .Func(_SC("ToPointsArray"), &Circle::ToPointsArray)
        // Member Overloads
        .Overload(_SC("Generate"), &Circle::Generate)
        .Overload(_SC("Generate"), &Circle::GenerateB)
        .Overload(_SC("Generate"), &Circle::GenerateR)
        .Overload(_SC("Generate"), &Circle::GenerateR2)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Circle >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Circle >)
        .StaticFmtFunc(_SC("FromStr"), &Circle::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Circle::GetEx)
    );
}

} // Namespace:: SqMod
