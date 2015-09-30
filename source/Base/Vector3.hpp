#ifndef _BASE_VECTOR3_HPP_
#define _BASE_VECTOR3_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Vector3
{
    // --------------------------------------------------------------------------------------------
    typedef SQFloat Value;
    // --------------------------------------------------------------------------------------------
    static const Vector3 NIL;
    static const Vector3 MIN;
    static const Vector3 MAX;
    // --------------------------------------------------------------------------------------------
    static SQChar Delim;
    // --------------------------------------------------------------------------------------------
    Value x, y, z;
    // --------------------------------------------------------------------------------------------
    Vector3() noexcept;
    Vector3(Value s) noexcept;
    Vector3(Value xv, Value yv, Value zv) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector3(const Vector4 & v) noexcept;
    Vector3(const Quaternion & q) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector3(const SQChar * values, char delim) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector3(const Vector3 & v) noexcept;
    Vector3(Vector3 && v) noexcept;
    // --------------------------------------------------------------------------------------------
    ~Vector3();
    // --------------------------------------------------------------------------------------------
    Vector3 & operator = (const Vector3 & v) noexcept;
    Vector3 & operator = (Vector3 && v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector3 & operator = (Value s) noexcept;
    Vector3 & operator = (const Vector4 & v) noexcept;
    Vector3 & operator = (const Quaternion & q) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector3 & operator += (const Vector3 & v) noexcept;
    Vector3 & operator -= (const Vector3 & v) noexcept;
    Vector3 & operator *= (const Vector3 & v) noexcept;
    Vector3 & operator /= (const Vector3 & v) noexcept;
    Vector3 & operator %= (const Vector3 & v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector3 & operator += (Value s) noexcept;
    Vector3 & operator -= (Value s) noexcept;
    Vector3 & operator *= (Value s) noexcept;
    Vector3 & operator /= (Value s) noexcept;
    Vector3 & operator %= (Value s) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector3 & operator ++ () noexcept;
    Vector3 & operator -- () noexcept;
    // --------------------------------------------------------------------------------------------
    Vector3 operator ++ (int) noexcept;
    Vector3 operator -- (int) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector3 operator + (const Vector3 & v) const noexcept;
    Vector3 operator - (const Vector3 & v) const noexcept;
    Vector3 operator * (const Vector3 & v) const noexcept;
    Vector3 operator / (const Vector3 & v) const noexcept;
    Vector3 operator % (const Vector3 & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector3 operator + (Value s) const noexcept;
    Vector3 operator - (Value s) const noexcept;
    Vector3 operator * (Value s) const noexcept;
    Vector3 operator / (Value s) const noexcept;
    Vector3 operator % (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector3 operator + () const noexcept;
    Vector3 operator - () const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Vector3 & v) const noexcept;
    bool operator != (const Vector3 & v) const noexcept;
    bool operator < (const Vector3 & v) const noexcept;
    bool operator > (const Vector3 & v) const noexcept;
    bool operator <= (const Vector3 & v) const noexcept;
    bool operator >= (const Vector3 & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Vector3 & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns) noexcept;
    void Set(Value nx, Value ny, Value nz) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const Vector3 & v) noexcept;
    void Set(const Vector4 & v) noexcept;
    void Set(const Quaternion & q) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, char delim) noexcept;
    // --------------------------------------------------------------------------------------------
    void Generate() noexcept;
    void Generate(Value min, Value max) noexcept;
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Clear() noexcept { x = 0.0, y = 0.0, z = 0.0; }
    // --------------------------------------------------------------------------------------------
    Vector3 Abs() const noexcept;
};

} // Namespace:: SqMod

#endif // _BASE_VECTOR3_HPP_
