// ------------------------------------------------------------------------------------------------
#include "Base/Color3.hpp"
#include "Base/Color4.hpp"
#include "Base/Shared.hpp"
#include "Base/DynArg.hpp"
#include "Base/Buffer.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("Color3"))

// ------------------------------------------------------------------------------------------------
const Color3 Color3::NIL = Color3();
const Color3 Color3::MIN = Color3(std::numeric_limits< Color3::Value >::min());
const Color3 Color3::MAX = Color3(std::numeric_limits< Color3::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Color3::Delim = ',';

// ------------------------------------------------------------------------------------------------
Color3::Color3() noexcept
    : r(0), g(0), b(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Color3::Color3(Value sv) noexcept
    : r(sv), g(sv), b(sv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Color3::Color3(Value rv, Value gv, Value bv) noexcept
    : r(rv), g(gv), b(bv)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Color3::Color3(Value rv, Value gv, Value bv, Value /*av*/) noexcept
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
Color3 Color3::operator ++ (int) // NOLINT(cert-dcl21-cpp)
{
    Color3 state(*this);
    ++r;
    ++g;
    ++b;
    return state;
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator -- (int) // NOLINT(cert-dcl21-cpp)
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
    return {static_cast<Value>(r + c.r), static_cast<Value>(g + c.g), static_cast<Value>(b + c.b)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator - (const Color3 & c) const
{
    return {static_cast<Value>(r - c.r), static_cast<Value>(g - c.g), static_cast<Value>(b - c.b)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator * (const Color3 & c) const
{
    return {static_cast<Value>(r * c.r), static_cast<Value>(g * c.g), static_cast<Value>(b * c.b)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator / (const Color3 & c) const
{
    return {static_cast<Value>(r / c.r), static_cast<Value>(g / c.g), static_cast<Value>(b / c.b)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator % (const Color3 & c) const
{
    return {static_cast<Value>(r % c.r), static_cast<Value>(g % c.g), static_cast<Value>(b % c.b)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator & (const Color3 & c) const
{
    return {static_cast<Value>(r & c.r), static_cast<Value>(g & c.g), static_cast<Value>(b & c.b)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator | (const Color3 & c) const
{
    return {static_cast<Value>(r | c.r), static_cast<Value>(g | c.g), static_cast<Value>(b | c.b)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator ^ (const Color3 & c) const
{
    return {static_cast<Value>(r ^ c.r), static_cast<Value>(g ^ c.g), static_cast<Value>(b ^ c.b)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator << (const Color3 & c) const
{
    return {static_cast<Value>(r << c.r), static_cast<Value>(g << c.g), static_cast<Value>(b << c.b)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator >> (const Color3 & c) const
{
    return {static_cast<Value>(r >> c.r), static_cast<Value>(g >> c.g), static_cast<Value>(b >> c.b)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator + (Value s) const
{
    return {static_cast<Value>(r + s), static_cast<Value>(g + s), static_cast<Value>(b + s)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator - (Value s) const
{
    return {static_cast<Value>(r - s), static_cast<Value>(g - s), static_cast<Value>(b - s)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator * (Value s) const
{
    return {static_cast<Value>(r * s), static_cast<Value>(g * s), static_cast<Value>(b * s)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator / (Value s) const
{
    return {static_cast<Value>(r / s), static_cast<Value>(g / s), static_cast<Value>(b / s)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator % (Value s) const
{
    return {static_cast<Value>(r % s), static_cast<Value>(g % s), static_cast<Value>(b % s)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator & (Value s) const
{
    return {static_cast<Value>(r & s), static_cast<Value>(g & s), static_cast<Value>(b & s)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator | (Value s) const
{
    return {static_cast<Value>(r | s), static_cast<Value>(g | s), static_cast<Value>(b | s)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator ^ (Value s) const
{
    return {static_cast<Value>(r ^ s), static_cast<Value>(g ^ s), static_cast<Value>(b ^ s)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator << (Value s) const
{
    return {static_cast<Value>(r << s), static_cast<Value>(g << s), static_cast<Value>(b << s)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator >> (Value s) const
{
    return {static_cast<Value>(r >> s), static_cast<Value>(g >> s), static_cast<Value>(b >> s)};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator + () const
{
    return {r, g, b};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator - () const
{
    return {0, 0, 0};
}

// ------------------------------------------------------------------------------------------------
Color3 Color3::operator ~ () const
{
    return {static_cast<Value>(~r), static_cast<Value>(~g), static_cast<Value>(~b)};
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
    return {r, g, b};
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
void Color3::SetStr(SQChar delim, StackStrF & values)
{
    SetColor3(Color3::GetEx(delim, values));
}

// ------------------------------------------------------------------------------------------------
void Color3::SetName(StackStrF & name)
{
    SetColor3(GetColor(name));
}

// ------------------------------------------------------------------------------------------------
Uint32 Color3::GetRGB() const
{
    return Uint32(r << 16u | g << 8u | b); // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
void Color3::SetRGB(Uint32 p)
{
    r = static_cast< Value >((p >> 16u) & 0xFFu);
    g = static_cast< Value >((p >> 8u) & 0xFFu);
    b = static_cast< Value >((p) & 0xFFu);
}

// ------------------------------------------------------------------------------------------------
Uint32 Color3::GetRGBA() const
{
    return Uint32(r << 24u | g << 16u | b << 8u | 0u); // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
void Color3::SetRGBA(Uint32 p)
{
    r = static_cast< Value >((p >> 24u) & 0xFFu);
    g = static_cast< Value >((p >> 16u) & 0xFFu);
    b = static_cast< Value >((p >> 8u) & 0xFFu);
}

// ------------------------------------------------------------------------------------------------
Uint32 Color3::GetARGB() const
{
    return Uint32(0u << 24u | r << 16u | g << 8u | b); // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
void Color3::SetARGB(Uint32 p)
{
    r = static_cast< Value >((p >> 16u) & 0xFFu);
    g = static_cast< Value >((p >> 8u) & 0xFFu);
    b = static_cast< Value >((p) & 0xFFu);
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
LightObj Color3::Format(const String & spec, StackStrF & fmt) const
{
    String out;
    // Attempt to build the format string
    if (!BuildFormatString(out, fmt, 3, spec))
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
    Buffer::SzType n = buff.WriteF(0, out.c_str(), r, g, b);
    // Did the format failed?
    if (!n && !out.empty())
    {
        STHROWF("Format failed. Please check format specifier and parameter count.");
    }
    // Return the resulted string
    return LightObj{buff.Begin< SQChar >(), static_cast< SQInteger >(n)};
}

// ------------------------------------------------------------------------------------------------
const Color3 & Color3::Get(StackStrF & str)
{
    return Color3::GetEx(Color3::Delim, str);
}

// ------------------------------------------------------------------------------------------------
const Color3 & Color3::GetEx(SQChar delim, StackStrF & str)
{
    static Color3 col;
    // The minimum and maximum values supported by the Color3 type
    static constexpr Uint32 min = std::numeric_limits< Color3::Value >::min();
    static constexpr Uint32 max = std::numeric_limits< Color3::Value >::max();
    // Clear previous values, if any
    col.Clear();
    // Is the specified string empty?
    if (str.mLen <= 0)
    {
        return col; // Return the value as is!
    }
    // The format specifications that will be used to scan the string
    SQChar fs[] = _SC(" %u , %u , %u ");
    // Assign the specified delimiter
    fs[4] = delim;
    fs[9] = delim;
    // The sscanf function requires at least 32 bit integers
    Uint32 r = 0, g = 0, b = 0;
    // Attempt to extract the component values from the specified string
    std::sscanf(str.mPtr, fs, &r, &g, &b);
    // Cast the extracted integers to the value used by the Color3 type
    col.r = static_cast< Color3::Value >(Clamp(r, min, max));
    col.g = static_cast< Color3::Value >(Clamp(g, min, max));
    col.b = static_cast< Color3::Value >(Clamp(b, min, max));
    // Return the resulted value
    return col;
}

// ================================================================================================
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void Register_Color3(HSQUIRRELVM vm)
#pragma clang diagnostic pop
{
    typedef Color3::Value Val;

    RootTable(vm).Bind(Typename::Str,
        Class< Color3 >(vm, Typename::Str)
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
        .SquirrelFunc(_SC("cmp"), &SqDynArgFwd< SqDynArgCmpFn< Color3 >, SQFloat, SQInteger, bool, std::nullptr_t, Color3 >)
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Color3::ToString)
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
        .FmtFunc(_SC("SetStr"), &Color3::SetStr)
        .FmtFunc(_SC("SetName"), &Color3::SetName)
        .Func(_SC("Clear"), &Color3::Clear)
        .FmtFunc(_SC("Format"), &Color3::Format)
        .Func(_SC("Random"), &Color3::Random)
        .Func(_SC("Inverse"), &Color3::Inverse)
        // Member Overloads
        .Overload< void (Color3::*)(void) >(_SC("Generate"), &Color3::Generate)
        .Overload< void (Color3::*)(Val, Val) >(_SC("Generate"), &Color3::Generate)
        .Overload< void (Color3::*)(Val, Val, Val, Val, Val, Val) >(_SC("Generate"), &Color3::Generate)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Color3 >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Color3 >)
        .StaticFmtFunc(_SC("FromStr"), &Color3::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Color3::GetEx)
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
