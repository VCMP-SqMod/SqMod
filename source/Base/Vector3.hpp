#ifndef _BASE_VECTOR3_HPP_
#define _BASE_VECTOR3_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class used to represent a three-dimensional vector.
*/
struct Vector3
{
    /* --------------------------------------------------------------------------------------------
     * The type of value used by components of type.
    */
    typedef float Value;

    /* --------------------------------------------------------------------------------------------
     * Helper instances for common values mostly used as return types or comparison.
    */
    static const Vector3 NIL;       // ( 0,  0,  0)
    static const Vector3 MIN;       // (<0, <0, <0)
    static const Vector3 MAX;       // (>0, >0, >0)
    static const Vector3 LEFT;      // (-1,  0,  0)
    static const Vector3 RIGHT;     // ( 1,  0,  0)
    static const Vector3 UP;        // ( 0,  1,  0)
    static const Vector3 DOWN;      // ( 0, -1,  0)
    static const Vector3 FORWARD;   // ( 0,  0,  1)
    static const Vector3 BACK;      // ( 0,  0, -1)
    static const Vector3 ONE;       // ( 1,  1,  1)

    /* --------------------------------------------------------------------------------------------
     * The delimiter character to be used when extracting values from strings.
    */
    static SQChar Delim;

    /* --------------------------------------------------------------------------------------------
     * The x, y and z components of this type.
    */
    Value x, y, z;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Vector3();

    /* --------------------------------------------------------------------------------------------
     * Construct a vector with the same scalar value for all components.
    */
    explicit Vector3(Value sv);

