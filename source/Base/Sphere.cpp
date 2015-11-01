#include "Base/Sphere.hpp"
#include "Base/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Sphere Sphere::NIL = Sphere();
const Sphere Sphere::MIN = Sphere(0.0);
const Sphere Sphere::MAX = Sphere(std::numeric_limits<Sphere::Value>::max());

// ------------------------------------------------------------------------------------------------
SQChar Sphere::Delim = ',';

// ------------------------------------------------------------------------------------------------
Sphere::Sphere()
    : pos(0.0, 0.0, 0.0), rad(0.0)
{

}

Sphere::Sphere(Value r)
    : pos(0.0, 0.0, 0.0), rad(r)
{

}

Sphere::Sphere(const Vector3 & p)
    : pos(p), rad(0.0)
{

}

Sphere::Sphere(const Vector3 & p, Value r)
    : pos(p), rad(r)
{

}

Sphere::Sphere(Value x, Value y, Value z, Value r)
    : pos(x, y, z), rad(r)
{

}

// ------------------------------------------------------------------------------------------------
Sphere::Sphere(const Sphere & s)
    : pos(s.pos), rad(s.rad)
{

}

Sphere::Sphere(Sphere && s)
    : pos(s.pos), rad(s.rad)
{

}

// ------------------------------------------------------------------------------------------------
Sphere::~Sphere()
{

}

// ------------------------------------------------------------------------------------------------
Sphere & Sphere::operator = (const Sphere & s)
{
    pos = s.pos;
    rad = s.rad;
    return *this;
}

Sphere & Sphere::operator = (Sphere && s)
{
    pos = s.pos;
    rad = s.rad;
    return *this;
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
    return Sphere(pos + p);
}

Sphere Sphere::operator - (const Vector3 & p) const
{
    return Sphere(pos - p);
}

Sphere Sphere::operator * (const Vector3 & p) const
{
    return Sphere(pos * p);
}

Sphere Sphere::operator / (const Vector3 & p) const
{
    return Sphere(pos / p);
}

Sphere Sphere::operator % (const Vector3 & p) const
{
    return Sphere(pos % p);
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
    return (rad == s.rad) && (pos == s.pos);
}

bool Sphere::operator != (const Sphere & s) const
{
    return (rad != s.rad) && (pos != s.pos);
}

bool Sphere::operator < (const Sphere & s) const
{
    return (rad < s.rad) && (pos < s.pos);
}

bool Sphere::operator > (const Sphere & s) const
{
    return (rad > s.rad) && (pos > s.pos);
}

bool Sphere::operator <= (const Sphere & s) const
{
    return (rad <= s.rad) && (pos <= s.pos);
}

bool Sphere::operator >= (const Sphere & s) const
{
    return (rad >= s.rad) && (pos >= s.pos);
}

