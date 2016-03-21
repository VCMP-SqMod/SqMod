// ------------------------------------------------------------------------------------------------
#include "Base/Color3.hpp"
#include "Base/Color4.hpp"
#include "Base/Shared.hpp"
#include "Library/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Color3 Color3::NIL = Color3();
const Color3 Color3::MIN = Color3(NumLimit< Color3::Value >::Min);
const Color3 Color3::MAX = Color3(NumLimit< Color3::Value >::Max);

// ------------------------------------------------------------------------------------------------
SQChar Color3::Delim = ',';

// ------------------------------------------------------------------------------------------------
SQInteger Color3::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("Color3");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Color3::Color3()
    : r(0), g(0), b(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Color3::Color3(Value sv)
    : r(sv), g(sv), b(sv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Color3::Color3(Value rv, Value gv, Value bv)
    : r(rv), g(gv), b(bv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator = (Value s)
{
    r = s;
    g = s;
    b = s;
    return *this;
}

Color3 & Color3::operator = (CSStr name)
{
    Set(GetColor(name));
    return *this;
}

Color3 & Color3::operator = (const Color4 & c)
{
    r = c.r;
    g = c.g;
    b = c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator += (const Color3 & c)
{
    r += c.r;
    g += c.g;
    b += c.b;
    return *this;
}

Color3 & Color3::operator -= (const Color3 & c)
{
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
}

Color3 & Color3::operator *= (const Color3 & c)
{
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
}

Color3 & Color3::operator /= (const Color3 & c)
{
    r /= c.r;
    g /= c.g;
    b /= c.b;
    return *this;
}

Color3 & Color3::operator %= (const Color3 & c)
{
    r %= c.r;
    g %= c.g;
    b %= c.b;
    return *this;
}

Color3 & Color3::operator &= (const Color3 & c)
{
    r &= c.r;
    g &= c.g;
    b &= c.b;
    return *this;
}

Color3 & Color3::operator |= (const Color3 & c)
{
    r |= c.r;
    g |= c.g;
    b |= c.b;
    return *this;
}

Color3 & Color3::operator ^= (const Color3 & c)
{
    r ^= c.r;
    g ^= c.g;
    b ^= c.b;
    return *this;
}

Color3 & Color3::operator <<= (const Color3 & c)
{
    r <<= c.r;
    g <<= c.g;
    b <<= c.b;
    return *this;
}

Color3 & Color3::operator >>= (const Color3 & c)
{
    r >>= c.r;
    g >>= c.g;
    b >>= c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator += (Value s)
{
    r += s;
    g += s;
    b += s;
    return *this;
}

Color3 & Color3::operator -= (Value s)
{
    r -= s;
    g -= s;
    b -= s;
    return *this;
}

Color3 & Color3::operator *= (Value s)
{
    r *= s;
    g *= s;
    b *= s;
    return *this;
}

Color3 & Color3::operator /= (Value s)
{
    r /= s;
    g /= s;
    b /= s;
    return *this;
}

Color3 & Color3::operator %= (Value s)
{
    r %= s;
    g %= s;
    b %= s;
    return *this;
}

Color3 & Color3::operator &= (Value s)
{
    r &= s;
    g &= s;
    b &= s;
    return *this;
}

Color3 & Color3::operator |= (Value s)
{
    r |= s;
    g |= s;
    b |= s;
    return *this;
}

Color3 & Color3::operator ^= (Value s)
{
    r ^= s;
    g ^= s;
    b ^= s;
    return *this;
}

Color3 & Color3::operator <<= (Value s)
{
    r <<= s;
    g <<= s;
    b <<= s;
    return *this;
}

Color3 & Color3::operator >>= (Value s)
{
    r >>= s;
    g >>= s;
    b >>= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator ++ ()
{
    ++r;
    ++g;
    ++b;
    return *this;
}

Color3 & Color3::operator -- ()
{
    --r;
    --g;
    --b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator ++ (int)
{
    Color3 state(*this);
    ++r;
    ++g;
    ++b;
    return state;
}

Color3 Color3::operator -- (int)
{
    Color3 state(*this);
    --r;
    --g;
    --b;
    return state;
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator + (const Color3 & c) const
{
    return Color3(r + c.r, g + c.g, b + c.b);
}

Color3 Color3::operator - (const Color3 & c) const
{
    return Color3(r - c.r, g - c.g, b - c.b);
}

Color3 Color3::operator * (const Color3 & c) const
{
    return Color3(r * c.r, g * c.g, b * c.b);
}

Color3 Color3::operator / (const Color3 & c) const
{
    return Color3(r / c.r, g / c.g, b / c.b);
}

Color3 Color3::operator % (const Color3 & c) const
{
    return Color3(r % c.r, g % c.g, b % c.b);
}

Color3 Color3::operator & (const Color3 & c) const
{
    return Color3(r & c.r, g & c.g, b & c.b);
}

Color3 Color3::operator | (const Color3 & c) const
{
    return Color3(r | c.r, g | c.g, b | c.b);
}

Color3 Color3::operator ^ (const Color3 & c) const
{
    return Color3(r ^ c.r, g ^ c.g, b ^ c.b);
}

Color3 Color3::operator << (const Color3 & c) const
{
    return Color3(r << c.r, g << c.g, b << c.b);
}

Color3 Color3::operator >> (const Color3 & c) const
{
    return Color3(r >> c.r, g >> c.g, b >> c.b);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator + (Value s) const
{
    return Color3(r + s, g + s, b + s);
}

Color3 Color3::operator - (Value s) const
{
    return Color3(r - s, g - s, b - s);
}

Color3 Color3::operator * (Value s) const
{
    return Color3(r * s, g * s, b * s);
}

Color3 Color3::operator / (Value s) const
{
    return Color3(r / s, g / s, b / s);
}

Color3 Color3::operator % (Value s) const
{
    return Color3(r % s, g % s, b % s);
}

Color3 Color3::operator & (Value s) const
{
    return Color3(r & s, g & s, b & s);
}

Color3 Color3::operator | (Value s) const
{
    return Color3(r | s, g | s, b | s);
}

Color3 Color3::operator ^ (Value s) const
{
    return Color3(r ^ s, g ^ s, b ^ s);
}

Color3 Color3::operator << (Value s) const
{
    return Color3(r << s, g << s, b << s);
}

Color3 Color3::operator >> (Value s) const
{
    return Color3(r >> s, g >> s, b >> s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator + () const
{
    return Color3(r, g, b);
}

Color3 Color3::operator - () const
{
    return Color3(0, 0, 0);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator ~ () const
{
    return Color3(~r, ~g, ~b);
}

// ------------------------------------------------------------------------------------------------
bool Color3::operator == (const Color3 & c) const
{
    return (r == c.r) && (g == c.g) && (b == c.b);
}

bool Color3::operator != (const Color3 & c) const
{
    return (r != c.r) && (g != c.g) && (b != c.b);
}

bool Color3::operator < (const Color3 & c) const
{
    return (r < c.r) && (g < c.g) && (b < c.b);
}

bool Color3::operator > (const Color3 & c) const
{
    return (r > c.r) && (g > c.g) && (b > c.b);
}

bool Color3::operator <= (const Color3 & c) const
{
    return (r <= c.r) && (g <= c.g) && (b <= c.b);
}

bool Color3::operator >= (const Color3 & c) const
{
    return (r >= c.r) && (g >= c.g) && (b >= c.b);
}

// ------------------------------------------------------------------------------------------------
Color3::operator Color4 () const
{
    return Color4(r, g, b);
}

// ------------------------------------------------------------------------------------------------
Int32 Color3::Cmp(const Color3 & o) const
{
    if (*this == o)
        return 0;
    else if (*this > o)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr Color3::ToString() const
{
    return ToStrF("%u,%u,%u", r, g, b);
}

// ------------------------------------------------------------------------------------------------
void Color3::Set(Value ns)
{
    r = ns;
    g = ns;
    b = ns;
}

void Color3::Set(Value nr, Value ng, Value nb)
{
    r = nr;
    g = ng;
    b = nb;
}

// ------------------------------------------------------------------------------------------------
void Color3::Set(const Color3 & c)
{
    r = c.r;
    g = c.g;
    b = c.b;
}

void Color3::Set(const Color4 & c)
{
    r = c.r;
    g = c.g;
    b = c.b;
}

// ------------------------------------------------------------------------------------------------
void Color3::Set(CSStr str, SQChar delim)
{
    Set(GetColor3(str, delim));
}

// ------------------------------------------------------------------------------------------------
void Color3::SetCol(CSStr name)
{
    Set(GetColor(name));
}

// ------------------------------------------------------------------------------------------------
Uint32 Color3::GetRGB() const
{
    return Uint32(r << 16 | g << 8 | b);
}

void Color3::SetRGB(Uint32 p)
{
    r = static_cast< Value >((p >> 16) & 0xFF);
    g = static_cast< Value >((p >> 8) & 0xFF);
    b = static_cast< Value >((p) & 0xFF);
}

// ------------------------------------------------------------------------------------------------
Uint32 Color3::GetRGBA() const
{
    return Uint32(r << 24 | g << 16 | b << 8 | 0x00);
}

void Color3::SetRGBA(Uint32 p)
{
    r = static_cast< Value >((p >> 24) & 0xFF);
    g = static_cast< Value >((p >> 16) & 0xFF);
    b = static_cast< Value >((p >> 8) & 0xFF);
}

// ------------------------------------------------------------------------------------------------
Uint32 Color3::GetARGB() const
{
    return Uint32(0x00 << 24 | r << 16 | g << 8 | b);
}

void Color3::SetARGB(Uint32 p)
{
    r = static_cast< Value >((p >> 16) & 0xFF);
    g = static_cast< Value >((p >> 8) & 0xFF);
    b = static_cast< Value >((p) & 0xFF);
}

// ------------------------------------------------------------------------------------------------
void Color3::Generate()
{
    r = GetRandomUint8();
    g = GetRandomUint8();
    b = GetRandomUint8();
}

void Color3::Generate(Value min, Value max)
{
    if (max < min)
        STHROWF("max value is lower than min value");

    r = GetRandomUint8(min, max);
    g = GetRandomUint8(min, max);
    b = GetRandomUint8(min, max);
}

void Color3::Generate(Value rmin, Value rmax, Value gmin, Value gmax, Value bmin, Value bmax)
{
    if (rmax < rmin || gmax < gmin || bmax < bmin)
        STHROWF("max value is lower than min value");

    r = GetRandomUint8(rmin, rmax);
    g = GetRandomUint8(gmin, gmax);
    b = GetRandomUint8(bmin, bmax);
}

// ------------------------------------------------------------------------------------------------
void Color3::Random()
{
    Set(GetRandomColor());
}

// ------------------------------------------------------------------------------------------------
void Color3::Inverse()
{
    r = static_cast< Value >(~r);
    g = static_cast< Value >(~g);
    b = static_cast< Value >(~b);
}

// ------------------------------------------------------------------------------------------------
const Color3 & GetColor3(CSStr str)
{
    return GetColor3(str, Color3::Delim);
}

// ------------------------------------------------------------------------------------------------
const Color3 & GetColor3(CSStr str, SQChar delim)
{
    // The format specifications that will be used to scan the string
    static SQChar fs[] = _SC(" %u , %u , %u ");
    static Color3 col;
    // The minimum and maximum values supported by the Color3 type
    static const Uint32 min = std::numeric_limits< Color3::Value >::min();
    static const Uint32 max = std::numeric_limits< Color3::Value >::max();
    // Clear previous values, if any
    col.Clear();
    // Is the specified string empty?
    if (!str || *str == '\0')
    {
        return col; // Return the value as is!
    }
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    // The sscanf function requires at least 32 bit integers
    Uint32 r = 0, g = 0, b = 0;
    // Attempt to extract the component values from the specified string
    std::sscanf(str, fs, &r, &g, &b);
    // Cast the extracted integers to the value used by the Color3 type
    col.r = static_cast< Color3::Value >(Clamp(r, min, max));
    col.g = static_cast< Color3::Value >(Clamp(g, min, max));
    col.b = static_cast< Color3::Value >(Clamp(b, min, max));
    // Return the resulted value
    return col;
}

// ================================================================================================
void Register_Color3(HSQUIRRELVM vm)
{
    typedef Color3::Value Val;

    RootTable(vm).Bind(_SC("Color3"), Class< Color3 >(vm, _SC("Color3"))
        /* Constructors */
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val, Val >()
        /* Static Members */
        .SetStaticValue(_SC("Delim"), &Color3::Delim)
        /* Member Variables */
        .Var(_SC("r"), &Color3::r)
        .Var(_SC("g"), &Color3::g)
        .Var(_SC("b"), &Color3::b)
        /* Properties */
        .Prop(_SC("rgb"), &Color3::GetRGB, &Color3::SetRGB)
        .Prop(_SC("rgba"), &Color3::GetRGBA, &Color3::SetRGBA)
        .Prop(_SC("argb"), &Color3::GetARGB, &Color3::SetARGB)
        .Prop(_SC("str"), &Color3::SetCol)
        /* Core Metamethods */
        .Func(_SC("_tostring"), &Color3::ToString)
        .SquirrelFunc(_SC("_typename"), &Color3::Typename)
        .Func(_SC("_cmp"), &Color3::Cmp)
        /* Metamethods */
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("_add"), &Color3::operator +)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("_sub"), &Color3::operator -)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("_mul"), &Color3::operator *)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("_div"), &Color3::operator /)
        .Func<Color3 (Color3::*)(const Color3 &) const>(_SC("_modulo"), &Color3::operator %)
        .Func<Color3 (Color3::*)(void) const>(_SC("_unm"), &Color3::operator -)
        /* Setters */
        .Overload<void (Color3::*)(Val)>(_SC("Set"), &Color3::Set)
        .Overload<void (Color3::*)(Val, Val, Val)>(_SC("Set"), &Color3::Set)
        .Overload<void (Color3::*)(const Color3 &)>(_SC("SetCol3"), &Color3::Set)
        .Overload<void (Color3::*)(const Color4 &)>(_SC("SetCol4"), &Color3::Set)
        .Overload<void (Color3::*)(CSStr, SQChar)>(_SC("SetStr"), &Color3::Set)
        /* Random Generators */
        .Overload<void (Color3::*)(void)>(_SC("Generate"), &Color3::Generate)
        .Overload<void (Color3::*)(Val, Val)>(_SC("Generate"), &Color3::Generate)
        .Overload<void (Color3::*)(Val, Val, Val, Val, Val, Val)>(_SC("Generate"), &Color3::Generate)
        /* Utility Methods */
        .Func(_SC("Clear"), &Color3::Clear)
        .Func(_SC("Random"), &Color3::Random)
        .Func(_SC("Inverse"), &Color3::Inverse)
        /* Operator Exposure */
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
        // Static Overloads
        .StaticOverload< const Color3 & (*)(CSStr) >(_SC("FromStr"), &GetColor3)
        .StaticOverload< const Color3 & (*)(CSStr, SQChar) >(_SC("FromStr"), &GetColor3)
    );
}

} // Namespace:: SqMod
