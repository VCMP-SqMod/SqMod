#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"
#include "Base/Vector2.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class used to represent a two-dimensional circle.
*/
struct Circle
{
    /* --------------------------------------------------------------------------------------------
     * The type of value used by components of type.
    */
    typedef float Value;

    /* --------------------------------------------------------------------------------------------
     * Helper instances for common values mostly used as return types or comparison.
    */
    static const Circle NIL;
    static const Circle MIN;
    static const Circle MAX;

    /* --------------------------------------------------------------------------------------------
     * The delimiter character to be used when extracting values from strings.
    */
    static SQChar Delim;

    /* --------------------------------------------------------------------------------------------
     * The position and radius components of this type.
    */
    Vector2     pos;
    Value       rad;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Circle() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct a circle at position 0,0 using the specified radius.
    */
    Circle(Value rv) noexcept; // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    /* --------------------------------------------------------------------------------------------
     * Construct a circle at the specified position using the specified radius.
    */
    Circle(const Vector2 & pv, Value rv) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct a circle at the specified position using the specified radius.
    */
    Circle(Value xv, Value yv, Value rv) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Circle(const Circle & o)  noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Circle(Circle && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Circle() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Circle & operator = (const Circle & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Circle & operator = (Circle && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Radius assignment operator.
    */
    Circle & operator = (Value r);

    /* --------------------------------------------------------------------------------------------
     * Position assignment operator.
    */
    Circle & operator = (const Vector2 & p);

    /* --------------------------------------------------------------------------------------------
     * Addition assignment operator.
    */
    Circle & operator += (const Circle & c);

    /* --------------------------------------------------------------------------------------------
     * Subtraction assignment operator.
    */
    Circle & operator -= (const Circle & c);

    /* --------------------------------------------------------------------------------------------
     * Multiplication assignment operator.
    */
    Circle & operator *= (const Circle & c);

    /* --------------------------------------------------------------------------------------------
     * Division assignment operator.
    */
    Circle & operator /= (const Circle & c);

    /* --------------------------------------------------------------------------------------------
     * Modulo assignment operator.
    */
    Circle & operator %= (const Circle & c);

    /* --------------------------------------------------------------------------------------------
     * Radius addition assignment operator.
    */
    Circle & operator += (Value r);

    /* --------------------------------------------------------------------------------------------
     * Radius subtraction assignment operator.
    */
    Circle & operator -= (Value r);

    /* --------------------------------------------------------------------------------------------
     * Radius multiplication assignment operator.
    */
    Circle & operator *= (Value r);

    /* --------------------------------------------------------------------------------------------
     * Radius division assignment operator.
    */
    Circle & operator /= (Value r);

    /* --------------------------------------------------------------------------------------------
     * Radius modulo assignment operator.
    */
    Circle & operator %= (Value r);

    /* --------------------------------------------------------------------------------------------
     * Position addition assignment operator.
    */
    Circle & operator += (const Vector2 & p);

    /* --------------------------------------------------------------------------------------------
     * Position subtraction assignment operator.
    */
    Circle & operator -= (const Vector2 & p);

    /* --------------------------------------------------------------------------------------------
     * Position multiplication assignment operator.
    */
    Circle & operator *= (const Vector2 & p);

    /* --------------------------------------------------------------------------------------------
     * Position division assignment operator.
    */
    Circle & operator /= (const Vector2 & p);

    /* --------------------------------------------------------------------------------------------
     * Position modulo assignment operator.
    */
    Circle & operator %= (const Vector2 & p);

    /* --------------------------------------------------------------------------------------------
     * Pre-increment operator.
    */
    Circle & operator ++ ();

    /* --------------------------------------------------------------------------------------------
     * Pre-decrement operator.
    */
    Circle & operator -- ();

    /* --------------------------------------------------------------------------------------------
     * Post-increment operator.
    */
    Circle operator ++ (int); // NOLINT(cert-dcl21-cpp)

    /* --------------------------------------------------------------------------------------------
     * Post-decrement operator.
    */
    Circle operator -- (int); // NOLINT(cert-dcl21-cpp)

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    Circle operator + (const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    Circle operator - (const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    Circle operator * (const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    Circle operator / (const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Modulo operator.
    */
    Circle operator % (const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Radius addition operator.
    */
    Circle operator + (Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Radius subtraction operator.
    */
    Circle operator - (Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Radius multiplication operator.
    */
    Circle operator * (Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Radius division operator.
    */
    Circle operator / (Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Radius modulo operator.
    */
    Circle operator % (Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Position addition operator.
    */
    Circle operator + (const Vector2 & p) const;

    /* --------------------------------------------------------------------------------------------
     * Position subtraction operator.
    */
    Circle operator - (const Vector2 & p) const;

    /* --------------------------------------------------------------------------------------------
     * Position multiplication operator.
    */
    Circle operator * (const Vector2 & p) const;

    /* --------------------------------------------------------------------------------------------
     * Position division operator.
    */
    Circle operator / (const Vector2 & p) const;

    /* --------------------------------------------------------------------------------------------
     * Position modulo operator.
    */
    Circle operator % (const Vector2 & p) const;

    /* --------------------------------------------------------------------------------------------
     * Unary plus operator.
    */
    Circle operator + () const;

    /* --------------------------------------------------------------------------------------------
     * Unary minus operator.
    */
    Circle operator - () const;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Circle & c) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQFloat s) const
    {
        return Cmp(Circle(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQInteger s) const
    {
        return Cmp(Circle(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(bool s) const
    {
        return Cmp(Circle(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(std::nullptr_t) const
    {
        return Cmp(Circle(static_cast< Value >(0)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Set the specified radius.
    */
    void SetRadius(Value nr);

    /* --------------------------------------------------------------------------------------------
     * Copy the circle from another instance of this type.
    */
    void SetCircle(const Circle & nc);

    /* --------------------------------------------------------------------------------------------
     * Set the specified position and radius.
    */
    void SetCircleEx(Value nx, Value ny, Value nr);

    /* --------------------------------------------------------------------------------------------
     * Set the specified position and radius.
    */
    void SetValues(const Vector2 & np, Value nr);

    /* --------------------------------------------------------------------------------------------
     * Set the specified position.
    */
    void SetPosition(const Vector2 & np);

    /* --------------------------------------------------------------------------------------------
     * Set the specified position.
    */
    void SetPositionEx(Value nx, Value ny);

    /* --------------------------------------------------------------------------------------------
     * Set the values extracted from the specified string using the specified delimiter.
    */
    void SetStr(SQChar delim, StackStrF & values);

    /* --------------------------------------------------------------------------------------------
     * Generate a randomly sized and positioned circle.
    */
    void Generate();

    /* --------------------------------------------------------------------------------------------
     * Generate a randomly sized or positioned circle within the specified bounds.
    */
    void Generate(Value min, Value max, bool r);

    /* --------------------------------------------------------------------------------------------
     * Generate a randomly positioned circle within the specified bounds.
    */
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax);

    /* --------------------------------------------------------------------------------------------
     * Generate a randomly sized and positioned circle within the specified bounds.
    */
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value rmin, Value rmax);

    /* --------------------------------------------------------------------------------------------
     * Clear the component values to default.
    */
    void Clear()
    {
        pos.Clear();
        rad = 0.0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a new instance of this type with absolute component values.
    */
    Circle Abs() const;

    /* --------------------------------------------------------------------------------------------
     * Transform this into an array of Vector2 points that form a circle.
    */
    Array ToPointsArray(SQInteger num_segments) const;

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Circle type from a string.
    */
    static const Circle & Get(StackStrF & str);

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Circle type from a string.
    */
    static const Circle & GetEx(SQChar delim, StackStrF & str);
};

} // Namespace:: SqMod