// ------------------------------------------------------------------------------------------------
SQInteger Sphere::Cmp(const Sphere & s) const
{
    return *this == s ? 0 : (*this > s ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Sphere::ToString() const
{
    return ToStringF("%f,%f,%f,%f", pos.x, pos.y, pos.z, rad);
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
void Sphere::Set(const SQChar * values, SQChar delim)
{
    Set(GetSphere(values, delim));
}

// ------------------------------------------------------------------------------------------------
void Sphere::Generate()
{
    pos.Generate();
    rad = RandomVal<Value>::Get();
}

void Sphere::Generate(Value min, Value max, bool r)
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

void Sphere::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax)
{
    pos.Generate(xmin, xmax, ymin, ymax, zmin, zmax);
}

void Sphere::Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value rmin, Value rmax)
{
    if (std::isless(rmax, rmin))
    {
        LogErr("max value is lower than min value");
    }
    else
    {
        pos.Generate(xmin, xmax, ymin, ymax, zmin, zmax);
        rad = RandomVal<Value>::Get(rmin, rmax);
    }
}

// ------------------------------------------------------------------------------------------------
Sphere Sphere::Abs() const
{
    return Sphere(pos.Abs(), std::fabs(rad));
}

// ------------------------------------------------------------------------------------------------
bool Register_Sphere(HSQUIRRELVM vm)
{
    LogDbg("Beginning registration of <Sphere> type");

    typedef Sphere::Value Val;

    Sqrat::RootTable(vm).Bind(_SC("Sphere"), Sqrat::Class<Sphere>(vm, _SC("Sphere"))
        .Ctor()
        .Ctor<Val>()
        .Ctor<const Vector3 &, Val>()
        .Ctor<Val, Val, Val, Val>()

        .SetStaticValue(_SC("delim"), &Sphere::Delim)

        .Var(_SC("pos"), &Sphere::pos)
        .Var(_SC("rad"), &Sphere::rad)

        .Prop(_SC("abs"), &Sphere::Abs)

        .Func(_SC("_tostring"), &Sphere::ToString)
        .Func(_SC("_cmp"), &Sphere::Cmp)

        .Func<Sphere (Sphere::*)(const Sphere &) const>(_SC("_add"), &Sphere::operator +)
        .Func<Sphere (Sphere::*)(const Sphere &) const>(_SC("_sub"), &Sphere::operator -)
        .Func<Sphere (Sphere::*)(const Sphere &) const>(_SC("_mul"), &Sphere::operator *)
        .Func<Sphere (Sphere::*)(const Sphere &) const>(_SC("_div"), &Sphere::operator /)
        .Func<Sphere (Sphere::*)(const Sphere &) const>(_SC("_modulo"), &Sphere::operator %)
        .Func<Sphere (Sphere::*)(void) const>(_SC("_unm"), &Sphere::operator -)

        .Overload<void (Sphere::*)(const Sphere &)>(_SC("set"), &Sphere::Set)
        .Overload<void (Sphere::*)(const Vector3 &, Val)>(_SC("set"), &Sphere::Set)
        .Overload<void (Sphere::*)(Val, Val, Val, Val)>(_SC("set"), &Sphere::Set)
        .Overload<void (Sphere::*)(Val)>(_SC("set_rad"), &Sphere::Set)
        .Overload<void (Sphere::*)(const Vector3 &)>(_SC("set_vec3"), &Sphere::Set)
        .Overload<void (Sphere::*)(Val, Val, Val)>(_SC("set_vec3"), &Sphere::Set)
        .Overload<void (Sphere::*)(const SQChar *, SQChar)>(_SC("set_str"), &Sphere::Set)

        .Func(_SC("clear"), &Sphere::Clear)

        .Func<Sphere & (Sphere::*)(const Sphere &)>(_SC("opAddAssign"), &Sphere::operator +=)
        .Func<Sphere & (Sphere::*)(const Sphere &)>(_SC("opSubAssign"), &Sphere::operator -=)
        .Func<Sphere & (Sphere::*)(const Sphere &)>(_SC("opMulAssign"), &Sphere::operator *=)
        .Func<Sphere & (Sphere::*)(const Sphere &)>(_SC("opDivAssign"), &Sphere::operator /=)
        .Func<Sphere & (Sphere::*)(const Sphere &)>(_SC("opModAssign"), &Sphere::operator %=)

        .Func<Sphere & (Sphere::*)(Sphere::Value)>(_SC("opAddAssignR"), &Sphere::operator +=)
        .Func<Sphere & (Sphere::*)(Sphere::Value)>(_SC("opSubAssignR"), &Sphere::operator -=)
        .Func<Sphere & (Sphere::*)(Sphere::Value)>(_SC("opMulAssignR"), &Sphere::operator *=)
        .Func<Sphere & (Sphere::*)(Sphere::Value)>(_SC("opDivAssignR"), &Sphere::operator /=)
        .Func<Sphere & (Sphere::*)(Sphere::Value)>(_SC("opModAssignR"), &Sphere::operator %=)

        .Func<Sphere & (Sphere::*)(const Vector3 &)>(_SC("opAddAssignP"), &Sphere::operator +=)
        .Func<Sphere & (Sphere::*)(const Vector3 &)>(_SC("opSubAssignP"), &Sphere::operator -=)
        .Func<Sphere & (Sphere::*)(const Vector3 &)>(_SC("opMulAssignP"), &Sphere::operator *=)
        .Func<Sphere & (Sphere::*)(const Vector3 &)>(_SC("opDivAssignP"), &Sphere::operator /=)
        .Func<Sphere & (Sphere::*)(const Vector3 &)>(_SC("opModAssignP"), &Sphere::operator %=)

        .Func<Sphere & (Sphere::*)(void)>(_SC("opPreInc"), &Sphere::operator ++)
        .Func<Sphere & (Sphere::*)(void)>(_SC("opPreDec"), &Sphere::operator --)
        .Func<Sphere (Sphere::*)(int)>(_SC("opPostInc"), &Sphere::operator ++)
        .Func<Sphere (Sphere::*)(int)>(_SC("opPostDec"), &Sphere::operator --)

        .Func<Sphere (Sphere::*)(const Sphere &) const>(_SC("opAdd"), &Sphere::operator +)
        .Func<Sphere (Sphere::*)(const Sphere &) const>(_SC("opSub"), &Sphere::operator -)
        .Func<Sphere (Sphere::*)(const Sphere &) const>(_SC("opMul"), &Sphere::operator *)
        .Func<Sphere (Sphere::*)(const Sphere &) const>(_SC("opDiv"), &Sphere::operator /)
        .Func<Sphere (Sphere::*)(const Sphere &) const>(_SC("opMod"), &Sphere::operator %)

        .Func<Sphere (Sphere::*)(Sphere::Value) const>(_SC("opAddR"), &Sphere::operator +)
        .Func<Sphere (Sphere::*)(Sphere::Value) const>(_SC("opSubR"), &Sphere::operator -)
        .Func<Sphere (Sphere::*)(Sphere::Value) const>(_SC("opMulR"), &Sphere::operator *)
        .Func<Sphere (Sphere::*)(Sphere::Value) const>(_SC("opDivR"), &Sphere::operator /)
        .Func<Sphere (Sphere::*)(Sphere::Value) const>(_SC("opModR"), &Sphere::operator %)

        .Func<Sphere (Sphere::*)(const Vector3 &) const>(_SC("opAddP"), &Sphere::operator +)
        .Func<Sphere (Sphere::*)(const Vector3 &) const>(_SC("opSubP"), &Sphere::operator -)
        .Func<Sphere (Sphere::*)(const Vector3 &) const>(_SC("opMulP"), &Sphere::operator *)
        .Func<Sphere (Sphere::*)(const Vector3 &) const>(_SC("opDivP"), &Sphere::operator /)
        .Func<Sphere (Sphere::*)(const Vector3 &) const>(_SC("opModP"), &Sphere::operator %)

        .Func<Sphere (Sphere::*)(void) const>(_SC("opUnPlus"), &Sphere::operator +)
        .Func<Sphere (Sphere::*)(void) const>(_SC("opUnMinus"), &Sphere::operator -)

        .Func<bool (Sphere::*)(const Sphere &) const>(_SC("opEqual"), &Sphere::operator ==)
        .Func<bool (Sphere::*)(const Sphere &) const>(_SC("opNotEqual"), &Sphere::operator !=)
        .Func<bool (Sphere::*)(const Sphere &) const>(_SC("opLessThan"), &Sphere::operator <)
        .Func<bool (Sphere::*)(const Sphere &) const>(_SC("opGreaterThan"), &Sphere::operator >)
        .Func<bool (Sphere::*)(const Sphere &) const>(_SC("opLessEqual"), &Sphere::operator <=)
        .Func<bool (Sphere::*)(const Sphere &) const>(_SC("opGreaterEqual"), &Sphere::operator >=)
    );

    LogDbg("Registration of <Sphere> type was successful");

    return true;
}

} // Namespace:: SqMod
