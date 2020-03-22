#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Vector3.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class used to represent an axis aligned bounding box in three-dimensional space.
*/
struct AABB
{
    /* --------------------------------------------------------------------------------------------
     * The type of value used by components of type.
    */
    typedef float Value;

    /* --------------------------------------------------------------------------------------------
     * Helper instances for common values mostly used as return types or comparison.
    */
    static const AABB NIL;
    static const AABB MIN;
    static const AABB MAX;

    /* --------------------------------------------------------------------------------------------
     * The delimiter character to be used when extracting values from strings.
    */
    static SQChar Delim;

    /* --------------------------------------------------------------------------------------------
     * The minimum and maximum components of this type.
    */
    Vector3 min, max;

    /* --------------------------------------------------------------------------------------------
     * Construct with zero size.
    */
    AABB() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct a an equally sized and perfectly shaped box from scalar values.
    */
    explicit AABB(Value mins, Value maxs) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct a an equally sized but imperfectly shaped box from individual components of a
     * three-dimensional point.
    */
    AABB(Value xv, Value yv, Value zv) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct a an unequally sized and imperfectly shaped box from individual components of two
     * three-dimensional points.
    */
    AABB(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct a an unequally sized and imperfectly shaped box from two three-dimensional
     * vectors representing two three-dimensional points.
    */
    AABB(const Vector3 & vmin, const Vector3 & vmax) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    AABB(const AABB & o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    AABB(AABB && o)  noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~AABB() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    AABB & operator = (const AABB & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    AABB & operator = (AABB && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Three-dimensional vector assignment operator.
    */
    AABB & operator = (const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Addition assignment operator.
    */
    AABB & operator += (const AABB & b);

    /* --------------------------------------------------------------------------------------------
     * Subtraction assignment operator.
    */
    AABB & operator -= (const AABB & b);

    /* --------------------------------------------------------------------------------------------
     * Multiplication assignment operator.
    */
    AABB & operator *= (const AABB & b);

    /* --------------------------------------------------------------------------------------------
     * Division assignment operator.
    */
    AABB & operator /= (const AABB & b);

    /* --------------------------------------------------------------------------------------------
     * Modulo assignment operator.
    */
    AABB & operator %= (const AABB & b);

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition assignment operator.
    */
    AABB & operator += (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction assignment operator.
    */
    AABB & operator -= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication assignment operator.
    */
    AABB & operator *= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value division assignment operator.
    */
    AABB & operator /= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo assignment operator.
    */
    AABB & operator %= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Pre-increment operator.
    */
    AABB & operator ++ ();

    /* --------------------------------------------------------------------------------------------
     * Pre-decrement operator.
    */
    AABB & operator -- ();

    /* --------------------------------------------------------------------------------------------
     * Post-increment operator.
    */
    AABB operator ++ (int); // NOLINT(cert-dcl21-cpp)

    /* --------------------------------------------------------------------------------------------
     * Post-decrement operator.
    */
    AABB operator -- (int); // NOLINT(cert-dcl21-cpp)

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    AABB operator + (const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    AABB operator - (const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    AABB operator * (const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    AABB operator / (const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Modulo operator.
    */
    AABB operator % (const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition operator.
    */
    AABB operator + (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction operator.
    */
    AABB operator - (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication operator.
    */
    AABB operator * (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value division operator.
    */
    AABB operator / (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo operator.
    */
    AABB operator % (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Unary plus operator.
    */
    AABB operator + () const;

    /* --------------------------------------------------------------------------------------------
     * Unary minus operator.
    */
    AABB operator - () const;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const AABB & b) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQFloat s) const
    {
        return Cmp(AABB(s, s, s, s, s, s));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQInteger s) const
    {
        const auto v = static_cast< Value >(s);
        return Cmp(AABB(v, v, v, v, v, v));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(bool s) const
    {
        const auto v = static_cast< Value >(s);
        return Cmp(AABB(v, v, v, v, v, v));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(std::nullptr_t) const
    {
        const auto v = static_cast< Value >(0);
        return Cmp(AABB(v, v, v, v, v, v));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Set the values extracted from the specified string using the specified delimiter.
    */
    void SetStr(SQChar delim, StackStrF & values);

    /* --------------------------------------------------------------------------------------------
     * Clear the component values to default.
    */
    void Clear();

    /* --------------------------------------------------------------------------------------------
     * Define from minimum and maximum floats (all dimensions same).
    */
    void DefineScalar(Value mins, Value maxs);

    /* --------------------------------------------------------------------------------------------
     * Define from a point.
    */
    void DefineVector3(const Vector3 & point);

    /* --------------------------------------------------------------------------------------------
     * Define from a point.
    */
    void DefineVector3Ex(Value x, Value y, Value z);

    /* --------------------------------------------------------------------------------------------
     * Define from minimum and maximum vectors.
    */
    void DefineAllVector3(const Vector3 & nmin, const Vector3 & nmax);

    /* --------------------------------------------------------------------------------------------
     * Define from minimum and maximum vectors.
    */
    void DefineAllVector3Ex(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax);

    /* --------------------------------------------------------------------------------------------
     * Define from another bounding box.
    */
    void DefineAABB(const AABB & box);

    /* --------------------------------------------------------------------------------------------
     * Define from a sphere.
    */
    void DefineSphere(const Sphere & sphere);

    /* --------------------------------------------------------------------------------------------
     * Define from a sphere.
    */
    void DefineSphereEx(Value x, Value y, Value z, Value r);

    /* --------------------------------------------------------------------------------------------
     * Merge a point into this bounding box.
    */
    void MergeVector3(const Vector3 & point);

    /* --------------------------------------------------------------------------------------------
     * Merge a point into this bounding box.
    */
    void MergeVector3Ex(Value x, Value y, Value z);

    /* --------------------------------------------------------------------------------------------
     * Merge another bounding box into this bounding box.
    */
    void MergeAABB(const AABB & box);

    /* --------------------------------------------------------------------------------------------
     * Merge another bounding box into this bounding box.
    */
    void MergeAABBEx(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax);

    /* --------------------------------------------------------------------------------------------
     * Merge a sphere into this bounding box.
    */
    void MergeSphere(const Sphere & sphere);

    /* --------------------------------------------------------------------------------------------
     * Merge a sphere into this bounding box.
    */
    void MergeSphereEx(Value x, Value y, Value z, Value r);

    /* --------------------------------------------------------------------------------------------
     * Check if the box is empty. This means that there is no space between the min and max edge.
    */
    bool Empty() const;

    /* --------------------------------------------------------------------------------------------
     * Return true if this bounding box is defined via a previous call to Define() or Merge().
    */
    bool Defined() const;

    /* --------------------------------------------------------------------------------------------
     * Return center.
    */
    Vector3 Center() const;

    /* --------------------------------------------------------------------------------------------
     * Get size/extent of the box (maximal distance of two points in the box).
    */
    Vector3 Size() const;

    /* --------------------------------------------------------------------------------------------
     * Return half-size.
    */
    Vector3 HalfSize() const;

    /* --------------------------------------------------------------------------------------------
     * Get radius of the bounding sphere.
    */
    Value Radius() const;

    /* --------------------------------------------------------------------------------------------
     * Get the volume enclosed by the box in cubed units.
    */
    Value Volume() const;

    /* --------------------------------------------------------------------------------------------
     * Get the surface area of the box in squared units.
    */
    Value Area() const;

    /* --------------------------------------------------------------------------------------------
     * Test if a point is inside.
    */
    Int32 IsVector3Inside(const Vector3 & point) const;

    /* --------------------------------------------------------------------------------------------
     * Test if a point is inside.
    */
    Int32 IsVector3InsideEx(Value x, Value y, Value z) const;

    /* --------------------------------------------------------------------------------------------
     * Test if another bounding box is inside, outside or intersects.
    */
    Int32 IsAABBInside(const AABB & box) const;

    /* --------------------------------------------------------------------------------------------
     * Test if another bounding box is inside, outside or intersects.
    */
    Int32 IsAABBInsideEx(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax) const;

    /* --------------------------------------------------------------------------------------------
     * Test if another bounding box is (partially) inside or outside.
    */
    Int32 IsAABBInsideFast(const AABB & box) const;

    /* --------------------------------------------------------------------------------------------
     * Test if another bounding box is (partially) inside or outside.
    */
    Int32 IsAABBInsideFastEx(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax) const;

    /* --------------------------------------------------------------------------------------------
     * Test if a sphere is inside, outside or intersects.
    */
    Int32 IsSphereInside(const Sphere & sphere) const;

    /* --------------------------------------------------------------------------------------------
     * Test if a sphere is inside, outside or intersects.
    */
    Int32 IsSphereInsideEx(Value x, Value y, Value z, Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Test if a sphere is (partially) inside or outside.
    */
    Int32 IsSphereInsideFast(const Sphere & sphere) const;

    /* --------------------------------------------------------------------------------------------
     * Test if a sphere is (partially) inside or outside.
    */
    Int32 IsSphereInsideFastEx(Value x, Value y, Value z, Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the AABB type from a string.
    */
    static const AABB & Get(StackStrF & str);

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the AABB type from a string.
    */
    static const AABB & GetEx(SQChar delim, StackStrF & str);
};

} // Namespace:: SqMod
