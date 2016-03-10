#ifndef _SQMMDB_COMMON_HPP_
#define _SQMMDB_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "ModBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <cassert>
#include <cmath>

// ------------------------------------------------------------------------------------------------
#include <maxminddb.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQMMDB_NAME "Squirrel MaxmindDB Module"
#define SQMMDB_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQMMDB_COPYRIGHT "Copyright (C) 2016 Sandu Liviu Catalin"
#define SQMMDB_HOST_NAME "SqModMMDBHost"
#define SQMMDB_VERSION 001
#define SQMMDB_VERSION_STR "0.0.1"
#define SQMMDB_VERSION_MAJOR 0
#define SQMMDB_VERSION_MINOR 0
#define SQMMDB_VERSION_PATCH 1

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Database;
class SockAddr;
class EntryDataList;
class LookupResult;

/* ------------------------------------------------------------------------------------------------
 * Retrieve the temporary buffer.
*/
SStr GetTempBuff();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the size of the temporary buffer.
*/
Uint32 GetTempBuffSize();

/* ------------------------------------------------------------------------------------------------
 * Throw a formatted exception.
*/
void SqThrowF(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string.
*/
CSStr FmtStr(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Implements RAII to restore the VM stack to it's initial size on function exit.
*/
struct StackGuard
{
    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    StackGuard(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StackGuard();

private:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    StackGuard(const StackGuard &);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    StackGuard(StackGuard &&);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    StackGuard & operator = (const StackGuard &);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    StackGuard & operator = (StackGuard &&);

private:

    // --------------------------------------------------------------------------------------------
    Int32       m_Top; /* The top of the stack when this instance was created. */
    HSQUIRRELVM m_VM; /* The VM where the stack should be restored. */
};

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted INI document instance.
*/
class DbRef
{
    // --------------------------------------------------------------------------------------------
    friend class Database;

public:

    // --------------------------------------------------------------------------------------------
    typedef MMDB_s          Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter; // Reference counter type.

private:

    // --------------------------------------------------------------------------------------------
    Pointer     m_Ptr; // The document reader, writer and manager instance.
    Counter*    m_Ref; // Reference count to the managed instance.

    /* --------------------------------------------------------------------------------------------
     * Creates a database structure.
    */
    static Pointer Create();

    /* --------------------------------------------------------------------------------------------
     * Destroyes the specified database structure.
    */
    static void Destroy(Pointer db);

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a document instance.
    */
    void Grab()
    {
        if (m_Ptr)
            ++(*m_Ref);
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a document instance.
    */
    void Drop()
    {
        if (m_Ptr && --(*m_Ref) == 0)
        {
            MMDB_close(m_Ptr);
            Destroy(m_Ptr);
            delete m_Ref;
            m_Ptr = NULL;
            m_Ref = NULL;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    DbRef(bool make)
        : m_Ptr(make ? Create() : NULL), m_Ref(m_Ptr ? new Counter(1) : NULL)
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    DbRef()
        : m_Ptr(NULL), m_Ref(NULL)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    DbRef(const DbRef & o)
        : m_Ptr(o.m_Ptr), m_Ref(o.m_Ref)

    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DbRef(DbRef && o)
        : m_Ptr(o.m_Ptr), m_Ref(o.m_Ref)

    {
        o.m_Ptr = NULL;
        o.m_Ref = NULL;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DbRef()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    DbRef & operator = (const DbRef & o)
    {
        if (m_Ptr != o.m_Ptr)
        {
            Drop();
            m_Ptr = o.m_Ptr;
            m_Ref = o.m_Ref;
            Grab();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DbRef & operator = (DbRef && o)
    {
        if (m_Ptr != o.m_Ptr)
        {
            m_Ptr = o.m_Ptr;
            m_Ref = o.m_Ref;
            o.m_Ptr = NULL;
            o.m_Ref = NULL;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two document instances.
    */
    bool operator == (const DbRef & o) const
    {
        return (m_Ptr == o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two document instances.
    */
    bool operator != (const DbRef & o) const
    {
        return (m_Ptr != o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance pointer.
    */
    operator Pointer ()
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance pointer.
    */
    operator ConstPtr () const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    operator Reference ()
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    operator ConstRef () const
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Pointer operator -> () const
    {
        assert(m_Ptr);
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Reference operator * () const
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Pointer DbPtr()
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Pointer DbPtr() const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to the managed instance.
    */
    Counter Count() const
    {
        return (m_Ptr && m_Ref) ? (*m_Ref) : 0;
    }
};

// ------------------------------------------------------------------------------------------------
template < typename T > struct NumLimit;

// ------------------------------------------------------------------------------------------------
template <> struct NumLimit< char > { static const char Min, Max; };
template <> struct NumLimit< signed char > { static const signed char Min, Max; };
template <> struct NumLimit< unsigned char > { static const unsigned char Min, Max; };
template <> struct NumLimit< signed short > { static const signed short Min, Max; };
template <> struct NumLimit< unsigned short > { static const unsigned short Min, Max; };
template <> struct NumLimit< signed int > { static const signed int Min, Max; };
template <> struct NumLimit< unsigned int > { static const unsigned int Min, Max; };
template <> struct NumLimit< signed long > { static const signed long Min, Max; };
template <> struct NumLimit< unsigned long > { static const unsigned long Min, Max; };
template <> struct NumLimit< signed long long > { static const signed long long Min, Max; };
template <> struct NumLimit< unsigned long long > { static const unsigned long long Min, Max; };
template <> struct NumLimit< float > { static const float Min, Max; };
template <> struct NumLimit< double > { static const double Min, Max; };
template <> struct NumLimit< long double > { static const long double Min, Max; };

// ------------------------------------------------------------------------------------------------
template< typename T > inline bool EpsEq(const T a, const T b)
{
    return abs(a - b) <= 0;
}

template <> inline bool EpsEq(const Float32 a, const Float32 b)
{
    return fabs(a - b) <= 0.000001f;
}

template <> inline bool EpsEq(const Float64 a, const Float64 b)
{
    return fabs(a - b) <= 0.000000001d;
}

// ------------------------------------------------------------------------------------------------
template< typename T > inline bool EpsLt(const T a, const T b)
{
    return !EpsEq(a, b) && (a < b);
}

template <> inline bool EpsLt(const Float32 a, const Float32 b)
{
    return !EpsEq(a, b) && (a - b) < 0.000001f;
}

template <> inline bool EpsLt(const Float64 a, const Float64 b)
{
    return !EpsEq(a, b) && (a - b) < 0.000000001d;
}

// ------------------------------------------------------------------------------------------------
template< typename T > inline bool EpsGt(const T a, const T b)
{
    return !EpsEq(a, b) && (a > b);
}

template <> inline bool EpsGt(const Float32 a, const Float32 b)
{
    return !EpsEq(a, b) && (a - b) > 0.000001f;
}

template <> inline bool EpsGt(const Float64 a, const Float64 b)
{
    return !EpsEq(a, b) && (a - b) > 0.000000001d;
}

// ------------------------------------------------------------------------------------------------
template< typename T > inline bool EpsLtEq(const T a, const T b)
{
    return !EpsEq(a, b) || (a < b);
}

template <> inline bool EpsLtEq(const Float32 a, const Float32 b)
{
    return !EpsEq(a, b) || (a - b) < 0.000001f;
}

template <> inline bool EpsLtEq(const Float64 a, const Float64 b)
{
    return !EpsEq(a, b) || (a - b) < 0.000000001d;
}

// ------------------------------------------------------------------------------------------------
template< typename T > inline bool EpsGtEq(const T a, const T b)
{
    return !EpsEq(a, b) || (a > b);
}

template <> inline bool EpsGtEq(const Float32 a, const Float32 b)
{
    return !EpsEq(a, b) || (a - b) > 0.000001f;
}

template <> inline bool EpsGtEq(const Float64 a, const Float64 b)
{
    return !EpsEq(a, b) || (a - b) > 0.000000001d;
}

// ------------------------------------------------------------------------------------------------
template< typename T > inline T Clamp(T val, T min, T max)
{
    return val < min ? min : (val > max ? max : val);
}

} // Namespace:: SqMod

#endif // _SQMMDB_COMMON_HPP_
