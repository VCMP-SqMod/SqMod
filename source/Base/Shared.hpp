#ifndef _BASE_SHARED_HPP_
#define _BASE_SHARED_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <cmath>
#include <limits>

// ------------------------------------------------------------------------------------------------
#include <vcmp.h>
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern const SQChar * g_EmptyStr;

/* ------------------------------------------------------------------------------------------------
 * Proxies to communicate with the server.
*/
extern PluginFuncs*         _Func;
extern PluginCallbacks*     _Clbk;
extern PluginInfo*          _Info;

/* ------------------------------------------------------------------------------------------------
 * Perform an equality comparison between two values taking into account floating point issues.
*/
template< typename T > inline bool EpsEq(const T a, const T b)
{
    return abs(a - b) <= 0;
}

template <> inline bool EpsEq(const Float32 a, const Float32 b)
{
    return fabs(a - b) <= 0.000001f;
}

template <> inline bool EpsEq(const Float64 a, const Float64 b)
{
    return fabs(a - b) <= 0.000000001d;
}

/* ------------------------------------------------------------------------------------------------
 * Perform a less than comparison between two values taking into account floating point issues.
*/
template< typename T > inline bool EpsLt(const T a, const T b)
{
    return !EpsEq(a, b) && (a < b);
}

template <> inline bool EpsLt(const Float32 a, const Float32 b)
{
    return !EpsEq(a, b) && (a - b) < 0.000001f;
}

template <> inline bool EpsLt(const Float64 a, const Float64 b)
{
    return !EpsEq(a, b) && (a - b) < 0.000000001d;
}

/* ------------------------------------------------------------------------------------------------
 * Perform a greater than comparison between two values taking into account floating point issues.
*/
template< typename T > inline bool EpsGt(const T a, const T b)
{
    return !EpsEq(a, b) && (a > b);
}

template <> inline bool EpsGt(const Float32 a, const Float32 b)
{
    return !EpsEq(a, b) && (a - b) > 0.000001f;
}

template <> inline bool EpsGt(const Float64 a, const Float64 b)
{
    return !EpsEq(a, b) && (a - b) > 0.000000001d;
}

/* ------------------------------------------------------------------------------------------------
 * Perform a less than or equal comparison between two values taking into account
 * floating point issues.
*/
template< typename T > inline bool EpsLtEq(const T a, const T b)
{
    return !EpsEq(a, b) || (a < b);
}

template <> inline bool EpsLtEq(const Float32 a, const Float32 b)
{
    return !EpsEq(a, b) || (a - b) < 0.000001f;
}

template <> inline bool EpsLtEq(const Float64 a, const Float64 b)
{
    return !EpsEq(a, b) || (a - b) < 0.000000001d;
}

/* ------------------------------------------------------------------------------------------------
 * Perform a greater than or equal comparison between two values taking into account
 * floating point issues.
*/
template< typename T > inline bool EpsGtEq(const T a, const T b)
{
    return !EpsEq(a, b) || (a > b);
}

template <> inline bool EpsGtEq(const Float32 a, const Float32 b)
{
    return !EpsEq(a, b) || (a - b) > 0.000001f;
}

template <> inline bool EpsGtEq(const Float64 a, const Float64 b)
{
    return !EpsEq(a, b) || (a - b) > 0.000000001d;
}

