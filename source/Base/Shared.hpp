#ifndef _BASE_SHARED_HPP_
#define _BASE_SHARED_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"

// ------------------------------------------------------------------------------------------------
#include <cmath>
#include <limits>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <type_traits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
#ifdef PI
    #undef PI
#endif

// ------------------------------------------------------------------------------------------------
const Float32 PI = 3.14159265359f;
const Float32 RECIPROCAL_PI = 1.0f/PI;
const Float32 HALF_PI = PI/2.0f;

// ------------------------------------------------------------------------------------------------
#ifdef PI64
    #undef PI64
#endif

// ------------------------------------------------------------------------------------------------
const Float64 PI64 = 3.1415926535897932384626433832795028841971693993751;
const Float64 RECIPROCAL_PI64 = 1.0/PI64;

// ------------------------------------------------------------------------------------------------
const Float32 DEGTORAD = PI / 180.0f;
const Float32 RADTODEG   = 180.0f / PI;
const Float64 DEGTORAD64 = PI64 / 180.0;
const Float64 RADTODEG64 = 180.0 / PI64;

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template< typename T > inline bool EpsEq(const T a, const T b)
{
    return abs(a - b) <= std::numeric_limits<T>::epsilon();
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
 * ...
*/
template< typename T > inline T Clamp(T val, T min, T max)
{
    return val < min ? min : (val > max ? max : val);
}

template<> inline Float32 Clamp(const Float32 val, const Float32 min, const Float32 max)
{
    return std::isless(val, min) ? min : (std::isgreater(val, max) ? max : val);
}

template<> inline Float64 Clamp(const Float64 val, const Float64 min, const Float64 max)
{
    return std::isless(val, min) ? min : (std::isgreater(val, max) ? max : val);
}

/* ------------------------------------------------------------------------------------------------
 * Simple functions to quickly forward logging messages without including the logging system
*/
void LogDbg(const char * fmt, ...);
void LogMsg(const char * fmt, ...);
void LogScs(const char * fmt, ...);
void LogInf(const char * fmt, ...);
void LogWrn(const char * fmt, ...);
void LogErr(const char * fmt, ...);
void LogFtl(const char * fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
const SQChar * ToStringF(const char * fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
void InitMTRG64();
void InitMTRG64();

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Int8 GetRandomInt8();
Int8 GetRandomInt8(Int8 min, Int8 max);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Uint8 GetRandomUint8();
Uint8 GetRandomUint8(Uint8 min, Uint8 max);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Int16 GetRandomInt16();
Int16 GetRandomInt16(Int16 min, Int16 max);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Uint16 GetRandomUint16();
Uint16 GetRandomUint16(Uint16 min, Uint16 max);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Int32 GetRandomInt32();
Int32 GetRandomInt32(Int32 min, Int32 max);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Uint32 GetRandomUint32();
Uint32 GetRandomUint32(Uint32 min, Uint32 max);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Int64 GetRandomInt64();
Int64 GetRandomInt64(Int64 min, Int64 max);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Uint64 GetRandomUint64();
Uint64 GetRandomUint64(Uint64 min, Uint64 max);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Float32 GetRandomFloat32();
Float32 GetRandomFloat32(Float32 min, Float32 max);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Float64 GetRandomFloat64();
Float64 GetRandomFloat64(Float64 min, Float64 max);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
String GetRandomString(String::size_type len);
String GetRandomString(String::size_type len, String::value_type min, String::value_type max);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
bool GetRandomBool();

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <typename T> struct RandomVal
{ /* ... */ };

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> struct RandomVal<Int8>
{
    static inline Int8 Get() { return GetRandomInt8(); }
    static inline Int8 Get(Int8 min, Int8 max) { return GetRandomInt8(min, max); }
};

template <> struct RandomVal<Uint8>
{
    static inline Uint8 Get() { return GetRandomUint8(); }
    static inline Uint8 Get(Uint8 min, Uint8 max) { return GetRandomUint8(min, max); }
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> struct RandomVal<Int16>
{
    static inline Int16 Get() { return GetRandomInt16(); }
    static inline Int16 Get(Int16 min, Int16 max) { return GetRandomInt16(min, max); }
};

template <> struct RandomVal<Uint16>
{
    static inline Uint16 Get() { return GetRandomUint16(); }
    static inline Uint16 Get(Uint16 min, Uint16 max) { return GetRandomUint16(min, max); }
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> struct RandomVal<Int32>
{
    static inline Int32 Get() { return GetRandomInt32(); }
    static inline Int32 Get(Int32 min, Int32 max) { return GetRandomInt32(min, max); }
};

template <> struct RandomVal<Uint32>
{
    static inline Uint32 Get() { return GetRandomUint32(); }
    static inline Uint32 Get(Uint32 min, Uint32 max) { return GetRandomUint32(min, max); }
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> struct RandomVal<Int64>
{
    static inline Int64 Get() { return GetRandomInt64(); }
    static inline Int64 Get(Int64 min, Int64 max) { return GetRandomInt64(min, max); }
};

template <> struct RandomVal<Uint64>
{
    static inline Uint64 Get() { return GetRandomUint64(); }
    static inline Uint64 Get(Uint64 min, Uint64 max) { return GetRandomUint64(min, max); }
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> struct RandomVal<Float32>
{
    static inline Float32 Get() { return GetRandomFloat32(); }
    static inline Float32 Get(Float32 min, Float32 max) { return GetRandomFloat32(min, max); }
};

template <> struct RandomVal<Float64>
{
    static inline Float64 Get() { return GetRandomFloat64(); }
    static inline Float64 Get(Float64 min, Float64 max) { return GetRandomFloat64(min, max); }
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> struct RandomVal<String>
{
    static inline String Get(String::size_type len) { return GetRandomString(len); }
    static inline String Get(String::size_type len, String::value_type min, String::value_type max)
    { return GetRandomString(len, min, max); }
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
template <> struct RandomVal<bool>
{
    static inline bool Get() { return GetRandomBool(); }
};

/* ------------------------------------------------------------------------------------------------
 * ...
*/
const Color3 & GetRandomColor();

/* ------------------------------------------------------------------------------------------------
 * Simple function to check whether the specified string can be considered as a boolean value
*/
bool SToB(const SQChar * str);

/* ------------------------------------------------------------------------------------------------
 * Utility used to unify the string converstion functions under one name.
*/
template < typename T > struct SToI { static constexpr auto Fn = static_cast< int(*)(const String &, std::size_t*, int) >(std::stoi); };
template < typename T > struct SToF { static constexpr auto Fn = static_cast< float(*)(const String &, std::size_t*) >(std::stof); };

// ------------------------------------------------------------------------------------------------
template <> struct SToI < char > { static constexpr auto Fn = static_cast< int(*)(const String &, std::size_t*, int) >(std::stoi); };
template <> struct SToI < signed char > { static constexpr auto Fn = static_cast< int(*)(const String &, std::size_t*, int) >(std::stoi); };
template <> struct SToI < unsigned char > { static constexpr auto Fn = static_cast< int(*)(const String &, std::size_t*, int) >(std::stoi); };
template <> struct SToI < short > { static constexpr auto Fn = static_cast< int(*)(const String &, std::size_t*, int) >(std::stoi); };
template <> struct SToI < unsigned short > { static constexpr auto Fn = static_cast< int(*)(const String &, std::size_t*, int) >(std::stoi); };
template <> struct SToI < int > { static constexpr auto Fn = static_cast< int(*)(const String &, std::size_t*, int) >(std::stoi); };
template <> struct SToI < unsigned int > { static constexpr auto Fn = static_cast< unsigned long(*)(const String &, std::size_t*, int) >(std::stoul); };
template <> struct SToI < long > { static constexpr auto Fn = static_cast< long(*)(const String &, std::size_t*, int) >(std::stol); };
template <> struct SToI < unsigned long > { static constexpr auto Fn = static_cast< unsigned long(*)(const String &, std::size_t*, int) >(std::stoul); };
template <> struct SToI < long long > { static constexpr auto Fn = static_cast< long long(*)(const String &, std::size_t*, int b) >(std::stoll); };
template <> struct SToI < unsigned long long > { static constexpr auto Fn = static_cast< unsigned long long(*)(const String &, std::size_t*, int) >(std::stoull); };
template <> struct SToF < float > { static constexpr auto Fn = static_cast< float(*)(const String &, std::size_t*) >(std::stof); };
template <> struct SToF < double > { static constexpr auto Fn = static_cast< double(*)(const String &, std::size_t*) >(std::stod); };
template <> struct SToF < long double > { static constexpr auto Fn = static_cast< long double(*)(const String &, std::size_t*) >(std::stold); };

// ------------------------------------------------------------------------------------------------
template < typename T > inline String NToS(T n) { return std::to_string(n); }
template < typename T > inline const SQChar * NToCS(T n) { return std::to_string(n).c_str(); }

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Color3 GetColor(const SQChar * name);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Circle GetCircle(const SQChar * str, SQChar delim);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
AABB GetAABB(const SQChar * str, SQChar delim);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Color3 GetColor3(const SQChar * str, SQChar delim);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Color4 GetColor4(const SQChar * str, SQChar delim);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Quaternion GetQuaternion(const SQChar * str, SQChar delim);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Sphere GetSphere(const SQChar * str, SQChar delim);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Vector2f GetVector2f(const SQChar * str, SQChar delim);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Vector2i GetVector2i(const SQChar * str, SQChar delim);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Vector2u GetVector2u(const SQChar * str, SQChar delim);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Vector3 GetVector3(const SQChar * str, SQChar delim);

/* ------------------------------------------------------------------------------------------------
 * ...
*/
Vector4 GetVector4(const SQChar * str, SQChar delim);

/* ------------------------------------------------------------------------------------------------
 * Fake deleter meant for classes that should not be deleted by smart pointers
*/
template <typename T> struct FakeDeleter
{
    void operator () (T * /* ptr */) const { /* Ignored... */ }
};

/* ------------------------------------------------------------------------------------------------
 * Utility used to generate a string with an arbitrary text surrounded by a specific character
*/
const SQChar * LeftStr(const SQChar * t, SQChar f, SQUint32 w = 72);
const SQChar * LeftStr(const SQChar * t, SQChar f, SQUint32 w = 72, SQUint32 o = 0);
const SQChar * RightStr(const SQChar * t, SQChar f, SQUint32 w = 72);
const SQChar * RightStr(const SQChar * t, SQChar f, SQUint32 w = 72, SQUint32 o = 0);
const SQChar * CenterStr(const SQChar * t, SQChar f, SQUint32 w = 72);

/* ------------------------------------------------------------------------------------------------
 * Function used insert arbitrary text at certain positions within a string
*/
const SQChar * InsertStr(const SQChar * f, const std::vector< const SQChar * > & a);

// Utility for the <InsertStr> function
const SQChar * InsStr(const SQChar * f);

template < typename... Args > const SQChar * InsStr(const SQChar * f, Args... args)
{
    return InsertStr(f, {{args...}});
}

} // Namespace:: SqMod

#endif // _BASE_SHARED_HPP_
