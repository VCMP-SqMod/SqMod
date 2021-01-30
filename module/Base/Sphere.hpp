#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Vector3.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class used to represent a three-dimensional sphere.
*/
struct Sphere
{
    /* --------------------------------------------------------------------------------------------
     * The type of value used by components of type.
    */
    typedef float Value;

    /* --------------------------------------------------------------------------------------------
     * Helper instances for common values mostly used as return types or comparison.
    */
    static const Sphere NIL;
    static const Sphere MIN;
    static const Sphere MAX;

    /* --------------------------------------------------------------------------------------------
     * The delimiter character to be used when extracting values from strings.
    */
    static SQChar Delim;

    /* --------------------------------------------------------------------------------------------
     * The position and radius components of this type.
    */
    Vector3     pos{};
    Value       rad{0};

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Sphere() noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Construct a sphere at position 0,0,0 using the specified radius.
    */
    Sphere(Value rv) noexcept; // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)

    /* --------------------------------------------------------------------------------------------
     * Construct a sphere at the specified position using the specified radius.
    */
    Sphere(const Vector3 & pv, Value rv) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Construct a sphere at the specified position using the specified radius.
    */
    Sphere(Value xv, Value yv, Value zv, Value rv) noexcept;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Sphere(const Sphere & o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Sphere(Sphere && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Sphere() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Sphere & operator = (const Sphere & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Sphere & operator = (Sphere && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Radius assignment operator.
    */
    Sphere & operator = (Value r);

    /* --------------------------------------------------------------------------------------------
     * Position assignment operator.
    */
    Sphere & operator = (const Vector3 & p);

    /* --------------------------------------------------------------------------------------------
     * Addition assignment operator.
    */
    Sphere & operator += (const Sphere & s);

    /* --------------------------------------------------------------------------------------------
     * Subtraction assignment operator.
    */
    Sphere & operator -= (const Sphere & s);

    /* --------------------------------------------------------------------------------------------
     * Multiplication assignment operator.
    */
    Sphere & operator *= (const Sphere & s);

    /* --------------------------------------------------------------------------------------------
     * Division assignment operator.
    */
    Sphere & operator /= (const Sphere & s);

    /* --------------------------------------------------------------------------------------------
     * Modulo assignment operator.
    */
    Sphere & operator %= (const Sphere & s);

    /* --------------------------------------------------------------------------------------------
     * Radius addition assignment operator.
    */
    Sphere & operator += (Value r);

    /* --------------------------------------------------------------------------------------------
     * Radius subtraction assignment operator.
    */
    Sphere & operator -= (Value r);

    /* --------------------------------------------------------------------------------------------
     * Radius multiplication assignment operator.
    */
    Sphere & operator *= (Value r);

    /* --------------------------------------------------------------------------------------------
     * Radius division assignment operator.
    */
    Sphere & operator /= (Value r);

    /* --------------------------------------------------------------------------------------------
     * Radius modulo assignment operator.
    */
    Sphere & operator %= (Value r);

    /* --------------------------------------------------------------------------------------------
     * Position addition assignment operator.
    */
    Sphere & operator += (const Vector3 & p);

    /* --------------------------------------------------------------------------------------------
     * Position subtraction assignment operator.
    */
    Sphere & operator -= (const Vector3 & p);

    /* --------------------------------------------------------------------------------------------
     * Position multiplication assignment operator.
    */
    Sphere & operator *= (const Vector3 & p);

    /* --------------------------------------------------------------------------------------------
     * Position division assignment operator.
    */
    Sphere & operator /= (const Vector3 & p);

    /* --------------------------------------------------------------------------------------------
     * Position modulo assignment operator.
    */
    Sphere & operator %= (const Vector3 & p);

    /* --------------------------------------------------------------------------------------------
     * Pre-increment operator.
    */
    Sphere & operator ++ ();

    /* --------------------------------------------------------------------------------------------
     * Pre-decrement operator.
    */
    Sphere & operator -- ();

    /* --------------------------------------------------------------------------------------------
     * Post-increment operator.
    */
    Sphere operator ++ (int); // NOLINT(cert-dcl21-cpp)

    /* --------------------------------------------------------------------------------------------
     * Post-decrement operator.
    */
    Sphere operator -- (int); // NOLINT(cert-dcl21-cpp)

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    Sphere operator + (const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    Sphere operator - (const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    Sphere operator * (const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    Sphere operator / (const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Modulo operator.
    */
    Sphere operator % (const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Radius addition operator.
    */
    Sphere operator + (Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Radius subtraction operator.
    */
    Sphere operator - (Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Radius multiplication operator.
    */
    Sphere operator * (Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Radius division operator.
    */
    Sphere operator / (Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Radius modulo operator.
    */
    Sphere operator % (Value r) const;

    /* --------------------------------------------------------------------------------------------
     * Position addition operator.
    */
    Sphere operator + (const Vector3 & p) const;

    /* --------------------------------------------------------------------------------------------
     * Position subtraction operator.
    */
    Sphere operator - (const Vector3 & p) const;

    /* --------------------------------------------------------------------------------------------
     * Position multiplication operator.
    */
    Sphere operator * (const Vector3 & p) const;

    /* --------------------------------------------------------------------------------------------
     * Position division operator.
    */
    Sphere operator / (const Vector3 & p) const;

    /* --------------------------------------------------------------------------------------------
     * Position modulo operator.
    */
    Sphere operator % (const Vector3 & p) const;

    /* --------------------------------------------------------------------------------------------
     * Unary plus operator.
    */
    Sphere operator + () const;

    /* --------------------------------------------------------------------------------------------
     * Unary minus operator.
    */
    Sphere operator - () const;

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const Sphere & s) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(SQFloat s) const
    {
        return Cmp(Sphere(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(SQInteger s) const
    {
        return Cmp(Sphere(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(bool s) const
    {
        return Cmp(Sphere(static_cast< Value >(s)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(std::nullptr_t) const
    {
        return Cmp(Sphere(static_cast< Value >(0)));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    SQMOD_NODISCARD String ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Set the specified radius.
    */
    void SetRadius(Value nr);

    /* --------------------------------------------------------------------------------------------
     *  Copy the sphere from another instance of this type.
    */
    void SetSphere(const Sphere & ns);

    /* --------------------------------------------------------------------------------------------
     * Set the specified position and radius.
    */
    void SetSphereEx(Value nx, Value ny, Value nz, Value nr);

    /* --------------------------------------------------------------------------------------------
     * Set the specified position and radius.
    */
    void SetValues(const Vector3 & np, Value nr);

    /* --------------------------------------------------------------------------------------------
     * Set the position from the specified position.
    */
    void SetPosition(const Vector3 & np);

    /* --------------------------------------------------------------------------------------------
     * Set the specified position.
    */
    void SetPositionEx(Value nx, Value ny, Value nz);

    /* --------------------------------------------------------------------------------------------
     * Set the values extracted from the specified string using the specified delimiter.
    */
    void SetStr(SQChar delim, StackStrF & values);

    /* --------------------------------------------------------------------------------------------
     * Generate a randomly sized and positioned sphere.
    */
    void Generate();

    /* --------------------------------------------------------------------------------------------
     * Generate a randomly sized or positioned sphere within the specified bounds.
    */
    void Generate(Value min, Value max, bool r);

    /* --------------------------------------------------------------------------------------------
     * Generate a randomly positioned sphere within the specified bounds.
    */
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax);

    /* --------------------------------------------------------------------------------------------
     * Generate a randomly sized and positioned sphere within the specified bounds.
    */
    void Generate(Value xmin, Value xmax, Value ymin, Value ymax, Value zmin, Value zmax, Value rmin, Value rmax);

    /* --------------------------------------------------------------------------------------------
     * Clear the component values to default.
    */
    void Clear()
    {
        pos.Clear(); rad = 0.0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a new instance of this type with absolute component values.
    */
    SQMOD_NODISCARD Sphere Abs() const;

    /* --------------------------------------------------------------------------------------------
     * Generate a formatted string with the values from this instance.
    */
    SQMOD_NODISCARD String Format(StackStrF & str) const;

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Sphere type from a string.
    */
    static const Sphere & Get(StackStrF & str);

    /* --------------------------------------------------------------------------------------------
     * Extract the values for components of the Sphere type from a string.
    */
    static const Sphere & GetEx(SQChar delim, StackStrF & str);
};

} // Namespace:: SqMod
