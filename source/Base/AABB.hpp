#ifndef _BASE_AABB_HPP_
#define _BASE_AABB_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"
#include "Base/Vector3.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct AABB
{
    // --------------------------------------------------------------------------------------------
    typedef SQFloat Value;
    // --------------------------------------------------------------------------------------------
    static const AABB NIL;
    static const AABB MIN;
    static const AABB MAX;
    // --------------------------------------------------------------------------------------------
    static SQChar Delim;
    // --------------------------------------------------------------------------------------------
    Vector3 min, max;
    // --------------------------------------------------------------------------------------------
    AABB() noexcept;
    AABB(Value s) noexcept;
    AABB(Value x, Value y, Value z) noexcept;
    AABB(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax) noexcept;
    // --------------------------------------------------------------------------------------------
    AABB(const Vector3 & b) noexcept;
    AABB(const Vector3 & vmin, const Vector3 & vmax) noexcept;
    // --------------------------------------------------------------------------------------------
    AABB(const Vector4 & b) noexcept;
    AABB(const Vector4 & vmin, const Vector4 & vmax) noexcept;
    // --------------------------------------------------------------------------------------------
    AABB(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    AABB(const AABB & b) noexcept;
    AABB(AABB && b) noexcept;
    // --------------------------------------------------------------------------------------------
    ~AABB();
    // --------------------------------------------------------------------------------------------
    AABB & operator = (const AABB & b) noexcept;
    AABB & operator = (AABB && b) noexcept;
    // --------------------------------------------------------------------------------------------
    AABB & operator = (Value s) noexcept;
    AABB & operator = (const Vector3 & v) noexcept;
    AABB & operator = (const Vector4 & v) noexcept;
    // --------------------------------------------------------------------------------------------
    AABB & operator += (const AABB & b) noexcept;
    AABB & operator -= (const AABB & b) noexcept;
    AABB & operator *= (const AABB & b) noexcept;
    AABB & operator /= (const AABB & b) noexcept;
    AABB & operator %= (const AABB & b) noexcept;
    // --------------------------------------------------------------------------------------------
    AABB & operator += (Value s) noexcept;
    AABB & operator -= (Value s) noexcept;
    AABB & operator *= (Value s) noexcept;
    AABB & operator /= (Value s) noexcept;
    AABB & operator %= (Value s) noexcept;
    // --------------------------------------------------------------------------------------------
    AABB & operator ++ () noexcept;
    AABB & operator -- () noexcept;
    // --------------------------------------------------------------------------------------------
    AABB operator ++ (int) noexcept;
    AABB operator -- (int) noexcept;
    // --------------------------------------------------------------------------------------------
    AABB operator + (const AABB & b) const noexcept;
    AABB operator - (const AABB & b) const noexcept;
    AABB operator * (const AABB & b) const noexcept;
    AABB operator / (const AABB & b) const noexcept;
    AABB operator % (const AABB & b) const noexcept;
    // --------------------------------------------------------------------------------------------
    AABB operator + (Value s) const noexcept;
    AABB operator - (Value s) const noexcept;
    AABB operator * (Value s) const noexcept;
    AABB operator / (Value s) const noexcept;
    AABB operator % (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    AABB operator + () const noexcept;
    AABB operator - () const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const AABB & b) const noexcept;
    bool operator != (const AABB & b) const noexcept;
    bool operator < (const AABB & b) const noexcept;
    bool operator > (const AABB & b) const noexcept;
    bool operator <= (const AABB & b) const noexcept;
    bool operator >= (const AABB & b) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const AABB & b) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns) noexcept;
    void Set(Value nx, Value ny, Value nz) noexcept;
    void Set(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const AABB & b) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const Vector3 & v) noexcept;
    void Set(const Vector3 & nmin, const Vector3 & nmax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const Vector4 & v) noexcept;
    void Set(const Vector4 & nmin, const Vector4 & nmax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    void Clear() noexcept { min.Clear(); max.Clear(); }
    // --------------------------------------------------------------------------------------------
    AABB Abs() const noexcept;
};

} // Namespace:: SqMod

#endif // _BASE_AABB_HPP_
