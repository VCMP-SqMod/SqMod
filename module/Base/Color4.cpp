// ------------------------------------------------------------------------------------------------
#include "Base/Color4.hpp"
#include "Base/Color3.hpp"
#include "Base/Shared.hpp"
#include "Base/DynArg.hpp"
#include "Base/Buffer.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("Color4"))

// ------------------------------------------------------------------------------------------------
const Color4 Color4::NIL = Color4();
const Color4 Color4::MIN = Color4(std::numeric_limits< Color4::Value >::min());
const Color4 Color4::MAX = Color4(std::numeric_limits< Color4::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Color4::Delim = ',';

// ------------------------------------------------------------------------------------------------
Color4::Color4() noexcept
    : r(0), g(0), b(0), a(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Color4::Color4(Value sv) noexcept
    : r(sv), g(sv), b(sv), a(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Color4::Color4(Value rv, Value gv, Value bv) noexcept
    : r(rv), g(gv), b(bv), a(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Color4::Color4(Value rv, Value gv, Value bv, Value av) noexcept
    : r(rv), g(gv), b(bv), a(av)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator = (Value s)
{
    r = s;
    g = s;
    b = s;
    a = s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator = (const Color3 & c)
{
    r = c.r;
    g = c.g;
    b = c.b;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator += (const Color4 & c)
{
    r += c.r;
    g += c.g;
    b += c.b;
    a += c.a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator -= (const Color4 & c)
{
    r -= c.r;
    g -= c.g;
    b -= c.b;
    a -= c.a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator *= (const Color4 & c)
{
    r *= c.r;
    g *= c.g;
    b *= c.b;
    a *= c.a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator /= (const Color4 & c)
{
    r /= c.r;
    g /= c.g;
    b /= c.b;
    a /= c.a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator %= (const Color4 & c)
{
    r %= c.r;
    g %= c.g;
    b %= c.b;
    a %= c.a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator &= (const Color4 & c)
{
    r &= c.r;
    g &= c.g;
    b &= c.b;
    a &= c.a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator |= (const Color4 & c)
{
    r |= c.r;
    g |= c.g;
    b |= c.b;
    a |= c.a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator ^= (const Color4 & c)
{
    r ^= c.r;
    g ^= c.g;
    b ^= c.b;
    a ^= c.a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator <<= (const Color4 & c)
{
    r <<= c.r;
    g <<= c.g;
    b <<= c.b;
    a <<= c.a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator >>= (const Color4 & c)
{
    r >>= c.r;
    g >>= c.g;
    b >>= c.b;
    a >>= c.a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator += (Value s)
{
    r += s;
    g += s;
    b += s;
    a += s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator -= (Value s)
{
    r -= s;
    g -= s;
    b -= s;
    a -= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator *= (Value s)
{
    r *= s;
    g *= s;
    b *= s;
    a *= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator /= (Value s)
{
    r /= s;
    g /= s;
    b /= s;
    a /= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator %= (Value s)
{
    r %= s;
    g %= s;
    b %= s;
    a %= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator &= (Value s)
{
    r &= s;
    g &= s;
    b &= s;
    a &= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator |= (Value s)
{
    r |= s;
    g |= s;
    b |= s;
    a |= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator ^= (Value s)
{
    r ^= s;
    g ^= s;
    b ^= s;
    a ^= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator <<= (Value s)
{
    r <<= s;
    g <<= s;
    b <<= s;
    a <<= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator >>= (Value s)
{
    r >>= s;
    g >>= s;
    b >>= s;
    a >>= s;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator ++ ()
{
    ++r;
    ++g;
    ++b;
    ++a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::operator -- ()
{
    --r;
    --g;
    --b;
    --a;
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator ++ (int) // NOLINT(cert-dcl21-cpp)
{
    Color4 state(*this);
    ++r;
    ++g;
    ++b;
    ++a;
    return state;
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator -- (int) // NOLINT(cert-dcl21-cpp)
{
    Color4 state(*this);
    --r;
    --g;
    --b;
    --a;
    return state;
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator + (const Color4 & c) const
{
    return {static_cast<Value>(r + c.r), static_cast<Value>(g + c.g), static_cast<Value>(b + c.b), static_cast<Value>(a + c.a)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator - (const Color4 & c) const
{
    return {static_cast<Value>(r - c.r), static_cast<Value>(g - c.g), static_cast<Value>(b - c.b), static_cast<Value>(a - c.a)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator * (const Color4 & c) const
{
    return {static_cast<Value>(r * c.r), static_cast<Value>(g * c.g), static_cast<Value>(b * c.b), static_cast<Value>(a * c.a)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator / (const Color4 & c) const
{
    return {static_cast<Value>(r / c.r), static_cast<Value>(g / c.g), static_cast<Value>(b / c.b), static_cast<Value>(a / c.a)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator % (const Color4 & c) const
{
    return {static_cast<Value>(r % c.r), static_cast<Value>(g % c.g), static_cast<Value>(b % c.b), static_cast<Value>(a % c.a)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator & (const Color4 & c) const
{
    return {static_cast<Value>(r & c.r), static_cast<Value>(g & c.g), static_cast<Value>(b & c.b), static_cast<Value>(a & c.a)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator | (const Color4 & c) const
{
    return {static_cast<Value>(r | c.r), static_cast<Value>(g | c.g), static_cast<Value>(b | c.b), static_cast<Value>(a | c.a)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator ^ (const Color4 & c) const
{
    return {static_cast<Value>(r ^ c.r), static_cast<Value>(g ^ c.g), static_cast<Value>(b ^ c.b), static_cast<Value>(a ^ c.a)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator << (const Color4 & c) const
{
    return {static_cast<Value>(r << c.r), static_cast<Value>(g << c.g), static_cast<Value>(b << c.b), static_cast<Value>(a << c.a)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator >> (const Color4 & c) const
{
    return {static_cast<Value>(r >> c.r), static_cast<Value>(g >> c.g), static_cast<Value>(b >> c.b), static_cast<Value>(a >> c.a)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator + (Value s) const
{
    return {static_cast<Value>(r + s), static_cast<Value>(g + s), static_cast<Value>(b + s), static_cast<Value>(a + s)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator - (Value s) const
{
    return {static_cast<Value>(r - s), static_cast<Value>(g - s), static_cast<Value>(b - s), static_cast<Value>(a - s)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator * (Value s) const
{
    return {static_cast<Value>(r * s), static_cast<Value>(g * s), static_cast<Value>(b * s), static_cast<Value>(a * s)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator / (Value s) const
{
    return {static_cast<Value>(r / s), static_cast<Value>(g / s), static_cast<Value>(b / s), static_cast<Value>(a / s)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator % (Value s) const
{
    return {static_cast<Value>(r % s), static_cast<Value>(g % s), static_cast<Value>(b % s), static_cast<Value>(a % s)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator & (Value s) const
{
    return {static_cast<Value>(r & s), static_cast<Value>(g & s), static_cast<Value>(b & s), static_cast<Value>(a & s)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator | (Value s) const
{
    return {static_cast<Value>(r | s), static_cast<Value>(g | s), static_cast<Value>(b | s), static_cast<Value>(a | s)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator ^ (Value s) const
{
    return {static_cast<Value>(r ^ s), static_cast<Value>(g ^ s), static_cast<Value>(b ^ s), static_cast<Value>(a ^ s)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator << (Value s) const
{
    return {static_cast<Value>(r << s), static_cast<Value>(g << s), static_cast<Value>(b << s), static_cast<Value>(a << s)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator >> (Value s) const
{
    return {static_cast<Value>(r >> s), static_cast<Value>(g >> s), static_cast<Value>(b >> s), static_cast<Value>(a >> s)};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator + () const
{
    return {r, g, b, a};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator - () const
{
    return {0, 0, 0, 0};
}

// ------------------------------------------------------------------------------------------------
Color4 Color4::operator ~ () const
{
    return {static_cast<Value>(~r), static_cast<Value>(~g), static_cast<Value>(~b), static_cast<Value>(~a)};
}

// ------------------------------------------------------------------------------------------------
bool Color4::operator == (const Color4 & c) const
{
    return (r == c.r) && (g == c.g) && (b == c.b) && (a == c.a);
}

// ------------------------------------------------------------------------------------------------
bool Color4::operator != (const Color4 & c) const
{
    return (r != c.r) || (g != c.g) || (b != c.b) || (a != c.a);
}

// ------------------------------------------------------------------------------------------------
bool Color4::operator < (const Color4 & c) const
{
    return (r < c.r) && (g < c.g) && (b < c.b) && (a < c.a);
}

// ------------------------------------------------------------------------------------------------
bool Color4::operator > (const Color4 & c) const
{
    return (r > c.r) && (g > c.g) && (b > c.b) && (a > c.a);
}

// ------------------------------------------------------------------------------------------------
bool Color4::operator <= (const Color4 & c) const
{
    return (r <= c.r) && (g <= c.g) && (b <= c.b) && (a <= c.a);
}

// ------------------------------------------------------------------------------------------------
bool Color4::operator >= (const Color4 & c) const
{
    return (r >= c.r) && (g >= c.g) && (b >= c.b) && (a >= c.a);
}

// ------------------------------------------------------------------------------------------------
Color4::operator Color3 () const
{
    return {r, g, b};
}

// ------------------------------------------------------------------------------------------------
Int32 Color4::Cmp(const Color4 & o) const
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
CSStr Color4::ToString() const
{
    return ToStrF("%u,%u,%u,%u", r, g, b, a);
}

// ------------------------------------------------------------------------------------------------
void Color4::SetScalar(Value ns)
{
    r = ns;
    g = ns;
    b = ns;
    a = ns;
}

// ------------------------------------------------------------------------------------------------
void Color4::SetColor3(const Color3 & c)
{
    r = c.r;
    g = c.g;
    b = c.b;
    a = 0;
}

// ------------------------------------------------------------------------------------------------
void Color4::SetColor3Ex(Value nr, Value ng, Value nb)
{
    r = nr;
    g = ng;
    b = nb;
}

// ------------------------------------------------------------------------------------------------
void Color4::SetColor4(const Color4 & c)
{
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
}

// ------------------------------------------------------------------------------------------------
void Color4::SetColor4Ex(Value nr, Value ng, Value nb, Value na)
{
    r = nr;
    g = ng;
    b = nb;
    a = na;
}

// ------------------------------------------------------------------------------------------------
void Color4::SetStr(SQChar delim, StackStrF & values)
{
    SetColor4(Color4::GetEx(delim, values));
}

// ------------------------------------------------------------------------------------------------
void Color4::SetName(StackStrF & name)
{
    SetColor3(GetColor(name));
}

// ------------------------------------------------------------------------------------------------
Uint32 Color4::GetRGB() const
{
    return Uint32(r << 16u | g << 8u | b); // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
void Color4::SetRGB(Uint32 p)
{
    r = static_cast< Value >((p >> 16u) & 0xFFu);
    g = static_cast< Value >((p >> 8u) & 0xFFu);
    b = static_cast< Value >((p) & 0xFFu);
}

// ------------------------------------------------------------------------------------------------
Uint32 Color4::GetRGBA() const
{
    return Uint32(r << 24u | g << 16u | b << 8u | a); // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
void Color4::SetRGBA(Uint32 p)
{
    r = static_cast< Value >((p >> 24u) & 0xFFu);
    g = static_cast< Value >((p >> 16u) & 0xFFu);
    b = static_cast< Value >((p >> 8u) & 0xFFu);
    a = static_cast< Value >((p) & 0xFFu);
}

// ------------------------------------------------------------------------------------------------
Uint32 Color4::GetARGB() const
{
    return Uint32(a << 24u | r << 16u | g << 8u | b); // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
void Color4::SetARGB(Uint32 p)
{
    a = static_cast< Value >((p >> 24u) & 0xFFu);
    r = static_cast< Value >((p >> 16u) & 0xFFu);
    g = static_cast< Value >((p >> 8u) & 0xFFu);
    b = static_cast< Value >((p) & 0xFFu);
}

// ------------------------------------------------------------------------------------------------
void Color4::Generate()
{
    r = GetRandomUint8();
    g = GetRandomUint8();
    b = GetRandomUint8();
    a = GetRandomUint8();
}

// ------------------------------------------------------------------------------------------------
void Color4::Generate(Value min, Value max)
{
    if (max < min)
    {
        STHROWF("max value is lower than min value");
    }

    r = GetRandomUint8(min, max);
    g = GetRandomUint8(min, max);
    b = GetRandomUint8(min, max);
    a = GetRandomUint8(min, max);
}

// ------------------------------------------------------------------------------------------------
void Color4::Generate(Value rmin, Value rmax, Value gmin, Value gmax, Value bmin, Value bmax, Value amin, Value amax)
{
    if (rmax < rmin || gmax < gmin || bmax < bmin || amax < amin)
    {
        STHROWF("max value is lower than min value");
    }

    r = GetRandomUint8(rmin, rmax);
    g = GetRandomUint8(gmin, gmax);
    b = GetRandomUint8(bmin, bmax);
    a = GetRandomUint8(bmin, bmax);
}

// ------------------------------------------------------------------------------------------------
void Color4::Random()
{
    SetColor3(GetRandomColor());
}

// ------------------------------------------------------------------------------------------------
void Color4::Inverse()
{
    r = static_cast< Value >(~r);
    g = static_cast< Value >(~g);
    b = static_cast< Value >(~b);
    a = static_cast< Value >(~a);
}

// ------------------------------------------------------------------------------------------------
LightObj Color4::Format(const String & spec, StackStrF & fmt) const
{
    String out;
    // Attempt to build the format string
    if (!BuildFormatString(out, fmt, 4, spec))
    {
        return LightObj{}; // Default to null
    }
    // Empty string is unacceptable
    else if (out.empty())
    {
        STHROWF("Unable to build a valid format string.");
    }
    // Grab a temporary buffer
    Buffer buff(out.size());
    // Generate the string
    Buffer::SzType n = buff.WriteF(0, out.c_str(), r, g, b, a);
    // Did the format failed?
    if (!n && !out.empty())
    {
        STHROWF("Format failed. Please check format specifier and parameter count.");
    }
    // Return the resulted string
    return LightObj{buff.Begin< SQChar >(), static_cast< SQInteger >(n)};
}

// ------------------------------------------------------------------------------------------------
const Color4 & Color4::Get(StackStrF & str)
{
    return Color4::GetEx(Color4::Delim, str);
}

// ------------------------------------------------------------------------------------------------
const Color4 & Color4::GetEx(SQChar delim, StackStrF & str)
{
    static Color4 col;
    // The minimum and maximum values supported by the Color4 type
    static constexpr Uint32 min = std::numeric_limits< Color4::Value >::min();
    static constexpr Uint32 max = std::numeric_limits< Color4::Value >::max();
    // Clear previous values, if any
    col.Clear();
    // Is the specified string empty?
    if (str.mLen <= 0)
    {
        return col; // Return the value as is!
    }
    // The format specifications that will be used to scan the string
    SQChar fs[] = _SC(" %u , %u , %u , %u ");
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    fs[14] = delim;
    // The sscanf function requires at least 32 bit integers
    Uint32 r = 0, g = 0, b = 0, a = 0;
    // Attempt to extract the component values from the specified string
    std::sscanf(str.mPtr, fs, &r, &g, &b, &a);
    // Cast the extracted integers to the value used by the Color4 type
    col.r = static_cast< Color4::Value >(Clamp(r, min, max));
    col.g = static_cast< Color4::Value >(Clamp(g, min, max));
    col.b = static_cast< Color4::Value >(Clamp(b, min, max));
    col.a = static_cast< Color4::Value >(Clamp(a, min, max));
    // Return the resulted value
    return col;
}

// ================================================================================================
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void Register_Color4(HSQUIRRELVM vm)
#pragma clang diagnostic pop
{
    typedef Color4::Value Val;

    RootTable(vm).Bind(Typename::Str,
        Class< Color4 >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< Val >()
        .Ctor< Val, Val, Val >()
        .Ctor< Val, Val, Val, Val >()
        // Member Variables
        .Var(_SC("r"), &Color4::r)
        .Var(_SC("g"), &Color4::g)
        .Var(_SC("b"), &Color4::b)
        .Var(_SC("a"), &Color4::a)
        .Var(_SC("R"), &Color4::r)
        .Var(_SC("G"), &Color4::g)
        .Var(_SC("B"), &Color4::b)
        .Var(_SC("A"), &Color4::a)
        // Core Meta-methods
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< Color4 >, SQFloat, SQInteger, bool, std::nullptr_t, Color4 >)
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Color4::ToString)
        // Meta-methods
        .SquirrelFunc(_SC("_add"), &SqDynArgFwd< SqDynArgAddFn< Color4 >, SQFloat, SQInteger, bool, std::nullptr_t, Color4 >)
        .SquirrelFunc(_SC("_sub"), &SqDynArgFwd< SqDynArgSubFn< Color4 >, SQFloat, SQInteger, bool, std::nullptr_t, Color4 >)
        .SquirrelFunc(_SC("_mul"), &SqDynArgFwd< SqDynArgMulFn< Color4 >, SQFloat, SQInteger, bool, std::nullptr_t, Color4 >)
        .SquirrelFunc(_SC("_div"), &SqDynArgFwd< SqDynArgDivFn< Color4 >, SQFloat, SQInteger, bool, std::nullptr_t, Color4 >)
        .SquirrelFunc(_SC("_modulo"), &SqDynArgFwd< SqDynArgModFn< Color4 >, SQFloat, SQInteger, bool, std::nullptr_t, Color4 >)
        .Func< Color4 (Color4::*)(void) const >(_SC("_unm"), &Color4::operator -)
        // Properties
        .Prop(_SC("RGB"), &Color4::GetRGB, &Color4::SetRGB)
        .Prop(_SC("RGBA"), &Color4::GetRGBA, &Color4::SetRGBA)
        .Prop(_SC("ARGB"), &Color4::GetARGB, &Color4::SetARGB)
        // Member Methods
        .Func(_SC("SetScalar"), &Color4::SetScalar)
        .Func(_SC("SetColor3"), &Color4::SetColor3)
        .Func(_SC("SetColor3Ex"), &Color4::SetColor3Ex)
        .Func(_SC("SetColor4"), &Color4::SetColor4)
        .Func(_SC("SetColor4Ex"), &Color4::SetColor4Ex)
        .FmtFunc(_SC("SetStr"), &Color4::SetStr)
        .FmtFunc(_SC("SetName"), &Color4::SetName)
        .Func(_SC("Clear"), &Color4::Clear)
        .FmtFunc(_SC("Format"), &Color4::Format)
        .Func(_SC("Random"), &Color4::Random)
        .Func(_SC("Inverse"), &Color4::Inverse)
        // Member Overloads
        .Overload< void (Color4::*)(void) >(_SC("Generate"), &Color4::Generate)
        .Overload< void (Color4::*)(Val, Val) >(_SC("Generate"), &Color4::Generate)
        .Overload< void (Color4::*)(Val, Val, Val, Val, Val, Val, Val, Val) >(_SC("Generate"), &Color4::Generate)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Color4 >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Color4 >)
        .StaticFmtFunc(_SC("FromStr"), &Color4::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Color4::GetEx)
        // Operator Exposure
        .Func< Color4 & (Color4::*)(const Color4 &) >(_SC("opAddAssign"), &Color4::operator +=)
        .Func< Color4 & (Color4::*)(const Color4 &) >(_SC("opSubAssign"), &Color4::operator -=)
        .Func< Color4 & (Color4::*)(const Color4 &) >(_SC("opMulAssign"), &Color4::operator *=)
        .Func< Color4 & (Color4::*)(const Color4 &) >(_SC("opDivAssign"), &Color4::operator /=)
        .Func< Color4 & (Color4::*)(const Color4 &) >(_SC("opModAssign"), &Color4::operator %=)
        .Func< Color4 & (Color4::*)(const Color4 &) >(_SC("opAndAssign"), &Color4::operator &=)
        .Func< Color4 & (Color4::*)(const Color4 &) >(_SC("opOrAssign"), &Color4::operator |=)
        .Func< Color4 & (Color4::*)(const Color4 &) >(_SC("opXorAssign"), &Color4::operator ^=)
        .Func< Color4 & (Color4::*)(const Color4 &) >(_SC("opShlAssign"), &Color4::operator <<=)
        .Func< Color4 & (Color4::*)(const Color4 &) >(_SC("opShrAssign"), &Color4::operator >>=)
        .Func< Color4 & (Color4::*)(Color4::Value) >(_SC("opAddAssignS"), &Color4::operator +=)
        .Func< Color4 & (Color4::*)(Color4::Value) >(_SC("opSubAssignS"), &Color4::operator -=)
        .Func< Color4 & (Color4::*)(Color4::Value) >(_SC("opMulAssignS"), &Color4::operator *=)
        .Func< Color4 & (Color4::*)(Color4::Value) >(_SC("opDivAssignS"), &Color4::operator /=)
        .Func< Color4 & (Color4::*)(Color4::Value) >(_SC("opModAssignS"), &Color4::operator %=)
        .Func< Color4 & (Color4::*)(Color4::Value) >(_SC("opAndAssignS"), &Color4::operator &=)
        .Func< Color4 & (Color4::*)(Color4::Value) >(_SC("opOrAssignS"), &Color4::operator |=)
        .Func< Color4 & (Color4::*)(Color4::Value) >(_SC("opXorAssignS"), &Color4::operator ^=)
        .Func< Color4 & (Color4::*)(Color4::Value) >(_SC("opShlAssignS"), &Color4::operator <<=)
        .Func< Color4 & (Color4::*)(Color4::Value) >(_SC("opShrAssignS"), &Color4::operator >>=)
        .Func< Color4 & (Color4::*)(void) >(_SC("opPreInc"), &Color4::operator ++)
        .Func< Color4 & (Color4::*)(void) >(_SC("opPreDec"), &Color4::operator --)
        .Func< Color4 (Color4::*)(int) >(_SC("opPostInc"), &Color4::operator ++)
        .Func< Color4 (Color4::*)(int) >(_SC("opPostDec"), &Color4::operator --)
        .Func< Color4 (Color4::*)(const Color4 &) const >(_SC("opAdd"), &Color4::operator +)
        .Func< Color4 (Color4::*)(const Color4 &) const >(_SC("opSub"), &Color4::operator -)
        .Func< Color4 (Color4::*)(const Color4 &) const >(_SC("opMul"), &Color4::operator *)
        .Func< Color4 (Color4::*)(const Color4 &) const >(_SC("opDiv"), &Color4::operator /)
        .Func< Color4 (Color4::*)(const Color4 &) const >(_SC("opMod"), &Color4::operator %)
        .Func< Color4 (Color4::*)(const Color4 &) const >(_SC("opAnd"), &Color4::operator &)
        .Func< Color4 (Color4::*)(const Color4 &) const >(_SC("opOr"), &Color4::operator |)
        .Func< Color4 (Color4::*)(const Color4 &) const >(_SC("opShl"), &Color4::operator ^)
        .Func< Color4 (Color4::*)(const Color4 &) const >(_SC("opShl"), &Color4::operator <<)
        .Func< Color4 (Color4::*)(const Color4 &) const >(_SC("opShr"), &Color4::operator >>)
        .Func< Color4 (Color4::*)(Color4::Value) const >(_SC("opAddS"), &Color4::operator +)
        .Func< Color4 (Color4::*)(Color4::Value) const >(_SC("opSubS"), &Color4::operator -)
        .Func< Color4 (Color4::*)(Color4::Value) const >(_SC("opMulS"), &Color4::operator *)
        .Func< Color4 (Color4::*)(Color4::Value) const >(_SC("opDivS"), &Color4::operator /)
        .Func< Color4 (Color4::*)(Color4::Value) const >(_SC("opModS"), &Color4::operator %)
        .Func< Color4 (Color4::*)(Color4::Value) const >(_SC("opAndS"), &Color4::operator &)
        .Func< Color4 (Color4::*)(Color4::Value) const >(_SC("opOrS"), &Color4::operator |)
        .Func< Color4 (Color4::*)(Color4::Value) const >(_SC("opShlS"), &Color4::operator ^)
        .Func< Color4 (Color4::*)(Color4::Value) const >(_SC("opShlS"), &Color4::operator <<)
        .Func< Color4 (Color4::*)(Color4::Value) const >(_SC("opShrS"), &Color4::operator >>)
        .Func< Color4 (Color4::*)(void) const >(_SC("opUnPlus"), &Color4::operator +)
        .Func< Color4 (Color4::*)(void) const >(_SC("opUnMinus"), &Color4::operator -)
        .Func< Color4 (Color4::*)(void) const >(_SC("opCom"), &Color4::operator ~)
        .Func< bool (Color4::*)(const Color4 &) const >(_SC("opEqual"), &Color4::operator ==)
        .Func< bool (Color4::*)(const Color4 &) const >(_SC("opNotEqual"), &Color4::operator !=)
        .Func< bool (Color4::*)(const Color4 &) const >(_SC("opLessThan"), &Color4::operator <)
        .Func< bool (Color4::*)(const Color4 &) const >(_SC("opGreaterThan"), &Color4::operator >)
        .Func< bool (Color4::*)(const Color4 &) const >(_SC("opLessEqual"), &Color4::operator <=)
        .Func< bool (Color4::*)(const Color4 &) const >(_SC("opGreaterEqual"), &Color4::operator >=)
    );
}

} // Namespace:: SqMod
