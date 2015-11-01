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
    Vector2i();
    Vector2i(Value s);
    Vector2i(Value xv, Value yv);
    // --------------------------------------------------------------------------------------------
    Vector2i(const Vector2u & v);
    Vector2i(const Vector2f & v);
    // --------------------------------------------------------------------------------------------
    Vector2i(const SQChar * values, SQChar delim);
    // --------------------------------------------------------------------------------------------
    Vector2i(const Vector2i & v);
    Vector2i(Vector2i && v);
    // --------------------------------------------------------------------------------------------
    ~Vector2i();
    // --------------------------------------------------------------------------------------------
    Vector2i & operator = (const Vector2i & v);
    Vector2i & operator = (Vector2i && v);
    // --------------------------------------------------------------------------------------------
    Vector2i & operator = (Value s);
    Vector2i & operator = (const SQChar * values);
    Vector2i & operator = (const Vector2u & v);
    Vector2i & operator = (const Vector2f & v);
    // --------------------------------------------------------------------------------------------
    Vector2i & operator += (const Vector2i & v);
    Vector2i & operator -= (const Vector2i & v);
    Vector2i & operator *= (const Vector2i & v);
    Vector2i & operator /= (const Vector2i & v);
    Vector2i & operator %= (const Vector2i & v);
    Vector2i & operator &= (const Vector2i & v);
    Vector2i & operator |= (const Vector2i & v);
    Vector2i & operator ^= (const Vector2i & v);
    Vector2i & operator <<= (const Vector2i & v);
    Vector2i & operator >>= (const Vector2i & v); 
    // --------------------------------------------------------------------------------------------
    Vector2i & operator += (Value s);
    Vector2i & operator -= (Value s);
    Vector2i & operator *= (Value s);
    Vector2i & operator /= (Value s);
    Vector2i & operator %= (Value s);
    Vector2i & operator &= (Value s);
    Vector2i & operator |= (Value s);
    Vector2i & operator ^= (Value s);
    Vector2i & operator <<= (Value s);
    Vector2i & operator >>= (Value s); 
    // --------------------------------------------------------------------------------------------
    Vector2i & operator ++ ();
    Vector2i & operator -- ();
    // --------------------------------------------------------------------------------------------
    Vector2i operator ++ (int);
    Vector2i operator -- (int); 
    // --------------------------------------------------------------------------------------------
    Vector2i operator + (const Vector2i & v) const;
    Vector2i operator - (const Vector2i & v) const;
    Vector2i operator * (const Vector2i & v) const;
    Vector2i operator / (const Vector2i & v) const;
    Vector2i operator % (const Vector2i & v) const;
    Vector2i operator & (const Vector2i & v) const;
    Vector2i operator | (const Vector2i & v) const;
    Vector2i operator ^ (const Vector2i & v) const;
    Vector2i operator << (const Vector2i & v) const;
    Vector2i operator >> (const Vector2i & v) const;
    // --------------------------------------------------------------------------------------------
    Vector2i operator + (Value s) const;
    Vector2i operator - (Value s) const;
    Vector2i operator * (Value s) const;
    Vector2i operator / (Value s) const;
    Vector2i operator % (Value s) const;
    Vector2i operator & (Value s) const;
    Vector2i operator | (Value s) const;
    Vector2i operator ^ (Value s) const;
    Vector2i operator << (Value s) const;
    Vector2i operator >> (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector2i operator + () const;
    Vector2i operator - () const;
    // --------------------------------------------------------------------------------------------
    Vector2i operator ~ () const;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Vector2i & v) const;
    bool operator != (const Vector2i & v) const;
    bool operator < (const Vector2i & v) const;
    bool operator > (const Vector2i & v) const;
    bool operator <= (const Vector2i & v) const;
    bool operator >= (const Vector2i & v) const;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Vector2i & v) const;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns);
    void Set(Value nx, Value ny);
    // --------------------------------------------------------------------------------------------
    void Set(const Vector2i & v);
    void Set(const Vector2u & v);
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
    Vector2i Abs() const;
};

} // Namespace:: SqMod

#endif // _BASE_VECTOR2I_HPP_
