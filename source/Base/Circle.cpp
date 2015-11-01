#include "Base/Circle.hpp"
#include "Base/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Circle Circle::NIL = Circle();
const Circle Circle::MIN = Circle(0.0);
const Circle Circle::MAX = Circle(std::numeric_limits<Circle::Value>::max());

// ------------------------------------------------------------------------------------------------
SQChar Circle::Delim = ',';

// ------------------------------------------------------------------------------------------------
Circle::Circle()
    : pos(0.0, 0.0), rad(0.0)
{

}

Circle::Circle(Value r)
    : pos(0.0, 0.0), rad(r)
{

}

Circle::Circle(const Vector2f & p)
    : pos(p), rad(0.0)
{

}

Circle::Circle(const Vector2f & p, Value r)
    : pos(p), rad(r)
{

}

Circle::Circle(Value x, Value y, Value r)
    : pos(x, y), rad(r)
{

}

// ------------------------------------------------------------------------------------------------
Circle::Circle(const Circle & c)
    : pos(c.pos), rad(c.rad)
{

}

Circle::Circle(Circle && c)
    : pos(c.pos), rad(c.rad)
{

}

// ------------------------------------------------------------------------------------------------
Circle::~Circle()
{

}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator = (const Circle & c)
{
    pos = c.pos;
    rad = c.rad;
    return *this;
}

