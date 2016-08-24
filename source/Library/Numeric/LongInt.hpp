#ifndef _LIBRARY_NUMERIC_LONGINT_HPP_
#define _LIBRARY_NUMERIC_LONGINT_HPP_

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
     * String encoded with fall back value constructor.
    */
    LongInt(CSStr text, SQInteger fall);

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
    LongInt< Type > operator + (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data + o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data - o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data * o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data / o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data % o.m_Data);
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
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString();

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

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
     * String encoded with fall back value constructor.
    */
    LongInt(CSStr text, SQInteger fall);

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
    LongInt< Type > operator + (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data + o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Subtraction operator.
    */
    LongInt< Type > operator - (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data - o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Multiplication operator.
    */
    LongInt< Type > operator * (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data * o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Division operator.
    */
    LongInt< Type > operator / (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data / o.m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     * Modulus operator.
    */
    LongInt< Type > operator % (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data % o.m_Data);
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
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString();

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

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
typedef LongInt< Int64 > SLongInt;
typedef LongInt< Uint64 > ULongInt;

} // Namespace:: SqMod

#endif // _LIBRARY_NUMERIC_LONGINT_HPP_
