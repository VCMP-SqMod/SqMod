#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Compute the next power of two for the specified number.
*/
SQMOD_NODISCARD inline unsigned int NextPow2(unsigned int num)
{
    --num;
    num |= num >> 1u;
    num |= num >> 2u;
    num |= num >> 4u;
    num |= num >> 8u;
    num |= num >> 16u;
    return ++num;
}

/* ------------------------------------------------------------------------------------------------
 * Retrieve the maximum value of a fundamental type.
*/
template < typename T > SQMOD_NODISCARD constexpr T MaxOf() noexcept
{
    return std::numeric_limits< T >::max();
}

/* ------------------------------------------------------------------------------------------------
 * Retrieve the minimum value of a fundamental type.
*/
template < typename T > SQMOD_NODISCARD constexpr T MinOf() noexcept
{
    return std::numeric_limits< T >::min();
}

/* ------------------------------------------------------------------------------------------------
 * Perform an equality comparison between two values taking into account floating point issues.
*/
template < typename T, typename U > struct EpsCmp
{
    static inline bool EQ(const T & a, const U & b) { return abs(a - b) <= 0; }
    static inline bool LT(const T & a, const U & b) { return !EQ(a, b) && (a < b); }
    static inline bool GT(const T & a, const U & b) { return !EQ(a, b) && (a > b); }
    static inline bool LE(const T & a, const U & b) { return EQ(a, b) || (a < b); }
    static inline bool GE(const T & a, const U & b) { return EQ(a, b) || (a > b); }
};

/* ------------------------------------------------------------------------------------------------
 * Perform an equality comparison between two real values taking into account floating point issues.
*/
template < > struct EpsCmp< float, float > {
    static inline bool EQ(const float & a, const float & b) { return fabsf(a - b) <= 0.000001f; }
    static inline bool LT(const float & a, const float & b) { return !EQ(a, b) && (a - b) < 0.000001f; }
    static inline bool GT(const float & a, const float & b) { return !EQ(a, b) && (a - b) > 0.000001f; }
    static inline bool LE(const float & a, const float & b) { return EQ(a, b) || (a - b) < 0.000001f; }
    static inline bool GE(const float & a, const float & b) { return EQ(a, b) || (a - b) > 0.000001f; }
};

/* ------------------------------------------------------------------------------------------------
 * Perform an equality comparison between two real values taking into account floating point issues.
*/
template < > struct EpsCmp< double, double > {
    static inline bool EQ(const double & a, const double & b) { return fabs(a - b) <= 0.000000001; }
    static inline bool LT(const double & a, const double & b) { return !EQ(a, b) && (a - b) < 0.000000001; }
    static inline bool GT(const double & a, const double & b) { return !EQ(a, b) && (a - b) > 0.000000001; }
    static inline bool LE(const double & a, const double & b) { return EQ(a, b) || (a - b) < 0.000000001; }
    static inline bool GE(const double & a, const double & b) { return EQ(a, b) || (a - b) > 0.000000001; }
};

/* ------------------------------------------------------------------------------------------------
 * Perform an equality comparison between two real values taking into account floating point issues.
*/
template < > struct EpsCmp< float, double > {
    static inline bool EQ(const float & a, const double & b) { return fabs(static_cast< double >(a) - b) <= 0.000001; }
    static inline bool LT(const float & a, const double & b) { return !EQ(a, b) && (static_cast< double >(a) - b) < 0.000001; }
    static inline bool GT(const float & a, const double & b) { return !EQ(a, b) && (static_cast< double >(a) - b) > 0.000001; }
    static inline bool LE(const float & a, const double & b) { return EQ(a, b) || (static_cast< double >(a) - b) < 0.000001; }
    static inline bool GE(const float & a, const double & b) { return EQ(a, b) || (static_cast< double >(a) - b) > 0.000001; }
};

/* ------------------------------------------------------------------------------------------------
 * Perform an equality comparison between two real values taking into account floating point issues.
*/
template < > struct EpsCmp< double, float > {
    static inline bool EQ(const double & a, const float & b) { return fabs(a - static_cast< double >(b)) <= 0.000001; }
    static inline bool LT(const double & a, const float & b) { return !EQ(a, b) && (a - static_cast< double >(b)) < 0.000001; }
    static inline bool GT(const double & a, const float & b) { return !EQ(a, b) && (a - static_cast< double >(b)) > 0.000001; }
    static inline bool LE(const double & a, const float & b) { return EQ(a, b) || (a - static_cast< double >(b)) < 0.000001; }
    static inline bool GE(const double & a, const float & b) { return EQ(a, b) || (a - static_cast< double >(b)) > 0.000001; }
};

