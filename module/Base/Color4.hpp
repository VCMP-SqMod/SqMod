#pragma once

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class used to represent a transparent RGBA color.
*/
struct Color4
{
    /* --------------------------------------------------------------------------------------------
     * The type of value used by components of type.
    */
    typedef unsigned char Value;

    /* --------------------------------------------------------------------------------------------
     * Helper instances for common values mostly used as return types or comparison.
    */
    static const Color4 NIL;
    static const Color4 MIN;
    static const Color4 MAX;

    /* --------------------------------------------------------------------------------------------
     * The delimiter character to be used when extracting values from strings.
    */
    static SQChar Delim;

    /* --------------------------------------------------------------------------------------------
     * The red, green and blue components of this type.
    */
    Value r, g, b, a;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Color4() noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct a color with all components with the same specified color.
    */
    explicit Color4(Value sv) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct with individually specified red, green and blue colors.
    */
    Color4(Value rv, Value gv, Value bv) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct with individually specified red, green, blue and alpha colors.
    */
    Color4(Value rv, Value gv, Value bv, Value av) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Color4(const Color4 & o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Color4(Color4 && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Color4() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Color4 & operator = (const Color4 & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Color4 & operator = (Color4 && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Scalar value assignment operator.
    */
    Color4 & operator = (Value s);

    /* --------------------------------------------------------------------------------------------
     * Opaque color assignment operator.
    */
    Color4 & operator = (const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Addition assignment operator.
    */
    Color4 & operator += (const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Subtraction assignment operator.
    */
    Color4 & operator -= (const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Multiplication assignment operator.
    */
    Color4 & operator *= (const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Division assignment operator.
    */
    Color4 & operator /= (const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Modulo assignment operator.
    */
    Color4 & operator %= (const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Bitwise AND assignment operator.
    */
    Color4 & operator &= (const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Bitwise OR assignment operator.
    */
    Color4 & operator |= (const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Bitwise XOR assignment operator.
    */
    Color4 & operator ^= (const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Bitwise left shift assignment operator.
    */
    Color4 & operator <<= (const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Bitwise right shift assignment operator.
    */
    Color4 & operator >>= (const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition assignment operator.
    */
    Color4 & operator += (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction assignment operator.
    */
    Color4 & operator -= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication assignment operator.
    */
    Color4 & operator *= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value division assignment operator.
    */
    Color4 & operator /= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo assignment operator.
    */
    Color4 & operator %= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise AND assignment operator.
    */
    Color4 & operator &= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise OR assignment operator.
    */
    Color4 & operator |= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise XOR assignment operator.
    */
    Color4 & operator ^= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise left shift assignment operator.
    */
    Color4 & operator <<= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise right shift assignment operator.
    */
    Color4 & operator >>= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Pre-increment operator.
    */
    Color4 & operator ++ ();

    /* --------------------------------------------------------------------------------------------
     * Pre-decrement operator.
    */
    Color4 & operator -- ();

    /* --------------------------------------------------------------------------------------------
     * Post-increment operator.
    */
    Color4 operator ++ (int); // NOLINT(cert-dcl21-cpp)

    /* --------------------------------------------------------------------------------------------
     * Post-decrement operator.
    */
    Color4 operator -- (int); // NOLINT(cert-dcl21-cpp)

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    Color4 operator + (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    Color4 operator - (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    Color4 operator * (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    Color4 operator / (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Modulo operator.
    */
    Color4 operator % (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise AND operator.
    */
    Color4 operator & (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise OR operator.
    */
    Color4 operator | (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise XOR operator.
    */
    Color4 operator ^ (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise shift left operator.
    */
    Color4 operator << (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise shift right operator.
    */
    Color4 operator >> (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition operator.
    */
    Color4 operator + (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction operator.
    */
    Color4 operator - (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication operator.
    */
    Color4 operator * (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value division operator.
    */
    Color4 operator / (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo operator.
    */
    Color4 operator % (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise AND operator.
    */
    Color4 operator & (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise OR operator.
    */
    Color4 operator | (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise XOR operator.
    */
    Color4 operator ^ (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise shift left operator.
    */
    Color4 operator << (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise shift right operator.
    */
    Color4 operator >> (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Unary plus operator.
    */
    Color4 operator + () const;

    /* --------------------------------------------------------------------------------------------
     * Unary minus operator.
    */
    Color4 operator - () const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise NOT operator.
    */
    Color4 operator ~ () const;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to opaque color.
    */
    operator Color3 () const; // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Color4 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQInteger s) const
    {
        return Cmp(Color4(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQFloat s) const
    {
        return Cmp(Color4(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(bool s) const
    {
        return Cmp(Color4(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(std::nullptr_t) const
    {
        return Cmp(Color4(static_cast< Value >(0)));
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
     * Copy the values from an opaque color.
    */
    void SetColor3(const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified values.
    */
    void SetColor3Ex(Value nr, Value ng, Value nb);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from another instance of this type.
    */
    void SetColor4(const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified values.
    */
    void SetColor4Ex(Value nr, Value ng, Value nb, Value na);

    /* --------------------------------------------------------------------------------------------
     * Set the values extracted from the specified string using the specified delimiter.
    */
    void SetStr(SQChar delim, StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Set the values from the identified color.
    */
    void SetName(StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Get the component values packed inside an integer value.
    */
    Uint32 GetRGB() const;

    /* --------------------------------------------------------------------------------------------
     * Set the component values wxtracted from an integer value.
    */
    void SetRGB(Uint32 p);

    /* --------------------------------------------------------------------------------------------
     * Get the component values packed inside an integer value.
    */
    Uint32 GetRGBA() const;

    /* --------------------------------------------------------------------------------------------
     * Set the component values wxtracted from an integer value.
    */
    void SetRGBA(Uint32 p);

    /* --------------------------------------------------------------------------------------------
     * Get the component values packed inside an integer value.
    */
    Uint32 GetARGB() const;

    /* --------------------------------------------------------------------------------------------
     * Set the component values wxtracted from an integer value.
    */
    void SetARGB(Uint32 p);

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
    void Generate(Value rmin, Value rmax, Value gmin, Value gmax, Value bmin, Value bmax, Value amin, Value amax);

    /* --------------------------------------------------------------------------------------------
     * Clear the component values to default.
    */
    void Clear()
    {
        r = 0, g = 0, b = 0, a = 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Set the component values to a randomly chosen color.
    */
    void Random();

    /* --------------------------------------------------------------------------------------------
     * Inverse the color.
    */
    void Inverse();

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Color4 type from a string.
    */
    static const Color4 & Get(StackStrF & str);

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Color4 type from a string.
    */
    static const Color4 & GetEx(SQChar delim, StackStrF & str);
};

} // Namespace:: SqMod