Circle & Circle::operator = (Circle && c)
{
    pos = c.pos;
    rad = c.rad;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator = (Value r)
{
    rad = r;
    return *this;
}

Circle & Circle::operator = (const Vector2f & p)
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

Circle & Circle::operator -= (const Circle & c)
{
    pos -= c.pos;
    rad -= c.rad;
    return *this;
}

Circle & Circle::operator *= (const Circle & c)
{
    pos *= c.pos;
    rad *= c.rad;
    return *this;
}

Circle & Circle::operator /= (const Circle & c)
{
    pos /= c.pos;
    rad /= c.rad;
    return *this;
}

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

Circle & Circle::operator -= (Value r)
{
    rad -= r;
    return *this;
}

Circle & Circle::operator *= (Value r)
{
    rad *= r;
    return *this;
}

Circle & Circle::operator /= (Value r)
{
    rad /= r;
    return *this;
}

Circle & Circle::operator %= (Value r)
{
    rad = std::fmod(rad, r);
    return *this;
}

// ------------------------------------------------------------------------------------------------
Circle & Circle::operator += (const Vector2f & p)
{
    pos += p;
    return *this;
}

Circle & Circle::operator -= (const Vector2f & p)
{
    pos -= p;
    return *this;
}

Circle & Circle::operator *= (const Vector2f & p)
{
    pos *= p;
    return *this;
}

Circle & Circle::operator /= (const Vector2f & p)
{
    pos /= p;
    return *this;
}

Circle & Circle::operator %= (const Vector2f & p)
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

Circle Circle::operator - (const Circle & c) const
{
    return Circle(pos - c.pos, rad - c.rad);
}

Circle Circle::operator * (const Circle & c) const
{
    return Circle(pos * c.pos, rad * c.rad);
}

Circle Circle::operator / (const Circle & c) const
{
    return Circle(pos / c.pos, rad / c.rad);
}

Circle Circle::operator % (const Circle & c) const
{
    return Circle(pos % c.pos, std::fmod(rad, c.rad));
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator + (Value r) const
{
    return Circle(rad + r);
}

Circle Circle::operator - (Value r) const
{
    return Circle(rad - r);
}

Circle Circle::operator * (Value r) const
{
    return Circle(rad * r);
}

Circle Circle::operator / (Value r) const
{
    return Circle(rad / r);
}

Circle Circle::operator % (Value r) const
{
    return Circle(std::fmod(rad, r));
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator + (const Vector2f & p) const
{
    return Circle(pos + p);
}

Circle Circle::operator - (const Vector2f & p) const
{
    return Circle(pos - p);
}

Circle Circle::operator * (const Vector2f & p) const
{
    return Circle(pos * p);
}

Circle Circle::operator / (const Vector2f & p) const
{
    return Circle(pos / p);
}

Circle Circle::operator % (const Vector2f & p) const
{
    return Circle(pos % p);
}

// ------------------------------------------------------------------------------------------------
Circle Circle::operator + () const
{
    return Circle(pos.Abs(), std::fabs(rad));
}

Circle Circle::operator - () const
{
    return Circle(-pos, -rad);
}

// ------------------------------------------------------------------------------------------------
bool Circle::operator == (const Circle & c) const
{
    return (rad == c.rad) && (pos == c.pos);
}

bool Circle::operator != (const Circle & c) const
{
    return (rad != c.rad) && (pos != c.pos);
}

bool Circle::operator < (const Circle & c) const
{
    return (rad < c.rad) && (pos < c.pos);
}

bool Circle::operator > (const Circle & c) const
{
    return (rad > c.rad) && (pos > c.pos);
}

bool Circle::operator <= (const Circle & c) const
{
    return (rad <= c.rad) && (pos <= c.pos);
}

bool Circle::operator >= (const Circle & c) const
{
    return (rad >= c.rad) && (pos >= c.pos);
}

// ------------------------------------------------------------------------------------------------
SQInteger Circle::Cmp(const Circle & c) const
{
    return *this == c ? 0 : (*this > c ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Circle::ToString() const
{
    return ToStringF("%f,%f,%f", pos.x, pos.y, rad);
}

// ------------------------------------------------------------------------------------------------
void Circle::Set(Value nr)
{
    rad = nr;
}

void Circle::Set(const Circle & nc)
{
    pos = nc.pos;
    rad = nc.rad;
}

void Circle::Set(const Vector2f & np)
{
    pos = np;
}

void Circle::Set(const Vector2f & np, Value nr)
{
    pos = np;
    rad = nr;
}

// ------------------------------------------------------------------------------------------------
void Circle::Set(Value nx, Value ny)
{
    pos.Set(nx, ny);
}

void Circle::Set(Value nx, Value ny, Value nr)
{
    pos.Set(nx, ny);
    rad = nr;
}

// ------------------------------------------------------------------------------------------------
void Circle::Set(const SQChar * values, SQChar delim)
{
    Set(GetCircle(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Circle::Generate()
{
    pos.Generate();
    rad = RandomVal<Value>::Get();
}

void Circle::Generate(Value min, Value max, bool r)
{
    if (max < min)
    {
        LogErr("max value is lower than min value");
    }
    else if (r)
    {
        rad = RandomVal<Value>::Get(min, max);
    }
    else
    {
        pos.Generate(min, max);
    }
}

void Circle::Generate(Value xmin, Value xmax, Value ymin, Value ymax)
{
    pos.Generate(xmin, xmax, ymin, ymax);
}

void Circle::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value rmin, Value rmax)
{
    if (std::isless(rmax, rmin))
    {
        LogErr("max value is lower than min value");
    }
    else
    {
        pos.Generate(xmin, xmax, ymin, ymax);
        rad = RandomVal<Value>::Get(rmin, rmax);
    }
}

// ------------------------------------------------------------------------------------------------
Circle Circle::Abs() const
{
    return Circle(pos.Abs(), std::fabs(rad));
}

// ------------------------------------------------------------------------------------------------
bool Register_Circle(HSQUIRRELVM vm)
{
    LogDbg("Beginning registration of <Circle> type");

    typedef Circle::Value Val;

    Sqrat::RootTable(vm).Bind(_SC("Circle"), Sqrat::Class<Circle>(vm, _SC("Circle"))
        .Ctor()
        .Ctor<Val>()
        .Ctor<const Vector2f &, Val>()
        .Ctor<Val, Val, Val>()

        .SetStaticValue(_SC("delim"), &Circle::Delim)

        .Var(_SC("pos"), &Circle::pos)
        .Var(_SC("rad"), &Circle::rad)

        .Prop(_SC("abs"), &Circle::Abs)

        .Func(_SC("_tostring"), &Circle::ToString)
        .Func(_SC("_cmp"), &Circle::Cmp)

        .Func<Circle (Circle::*)(const Circle &) const>(_SC("_add"), &Circle::operator +)
        .Func<Circle (Circle::*)(const Circle &) const>(_SC("_sub"), &Circle::operator -)
        .Func<Circle (Circle::*)(const Circle &) const>(_SC("_mul"), &Circle::operator *)
        .Func<Circle (Circle::*)(const Circle &) const>(_SC("_div"), &Circle::operator /)
        .Func<Circle (Circle::*)(const Circle &) const>(_SC("_modulo"), &Circle::operator %)
        .Func<Circle (Circle::*)(void) const>(_SC("_unm"), &Circle::operator -)

        .Overload<void (Circle::*)(const Circle &)>(_SC("set"), &Circle::Set)
        .Overload<void (Circle::*)(const Vector2f &, Val)>(_SC("set"), &Circle::Set)
        .Overload<void (Circle::*)(Val, Val, Val)>(_SC("set"), &Circle::Set)
        .Overload<void (Circle::*)(Val)>(_SC("set_rad"), &Circle::Set)
        .Overload<void (Circle::*)(const Vector2f &)>(_SC("set_vec2"), &Circle::Set)
        .Overload<void (Circle::*)(Val, Val)>(_SC("set_vec2"), &Circle::Set)
        .Overload<void (Circle::*)(const SQChar *, SQChar)>(_SC("set_str"), &Circle::Set)

        .Func(_SC("clear"), &Circle::Clear)

        .Func<Circle & (Circle::*)(const Circle &)>(_SC("opAddAssign"), &Circle::operator +=)
        .Func<Circle & (Circle::*)(const Circle &)>(_SC("opSubAssign"), &Circle::operator -=)
        .Func<Circle & (Circle::*)(const Circle &)>(_SC("opMulAssign"), &Circle::operator *=)
        .Func<Circle & (Circle::*)(const Circle &)>(_SC("opDivAssign"), &Circle::operator /=)
        .Func<Circle & (Circle::*)(const Circle &)>(_SC("opModAssign"), &Circle::operator %=)

        .Func<Circle & (Circle::*)(Circle::Value)>(_SC("opAddAssignR"), &Circle::operator +=)
        .Func<Circle & (Circle::*)(Circle::Value)>(_SC("opSubAssignR"), &Circle::operator -=)
        .Func<Circle & (Circle::*)(Circle::Value)>(_SC("opMulAssignR"), &Circle::operator *=)
        .Func<Circle & (Circle::*)(Circle::Value)>(_SC("opDivAssignR"), &Circle::operator /=)
        .Func<Circle & (Circle::*)(Circle::Value)>(_SC("opModAssignR"), &Circle::operator %=)

        .Func<Circle & (Circle::*)(const Vector2f &)>(_SC("opAddAssignP"), &Circle::operator +=)
        .Func<Circle & (Circle::*)(const Vector2f &)>(_SC("opSubAssignP"), &Circle::operator -=)
        .Func<Circle & (Circle::*)(const Vector2f &)>(_SC("opMulAssignP"), &Circle::operator *=)
        .Func<Circle & (Circle::*)(const Vector2f &)>(_SC("opDivAssignP"), &Circle::operator /=)
        .Func<Circle & (Circle::*)(const Vector2f &)>(_SC("opModAssignP"), &Circle::operator %=)

        .Func<Circle & (Circle::*)(void)>(_SC("opPreInc"), &Circle::operator ++)
        .Func<Circle & (Circle::*)(void)>(_SC("opPreDec"), &Circle::operator --)
        .Func<Circle (Circle::*)(int)>(_SC("opPostInc"), &Circle::operator ++)
        .Func<Circle (Circle::*)(int)>(_SC("opPostDec"), &Circle::operator --)

        .Func<Circle (Circle::*)(const Circle &) const>(_SC("opAdd"), &Circle::operator +)
        .Func<Circle (Circle::*)(const Circle &) const>(_SC("opSub"), &Circle::operator -)
        .Func<Circle (Circle::*)(const Circle &) const>(_SC("opMul"), &Circle::operator *)
        .Func<Circle (Circle::*)(const Circle &) const>(_SC("opDiv"), &Circle::operator /)
        .Func<Circle (Circle::*)(const Circle &) const>(_SC("opMod"), &Circle::operator %)

        .Func<Circle (Circle::*)(Circle::Value) const>(_SC("opAddR"), &Circle::operator +)
        .Func<Circle (Circle::*)(Circle::Value) const>(_SC("opSubR"), &Circle::operator -)
        .Func<Circle (Circle::*)(Circle::Value) const>(_SC("opMulR"), &Circle::operator *)
        .Func<Circle (Circle::*)(Circle::Value) const>(_SC("opDivR"), &Circle::operator /)
        .Func<Circle (Circle::*)(Circle::Value) const>(_SC("opModR"), &Circle::operator %)

        .Func<Circle (Circle::*)(const Vector2f &) const>(_SC("opAddP"), &Circle::operator +)
        .Func<Circle (Circle::*)(const Vector2f &) const>(_SC("opSubP"), &Circle::operator -)
        .Func<Circle (Circle::*)(const Vector2f &) const>(_SC("opMulP"), &Circle::operator *)
        .Func<Circle (Circle::*)(const Vector2f &) const>(_SC("opDivP"), &Circle::operator /)
        .Func<Circle (Circle::*)(const Vector2f &) const>(_SC("opModP"), &Circle::operator %)

        .Func<Circle (Circle::*)(void) const>(_SC("opUnPlus"), &Circle::operator +)
        .Func<Circle (Circle::*)(void) const>(_SC("opUnMinus"), &Circle::operator -)

        .Func<bool (Circle::*)(const Circle &) const>(_SC("opEqual"), &Circle::operator ==)
        .Func<bool (Circle::*)(const Circle &) const>(_SC("opNotEqual"), &Circle::operator !=)
        .Func<bool (Circle::*)(const Circle &) const>(_SC("opLessThan"), &Circle::operator <)
        .Func<bool (Circle::*)(const Circle &) const>(_SC("opGreaterThan"), &Circle::operator >)
        .Func<bool (Circle::*)(const Circle &) const>(_SC("opLessEqual"), &Circle::operator <=)
        .Func<bool (Circle::*)(const Circle &) const>(_SC("opGreaterEqual"), &Circle::operator >=)
    );

    LogDbg("Registration of <Circle> type was successful");

    return true;
}

} // Namespace:: SqMod
