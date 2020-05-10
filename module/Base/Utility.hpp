#pragma once

// ------------------------------------------------------------------------------------------------
#include <SqBase.hpp>
#include <vcmp.h>

// ------------------------------------------------------------------------------------------------
#include <cmath>
#include <limits>
#include <cassert>

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Buffer;

/* ------------------------------------------------------------------------------------------------
 * Proxies to communicate with the server.
*/
extern PluginFuncs*         _Func;
extern PluginCallbacks*     _Clbk;
extern PluginInfo*          _Info;

/* ------------------------------------------------------------------------------------------------
 * Helper to register pure typename functions for better performance.
*/
#define SQMODE_DECL_TYPENAME(t, s) /*
*/ namespace { /*
*/  struct t { /*
*/   static const SQChar Str[]; /*
*/   static SQInteger Fn(HSQUIRRELVM vm); /*
*/  }; /*
*/  const SQChar t::Str[] = s; /*
*/  SQInteger t::Fn(HSQUIRRELVM vm) { /*
*/   sq_pushstring(vm, Str, sizeof(Str) / sizeof(SQChar)); /*
*/   return 1; /*
*/  } /*
*/ } /*
*/

/* ------------------------------------------------------------------------------------------------
 * Retrieve the temporary buffer.
*/
SStr GetTempBuff();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the size of the temporary buffer.
*/
Uint32 GetTempBuffSize();

