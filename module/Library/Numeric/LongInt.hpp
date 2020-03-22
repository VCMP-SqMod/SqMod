#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
template < typename T > class LongInt;

/* ------------------------------------------------------------------------------------------------
 * Specialization of the Long int class for signed integers.
*/
template <> class LongInt< Int64 >
{
public:

    // --------------------------------------------------------------------------------------------
    typedef Int64 Type; // The specialized type.

private:

    // --------------------------------------------------------------------------------------------
    Type    m_Data; // The assigned value.
    SQChar  m_Text[32]; // String representation of the value.

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
    explicit LongInt(CSStr text);

    /* --------------------------------------------------------------------------------------------
     * String encoded with explicit base constructor.
    */
    LongInt(CSStr text, Uint32 base);

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
    ~LongInt()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt & operator = (const LongInt< Type > & o)
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
    LongInt< Type > & operator = (CSStr text);

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
    operator Type () const
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Adition operator.
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
     * Adition operator.
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
     * Adition operator.
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
     * Adition operator.
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
     * Adition operator.
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
     * Adition operator.
    */
    LongInt< Type > operator + (CSStr str) const
    {
        return LongInt< Type >(m_Data + ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (CSStr str) const
    {
        return LongInt< Type >(m_Data - ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (CSStr str) const
    {
        return LongInt< Type >(m_Data * ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (CSStr str) const
    {
        return LongInt< Type >(m_Data / ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (CSStr str) const
    {
        return LongInt< Type >(m_Data % ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Unarry minus operator.
    */
    LongInt< Type > operator - () const
    {
        return LongInt< Type >(-m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const LongInt< Type > & o) const
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
    Int32 Cmp(const LongInt< Uint64 > & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare an instance of this type with a scalar value.
    */
    Int32 Cmp(SQInteger s) const
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
    Int32 Cmp(SQFloat s) const
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
    Int32 Cmp(bool s) const
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
    Int32 Cmp(std::nullptr_t) const
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
    Int32 Cmp(CSStr str) const
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
    CSStr ToString();

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
    Type GetNum() const
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve an a Squirrel integer value.
    */
    SQInteger GetSNum() const
    {
        return ClampL< Type, SQInteger >(m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Assign a string value.
    */
    void SetStr(CSStr text)
    {
        *this = text;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a string value.
    */
    CSStr GetCStr()
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
    SQInteger ToSqInteger() const
    {
        return ClampL< Type, SQInteger >(m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel float.
    */
    SQFloat ToSqFloat() const
    {
        return ClampL< Float64, SQFloat >(static_cast< Float64 >(m_Data));
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel string.
    */
    CSStr ToSqString()
    {
        return ToString();
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel boolean.
    */
    bool ToSqBool() const
    {
        return (m_Data > 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel character.
    */
    SQChar ToSqChar() const
    {
        return ClampL< Type, SQChar >(m_Data);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Specialization of the Long int class for unsigned integers.
*/
template <> class LongInt< Uint64 >
{
public:

    // --------------------------------------------------------------------------------------------
    typedef Uint64 Type; // The specialized type.

private:

    // --------------------------------------------------------------------------------------------
    Type    m_Data; // The assigned value.
    SQChar  m_Text[32]; // String representation of the value.

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
    LongInt(Type n)
        : m_Data(n), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * String encoded constructor.
    */
    LongInt(CSStr text);

    /* --------------------------------------------------------------------------------------------
     * String encoded with explicit base constructor.
    */
    LongInt(CSStr text, Uint32 base);

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
    ~LongInt()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    LongInt & operator = (const LongInt< Type > & o)
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
    LongInt< Type > & operator = (CSStr text);

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
    operator Type () const
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Adition operator.
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
     * Adition operator.
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
     * Adition operator.
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
     * Adition operator.
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
     * Adition operator.
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
     * Adition operator.
    */
    LongInt< Type > operator + (CSStr str) const
    {
        return LongInt< Type >(m_Data + ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (CSStr str) const
    {
        return LongInt< Type >(m_Data - ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (CSStr str) const
    {
        return LongInt< Type >(m_Data * ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (CSStr str) const
    {
        return LongInt< Type >(m_Data / ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (CSStr str) const
    {
        return LongInt< Type >(m_Data % ConvTo< Type >::From(str));
    }

    /* --------------------------------------------------------------------------------------------
     * Unarry minus operator.
    */
    LongInt< Type > operator - () const
    {
        return LongInt< Type >(-m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const LongInt< Type > & o) const
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
    Int32 Cmp(const LongInt< Int64 > & o) const
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
    Int32 Cmp(SQInteger s) const
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
    Int32 Cmp(SQFloat s) const
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
    Int32 Cmp(bool s) const
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
    Int32 Cmp(std::nullptr_t) const
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
    Int32 Cmp(CSStr str) const
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
    CSStr ToString();

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
    Type GetNum() const
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve an a Squirrel integer value.
    */
    SQInteger GetSNum() const
    {
        return (SQInteger)(m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Assign a string value.
    */
    void SetStr(CSStr text)
    {
        *this = text;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve a string value.
    */
    CSStr GetCStr()
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
    SQInteger ToSqInteger() const
    {
        return ClampL< Type, SQInteger >(m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel float.
    */
    SQFloat ToSqFloat() const
    {
        return ClampL< Float64, SQFloat >(static_cast< Float64 >(m_Data));
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel string.
    */
    CSStr ToSqString()
    {
        return ToString();
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel boolean.
    */
    bool ToSqBool() const
    {
        return (m_Data > 0);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to convert the long integer to a squirrel character.
    */
    SQChar ToSqChar() const
    {
        return ClampL< Type, SQChar >(m_Data);
    }
};

// ------------------------------------------------------------------------------------------------
inline Int32 LongInt< Int64 >::Cmp(const LongInt< Uint64 > & o) const
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
typedef LongInt< Int64 > SLongInt;
typedef LongInt< Uint64 > ULongInt;

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as a signed long integer.
*/
Int64 PopStackSLong(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as an unsigned long integer.
*/
Uint64 PopStackULong(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Get a persistent LongInt instance with the given values.
*/
const SLongInt & GetSLongInt();
const SLongInt & GetSLongInt(Int64 n);
const SLongInt & GetSLongInt(CSStr s);
const ULongInt & GetULongInt();
const ULongInt & GetULongInt(Uint64 n);
const ULongInt & GetULongInt(CSStr s);

} // Namespace:: SqMod
