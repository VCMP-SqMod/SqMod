#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Helper constants used by the bas types.
*/
constexpr Float32 SQMOD_PI = 3.14159265358979323846264338327950288f;
constexpr Float64 SQMOD_PI64 = 3.1415926535897932384626433832795028841971693993751d;

constexpr Float32 SQMOD_RECIPROCAL_PI = (1.0f / SQMOD_PI);
constexpr Float64 SQMOD_RECIPROCAL_PI64 = 1.0 / SQMOD_PI64;

constexpr Float32 SQMOD_HALF_PI = (SQMOD_PI * 0.5f);
constexpr Float32 SQMOD_HALF_PI64 = (SQMOD_PI64 * 0.5);

constexpr Float32 SQMOD_DEGTORAD = SQMOD_PI / 180.0f;
constexpr Float64 SQMOD_DEGTORAD64 = SQMOD_PI64 / 180.0;

constexpr Float32 SQMOD_DEGTORAD_2 = SQMOD_PI / 360.0f; // M_DEGTORAD / 2.f
constexpr Float64 SQMOD_DEGTORAD64_2 = SQMOD_PI64 / 360.0; // M_DEGTORAD / 2.f

constexpr Float32 SQMOD_RADTODEG   = 1.0f / SQMOD_DEGTORAD;
constexpr Float64 SQMOD_RADTODEG64 = 1.0 / SQMOD_DEGTORAD64;

/* ------------------------------------------------------------------------------------------------
 * Intersection test result.
*/
enum Intersection
{
    SQMODI_OUTSIDE = 0,
    SQMODI_INTERSECTS,
    SQMODI_INSIDE,
};

/* ------------------------------------------------------------------------------------------------
 * Helper used to reference and keep track of signal instances.
*/
typedef std::pair< Signal *, LightObj > SignalPair;

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Primary logging functions.
*/
extern void LogDbg(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogUsr(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogScs(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogInf(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogWrn(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogErr(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogFtl(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Secondary logging functions.
*/
extern void LogSDbg(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSUsr(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSScs(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSInf(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSWrn(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSErr(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);
extern void LogSFtl(CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 1, 2);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Primary logging functions.
*/
extern void LogDbgV(CCStr fmt, va_list vlist);
extern void LogUsrV(CCStr fmt, va_list vlist);
extern void LogScsV(CCStr fmt, va_list vlist);
extern void LogInfV(CCStr fmt, va_list vlist);
extern void LogWrnV(CCStr fmt, va_list vlist);
extern void LogErrV(CCStr fmt, va_list vlist);
extern void LogFtlV(CCStr fmt, va_list vlist);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Secondary logging functions.
*/
extern void LogSDbgV(CCStr fmt, va_list vlist);
extern void LogSUsrV(CCStr fmt, va_list vlist);
extern void LogSScsV(CCStr fmt, va_list vlist);
extern void LogSInfV(CCStr fmt, va_list vlist);
extern void LogSWrnV(CCStr fmt, va_list vlist);
extern void LogSErrV(CCStr fmt, va_list vlist);
extern void LogSFtlV(CCStr fmt, va_list vlist);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Primary conditional logging functions.
*/
extern bool cLogDbg(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogUsr(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogScs(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogInf(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogWrn(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogErr(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogFtl(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Secondary conditional logging functions.
*/
extern bool cLogSDbg(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSUsr(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSScs(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSInf(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSWrn(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSErr(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);
extern bool cLogSFtl(bool exp, CCStr fmt, ...) SQMOD_FORMAT_ATTR(printf, 2, 3);

/* ------------------------------------------------------------------------------------------------
 * Initialize a signal instance into the specified pair.
*/
extern void InitSignalPair(SignalPair & sp, LightObj & et, const char * name);

/* ------------------------------------------------------------------------------------------------
 * Reset/release the specified signal pair.
*/
extern void ResetSignalPair(SignalPair & sp, bool clear = true);

/* ------------------------------------------------------------------------------------------------
 * A simple implementation of name filtering.
*/
bool NameFilterCheck(CSStr filter, CSStr name);

/* ------------------------------------------------------------------------------------------------
 * A simple implementation of name filtering without case sensitivity.
*/
bool NameFilterCheckInsensitive(CSStr filter, CSStr name);

/* ------------------------------------------------------------------------------------------------
 * Obtain a randomly chosen color from a list of known colors.
*/
const Color3 & GetRandomColor();

/* ------------------------------------------------------------------------------------------------
 * Attempt to identify the color in the specified name and return it.
*/
Color3 GetColorStr(CSStr name);

/* ------------------------------------------------------------------------------------------------
 * Attempt to identify the color in the specified name and return it.
*/
Color3 GetColor(StackStrF & name);

/* ------------------------------------------------------------------------------------------------
 * Throw the last system error as an exception.
*/
void SqThrowLastF(CSStr msg, ...);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the string delimiter of a base type.
*/
template < typename T > inline SQInteger SqGetDelimiter()
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
 * Used internally to build format strings for math types.
*/
bool BuildFormatString(String & out, StackStrF & fmt, Uint32 arg, const String & spec);

} // Namespace:: SqMod
