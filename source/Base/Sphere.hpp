#ifndef _BASE_SPHERE_HPP_
#define _BASE_SPHERE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Config.hpp"
#include "Base/Vector3.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
struct Sphere
{
    // --------------------------------------------------------------------------------------------
    typedef SQFloat Value;
    // --------------------------------------------------------------------------------------------
    static const Sphere NIL;
    static const Sphere MIN;
    static const Sphere MAX;
    // --------------------------------------------------------------------------------------------
    static SQChar Delim;
    // --------------------------------------------------------------------------------------------
    Vector3     pos;
    Value       rad;
    // --------------------------------------------------------------------------------------------
    Sphere();
    Sphere(Value r);
    Sphere(const Vector3 & p);
    Sphere(const Vector3 & p, Value r);
    Sphere(Value x, Value y, Value z, Value r);
    // --------------------------------------------------------------------------------------------
    Sphere(const Sphere & s);
    Sphere(Sphere && s);
    // --------------------------------------------------------------------------------------------
    ~Sphere();
    // --------------------------------------------------------------------------------------------
    Sphere & operator = (const Sphere & s);
    Sphere & operator = (Sphere && s);
    // --------------------------------------------------------------------------------------------
    Sphere & operator = (Value r);
    Sphere & operator = (const Vector3 & p);
    // --------------------------------------------------------------------------------------------
    Sphere & operator += (const Sphere & s);
    Sphere & operator -= (const Sphere & s);
    Sphere & operator *= (const Sphere & s);
    Sphere & operator /= (const Sphere & s);
    Sphere & operator %= (const Sphere & s);
    // --------------------------------------------------------------------------------------------
    Sphere & operator += (Value r);
    Sphere & operator -= (Value r);
    Sphere & operator *= (Value r);
    Sphere & operator /= (Value r);
    Sphere & operator %= (Value r);
    // --------------------------------------------------------------------------------------------
    Sphere & operator += (const Vector3 & p);
    Sphere & operator -= (const Vector3 & p);
    Sphere & operator *= (const Vector3 & p);
    Sphere & operator /= (const Vector3 & p);
    Sphere & operator %= (const Vector3 & p);
    // --------------------------------------------------------------------------------------------
    Sphere & operator ++ ();
    Sphere & operator -- ();
    // --------------------------------------------------------------------------------------------
    Sphere operator ++ (int);
    Sphere operator -- (int);
    // --------------------------------------------------------------------------------------------
    Sphere operator + (const Sphere & s) const;
    Sphere operator - (const Sphere & s) const;
    Sphere operator * (const Sphere & s) const;
    Sphere operator / (const Sphere & s) const;
    Sphere operator % (const Sphere & s) const;
    // --------------------------------------------------------------------------------------------
    Sphere operator + (Value r) const;
    Sphere operator - (Value r) const;
    Sphere operator * (Value r) const;
    Sphere operator / (Value r) const;
    Sphere operator % (Value r) const;
    // --------------------------------------------------------------------------------------------
    Sphere operator + (const Vector3 & p) const;
    Sphere operator - (const Vector3 & p) const;
    Sphere operator * (const Vector3 & p) const;
    Sphere operator / (const Vector3 & p) const;
    Sphere operator % (const Vector3 & p) const;
    // --------------------------------------------------------------------------------------------
    Sphere operator + () const;
    Sphere operator - () const;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Sphere & s) const;
    bool operator != (const Sphere & s) const;
    bool operator < (const Sphere & s) const;
    bool operator > (const Sphere & s) const;
    bool operator <= (const Sphere & s) const;
    bool operator >= (const Sphere & s) const;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Sphere & s) const;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const;
    // --------------------------------------------------------------------------------------------
    void Set(Value nr);
    void Set(const Sphere & ns);
    void Set(const Vector3 & np);
    void Set(const Vector3 & np, Value nr);
    // --------------------------------------------------------------------------------------------
    void Set(Value nx, Value ny, Value nz);
    void Set(Value nx, Value ny, Value nz, Value nr);
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, SQChar delim);
    // --------------------------------------------------------------------------------------------
    void Generate();
    void Generate(Value min, Value max, bool r);
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax);
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value rmin, Value rmax);
    // --------------------------------------------------------------------------------------------
    void Clear() { pos.Clear(); rad = 0.0; }
    // --------------------------------------------------------------------------------------------
    Sphere Abs() const;
};

} // Namespace:: SqMod

#endif // _BASE_SPHERE_HPP_
