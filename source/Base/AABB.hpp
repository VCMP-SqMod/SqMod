#ifndef _BASE_AABB_HPP_
#define _BASE_AABB_HPP_

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
     * Default constructor.
    */
    AABB();

    /* --------------------------------------------------------------------------------------------
     * Construct a an equally sized and perfectly shaped box from a scalar value.
    */
    AABB(Value sv);

    /* --------------------------------------------------------------------------------------------
     * Construct a an equally sized but imperfectly shaped box from individual components of a
     * three-dimensional point.
    */
    AABB(Value xv, Value yv, Value zv);

    /* --------------------------------------------------------------------------------------------
     * Construct a an unequally sized and imperfectly shaped box from individual components of two
     * three-dimensional points.
    */
    AABB(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax);

    /* --------------------------------------------------------------------------------------------
     * Construct a an unequally sized and imperfectly shaped box from two three-dimensional
     * vectors representing two three-dimensional points.
    */
    AABB(const Vector3 & vmin, const Vector3 & vmax);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    AABB(const AABB & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    AABB(AABB && o) = default;

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
     * Scalar value assignment operator.
    */
    AABB & operator = (Value s);

    /* --------------------------------------------------------------------------------------------
     * Three-dimensional vector assignment operator.
    */
    AABB & operator = (const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Four-dimensional vector assignment operator threated as a three-dimensional vector.
    */
    AABB & operator = (const Vector4 & v);

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
    AABB operator ++ (int);

    /* --------------------------------------------------------------------------------------------
     * Post-decrement operator.
    */
    AABB operator -- (int);

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
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Set an equally sized and perfectly shaped box from a scalar value.
    */
    void Set(Value ns);

    /* --------------------------------------------------------------------------------------------
     * Set an equally sized but imperfectly shaped box from individual components of a
     * three-dimensional point.
    */
    void Set(Value nx, Value ny, Value nz);

    /* --------------------------------------------------------------------------------------------
     * Set an unequally sized and imperfectly shaped box from individual components of two
     * three-dimensional points.
    */
    void Set(Value xmin, Value ymin, Value zmin, Value xmax, Value ymax, Value zmax);

    /* --------------------------------------------------------------------------------------------
     * Set the same box from another instance of this type.
    */
    void Set(const AABB & b);

    /* --------------------------------------------------------------------------------------------
     * Set an equally sized and imperfectly shaped box from a single three-dimensional vector
     * representing a single three-dimensional point.
    */
    void Set(const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Set an unequally sized and imperfectly shaped box from two three-dimensional vectors
     * representing two three-dimensional points.
    */
    void Set(const Vector3 & nmin, const Vector3 & nmax);

    /* --------------------------------------------------------------------------------------------
     * Set an equally sized and imperfectly shaped box from a single four-dimensional vector
     * representing a single three-dimensional point.
    */
    void Set(const Vector4 & v);

    /* --------------------------------------------------------------------------------------------
     * Set an unequally sized and imperfectly shaped box from two four-dimensional vectors
     * representing two three-dimensional points.
    */
    void Set(const Vector4 & nmin, const Vector4 & nmax);

    /* --------------------------------------------------------------------------------------------
     * Set the values extracted from the specified string using the specified delimiter.
    */
    void Set(CSStr values, SQChar delim);

    /* --------------------------------------------------------------------------------------------
     * Clear the component values to default.
    */
    void Clear()
    {
        min.Clear();
        max.Clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a new instance of this type with absolute component values.
    */
    AABB Abs() const;
};

} // Namespace:: SqMod

#endif // _BASE_AABB_HPP_
