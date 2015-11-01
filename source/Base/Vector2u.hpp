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
    Vector2u();
    Vector2u(Value s);
    Vector2u(Value xv, Value yv);
    // --------------------------------------------------------------------------------------------
    Vector2u(const Vector2i & v);
    Vector2u(const Vector2f & v);
    // --------------------------------------------------------------------------------------------
    Vector2u(const SQChar * values, SQChar delim);
    // --------------------------------------------------------------------------------------------
    Vector2u(const Vector2u & v);
    Vector2u(Vector2u && v);
    // --------------------------------------------------------------------------------------------
    ~Vector2u();
    // --------------------------------------------------------------------------------------------
    Vector2u & operator = (const Vector2u & v);
    Vector2u & operator = (Vector2u && v);
    // --------------------------------------------------------------------------------------------
    Vector2u & operator = (Value s);
    Vector2u & operator = (const SQChar * values);
    Vector2u & operator = (const Vector2i & v);
    Vector2u & operator = (const Vector2f & v);
    // --------------------------------------------------------------------------------------------
    Vector2u & operator += (const Vector2u & v);
    Vector2u & operator -= (const Vector2u & v);
    Vector2u & operator *= (const Vector2u & v);
    Vector2u & operator /= (const Vector2u & v);
    Vector2u & operator %= (const Vector2u & v);
    Vector2u & operator &= (const Vector2u & v);
    Vector2u & operator |= (const Vector2u & v);
    Vector2u & operator ^= (const Vector2u & v);
    Vector2u & operator <<= (const Vector2u & v);
    Vector2u & operator >>= (const Vector2u & v); 
    // --------------------------------------------------------------------------------------------
    Vector2u & operator += (Value s);
    Vector2u & operator -= (Value s);
    Vector2u & operator *= (Value s);
    Vector2u & operator /= (Value s);
    Vector2u & operator %= (Value s);
    Vector2u & operator &= (Value s);
    Vector2u & operator |= (Value s);
    Vector2u & operator ^= (Value s);
    Vector2u & operator <<= (Value s);
    Vector2u & operator >>= (Value s); 
    // --------------------------------------------------------------------------------------------
    Vector2u & operator ++ ();
    Vector2u & operator -- ();
    // --------------------------------------------------------------------------------------------
    Vector2u operator ++ (int);
    Vector2u operator -- (int); 
    // --------------------------------------------------------------------------------------------
    Vector2u operator + (const Vector2u & v) const;
    Vector2u operator + (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector2u operator - (const Vector2u & v) const;
    Vector2u operator - (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector2u operator * (const Vector2u & v) const;
    Vector2u operator * (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector2u operator / (const Vector2u & v) const;
    Vector2u operator / (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector2u operator % (const Vector2u & v) const;
    Vector2u operator % (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector2u operator & (const Vector2u & v) const;
    Vector2u operator & (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector2u operator | (const Vector2u & v) const;
    Vector2u operator | (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector2u operator ^ (const Vector2u & v) const;
    Vector2u operator ^ (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector2u operator << (const Vector2u & v) const;
    Vector2u operator << (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector2u operator >> (const Vector2u & v) const;
    Vector2u operator >> (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector2u operator + () const;
    Vector2u operator - () const;
    // --------------------------------------------------------------------------------------------
    Vector2u operator ~ () const;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Vector2u & v) const;
    bool operator != (const Vector2u & v) const;
    bool operator < (const Vector2u & v) const;
    bool operator > (const Vector2u & v) const;
    bool operator <= (const Vector2u & v) const;
    bool operator >= (const Vector2u & v) const;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Vector2u & v) const;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns);
    void Set(Value nx, Value ny);
    // --------------------------------------------------------------------------------------------
    void Set(const Vector2u & v);
    void Set(const Vector2i & v);
    void Set(const Vector2f & v);
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, SQChar delim);
    // --------------------------------------------------------------------------------------------
    void Generate();
    void Generate(Value min, Value max);
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax);
    // --------------------------------------------------------------------------------------------
    void Clear() { x = 0, y = 0; }
    // --------------------------------------------------------------------------------------------
    Vector2u Abs() const;
};

} // Namespace:: SqMod

#endif // _BASE_VECTOR2U_HPP_
