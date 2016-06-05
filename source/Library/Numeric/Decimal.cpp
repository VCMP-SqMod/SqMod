// ------------------------------------------------------------------------------------------------
#include "Library/Numeric/Decimal.hpp"
#include "Library/Numeric/LongInt.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Int64 Decimal::s_Factors[] = {
    static_cast< Int64 >(std::pow(10, 0)),
    static_cast< Int64 >(std::pow(10, 1)),
    static_cast< Int64 >(std::pow(10, 2)),
    static_cast< Int64 >(std::pow(10, 3)),
    static_cast< Int64 >(std::pow(10, 4)),
    static_cast< Int64 >(std::pow(10, 5)),
    static_cast< Int64 >(std::pow(10, 6)),
    static_cast< Int64 >(std::pow(10, 7)),
    static_cast< Int64 >(std::pow(10, 8)),
    static_cast< Int64 >(std::pow(10, 9)),
    static_cast< Int64 >(std::pow(10, 10)),
    static_cast< Int64 >(std::pow(10, 11)),
    static_cast< Int64 >(std::pow(10, 12)),
    static_cast< Int64 >(std::pow(10, 13)),
    static_cast< Int64 >(std::pow(10, 14)),
    static_cast< Int64 >(std::pow(10, 15)),
    static_cast< Int64 >(std::pow(10, 16)),
    static_cast< Int64 >(std::pow(10, 17)),
    static_cast< Int64 >(std::pow(10, 18)),
    static_cast< Int64 >(std::pow(10, 19)),
    static_cast< Int64 >(std::pow(10, 20)),
    static_cast< Int64 >(std::pow(10, 21)),
    static_cast< Int64 >(std::pow(10, 22)),
    static_cast< Int64 >(std::pow(10, 23)),
    static_cast< Int64 >(std::pow(10, 24)),
    static_cast< Int64 >(std::pow(10, 25)),
    static_cast< Int64 >(std::pow(10, 26)),
    static_cast< Int64 >(std::pow(10, 27)),
    static_cast< Int64 >(std::pow(10, 28)),
    static_cast< Int64 >(std::pow(10, 29)),
    static_cast< Int64 >(std::pow(10, 30)),
    static_cast< Int64 >(std::pow(10, 31)),
    static_cast< Int64 >(std::pow(10, 32)),
    static_cast< Int64 >(std::pow(10, 33)),
    static_cast< Int64 >(std::pow(10, 34)),
    static_cast< Int64 >(std::pow(10, 35)),
    static_cast< Int64 >(std::pow(10, 36)),
    static_cast< Int64 >(std::pow(10, 37)),
    static_cast< Int64 >(std::pow(10, 38)),
    static_cast< Int64 >(std::pow(10, 39)),
    static_cast< Int64 >(std::pow(10, 40)),
    static_cast< Int64 >(std::pow(10, 41)),
    static_cast< Int64 >(std::pow(10, 42)),
    static_cast< Int64 >(std::pow(10, 43)),
    static_cast< Int64 >(std::pow(10, 44)),
    static_cast< Int64 >(std::pow(10, 45)),
    static_cast< Int64 >(std::pow(10, 46)),
    static_cast< Int64 >(std::pow(10, 47)),
    static_cast< Int64 >(std::pow(10, 48)),
    static_cast< Int64 >(std::pow(10, 49)),
    static_cast< Int64 >(std::pow(10, 40)),
    static_cast< Int64 >(std::pow(10, 51)),
    static_cast< Int64 >(std::pow(10, 52)),
    static_cast< Int64 >(std::pow(10, 53)),
    static_cast< Int64 >(std::pow(10, 54)),
    static_cast< Int64 >(std::pow(10, 55)),
    static_cast< Int64 >(std::pow(10, 56)),
    static_cast< Int64 >(std::pow(10, 57)),
    static_cast< Int64 >(std::pow(10, 58)),
    static_cast< Int64 >(std::pow(10, 59)),
    static_cast< Int64 >(std::pow(10, 60)),
    static_cast< Int64 >(std::pow(10, 61)),
    static_cast< Int64 >(std::pow(10, 62)),
    static_cast< Int64 >(std::pow(10, 63)),
};

// ------------------------------------------------------------------------------------------------
SQInteger Decimal::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqDecimal");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void Decimal::ValidatePrecision(Uint8 precision)
{
    if (precision >= (sizeof(s_Factors) / sizeof(Int64)))
    {
        STHROWF("Out of range precision: %u >= %u", precision, (sizeof(s_Factors) / sizeof(Int64)));
    }
}

// ------------------------------------------------------------------------------------------------
Int64 Decimal::Convert(const Decimal & dec) const
{
    // Do they already have the same precision
    if (m_Precision == dec.m_Precision)
    {
        return dec.m_Value;
    }
    // Return the value converted to the new precision
    return (dec.m_Value / s_Factors[dec.m_Precision]) * s_Factors[m_Precision];
}

