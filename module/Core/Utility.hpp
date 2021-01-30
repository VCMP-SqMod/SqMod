#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Compute the next power of two for the specified number.
*/
SQMOD_NODISCARD inline uint32_t NextPow2(uint32_t num)
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

template <> struct ConvNum< int8_t >
{
    static const char * ToStr(int8_t v);
    static int8_t FromStr(const char * s);
    static int8_t FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< uint8_t >
{
    static const char * ToStr(uint8_t v);
    static uint8_t FromStr(const char * s);
    static uint8_t FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< int16_t >
{
    static const char * ToStr(int16_t v);
    static int16_t FromStr(const char * s);
    static int16_t FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< uint16_t >
{
    static const char * ToStr(uint16_t v);
    static uint16_t FromStr(const char * s);
    static uint16_t FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< int32_t >
{
    static const char * ToStr(int32_t v);
    static int32_t FromStr(const char * s);
    static int32_t FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< uint32_t >
{
    static const char * ToStr(uint32_t v);
    static uint32_t FromStr(const char * s);
    static uint32_t FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< int64_t >
{
    static const char * ToStr(int64_t v);
    static int64_t FromStr(const char * s);
    static int64_t FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< uint64_t >
{
    static const char * ToStr(uint64_t v);
    static uint64_t FromStr(const char * s);
    static uint64_t FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< long >
{
    static const char * ToStr(long v);
    static long FromStr(const char * s);
    static long FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< unsigned long >
{
    static const char * ToStr(unsigned long v);
    static unsigned long FromStr(const char * s);
    static unsigned long FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< float >
{
    static const char * ToStr(float v);
    static float FromStr(const char * s);
    static float FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< double >
{
    static const char * ToStr(double v);
    static double FromStr(const char * s);
    static double FromStr(const char * s, int32_t base);
};

template <> struct ConvNum< bool >
{
    static const char * ToStr(bool v);
    static bool FromStr(const char * s);
    static bool FromStr(const char * s, int32_t base);
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
template <> template <> inline int8_t ConvTo< int8_t >::From< const char * >(const char * v)
{
    return ConvNum< int8_t >::FromStr(v);
}

template <> template <> inline int16_t ConvTo< int16_t >::From< const char * >(const char * v)
{
    return ConvNum< int16_t >::FromStr(v);
}

template <> template <> inline int32_t ConvTo< int32_t >::From< const char * >(const char * v)
{
    return ConvNum< int32_t >::FromStr(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert a string to 8/16/32 bit unsigned integers.
*/
template <> template <> inline uint8_t ConvTo< uint8_t >::From< const char * >(const char * v)
{
    return ConvNum< uint8_t >::FromStr(v);
}

template <> template <> inline uint16_t ConvTo< uint16_t >::From< const char * >(const char * v)
{
    return ConvNum< uint16_t >::FromStr(v);
}

template <> template <> inline uint32_t ConvTo< uint32_t >::From< const char * >(const char * v)
{
    return ConvNum< uint32_t >::FromStr(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 8 bit signed integer from any unsigned integer.
*/
template <> template <> inline int8_t ConvTo< int8_t >::From< uint8_t >(uint8_t v)
{
    return (v >= static_cast< uint8_t >(Max)) ? Max : static_cast< int8_t >(v);
}

template <> template <> inline int8_t ConvTo< int8_t >::From< uint16_t >(uint16_t v)
{
    return (v >= static_cast< uint8_t >(Max)) ? Max : static_cast< int8_t >(v);
}

template <> template <> inline int8_t ConvTo< int8_t >::From< uint32_t >(uint32_t v)
{
    return (v >= static_cast< uint8_t >(Max)) ? Max : static_cast< int8_t >(v);
}

template <> template <> inline int8_t ConvTo< int8_t >::From< uint64_t >(uint64_t v)
{
    return (v >= static_cast< uint8_t >(Max)) ? Max : static_cast< int8_t >(v);
}

template <> template <> inline int8_t ConvTo< int8_t >::From< unsigned long >(unsigned long v)
{
    return (v >= static_cast< uint8_t >(Max)) ? Max : static_cast< int8_t >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 16 bit signed integer from any unsigned integer.
*/
template <> template <> inline int16_t ConvTo< int16_t >::From< uint8_t >(uint8_t v)
{
    return static_cast< int16_t >(v);
}

template <> template <> inline int16_t ConvTo< int16_t >::From< uint16_t >(uint16_t v)
{
    return (v >= static_cast< uint16_t >(Max)) ? Max : static_cast< int16_t >(v);
}

template <> template <> inline int16_t ConvTo< int16_t >::From< uint32_t >(uint32_t v)
{
    return (v >= static_cast< uint16_t >(Max)) ? Max : static_cast< int16_t >(v);
}

template <> template <> inline int16_t ConvTo< int16_t >::From< uint64_t >(uint64_t v)
{
    return (v >= static_cast< uint16_t >(Max)) ? Max : static_cast< int16_t >(v);
}

template <> template <> inline int16_t ConvTo< int16_t >::From< unsigned long >(unsigned long v)
{
    return (v >= static_cast< uint16_t >(Max)) ? Max : static_cast< int16_t >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 32 bit signed integer from any unsigned integer.
*/
template <> template <> inline int32_t ConvTo< int32_t >::From< uint8_t >(uint8_t v)
{
    return static_cast< int32_t >(v);
}

template <> template <> inline int32_t ConvTo< int32_t >::From< uint16_t >(uint16_t v)
{
    return static_cast< int32_t >(v);
}

template <> template <> inline int32_t ConvTo< int32_t >::From< uint32_t >(uint32_t v)
{
    return (v >= static_cast< uint32_t >(Max)) ? Max : static_cast< int32_t >(v);
}

template <> template <> inline int32_t ConvTo< int32_t >::From< uint64_t >(uint64_t v)
{
    return (v >= static_cast< uint32_t >(Max)) ? Max : static_cast< int32_t >(v);
}

template <> template <> inline int32_t ConvTo< int32_t >::From< unsigned long >(unsigned long v)
{
    return (v >= static_cast< uint32_t >(Max)) ? Max : static_cast< int32_t >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 8 bit unsigned integer from any signed integer.
*/
template <> template <> inline uint8_t ConvTo< uint8_t >::From< int8_t >(int8_t v)
{
    return (v <= 0) ? static_cast< uint8_t >(0) : static_cast< uint8_t >(v);
}

template <> template <> inline uint8_t ConvTo< uint8_t >::From< int16_t >(int16_t v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< int16_t >(Max))
    {
        return Max;
    }
    return static_cast< uint8_t >(v);
}

template <> template <> inline uint8_t ConvTo< uint8_t >::From< int32_t >(int32_t v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< int16_t >(Max))
    {
        return Max;
    }
    return static_cast< uint8_t >(v);
}

template <> template <> inline uint8_t ConvTo< uint8_t >::From< int64_t >(int64_t v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< int16_t >(Max))
    {
        return Max;
    }
    return static_cast< uint8_t >(v);
}

template <> template <> inline uint8_t ConvTo< uint8_t >::From< long >(long v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< int16_t >(Max))
    {
        return Max;
    }
    return static_cast< uint8_t >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 16 bit unsigned integer from any signed integer.
*/
template <> template <> inline uint16_t ConvTo< uint16_t >::From< int8_t >(int8_t v)
{
    return (v <= 0) ? static_cast< uint16_t >(0) : static_cast< uint16_t >(v);
}

template <> template <> inline uint16_t ConvTo< uint16_t >::From< int16_t >(int16_t v)
{
    return (v <= 0) ? static_cast< uint16_t >(0) : static_cast< uint16_t >(v);
}

template <> template <> inline uint16_t ConvTo< uint16_t >::From< int32_t >(int32_t v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< int32_t >(Max))
    {
        return Max;
    }
    return static_cast< uint16_t >(v);
}

template <> template <> inline uint16_t ConvTo< uint16_t >::From< int64_t >(int64_t v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< int32_t >(Max))
    {
        return Max;
    }
    return static_cast< uint16_t >(v);
}

template <> template <> inline uint16_t ConvTo< uint16_t >::From< long >(long v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< int32_t >(Max))
    {
        return Max;
    }
    return static_cast< uint16_t >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 32 bit unsigned integer from any signed integer.
*/
template <> template <> inline uint32_t ConvTo< uint32_t >::From< int8_t >(int8_t v)
{
    return (v <= 0) ? 0 : static_cast< uint32_t >(v);
}

template <> template <> inline uint32_t ConvTo< uint32_t >::From< int16_t >(int16_t v)
{
    return (v <= 0) ? 0 : static_cast< uint32_t >(v);
}

template <> template <> inline uint32_t ConvTo< uint32_t >::From< int32_t >(int32_t v)
{
    return (v <= 0) ? 0 : static_cast< uint32_t >(v);
}

template <> template <> inline uint32_t ConvTo< uint32_t >::From< int64_t >(int64_t v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< int64_t >(Max))
    {
        return Max;
    }
    return static_cast< uint32_t >(v);
}

template <> template <> inline uint32_t ConvTo< uint32_t >::From< long >(long v)
{
#if (ULONG_MAX > UINT_MAX)

    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< int64_t >(Max))
    {
        return Max;
    }
    return static_cast< uint32_t >(v);

#else
    return (v <= 0) ? 0 : static_cast< uint32_t >(v);
#endif
}


/* ------------------------------------------------------------------------------------------------
 * Convert to signed integer from 32 bit floating point number.
*/
template <> template <> inline int8_t ConvTo< int8_t >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< int8_t >(v);
}

template <> template <> inline int16_t ConvTo< int16_t >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< int16_t >(v);
}

template <> template <> inline int32_t ConvTo< int32_t >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< int32_t >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to signed integer from 64 bit floating point number.
*/
template <> template <> inline int8_t ConvTo< int8_t >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< int8_t >(v);
}

template <> template <> inline int16_t ConvTo< int16_t >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< int16_t >(v);
}

template <> template <> inline int32_t ConvTo< int32_t >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< int32_t >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to unsigned integer from 32 bit floating point number.
*/
template <> template <> inline uint8_t ConvTo< uint8_t >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< uint8_t >(v);
}

template <> template <> inline uint16_t ConvTo< uint16_t >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< uint16_t >(v);
}

template <> template <> inline uint32_t ConvTo< uint32_t >::From< float >(float v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< uint32_t >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to unsigned integer from 64 bit floating point number.
*/
template <> template <> inline uint8_t ConvTo< uint8_t >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< uint8_t >(v);
}

template <> template <> inline uint16_t ConvTo< uint16_t >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< uint16_t >(v);
}

template <> template <> inline uint32_t ConvTo< uint32_t >::From< double >(double v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< uint32_t >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to 64 bit signed integer.
*/
template <> struct ConvTo< int64_t >
{
    // --------------------------------------------------------------------------------------------
    static constexpr int64_t Min = std::numeric_limits< int64_t >::min();
    static constexpr int64_t Max = std::numeric_limits< int64_t >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline int64_t From(T v)
    {
        return static_cast< int64_t >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline int64_t ConvTo< int64_t >::From< const char * >(const char * v)
{
    return ConvNum< int64_t >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
template <> inline int64_t ConvTo< int64_t >::From< uint64_t >(uint64_t v)
{
    return (v >= static_cast< uint64_t >(Max)) ? Max : static_cast< int64_t >(v);
}

#if (ULONG_MAX > UINT_MAX)

// ------------------------------------------------------------------------------------------------
template <> inline int64_t ConvTo< int64_t >::From< unsigned long >(unsigned long v)
{
    return (v >= static_cast< unsigned long >(Max)) ? Max : static_cast< int64_t >(v);
}

#endif

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to 64 bit unsigned integer.
*/
template <> struct ConvTo< uint64_t >
{
    // --------------------------------------------------------------------------------------------
    static constexpr uint64_t Min = std::numeric_limits< uint64_t >::min();
    static constexpr uint64_t Max = std::numeric_limits< uint64_t >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline uint64_t From(T v)
    {
        return (v <= static_cast< T >(0)) ? 0 : static_cast< uint64_t >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline uint64_t ConvTo< uint64_t >::From< const char * >(const char * v)
{
    return ConvNum< uint64_t >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
template <> inline uint64_t ConvTo< uint64_t >::From< float >(float v)
{
    return From(ConvTo< int64_t >::From(v));
}

// ------------------------------------------------------------------------------------------------
template <> inline uint64_t ConvTo< uint64_t >::From< double >(double v)
{
    return From(ConvTo< int64_t >::From(v));
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

template <> inline long ConvTo< long >::From< uint32_t >(uint32_t v)
{
    return (v >= static_cast< uint32_t >(Max)) ? Max : static_cast< long >(v);
}

#endif

// ------------------------------------------------------------------------------------------------
template <> inline long ConvTo< long >::From< uint64_t >(uint64_t v)
{
    return (v >= static_cast< uint64_t >(Max)) ? Max : static_cast< long >(v);
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

template <> inline unsigned long ConvTo< unsigned long >::From< int64_t >(int64_t v)
{
    if (v <= 0)
    {
        return Min;
    }
    else if (v >= static_cast< int64_t >(Max))
    {
        return Max;
    }
    return static_cast< unsigned long >(v);
}

template <> inline unsigned long ConvTo< unsigned long >::From< uint64_t >(uint64_t v)
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
typedef BitGuard< uint8_t >    BitGuardU8;
typedef BitGuard< uint16_t >   BitGuardU16;
typedef BitGuard< uint32_t >   BitGuardU32;

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
