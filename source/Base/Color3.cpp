// ------------------------------------------------------------------------------------------------
#include "Base/Color3.hpp"
#include "Base/Color4.hpp"
#include "Base/Shared.hpp"
#include "Base/DynArg.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Color3 Color3::NIL = Color3();
const Color3 Color3::MIN = Color3(std::numeric_limits< Color3::Value >::min());
const Color3 Color3::MAX = Color3(std::numeric_limits< Color3::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Color3::Delim = ',';

// ------------------------------------------------------------------------------------------------
SQInteger Color3::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("Color3");
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
Color3::Color3(Value rv, Value gv, Value bv, Value /*av*/)
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

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator = (CSStr name)
{
    SetColor3(GetColor(name));
    return *this;
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator -= (const Color3 & c)
{
    r -= c.r;
    g -= c.g;
    b -= c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator *= (const Color3 & c)
{
    r *= c.r;
    g *= c.g;
    b *= c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator /= (const Color3 & c)
{
    r /= c.r;
    g /= c.g;
    b /= c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator %= (const Color3 & c)
{
    r %= c.r;
    g %= c.g;
    b %= c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator &= (const Color3 & c)
{
    r &= c.r;
    g &= c.g;
    b &= c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator |= (const Color3 & c)
{
    r |= c.r;
    g |= c.g;
    b |= c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator ^= (const Color3 & c)
{
    r ^= c.r;
    g ^= c.g;
    b ^= c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator <<= (const Color3 & c)
{
    r <<= c.r;
    g <<= c.g;
    b <<= c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator -= (Value s)
{
    r -= s;
    g -= s;
    b -= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator *= (Value s)
{
    r *= s;
    g *= s;
    b *= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator /= (Value s)
{
    r /= s;
    g /= s;
    b /= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator %= (Value s)
{
    r %= s;
    g %= s;
    b %= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator &= (Value s)
{
    r &= s;
    g &= s;
    b &= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator |= (Value s)
{
    r |= s;
    g |= s;
    b |= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator ^= (Value s)
{
    r ^= s;
    g ^= s;
    b ^= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color3 & Color3::operator <<= (Value s)
{
    r <<= s;
    g <<= s;
    b <<= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator - (const Color3 & c) const
{
    return Color3(r - c.r, g - c.g, b - c.b);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator * (const Color3 & c) const
{
    return Color3(r * c.r, g * c.g, b * c.b);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator / (const Color3 & c) const
{
    return Color3(r / c.r, g / c.g, b / c.b);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator % (const Color3 & c) const
{
    return Color3(r % c.r, g % c.g, b % c.b);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator & (const Color3 & c) const
{
    return Color3(r & c.r, g & c.g, b & c.b);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator | (const Color3 & c) const
{
    return Color3(r | c.r, g | c.g, b | c.b);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator ^ (const Color3 & c) const
{
    return Color3(r ^ c.r, g ^ c.g, b ^ c.b);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator << (const Color3 & c) const
{
    return Color3(r << c.r, g << c.g, b << c.b);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator >> (const Color3 & c) const
{
    return Color3(r >> c.r, g >> c.g, b >> c.b);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator + (Value s) const
{
    return Color3(r + s, g + s, b + s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator - (Value s) const
{
    return Color3(r - s, g - s, b - s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator * (Value s) const
{
    return Color3(r * s, g * s, b * s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator / (Value s) const
{
    return Color3(r / s, g / s, b / s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator % (Value s) const
{
    return Color3(r % s, g % s, b % s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator & (Value s) const
{
    return Color3(r & s, g & s, b & s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator | (Value s) const
{
    return Color3(r | s, g | s, b | s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator ^ (Value s) const
{
    return Color3(r ^ s, g ^ s, b ^ s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator << (Value s) const
{
    return Color3(r << s, g << s, b << s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator >> (Value s) const
{
    return Color3(r >> s, g >> s, b >> s);
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator + () const
{
    return Color3(r, g, b);
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
bool Color3::operator != (const Color3 & c) const
{
    return (r != c.r) || (g != c.g) || (b != c.b);
}

// ------------------------------------------------------------------------------------------------
bool Color3::operator < (const Color3 & c) const
{
    return (r < c.r) && (g < c.g) && (b < c.b);
}

// ------------------------------------------------------------------------------------------------
bool Color3::operator > (const Color3 & c) const
{
    return (r > c.r) && (g > c.g) && (b > c.b);
}

// ------------------------------------------------------------------------------------------------
bool Color3::operator <= (const Color3 & c) const
{
    return (r <= c.r) && (g <= c.g) && (b <= c.b);
}

// ------------------------------------------------------------------------------------------------
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
CSStr Color3::ToString() const
{
    return ToStrF("%u,%u,%u", r, g, b);
}

// ------------------------------------------------------------------------------------------------
void Color3::SetScalar(Value ns)
{
    r = ns;
    g = ns;
    b = ns;
}

// ------------------------------------------------------------------------------------------------
void Color3::SetColor3(const Color3 & c)
{
    r = c.r;
    g = c.g;
    b = c.b;
}

// ------------------------------------------------------------------------------------------------
void Color3::SetColor3Ex(Value nr, Value ng, Value nb)
{
    r = nr;
    g = ng;
    b = nb;
}

// ------------------------------------------------------------------------------------------------
void Color3::SetColor4(const Color4 & c)
{
    r = c.r;
    g = c.g;
    b = c.b;
}

// ------------------------------------------------------------------------------------------------
void Color3::SetColor4Ex(Value nr, Value ng, Value nb, Value /*na*/)
{
    r = nr;
    g = ng;
    b = nb;
}

// ------------------------------------------------------------------------------------------------
void Color3::SetStr(CSStr str, SQChar delim)
{
    SetColor3(Color3::Get(str, delim));
}

// ------------------------------------------------------------------------------------------------
void Color3::SetName(CSStr name)
{
    SetColor3(GetColor(name));
}

// ------------------------------------------------------------------------------------------------
Uint32 Color3::GetRGB() const
{
    return Uint32(r << 16 | g << 8 | b);
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
void Color3::Generate(Value min, Value max)
{
    if (max < min)
    {
        STHROWF("max value is lower than min value");
    }

    r = GetRandomUint8(min, max);
    g = GetRandomUint8(min, max);
    b = GetRandomUint8(min, max);
}

// ------------------------------------------------------------------------------------------------
void Color3::Generate(Value rmin, Value rmax, Value gmin, Value gmax, Value bmin, Value bmax)
{
    if (rmax < rmin || gmax < gmin || bmax < bmin)
    {
        STHROWF("max value is lower than min value");
    }

    r = GetRandomUint8(rmin, rmax);
    g = GetRandomUint8(gmin, gmax);
    b = GetRandomUint8(bmin, bmax);
}

// ------------------------------------------------------------------------------------------------
void Color3::Random()
{
    SetColor3(GetRandomColor());
}

// ------------------------------------------------------------------------------------------------
void Color3::Inverse()
{
    r = static_cast< Value >(~r);
    g = static_cast< Value >(~g);
    b = static_cast< Value >(~b);
}

// ------------------------------------------------------------------------------------------------
const Color3 & Color3::Get(CSStr str)
{
    return Color3::Get(str, Color3::Delim);
}

// ------------------------------------------------------------------------------------------------
const Color3 & Color3::Get(CSStr str, SQChar delim)
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

// ------------------------------------------------------------------------------------------------
const Color3 & GetColor3()
{
    static Color3 col;
    col.Clear();
    return col;
}

// ------------------------------------------------------------------------------------------------
const Color3 & GetColor3(Uint8 sv)
{
    static Color3 col;
    col.SetScalar(sv);
    return col;
}

// ------------------------------------------------------------------------------------------------
const Color3 & GetColor3(Uint8 rv, Uint8 gv, Uint8 bv)
{
    static Color3 col;
    col.SetColor3Ex(rv, gv, bv);
    return col;
}

// ------------------------------------------------------------------------------------------------
const Color3 & GetColor3(const Color3 & o)
{
    static Color3 col;
    col.SetColor3(o);
    return col;
}

// ================================================================================================
void Register_Color3(HSQUIRRELVM vm)
{
    typedef Color3::Value Val;

    RootTable(vm).Bind(_SC("Color3"), Class< Color3 >(vm, _SC("Color3"))
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val, Val >()
        .Ctor< Val, Val, Val, Val >()
        // Member Variables
        .Var(_SC("r"), &Color3::r)
        .Var(_SC("g"), &Color3::g)
        .Var(_SC("b"), &Color3::b)
        .Var(_SC("R"), &Color3::r)
        .Var(_SC("G"), &Color3::g)
        .Var(_SC("B"), &Color3::b)
        // Core Meta-methods
        .Func(_SC("_tostring"), &Color3::ToString)
        .SquirrelFunc(_SC("_typename"), &Color3::Typename)
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< Color3 >, SQFloat, SQInteger, bool, std::nullptr_t, Color3 >)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< Color3 >, SQFloat, SQInteger, bool, std::nullptr_t, Color3 >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< Color3 >, SQFloat, SQInteger, bool, std::nullptr_t, Color3 >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< Color3 >, SQFloat, SQInteger, bool, std::nullptr_t, Color3 >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< Color3 >, SQFloat, SQInteger, bool, std::nullptr_t, Color3 >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< Color3 >, SQFloat, SQInteger, bool, std::nullptr_t, Color3 >)
        .Func< Color3 (Color3::*)(void) const >(_SC("_unm"), &Color3::operator -)
        // Properties
        .Prop(_SC("RGB"), &Color3::GetRGB, &Color3::SetRGB)
        .Prop(_SC("RGBA"), &Color3::GetRGBA, &Color3::SetRGBA)
        .Prop(_SC("ARGB"), &Color3::GetARGB, &Color3::SetARGB)
        // Member Methods
        .Func(_SC("SetScalar"), &Color3::SetScalar)
        .Func(_SC("SetColor3"), &Color3::SetColor3)
        .Func(_SC("SetColor3Ex"), &Color3::SetColor3Ex)
        .Func(_SC("SetColor4"), &Color3::SetColor4)
        .Func(_SC("SetColor4Ex"), &Color3::SetColor4Ex)
        .Func(_SC("SetStr"), &Color3::SetStr)
        .Func(_SC("SetName"), &Color3::SetName)
        .Func(_SC("Clear"), &Color3::Clear)
        .Func(_SC("Random"), &Color3::Random)
        .Func(_SC("Inverse"), &Color3::Inverse)
        // Member Overloads
        .Overload< void (Color3::*)(void) >(_SC("Generate"), &Color3::Generate)
        .Overload< void (Color3::*)(Val, Val) >(_SC("Generate"), &Color3::Generate)
        .Overload< void (Color3::*)(Val, Val, Val, Val, Val, Val) >(_SC("Generate"), &Color3::Generate)
        // Static Overloads
        .StaticOverload< const Color3 & (*)(CSStr) >(_SC("FromStr"), &Color3::Get)
        .StaticOverload< const Color3 & (*)(CSStr, SQChar) >(_SC("FromStr"), &Color3::Get)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Color3 >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Color3 >)
        // Operator Exposure
        .Func< Color3 & (Color3::*)(const Color3 &) >(_SC("opAddAssign"), &Color3::operator +=)
        .Func< Color3 & (Color3::*)(const Color3 &) >(_SC("opSubAssign"), &Color3::operator -=)
        .Func< Color3 & (Color3::*)(const Color3 &) >(_SC("opMulAssign"), &Color3::operator *=)
        .Func< Color3 & (Color3::*)(const Color3 &) >(_SC("opDivAssign"), &Color3::operator /=)
        .Func< Color3 & (Color3::*)(const Color3 &) >(_SC("opModAssign"), &Color3::operator %=)
        .Func< Color3 & (Color3::*)(const Color3 &) >(_SC("opAndAssign"), &Color3::operator &=)
        .Func< Color3 & (Color3::*)(const Color3 &) >(_SC("opOrAssign"), &Color3::operator |=)
        .Func< Color3 & (Color3::*)(const Color3 &) >(_SC("opXorAssign"), &Color3::operator ^=)
        .Func< Color3 & (Color3::*)(const Color3 &) >(_SC("opShlAssign"), &Color3::operator <<=)
        .Func< Color3 & (Color3::*)(const Color3 &) >(_SC("opShrAssign"), &Color3::operator >>=)
        .Func< Color3 & (Color3::*)(Color3::Value) >(_SC("opAddAssignS"), &Color3::operator +=)
        .Func< Color3 & (Color3::*)(Color3::Value) >(_SC("opSubAssignS"), &Color3::operator -=)
        .Func< Color3 & (Color3::*)(Color3::Value) >(_SC("opMulAssignS"), &Color3::operator *=)
        .Func< Color3 & (Color3::*)(Color3::Value) >(_SC("opDivAssignS"), &Color3::operator /=)
        .Func< Color3 & (Color3::*)(Color3::Value) >(_SC("opModAssignS"), &Color3::operator %=)
        .Func< Color3 & (Color3::*)(Color3::Value) >(_SC("opAndAssignS"), &Color3::operator &=)
        .Func< Color3 & (Color3::*)(Color3::Value) >(_SC("opOrAssignS"), &Color3::operator |=)
        .Func< Color3 & (Color3::*)(Color3::Value) >(_SC("opXorAssignS"), &Color3::operator ^=)
        .Func< Color3 & (Color3::*)(Color3::Value) >(_SC("opShlAssignS"), &Color3::operator <<=)
        .Func< Color3 & (Color3::*)(Color3::Value) >(_SC("opShrAssignS"), &Color3::operator >>=)
        .Func< Color3 & (Color3::*)(void) >(_SC("opPreInc"), &Color3::operator ++)
        .Func< Color3 & (Color3::*)(void) >(_SC("opPreDec"), &Color3::operator --)
        .Func< Color3 (Color3::*)(int) >(_SC("opPostInc"), &Color3::operator ++)
        .Func< Color3 (Color3::*)(int) >(_SC("opPostDec"), &Color3::operator --)
        .Func< Color3 (Color3::*)(const Color3 &) const >(_SC("opAdd"), &Color3::operator +)
        .Func< Color3 (Color3::*)(const Color3 &) const >(_SC("opSub"), &Color3::operator -)
        .Func< Color3 (Color3::*)(const Color3 &) const >(_SC("opMul"), &Color3::operator *)
        .Func< Color3 (Color3::*)(const Color3 &) const >(_SC("opDiv"), &Color3::operator /)
        .Func< Color3 (Color3::*)(const Color3 &) const >(_SC("opMod"), &Color3::operator %)
        .Func< Color3 (Color3::*)(const Color3 &) const >(_SC("opAnd"), &Color3::operator &)
        .Func< Color3 (Color3::*)(const Color3 &) const >(_SC("opOr"), &Color3::operator |)
        .Func< Color3 (Color3::*)(const Color3 &) const >(_SC("opShl"), &Color3::operator ^)
        .Func< Color3 (Color3::*)(const Color3 &) const >(_SC("opShl"), &Color3::operator <<)
        .Func< Color3 (Color3::*)(const Color3 &) const >(_SC("opShr"), &Color3::operator >>)
        .Func< Color3 (Color3::*)(Color3::Value) const >(_SC("opAddS"), &Color3::operator +)
        .Func< Color3 (Color3::*)(Color3::Value) const >(_SC("opSubS"), &Color3::operator -)
        .Func< Color3 (Color3::*)(Color3::Value) const >(_SC("opMulS"), &Color3::operator *)
        .Func< Color3 (Color3::*)(Color3::Value) const >(_SC("opDivS"), &Color3::operator /)
        .Func< Color3 (Color3::*)(Color3::Value) const >(_SC("opModS"), &Color3::operator %)
        .Func< Color3 (Color3::*)(Color3::Value) const >(_SC("opAndS"), &Color3::operator &)
        .Func< Color3 (Color3::*)(Color3::Value) const >(_SC("opOrS"), &Color3::operator |)
        .Func< Color3 (Color3::*)(Color3::Value) const >(_SC("opShlS"), &Color3::operator ^)
        .Func< Color3 (Color3::*)(Color3::Value) const >(_SC("opShlS"), &Color3::operator <<)
        .Func< Color3 (Color3::*)(Color3::Value) const >(_SC("opShrS"), &Color3::operator >>)
        .Func< Color3 (Color3::*)(void) const >(_SC("opUnPlus"), &Color3::operator +)
        .Func< Color3 (Color3::*)(void) const >(_SC("opUnMinus"), &Color3::operator -)
        .Func< Color3 (Color3::*)(void) const >(_SC("opCom"), &Color3::operator ~)
        .Func< bool (Color3::*)(const Color3 &) const >(_SC("opEqual"), &Color3::operator ==)
        .Func< bool (Color3::*)(const Color3 &) const >(_SC("opNotEqual"), &Color3::operator !=)
        .Func< bool (Color3::*)(const Color3 &) const >(_SC("opLessThan"), &Color3::operator <)
        .Func< bool (Color3::*)(const Color3 &) const >(_SC("opGreaterThan"), &Color3::operator >)
        .Func< bool (Color3::*)(const Color3 &) const >(_SC("opLessEqual"), &Color3::operator <=)
        .Func< bool (Color3::*)(const Color3 &) const >(_SC("opGreaterEqual"), &Color3::operator >=)
    );
}

} // Namespace:: SqMod
