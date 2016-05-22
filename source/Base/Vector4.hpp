#ifndef _BASE_VECTOR4_HPP_
#define _BASE_VECTOR4_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class used to represent a four-dimensional vector.
*/
struct Vector4
{
    /* --------------------------------------------------------------------------------------------
     * The type of value used by components of type.
    */
    typedef float Value;

    /* --------------------------------------------------------------------------------------------
     * Helper instances for common values mostly used as return types or comparison.
    */
    static const Vector4 NIL;
    static const Vector4 MIN;
    static const Vector4 MAX;

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
    Vector4();

    /* --------------------------------------------------------------------------------------------
     * Construct a vector with the same scalar value for all components.
    */
    Vector4(Value sv);

    /* --------------------------------------------------------------------------------------------
     * Construct a vector with the specified component values.
    */
    Vector4(Value xv, Value yv, Value zv);

    /* --------------------------------------------------------------------------------------------
     * Construct a vector with the specified component values.
    */
    Vector4(Value xv, Value yv, Value zv, Value wv);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Vector4(const Vector4 & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Vector4(Vector4 && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Vector4() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Vector4 & operator = (const Vector4 & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Vector4 & operator = (Vector4 && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Scalar value assignment operator.
    */
    Vector4 & operator = (Value s);

    /* --------------------------------------------------------------------------------------------
     * Three-dimensional vector assignment operator.
    */
    Vector4 & operator = (const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Quaternion rotation assignment operator.
    */
    Vector4 & operator = (const Quaternion & q);

    /* --------------------------------------------------------------------------------------------
     * Addition assignment operator.
    */
    Vector4 & operator += (const Vector4 & v);

    /* --------------------------------------------------------------------------------------------
     * Subtraction assignment operator.
    */
    Vector4 & operator -= (const Vector4 & v);

    /* --------------------------------------------------------------------------------------------
     * Multiplication assignment operator.
    */
    Vector4 & operator *= (const Vector4 & v);

    /* --------------------------------------------------------------------------------------------
     * Division assignment operator.
    */
    Vector4 & operator /= (const Vector4 & v);

    /* --------------------------------------------------------------------------------------------
     * Modulo assignment operator.
    */
    Vector4 & operator %= (const Vector4 & v);

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition assignment operator.
    */
    Vector4 & operator += (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction assignment operator.
    */
    Vector4 & operator -= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication assignment operator.
    */
    Vector4 & operator *= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value division assignment operator.
    */
    Vector4 & operator /= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo assignment operator.
    */
    Vector4 & operator %= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Pre-increment operator.
    */
    Vector4 & operator ++ ();

    /* --------------------------------------------------------------------------------------------
     * Pre-decrement operator.
    */
    Vector4 & operator -- ();

    /* --------------------------------------------------------------------------------------------
     * Post-increment operator.
    */
    Vector4 operator ++ (int);

    /* --------------------------------------------------------------------------------------------
     * Post-decrement operator.
    */
    Vector4 operator -- (int); 

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    Vector4 operator + (const Vector4 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    Vector4 operator - (const Vector4 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    Vector4 operator * (const Vector4 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    Vector4 operator / (const Vector4 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Modulo operator.
    */
    Vector4 operator % (const Vector4 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition operator.
    */
    Vector4 operator + (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction operator.
    */
    Vector4 operator - (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication operator.
    */
    Vector4 operator * (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value division operator.
    */
    Vector4 operator / (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo operator.
    */
    Vector4 operator % (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Unary plus operator.
    */
    Vector4 operator + () const;

    /* --------------------------------------------------------------------------------------------
     * Unary minus operator.
    */
    Vector4 operator - () const;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Vector4 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Vector4 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Vector4 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Vector4 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Vector4 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Vector4 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Vector4 & v) const;

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
    void Set(Value ns);

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified values.
    */
    void Set(Value nx, Value ny, Value nz);

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified values.
    */
    void Set(Value nx, Value ny, Value nz, Value nw);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from another instance of this type.
    */
    void Set(const Vector4 & v);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from a three-dimensional vector.
    */
    void Set(const Vector3 & v);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from a quaternion rotation.
    */
    void Set(const Quaternion & q);

    /* --------------------------------------------------------------------------------------------
     * Set the values extracted from the specified string using the specified delimiter.
    */
    void Set(CSStr values, SQChar delim);

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
    Vector4 Abs() const;

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Vector4 type from a string.
    */
    static const Vector4 & Get(CSStr str);

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Vector4 type from a string.
    */
    static const Vector4 & Get(CSStr str, SQChar delim);

};

} // Namespace:: SqMod

#endif // _BASE_VECTOR4_HPP_