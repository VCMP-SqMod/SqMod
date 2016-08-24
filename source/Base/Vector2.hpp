#ifndef _BASE_VECTOR2_HPP_
#define _BASE_VECTOR2_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class used to represent a two-dimensional vector.
*/
struct Vector2
{
    /* --------------------------------------------------------------------------------------------
     * The type of value used by components of type.
    */
    typedef float Value;

    /* --------------------------------------------------------------------------------------------
     * Helper instances for common values mostly used as return types or comparison.
    */
    static const Vector2 NIL;
    static const Vector2 MIN;
    static const Vector2 MAX;

    /* --------------------------------------------------------------------------------------------
     * The delimiter character to be used when extracting values from strings.
    */
    static SQChar Delim;

    /* --------------------------------------------------------------------------------------------
     * The x and y components of this type.
    */
    Value x, y;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Vector2();

    /* --------------------------------------------------------------------------------------------
     * Construct a vector with the same scalar value for all components.
    */
    explicit Vector2(Value sv);

    /* --------------------------------------------------------------------------------------------
     * Construct a vector with the specified component values.
    */
    Vector2(Value xv, Value yv);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Vector2(const Vector2 & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Vector2(Vector2 && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Vector2() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Vector2 & operator = (const Vector2 & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Vector2 & operator = (Vector2 && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Scalar value assignment operator.
    */
    Vector2 & operator = (Value s);

    /* --------------------------------------------------------------------------------------------
     * String assignment operator.
    */
    Vector2 & operator = (CSStr values);

    /* --------------------------------------------------------------------------------------------
     * Integral two-dimensional vector assignment.
    */
    Vector2 & operator = (const Vector2i & v);

    /* --------------------------------------------------------------------------------------------
     * Addition assignment operator.
    */
    Vector2 & operator += (const Vector2 & v);

    /* --------------------------------------------------------------------------------------------
     * Subtraction assignment operator.
    */
    Vector2 & operator -= (const Vector2 & v);

    /* --------------------------------------------------------------------------------------------
     * Multiplication assignment operator.
    */
    Vector2 & operator *= (const Vector2 & v);

    /* --------------------------------------------------------------------------------------------
     * Division assignment operator.
    */
    Vector2 & operator /= (const Vector2 & v);

    /* --------------------------------------------------------------------------------------------
     * Modulo assignment operator.
    */
    Vector2 & operator %= (const Vector2 & v);

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition assignment operator.
    */
    Vector2 & operator += (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction assignment operator.
    */
    Vector2 & operator -= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication assignment operator.
    */
    Vector2 & operator *= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value division assignment operator.
    */
    Vector2 & operator /= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo assignment operator.
    */
    Vector2 & operator %= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Pre-increment operator.
    */
    Vector2 & operator ++ ();

    /* --------------------------------------------------------------------------------------------
     * Pre-decrement operator.
    */
    Vector2 & operator -- ();

    /* --------------------------------------------------------------------------------------------
     * Post-increment operator.
    */
    Vector2 operator ++ (int);

    /* --------------------------------------------------------------------------------------------
     * Post-decrement operator.
    */
    Vector2 operator -- (int);

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    Vector2 operator + (const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    Vector2 operator - (const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    Vector2 operator * (const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    Vector2 operator / (const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Modulo operator.
    */
    Vector2 operator % (const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition operator.
    */
    Vector2 operator + (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction operator.
    */
    Vector2 operator - (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication operator.
    */
    Vector2 operator * (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value division operator.
    */
    Vector2 operator / (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo operator.
    */
    Vector2 operator % (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Unary plus operator.
    */
    Vector2 operator + () const;

    /* --------------------------------------------------------------------------------------------
     * Unary minus operator.
    */
    Vector2 operator - () const;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Vector2 & v) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQFloat s) const
    {
        return Cmp(Vector2(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQInteger s) const
    {
        return Cmp(Vector2(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(bool s) const
    {
        return Cmp(Vector2(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(std::nullptr_t) const
    {
        return Cmp(Vector2(static_cast< Value >(0)));
    }

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
    void SetVector2(const Vector2 & v);

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified values.
    */
    void SetVector2Ex(Value nx, Value ny);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from an integral two-dimensional vector.
    */
    void SetVector2i(const Vector2i & v);

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
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax);

    /* --------------------------------------------------------------------------------------------
     * Clear the component values to default.
    */
    void Clear()
    {
        x = 0.0, y = 0.0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a new instance of this type with absolute component values.
    */
    Vector2 Abs() const;

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Vector2 type from a string.
    */
    static const Vector2 & Get(CSStr str);

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Vector2 type from a string.
    */
    static const Vector2 & Get(CSStr str, SQChar delim);

};

} // Namespace:: SqMod

#endif // _BASE_VECTOR2_HPP_
