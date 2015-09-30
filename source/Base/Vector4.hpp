#ifndef _BASE_VECTOR4_HPP_
#define _BASE_VECTOR4_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Vector4
{
    // --------------------------------------------------------------------------------------------
    typedef SQFloat Value;
    // --------------------------------------------------------------------------------------------
    static const Vector4 NIL;
    static const Vector4 MIN;
    static const Vector4 MAX;
    // --------------------------------------------------------------------------------------------
    static SQChar Delim;
    // --------------------------------------------------------------------------------------------
    Value x, y, z, w;
    // --------------------------------------------------------------------------------------------
    Vector4() noexcept;
    Vector4(Value s) noexcept;
    Vector4(Value xv, Value yv, Value zv) noexcept;
    Vector4(Value xv, Value yv, Value zv, Value wv) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector4(const Vector3 & v) noexcept;
    Vector4(const Quaternion & q) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector4(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector4(const Vector4 & v) noexcept;
    Vector4(Vector4 && v) noexcept;
    // --------------------------------------------------------------------------------------------
    ~Vector4();
    // --------------------------------------------------------------------------------------------
    Vector4 & operator = (const Vector4 & v) noexcept;
    Vector4 & operator = (Vector4 && v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector4 & operator = (Value s) noexcept;
    Vector4 & operator = (const Vector3 & v) noexcept;
    Vector4 & operator = (const Quaternion & q) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector4 & operator += (const Vector4 & v) noexcept;
    Vector4 & operator -= (const Vector4 & v) noexcept;
    Vector4 & operator *= (const Vector4 & v) noexcept;
    Vector4 & operator /= (const Vector4 & v) noexcept;
    Vector4 & operator %= (const Vector4 & v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector4 & operator += (Value s) noexcept;
    Vector4 & operator -= (Value s) noexcept;
    Vector4 & operator *= (Value s) noexcept;
    Vector4 & operator /= (Value s) noexcept;
    Vector4 & operator %= (Value s) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector4 & operator ++ () noexcept;
    Vector4 & operator -- () noexcept;
    // --------------------------------------------------------------------------------------------
    Vector4 operator ++ (int) noexcept;
    Vector4 operator -- (int) noexcept; 
    // --------------------------------------------------------------------------------------------
    Vector4 operator + (const Vector4 & v) const noexcept;
    Vector4 operator - (const Vector4 & v) const noexcept;
    Vector4 operator * (const Vector4 & v) const noexcept;
    Vector4 operator / (const Vector4 & v) const noexcept;
    Vector4 operator % (const Vector4 & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector4 operator + (Value s) const noexcept;
    Vector4 operator - (Value s) const noexcept;
    Vector4 operator * (Value s) const noexcept;
    Vector4 operator / (Value s) const noexcept;
    Vector4 operator % (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector4 operator + () const noexcept;
    Vector4 operator - () const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Vector4 & v) const noexcept;
    bool operator != (const Vector4 & v) const noexcept;
    bool operator < (const Vector4 & v) const noexcept;
    bool operator > (const Vector4 & v) const noexcept;
    bool operator <= (const Vector4 & v) const noexcept;
    bool operator >= (const Vector4 & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Vector4 & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns) noexcept;
    void Set(Value nx, Value ny, Value nz) noexcept;
    void Set(Value nx, Value ny, Value nz, Value nw) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const Vector4 & v) noexcept;
    void Set(const Vector3 & v) noexcept;
    void Set(const Quaternion & q) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    void Generate() noexcept;
    void Generate(Value min, Value max) noexcept;
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value wmin, Value wmax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Clear() noexcept { x = 0.0, y = 0.0, z = 0.0, w = 0.0; }
    // --------------------------------------------------------------------------------------------
    Vector4 Abs() const noexcept;
};

} // Namespace:: SqMod

#endif // _BASE_VECTOR4_HPP_