/* ------------------------------------------------------------------------------------------------
 * Perform an equality comparison between two real values taking into account floating point issues.
*/
template < typename T > struct EpsCmp< float, T > {
    static inline bool EQ(const float & a, const T & b) { return fabsf(a - static_cast< float >(b)) <= 0.000001f; }
    static inline bool LT(const float & a, const T & b) { return !EQ(a, b) && (a - static_cast< float >(b)) < 0.000001f; }
    static inline bool GT(const float & a, const T & b) { return !EQ(a, b) && (a - static_cast< float >(b)) > 0.000001f; }
    static inline bool LE(const float & a, const T & b) { return EQ(a, b) || (a - static_cast< float >(b)) < 0.000001f; }
    static inline bool GE(const float & a, const T & b) { return EQ(a, b) || (a - static_cast< float >(b)) > 0.000001f; }
};

/* ------------------------------------------------------------------------------------------------
 * Perform an equality comparison between two real values taking into account floating point issues.
*/
template < typename T > struct EpsCmp< T, float > {
    static inline bool EQ(const T & a, const float & b) { return fabsf(static_cast< float >(a) - b) <= 0.000001f; }
    static inline bool LT(const T & a, const float & b) { return !EQ(a, b) && (static_cast< float >(a) - b) < 0.000001f; }
    static inline bool GT(const T & a, const float & b) { return !EQ(a, b) && (static_cast< float >(a) - b) > 0.000001f; }
    static inline bool LE(const T & a, const float & b) { return EQ(a, b) || (static_cast< float >(a) - b) < 0.000001f; }
    static inline bool GE(const T & a, const float & b) { return EQ(a, b) || (static_cast< float >(a) - b) > 0.000001f; }
};

/* ------------------------------------------------------------------------------------------------
 * Perform an equality comparison between two real values taking into account floating point issues.
*/
template < typename T > struct EpsCmp< double, T > {
    static inline bool EQ(const double & a, const T & b) { return fabs(a - static_cast< double >(b)) <= 0.000000001; }
    static inline bool LT(const double & a, const T & b) { return !EQ(a, b) && (a - static_cast< double >(b)) < 0.000000001; }
    static inline bool GT(const double & a, const T & b) { return !EQ(a, b) && (a - static_cast< double >(b)) > 0.000000001; }
    static inline bool LE(const double & a, const T & b) { return EQ(a, b) || (a - static_cast< double >(b)) < 0.000000001; }
    static inline bool GE(const double & a, const T & b) { return EQ(a, b) || (a - static_cast< double >(b)) > 0.000000001; }
};

/* ------------------------------------------------------------------------------------------------
 * Perform an equality comparison between two real values taking into account floating point issues.
*/
template < typename T > struct EpsCmp< T, double > {
    static inline bool EQ(const T & a, const double & b) { return fabs(static_cast< double >(a) - b) <= 0.000000001; }
    static inline bool LT(const T & a, const double & b) { return !EQ(a, b) && (static_cast< double >(a) - b) < 0.000000001; }
    static inline bool GT(const T & a, const double & b) { return !EQ(a, b) && (static_cast< double >(a) - b) > 0.000000001; }
    static inline bool LE(const T & a, const double & b) { return EQ(a, b) || (static_cast< double >(a) - b) < 0.000000001; }
    static inline bool GE(const T & a, const double & b) { return EQ(a, b) || (static_cast< double >(a) - b) > 0.000000001; }
};

/* ------------------------------------------------------------------------------------------------
 * Alias of precise comparison helpers.
*/
template < typename T, typename U > using ECMP = EpsCmp< typename std::decay< T >::type, typename std::decay< U >::type >;

/* ------------------------------------------------------------------------------------------------
 * Perform an equality comparison between two values taking into account floating point issues.
*/
template < typename T, typename U > inline constexpr bool EpsEq(T a, U b)
{
	return ECMP< T, U >::EQ(a, b);
}

