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
    Vector4();
    Vector4(Value s);
    Vector4(Value xv, Value yv, Value zv);
    Vector4(Value xv, Value yv, Value zv, Value wv);
    // --------------------------------------------------------------------------------------------
    Vector4(const Vector3 & v);
    Vector4(const Quaternion & q);
    // --------------------------------------------------------------------------------------------
    Vector4(const SQChar * values, SQChar delim);
    // --------------------------------------------------------------------------------------------
    Vector4(const Vector4 & v);
    Vector4(Vector4 && v);
    // --------------------------------------------------------------------------------------------
    ~Vector4();
    // --------------------------------------------------------------------------------------------
    Vector4 & operator = (const Vector4 & v);
    Vector4 & operator = (Vector4 && v);
    // --------------------------------------------------------------------------------------------
    Vector4 & operator = (Value s);
    Vector4 & operator = (const Vector3 & v);
    Vector4 & operator = (const Quaternion & q);
    // --------------------------------------------------------------------------------------------
    Vector4 & operator += (const Vector4 & v);
    Vector4 & operator -= (const Vector4 & v);
    Vector4 & operator *= (const Vector4 & v);
    Vector4 & operator /= (const Vector4 & v);
    Vector4 & operator %= (const Vector4 & v);
    // --------------------------------------------------------------------------------------------
    Vector4 & operator += (Value s);
    Vector4 & operator -= (Value s);
    Vector4 & operator *= (Value s);
    Vector4 & operator /= (Value s);
    Vector4 & operator %= (Value s);
    // --------------------------------------------------------------------------------------------
    Vector4 & operator ++ ();
    Vector4 & operator -- ();
    // --------------------------------------------------------------------------------------------
    Vector4 operator ++ (int);
    Vector4 operator -- (int); 
    // --------------------------------------------------------------------------------------------
    Vector4 operator + (const Vector4 & v) const;
    Vector4 operator - (const Vector4 & v) const;
    Vector4 operator * (const Vector4 & v) const;
    Vector4 operator / (const Vector4 & v) const;
    Vector4 operator % (const Vector4 & v) const;
    // --------------------------------------------------------------------------------------------
    Vector4 operator + (Value s) const;
    Vector4 operator - (Value s) const;
    Vector4 operator * (Value s) const;
    Vector4 operator / (Value s) const;
    Vector4 operator % (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector4 operator + () const;
    Vector4 operator - () const;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Vector4 & v) const;
    bool operator != (const Vector4 & v) const;
    bool operator < (const Vector4 & v) const;
    bool operator > (const Vector4 & v) const;
    bool operator <= (const Vector4 & v) const;
    bool operator >= (const Vector4 & v) const;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Vector4 & v) const;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns);
    void Set(Value nx, Value ny, Value nz);
    void Set(Value nx, Value ny, Value nz, Value nw);
    // --------------------------------------------------------------------------------------------
    void Set(const Vector4 & v);
    void Set(const Vector3 & v);
    void Set(const Quaternion & q);
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, SQChar delim);
    // --------------------------------------------------------------------------------------------
    void Generate();
    void Generate(Value min, Value max);
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value wmin, Value wmax);
    // --------------------------------------------------------------------------------------------
    void Clear() { x = 0.0, y = 0.0, z = 0.0, w = 0.0; }
    // --------------------------------------------------------------------------------------------
    Vector4 Abs() const;
};

} // Namespace:: SqMod

#endif // _BASE_VECTOR4_HPP_
