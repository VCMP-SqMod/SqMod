#ifndef _LIBRARY_NUMERIC_DECIMAL_HPP_
#define _LIBRARY_NUMERIC_DECIMAL_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Simple decimal data type support. Mostly for working with databases that support this type.
*/
class Decimal
{
private:

    // --------------------------------------------------------------------------------------------
    Int64   m_Value; // Decimal value
    Uint8   m_Precision; // Decimal precision

    // --------------------------------------------------------------------------------------------
    static const Int64 s_Factors[];

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the given precision.
    */
    static void ValidatePrecision(Uint8 precision);

    /* --------------------------------------------------------------------------------------------
     * Convert a decimal to the same precision as this one.
    */
    Int64 Convert(const Decimal & dec) const;

    /* --------------------------------------------------------------------------------------------
     * Validate against given decimal.
    */
    Int32 Compare(const Decimal & o) const;

    /* --------------------------------------------------------------------------------------------
     * Result = (a * b) / d
    */
    static Int64 MultiplyDivide(Int64 a, Int64 b, Int64 d)
    {
        if ((std::abs(a) <= std::numeric_limits< Int32 >::max()) ||
            (std::abs(b) <= std::numeric_limits< Int32 >::max()))
        {
            return std::llround(static_cast< Float64 >(a * b) / static_cast< Float64 >(d));
        }

        return std::llround(static_cast< Float64 >(a) * static_cast< Float64 >(b) / static_cast< Float64 >(d));
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Decimal()
        : m_Value(0), m_Precision(4)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * String constructor.
    */
    Decimal(CSStr value);

    /* --------------------------------------------------------------------------------------------
     * Numeric constructor.
    */
    Decimal(SQInteger value, Uint8 precision);

#ifndef _SQ64

    /* --------------------------------------------------------------------------------------------
     * Numeric constructor.
    */
    Decimal(Int64 value, Uint8 precision);

#endif // _SQ64

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Decimal(const Decimal & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Decimal(Decimal && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Decimal() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Decimal & operator = (const Decimal & o)
    {
        m_Value = Convert(o);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Decimal & operator = (Decimal && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Decimal & o) const
    {
        return Compare(o);
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
     * Perform an equality comparison between to instances of this type.
    */
    bool operator == (const Decimal & o) const
    {
        return m_Value == Convert(o);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between to instances of this type.
    */
    bool operator != (const Decimal & o) const
    {
        return m_Value != Convert(o);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform a less than comparison between to instances of this type.
    */
    bool operator < (const Decimal & o) const
    {
        return m_Value < Convert(o);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform a greater than comparison between to instances of this type.
    */
    bool operator > (const Decimal & o) const
    {
        return m_Value > Convert(o);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform a less than or equal comparison between to instances of this type.
    */
    bool operator <= (const Decimal & o) const
    {
        return m_Value <= Convert(o);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform a greater than or equal comparison between to instances of this type.
    */
    bool operator >= (const Decimal & o) const
    {
        return m_Value >= Convert(o);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an addition between to instances of this type.
    */
    Decimal operator + (const Decimal & o) const
    {
        return Decimal(m_Value + Convert(o), m_Precision);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an subtraction between to instances of this type.
    */
    Decimal operator - (const Decimal & o) const
    {
        return Decimal(m_Value - Convert(o), m_Precision);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an multiplication between to instances of this type.
    */
    Decimal operator * (const Decimal & o) const
    {
        return Decimal(MultiplyDivide(m_Value, Convert(o), s_Factors[m_Precision]), m_Precision);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an division between to instances of this type.
    */
    Decimal operator / (const Decimal & o) const
    {
        return Decimal(MultiplyDivide(m_Value, s_Factors[m_Precision], Convert(o)), m_Precision);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an assignment addition between to instances of this type.
    */
    Decimal & operator += (const Decimal & o)
    {
        m_Value += Convert(o);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an subtraction addition between to instances of this type.
    */
    Decimal & operator -= (const Decimal & o)
    {
        m_Value -= Convert(o);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an multiplication addition between to instances of this type.
    */
    Decimal & operator *= (const Decimal & o)
    {
        m_Value -= MultiplyDivide(m_Value, Convert(o), s_Factors[m_Precision]);
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an division addition between to instances of this type.
    */
    Decimal & operator /= (const Decimal & o)
    {
        m_Value -= MultiplyDivide(m_Value, s_Factors[m_Precision], Convert(o));
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Convert a decimal to the same precision as this one.
    */
    SLongInt GetConverted(const Decimal & dec) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the factor.
    */
    SLongInt GetFactor() const;

    /* --------------------------------------------------------------------------------------------
     * Returns integer value = real_value * (10 ^ precision)
    */
    SLongInt GetUnbiased() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value.
    */
    Int64 GetValue() const
    {
        return m_Value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the precision.
    */
    Uint8 GetPrecision() const
    {
        return m_Precision;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the absolute value.
    */
    Decimal GetAbs() const
    {
        if (m_Value >= 0)
        {
            return *this;
        }
        // Calculate the absolute value
        return Decimal(0, s_Factors[m_Precision]) - *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the decimal value as a native script floating point value.
    */
    SQFloat GetSqFloat() const
    {
        return static_cast< SQFloat >(m_Value) / static_cast< SQFloat >(m_Precision * s_Factors[m_Precision]);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the decimal value as a single floating point value.
    */
    Float32 GetFloat32() const
    {
        return static_cast< Float32 >(m_Value) / static_cast< Float32 >(m_Precision * s_Factors[m_Precision]);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the decimal value as a double floating point value.
    */
    Float64 GetFloat64() const
    {
        return static_cast< Float64 >(m_Value) / static_cast< Float64 >(m_Precision * s_Factors[m_Precision]);
    }

    /* --------------------------------------------------------------------------------------------
     * Convert the decimal to a string and store it into the given buffer.
    */
    void MakeString(CStr buffer, Uint32 size) const;

};

} // Namespace:: SqMod

#endif // _LIBRARY_NUMERIC_DECIMAL_HPP_
