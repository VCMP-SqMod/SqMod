#ifndef _BASE_SHARED_HPP_
#define _BASE_SHARED_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <math.h>
#include <assert.h>

// ------------------------------------------------------------------------------------------------
#include <vcmp.h>
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern const SQChar * g_EmptyStr;

/* ------------------------------------------------------------------------------------------------
 * Proxies to comunicate with the server.
*/
extern PluginFuncs*         _Func;
extern PluginCallbacks*     _Clbk;
extern PluginInfo*          _Info;

// ------------------------------------------------------------------------------------------------
template < typename T > struct NumLimit;

// ------------------------------------------------------------------------------------------------
template <> struct NumLimit< char > { static const char Min, Max; };
template <> struct NumLimit< signed char > { static const signed char Min, Max; };
template <> struct NumLimit< unsigned char > { static const unsigned char Min, Max; };
template <> struct NumLimit< signed short > { static const signed short Min, Max; };
template <> struct NumLimit< unsigned short > { static const unsigned short Min, Max; };
template <> struct NumLimit< signed int > { static const signed int Min, Max; };
template <> struct NumLimit< unsigned int > { static const unsigned int Min, Max; };
template <> struct NumLimit< signed long > { static const signed long Min, Max; };
template <> struct NumLimit< unsigned long > { static const unsigned long Min, Max; };
template <> struct NumLimit< signed long long > { static const signed long long Min, Max; };
template <> struct NumLimit< unsigned long long > { static const unsigned long long Min, Max; };
template <> struct NumLimit< float > { static const float Min, Max; };
template <> struct NumLimit< double > { static const double Min, Max; };
template <> struct NumLimit< long double > { static const long double Min, Max; };

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
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

// ------------------------------------------------------------------------------------------------
template< typename T > inline T Clamp(T val, T min, T max)
{
    return val < min ? min : (val > max ? max : val);
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

// ------------------------------------------------------------------------------------------------
void LogDbg(CCStr fmt, ...);
void LogUsr(CCStr fmt, ...);
void LogScs(CCStr fmt, ...);
void LogInf(CCStr fmt, ...);
void LogWrn(CCStr fmt, ...);
void LogErr(CCStr fmt, ...);
void LogFtl(CCStr fmt, ...);

// ------------------------------------------------------------------------------------------------
void LogSDbg(CCStr fmt, ...);
void LogSUsr(CCStr fmt, ...);
void LogSScs(CCStr fmt, ...);
void LogSInf(CCStr fmt, ...);
void LogSWrn(CCStr fmt, ...);
void LogSErr(CCStr fmt, ...);
void LogSFtl(CCStr fmt, ...);

// ------------------------------------------------------------------------------------------------
bool cLogDbg(bool cond, CCStr fmt, ...);
bool cLogUsr(bool cond, CCStr fmt, ...);
bool cLogScs(bool cond, CCStr fmt, ...);
bool cLogInf(bool cond, CCStr fmt, ...);
bool cLogWrn(bool cond, CCStr fmt, ...);
bool cLogErr(bool cond, CCStr fmt, ...);
bool cLogFtl(bool cond, CCStr fmt, ...);

// ------------------------------------------------------------------------------------------------
bool cLogSDbg(bool cond, CCStr fmt, ...);
bool cLogSUsr(bool cond, CCStr fmt, ...);
bool cLogSScs(bool cond, CCStr fmt, ...);
bool cLogSInf(bool cond, CCStr fmt, ...);
bool cLogSWrn(bool cond, CCStr fmt, ...);
bool cLogSErr(bool cond, CCStr fmt, ...);
bool cLogSFtl(bool cond, CCStr fmt, ...);

// ------------------------------------------------------------------------------------------------
void SqThrow(CCStr fmt, ...);

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

// ------------------------------------------------------------------------------------------------
Object & NullObject();

// ------------------------------------------------------------------------------------------------
Array & NullArray();

// ------------------------------------------------------------------------------------------------
Function & NullFunction();

// ------------------------------------------------------------------------------------------------
template < typename T > Object MakeObject(const T & v)
{
    PushVar< T >(DefaultVM::Get(), v);
    Var< Object > var(DefaultVM::Get(), -1);
    sq_pop(DefaultVM::Get(), 1);
    return var.value;
}

// ------------------------------------------------------------------------------------------------
template < typename T > Object MakeObject(HSQUIRRELVM vm, const T & v)
{
    PushVar< T >(vm, v);
    Var< Object > var(vm, -1);
    sq_pop(vm, 1);
    return var.value;
}

/* ------------------------------------------------------------------------------------------------
 * Simple function to check whether the specified string can be considered as a boolean value
*/
bool SToB(CSStr str);

/* ------------------------------------------------------------------------------------------------
 *
*/
CSStr ToStrF(CCStr fmt, ...);

/* ------------------------------------------------------------------------------------------------
 *
*/
CSStr ToStringF(CCStr fmt, ...);

// ------------------------------------------------------------------------------------------------
const Color3 & GetRandomColor();

/* ------------------------------------------------------------------------------------------------
 * Value extractors.
*/
Color3 GetColor(CSStr name);
AABB GetAABB(CSStr str, SQChar delim);
Circle GetCircle(CSStr str, SQChar delim);
Color3 GetColor3(CSStr str, SQChar delim);
Color4 GetColor4(CSStr str, SQChar delim);
Quaternion GetQuaternion(CSStr str, SQChar delim);
Sphere GetSphere(CSStr str, SQChar delim);
Vector2 GetVector2(CSStr str, SQChar delim);
Vector2i GetVector2i(CSStr str, SQChar delim);
Vector3 GetVector3(CSStr str, SQChar delim);
Vector4 GetVector4(CSStr str, SQChar delim);

} // Namespace:: SqMod

#endif // _BASE_SHARED_HPP_