/* ------------------------------------------------------------------------------------------------
 * Perform an inequality comparison between two values taking into account floating point issues.
*/
template < typename T, typename U > inline constexpr bool EpsNe(T a, U b)
{
	return ECMP< T, U >::NE(a, b);
}

/* ------------------------------------------------------------------------------------------------
 * Perform a less than comparison between two values taking into account floating point issues.
*/
template < typename T, typename U > inline constexpr bool EpsLt(T a, U b)
{
	return ECMP< T, U >::LT(a, b);
}

/* ------------------------------------------------------------------------------------------------
 * Perform a greater than comparison between two values taking into account floating point issues.
*/
template < typename T, typename U > inline constexpr bool EpsGt(T a, U b)
{
	return ECMP< T, U >::GT(a, b);
}

/* ------------------------------------------------------------------------------------------------
 * Perform a less than or equal comparison between two values taking into account
 * floating point issues.
*/
template < typename T, typename U > inline constexpr bool EpsLtEq(T a, U b)
{
	return ECMP< T, U >::LE(a, b);
}

/* ------------------------------------------------------------------------------------------------
 * Perform a greater than or equal comparison between two values taking into account
 * floating point issues.
*/
template < typename T, typename U > inline constexpr bool EpsGtEq(T a, U b)
{
	return ECMP< T, U >::GE(a, b);
}

/* ------------------------------------------------------------------------------------------------
 * Utility used to convert strings to numeric values and/or backwards.
*/
template < typename T > struct ConvNum;

/* ------------------------------------------------------------------------------------------------
 * Specializations for each numeric type conversion to string and/or backwards.
*/

template <> struct ConvNum< signed char >
{
    static const char * ToStr(signed char v);
    static signed char FromStr(const char * s);
    static signed char FromStr(const char * s, signed int base);
};

template <> struct ConvNum< unsigned char >
{
    static const char * ToStr(unsigned char v);
    static unsigned char FromStr(const char * s);
    static unsigned char FromStr(const char * s, signed int base);
};

template <> struct ConvNum< signed short >
{
    static const char * ToStr(signed short v);
    static signed short FromStr(const char * s);
    static signed short FromStr(const char * s, signed int base);
};

template <> struct ConvNum< unsigned short >
{
    static const char * ToStr(unsigned short v);
    static unsigned short FromStr(const char * s);
    static unsigned short FromStr(const char * s, signed int base);
};

template <> struct ConvNum< signed int >
{
    static const char * ToStr(signed int v);
    static signed int FromStr(const char * s);
    static signed int FromStr(const char * s, signed int base);
};

template <> struct ConvNum< unsigned int >
{
    static const char * ToStr(unsigned int v);
    static unsigned int FromStr(const char * s);
    static unsigned int FromStr(const char * s, signed int base);
};

template <> struct ConvNum< signed long long >
{
    static const char * ToStr(signed long long v);
    static signed long long FromStr(const char * s);
    static signed long long FromStr(const char * s, signed int base);
};

template <> struct ConvNum< unsigned long long >
{
    static const char * ToStr(unsigned long long v);
    static unsigned long long FromStr(const char * s);
    static unsigned long long FromStr(const char * s, signed int base);
};

template <> struct ConvNum< long >
{
    static const char * ToStr(long v);
    static long FromStr(const char * s);
    static long FromStr(const char * s, signed int base);
};

template <> struct ConvNum< unsigned long >
{
    static const char * ToStr(unsigned long v);
    static unsigned long FromStr(const char * s);
    static unsigned long FromStr(const char * s, signed int base);
};

template <> struct ConvNum< float >
{
    static const char * ToStr(float v);
    static float FromStr(const char * s);
    static float FromStr(const char * s, signed int base);
};

template <> struct ConvNum< double >
{
    static const char * ToStr(double v);
    static double FromStr(const char * s);
    static double FromStr(const char * s, signed int base);
};

template <> struct ConvNum< bool >
{
    static const char * ToStr(bool v);
    static bool FromStr(const char * s);
    static bool FromStr(const char * s, signed int base);
};

