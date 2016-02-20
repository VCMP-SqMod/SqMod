#ifndef _LIBRARY_NUMERIC_HPP_
#define _LIBRARY_NUMERIC_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
template < typename T > class LongInt;

// ------------------------------------------------------------------------------------------------
template <> class LongInt< Int64 >
{
public:

    // --------------------------------------------------------------------------------------------
    typedef Int64 Type;

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt()
        : m_Data(0), m_Text()
    {
        /* ... */
    }

    LongInt(Type n)
        : m_Data(n), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt(CSStr text);
    LongInt(CSStr text, Object & /* null */);

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt(const LongInt< Type > & o)
        : m_Data(o.m_Data), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
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
     *
    */
    LongInt< Type > & operator = (Type data)
    {
        m_Data = data;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt< Type > & operator = (CSStr text);

    /* --------------------------------------------------------------------------------------------
     *
    */
    bool operator == (const LongInt< Type > & o) const
    {
        return (m_Data == o.m_Data);
    }

    bool operator != (const LongInt< Type > & o) const
    {
        return (m_Data != o.m_Data);
    }

    bool operator < (const LongInt< Type > & o) const
    {
        return (m_Data < o.m_Data);
    }

    bool operator > (const LongInt< Type > & o) const
    {
        return (m_Data > o.m_Data);
    }

    bool operator <= (const LongInt< Type > & o) const
    {
        return (m_Data <= o.m_Data);
    }

    bool operator >= (const LongInt< Type > & o) const
    {
        return (m_Data >= o.m_Data);
    }


    /* --------------------------------------------------------------------------------------------
     *
    */
    operator Type () const { return m_Data; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt< Type > operator + (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data + o.m_Data);
    }

    LongInt< Type > operator - (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data - o.m_Data);
    }

    LongInt< Type > operator * (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data * o.m_Data);
    }

    LongInt< Type > operator / (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data / o.m_Data);
    }

    LongInt< Type > operator % (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data % o.m_Data);
    }


    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt< Type > operator - () const
    {
        return LongInt< Type >(-m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Int32 Cmp(const LongInt< Type > & o) const
    {
        if (m_Data == o.m_Data)
            return 0;
        else if (m_Data > o.m_Data)
            return 1;
        else
            return -1;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    CSStr ToString();

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetNum(Type data)
    {
        m_Data = data;
    }

    Type GetNum() const
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQInteger GetSNum() const
    {
        return (SQInteger)(m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetStr(CSStr text)
    {
        *this = text;
    }

    CSStr GetCStr()
    {
        return ToString();
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void Random();
    void Random(Type n);
    void Random(Type m, Type n);

private:

    // --------------------------------------------------------------------------------------------
    Type    m_Data;
    SQChar  m_Text[32];
};

// ------------------------------------------------------------------------------------------------
template <> class LongInt< Uint64 >
{
public:

    // --------------------------------------------------------------------------------------------
    typedef Uint64 Type;

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt()
        : m_Data(0), m_Text()
    {
        /* ... */
    }

    LongInt(Type n)
        : m_Data(n), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt(CSStr text);
    LongInt(CSStr text, Object & /* null */);

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt(const LongInt< Type > & o)
        : m_Data(o.m_Data), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     *
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
     *
    */
    LongInt< Type > & operator = (Type data)
    {
        m_Data = data;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt< Type > & operator = (CSStr text);

    /* --------------------------------------------------------------------------------------------
     *
    */
    bool operator == (const LongInt< Type > & o) const
    {
        return (m_Data == o.m_Data);
    }

    bool operator != (const LongInt< Type > & o) const
    {
        return (m_Data != o.m_Data);
    }

    bool operator < (const LongInt< Type > & o) const
    {
        return (m_Data < o.m_Data);
    }

    bool operator > (const LongInt< Type > & o) const
    {
        return (m_Data > o.m_Data);
    }

    bool operator <= (const LongInt< Type > & o) const
    {
        return (m_Data <= o.m_Data);
    }

    bool operator >= (const LongInt< Type > & o) const
    {
        return (m_Data >= o.m_Data);
    }


    /* --------------------------------------------------------------------------------------------
     *
    */
    operator Type () const { return m_Data; }

    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt< Type > operator + (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data + o.m_Data);
    }

    LongInt< Type > operator - (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data - o.m_Data);
    }

    LongInt< Type > operator * (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data * o.m_Data);
    }

    LongInt< Type > operator / (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data / o.m_Data);
    }

    LongInt< Type > operator % (const LongInt< Type > & o) const
    {
        return LongInt< Type >(m_Data % o.m_Data);
    }


    /* --------------------------------------------------------------------------------------------
     *
    */
    LongInt< Type > operator - () const
    {
        return LongInt< Type >(-m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    Int32 Cmp(const LongInt< Type > & o) const
    {
        if (m_Data == o.m_Data)
            return 0;
        else if (m_Data > o.m_Data)
            return 1;
        else
            return -1;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    CSStr ToString();

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetNum(Type data)
    {
        m_Data = data;
    }

    Type GetNum() const
    {
        return m_Data;
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    SQInteger GetSNum() const
    {
        return (SQInteger)(m_Data);
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void SetStr(CSStr text)
    {
        *this = text;
    }

    CSStr GetCStr()
    {
        return ToString();
    }

    /* --------------------------------------------------------------------------------------------
     *
    */
    void Random();
    void Random(Type n);
    void Random(Type m, Type n);

private:

    // --------------------------------------------------------------------------------------------
    Type    m_Data;
    SQChar  m_Text[32];
};

// ------------------------------------------------------------------------------------------------
typedef LongInt< Int64 > SLongInt;
typedef LongInt< Uint64 > ULongInt;

} // Namespace:: SqMod

#endif // _LIBRARY_NUMERIC_HPP_
