#include "Base/Color3.hpp"
#include "Base/Color4.hpp"
#include "Base/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Color3 Color3::NIL = Color3();
const Color3 Color3::MIN = Color3(std::numeric_limits<Color3::Value>::min());
const Color3 Color3::MAX = Color3(std::numeric_limits<Color3::Value>::max());

// ------------------------------------------------------------------------------------------------
SQChar Color3::Delim = ',';

// ------------------------------------------------------------------------------------------------
Color3::Color3() noexcept
    : r(0), g(0), b(0)
{

}

Color3::Color3(Value s) noexcept
    : r(s), g(s), b(s)
{

}

Color3::Color3(Value rv, Value gv, Value bv) noexcept
    : r(rv), g(gv), b(bv)
{

}

// ------------------------------------------------------------------------------------------------
Color3::Color3(const Color4 & c) noexcept
    : r(c.r), g(c.g), b(c.b)
{

}

// ------------------------------------------------------------------------------------------------
Color3::Color3(const SQChar * name) noexcept
    : Color3(GetColor(name))
{

}

Color3::Color3(const SQChar * str, SQChar delim) noexcept
    : Color3(GetColor3(str, delim))
{

}

// ------------------------------------------------------------------------------------------------
Color3::Color3(const Color3 & c) noexcept
    : r(c.r), g(c.g), b(c.b)
{

}

Color3::Color3(Color3 && c) noexcept
    : r(c.r), g(c.g), b(c.b)
{

}

// ------------------------------------------------------------------------------------------------
Color3::~Color3()
{

}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator = (const Color3 & c) noexcept
{
    r = c.r;
    g = c.g;
    b = c.b;
    return *this;
}

