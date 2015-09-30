#ifndef _BASE_VECTOR2F_HPP_
#define _BASE_VECTOR2F_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Vector2f
{
    // --------------------------------------------------------------------------------------------
    typedef SQFloat Value;
    // --------------------------------------------------------------------------------------------
    static const Vector2f NIL;
    static const Vector2f MIN;
    static const Vector2f MAX;
    // --------------------------------------------------------------------------------------------
    static SQChar Delim;
    // --------------------------------------------------------------------------------------------
    Value x, y;
    // --------------------------------------------------------------------------------------------
    Vector2f() noexcept;
    Vector2f(Value s) noexcept;
    Vector2f(Value xv, Value yv) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2f(const Vector2i & v) noexcept;
    Vector2f(const Vector2u & v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2f(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2f(const Vector2f & v) noexcept;
    Vector2f(Vector2f && v) noexcept;
    // --------------------------------------------------------------------------------------------
    ~Vector2f();
    // --------------------------------------------------------------------------------------------
    Vector2f & operator = (const Vector2f & v) noexcept;
    Vector2f & operator = (Vector2f && v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2f & operator = (Value s) noexcept;
    Vector2f & operator = (const SQChar * values) noexcept;
    Vector2f & operator = (const Vector2i & v) noexcept;
    Vector2f & operator = (const Vector2u & v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2f & operator += (const Vector2f & v) noexcept;
    Vector2f & operator -= (const Vector2f & v) noexcept;
    Vector2f & operator *= (const Vector2f & v) noexcept;
    Vector2f & operator /= (const Vector2f & v) noexcept;
    Vector2f & operator %= (const Vector2f & v) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2f & operator += (Value s) noexcept;
    Vector2f & operator -= (Value s) noexcept;
    Vector2f & operator *= (Value s) noexcept;
    Vector2f & operator /= (Value s) noexcept;
    Vector2f & operator %= (Value s) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2f & operator ++ () noexcept;
    Vector2f & operator -- () noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2f operator ++ (int) noexcept;
    Vector2f operator -- (int) noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2f operator + (const Vector2f & v) const noexcept;
    Vector2f operator - (const Vector2f & v) const noexcept;
    Vector2f operator * (const Vector2f & v) const noexcept;
    Vector2f operator / (const Vector2f & v) const noexcept;
    Vector2f operator % (const Vector2f & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2f operator + (Value s) const noexcept;
    Vector2f operator - (Value s) const noexcept;
    Vector2f operator * (Value s) const noexcept;
    Vector2f operator / (Value s) const noexcept;
    Vector2f operator % (Value s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Vector2f operator + () const noexcept;
    Vector2f operator - () const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Vector2f & v) const noexcept;
    bool operator != (const Vector2f & v) const noexcept;
    bool operator < (const Vector2f & v) const noexcept;
    bool operator > (const Vector2f & v) const noexcept;
    bool operator <= (const Vector2f & v) const noexcept;
    bool operator >= (const Vector2f & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Vector2f & v) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns) noexcept;
    void Set(Value nx, Value ny) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const Vector2f & v) noexcept;
    void Set(const Vector2i & v) noexcept;
    void Set(const Vector2u & v) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    void Generate() noexcept;
    void Generate(Value min, Value max) noexcept;
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Clear() noexcept { x = 0.0, y = 0.0; }
    // --------------------------------------------------------------------------------------------
    Vector2f Abs() const noexcept;
};

} // Namespace:: SqMod

#endif // _BASE_VECTOR2F_HPP_
