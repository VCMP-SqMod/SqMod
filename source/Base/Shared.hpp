#ifndef _BASE_SHARED_HPP_
#define _BASE_SHARED_HPP_

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
extern void LogDbg(CCStr fmt, ...);
extern void LogUsr(CCStr fmt, ...);
extern void LogScs(CCStr fmt, ...);
extern void LogInf(CCStr fmt, ...);
extern void LogWrn(CCStr fmt, ...);
extern void LogErr(CCStr fmt, ...);
extern void LogFtl(CCStr fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Secondary logging functions.
*/
extern void LogSDbg(CCStr fmt, ...);
extern void LogSUsr(CCStr fmt, ...);
extern void LogSScs(CCStr fmt, ...);
extern void LogSInf(CCStr fmt, ...);
extern void LogSWrn(CCStr fmt, ...);
extern void LogSErr(CCStr fmt, ...);
extern void LogSFtl(CCStr fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Primary conditional logging functions.
*/
extern bool cLogDbg(bool exp, CCStr fmt, ...);
extern bool cLogUsr(bool exp, CCStr fmt, ...);
extern bool cLogScs(bool exp, CCStr fmt, ...);
extern bool cLogInf(bool exp, CCStr fmt, ...);
extern bool cLogWrn(bool exp, CCStr fmt, ...);
extern bool cLogErr(bool exp, CCStr fmt, ...);
extern bool cLogFtl(bool exp, CCStr fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * Forward declarations of the logging functions to avoid including the logger everywhere.
 * Secondary conditional logging functions.
*/
extern bool cLogSDbg(bool exp, CCStr fmt, ...);
extern bool cLogSUsr(bool exp, CCStr fmt, ...);
extern bool cLogSScs(bool exp, CCStr fmt, ...);
extern bool cLogSInf(bool exp, CCStr fmt, ...);
extern bool cLogSWrn(bool exp, CCStr fmt, ...);
extern bool cLogSErr(bool exp, CCStr fmt, ...);
extern bool cLogSFtl(bool exp, CCStr fmt, ...);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent AABB instance with the given values.
*/
extern const AABB & GetAABB();
extern const AABB & GetAABB(Float32 mins, Float32 maxs);
extern const AABB & GetAABB(Float32 xv, Float32 yv, Float32 zv);
extern const AABB & GetAABB(Float32 xmin, Float32 ymin, Float32 zmin, Float32 xmax, Float32 ymax, Float32 zmax);
extern const AABB & GetAABB(const Vector3 & vmin, const Vector3 & vmax);
extern const AABB & GetAABB(const AABB & o);
extern const AABB & GetAABB(AABB && o);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent Circle instance with the given values.
*/
extern const Circle & GetCircle();
extern const Circle & GetCircle(Float32 rv);
extern const Circle & GetCircle(const Vector2 & pv, Float32 rv);
extern const Circle & GetCircle(Float32 xv, Float32 yv, Float32 rv);
extern const Circle & GetCircle(const Circle & o);
extern const Circle & GetCircle(Circle && o);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent Color3 instance with the given values.
*/
extern const Color3 & GetColor3();
extern const Color3 & GetColor3(Uint8 sv);
extern const Color3 & GetColor3(Uint8 rv, Uint8 gv, Uint8 bv);
extern const Color3 & GetColor3(const Color3 & o);
extern const Color3 & GetColor3(Color3 && o);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent Color4 instance with the given values.
*/
extern const Color4 & GetColor4();
extern const Color4 & GetColor4(Uint8 sv);
extern const Color4 & GetColor4(Uint8 rv, Uint8 gv, Uint8 bv);
extern const Color4 & GetColor4(Uint8 rv, Uint8 gv, Uint8 bv, Uint8 av);
extern const Color4 & GetColor4(const Color4 & o);
extern const Color4 & GetColor4(Color4 && o);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent Quaternion instance with the given values.
*/
extern const Quaternion & GetQuaternion();
extern const Quaternion & GetQuaternion(Float32 sv);
extern const Quaternion & GetQuaternion(Float32 xv, Float32 yv, Float32 zv);
extern const Quaternion & GetQuaternion(Float32 xv, Float32 yv, Float32 zv, Float32 wv);
extern const Quaternion & GetQuaternion(const Quaternion & o);
extern const Quaternion & GetQuaternion(Quaternion && o);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent Sphere instance with the given values.
*/
extern const Sphere & GetSphere();
extern const Sphere & GetSphere(Float32 rv);
extern const Sphere & GetSphere(const Vector3 & pv, Float32 rv);
extern const Sphere & GetSphere(Float32 xv, Float32 yv, Float32 zv, Float32 rv);
extern const Sphere & GetSphere(const Sphere & o);
extern const Sphere & GetSphere(Sphere && o);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent Vector2 instance with the given values.
*/
extern const Vector2 & GetVector2();
extern const Vector2 & GetVector2(Float32 sv);
extern const Vector2 & GetVector2(Float32 xv, Float32 yv);
extern const Vector2 & GetVector2(const Vector2 & o);
extern const Vector2 & GetVector2(Vector2 && o);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent Vector2i instance with the given values.
*/
extern const Vector2i & GetVector2i();
extern const Vector2i & GetVector2i(Int32 sv);
extern const Vector2i & GetVector2i(Int32 xv, Int32 yv);
extern const Vector2i & GetVector2i(const Vector2i & o);
extern const Vector2i & GetVector2i(Vector2i && o);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent Vector3 instance with the given values.
*/
extern const Vector3 & GetVector3();
extern const Vector3 & GetVector3(Float32 sv);
extern const Vector3 & GetVector3(Float32 xv, Float32 yv, Float32 zv);
extern const Vector3 & GetVector3(const Vector3 & o);
extern const Vector3 & GetVector3(Vector3 && o);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent Vector4 instance with the given values.
*/
extern const Vector4 & GetVector4();
extern const Vector4 & GetVector4(Float32 sv);
extern const Vector4 & GetVector4(Float32 xv, Float32 yv, Float32 zv);
extern const Vector4 & GetVector4(Float32 xv, Float32 yv, Float32 zv, Float32 wv);
extern const Vector4 & GetVector4(const Vector4 & o);
extern const Vector4 & GetVector4(Vector4 && o);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent LongInt instance with the given values.
*/
const SLongInt & GetSLongInt();
const SLongInt & GetSLongInt(Int64 n);
const SLongInt & GetSLongInt(CSStr s);
const ULongInt & GetULongInt();
const ULongInt & GetULongInt(Uint64 n);
const ULongInt & GetULongInt(CSStr s);

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
Color3 GetColor(const StackStrF & name);

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

} // Namespace:: SqMod

#endif // _BASE_SHARED_HPP_