Color3 & Color3::operator = (Color3 && c) noexcept
{
    r = c.r;
    g = c.g;
    b = c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator = (Value s) noexcept
{
    r = s;
    g = s;
    b = s;
    return *this;
}

Color3 & Color3::operator = (const SQChar * name) noexcept
{
    Set(GetColor(name));
    return *this;
}

Color3 & Color3::operator = (const Color4 & c) noexcept
{
    r = c.r;
    g = c.g;
    b = c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator += (const Color3 & c) noexcept
{
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}

Color3 & Color3::operator -= (const Color3 & c) noexcept
{
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
}

Color3 & Color3::operator *= (const Color3 & c) noexcept
{
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
}

Color3 & Color3::operator /= (const Color3 & c) noexcept
{
    r /= c.r;
    g /= c.g;
    b /= c.b;
    return *this;
}

Color3 & Color3::operator %= (const Color3 & c) noexcept
{
    r %= c.r;
    g %= c.g;
    b %= c.b;
    return *this;
}

Color3 & Color3::operator &= (const Color3 & c) noexcept
{
    r &= c.r;
    g &= c.g;
    b &= c.b;
    return *this;
}

Color3 & Color3::operator |= (const Color3 & c) noexcept
{
    r |= c.r;
    g |= c.g;
    b |= c.b;
    return *this;
}

Color3 & Color3::operator ^= (const Color3 & c) noexcept
{
    r ^= c.r;
    g ^= c.g;
    b ^= c.b;
    return *this;
}

Color3 & Color3::operator <<= (const Color3 & c) noexcept
{
    r <<= c.r;
    g <<= c.g;
    b <<= c.b;
    return *this;
}

Color3 & Color3::operator >>= (const Color3 & c) noexcept
{
    r >>= c.r;
    g >>= c.g;
    b >>= c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator += (Value s) noexcept
{
    r += s;
    g += s;
    b += s;
    return *this;
}

Color3 & Color3::operator -= (Value s) noexcept
{
    r -= s;
    g -= s;
    b -= s;
    return *this;
}

Color3 & Color3::operator *= (Value s) noexcept
{
    r *= s;
    g *= s;
    b *= s;
    return *this;
}

Color3 & Color3::operator /= (Value s) noexcept
{
    r /= s;
    g /= s;
    b /= s;
    return *this;
}

Color3 & Color3::operator %= (Value s) noexcept
{
    r %= s;
    g %= s;
    b %= s;
    return *this;
}

Color3 & Color3::operator &= (Value s) noexcept
{
    r &= s;
    g &= s;
    b &= s;
    return *this;
}

Color3 & Color3::operator |= (Value s) noexcept
{
    r |= s;
    g |= s;
    b |= s;
    return *this;
}

Color3 & Color3::operator ^= (Value s) noexcept
{
    r ^= s;
    g ^= s;
    b ^= s;
    return *this;
}

Color3 & Color3::operator <<= (Value s) noexcept
{
    r <<= s;
    g <<= s;
    b <<= s;
    return *this;
}

Color3 & Color3::operator >>= (Value s) noexcept
{
    r >>= s;
    g >>= s;
    b >>= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator ++ () noexcept
{
    ++r;
    ++g;
    ++b;
    return *this;
}

Color3 & Color3::operator -- () noexcept
{
    --r;
    --g;
    --b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator ++ (int) noexcept
{
    Color3 state(*this);
    ++r;
    ++g;
    ++b;
    return state;
}

Color3 Color3::operator -- (int) noexcept
{
    Color3 state(*this);
    --r;
    --g;
    --b;
    return state;
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator + (const Color3 & c) const noexcept
{
    return Color3(r + c.r, g + c.g, b + c.b);
}

Color3 Color3::operator - (const Color3 & c) const noexcept
{
    return Color3(r - c.r, g - c.g, b - c.b);
}

Color3 Color3::operator * (const Color3 & c) const noexcept
{
    return Color3(r * c.r, g * c.g, b * c.b);
}

Color3 Color3::operator / (const Color3 & c) const noexcept
{
    return Color3(r / c.r, g / c.g, b / c.b);
}

Color3 Color3::operator % (const Color3 & c) const noexcept
{
    return Color3(r % c.r, g % c.g, b % c.b);
}

Color3 Color3::operator & (const Color3 & c) const noexcept
{
    return Color3(r & c.r, g & c.g, b & c.b);
}

Color3 Color3::operator | (const Color3 & c) const noexcept
{
    return Color3(r | c.r, g | c.g, b | c.b);
}

Color3 Color3::operator ^ (const Color3 & c) const noexcept
{
    return Color3(r ^ c.r, g ^ c.g, b ^ c.b);
}

Color3 Color3::operator << (const Color3 & c) const noexcept
{
    return Color3(r << c.r, g << c.g, b << c.b);
}

Color3 Color3::operator >> (const Color3 & c) const noexcept
{
    return Color3(r >> c.r, g >> c.g, b >> c.b);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator + (Value s) const noexcept
{
    return Color3(r + s, g + s, b + s);
}

Color3 Color3::operator - (Value s) const noexcept
{
    return Color3(r - s, g - s, b - s);
}

Color3 Color3::operator * (Value s) const noexcept
{
    return Color3(r * s, g * s, b * s);
}

Color3 Color3::operator / (Value s) const noexcept
{
    return Color3(r / s, g / s, b / s);
}

Color3 Color3::operator % (Value s) const noexcept
{
    return Color3(r % s, g % s, b % s);
}

Color3 Color3::operator & (Value s) const noexcept
{
    return Color3(r & s, g & s, b & s);
}

Color3 Color3::operator | (Value s) const noexcept
{
    return Color3(r | s, g | s, b | s);
}

Color3 Color3::operator ^ (Value s) const noexcept
{
    return Color3(r ^ s, g ^ s, b ^ s);
}

Color3 Color3::operator << (Value s) const noexcept
{
    return Color3(r << s, g << s, b << s);
}

Color3 Color3::operator >> (Value s) const noexcept
{
    return Color3(r >> s, g >> s, b >> s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator + () const noexcept
{
    return Color3(r, g, b);
}

Color3 Color3::operator - () const noexcept
{
    return Color3(0, 0, 0);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator ~ () const noexcept
{
    return Color3(~r, ~g, ~b);
}

// ------------------------------------------------------------------------------------------------
bool Color3::operator == (const Color3 & c) const noexcept
{
    return (r == c.r) && (g == c.g) && (b == c.b);
}

bool Color3::operator != (const Color3 & c) const noexcept
{
    return (r != c.r) && (g != c.g) && (b != c.b);
}

bool Color3::operator < (const Color3 & c) const noexcept
{
    return (r < c.r) && (g < c.g) && (b < c.b);
}

bool Color3::operator > (const Color3 & c) const noexcept
{
    return (r > c.r) && (g > c.g) && (b > c.b);
}

bool Color3::operator <= (const Color3 & c) const noexcept
{
    return (r <= c.r) && (g <= c.g) && (b <= c.b);
}

bool Color3::operator >= (const Color3 & c) const noexcept
{
    return (r >= c.r) && (g >= c.g) && (b >= c.b);
}

// ------------------------------------------------------------------------------------------------
SQInteger Color3::Cmp(const Color3 & c) const noexcept
{
    return *this == c ? 0 : (*this > c ? 1 : -1);
}

// ------------------------------------------------------------------------------------------------
const SQChar * Color3::ToString() const noexcept
{
    return ToStringF("%u,%u,%u", r, g, b);
}

// ------------------------------------------------------------------------------------------------
void Color3::Set(Value ns) noexcept
{
    r = ns;
    g = ns;
    b = ns;
}

void Color3::Set(Value nr, Value ng, Value nb) noexcept
{
    r = nr;
    g = ng;
    b = nb;
}

// ------------------------------------------------------------------------------------------------
void Color3::Set(const Color3 & c) noexcept
{
    r = c.r;
    g = c.g;
    b = c.b;
}

void Color3::Set(const Color4 & c) noexcept
{
    r = c.r;
    g = c.g;
    b = c.b;
}

// ------------------------------------------------------------------------------------------------
void Color3::Set(const SQChar * str, SQChar delim) noexcept
{
    Set(GetColor3(str, delim));
}

// ------------------------------------------------------------------------------------------------
void Color3::SetCol(const SQChar * name) noexcept
{
    Set(GetColor(name));
}

// ------------------------------------------------------------------------------------------------
SQUint32 Color3::GetRGB() const noexcept
{
    return static_cast<SQUint32>(r << 16 | g << 8 | b);
}

void Color3::SetRGB(SQUint32 p) noexcept
{
    r = static_cast<Value>((p >> 16) & 0xFF);
    g = static_cast<Value>((p >> 8) & 0xFF);
    b = static_cast<Value>((p) & 0xFF);
}

// ------------------------------------------------------------------------------------------------
SQUint32 Color3::GetRGBA() const noexcept
{
    return static_cast<SQUint32>(r << 24 | g << 16 | b << 8 | 0x00);
}

void Color3::SetRGBA(SQUint32 p) noexcept
{
    r = static_cast<Value>((p >> 24) & 0xFF);
    g = static_cast<Value>((p >> 16) & 0xFF);
    b = static_cast<Value>((p >> 8) & 0xFF);
}

// ------------------------------------------------------------------------------------------------
SQUint32 Color3::GetARGB() const noexcept
{
    return static_cast<SQUint32>(0x00 << 24 | r << 16 | g << 8 | b);
}

void Color3::SetARGB(SQUint32 p) noexcept
{
    r = static_cast<Value>((p >> 16) & 0xFF);
    g = static_cast<Value>((p >> 8) & 0xFF);
    b = static_cast<Value>((p) & 0xFF);
}

// ------------------------------------------------------------------------------------------------
void Color3::Generate() noexcept
{
    r = RandomVal<Value>::Get();
    g = RandomVal<Value>::Get();
    b = RandomVal<Value>::Get();
}

void Color3::Generate(Value min, Value max) noexcept
{
    if (max < min)
    {
        LogErr("max value is lower than min value");
    }
    else
    {
        r = RandomVal<Value>::Get(min, max);
        g = RandomVal<Value>::Get(min, max);
        b = RandomVal<Value>::Get(min, max);
    }
}

void Color3::Generate(Value rmin, Value rmax, Value gmin, Value gmax, Value bmin, Value bmax) noexcept
{
    if (rmax < rmin || gmax < gmin || bmax < bmin)
    {
        LogErr("max value is lower than min value");
    }
    else
    {
        r = RandomVal<Value>::Get(rmin, rmax);
        g = RandomVal<Value>::Get(gmin, gmax);
        b = RandomVal<Value>::Get(bmin, bmax);
    }
}

// ------------------------------------------------------------------------------------------------
void Color3::Random() noexcept
{
    Set(GetRandomColor());
}

// ------------------------------------------------------------------------------------------------
void Color3::Inverse() noexcept
{
    r = static_cast<Value>(~r);
    g = static_cast<Value>(~g);
    b = static_cast<Value>(~b);
}

// ================================================================================================
bool Register_Color3(HSQUIRRELVM vm)
{
    LogDbg("Beginning registration of <Color3> type");

    typedef Color3::Value Val;

    Sqrat::RootTable(vm).Bind(_SC("Color3"), Sqrat::Class<Color3>(vm, _SC("Color3"))
        .Ctor()
        .Ctor<Val>()
        .Ctor<Val, Val, Val>()
        .Ctor<const SQChar *, SQChar>()

        .SetStaticValue(_SC("delim"), &Color3::Delim)

        .Var(_SC("r"), &Color3::r)
        .Var(_SC("g"), &Color3::g)
        .Var(_SC("b"), &Color3::b)

        .Prop(_SC("rgb"), &Color3::GetRGB, &Color3::SetRGB)
        .Prop(_SC("rgba"), &Color3::GetRGBA, &Color3::SetRGBA)
        .Prop(_SC("argb"), &Color3::GetARGB, &Color3::SetARGB)
        .Prop(_SC("str"), &Color3::SetCol)

        .Func(_SC("_tostring"), &Color3::ToString)
        .Func(_SC("_cmp"), &Color3::Cmp)

        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("_add"), &Color3::operator +)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("_sub"), &Color3::operator -)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("_mul"), &Color3::operator *)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("_div"), &Color3::operator /)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("_modulo"), &Color3::operator %)
        .Func<Color3 (Color3::*)(void) const>(_SC("_unm"), &Color3::operator -)

        .Overload<void (Color3::*)(Val)>(_SC("set"), &Color3::Set)
        .Overload<void (Color3::*)(Val, Val, Val)>(_SC("set"), &Color3::Set)
        .Overload<void (Color3::*)(const Color3 &)>(_SC("set_col3"), &Color3::Set)
        .Overload<void (Color3::*)(const Color4 &)>(_SC("set_col4"), &Color3::Set)
        .Overload<void (Color3::*)(const SQChar *, SQChar)>(_SC("set_str"), &Color3::Set)

        .Overload<void (Color3::*)(void)>(_SC("generate"), &Color3::Generate)
        .Overload<void (Color3::*)(Val, Val)>(_SC("generate"), &Color3::Generate)
        .Overload<void (Color3::*)(Val, Val, Val, Val, Val, Val)>(_SC("generate"), &Color3::Generate)

        .Func(_SC("clear"), &Color3::Clear)
        .Func(_SC("random"), &Color3::Random)
        .Func(_SC("inverse"), &Color3::Inverse)

        .Func<Color3 & (Color3::*)(const Color3 &)>(_SC("opAddAssign"), &Color3::operator +=)
        .Func<Color3 & (Color3::*)(const Color3 &)>(_SC("opSubAssign"), &Color3::operator -=)
        .Func<Color3 & (Color3::*)(const Color3 &)>(_SC("opMulAssign"), &Color3::operator *=)
        .Func<Color3 & (Color3::*)(const Color3 &)>(_SC("opDivAssign"), &Color3::operator /=)
        .Func<Color3 & (Color3::*)(const Color3 &)>(_SC("opModAssign"), &Color3::operator %=)
        .Func<Color3 & (Color3::*)(const Color3 &)>(_SC("opAndAssign"), &Color3::operator &=)
        .Func<Color3 & (Color3::*)(const Color3 &)>(_SC("opOrAssign"), &Color3::operator |=)
        .Func<Color3 & (Color3::*)(const Color3 &)>(_SC("opXorAssign"), &Color3::operator ^=)
        .Func<Color3 & (Color3::*)(const Color3 &)>(_SC("opShlAssign"), &Color3::operator <<=)
        .Func<Color3 & (Color3::*)(const Color3 &)>(_SC("opShrAssign"), &Color3::operator >>=)

        .Func<Color3 & (Color3::*)(Color3::Value)>(_SC("opAddAssignS"), &Color3::operator +=)
        .Func<Color3 & (Color3::*)(Color3::Value)>(_SC("opSubAssignS"), &Color3::operator -=)
        .Func<Color3 & (Color3::*)(Color3::Value)>(_SC("opMulAssignS"), &Color3::operator *=)
        .Func<Color3 & (Color3::*)(Color3::Value)>(_SC("opDivAssignS"), &Color3::operator /=)
        .Func<Color3 & (Color3::*)(Color3::Value)>(_SC("opModAssignS"), &Color3::operator %=)
        .Func<Color3 & (Color3::*)(Color3::Value)>(_SC("opAndAssignS"), &Color3::operator &=)
        .Func<Color3 & (Color3::*)(Color3::Value)>(_SC("opOrAssignS"), &Color3::operator |=)
        .Func<Color3 & (Color3::*)(Color3::Value)>(_SC("opXorAssignS"), &Color3::operator ^=)
        .Func<Color3 & (Color3::*)(Color3::Value)>(_SC("opShlAssignS"), &Color3::operator <<=)
        .Func<Color3 & (Color3::*)(Color3::Value)>(_SC("opShrAssignS"), &Color3::operator >>=)

        .Func<Color3 & (Color3::*)(void)>(_SC("opPreInc"), &Color3::operator ++)
        .Func<Color3 & (Color3::*)(void)>(_SC("opPreDec"), &Color3::operator --)
        .Func<Color3 (Color3::*)(int)>(_SC("opPostInc"), &Color3::operator ++)
        .Func<Color3 (Color3::*)(int)>(_SC("opPostDec"), &Color3::operator --)

        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("opAdd"), &Color3::operator +)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("opSub"), &Color3::operator -)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("opMul"), &Color3::operator *)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("opDiv"), &Color3::operator /)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("opMod"), &Color3::operator %)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("opAnd"), &Color3::operator &)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("opOr"), &Color3::operator |)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("opShl"), &Color3::operator ^)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("opShl"), &Color3::operator <<)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("opShr"), &Color3::operator >>)

        .Func<Color3 (Color3::*)(Color3::Value) const>(_SC("opAddS"), &Color3::operator +)
        .Func<Color3 (Color3::*)(Color3::Value) const>(_SC("opSubS"), &Color3::operator -)
        .Func<Color3 (Color3::*)(Color3::Value) const>(_SC("opMulS"), &Color3::operator *)
        .Func<Color3 (Color3::*)(Color3::Value) const>(_SC("opDivS"), &Color3::operator /)
        .Func<Color3 (Color3::*)(Color3::Value) const>(_SC("opModS"), &Color3::operator %)
        .Func<Color3 (Color3::*)(Color3::Value) const>(_SC("opAndS"), &Color3::operator &)
        .Func<Color3 (Color3::*)(Color3::Value) const>(_SC("opOrS"), &Color3::operator |)
        .Func<Color3 (Color3::*)(Color3::Value) const>(_SC("opShlS"), &Color3::operator ^)
        .Func<Color3 (Color3::*)(Color3::Value) const>(_SC("opShlS"), &Color3::operator <<)
        .Func<Color3 (Color3::*)(Color3::Value) const>(_SC("opShrS"), &Color3::operator >>)

        .Func<Color3 (Color3::*)(void) const>(_SC("opUnPlus"), &Color3::operator +)
        .Func<Color3 (Color3::*)(void) const>(_SC("opUnMinus"), &Color3::operator -)
        .Func<Color3 (Color3::*)(void) const>(_SC("opCom"), &Color3::operator ~)

        .Func<bool (Color3::*)(const Color3 &) const>(_SC("opEqual"), &Color3::operator ==)
        .Func<bool (Color3::*)(const Color3 &) const>(_SC("opNotEqual"), &Color3::operator !=)
        .Func<bool (Color3::*)(const Color3 &) const>(_SC("opLessThan"), &Color3::operator <)
        .Func<bool (Color3::*)(const Color3 &) const>(_SC("opGreaterThan"), &Color3::operator >)
        .Func<bool (Color3::*)(const Color3 &) const>(_SC("opLessEqual"), &Color3::operator <=)
        .Func<bool (Color3::*)(const Color3 &) const>(_SC("opGreaterEqual"), &Color3::operator >=)
    );

    LogDbg("Registration of <Color3> type was successful");

    return true;
}

} // Namespace:: SqMod