/* ------------------------------------------------------------------------------------------------
 *
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

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int8 ConvTo< Int8 >::From< Uint8 >(Uint8 v)
{
    return (v >= static_cast< Uint8 >(Max)) ? Max : static_cast< Int8 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int8 ConvTo< Int8 >::From< Uint16 >(Uint16 v)
{
    return (v >= static_cast< Uint16 >(Max)) ? Max : static_cast< Int8 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int8 ConvTo< Int8 >::From< Uint32 >(Uint32 v)
{
    return (v >= static_cast< Uint32 >(Max)) ? Max : static_cast< Int8 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int8 ConvTo< Int8 >::From< Uint64 >(Uint64 v)
{
    return (v >= static_cast< Uint64 >(Max)) ? Max : static_cast< Int8 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int16 ConvTo< Int16 >::From< Uint8 >(Uint8 v)
{
    return static_cast< Int16 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int16 ConvTo< Int16 >::From< Uint16 >(Uint16 v)
{
    return (v >= static_cast< Uint16 >(Max)) ? Max : static_cast< Int16 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int16 ConvTo< Int16 >::From< Uint32 >(Uint32 v)
{
    return (v >= static_cast< Uint32 >(Max)) ? Max : static_cast< Int16 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int16 ConvTo< Int16 >::From< Uint64 >(Uint64 v)
{
    return (v >= static_cast< Uint64 >(Max)) ? Max : static_cast< Int16 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int32 ConvTo< Int32 >::From< Uint8 >(Uint8 v)
{
    return static_cast< Int32 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int32 ConvTo< Int32 >::From< Uint16 >(Uint16 v)
{
    return static_cast< Int32 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int32 ConvTo< Int32 >::From< Uint32 >(Uint32 v)
{
    return (v >= static_cast< Uint32 >(Max)) ? Max : static_cast< Int32 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int32 ConvTo< Int32 >::From< Uint64 >(Uint64 v)
{
    return (v >= static_cast< Uint64 >(Max)) ? Max : static_cast< Int32 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint8 ConvTo< Uint8 >::From< Int8 >(Int8 v)
{
    return (v <= 0) ? 0 : static_cast< Uint8 >(v);
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint8 ConvTo< Uint8 >::From< Int32 >(Int32 v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int32 >(Max))
    {
        return Max;
    }
    return static_cast< Uint8 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint8 ConvTo< Uint8 >::From< Int64 >(Int64 v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int64 >(Max))
    {
        return Max;
    }
    return static_cast< Uint8 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint16 ConvTo< Uint16 >::From< Int8 >(Int8 v)
{
    return (v <= 0) ? 0 : static_cast< Uint16 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint16 ConvTo< Uint16 >::From< Int16 >(Int16 v)
{
    return (v <= 0) ? 0 : static_cast< Uint16 >(v);
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint16 ConvTo< Uint16 >::From< Int64 >(Int64 v)
{
    if (v <= 0)
    {
        return 0;
    }
    else if (v >= static_cast< Int64 >(Max))
    {
        return Max;
    }
    return static_cast< Uint16 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint32 ConvTo< Uint32 >::From< Int8 >(Int8 v)
{
    return (v <= 0) ? 0 : static_cast< Uint32 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint32 ConvTo< Uint32 >::From< Int16 >(Int16 v)
{
    return (v <= 0) ? 0 : static_cast< Uint32 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint32 ConvTo< Uint32 >::From< Int32 >(Int32 v)
{
    return (v <= 0) ? 0 : static_cast< Uint32 >(v);
}

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int8 ConvTo< Int8 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, static_cast< Float32 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float32 >(Max)))
    {
        return Max;
    }
    return static_cast< Int8 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int16 ConvTo< Int16 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, static_cast< Float32 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float32 >(Max)))
    {
        return Max;
    }
    return static_cast< Int16 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int32 ConvTo< Int32 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, static_cast< Float32 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float32 >(Max)))
    {
        return Max;
    }
    return static_cast< Int32 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int8 ConvTo< Int8 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, static_cast< Float64 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float64 >(Max)))
    {
        return Max;
    }
    return static_cast< Int8 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int16 ConvTo< Int16 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, static_cast< Float64 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float64 >(Max)))
    {
        return Max;
    }
    return static_cast< Int16 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Int32 ConvTo< Int32 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, static_cast< Float64 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float64 >(Max)))
    {
        return Max;
    }
    return static_cast< Int32 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint8 ConvTo< Uint8 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, static_cast< Float32 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float32 >(Max)))
    {
        return Max;
    }
    return static_cast< Uint8 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint16 ConvTo< Uint16 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, static_cast< Float32 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float32 >(Max)))
    {
        return Max;
    }
    return static_cast< Uint16 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint32 ConvTo< Uint32 >::From< Float32 >(Float32 v)
{
    if (EpsLt(v, static_cast< Float32 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float32 >(Max)))
    {
        return Max;
    }
    return static_cast< Uint32 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint8 ConvTo< Uint8 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, static_cast< Float64 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float64 >(Max)))
    {
        return Max;
    }
    return static_cast< Uint8 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint16 ConvTo< Uint16 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, static_cast< Float64 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float64 >(Max)))
    {
        return Max;
    }
    return static_cast< Uint16 >(v);
}

// ------------------------------------------------------------------------------------------------
template <> template <> inline Uint32 ConvTo< Uint32 >::From< Float64 >(Float64 v)
{
    if (EpsLt(v, static_cast< Float64 >(Min)))
    {
        return Min;
    }
    else if (EpsGt(v, static_cast< Float64 >(Max)))
    {
        return Max;
    }
    return static_cast< Uint32 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to signed long long integer.
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
template <> inline Int64 ConvTo< Int64 >::From< Uint64 >(Uint64 v)
{
    return (v >= static_cast< Uint64 >(Max)) ? Max : static_cast< Int64 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to unsigned long long integer.
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
 * Convert other numeric values to a floating point value.
*/
template <> struct ConvTo< Float32 >
{
    // --------------------------------------------------------------------------------------------
    static constexpr Float32 Min = std::numeric_limits< Float32 >::min();
    static constexpr Float32 Max = std::numeric_limits< Float32 >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline Float32 From(T v)
    {
        return static_cast< Float32 >(v);
    }
};