/* ------------------------------------------------------------------------------------------------
 * Utility used to cast between specialized types and perform proper conversion.
*/
template < typename T > struct ConvTo
{
    // --------------------------------------------------------------------------------------------
    static constexpr T Min = std::numeric_limits< T >::min();
    static constexpr T Max = std::numeric_limits< T >::max();

    // --------------------------------------------------------------------------------------------
    template < typename U > static inline T From(U v)
    {
        if (v > Max)
        {
            return Max;
        }
        else if (v < Min)
        {
            return Min;
        }
        return static_cast< T >(v);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Convert a string to 8/16/32 bit signed integers.
*/
template <> template <> inline signed char ConvTo< signed char >::From< const char * >(const char * v)
{
    return ConvNum< signed char >::FromStr(v);
}

template <> template <> inline signed short ConvTo< signed short >::From< const char * >(const char * v)
{
    return ConvNum< signed short >::FromStr(v);
}

template <> template <> inline signed int ConvTo< signed int >::From< const char * >(const char * v)
{
    return ConvNum< signed int >::FromStr(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert a string to 8/16/32 bit unsigned integers.
*/
template <> template <> inline unsigned char ConvTo< unsigned char >::From< const char * >(const char * v)
{
    return ConvNum< unsigned char >::FromStr(v);
}

template <> template <> inline unsigned short ConvTo< unsigned short >::From< const char * >(const char * v)
{
    return ConvNum< unsigned short >::FromStr(v);
}

template <> template <> inline unsigned int ConvTo< unsigned int >::From< const char * >(const char * v)
{
    return ConvNum< unsigned int >::FromStr(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 8 bit signed integer from any unsigned integer.
*/
template <> template <> inline signed char ConvTo< signed char >::From< unsigned char >(unsigned char v)
{
    return (v >= static_cast< unsigned char >(Max)) ? Max : static_cast< signed char >(v);
}

template <> template <> inline signed char ConvTo< signed char >::From< unsigned short >(unsigned short v)
{
    return (v >= static_cast< unsigned char >(Max)) ? Max : static_cast< signed char >(v);
}

template <> template <> inline signed char ConvTo< signed char >::From< unsigned int >(unsigned int v)
{
    return (v >= static_cast< unsigned char >(Max)) ? Max : static_cast< signed char >(v);
}

template <> template <> inline signed char ConvTo< signed char >::From< unsigned long long >(unsigned long long v)
{
    return (v >= static_cast< unsigned char >(Max)) ? Max : static_cast< signed char >(v);
}

template <> template <> inline signed char ConvTo< signed char >::From< unsigned long >(unsigned long v)
{
    return (v >= static_cast< unsigned char >(Max)) ? Max : static_cast< signed char >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 16 bit signed integer from any unsigned integer.
*/
template <> template <> inline signed short ConvTo< signed short >::From< unsigned char >(unsigned char v)
{
    return static_cast< signed short >(v);
}

template <> template <> inline signed short ConvTo< signed short >::From< unsigned short >(unsigned short v)
{
    return (v >= static_cast< unsigned short >(Max)) ? Max : static_cast< signed short >(v);
}

template <> template <> inline signed short ConvTo< signed short >::From< unsigned int >(unsigned int v)
{
    return (v >= static_cast< unsigned short >(Max)) ? Max : static_cast< signed short >(v);
}

template <> template <> inline signed short ConvTo< signed short >::From< unsigned long long >(unsigned long long v)
{
    return (v >= static_cast< unsigned short >(Max)) ? Max : static_cast< signed short >(v);
}

template <> template <> inline signed short ConvTo< signed short >::From< unsigned long >(unsigned long v)
{
    return (v >= static_cast< unsigned short >(Max)) ? Max : static_cast< signed short >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 32 bit signed integer from any unsigned integer.
*/
template <> template <> inline signed int ConvTo< signed int >::From< unsigned char >(unsigned char v)
{
    return static_cast< signed int >(v);
}

template <> template <> inline signed int ConvTo< signed int >::From< unsigned short >(unsigned short v)
{
    return static_cast< signed int >(v);
}

template <> template <> inline signed int ConvTo< signed int >::From< unsigned int >(unsigned int v)
{
    return (v >= static_cast< unsigned int >(Max)) ? Max : static_cast< signed int >(v);
}

template <> template <> inline signed int ConvTo< signed int >::From< unsigned long long >(unsigned long long v)
{
    return (v >= static_cast< unsigned int >(Max)) ? Max : static_cast< signed int >(v);
}

template <> template <> inline signed int ConvTo< signed int >::From< unsigned long >(unsigned long v)
{
    return (v >= static_cast< unsigned int >(Max)) ? Max : static_cast< signed int >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 8 bit unsigned integer from any signed integer.
*/
template <> template <> inline unsigned char ConvTo< unsigned char >::From< signed char >(signed char v)
{
    return (v <= 0) ? static_cast< unsigned char >(0) : static_cast< unsigned char >(v);
}

template <> template <> inline unsigned char ConvTo< unsigned char >::From< signed short >(signed short v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< signed short >(Max))
    {
        return Max;
    }
    return static_cast< unsigned char >(v);
}

template <> template <> inline unsigned char ConvTo< unsigned char >::From< signed int >(signed int v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< signed short >(Max))
    {
        return Max;
    }
    return static_cast< unsigned char >(v);
}

template <> template <> inline unsigned char ConvTo< unsigned char >::From< signed long long >(signed long long v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< signed short >(Max))
    {
        return Max;
    }
    return static_cast< unsigned char >(v);
}

template <> template <> inline unsigned char ConvTo< unsigned char >::From< long >(long v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< signed short >(Max))
    {
        return Max;
    }
    return static_cast< unsigned char >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 16 bit unsigned integer from any signed integer.
*/
template <> template <> inline unsigned short ConvTo< unsigned short >::From< signed char >(signed char v)
{
    return (v <= 0) ? static_cast< unsigned short >(0) : static_cast< unsigned short >(v);
}

template <> template <> inline unsigned short ConvTo< unsigned short >::From< signed short >(signed short v)
{
    return (v <= 0) ? static_cast< unsigned short >(0) : static_cast< unsigned short >(v);
}

template <> template <> inline unsigned short ConvTo< unsigned short >::From< signed int >(signed int v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< signed int >(Max))
    {
        return Max;
    }
    return static_cast< unsigned short >(v);
}

template <> template <> inline unsigned short ConvTo< unsigned short >::From< signed long long >(signed long long v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< signed int >(Max))
    {
        return Max;
    }
    return static_cast< unsigned short >(v);
}

template <> template <> inline unsigned short ConvTo< unsigned short >::From< long >(long v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< signed int >(Max))
    {
        return Max;
    }
    return static_cast< unsigned short >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 32 bit unsigned integer from any signed integer.
*/
template <> template <> inline unsigned int ConvTo< unsigned int >::From< signed char >(signed char v)
{
    return (v <= 0) ? 0 : static_cast< unsigned int >(v);
}

template <> template <> inline unsigned int ConvTo< unsigned int >::From< signed short >(signed short v)
{
    return (v <= 0) ? 0 : static_cast< unsigned int >(v);
}

template <> template <> inline unsigned int ConvTo< unsigned int >::From< signed int >(signed int v)
{
    return (v <= 0) ? 0 : static_cast< unsigned int >(v);
}

template <> template <> inline unsigned int ConvTo< unsigned int >::From< signed long long >(signed long long v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< signed long long >(Max))
    {
        return Max;
    }
    return static_cast< unsigned int >(v);
}

template <> template <> inline unsigned int ConvTo< unsigned int >::From< long >(long v)
{
#if (ULONG_MAX > UINT_MAX)

    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< signed long long >(Max))
    {
        return Max;
    }
    return static_cast< unsigned int >(v);

#else
    return (v <= 0) ? 0 : static_cast< unsigned int >(v);
#endif
}


/* ------------------------------------------------------------------------------------------------
 * Convert to signed integer from 32 bit floating point number.
*/
template <> template <> inline signed char ConvTo< signed char >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< signed char >(v);
}

template <> template <> inline signed short ConvTo< signed short >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< signed short >(v);
}

