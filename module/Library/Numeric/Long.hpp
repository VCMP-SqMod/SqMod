#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
template < typename T > class LongInt;

/* ------------------------------------------------------------------------------------------------
 * Specialization of the Long int class for signed integers.
*/
template <> class LongInt< int64_t >
{
public:

    // --------------------------------------------------------------------------------------------
    typedef int64_t Type; // The specialized type.

private:

    // --------------------------------------------------------------------------------------------
    Type    m_Data; // The assigned value.
    SQChar  m_Text[24]; // String representation of the value.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LongInt()
        : m_Data(0), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit value constructor.
    */
    explicit LongInt(Type n)
        : m_Data(n), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * String encoded constructor.
    */
    explicit LongInt(const SQChar * text);

    /* --------------------------------------------------------------------------------------------
     * String encoded with explicit base constructor.
    */
    LongInt(const SQChar * text, uint32_t base);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    LongInt(const LongInt< Type > & o)
        : m_Data(o.m_Data), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~LongInt() = default;

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt & operator = (const LongInt< Type > & o) // NOLINT(bugprone-unhandled-self-assignment)
    {
        m_Data = o.m_Data;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    LongInt< Type > & operator = (Type data)
    {
        m_Data = data;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    LongInt< Type > & operator = (const SQChar * text);

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const LongInt< Type > & o) const
    {
        return (m_Data == o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const LongInt< Type > & o) const
    {
        return (m_Data != o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const LongInt< Type > & o) const
    {
        return (m_Data < o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const LongInt< Type > & o) const
    {
        return (m_Data > o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const LongInt< Type > & o) const
    {
        return (m_Data <= o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const LongInt< Type > & o) const
    {
        return (m_Data >= o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the specialized type.
    */
    operator Type () const // NOLINT(google-explicit-constructor)
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    template < typename U > LongInt< Type > operator + (const LongInt< U > & o) const
    {
        return LongInt< Type >(m_Data + ConvTo< Type >::From(o.GetNum()));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    template < typename U > LongInt< Type > operator - (const LongInt< U > & o) const
    {
        return LongInt< Type >(m_Data - ConvTo< Type >::From(o.GetNum()));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    template < typename U > LongInt< Type > operator * (const LongInt< U > & o) const
    {
        return LongInt< Type >(m_Data * ConvTo< Type >::From(o.GetNum()));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    template < typename U > LongInt< Type > operator / (const LongInt< U > & o) const
    {
        return LongInt< Type >(m_Data / ConvTo< Type >::From(o.GetNum()));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    template < typename U > LongInt< Type > operator % (const LongInt< U > & o) const
    {
        return LongInt< Type >(m_Data % ConvTo< Type >::From(o.GetNum()));
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    LongInt< Type > operator + (SQInteger s) const
    {
        return LongInt< Type >(m_Data + ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (SQInteger s) const
    {
        return LongInt< Type >(m_Data - ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (SQInteger s) const
    {
        return LongInt< Type >(m_Data * ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (SQInteger s) const
    {
        return LongInt< Type >(m_Data / ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (SQInteger s) const
    {
        return LongInt< Type >(m_Data % ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    LongInt< Type > operator + (SQFloat s) const
    {
        return LongInt< Type >(m_Data + ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (SQFloat s) const
    {
        return LongInt< Type >(m_Data - ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (SQFloat s) const
    {
        return LongInt< Type >(m_Data * ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (SQFloat s) const
    {
        return LongInt< Type >(m_Data / ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (SQFloat s) const
    {
        return LongInt< Type >(m_Data % ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    LongInt< Type > operator + (bool s) const
    {
        return LongInt< Type >(m_Data + static_cast< Type >(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (bool s) const
    {
        return LongInt< Type >(m_Data - static_cast< Type >(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (bool s) const
    {
        return LongInt< Type >(m_Data * static_cast< Type >(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (bool s) const
    {
        return LongInt< Type >(m_Data / static_cast< Type >(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (bool s) const
    {
        return LongInt< Type >(m_Data % static_cast< Type >(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    LongInt< Type > operator + (std::nullptr_t) const
    {
        return LongInt< Type >(m_Data + static_cast< Type >(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (std::nullptr_t) const
    {
        return LongInt< Type >(m_Data - static_cast< Type >(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (std::nullptr_t) const
    {
        return LongInt< Type >(m_Data * static_cast< Type >(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (std::nullptr_t) const
    {
        return LongInt< Type >(static_cast< Type >(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (std::nullptr_t) const
    {
        return LongInt< Type >(static_cast< Type >(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    LongInt< Type > operator + (const SQChar * str) const
    {
        return LongInt< Type >(m_Data + ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (const SQChar * str) const
    {
        return LongInt< Type >(m_Data - ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (const SQChar * str) const
    {
        return LongInt< Type >(m_Data * ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (const SQChar * str) const
    {
        return LongInt< Type >(m_Data / ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (const SQChar * str) const
    {
        return LongInt< Type >(m_Data % ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Unary minus operator.
    */
    LongInt< Type > operator - () const
    {
        return LongInt< Type >(-m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const LongInt< Type > & o) const
    {
        if (m_Data == o.m_Data)
        {
            return 0;
        }
        else if (m_Data > o.m_Data)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with another one.
    */
    SQMOD_NODISCARD int32_t Cmp(const LongInt< uint64_t > & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(SQInteger s) const
    {
        if (m_Data == static_cast< Type >(s))
        {
            return 0;
        }
        else if (m_Data > static_cast< Type >(s))
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(SQFloat s) const
    {
        if (m_Data == static_cast< Type >(s))
        {
            return 0;
        }
        else if (m_Data > static_cast< Type >(s))
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(bool s) const
    {
        if (m_Data == static_cast< Type >(s))
        {
            return 0;
        }
        else if (m_Data > static_cast< Type >(s))
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }


    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(std::nullptr_t) const
    {
        if (m_Data == static_cast< Type >(0))
        {
            return 0;
        }
        else if (m_Data > static_cast< Type >(0))
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(const SQChar * str) const
    {
        const Type v = ConvTo< Type >::From(str);

        if (m_Data == v)
        {
            return 0;
        }
        else if (m_Data > v)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const SQChar * ToString();

    /* --------------------------------------------------------------------------------------------
     * Assign an integer value.
    */
    void SetNum(Type data)
    {
        m_Data = data;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve an the specialized value.
    */
    SQMOD_NODISCARD Type GetNum() const
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve an a Squirrel integer value.
    */
    SQMOD_NODISCARD SQInteger GetSNum() const
    {
        return ClampL< Type, SQInteger >(m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Assign a string value.
    */
    void SetStr(const SQChar * text)
    {
        *this = text;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a string value.
    */
    const SQChar * GetCStr()
    {
        return ToString();
    }

    /* --------------------------------------------------------------------------------------------
     * Generate a random value.
    */
    void Random();

    /* --------------------------------------------------------------------------------------------
     * Generate a random value.
    */
    void Random(Type n);

    /* --------------------------------------------------------------------------------------------
     * Generate a random value.
    */
    void Random(Type m, Type n);

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel integer.
    */
    SQMOD_NODISCARD SQInteger ToSqInteger() const
    {
        return ClampL< Type, SQInteger >(m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel float.
    */
    SQMOD_NODISCARD SQFloat ToSqFloat() const
    {
        return ClampL< double, SQFloat >(static_cast< double >(m_Data));
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel string.
    */
    const SQChar * ToSqString()
    {
        return ToString();
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel boolean.
    */
    SQMOD_NODISCARD bool ToSqBool() const
    {
        return (m_Data > 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel character.
    */
    SQMOD_NODISCARD SQChar ToSqChar() const
    {
        return ClampL< Type, SQChar >(m_Data);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of the Long int class for unsigned integers.
*/
template <> class LongInt< uint64_t >
{
public:

    // --------------------------------------------------------------------------------------------
    typedef uint64_t Type; // The specialized type.

private:

    // --------------------------------------------------------------------------------------------
    Type    m_Data; // The assigned value.
    SQChar  m_Text[24]; // String representation of the value.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    LongInt()
        : m_Data(0), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit value constructor.
    */
    explicit LongInt(Type n)
        : m_Data(n), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * String encoded constructor.
    */
    explicit LongInt(const SQChar * text);

    /* --------------------------------------------------------------------------------------------
     * String encoded with explicit base constructor.
    */
    LongInt(const SQChar * text, uint32_t base);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    LongInt(const LongInt< Type > & o)
        : m_Data(o.m_Data), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~LongInt() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    LongInt & operator = (const LongInt< Type > & o) // NOLINT(bugprone-unhandled-self-assignment)
    {
        m_Data = o.m_Data;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    LongInt< Type > & operator = (Type data)
    {
        m_Data = data;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Assignment operator.
    */
    LongInt< Type > & operator = (const SQChar * text);

    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const LongInt< Type > & o) const
    {
        return (m_Data == o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const LongInt< Type > & o) const
    {
        return (m_Data != o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Less than comparison operator.
    */
    bool operator < (const LongInt< Type > & o) const
    {
        return (m_Data < o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than comparison operator.
    */
    bool operator > (const LongInt< Type > & o) const
    {
        return (m_Data > o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Less than or equal comparison operator.
    */
    bool operator <= (const LongInt< Type > & o) const
    {
        return (m_Data <= o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Greater than or equal comparison operator.
    */
    bool operator >= (const LongInt< Type > & o) const
    {
        return (m_Data >= o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the specialized type.
    */
    operator Type () const // NOLINT(google-explicit-constructor)
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    template < typename U > LongInt< Type > operator + (const LongInt< U > & o) const
    {
        return LongInt< Type >(m_Data + ConvTo< Type >::From(o.GetNum()));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    template < typename U > LongInt< Type > operator - (const LongInt< U > & o) const
    {
        return LongInt< Type >(m_Data - ConvTo< Type >::From(o.GetNum()));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    template < typename U > LongInt< Type > operator * (const LongInt< U > & o) const
    {
        return LongInt< Type >(m_Data * ConvTo< Type >::From(o.GetNum()));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    template < typename U > LongInt< Type > operator / (const LongInt< U > & o) const
    {
        return LongInt< Type >(m_Data / ConvTo< Type >::From(o.GetNum()));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    template < typename U > LongInt< Type > operator % (const LongInt< U > & o) const
    {
        return LongInt< Type >(m_Data % ConvTo< Type >::From(o.GetNum()));
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    LongInt< Type > operator + (SQInteger s) const
    {
        return LongInt< Type >(m_Data + ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (SQInteger s) const
    {
        return LongInt< Type >(m_Data - ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (SQInteger s) const
    {
        return LongInt< Type >(m_Data * ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (SQInteger s) const
    {
        return LongInt< Type >(m_Data / ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (SQInteger s) const
    {
        return LongInt< Type >(m_Data % ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    LongInt< Type > operator + (SQFloat s) const
    {
        return LongInt< Type >(m_Data + ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (SQFloat s) const
    {
        return LongInt< Type >(m_Data - ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (SQFloat s) const
    {
        return LongInt< Type >(m_Data * ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (SQFloat s) const
    {
        return LongInt< Type >(m_Data / ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (SQFloat s) const
    {
        return LongInt< Type >(m_Data % ConvTo< Type >::From(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    LongInt< Type > operator + (bool s) const
    {
        return LongInt< Type >(m_Data + static_cast< Type >(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (bool s) const
    {
        return LongInt< Type >(m_Data - static_cast< Type >(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (bool s) const
    {
        return LongInt< Type >(m_Data * static_cast< Type >(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (bool s) const
    {
        return LongInt< Type >(m_Data / static_cast< Type >(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (bool s) const
    {
        return LongInt< Type >(m_Data % static_cast< Type >(s));
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    LongInt< Type > operator + (std::nullptr_t) const
    {
        return LongInt< Type >(m_Data + static_cast< Type >(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (std::nullptr_t) const
    {
        return LongInt< Type >(m_Data - static_cast< Type >(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (std::nullptr_t) const
    {
        return LongInt< Type >(m_Data * static_cast< Type >(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (std::nullptr_t) const
    {
        return LongInt< Type >(static_cast< Type >(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (std::nullptr_t) const
    {
        return LongInt< Type >(static_cast< Type >(0));
    }

    /* --------------------------------------------------------------------------------------------
     * Addition operator.
    */
    LongInt< Type > operator + (const SQChar * str) const
    {
        return LongInt< Type >(m_Data + ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (const SQChar * str) const
    {
        return LongInt< Type >(m_Data - ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (const SQChar * str) const
    {
        return LongInt< Type >(m_Data * ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (const SQChar * str) const
    {
        return LongInt< Type >(m_Data / ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (const SQChar * str) const
    {
        return LongInt< Type >(m_Data % ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Unary minus operator.
    */
    LongInt< Type > operator - () const
    {
        return LongInt< Type >(-m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const LongInt< Type > & o) const
    {
        if (m_Data == o.m_Data)
        {
            return 0;
        }
        else if (m_Data > o.m_Data)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with another one.
    */
    SQMOD_NODISCARD int32_t Cmp(const LongInt< int64_t > & o) const
    {
        const Type v = ConvTo< Type >::From(o.GetNum());

        if (m_Data == v)
        {
            return 0;
        }
        else if (m_Data > v)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(SQInteger s) const
    {
        if (m_Data == static_cast< Type >(s))
        {
            return 0;
        }
        else if (m_Data > static_cast< Type >(s))
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(SQFloat s) const
    {
        if (m_Data == static_cast< Type >(s))
        {
            return 0;
        }
        else if (m_Data > static_cast< Type >(s))
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(bool s) const
    {
        if (m_Data == static_cast< Type >(s))
        {
            return 0;
        }
        else if (m_Data > static_cast< Type >(s))
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }


    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(std::nullptr_t) const
    {
        if (m_Data == static_cast< Type >(0))
        {
            return 0;
        }
        else if (m_Data > static_cast< Type >(0))
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    SQMOD_NODISCARD int32_t Cmp(const SQChar * str) const
    {
        const Type v = ConvTo< Type >::From(str);

        if (m_Data == v)
        {
            return 0;
        }
        else if (m_Data > v)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const SQChar * ToString();

    /* --------------------------------------------------------------------------------------------
     * Assign an integer value.
    */
    void SetNum(Type data)
    {
        m_Data = data;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve an the specialized value.
    */
    SQMOD_NODISCARD Type GetNum() const
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve an a Squirrel integer value.
    */
    SQMOD_NODISCARD SQInteger GetSNum() const
    {
        return (SQInteger)(m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Assign a string value.
    */
    void SetStr(const SQChar * text)
    {
        *this = text;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a string value.
    */
    const SQChar * GetCStr()
    {
        return ToString();
    }

    /* --------------------------------------------------------------------------------------------
     * Generate a random value.
    */
    void Random();

    /* --------------------------------------------------------------------------------------------
     * Generate a random value.
    */
    void Random(Type n);

    /* --------------------------------------------------------------------------------------------
     * Generate a random value.
    */
    void Random(Type m, Type n);

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel integer.
    */
    SQMOD_NODISCARD SQInteger ToSqInteger() const
    {
        return ClampL< Type, SQInteger >(m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel float.
    */
    SQMOD_NODISCARD SQFloat ToSqFloat() const
    {
        return ClampL< double, SQFloat >(static_cast< double >(m_Data));
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel string.
    */
    const SQChar * ToSqString()
    {
        return ToString();
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel boolean.
    */
    SQMOD_NODISCARD bool ToSqBool() const
    {
        return (m_Data > 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel character.
    */
    SQMOD_NODISCARD SQChar ToSqChar() const
    {
        return ClampL< Type, SQChar >(m_Data);
    }
};

// ------------------------------------------------------------------------------------------------
inline int32_t LongInt< int64_t >::Cmp(const LongInt< uint64_t > & o) const
{
    const Type v = ConvTo< Type >::From(o.GetNum());

    if (m_Data == v)
    {
        return 0;
    }
    else if (m_Data > v)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
typedef LongInt< int64_t > SLongInt;
typedef LongInt< uint64_t > ULongInt;

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as a signed long integer.
*/
int64_t PopStackSLong(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as an unsigned long integer.
*/
uint64_t PopStackULong(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent LongInt instance with the given values.
*/
const SLongInt & GetSLongInt();
const SLongInt & GetSLongInt(int64_t n);
const SLongInt & GetSLongInt(const SQChar * s);
const ULongInt & GetULongInt();
const ULongInt & GetULongInt(uint64_t n);
const ULongInt & GetULongInt(const SQChar * s);

} // Namespace:: SqMod
