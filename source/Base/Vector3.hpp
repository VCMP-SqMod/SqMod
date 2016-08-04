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
    Vector3(Value sv);

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
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

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
     * Return the distance between this vector and another vector.
    */
    Value GetDistanceTo(const Vector3 & vec) const;


    /* --------------------------------------------------------------------------------------------
     * Return the squared distance between this vector and another vector.
    */
    Value GetSquaredDistanceTo(const Vector3 & vec) const;

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Vector3 type from a string.
    */
    static const Vector3 & Get(CSStr str);

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Vector3 type from a string.
    */
    static const Vector3 & Get(CSStr str, SQChar delim);

};

} // Namespace:: SqMod

#endif // _BASE_VECTOR3_HPP_