// ------------------------------------------------------------------------------------------------
Int32 Decimal::Compare(const Decimal & o) const
{
    // Convert ti the same precision
    const Int64 b = Convert(o);
    // Perform the comparison
    if (m_Value == b)
    {
        return 0;
    }
    else if (m_Value > b)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
Decimal::Decimal(CSStr value)
    : m_Value(0), m_Precision(0)
{
    // Is there anything to parse at least?
    if (!value)
    {
        STHROWF("Invalid decimal string: null");
    }
    // Skip whitespace characters
    while (std::isspace(*value))
    {
        ++value;
    }
    // Do we still have anything left to parse?
    if (*value == '\0')
    {
        return; // Leave default values
    }
    // Is there anything before the separator?
    if (*value == '.')
    {
        // Extract the amount of precision required
        m_Precision = std::strlen(++value);
        // Extract the value as is
        m_Value = std::strtoul(value, nullptr, 10);
        // Leave the remaining value to default
        return;
    }
    // Find the fraction separator
    CSStr point = std::strchr(value, '.');
    // Was there a fraction separator found?
    if (!point)
    {
        // Extract the value as is
        m_Value = std::strtoul(value, nullptr, 10);
        // Leave the remaining value to default
        return;
    }
    // Extract the amount of precision required
    m_Precision = ConvTo< Uint8 >::From(std::strlen(++point));
    // Validate the obtained precision
    ValidatePrecision(m_Precision);
    // Copy the value upto the the separator
    std::strncpy(GetTempBuff(), value, point - value - 1)[point - value - 1] = '\0';
    // Extract the value itself
    m_Value = (std::strtoul(GetTempBuff(), nullptr, 10)  * s_Factors[m_Precision]);
    m_Value += std::strtoul(point, nullptr, 10);
}

// ------------------------------------------------------------------------------------------------
Decimal::Decimal(SQInteger value, Uint8 precision)
    : m_Value(0), m_Precision(precision)
{
    // Validate the specified precision
    ValidatePrecision(m_Precision);
    // Assign the specified value
    m_Value = (s_Factors[m_Precision] * value);
}

#ifndef _SQ64

// ------------------------------------------------------------------------------------------------
Decimal::Decimal(Int64 value, Uint8 precision)
    : m_Value(0), m_Precision(precision)
{
    // Validate the specified precision
    ValidatePrecision(m_Precision);
    // Assign the specified value
    m_Value = (s_Factors[m_Precision] * value);
}

#endif // _SQ64

// ------------------------------------------------------------------------------------------------
CSStr Decimal::ToString() const
{
    // Generate the string into the temporary buffer
    MakeString(GetTempBuff(), GetTempBuffSize());
    // Return the resulted string
    return GetTempBuff();
}

// ------------------------------------------------------------------------------------------------
SLongInt Decimal::GetConverted(const Decimal & dec) const
{
    return SLongInt(Convert(dec));
}

// ------------------------------------------------------------------------------------------------
SLongInt Decimal::GetFactor() const
{
    return SLongInt(s_Factors[m_Precision]);
}

// ------------------------------------------------------------------------------------------------
SLongInt Decimal::GetUnbiased() const
{
    return SLongInt(m_Value);
}

// ------------------------------------------------------------------------------------------------
void Decimal::MakeString(CStr buffer, Uint32 size) const
{
    const Int64 after = m_Value % s_Factors[m_Precision];
    const Int64 before = (m_Value - after) / s_Factors[m_Precision];
    // Generate the format string
    if (std::snprintf(buffer, size, "%lld.%0*lld", before, m_Precision, after) < 0)
    {
        buffer[0] = '\0'; // At least make sure the string is null terminated
    }
}

// ------------------------------------------------------------------------------------------------
static Decimal Sq_DecimalFromLong(const SLongInt & value, Uint8 precision)
{
    return Decimal(value.GetNum(), precision);
}

// ================================================================================================
void Register_Decimal(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(_SC("SqDecimal"), Class< Decimal >(vm, _SC("SqDecimal"))
        // Constructors
        .Ctor()
        .Ctor< CSStr >()
        .Ctor< SQInteger, Uint8 >()
        // Core Meta-methods
        .Func(_SC("_tostring"), &Decimal::ToString)
        .SquirrelFunc(_SC("_typename"), &Decimal::Typename)
        .Func(_SC("_cmp"), &Decimal::Cmp)
        // Core Functions
        .Func(_SC("tofloat"), &Decimal::GetSqFloat)
        // Meta-methods
        .Func< Decimal (Decimal::*)(const Decimal &) const >(_SC("_add"), &Decimal::operator +)
        .Func< Decimal (Decimal::*)(const Decimal &) const >(_SC("_sub"), &Decimal::operator -)
        .Func< Decimal (Decimal::*)(const Decimal &) const >(_SC("_mul"), &Decimal::operator *)
        .Func< Decimal (Decimal::*)(const Decimal &) const >(_SC("_div"), &Decimal::operator /)
        // Properties
        .Prop(_SC("Factor"), &Decimal::GetFactor)
        .Prop(_SC("Unbiased"), &Decimal::GetUnbiased)
        .Prop(_SC("Precision"), &Decimal::GetPrecision)
        .Prop(_SC("Abs"), &Decimal::GetAbs)
        .Prop(_SC("Float"), &Decimal::GetSqFloat)
        .Prop(_SC("Float32"), &Decimal::GetFloat32)
        .Prop(_SC("Float64"), &Decimal::GetFloat64)
        .Prop(_SC("Str"), &Decimal::ToString)
        // Functions
        .Func(_SC("Convert"), &Decimal::GetConverted)
        // Static Functions
        .StaticFunc(_SC("Long"), &Sq_DecimalFromLong)
        .StaticFunc(_SC("FromLong"), &Sq_DecimalFromLong)
    );
}

} // Namespace:: SqMod
