#ifndef _SQSQLITE_COMMON_HPP_
#define _SQSQLITE_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "ModBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <map>
#include <vector>

// ------------------------------------------------------------------------------------------------
#include <assert.h>

// ------------------------------------------------------------------------------------------------
#include <sqlite3.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Connection;
class Statement;
class Column;
class Transaction;

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQSQLITE_NAME "Squirrel SQLite Module"
#define SQSQLITE_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQSQLITE_COPYRIGHT "Copyright (C) 2016 Sandu Liviu Catalin"
#define SQSQLITE_HOST_NAME "SqModSQLiteHost"
#define SQSQLITE_VERSION 001
#define SQSQLITE_VERSION_STR "0.0.1"
#define SQSQLITE_VERSION_MAJOR 0
#define SQSQLITE_VERSION_MINOR 0
#define SQSQLITE_VERSION_PATCH 1

/* ------------------------------------------------------------------------------------------------
 * Retrieve the temporary buffer.
*/
SStr GetTempBuff();

/* ------------------------------------------------------------------------------------------------
 * Throw a formatted exception.
*/
void SqThrowF(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string.
*/
CSStr FmtStr(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted query.
*/
CSStr QFmtStr(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Tests if a certain query string is empty.
*/
bool IsQueryEmpty(CSStr str);

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted database connection handle.
*/
class ConnHnd
{
    // --------------------------------------------------------------------------------------------
    friend class Connection;
    friend class Statement;

public:

    // --------------------------------------------------------------------------------------------
    typedef sqlite3         Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter; // Reference counter type.

protected:

    // --------------------------------------------------------------------------------------------
    typedef std::vector< String > QueryList; // Container used to queue queries.

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a certain connection.
    */
    struct Handle
    {
        // --------------------------------------------------------------------------------------------
        Pointer     mPtr; // The connection handle resource.
        Counter     mRef; // Reference count to the managed handle.

        // --------------------------------------------------------------------------------------------
        Int32       mStatus; // The last status code of this connection handle.

        // --------------------------------------------------------------------------------------------
        QueryList   mQueue; // A queue of queries to be executed in groups.

        // --------------------------------------------------------------------------------------------
        Int32       mFlags; // The flags used to create the database connection handle.
        String      mName; // The specified name to be used as the database file.
        String      mVFS; // The specified virtual file system.

        // --------------------------------------------------------------------------------------------
        bool        mMemory; // Whether the database exists in memory and not disk.
        bool        mTrace; // Whether tracing was activated on the database.
        bool        mProfile; // Whether profiling was activated on the database.

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Handle(Counter counter)
            : mPtr(NULL)
            , mRef(counter)
            , mStatus(SQLITE_OK)
            , mQueue()
            , mFlags(0)
            , mName()
            , mVFS()
            , mMemory(false)
            , mTrace(false)
            , mProfile(false)
        {
            /* ... */
        }

        /* --------------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Handle();

        /* --------------------------------------------------------------------------------------------
         * Create the database connection resource.
        */
        void Create(CSStr name, Int32 flags, CSStr vfs);

        /* --------------------------------------------------------------------------------------------
         * Execute a specific amount of queries from the queue.
        */
        Int32 Flush(Uint32 num, Object & env, Function & func);
    };

private:

    // --------------------------------------------------------------------------------------------
    Handle* m_Hnd;

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a connection handle.
    */
    void Grab()
    {
        if (m_Hnd)
            ++(m_Hnd->mRef);
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a connection handle.
    */
    void Drop()
    {
        if (m_Hnd && --(m_Hnd->mRef) == 0)
            delete m_Hnd; // Let the destructor take care of cleaning up (if necessary)
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    ConnHnd(CSStr name)
        : m_Hnd(name ? new Handle(1) : NULL)
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    ConnHnd()
        : m_Hnd(NULL)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ConnHnd(const ConnHnd & o)
        : m_Hnd(o.m_Hnd)

    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ConnHnd()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    ConnHnd & operator = (const ConnHnd & o)
    {
        if (m_Hnd != o.m_Hnd)
        {
            Drop();
            m_Hnd = o.m_Hnd;
            Grab();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Status assignment operator.
    */
    ConnHnd & operator = (Int32 status)
    {
        if (m_Hnd)
            m_Hnd->mStatus = status;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two connection handles.
    */
    bool operator == (const ConnHnd & o) const
    {
        return (m_Hnd == o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two connection handles.
    */
    bool operator != (const ConnHnd & o) const
    {
        return (m_Hnd != o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison with an integer value status.
    */
    bool operator == (Int32 status) const
    {
        if (m_Hnd)
            return (m_Hnd->mStatus == status);
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison with an integer value status.
    */
    bool operator != (Int32 status) const
    {
        if (m_Hnd)
            return (m_Hnd->mStatus != status);
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return m_Hnd && m_Hnd->mPtr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer ()
    {
        return m_Hnd ? m_Hnd->mPtr : NULL;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer () const
    {
        return m_Hnd ? m_Hnd->mPtr : NULL;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Reference ()
    {
        assert(m_Hnd && m_Hnd->mPtr);
        return *(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator ConstRef () const
    {
        assert(m_Hnd && m_Hnd->mPtr);
        return *(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Handle * operator -> () const
    {
        assert(m_Hnd);
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Handle & operator * () const
    {
        assert(m_Hnd);
        return *m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr()
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr() const
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to the managed instance.
    */
    Counter Count() const
    {
        assert(m_Hnd);
        return m_Hnd ? m_Hnd->mRef : 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CCStr ErrStr() const
    {
        assert(m_Hnd); // SQLite does it's null pointer validations internally
        return sqlite3_errstr(sqlite3_errcode(m_Hnd->mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CCStr ErrMsg() const
    {
        assert(m_Hnd); // SQLite does it's null pointer validations internally
        return sqlite3_errmsg(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 ErrNo() const
    {
        assert(m_Hnd); // SQLite does it's null pointer validations internally
        return sqlite3_errcode(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 ExErrNo() const
    {
        assert(m_Hnd); // SQLite does it's null pointer validations internally
        return sqlite3_extended_errcode(m_Hnd->mPtr);
    }
};

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted database statement handle.
*/
class StmtHnd
{
    // --------------------------------------------------------------------------------------------
    friend class Connection;
    friend class Statement;

public:

    // --------------------------------------------------------------------------------------------
    typedef sqlite3_stmt    Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter; // Reference counter type.

protected:

    // --------------------------------------------------------------------------------------------
    typedef std::map< String, int > Indexes; // Container used to identify column indexes.

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a certain statement.
    */
    struct Handle
    {
        // --------------------------------------------------------------------------------------------
        Pointer     mPtr; // The statement handle resource.
        Counter     mRef; // Reference count to the managed handle.

        // ----------------------------------------------------------------------------------------
        Int32       mStatus; // The last status code of this connection handle.

        // ----------------------------------------------------------------------------------------
        ConnHnd     mConn; // The handle to the associated database connection.

        // ----------------------------------------------------------------------------------------
        String      mQuery; // The query string used to create this statement.

        // ----------------------------------------------------------------------------------------
        Int32       mColumns; // The amount of columns available in this statement.
        Indexes     mIndexes; // An associative container with column names and their index.

        // ----------------------------------------------------------------------------------------
        bool        mGood; // True when a row has been fetched with step.
        bool        mDone; // True when the last step had no more rows to fetch.

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Handle(const ConnHnd & conn, Counter counter)
            : mPtr(NULL)
            , mRef(counter)
            , mStatus(SQLITE_OK)
            , mConn(conn)
            , mQuery()
            , mColumns(0)
            , mIndexes()
            , mGood(false)
            , mDone(false)
        {
            /* ... */
        }

        /* --------------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Handle();

        /* --------------------------------------------------------------------------------------------
         * Create the database statement resource.
        */
        void Create(CSStr query);

        /* --------------------------------------------------------------------------------------------
         * Check whether a specific index is in range.
        */
        bool CheckIndex(Int32 idx) const
        {
            return (idx >= 0) && (idx < mColumns);
        }

        /* --------------------------------------------------------------------------------------------
         * Retrieve the column index associated with the specified name.
        */
        Int32 GetColumnIndex(CSStr name);
    };

private:

    // --------------------------------------------------------------------------------------------
    Handle* m_Hnd;

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a statement handle.
    */
    void Grab()
    {
        if (m_Hnd)
            ++(m_Hnd->mRef);
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a statement handle.
    */
    void Drop()
    {
        if (m_Hnd && --(m_Hnd->mRef) == 0)
            delete m_Hnd; // Let the destructor take care of cleaning up (if necessary)
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    StmtHnd(const ConnHnd & db)
        : m_Hnd(new Handle(db, 1))
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    StmtHnd()
        : m_Hnd(NULL)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    StmtHnd(const StmtHnd & o)
        : m_Hnd(o.m_Hnd)

    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StmtHnd()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    StmtHnd & operator = (const StmtHnd & o)
    {
        if (m_Hnd != o.m_Hnd)
        {
            Drop();
            m_Hnd = o.m_Hnd;
            Grab();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Status assignment operator.
    */
    StmtHnd & operator = (Int32 status)
    {
        if (m_Hnd)
            m_Hnd->mStatus = status;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two statement handles.
    */
    bool operator == (const StmtHnd & o) const
    {
        return (m_Hnd == o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two statement handles.
    */
    bool operator != (const StmtHnd & o) const
    {
        return (m_Hnd != o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison with an integer value status.
    */
    bool operator == (Int32 status) const
    {
        if (m_Hnd)
            return (m_Hnd->mStatus == status);
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison with an integer value status.
    */
    bool operator != (Int32 status) const
    {
        if (m_Hnd)
            return (m_Hnd->mStatus != status);
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return m_Hnd && m_Hnd->mPtr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer ()
    {
        return m_Hnd ? m_Hnd->mPtr : NULL;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer () const
    {
        return m_Hnd ? m_Hnd->mPtr : NULL;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Reference ()
    {
        assert(m_Hnd && m_Hnd->mPtr);
        return *(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator ConstRef () const
    {
        assert(m_Hnd && m_Hnd->mPtr);
        return *(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Handle * operator -> () const
    {
        assert(m_Hnd);
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Handle & operator * () const
    {
        assert(m_Hnd);
        return *m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr()
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr() const
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to the managed instance.
    */
    Counter Count() const
    {
        assert(m_Hnd);
        return m_Hnd ? m_Hnd->mRef : 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CCStr ErrStr() const
    {
        assert(m_Hnd); // SQLite does it's null pointer validations internally
        return m_Hnd->mConn.ErrStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CCStr ErrMsg() const
    {
        assert(m_Hnd); // SQLite does it's null pointer validations internally
        return m_Hnd->mConn.ErrMsg();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 ErrNo() const
    {
        assert(m_Hnd); // SQLite does it's null pointer validations internally
        return m_Hnd->mConn.ErrNo();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 ExErrNo() const
    {
        assert(m_Hnd); // SQLite does it's null pointer validations internally
        return m_Hnd->mConn.ExErrNo();
    }
};

/* ------------------------------------------------------------------------------------------------
 * Retrieve the string representation of a certain status code.
*/
CSStr GetErrStr(Int32 status);

/* ------------------------------------------------------------------------------------------------
 * Set a specific heap limit.
*/
void SetSoftHeapLimit(Int32 limit);

/* ------------------------------------------------------------------------------------------------
 * Release the specified amount of memory.
*/
Int32 ReleaseMemory(Int32 bytes);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the current memory usage.
*/
Object GetMemoryUsage();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the memory high watermark.
*/
Object GetMemoryHighwaterMark(bool reset);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the escaped version of the specified string.
*/
CSStr EscapeString(CSStr str);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the escaped version of the specified string using the supplied format specifier.
*/
CCStr EscapeStringEx(SQChar spec, CCStr str);

/* ------------------------------------------------------------------------------------------------
 * Convert the values from the specified array to a list of column names string.
*/
CCStr ArrayToQueryColumns(Array & arr);

/* ------------------------------------------------------------------------------------------------
 * Convert the keys from the specified array to a list of column names string.
*/
CCStr TableToQueryColumns(Table & tbl);

} // Namespace:: SqMod

#endif // _SQSQLITE_COMMON_HPP_