    /* --------------------------------------------------------------------------------------------
     * Construct a vector with the specified component values.
    */
    Vector3(Value xv, Value yv, Value zv);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Vector3(const Vector3 & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Vector3(Vector3 && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Vector3() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Vector3 & operator = (const Vector3 & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Vector3 & operator = (Vector3 && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Scalar value assignment operator.
    */
    Vector3 & operator = (Value s);

    /* --------------------------------------------------------------------------------------------
     * Four-dimensional vector assignment.
    */
    Vector3 & operator = (const Vector4 & v);

    /* --------------------------------------------------------------------------------------------
     * Quaternion rotation assignment.
    */
    Vector3 & operator = (const Quaternion & q);

    /* --------------------------------------------------------------------------------------------
     * Addition assignment operator.
    */
    Vector3 & operator += (const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Subtraction assignment operator.
    */
    Vector3 & operator -= (const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Multiplication assignment operator.
    */
    Vector3 & operator *= (const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Division assignment operator.
    */
    Vector3 & operator /= (const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Modulo assignment operator.
    */
    Vector3 & operator %= (const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition assignment operator.
    */
    Vector3 & operator += (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction assignment operator.
    */
    Vector3 & operator -= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication assignment operator.
    */
    Vector3 & operator *= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value division assignment operator.
    */
    Vector3 & operator /= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo assignment operator.
    */
    Vector3 & operator %= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Pre-increment operator.
    */
    Vector3 & operator ++ ();

    /* --------------------------------------------------------------------------------------------
     * Pre-decrement operator.
    */
    Vector3 & operator -- ();

    /* --------------------------------------------------------------------------------------------
     * Post-increment operator.
    */
    Vector3 operator ++ (int);

    /* --------------------------------------------------------------------------------------------
     * Post-decrement operator.
    */
    Vector3 operator -- (int);

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    Vector3 operator + (const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    Vector3 operator - (const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    Vector3 operator * (const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    Vector3 operator / (const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Modulo operator.
    */
    Vector3 operator % (const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition operator.
    */
    Vector3 operator + (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction operator.
    */
    Vector3 operator - (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication operator.
    */
    Vector3 operator * (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value division operator.
    */
    Vector3 operator / (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo operator.
    */
    Vector3 operator % (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Unary plus operator.
    */
    Vector3 operator + () const;

    /* --------------------------------------------------------------------------------------------
     * Unary minus operator.
    */
    Vector3 operator - () const;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Vector3 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQFloat s) const
    {
        return Cmp(Vector3(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQInteger s) const
    {
        return Cmp(Vector3(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(bool s) const
    {
        return Cmp(Vector3(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(std::nullptr_t) const
    {
        return Cmp(static_cast< Value >(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified scalar value.
    */
    void SetScalar(Value ns);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from another instance of this type.
    */
    void SetVector3(const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified values.
    */
    void SetVector3Ex(Value nx, Value ny, Value nz);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from a four-dimensional vector.
    */
    void SetVector4(const Vector4 & v);

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified values.
    */
    void SetVector4Ex(Value nx, Value ny, Value nz, Value nw);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from a quaternion rotation.
    */
    void SetQuaternion(const Quaternion & q);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from a quaternion rotation.
    */
    void SetQuaternionEx(Value nx, Value ny, Value nz, Value nw);

    /* --------------------------------------------------------------------------------------------
     * Set the values extracted from the specified string using the specified delimiter.
    */
    void SetStr(SQChar delim, const StackStrF & values);

    /* --------------------------------------------------------------------------------------------
     * Generate random values for all components of this instance.
    */
    void Generate();

    /* --------------------------------------------------------------------------------------------
     * Generate random values for all components of this instance within the specified bounds.
    */
    void Generate(Value min, Value max);

    /* --------------------------------------------------------------------------------------------
     * Generate random values for all components of this instance within the specified bounds.
    */
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax);

    /* --------------------------------------------------------------------------------------------
     * Clear the component values to default.
    */
    void Clear()
    {
        x = 0.0, y = 0.0, z = 0.0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a new instance of this type with absolute component values.
    */
    Vector3 Abs() const;

    /* --------------------------------------------------------------------------------------------
     * Return whether is NaN.
    */
    bool IsNaN() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the length.
    */
    Value GetLength() const;

    /* --------------------------------------------------------------------------------------------
     * Assign the length.
    */
    void SetLength(Value length);

    /* --------------------------------------------------------------------------------------------
     * Return the squared length.
    */
    Value GetLengthSquared() const;

    /* --------------------------------------------------------------------------------------------
     * Assign the squared length.
    */
    void SetLengthSquared(Value length);

    /* --------------------------------------------------------------------------------------------
     * Return normalized to unit length.
    */
    Vector3 Normalized() const;

    /* --------------------------------------------------------------------------------------------
     * Normalize to unit length.
    */
    void Normalize();

    /* --------------------------------------------------------------------------------------------
     * Calculate dot product.
    */
    Value DotProduct(const Vector3 & vec) const;

    /* --------------------------------------------------------------------------------------------
     * Calculate absolute dot product.
    */
    Value AbsDotProduct(const Vector3 & vec) const;

    /* --------------------------------------------------------------------------------------------
     * Calculate cross product.
    */
    Vector3 CrossProduct(const Vector3 & vec) const;

    /* --------------------------------------------------------------------------------------------
     * Returns the angle between this vector and another vector in degrees.
    */
    Value Angle(const Vector3 & vec) const;

    /* --------------------------------------------------------------------------------------------
     * Return the distance between this vector and another vector.
    */
    Value GetDistanceTo(const Vector3 & vec) const;

    /* --------------------------------------------------------------------------------------------
     * Return the squared distance between this vector and another vector.
    */
    Value GetSquaredDistanceTo(const Vector3 & vec) const;

    /* --------------------------------------------------------------------------------------------
     * Linear interpolation with another vector.
    */
    bool IsBetweenPoints(const Vector3 & begin, const Vector3 & end) const;

    /* --------------------------------------------------------------------------------------------
     * Sets this vector to the linearly interpolated vector between a and b.
    */
    void Interpolate(const Vector3 & a, const Vector3 & b, Value d);

    /* --------------------------------------------------------------------------------------------
     * Sets this vector to the linearly interpolated vector between a and b.
    */
    Vector3 Interpolated(const Vector3 & vec, Value d) const;

    /* --------------------------------------------------------------------------------------------
     * Rotates the vector by a specified number of degrees around the Y axis and the specified center.
    */
    Vector3 Rotated(const Vector3 & axis, Value angle) const;

    /* --------------------------------------------------------------------------------------------
     * Rotates the vector by a specified number of degrees around the Y axis and the specified center.
    */
    void RotateXZBy(Value degrees)
    {
        CenterRotateXZBy(degrees, NIL);
    }

    /* --------------------------------------------------------------------------------------------
     * Rotates the vector by a specified number of degrees around the Y axis and the specified center.
    */
    void CenterRotateXZBy(Value degrees, const Vector3 & center);

    /* --------------------------------------------------------------------------------------------
     * Rotates the vector by a specified number of degrees around the Z axis and the specified center.
    */
    void RotateXYBy(Value degrees)
    {
        CenterRotateXYBy(degrees, NIL);
    }

    /* --------------------------------------------------------------------------------------------
     * Rotates the vector by a specified number of degrees around the Z axis and the specified center.
    */
    void CenterRotateXYBy(Value degrees, const Vector3 & center);

    /* --------------------------------------------------------------------------------------------
     * Rotates the vector by a specified number of degrees around the X axis and the specified center.
    */
    void RotateYZBy(Value degrees)
    {
        CenterRotateYZBy(degrees, NIL);
    }

    /* --------------------------------------------------------------------------------------------
     * Rotates the vector by a specified number of degrees around the X axis and the specified center.
    */
    void CenterRotateYZBy(Value degrees, const Vector3 & center);

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Vector3 type from a string.
    */
    static const Vector3 & Get(const StackStrF & str);

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Vector3 type from a string.
    */
    static const Vector3 & GetEx(SQChar delim, const StackStrF & str);

};

} // Namespace:: SqMod

#endif // _BASE_VECTOR3_HPP_
