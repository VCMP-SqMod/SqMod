// ------------------------------------------------------------------------------------------------
#include "Base/Color4.hpp"
#include "Base/Color3.hpp"
#include "Base/DynArg.hpp"
#include "Core/Buffer.hpp"
#include "Core/Utility.hpp"
#include "Library/Numeric/Random.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(Typename, _SC("Color4"))

// ------------------------------------------------------------------------------------------------
const Color4 Color4::NIL = Color4();
const Color4 Color4::MIN = Color4(std::numeric_limits< Color4::Value >::min());
const Color4 Color4::MAX = Color4(std::numeric_limits< Color4::Value >::max());

// ------------------------------------------------------------------------------------------------
SQChar Color4::Delim = ',';
bool Color4::UpperCaseHex = false;

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
int32_t Color4::Cmp(const Color4 & o) const
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
String Color4::ToString() const
{
    return fmt::format("{},{},{},{}", r, g, b, a);
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
uint32_t Color4::GetRGB() const
{
    return uint32_t(r << 16u | g << 8u | b); // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
void Color4::SetRGB(uint32_t p)
{
    r = static_cast< Value >((p >> 16u) & 0xFFu);
    g = static_cast< Value >((p >> 8u) & 0xFFu);
    b = static_cast< Value >((p) & 0xFFu);
}

// ------------------------------------------------------------------------------------------------
uint32_t Color4::GetRGBA() const
{
    return uint32_t(r << 24u | g << 16u | b << 8u | a); // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
void Color4::SetRGBA(uint32_t p)
{
    r = static_cast< Value >((p >> 24u) & 0xFFu);
    g = static_cast< Value >((p >> 16u) & 0xFFu);
    b = static_cast< Value >((p >> 8u) & 0xFFu);
    a = static_cast< Value >((p) & 0xFFu);
}

// ------------------------------------------------------------------------------------------------
uint32_t Color4::GetARGB() const
{
    return uint32_t(a << 24u | r << 16u | g << 8u | b); // NOLINT(hicpp-signed-bitwise)
}

// ------------------------------------------------------------------------------------------------
void Color4::SetARGB(uint32_t p)
{
    a = static_cast< Value >((p >> 24u) & 0xFFu);
    r = static_cast< Value >((p >> 16u) & 0xFFu);
    g = static_cast< Value >((p >> 8u) & 0xFFu);
    b = static_cast< Value >((p) & 0xFFu);
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::Generate()
{
    r = GetRandomUint8();
    g = GetRandomUint8();
    b = GetRandomUint8();
    a = GetRandomUint8();
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::GenerateB(Value min, Value max)
{
    if (max < min)
    {
        STHROWF("max value is lower than min value");
    }

    r = GetRandomUint8(min, max);
    g = GetRandomUint8(min, max);
    b = GetRandomUint8(min, max);
    a = GetRandomUint8(min, max);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
Color4 & Color4::GenerateR(Value rmin, Value rmax, Value gmin, Value gmax, Value bmin, Value bmax, Value amin, Value amax)
{
    if (rmax < rmin || gmax < gmin || bmax < bmin || amax < amin)
    {
        STHROWF("max value is lower than min value");
    }

    r = GetRandomUint8(rmin, rmax);
    g = GetRandomUint8(gmin, gmax);
    b = GetRandomUint8(bmin, bmax);
    a = GetRandomUint8(bmin, bmax);
    // Allow chaining
    return *this;
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
LightObj Color4::ToHex() const
{
    SQChar buff[16]; // 9 should be enough
    // Attempt to perform the format in the local buffer
    if (std::snprintf(buff, 16, UpperCaseHex ? "%02X%02X%02X%02X" : "%02x%02x%02x%02x", r, g, b, a) <= 0)
    {
        STHROWF("Format failed (somehow)");
    }
    // Return the resulted string
    return LightObj{buff, -1};
}

// ------------------------------------------------------------------------------------------------
LightObj Color4::ToHex3() const
{
    SQChar buff[16]; // 7 should be enough
    // Attempt to perform the format in the local buffer
    if (std::snprintf(buff, 16, UpperCaseHex ? "%02X%02X%02X" : "%02x%02x%02x", r, g, b) <= 0)
    {
        STHROWF("Format failed (somehow)");
    }
    // Return the resulted string
    return LightObj{buff, -1};
}

// ------------------------------------------------------------------------------------------------
String Color4::Format(StackStrF & str) const
{
    return fmt::format(fmt::runtime(str.ToStr())
        , fmt::arg("r", r)
        , fmt::arg("g", g)
        , fmt::arg("b", b)
        , fmt::arg("a", a)
    );
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
    static constexpr uint32_t min = std::numeric_limits< Color4::Value >::min();
    static constexpr uint32_t max = std::numeric_limits< Color4::Value >::max();
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
    uint32_t r = 0, g = 0, b = 0, a = 0;
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

/* ------------------------------------------------------------------------------------------------
 * Retrieve the hex case preference of the Color4 type.
*/
static bool SqGetColor4UpperCaseHex()
{
    return Color4::UpperCaseHex;
}

/* ------------------------------------------------------------------------------------------------
 * Modify the hex case preference of the Color4 type.
*/
static void SqSetColor4UpperCaseHex(bool t)
{
    Color4::UpperCaseHex = t;
}

// ================================================================================================
void Register_Color4(HSQUIRRELVM vm)
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
        .Prop(_SC("Hex"), &Color4::ToHex)
        .Prop(_SC("Hex3"), &Color4::ToHex3)
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
        .Overload(_SC("Generate"), &Color4::Generate)
        .Overload(_SC("Generate"), &Color4::GenerateB)
        .Overload(_SC("Generate"), &Color4::GenerateR)
        // Static Functions
        .StaticFunc(_SC("GetDelimiter"), &SqGetDelimiter< Color4 >)
        .StaticFunc(_SC("SetDelimiter"), &SqSetDelimiter< Color4 >)
        .StaticFunc(_SC("GetUpperCaseHex"), &SqGetColor4UpperCaseHex)
        .StaticFunc(_SC("SetUpperCaseHex"), &SqSetColor4UpperCaseHex)
        .StaticFmtFunc(_SC("FromStr"), &Color4::Get)
        .StaticFmtFunc(_SC("FromStrEx"), &Color4::GetEx)
    );
}

} // Namespace:: SqMod
