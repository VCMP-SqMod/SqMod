#ifndef _BASE_CIRCLE_HPP_
#define _BASE_CIRCLE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"
#include "Base/Vector2f.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Circle
{
    // --------------------------------------------------------------------------------------------
    typedef SQFloat Value;
    // --------------------------------------------------------------------------------------------
    static const Circle NIL;
    static const Circle MIN;
    static const Circle MAX;
    // --------------------------------------------------------------------------------------------
    static SQChar Delim;
    // --------------------------------------------------------------------------------------------
    Vector2f    pos;
    Value       rad;
    // --------------------------------------------------------------------------------------------
    Circle() noexcept;
    Circle(Value r) noexcept;
    Circle(const Vector2f & p) noexcept;
    Circle(const Vector2f & p, Value r) noexcept;
    Circle(Value x, Value y, Value r) noexcept;
    // --------------------------------------------------------------------------------------------
    Circle(const Circle & c) noexcept;
    Circle(Circle && c) noexcept;
    // --------------------------------------------------------------------------------------------
    ~Circle();
    // --------------------------------------------------------------------------------------------
    Circle & operator = (const Circle & c) noexcept;
    Circle & operator = (Circle && c) noexcept;
    // --------------------------------------------------------------------------------------------
    Circle & operator = (Value r) noexcept;
    Circle & operator = (const Vector2f & p) noexcept;
    // --------------------------------------------------------------------------------------------
    Circle & operator += (const Circle & c) noexcept;
    Circle & operator -= (const Circle & c) noexcept;
    Circle & operator *= (const Circle & c) noexcept;
    Circle & operator /= (const Circle & c) noexcept;
    Circle & operator %= (const Circle & c) noexcept;
    // --------------------------------------------------------------------------------------------
    Circle & operator += (Value r) noexcept;
    Circle & operator -= (Value r) noexcept;
    Circle & operator *= (Value r) noexcept;
    Circle & operator /= (Value r) noexcept;
    Circle & operator %= (Value r) noexcept;
    // --------------------------------------------------------------------------------------------
    Circle & operator += (const Vector2f & p) noexcept;
    Circle & operator -= (const Vector2f & p) noexcept;
    Circle & operator *= (const Vector2f & p) noexcept;
    Circle & operator /= (const Vector2f & p) noexcept;
    Circle & operator %= (const Vector2f & p) noexcept;
    // --------------------------------------------------------------------------------------------
    Circle & operator ++ () noexcept;
    Circle & operator -- () noexcept;
    // --------------------------------------------------------------------------------------------
    Circle operator ++ (int) noexcept;
    Circle operator -- (int) noexcept;
    // --------------------------------------------------------------------------------------------
    Circle operator + (const Circle & c) const noexcept;
    Circle operator - (const Circle & c) const noexcept;
    Circle operator * (const Circle & c) const noexcept;
    Circle operator / (const Circle & c) const noexcept;
    Circle operator % (const Circle & c) const noexcept;
    // --------------------------------------------------------------------------------------------
    Circle operator + (Value r) const noexcept;
    Circle operator - (Value r) const noexcept;
    Circle operator * (Value r) const noexcept;
    Circle operator / (Value r) const noexcept;
    Circle operator % (Value r) const noexcept;
    // --------------------------------------------------------------------------------------------
    Circle operator + (const Vector2f & p) const noexcept;
    Circle operator - (const Vector2f & p) const noexcept;
    Circle operator * (const Vector2f & p) const noexcept;
    Circle operator / (const Vector2f & p) const noexcept;
    Circle operator % (const Vector2f & p) const noexcept;
    // --------------------------------------------------------------------------------------------
    Circle operator + () const noexcept;
    Circle operator - () const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Circle & c) const noexcept;
    bool operator != (const Circle & c) const noexcept;
    bool operator < (const Circle & c) const noexcept;
    bool operator > (const Circle & c) const noexcept;
    bool operator <= (const Circle & c) const noexcept;
    bool operator >= (const Circle & c) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Circle & c) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value nr) noexcept;
    void Set(const Circle & nc) noexcept;
    void Set(const Vector2f & np) noexcept;
    void Set(const Vector2f & np, Value nr) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value nx, Value ny) noexcept;
    void Set(Value nx, Value ny, Value nr) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    void Generate() noexcept;
    void Generate(Value min, Value max, bool r) noexcept;
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax) noexcept;
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value rmin, Value rmax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Clear() noexcept { pos.Clear(); rad = 0.0; }
    // --------------------------------------------------------------------------------------------
    Circle Abs() const noexcept;
};

} // Namespace:: SqMod

#endif // _BASE_CIRCLE_HPP_