/* ------------------------------------------------------------------------------------------------
 * Output a message only if the _DEBUG was defined.
*/
void OutputDebug(const char * msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Output a formatted user message to the console.
*/
void OutputMessage(const char * msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Output a formatted error message to the console.
*/
void OutputError(const char * msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string and throw it as a sqrat exception.
*/
void SqThrowF(CCStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Quickly generate a formatted string on a small static buffer without any memory allocations.
 * NOTE: This does throw an exception in case of failure.
*/
CSStr FmtStr(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Quickly generate a formatted string on a small static buffer without any memory allocations.
 * NOTE: This does not throw an exception in case of failure.
*/
CSStr ToStrF(CCStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string on a temporary buffer and return the string but not the buffer.
 * NOTE: This does not throw an exception in case of failure.
*/
CSStr ToStringF(CCStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string and throw it as a squirrel exception.
*/
SQRESULT SqThrowErrorF(HSQUIRRELVM vm, CCStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Simple function to check whether the specified string can be considered as a boolean value
*/
bool SToB(CSStr str);

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a dummy StackStrF instance.
*/
StackStrF & DummyStackStrF();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null script object.
*/
Object & NullObject();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null script object.
*/
LightObj & NullLightObj();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null/empty script table.
*/
Table & NullTable();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null/empty script array.
*/
Array & NullArray();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null script function.
*/
Function & NullFunction();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null string.
*/
String & NullString();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a static string with a specific value.
*/
String & StringRef(const SQChar * str);

/* ------------------------------------------------------------------------------------------------
 * Compute the next power of two for the specified number.
*/
inline Uint32 NextPow2(Uint32 num)
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
template < typename T > constexpr T MaxOf() noexcept
{
    return std::numeric_limits< T >::max();
}

/* ------------------------------------------------------------------------------------------------
 * Retrieve the minimum value of a fundamental type.
*/
template < typename T > constexpr T MinOf() noexcept
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

template <> struct ConvNum< Int8 >
{
    static CCStr ToStr(Int8 v);
    static Int8 FromStr(CCStr s);
    static Int8 FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< Uint8 >
{
    static CCStr ToStr(Uint8 v);
    static Uint8 FromStr(CCStr s);
    static Uint8 FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< Int16 >
{
    static CCStr ToStr(Int16 v);
    static Int16 FromStr(CCStr s);
    static Int16 FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< Uint16 >
{
    static CCStr ToStr(Uint16 v);
    static Uint16 FromStr(CCStr s);
    static Uint16 FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< Int32 >
{
    static CCStr ToStr(Int32 v);
    static Int32 FromStr(CCStr s);
    static Int32 FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< Uint32 >
{
    static CCStr ToStr(Uint32 v);
    static Uint32 FromStr(CCStr s);
    static Uint32 FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< Int64 >
{
    static CCStr ToStr(Int64 v);
    static Int64 FromStr(CCStr s);
    static Int64 FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< Uint64 >
{
    static CCStr ToStr(Uint64 v);
    static Uint64 FromStr(CCStr s);
    static Uint64 FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< LongI >
{
    static CCStr ToStr(LongI v);
    static LongI FromStr(CCStr s);
    static LongI FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< Ulong >
{
    static CCStr ToStr(Ulong v);
    static Ulong FromStr(CCStr s);
    static Ulong FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< Float32 >
{
    static CCStr ToStr(Float32 v);
    static Float32 FromStr(CCStr s);
    static Float32 FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< Float64 >
{
    static CCStr ToStr(Float64 v);
    static Float64 FromStr(CCStr s);
    static Float64 FromStr(CCStr s, Int32 base);
};

template <> struct ConvNum< bool >
{
    static CCStr ToStr(bool v);
    static bool FromStr(CCStr s);
    static bool FromStr(CCStr s, Int32 base);
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
template <> template <> inline Int8 ConvTo< Int8 >::From< CCStr >(CCStr v)
{
    return ConvNum< Int8 >::FromStr(v);
}

template <> template <> inline Int16 ConvTo< Int16 >::From< CCStr >(CCStr v)
{
    return ConvNum< Int16 >::FromStr(v);
}

template <> template <> inline Int32 ConvTo< Int32 >::From< CCStr >(CCStr v)
{
    return ConvNum< Int32 >::FromStr(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert a string to 8/16/32 bit unsigned integers.
*/
template <> template <> inline Uint8 ConvTo< Uint8 >::From< CCStr >(CCStr v)
{
    return ConvNum< Uint8 >::FromStr(v);
}

template <> template <> inline Uint16 ConvTo< Uint16 >::From< CCStr >(CCStr v)
{
    return ConvNum< Uint16 >::FromStr(v);
}

template <> template <> inline Uint32 ConvTo< Uint32 >::From< CCStr >(CCStr v)
{
    return ConvNum< Uint32 >::FromStr(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 8 bit signed integer from any unsigned integer.
*/
template <> template <> inline Int8 ConvTo< Int8 >::From< Uint8 >(Uint8 v)
{
    return (v >= static_cast< Uint8 >(Max)) ? Max : static_cast< Int8 >(v);
}

template <> template <> inline Int8 ConvTo< Int8 >::From< Uint16 >(Uint16 v)
{
    return (v >= static_cast< Uint8 >(Max)) ? Max : static_cast< Int8 >(v);
}

template <> template <> inline Int8 ConvTo< Int8 >::From< Uint32 >(Uint32 v)
{
    return (v >= static_cast< Uint8 >(Max)) ? Max : static_cast< Int8 >(v);
}

template <> template <> inline Int8 ConvTo< Int8 >::From< Uint64 >(Uint64 v)
{
    return (v >= static_cast< Uint8 >(Max)) ? Max : static_cast< Int8 >(v);
}

template <> template <> inline Int8 ConvTo< Int8 >::From< Ulong >(Ulong v)
{
    return (v >= static_cast< Uint8 >(Max)) ? Max : static_cast< Int8 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 16 bit signed integer from any unsigned integer.
*/
template <> template <> inline Int16 ConvTo< Int16 >::From< Uint8 >(Uint8 v)
{
    return static_cast< Int16 >(v);
}

template <> template <> inline Int16 ConvTo< Int16 >::From< Uint16 >(Uint16 v)
{
    return (v >= static_cast< Uint16 >(Max)) ? Max : static_cast< Int16 >(v);
}

template <> template <> inline Int16 ConvTo< Int16 >::From< Uint32 >(Uint32 v)
{
    return (v >= static_cast< Uint16 >(Max)) ? Max : static_cast< Int16 >(v);
}

template <> template <> inline Int16 ConvTo< Int16 >::From< Uint64 >(Uint64 v)
{
    return (v >= static_cast< Uint16 >(Max)) ? Max : static_cast< Int16 >(v);
}

template <> template <> inline Int16 ConvTo< Int16 >::From< Ulong >(Ulong v)
{
    return (v >= static_cast< Uint16 >(Max)) ? Max : static_cast< Int16 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 32 bit signed integer from any unsigned integer.
*/
template <> template <> inline Int32 ConvTo< Int32 >::From< Uint8 >(Uint8 v)
{
    return static_cast< Int32 >(v);
}

template <> template <> inline Int32 ConvTo< Int32 >::From< Uint16 >(Uint16 v)
{
    return static_cast< Int32 >(v);
}

template <> template <> inline Int32 ConvTo< Int32 >::From< Uint32 >(Uint32 v)
{
    return (v >= static_cast< Uint32 >(Max)) ? Max : static_cast< Int32 >(v);
}

template <> template <> inline Int32 ConvTo< Int32 >::From< Uint64 >(Uint64 v)
{
    return (v >= static_cast< Uint32 >(Max)) ? Max : static_cast< Int32 >(v);
}

template <> template <> inline Int32 ConvTo< Int32 >::From< Ulong >(Ulong v)
{
    return (v >= static_cast< Uint32 >(Max)) ? Max : static_cast< Int32 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 8 bit unsigned integer from any signed integer.
*/
template <> template <> inline Uint8 ConvTo< Uint8 >::From< Int8 >(Int8 v)
{
    return (v <= 0) ? 0 : static_cast< Uint8 >(v);
}

template <> template <> inline Uint8 ConvTo< Uint8 >::From< Int16 >(Int16 v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int16 >(Max))
    {
        return Max;
    }
    return static_cast< Uint8 >(v);
}

template <> template <> inline Uint8 ConvTo< Uint8 >::From< Int32 >(Int32 v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int16 >(Max))
    {
        return Max;
    }
    return static_cast< Uint8 >(v);
}

template <> template <> inline Uint8 ConvTo< Uint8 >::From< Int64 >(Int64 v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int16 >(Max))
    {
        return Max;
    }
    return static_cast< Uint8 >(v);
}

template <> template <> inline Uint8 ConvTo< Uint8 >::From< LongI >(LongI v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int16 >(Max))
    {
        return Max;
    }
    return static_cast< Uint8 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 16 bit unsigned integer from any signed integer.
*/
template <> template <> inline Uint16 ConvTo< Uint16 >::From< Int8 >(Int8 v)
{
    return (v <= 0) ? 0 : static_cast< Uint16 >(v);
}

template <> template <> inline Uint16 ConvTo< Uint16 >::From< Int16 >(Int16 v)
{
    return (v <= 0) ? 0 : static_cast< Uint16 >(v);
}

template <> template <> inline Uint16 ConvTo< Uint16 >::From< Int32 >(Int32 v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int32 >(Max))
    {
        return Max;
    }
    return static_cast< Uint16 >(v);
}

template <> template <> inline Uint16 ConvTo< Uint16 >::From< Int64 >(Int64 v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int32 >(Max))
    {
        return Max;
    }
    return static_cast< Uint16 >(v);
}

template <> template <> inline Uint16 ConvTo< Uint16 >::From< LongI >(LongI v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int32 >(Max))
    {
        return Max;
    }
    return static_cast< Uint16 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to 32 bit unsigned integer from any signed integer.
*/
template <> template <> inline Uint32 ConvTo< Uint32 >::From< Int8 >(Int8 v)
{
    return (v <= 0) ? 0 : static_cast< Uint32 >(v);
}

template <> template <> inline Uint32 ConvTo< Uint32 >::From< Int16 >(Int16 v)
{
    return (v <= 0) ? 0 : static_cast< Uint32 >(v);
}

template <> template <> inline Uint32 ConvTo< Uint32 >::From< Int32 >(Int32 v)
{
    return (v <= 0) ? 0 : static_cast< Uint32 >(v);
}

template <> template <> inline Uint32 ConvTo< Uint32 >::From< Int64 >(Int64 v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int64 >(Max))
    {
        return Max;
    }
    return static_cast< Uint32 >(v);
}

template <> template <> inline Uint32 ConvTo< Uint32 >::From< LongI >(LongI v)
{
#if (ULONG_MAX > UINT_MAX)

    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int64 >(Max))
    {
        return Max;
    }
    return static_cast< Uint32 >(v);

#else
    return (v <= 0) ? 0 : static_cast< Uint32 >(v);
#endif
}


/* ------------------------------------------------------------------------------------------------
 * Convert to signed integer from 32 bit floating point number.
*/
template <> template <> inline Int8 ConvTo< Int8 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Int8 >(v);
}

template <> template <> inline Int16 ConvTo< Int16 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Int16 >(v);
}

template <> template <> inline Int32 ConvTo< Int32 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Int32 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to signed integer from 64 bit floating point number.
*/
template <> template <> inline Int8 ConvTo< Int8 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Int8 >(v);
}

template <> template <> inline Int16 ConvTo< Int16 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Int16 >(v);
}

template <> template <> inline Int32 ConvTo< Int32 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Int32 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to unsigned integer from 32 bit floating point number.
*/
template <> template <> inline Uint8 ConvTo< Uint8 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Uint8 >(v);
}

template <> template <> inline Uint16 ConvTo< Uint16 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Uint16 >(v);
}

template <> template <> inline Uint32 ConvTo< Uint32 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Uint32 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert to unsigned integer from 64 bit floating point number.
*/
template <> template <> inline Uint8 ConvTo< Uint8 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Uint8 >(v);
}

template <> template <> inline Uint16 ConvTo< Uint16 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Uint16 >(v);
}

template <> template <> inline Uint32 ConvTo< Uint32 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, Min))
    {
        return Min;
    }
    else if (EpsGt(v, Max))
    {
        return Max;
    }
    return static_cast< Uint32 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to 64 bit signed integer.
*/
template <> struct ConvTo< Int64 >
{
    // --------------------------------------------------------------------------------------------
    static constexpr Int64 Min = std::numeric_limits< Int64 >::min();
    static constexpr Int64 Max = std::numeric_limits< Int64 >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline Int64 From(T v)
    {
        return static_cast< Int64 >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline Int64 ConvTo< Int64 >::From< CCStr >(CCStr v)
{
    return ConvNum< Int64 >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
template <> inline Int64 ConvTo< Int64 >::From< Uint64 >(Uint64 v)
{
    return (v >= static_cast< Uint64 >(Max)) ? Max : static_cast< Int64 >(v);
}

#if (ULONG_MAX > UINT_MAX)

// ------------------------------------------------------------------------------------------------
template <> inline Int64 ConvTo< Int64 >::From< Ulong >(Ulong v)
{
    return (v >= static_cast< Ulong >(Max)) ? Max : static_cast< Int64 >(v);
}

#endif

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to 64 bit unsigned integer.
*/
template <> struct ConvTo< Uint64 >
{
    // --------------------------------------------------------------------------------------------
    static constexpr Uint64 Min = std::numeric_limits< Uint64 >::min();
    static constexpr Uint64 Max = std::numeric_limits< Uint64 >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline Uint64 From(T v)
    {
        return (v <= static_cast< T >(0)) ? 0 : static_cast< Uint64 >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline Uint64 ConvTo< Uint64 >::From< CCStr >(CCStr v)
{
    return ConvNum< Uint64 >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
template <> inline Uint64 ConvTo< Uint64 >::From< Float32 >(Float32 v)
{
    return From(ConvTo< Int64 >::From(v));
}

// ------------------------------------------------------------------------------------------------
template <> inline Uint64 ConvTo< Uint64 >::From< Float64 >(Float64 v)
{
    return From(ConvTo< Int64 >::From(v));
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to signed long integer.
*/
template <> struct ConvTo< LongI >
{
    // --------------------------------------------------------------------------------------------
    static constexpr LongI Min = std::numeric_limits< LongI >::min();
    static constexpr LongI Max = std::numeric_limits< LongI >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline LongI From(T v)
    {
        return static_cast< LongI >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline LongI ConvTo< LongI >::From< CCStr >(CCStr v)
{
    return ConvNum< LongI >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
#if (ULONG_MAX <= UINT_MAX)

template <> inline LongI ConvTo< LongI >::From< Uint32 >(Uint32 v)
{
    return (v >= static_cast< Uint32 >(Max)) ? Max : static_cast< LongI >(v);
}

#endif

// ------------------------------------------------------------------------------------------------
template <> inline LongI ConvTo< LongI >::From< Uint64 >(Uint64 v)
{
    return (v >= static_cast< Uint64 >(Max)) ? Max : static_cast< LongI >(v);
}

// ------------------------------------------------------------------------------------------------
template <> inline LongI ConvTo< LongI >::From< Ulong >(Ulong v)
{
    return (v >= static_cast< Ulong >(Max)) ? Max : static_cast< LongI >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to unsigned long integer.
*/
template <> struct ConvTo< Ulong >
{
    // --------------------------------------------------------------------------------------------
    static constexpr Ulong Min = std::numeric_limits< Ulong >::min();
    static constexpr Ulong Max = std::numeric_limits< Ulong >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline Ulong From(T v)
    {
        return (v <= static_cast< T >(0)) ? 0 : static_cast< Ulong >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline Ulong ConvTo< Ulong >::From< CCStr >(CCStr v)
{
    return ConvNum< Ulong >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
#if (ULONG_MAX <= UINT_MAX)

template <> inline Ulong ConvTo< Ulong >::From< Int64 >(Int64 v)
{
    if (v <= 0)
    {
        return Min;
    }
    else if (v >= static_cast< Int64 >(Max))
    {
        return Max;
    }
    return static_cast< Ulong >(v);
}

template <> inline Ulong ConvTo< Ulong >::From< Uint64 >(Uint64 v)
{
    return (v >= Max) ? Max : static_cast< Ulong >(v);
}

#endif

// ------------------------------------------------------------------------------------------------
template <> inline Ulong ConvTo< Ulong >::From< Float32 >(Float32 v)
{
    return From(ConvTo< LongI >::From(v));
}

// ------------------------------------------------------------------------------------------------
template <> inline Ulong ConvTo< Ulong >::From< Float64 >(Float64 v)
{
    return From(ConvTo< LongI >::From(v));
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to 32 bit floating point value.
*/
template <> struct ConvTo< Float32 >
{
    // --------------------------------------------------------------------------------------------
    static constexpr Float32 Min = std::numeric_limits< Float32 >::lowest();
    static constexpr Float32 Max = std::numeric_limits< Float32 >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline Float32 From(T v)
    {
        return static_cast< Float32 >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline Float32 ConvTo< Float32 >::From< CCStr >(CCStr v)
{
    return ConvNum< Float32 >::FromStr(v);
}

// ------------------------------------------------------------------------------------------------
template <> inline Float32 ConvTo< Float32 >::From< Float64 >(Float64 v)
{
    if (EpsGt(v, Max))
    {
        return Max;
    }
    else if (EpsLt(v, Min))
    {
        return Min;
    }
    return static_cast< Float32 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to 64 bit floating point value.
*/
template <> struct ConvTo< Float64 >
{
    // --------------------------------------------------------------------------------------------
    static constexpr Float64 Min = std::numeric_limits< Float64 >::lowest();
    static constexpr Float64 Max = std::numeric_limits< Float64 >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline Float64 From(T v)
    {
        return static_cast< Float64 >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline Float64 ConvTo< Float64 >::From< CCStr >(CCStr v)
{
    return ConvNum< Float64 >::FromStr(v);
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
template <> inline bool ConvTo< bool >::From< CCStr >(CCStr v)
{
    return ConvNum< bool >::FromStr(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to string value.
*/
template <> struct ConvTo< CStr >
{
    // --------------------------------------------------------------------------------------------
    template < typename T > static inline CStr From(T v)
    {
        return const_cast< CStr >(ConvNum< T >::ToStr(v));
    }
};

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to string value.
*/
template <> struct ConvTo< CCStr >
{
    // --------------------------------------------------------------------------------------------
    template < typename T > static inline CCStr From(T v)
    {
        return ConvNum< T >::ToStr(v);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Force a value to be within a certain range.
*/
template< typename T, typename U, typename V > inline T Clamp(T val, U min, V max)
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
template< typename T, typename U > inline T ClampMin(T val, U min)
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
template< typename T, typename U > inline T ClampMax(T val, U max)
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
template<> inline Float32 Clamp(Float32 val, Float32 min, Float32 max)
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
template<> inline Float64 Clamp(Float64 val, Float64 min, Float64 max)
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
typedef BitGuard< Uint8 >    BitGuardU8;
typedef BitGuard< Uint16 >   BitGuardU16;
typedef BitGuard< Uint32 >   BitGuardU32;

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
 * Retrieve the string representation of a certain type.
*/
CSStr SqTypeName(SQObjectType type);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the string representation of a certain type from a value on the stack.
*/
String SqTypeName(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Create a script string instance from a buffer.
*/
Object BufferToStrObj(const Buffer & b);

/* ------------------------------------------------------------------------------------------------
 * Create a script string instance from a portion of a buffer.
*/
Object BufferToStrObj(const Buffer & b, Uint32 size);

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as a native integer.
*/
SQInteger PopStackInteger(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as a native float.
*/
SQFloat PopStackFloat(HSQUIRRELVM vm, SQInteger idx);

} // Namespace:: SqMod
