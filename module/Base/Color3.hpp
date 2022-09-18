#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class used to represent an opaque RGB color.
*/
struct Color3
{
    /* --------------------------------------------------------------------------------------------
     * The type of value used by components of type.
    */
    typedef unsigned char Value;

    /* --------------------------------------------------------------------------------------------
     * Helper instances for common values mostly used as return types or comparison.
    */
    static const Color3 NIL;
    static const Color3 MIN;
    static const Color3 MAX;

    /* --------------------------------------------------------------------------------------------
     * The delimiter character to be used when extracting values from strings.
    */
    static SQChar Delim;

    /* --------------------------------------------------------------------------------------------
     * Whenever converting colors to hex, useupper case characters.
    */
    static bool UpperCaseHex;

    /* --------------------------------------------------------------------------------------------
     * The red, green and blue components of this type.
    */
    Value r{0}, g{0}, b{0};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Color3() noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Construct a color with all components with the same specified color.
    */
    explicit Color3(Value sv) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct with individually specified red, green and blue colors.
    */
    Color3(Value rv, Value gv, Value bv) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct with individually specified red, green, blue and alpha colors.
    */
    Color3(Value rv, Value gv, Value bv, Value av) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Color3(const Color3 & o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Color3(Color3 && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Color3() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Color3 & operator = (const Color3 & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Color3 & operator = (Color3 && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Scalar value assignment operator.
    */
    Color3 & operator = (Value s);

    /* --------------------------------------------------------------------------------------------
     * Transparent color assignment operator.
    */
    Color3 & operator = (const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Addition assignment operator.
    */
    Color3 & operator += (const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Subtraction assignment operator.
    */
    Color3 & operator -= (const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Multiplication assignment operator.
    */
    Color3 & operator *= (const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Division assignment operator.
    */
    Color3 & operator /= (const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Modulo assignment operator.
    */
    Color3 & operator %= (const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Bitwise AND assignment operator.
    */
    Color3 & operator &= (const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Bitwise OR assignment operator.
    */
    Color3 & operator |= (const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Bitwise XOR assignment operator.
    */
    Color3 & operator ^= (const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Bitwise left shift assignment operator.
    */
    Color3 & operator <<= (const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Bitwise right shift assignment operator.
    */
    Color3 & operator >>= (const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition assignment operator.
    */
    Color3 & operator += (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction assignment operator.
    */
    Color3 & operator -= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication assignment operator.
    */
    Color3 & operator *= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value division assignment operator.
    */
    Color3 & operator /= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo assignment operator.
    */
    Color3 & operator %= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise AND assignment operator.
    */
    Color3 & operator &= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise OR assignment operator.
    */
    Color3 & operator |= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise XOR assignment operator.
    */
    Color3 & operator ^= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise left shift assignment operator.
    */
    Color3 & operator <<= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise right shift assignment operator.
    */
    Color3 & operator >>= (Value s);

    /* --------------------------------------------------------------------------------------------
     * Pre-increment operator.
    */
    Color3 & operator ++ ();

    /* --------------------------------------------------------------------------------------------
     * Pre-decrement operator.
    */
    Color3 & operator -- ();

    /* --------------------------------------------------------------------------------------------
     * Post-increment operator.
    */
    Color3 operator ++ (int); // NOLINT(cert-dcl21-cpp)

    /* --------------------------------------------------------------------------------------------
     * Post-decrement operator.
    */
    Color3 operator -- (int); // NOLINT(cert-dcl21-cpp)

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    Color3 operator + (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    Color3 operator - (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    Color3 operator * (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    Color3 operator / (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Modulo operator.
    */
    Color3 operator % (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise AND operator.
    */
    Color3 operator & (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise OR operator.
    */
    Color3 operator | (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise XOR operator.
    */
    Color3 operator ^ (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise shift left operator.
    */
    Color3 operator << (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise shift right operator.
    */
    Color3 operator >> (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value addition operator.
    */
    Color3 operator + (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value subtraction operator.
    */
    Color3 operator - (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value multiplication operator.
    */
    Color3 operator * (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value division operator.
    */
    Color3 operator / (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value modulo operator.
    */
    Color3 operator % (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise AND operator.
    */
    Color3 operator & (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise OR operator.
    */
    Color3 operator | (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise XOR operator.
    */
    Color3 operator ^ (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise shift left operator.
    */
    Color3 operator << (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Scalar value bitwise shift right operator.
    */
    Color3 operator >> (Value s) const;

    /* --------------------------------------------------------------------------------------------
     * Unary plus operator.
    */
    Color3 operator + () const;

    /* --------------------------------------------------------------------------------------------
     * Unary minus operator.
    */
    Color3 operator - () const;

    /* --------------------------------------------------------------------------------------------
     * Bitwise NOT operator.
    */
    Color3 operator ~ () const;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to transparent color.
    */
    operator Color4 () const; // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const Color3 & c) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(SQInteger s) const
    {
        return Cmp(Color3(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(SQFloat s) const
    {
        return Cmp(Color3(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(bool s) const
    {
        return Cmp(Color3(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(std::nullptr_t) const
    {
        return Cmp(Color3(static_cast< Value >(0)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    SQMOD_NODISCARD String ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a JSON string.
    */
    void ToJSON(CtxJSON & ctx) const;

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified scalar value.
    */
    void SetScalar(Value ns);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from another instance of this type.
    */
    void SetColor3(const Color3 & c);

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified values.
    */
    void SetColor3Ex(Value nr, Value ng, Value nb);

    /* --------------------------------------------------------------------------------------------
     * Copy the values from an opaque color.
    */
    void SetColor4(const Color4 & c);

    /* --------------------------------------------------------------------------------------------
     * Set all components to the specified values.
    */
    void SetColor4Ex(Value nr, Value ng, Value nb, Value na);

    /* --------------------------------------------------------------------------------------------
     * Set the values extracted from the specified string using the specified delimiter.
    */
    void SetStr(SQChar delim, StackStrF & str);

    /* --------------------------------------------------------------------------------------------
     * Set the values from the identified color.
    */
    void SetName(StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Get the component values packed inside an integer value.
    */
    SQMOD_NODISCARD uint32_t GetRGB() const;

    /* --------------------------------------------------------------------------------------------
     * Set the component values wxtracted from an integer value.
    */
    void SetRGB(uint32_t p);

    /* --------------------------------------------------------------------------------------------
     * Get the component values packed inside an integer value.
    */
    SQMOD_NODISCARD uint32_t GetRGBA() const;

    /* --------------------------------------------------------------------------------------------
     * Set the component values wxtracted from an integer value.
    */
    void SetRGBA(uint32_t p);

    /* --------------------------------------------------------------------------------------------
     * Get the component values packed inside an integer value.
    */
    SQMOD_NODISCARD uint32_t GetARGB() const;

    /* --------------------------------------------------------------------------------------------
     * Set the component values wxtracted from an integer value.
    */
    void SetARGB(uint32_t p);

    /* --------------------------------------------------------------------------------------------
     * Generate random values for all components of this instance.
    */
    Color3 & Generate();

    /* --------------------------------------------------------------------------------------------
     * Generate random values for all components of this instance within the specified bounds.
    */
    Color3 & GenerateB(Value min, Value max);

    /* --------------------------------------------------------------------------------------------
     * Generate random values for all components of this instance within the specified bounds.
    */
    Color3 & GenerateR(Value rmin, Value rmax, Value gmin, Value gmax, Value bmin, Value bmax);

    /* --------------------------------------------------------------------------------------------
     * Clear the component values to default.
    */
    void Clear()
    {
        r = 0, g = 0, b = 0;
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
     * Generate a 3 component hex color from the values in this instance.
    */
    SQMOD_NODISCARD LightObj ToHex() const;

    /* --------------------------------------------------------------------------------------------
     * Generate a 4 component hex color from the values in this instance.
    */
    SQMOD_NODISCARD LightObj ToHex4() const;

    /* --------------------------------------------------------------------------------------------
     * Generate a formatted string with the values from this instance.
    */
    SQMOD_NODISCARD String Format(StackStrF & str) const;

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Color3 type from a string.
    */
    static const Color3 & Get(StackStrF & str);

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Color3 type from a string.
    */
    static const Color3 & GetEx( SQChar delim, StackStrF & str);
};

} // Namespace:: SqMod
