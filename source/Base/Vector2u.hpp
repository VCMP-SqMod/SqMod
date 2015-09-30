#ifndef _BASE_VECTOR2U_HPP_
#define _BASE_VECTOR2U_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Vector2u
{
    // --------------------------------------------------------------------------------------------
    typedef SQUnsignedInteger32 Value;
    // --------------------------------------------------------------------------------------------
    static const Vector2u NIL;
    static const Vector2u MIN;
    static const Vector2u MAX;
    // --------------------------------------------------------------------------------------------
    static SQChar Delim;
    // --------------------------------------------------------------------------------------------
    Value x, y;
    // --------------------------------------------------------------------------------------------
    Vector2u() noexcept;
    Vector2u(Value s) noexcept;
    Vector2u(Value xv, Value yv) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u(const Vector2i & v) noexcept;
    Vector2u(const Vector2f & v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u(const Vector2u & v) noexcept;
    Vector2u(Vector2u && v) noexcept;
    // --------------------------------------------------------------------------------------------
    ~Vector2u();
    // --------------------------------------------------------------------------------------------
    Vector2u & operator = (const Vector2u & v) noexcept;
    Vector2u & operator = (Vector2u && v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u & operator = (Value s) noexcept;
    Vector2u & operator = (const SQChar * values) noexcept;
    Vector2u & operator = (const Vector2i & v) noexcept;
    Vector2u & operator = (const Vector2f & v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u & operator += (const Vector2u & v) noexcept;
    Vector2u & operator -= (const Vector2u & v) noexcept;
    Vector2u & operator *= (const Vector2u & v) noexcept;
    Vector2u & operator /= (const Vector2u & v) noexcept;
    Vector2u & operator %= (const Vector2u & v) noexcept;
    Vector2u & operator &= (const Vector2u & v) noexcept;
    Vector2u & operator |= (const Vector2u & v) noexcept;
    Vector2u & operator ^= (const Vector2u & v) noexcept;
    Vector2u & operator <<= (const Vector2u & v) noexcept;
    Vector2u & operator >>= (const Vector2u & v) noexcept; 
    // --------------------------------------------------------------------------------------------
    Vector2u & operator += (Value s) noexcept;
    Vector2u & operator -= (Value s) noexcept;
    Vector2u & operator *= (Value s) noexcept;
    Vector2u & operator /= (Value s) noexcept;
    Vector2u & operator %= (Value s) noexcept;
    Vector2u & operator &= (Value s) noexcept;
    Vector2u & operator |= (Value s) noexcept;
    Vector2u & operator ^= (Value s) noexcept;
    Vector2u & operator <<= (Value s) noexcept;
    Vector2u & operator >>= (Value s) noexcept; 
    // --------------------------------------------------------------------------------------------
    Vector2u & operator ++ () noexcept;
    Vector2u & operator -- () noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator ++ (int) noexcept;
    Vector2u operator -- (int) noexcept; 
    // --------------------------------------------------------------------------------------------
    Vector2u operator + (const Vector2u & v) const noexcept;
    Vector2u operator + (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator - (const Vector2u & v) const noexcept;
    Vector2u operator - (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator * (const Vector2u & v) const noexcept;
    Vector2u operator * (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator / (const Vector2u & v) const noexcept;
    Vector2u operator / (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator % (const Vector2u & v) const noexcept;
    Vector2u operator % (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator & (const Vector2u & v) const noexcept;
    Vector2u operator & (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator | (const Vector2u & v) const noexcept;
    Vector2u operator | (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator ^ (const Vector2u & v) const noexcept;
    Vector2u operator ^ (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator << (const Vector2u & v) const noexcept;
    Vector2u operator << (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator >> (const Vector2u & v) const noexcept;
    Vector2u operator >> (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator + () const noexcept;
    Vector2u operator - () const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2u operator ~ () const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Vector2u & v) const noexcept;
    bool operator != (const Vector2u & v) const noexcept;
    bool operator < (const Vector2u & v) const noexcept;
    bool operator > (const Vector2u & v) const noexcept;
    bool operator <= (const Vector2u & v) const noexcept;
    bool operator >= (const Vector2u & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Vector2u & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns) noexcept;
    void Set(Value nx, Value ny) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const Vector2u & v) noexcept;
    void Set(const Vector2i & v) noexcept;
    void Set(const Vector2f & v) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    void Generate() noexcept;
    void Generate(Value min, Value max) noexcept;
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Clear() noexcept { x = 0, y = 0; }
    // --------------------------------------------------------------------------------------------
    Vector2u Abs() const noexcept;
};

} // Namespace:: SqMod

#endif // _BASE_VECTOR2U_HPP_
