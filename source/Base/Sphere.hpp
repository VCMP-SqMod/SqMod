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
    Sphere() noexcept;
    Sphere(Value r) noexcept;
    Sphere(const Vector3 & p) noexcept;
    Sphere(const Vector3 & p, Value r) noexcept;
    Sphere(Value x, Value y, Value z, Value r) noexcept;
    // --------------------------------------------------------------------------------------------
    Sphere(const Sphere & s) noexcept;
    Sphere(Sphere && s) noexcept;
    // --------------------------------------------------------------------------------------------
    ~Sphere();
    // --------------------------------------------------------------------------------------------
    Sphere & operator = (const Sphere & s) noexcept;
    Sphere & operator = (Sphere && s) noexcept;
    // --------------------------------------------------------------------------------------------
    Sphere & operator = (Value r) noexcept;
    Sphere & operator = (const Vector3 & p) noexcept;
    // --------------------------------------------------------------------------------------------
    Sphere & operator += (const Sphere & s) noexcept;
    Sphere & operator -= (const Sphere & s) noexcept;
    Sphere & operator *= (const Sphere & s) noexcept;
    Sphere & operator /= (const Sphere & s) noexcept;
    Sphere & operator %= (const Sphere & s) noexcept;
    // --------------------------------------------------------------------------------------------
    Sphere & operator += (Value r) noexcept;
    Sphere & operator -= (Value r) noexcept;
    Sphere & operator *= (Value r) noexcept;
    Sphere & operator /= (Value r) noexcept;
    Sphere & operator %= (Value r) noexcept;
    // --------------------------------------------------------------------------------------------
    Sphere & operator += (const Vector3 & p) noexcept;
    Sphere & operator -= (const Vector3 & p) noexcept;
    Sphere & operator *= (const Vector3 & p) noexcept;
    Sphere & operator /= (const Vector3 & p) noexcept;
    Sphere & operator %= (const Vector3 & p) noexcept;
    // --------------------------------------------------------------------------------------------
    Sphere & operator ++ () noexcept;
    Sphere & operator -- () noexcept;
    // --------------------------------------------------------------------------------------------
    Sphere operator ++ (int) noexcept;
    Sphere operator -- (int) noexcept;
    // --------------------------------------------------------------------------------------------
    Sphere operator + (const Sphere & s) const noexcept;
    Sphere operator - (const Sphere & s) const noexcept;
    Sphere operator * (const Sphere & s) const noexcept;
    Sphere operator / (const Sphere & s) const noexcept;
    Sphere operator % (const Sphere & s) const noexcept;
    // --------------------------------------------------------------------------------------------
    Sphere operator + (Value r) const noexcept;
    Sphere operator - (Value r) const noexcept;
    Sphere operator * (Value r) const noexcept;
    Sphere operator / (Value r) const noexcept;
    Sphere operator % (Value r) const noexcept;
    // --------------------------------------------------------------------------------------------
    Sphere operator + (const Vector3 & p) const noexcept;
    Sphere operator - (const Vector3 & p) const noexcept;
    Sphere operator * (const Vector3 & p) const noexcept;
    Sphere operator / (const Vector3 & p) const noexcept;
    Sphere operator % (const Vector3 & p) const noexcept;
    // --------------------------------------------------------------------------------------------
    Sphere operator + () const noexcept;
    Sphere operator - () const noexcept;
    // --------------------------------------------------------------------------------------------
    bool operator == (const Sphere & s) const noexcept;
    bool operator != (const Sphere & s) const noexcept;
    bool operator < (const Sphere & s) const noexcept;
    bool operator > (const Sphere & s) const noexcept;
    bool operator <= (const Sphere & s) const noexcept;
    bool operator >= (const Sphere & s) const noexcept;
    // --------------------------------------------------------------------------------------------
    SQInteger Cmp(const Sphere & s) const noexcept;
    // --------------------------------------------------------------------------------------------
    const SQChar * ToString() const noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value nr) noexcept;
    void Set(const Sphere & ns) noexcept;
    void Set(const Vector3 & np) noexcept;
    void Set(const Vector3 & np, Value nr) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(Value nx, Value ny, Value nz) noexcept;
    void Set(Value nx, Value ny, Value nz, Value nr) noexcept;
    // --------------------------------------------------------------------------------------------
    void Set(const SQChar * values, SQChar delim) noexcept;
    // --------------------------------------------------------------------------------------------
    void Generate() noexcept;
    void Generate(Value min, Value max, bool r) noexcept;
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax) noexcept;
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value rmin, Value rmax) noexcept;
    // --------------------------------------------------------------------------------------------
    void Clear() noexcept { pos.Clear(); rad = 0.0; }
    // --------------------------------------------------------------------------------------------
    Sphere Abs() const noexcept;
};

} // Namespace:: SqMod

#endif // _BASE_SPHERE_HPP_
