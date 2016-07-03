#ifndef _BASE_SHARED_HPP_
#define _BASE_SHARED_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
extern const AABB & GetAABB(Float32 sv);
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
 * Obtain a randomly chosen color from a list of known colors.
*/
const Color3 & GetRandomColor();

/* ------------------------------------------------------------------------------------------------
 * Attempt to identify the color in the specified name and return it.
*/
Color3 GetColor(CSStr name);

/* ------------------------------------------------------------------------------------------------
 * Throw the last system error as an exception.
*/
void SqThrowLastF(CSStr msg, ...);

/* ------------------------------------------------------------------------------------------------
 * RAII approach to delete an instance of a class if not released.
*/
template < typename T > class AutoDelete
{
private:

    // --------------------------------------------------------------------------------------------
    T * m_Inst; // The managed instance.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    AutoDelete(T * inst)
        : m_Inst(inst)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    AutoDelete(const AutoDelete & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    AutoDelete(AutoDelete && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~AutoDelete()
    {
        if (m_Inst)
        {
            delete m_Inst;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    AutoDelete & operator = (const AutoDelete & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    AutoDelete & operator = (AutoDelete && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Implicit cast to the managed instance.
    */
    operator T * ()
    {
        return m_Inst;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit cast to the managed instance.
    */
    operator  const T * () const
    {
        return m_Inst;
    }

    /* --------------------------------------------------------------------------------------------
     * Released the managed instance.
    */
    void Release()
    {
        m_Inst = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Released the managed instance.
    */
    T * Grab()
    {
        T * ptr = m_Inst;
        m_Inst = nullptr;
        return ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the managed instance.
    */
    T * Get()
    {
        return m_Inst;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the managed instance.
    */
    const T * Get() const
    {
        return m_Inst;
    }
};

} // Namespace:: SqMod

#endif // _BASE_SHARED_HPP_
