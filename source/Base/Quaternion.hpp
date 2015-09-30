#ifndef _BASE_QUATERNION_HPP_
#define _BASE_QUATERNION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Quaternion
{
    // --------------------------------------------------------------------------------------------
    typedef SQFloat Value;
    // --------------------------------------------------------------------------------------------
    static const Quaternion NIL;
    static const Quaternion MIN;
    static const Quaternion MAX;
    // --------------------------------------------------------------------------------------------
    static SQChar Delim;
    // --------------------------------------------------------------------------------------------
    Value x, y, z, w;
    // --------------------------------------------------------------------------------------------
    Quaternion() noexcept;
    Quaternion(Value s) noexcept;
    Quaternion(Value xv, Value yv, Value zv) noexcept;
    Quaternion(Value xv, Value yv, Value zv, Value wv) noexcept;
    // --------------------------------------------------------------------------------------------
    Quaternion(const Vector3 & v) noexcept;
    Quaternion(const Vector4 & v) noexcept;
    // --------------------------------------------------------------------------------------------
    Quaternion(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    Quaternion(const Quaternion & q) noexcept;
    Quaternion(Quaternion && q) noexcept;
    // --------------------------------------------------------------------------------------------
    ~Quaternion();
    // --------------------------------------------------------------------------------------------
    Quaternion & operator = (const Quaternion & q) noexcept;
    Quaternion & operator = (Quaternion && q) noexcept;
    // --------------------------------------------------------------------------------------------
    Quaternion & operator = (Value s) noexcept;
    Quaternion & operator = (const Vector3 & q) noexcept;
    Quaternion & operator = (const Vector4 & q) noexcept;
    // --------------------------------------------------------------------------------------------
    Quaternion & operator += (const Quaternion & q) noexcept;
    Quaternion & operator -= (const Quaternion & q) noexcept;
    Quaternion & operator *= (const Quaternion & q) noexcept;
    Quaternion & operator /= (const Quaternion & q) noexcept;
    Quaternion & operator %= (const Quaternion & q) noexcept;
    // --------------------------------------------------------------------------------------------
    Quaternion & operator += (Value s) noexcept;
    Quaternion & operator -= (Value s) noexcept;
    Quaternion & operator *= (Value s) noexcept;
    Quaternion & operator /= (Value s) noexcept;
    Quaternion & operator %= (Value s) noexcept;
    // --------------------------------------------------------------------------------------------
    Quaternion & operator ++ () noexcept;
    Quaternion & operator -- () noexcept;
    // --------------------------------------------------------------------------------------------
    Quaternion operator ++ (int) noexcept;
    Quaternion operator -- (int) noexcept;
    // --------------------------------------------------------------------------------------------
    Quaternion operator + (const Quaternion & q) const noexcept;
    Quaternion operator - (const Quaternion & q) const noexcept;
    Quaternion operator * (const Quaternion & q) const noexcept;
    Quaternion operator / (const Quaternion & q) const noexcept;
    Quaternion operator % (const Quaternion & q) const noexcept;
    // --------------------------------------------------------------------------------------------
    Quaternion operator + (Value s) const noexcept;
    Quaternion operator - (Value s) const noexcept;
    Quaternion operator * (Value s) const noexcept;
    Quaternion operator / (Value s) const noexcept;
    Quaternion operator % (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Quaternion operator + () const noexcept;
    Quaternion operator - () const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Quaternion & q) const noexcept;
    bool operator != (const Quaternion & q) const noexcept;
    bool operator < (const Quaternion & q) const noexcept;
    bool operator > (const Quaternion & q) const noexcept;
    bool operator <= (const Quaternion & q) const noexcept;
    bool operator >= (const Quaternion & q) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Quaternion & q) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns) noexcept;
    void Set(Value nx, Value ny, Value nz) noexcept;
    void Set(Value nx, Value ny, Value nz, Value nw) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const Quaternion & q) noexcept;
    void Set(const Vector3 & v) noexcept;
    void Set(const Vector4 & v) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    void Generate() noexcept;
    void Generate(Value min, Value max) noexcept;
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value wmin, Value wmax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Clear() noexcept { x = 0.0, y = 0.0, z = 0.0, w = 0.0; }
    // --------------------------------------------------------------------------------------------
    Quaternion Abs() const noexcept;
};

} // Namespace:: SqMod

#endif // _BASE_QUATERNION_HPP_