template <> template <> inline signed int ConvTo< signed int >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< signed int >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to signed integer from 64 bit floating point number.
*/
template <> template <> inline signed char ConvTo< signed char >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< signed char >(v);
}

template <> template <> inline signed short ConvTo< signed short >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< signed short >(v);
}

template <> template <> inline signed int ConvTo< signed int >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< signed int >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to unsigned integer from 32 bit floating point number.
*/
template <> template <> inline unsigned char ConvTo< unsigned char >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< unsigned char >(v);
}

template <> template <> inline unsigned short ConvTo< unsigned short >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< unsigned short >(v);
}

template <> template <> inline unsigned int ConvTo< unsigned int >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< unsigned int >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to unsigned integer from 64 bit floating point number.
*/
template <> template <> inline unsigned char ConvTo< unsigned char >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< unsigned char >(v);
}

template <> template <> inline unsigned short ConvTo< unsigned short >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< unsigned short >(v);
}

template <> template <> inline unsigned int ConvTo< unsigned int >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< unsigned int >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to 64 bit signed integer.
*/
template <> struct ConvTo< signed long long >
{
    // --------------------------------------------------------------------------------------------
    static constexpr signed long long Min = std::numeric_limits< signed long long >::min();
    static constexpr signed long long Max = std::numeric_limits< signed long long >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline signed long long From(T v)
    {
        return static_cast< signed long long >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline signed long long ConvTo< signed long long >::From< const char * >(const char * v)
{
    return ConvNum< signed long long >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
template <> inline signed long long ConvTo< signed long long >::From< unsigned long long >(unsigned long long v)
{
    return (v >= static_cast< unsigned long long >(Max)) ? Max : static_cast< signed long long >(v);
}

#if (ULONG_MAX > UINT_MAX)

// ------------------------------------------------------------------------------------------------
template <> inline signed long long ConvTo< signed long long >::From< unsigned long >(unsigned long v)
{
    return (v >= static_cast< unsigned long >(Max)) ? Max : static_cast< signed long long >(v);
}

#endif

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to 64 bit unsigned integer.
*/
template <> struct ConvTo< unsigned long long >
{
    // --------------------------------------------------------------------------------------------
    static constexpr unsigned long long Min = std::numeric_limits< unsigned long long >::min();
    static constexpr unsigned long long Max = std::numeric_limits< unsigned long long >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline unsigned long long From(T v)
    {
        return (v <= static_cast< T >(0)) ? 0 : static_cast< unsigned long long >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline unsigned long long ConvTo< unsigned long long >::From< const char * >(const char * v)
{
    return ConvNum< unsigned long long >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
template <> inline unsigned long long ConvTo< unsigned long long >::From< float >(float v)
{
    return From(ConvTo< signed long long >::From(v));
}

// ------------------------------------------------------------------------------------------------
template <> inline unsigned long long ConvTo< unsigned long long >::From< double >(double v)
{
    return From(ConvTo< signed long long >::From(v));
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to signed long integer.
*/
template <> struct ConvTo< long >
{
    // --------------------------------------------------------------------------------------------
    static constexpr long Min = std::numeric_limits< long >::min();
    static constexpr long Max = std::numeric_limits< long >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline long From(T v)
    {
        return static_cast< long >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline long ConvTo< long >::From< const char * >(const char * v)
{
    return ConvNum< long >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
#if (ULONG_MAX <= UINT_MAX)

template <> inline long ConvTo< long >::From< unsigned int >(unsigned int v)
{
    return (v >= static_cast< unsigned int >(Max)) ? Max : static_cast< long >(v);
}

#endif

// ------------------------------------------------------------------------------------------------
template <> inline long ConvTo< long >::From< unsigned long long >(unsigned long long v)
{
    return (v >= static_cast< unsigned long long >(Max)) ? Max : static_cast< long >(v);
}

// ------------------------------------------------------------------------------------------------
template <> inline long ConvTo< long >::From< unsigned long >(unsigned long v)
{
    return (v >= static_cast< unsigned long >(Max)) ? Max : static_cast< long >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to unsigned long integer.
*/
template <> struct ConvTo< unsigned long >
{
    // --------------------------------------------------------------------------------------------
    static constexpr unsigned long Min = std::numeric_limits< unsigned long >::min();
    static constexpr unsigned long Max = std::numeric_limits< unsigned long >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline unsigned long From(T v)
    {
        return (v <= static_cast< T >(0)) ? 0 : static_cast< unsigned long >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline unsigned long ConvTo< unsigned long >::From< const char * >(const char * v)
{
    return ConvNum< unsigned long >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
#if (ULONG_MAX <= UINT_MAX)

template <> inline unsigned long ConvTo< unsigned long >::From< signed long long >(signed long long v)
{
    if (v <= 0)
    {
        return Min;
    }
    else if (v >= static_cast< signed long long >(Max))
    {
        return Max;
    }
    return static_cast< unsigned long >(v);
}

template <> inline unsigned long ConvTo< unsigned long >::From< unsigned long long >(unsigned long long v)
{
    return (v >= Max) ? Max : static_cast< unsigned long >(v);
}

#endif

// ------------------------------------------------------------------------------------------------
template <> inline unsigned long ConvTo< unsigned long >::From< float >(float v)
{
    return From(ConvTo< long >::From(v));
}

// ------------------------------------------------------------------------------------------------
template <> inline unsigned long ConvTo< unsigned long >::From< double >(double v)
{
    return From(ConvTo< long >::From(v));
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to 32 bit floating point value.
*/
template <> struct ConvTo< float >
{
    // --------------------------------------------------------------------------------------------
    static constexpr float Min = std::numeric_limits< float >::lowest();
    static constexpr float Max = std::numeric_limits< float >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline float From(T v)
    {
        return static_cast< float >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline float ConvTo< float >::From< const char * >(const char * v)
{
    return ConvNum< float >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
template <> inline float ConvTo< float >::From< double >(double v)
{
    if (EpsGt(v, Max))
    {
        return Max;
    }
    else if (EpsLt(v, Min))
    {
        return Min;
    }
    return static_cast< float >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to 64 bit floating point value.
*/
template <> struct ConvTo< double >
{
    // --------------------------------------------------------------------------------------------
    static constexpr double Min = std::numeric_limits< double >::lowest();
    static constexpr double Max = std::numeric_limits< double >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline double From(T v)
    {
        return static_cast< double >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline double ConvTo< double >::From< const char * >(const char * v)
{
    return ConvNum< double >::FromStr(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to boolean value.
*/
template <> struct ConvTo< bool >
{
    // --------------------------------------------------------------------------------------------
    template < typename T > static inline bool From(T v)
    {
        return static_cast< bool >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline bool ConvTo< bool >::From< const char * >(const char * v)
{
    return ConvNum< bool >::FromStr(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to string value.
*/
template <> struct ConvTo< char * >
{
    // --------------------------------------------------------------------------------------------
    template < typename T > static inline char * From(T v)
    {
        return const_cast< char * >(ConvNum< T >::ToStr(v));
    }
};

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to string value.
*/
template <> struct ConvTo< const char * >
{
    // --------------------------------------------------------------------------------------------
    template < typename T > static inline const char * From(T v)
    {
        return ConvNum< T >::ToStr(v);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Force a value to be within a certain range.
*/
template< typename T, typename U, typename V > SQMOD_NODISCARD inline T Clamp(T val, U min, V max)
{
    // Is the specified value bellow the minimum?
    if (val < min)
    {
        return min;
    }
    // Is the specified value above the maximum?
    else if (val > max)
    {
        return max;
    }
    // Return the value as is
    return val;
}

/* ------------------------------------------------------------------------------------------------
 * Force a value to be higher then than the imposed limit.
*/
template< typename T, typename U > SQMOD_NODISCARD inline T ClampMin(T val, U min)
{
    // Is the specified value bellow the minimum?
    if (val < min)
    {
        return min;
    }
    // Return the value as is
    return val;
}

/* ------------------------------------------------------------------------------------------------
 * Force a value to be smaller then than the imposed limit.
*/
template< typename T, typename U > SQMOD_NODISCARD inline T ClampMax(T val, U max)
{
    // Is the specified value above the maximum?
    if (val > max)
    {
        return max;
    }
    // Return the value as is
    return val;
}

/* ------------------------------------------------------------------------------------------------
 * Force a value to be within a certain range.
*/
template<> SQMOD_NODISCARD inline float Clamp(float val, float min, float max)
{
    // Is the specified value bellow the minimum?
    if (EpsLt(val, min))
    {
        return min;
    }
    // Is the specified value above the maximum?
    else if (EpsGt(val, max))
    {
        return max;
    }
    // Return the value as is
    return val;
}

/* ------------------------------------------------------------------------------------------------
 * Force a value to be within a certain range.
*/
template<> SQMOD_NODISCARD inline double Clamp(double val, double min, double max)
{
    // Is the specified value bellow the minimum?
    if (EpsLt(val, min))
    {
        return min;
    }
    // Is the specified value above the maximum?
    else if (EpsGt(val, max))
    {
        return max;
    }
    // Return the value as is
    return val;
}

/* ------------------------------------------------------------------------------------------------
 * Force a value to be the boundaries of the specified type.
*/
template < typename T, typename U > inline U ClampL(T v)
{
    return ConvTo< U >::From(v);
}

/* ------------------------------------------------------------------------------------------------
 * Helper class allows the use of functions with ctype style as predicate for algorithms.
*/
struct IsCType
{
    // ------------------------------------------------------------------------------------------------
    typedef int (*CTypeFn)(int); // The signature of a ctype function.

private:

    // ------------------------------------------------------------------------------------------------
    CTypeFn m_Fn; // Pointer to the actual function that does the comparison.

public:

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit IsCType(CTypeFn fn)
        : m_Fn(fn)
    {
        /* ... */
    }

    /* ------------------------------------------------------------------------------------------------
     * Function call operator.
    */
    template < typename T > bool operator () (T c)
    {
        return (m_Fn(static_cast< int >(c)) != 0);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Helper class allows the use of functions with ctype style as predicate for algorithms.
*/
struct IsNotCType
{
    // ------------------------------------------------------------------------------------------------
    typedef int (*CTypeFn)(int); // The signature of a ctype function.

private:

    // ------------------------------------------------------------------------------------------------
    CTypeFn m_Fn; // Pointer to the actual function that does the comparison.

public:

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit IsNotCType(CTypeFn fn)
        : m_Fn(fn)
    {
        /* ... */
    }

    /* ------------------------------------------------------------------------------------------------
     * Function call operator.
    */
    template < typename T > bool operator () (T c)
    {
        return (m_Fn(static_cast< int >(c)) == 0);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Utility implementing RAII to toggle a bit mask on and off at all costs.
*/
template < typename T > struct BitGuard
{
private:

    /* ------------------------------------------------------------------------------------------------
     * The lock to be toggled.
    */
    T & m_Lock;

    /* ------------------------------------------------------------------------------------------------
     * The mask to be applied.
    */
    T   m_Mask;

public:

    /* ------------------------------------------------------------------------------------------------
     * Base constructor.
    */
    BitGuard(T & lock, T mask)
        : m_Lock(lock), m_Mask(mask)
    {
        // Apply the specified mask
        m_Lock |= m_Mask;
    }

    /* ------------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~BitGuard()
    {
        // In case one of the bits was turned off in the meantime
        m_Lock |= m_Mask;
        // Now turn off all the bits in the mask
        m_Lock ^= m_Mask;
    }
};

// ------------------------------------------------------------------------------------------------
typedef BitGuard< unsigned char >    BitGuardU8;
typedef BitGuard< unsigned short >   BitGuardU16;
typedef BitGuard< unsigned int >   BitGuardU32;

/* ------------------------------------------------------------------------------------------------
 * RAII approach to make sure a value is assigned regardless of what exceptions are thrown.
*/
template < typename T > class AutoAssign
{
private:

    // --------------------------------------------------------------------------------------------
    T & m_Var; // Variable to receive the value.
    T   m_Val; // Value to be assigned.

public:

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    AutoAssign(T & variable, T value)
        : m_Var(variable), m_Val(value)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    AutoAssign(T & variable, T value, T start)
        : m_Var(variable), m_Val(value)
    {
        m_Var = start;
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    AutoAssign(const AutoAssign & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    AutoAssign(AutoAssign && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~AutoAssign()
    {
        m_Var = m_Val;
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    AutoAssign & operator = (const AutoAssign & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    AutoAssign & operator = (AutoAssign && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Direct value assignment.
    */
    template < typename U > AutoAssign & operator = (U value)
    {
        m_Var = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Direct value assignment.
    */
    template < typename U > void Set(U value)
    {
        m_Val = value;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Retrieve the string delimiter of a base type.
*/
template < typename T > SQMOD_NODISCARD inline SQInteger SqGetDelimiter()
{
    return T::Delim;
}

/* ------------------------------------------------------------------------------------------------
 * Modify the string delimiter of a base type.
*/
template < typename T > inline void SqSetDelimiter(SQInteger c)
{
    T::Delim = ConvTo< SQChar >::From(c);
}

/* ------------------------------------------------------------------------------------------------
 * A simple implementation of name filtering.
*/
SQMOD_NODISCARD bool NameFilterCheck(const SQChar * filter, const SQChar * name);

/* ------------------------------------------------------------------------------------------------
 * A simple implementation of name filtering without case sensitivity.
*/
SQMOD_NODISCARD bool NameFilterCheckInsensitive(const SQChar * filter, const SQChar * name);

} // Namespace:: SqMod
