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
    Vector3();
    Vector3(Value s);
    Vector3(Value xv, Value yv, Value zv);
    // --------------------------------------------------------------------------------------------
    Vector3(const Vector4 & v);
    Vector3(const Quaternion & q);
    // --------------------------------------------------------------------------------------------
    Vector3(const SQChar * values, char delim);
    // --------------------------------------------------------------------------------------------
    Vector3(const Vector3 & v);
    Vector3(Vector3 && v);
    // --------------------------------------------------------------------------------------------
    ~Vector3();
    // --------------------------------------------------------------------------------------------
    Vector3 & operator = (const Vector3 & v);
    Vector3 & operator = (Vector3 && v);
    // --------------------------------------------------------------------------------------------
    Vector3 & operator = (Value s);
    Vector3 & operator = (const Vector4 & v);
    Vector3 & operator = (const Quaternion & q);
    // --------------------------------------------------------------------------------------------
    Vector3 & operator += (const Vector3 & v);
    Vector3 & operator -= (const Vector3 & v);
    Vector3 & operator *= (const Vector3 & v);
    Vector3 & operator /= (const Vector3 & v);
    Vector3 & operator %= (const Vector3 & v);
    // --------------------------------------------------------------------------------------------
    Vector3 & operator += (Value s);
    Vector3 & operator -= (Value s);
    Vector3 & operator *= (Value s);
    Vector3 & operator /= (Value s);
    Vector3 & operator %= (Value s);
    // --------------------------------------------------------------------------------------------
    Vector3 & operator ++ ();
    Vector3 & operator -- ();
    // --------------------------------------------------------------------------------------------
    Vector3 operator ++ (int);
    Vector3 operator -- (int);
    // --------------------------------------------------------------------------------------------
    Vector3 operator + (const Vector3 & v) const;
    Vector3 operator - (const Vector3 & v) const;
    Vector3 operator * (const Vector3 & v) const;
    Vector3 operator / (const Vector3 & v) const;
    Vector3 operator % (const Vector3 & v) const;
    // --------------------------------------------------------------------------------------------
    Vector3 operator + (Value s) const;
    Vector3 operator - (Value s) const;
    Vector3 operator * (Value s) const;
    Vector3 operator / (Value s) const;
    Vector3 operator % (Value s) const;
    // --------------------------------------------------------------------------------------------
    Vector3 operator + () const;
    Vector3 operator - () const;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Vector3 & v) const;
    bool operator != (const Vector3 & v) const;
    bool operator < (const Vector3 & v) const;
    bool operator > (const Vector3 & v) const;
    bool operator <= (const Vector3 & v) const;
    bool operator >= (const Vector3 & v) const;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Vector3 & v) const;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const;
    // --------------------------------------------------------------------------------------------
    void Set(Value ns);
    void Set(Value nx, Value ny, Value nz);
    // --------------------------------------------------------------------------------------------
    void Set(const Vector3 & v);
    void Set(const Vector4 & v);
    void Set(const Quaternion & q);
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, char delim);
    // --------------------------------------------------------------------------------------------
    void Generate();
    void Generate(Value min, Value max);
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax);
    // --------------------------------------------------------------------------------------------
    void Clear() { x = 0.0, y = 0.0, z = 0.0; }
    // --------------------------------------------------------------------------------------------
    Vector3 Abs() const;
};

} // Namespace:: SqMod

#endif // _BASE_VECTOR3_HPP_
