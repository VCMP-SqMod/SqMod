#ifndef _BASE_QUATERNION_HPP_
#define _BASE_QUATERNION_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Quaternion class for representing rotations.
*/
struct Quaternion
{
    /* --------------------------------------------------------------------------------------------
     * The type of value used by components of type.
    */
    typedef float Value;

    /* --------------------------------------------------------------------------------------------
     * Helper instances for common values mostly used as return types or comparison.
    */
    static const Quaternion NIL;
    static const Quaternion MIN;
    static const Quaternion MAX;
    static const Quaternion IDENTITY;

    /* --------------------------------------------------------------------------------------------
     * The delimiter character to be used when extracting values from strings.
    */
    static SQChar Delim;

    /* --------------------------------------------------------------------------------------------
     * The x, y, z and w components of this type.
    */
    Value x, y, z, w;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Quaternion();

    /* --------------------------------------------------------------------------------------------
     * Construct from scalar value.
    */
    explicit Quaternion(Value sv);

    /* --------------------------------------------------------------------------------------------
     * Construct from Euler angles (in degrees.)
    */
    Quaternion(Value xv, Value yv, Value zv);

    /* --------------------------------------------------------------------------------------------
     * Construct from individual values.
    */
    Quaternion(Value xv, Value yv, Value zv, Value wv);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Quaternion(const Quaternion & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Quaternion(Quaternion && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Quaternion() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Quaternion & operator = (const Quaternion & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Quaternion & operator = (Quaternion && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Scalar value assignment operator.
    */
    Quaternion & operator = (Value s);

    /* --------------------------------------------------------------------------------------------
     * Euler assignment operator.
    */
    Quaternion & operator = (const Vector3 & q);

    /* --------------------------------------------------------------------------------------------
     * Four-dimensional vector assignment operator threated as a three-dimensional vector.
    */
    Quaternion & operator = (const Vector4 & q);

    /* --------------------------------------------------------------------------------------------
     * Addition assignment operator.
    */
    Quaternion & operator += (const Quaternion & q);

    /* --------------------------------------------------------------------------------------------
     * Subtraction assignment operator.
    */
    Quaternion & operator -= (const Quaternion & q);

    /* --------------------------------------------------------------------------------------------
     * Multiplication assignment operator.
    */
    Quaternion & operator *= (const Quaternion & q);

    /* --------------------------------------------------------------------------------------------
     * Division assignment operator.
    */
    Quaternion & operator /= (const Quaternion & q);

    /* --------------------------------------------------------------------------------------------
     * Modulo assignment operator.
    */
    Quaternion & operator %= (const Quaternion & q);

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition assignment operator.
    */
    Quaternion & operator += (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction assignment operator.
    */
    Quaternion & operator -= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication assignment operator.
    */
    Quaternion & operator *= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value division assignment operator.
    */
    Quaternion & operator /= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo assignment operator.
    */
    Quaternion & operator %= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Pre-increment operator.
    */
    Quaternion & operator ++ ();

    /* --------------------------------------------------------------------------------------------
     * Pre-decrement operator.
    */
    Quaternion & operator -- ();

    /* --------------------------------------------------------------------------------------------
     * Post-increment operator.
    */
    Quaternion operator ++ (int);

    /* --------------------------------------------------------------------------------------------
     * Post-decrement operator.
    */
    Quaternion operator -- (int);

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    Quaternion operator + (const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    Quaternion operator - (const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    Quaternion operator * (const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    Quaternion operator / (const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Modulo operator.
    */
    Quaternion operator % (const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition operator.
    */
    Quaternion operator + (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction operator.
    */
    Quaternion operator - (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication operator.
    */
    Quaternion operator * (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value division operator.
    */
    Quaternion operator / (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo operator.
    */
    Quaternion operator % (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Unary plus operator.
    */
    Quaternion operator + () const;

    /* --------------------------------------------------------------------------------------------
     * Unary minus operator.
    */
    Quaternion operator - () const;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Quaternion & q) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQFloat s) const
    {
        return Cmp(Quaternion(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQInteger s) const
    {
        return Cmp(Quaternion(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(bool s) const
    {
        return Cmp(Quaternion(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(std::nullptr_t) const
    {
        return Cmp(Quaternion(static_cast< Value >(0)));
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
    void SetQuaternion(const Quaternion & q);

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified values.
    */
    void SetQuaternionEx(Value nx, Value ny, Value nz, Value nw);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from a three-dimensional vector as euler rotation.
    */
    void SetVector3(const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from a three-dimensional vector as euler rotation.
    */
    void SetVector3Ex(Value nx, Value ny, Value nz);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from a four-dimensional vector.
    */
    void SetVector4(const Vector4 & v);

    /* --------------------------------------------------------------------------------------------
     * Set the values extracted from the specified string using the specified delimiter.
    */
    void SetStr(CSStr values, SQChar delim);

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
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value wmin, Value wmax);

    /* --------------------------------------------------------------------------------------------
     * Clear the component values to default.
    */
    void Clear()
    {
        x = 0.0, y = 0.0, z = 0.0, w = 0.0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a new instance of this type with absolute component values.
    */
    Quaternion Abs() const;

    /* --------------------------------------------------------------------------------------------
     * Return whether is NaN.
    */
    bool IsNaN() const;

    /* --------------------------------------------------------------------------------------------
     * Return squared length.
    */
    Value LengthSquared() const;

    /* --------------------------------------------------------------------------------------------
     * Calculate dot product.
    */
    Value DotProduct(const Quaternion & quat) const;

    /* --------------------------------------------------------------------------------------------
     * Return conjugate.
    */
    Quaternion Conjugate() const;

    /* --------------------------------------------------------------------------------------------
     * Normalize to unit length.
    */
    void Normalize();

    /* --------------------------------------------------------------------------------------------
     * Return normalized to unit length.
    */
    Quaternion Normalized() const;

    /* --------------------------------------------------------------------------------------------
     * Define from an angle (in degrees) and axis.
    */
    void FromAngleAxis(Value angle, const Vector3 & axis);

    /* --------------------------------------------------------------------------------------------
     * Define from the rotation difference between two direction vectors.
    */
    void FromRotationTo(const Vector3 & start, const Vector3 & end);

    /* --------------------------------------------------------------------------------------------
     * Return inverse.
    */
    Quaternion Inverse() const;

    /* --------------------------------------------------------------------------------------------
     * Return Euler angles in degrees.
    */
    Vector3 ToEuler() const;

    /* --------------------------------------------------------------------------------------------
     * Return yaw angle in degrees.
    */
    Value YawAngle() const;

    /* --------------------------------------------------------------------------------------------
     * Return pitch angle in degrees.
    */
    Value PitchAngle() const;

    /* --------------------------------------------------------------------------------------------
     * Return roll angle in degrees.
    */
    Value RollAngle() const;

    /* --------------------------------------------------------------------------------------------
     * Spherical interpolation with another quaternion.
    */
    Quaternion Slerp(Quaternion quat, Value t) const;

    /* --------------------------------------------------------------------------------------------
     * Normalized linear interpolation with another quaternion.
    */
    Quaternion Nlerp(const Quaternion & quat, Value t) const;

    /* --------------------------------------------------------------------------------------------
     * Normalized linear interpolation with another quaternion.
    */
    Quaternion NlerpEx(const Quaternion & quat, Value t, bool shortest_path) const;

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Quaternion type from a string.
    */
    static const Quaternion & Get(CSStr str);

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Quaternion type from a string.
    */
    static const Quaternion & Get(CSStr str, SQChar delim);

};

} // Namespace:: SqMod

#endif // _BASE_QUATERNION_HPP_