// ------------------------------------------------------------------------------------------------
template <> inline Float32 ConvTo< Float32 >::From< Float64 >(Float64 v)
{
    if (EpsGt(v, static_cast< Float64 >(Max)))
    {
        return Max;
    }
    else if (EpsLt(v, static_cast< Float64 >(Min)))
    {
        return Min;
    }
    return static_cast< Float32 >(v);
}

/* ------------------------------------------------------------------------------------------------
 * Convert other numeric values to a double floating point value.
*/
template <> struct ConvTo< Float64 >
{
    // --------------------------------------------------------------------------------------------
    static constexpr Float64 Min = std::numeric_limits< Float64 >::min();
    static constexpr Float64 Max = std::numeric_limits< Float64 >::max();

    // --------------------------------------------------------------------------------------------
    template < typename T > static inline Float64 From(T v)
    {
        return static_cast< Float64 >(v);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Force a value to be within a certain range.
*/
template< typename T > inline T Clamp(T val, T min, T max)
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
 * Compute the next power of two for the specified number.
*/
inline Uint32 NextPow2(Uint32 num)
{
    --num;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    return ++num;
}

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
 * Retrieve a reference to a null script object.
*/
Object & NullObject();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null/empty script array.
*/
Array & NullArray();

/* ------------------------------------------------------------------------------------------------
 * Retrieve a reference to a null script function.
*/
Function & NullFunction();

/* ------------------------------------------------------------------------------------------------
 * Simple function to check whether the specified string can be considered as a boolean value
*/
bool SToB(CSStr str);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string and throw it as a sqrat exception.
*/
void SqThrowF(CCStr fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * Quickly generate a formatted string on a small static buffer without any memory allocations.
*/
CSStr ToStrF(CCStr fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string on a temporary buffer and return the string but not the buffer.
*/
CSStr ToStringF(CCStr fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * Obtain a randomly chosen color from a list of known colors.
*/
const Color3 & GetRandomColor();

/* ------------------------------------------------------------------------------------------------
 * Attempt to identify the color in the specified name and return it.
*/
Color3 GetColor(CSStr name);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Primary logging functions.
*/
void LogDbg(CCStr fmt, ...);
void LogUsr(CCStr fmt, ...);
void LogScs(CCStr fmt, ...);
void LogInf(CCStr fmt, ...);
void LogWrn(CCStr fmt, ...);
void LogErr(CCStr fmt, ...);
void LogFtl(CCStr fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Secondary logging functions.
*/
void LogSDbg(CCStr fmt, ...);
void LogSUsr(CCStr fmt, ...);
void LogSScs(CCStr fmt, ...);
void LogSInf(CCStr fmt, ...);
void LogSWrn(CCStr fmt, ...);
void LogSErr(CCStr fmt, ...);
void LogSFtl(CCStr fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Primary conditional logging functions.
*/
bool cLogDbg(bool cond, CCStr fmt, ...);
bool cLogUsr(bool cond, CCStr fmt, ...);
bool cLogScs(bool cond, CCStr fmt, ...);
bool cLogInf(bool cond, CCStr fmt, ...);
bool cLogWrn(bool cond, CCStr fmt, ...);
bool cLogErr(bool cond, CCStr fmt, ...);
bool cLogFtl(bool cond, CCStr fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Secondary conditional logging functions.
*/
bool cLogSDbg(bool cond, CCStr fmt, ...);
bool cLogSUsr(bool cond, CCStr fmt, ...);
bool cLogSScs(bool cond, CCStr fmt, ...);
bool cLogSInf(bool cond, CCStr fmt, ...);
bool cLogSWrn(bool cond, CCStr fmt, ...);
bool cLogSErr(bool cond, CCStr fmt, ...);
bool cLogSFtl(bool cond, CCStr fmt, ...);

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
    IsCType(CTypeFn fn)
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
    IsNotCType(CTypeFn fn)
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

} // Namespace:: SqMod

#endif // _BASE_SHARED_HPP_
