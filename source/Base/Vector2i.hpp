#ifndef _BASE_VECTOR2I_HPP_
#define _BASE_VECTOR2I_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Vector2i
{
    // --------------------------------------------------------------------------------------------
    typedef SQInt32 Value;
    // --------------------------------------------------------------------------------------------
    static const Vector2i NIL;
    static const Vector2i MIN;
    static const Vector2i MAX;
    // --------------------------------------------------------------------------------------------
    static SQChar Delim;
    // --------------------------------------------------------------------------------------------
    Value x, y;
    // --------------------------------------------------------------------------------------------
    Vector2i() noexcept;
    Vector2i(Value s) noexcept;
    Vector2i(Value xv, Value yv) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2i(const Vector2u & v) noexcept;
    Vector2i(const Vector2f & v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2i(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2i(const Vector2i & v) noexcept;
    Vector2i(Vector2i && v) noexcept;
    // --------------------------------------------------------------------------------------------
    ~Vector2i();
    // --------------------------------------------------------------------------------------------
    Vector2i & operator = (const Vector2i & v) noexcept;
    Vector2i & operator = (Vector2i && v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2i & operator = (Value s) noexcept;
    Vector2i & operator = (const SQChar * values) noexcept;
    Vector2i & operator = (const Vector2u & v) noexcept;
    Vector2i & operator = (const Vector2f & v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2i & operator += (const Vector2i & v) noexcept;
    Vector2i & operator -= (const Vector2i & v) noexcept;
    Vector2i & operator *= (const Vector2i & v) noexcept;
    Vector2i & operator /= (const Vector2i & v) noexcept;
    Vector2i & operator %= (const Vector2i & v) noexcept;
    Vector2i & operator &= (const Vector2i & v) noexcept;
    Vector2i & operator |= (const Vector2i & v) noexcept;
    Vector2i & operator ^= (const Vector2i & v) noexcept;
    Vector2i & operator <<= (const Vector2i & v) noexcept;
    Vector2i & operator >>= (const Vector2i & v) noexcept; 
    // --------------------------------------------------------------------------------------------
    Vector2i & operator += (Value s) noexcept;
    Vector2i & operator -= (Value s) noexcept;
    Vector2i & operator *= (Value s) noexcept;
    Vector2i & operator /= (Value s) noexcept;
    Vector2i & operator %= (Value s) noexcept;
    Vector2i & operator &= (Value s) noexcept;
    Vector2i & operator |= (Value s) noexcept;
    Vector2i & operator ^= (Value s) noexcept;
    Vector2i & operator <<= (Value s) noexcept;
    Vector2i & operator >>= (Value s) noexcept; 
    // --------------------------------------------------------------------------------------------
    Vector2i & operator ++ () noexcept;
    Vector2i & operator -- () noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2i operator ++ (int) noexcept;
    Vector2i operator -- (int) noexcept; 
    // --------------------------------------------------------------------------------------------
    Vector2i operator + (const Vector2i & v) const noexcept;
    Vector2i operator - (const Vector2i & v) const noexcept;
    Vector2i operator * (const Vector2i & v) const noexcept;
    Vector2i operator / (const Vector2i & v) const noexcept;
    Vector2i operator % (const Vector2i & v) const noexcept;
    Vector2i operator & (const Vector2i & v) const noexcept;
    Vector2i operator | (const Vector2i & v) const noexcept;
    Vector2i operator ^ (const Vector2i & v) const noexcept;
    Vector2i operator << (const Vector2i & v) const noexcept;
    Vector2i operator >> (const Vector2i & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2i operator + (Value s) const noexcept;
    Vector2i operator - (Value s) const noexcept;
    Vector2i operator * (Value s) const noexcept;
    Vector2i operator / (Value s) const noexcept;
    Vector2i operator % (Value s) const noexcept;
    Vector2i operator & (Value s) const noexcept;
    Vector2i operator | (Value s) const noexcept;
    Vector2i operator ^ (Value s) const noexcept;
    Vector2i operator << (Value s) const noexcept;
    Vector2i operator >> (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2i operator + () const noexcept;
    Vector2i operator - () const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2i operator ~ () const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Vector2i & v) const noexcept;
    bool operator != (const Vector2i & v) const noexcept;
    bool operator < (const Vector2i & v) const noexcept;
    bool operator > (const Vector2i & v) const noexcept;
    bool operator <= (const Vector2i & v) const noexcept;
    bool operator >= (const Vector2i & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Vector2i & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns) noexcept;
    void Set(Value nx, Value ny) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const Vector2i & v) noexcept;
    void Set(const Vector2u & v) noexcept;
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
    Vector2i Abs() const noexcept;
};

} // Namespace:: SqMod

#endif // _BASE_VECTOR2I_HPP_
