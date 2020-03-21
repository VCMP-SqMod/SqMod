#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/SQLite/Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * The structure that holds the data associated with a certain connection.
*/
struct ConnHnd
{
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
    typedef std::vector< String > QueryList; // Container used to queue queries.

public:

    // --------------------------------------------------------------------------------------------
    Pointer     mPtr; // The connection handle resource.

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

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ConnHnd();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    ConnHnd(const ConnHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    ConnHnd(ConnHnd && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ConnHnd();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    ConnHnd & operator = (const ConnHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    ConnHnd & operator = (ConnHnd && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Create the database connection resource.
    */
    void Create(CSStr name, Int32 flags, CSStr vfs);

    /* --------------------------------------------------------------------------------------------
     * Execute a specific amount of queries from the queue.
    */
    Int32 Flush(Uint32 num, Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CCStr ErrStr() const
    {
        return sqlite3_errstr(sqlite3_errcode(mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CCStr ErrMsg() const
    {
        return sqlite3_errmsg(mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 ErrNo() const
    {
        return sqlite3_errcode(mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 ExErrNo() const
    {
        return sqlite3_extended_errcode(mPtr);
    }
};

/* ------------------------------------------------------------------------------------------------
 * The structure that holds the data associated with a certain statement.
*/
struct StmtHnd
{
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
    typedef std::map< String, int > Indexes; // Container used to identify column indexes.

public:

    // --------------------------------------------------------------------------------------------
    Pointer     mPtr; // The statement handle resource.

    // --------------------------------------------------------------------------------------------
    Int32       mStatus; // The last status code of this connection handle.

    // --------------------------------------------------------------------------------------------
    ConnRef     mConn; // The handle to the associated database connection.

    // --------------------------------------------------------------------------------------------
    String      mQuery; // The query string used to create this statement.

    // --------------------------------------------------------------------------------------------
    Int32       mColumns; // The amount of columns available in this statement.
    Int32       mParameters; // The amount of parameters available in this statement.
    Indexes     mIndexes; // An associative container with column names and their index.

    // --------------------------------------------------------------------------------------------
    bool        mGood; // True when a row has been fetched with step.
    bool        mDone; // True when the last step had no more rows to fetch.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    StmtHnd(ConnRef conn);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StmtHnd(const StmtHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    StmtHnd(StmtHnd && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StmtHnd();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    StmtHnd & operator = (const StmtHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    StmtHnd & operator = (StmtHnd && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Create the database statement resource.
    */
    void Create(CSStr query, SQInteger length = -1);

    /* --------------------------------------------------------------------------------------------
     * Check whether a specific column index is in range.
    */
    bool CheckColumn(Int32 idx) const
    {
        return (idx >= 0) && (idx < mColumns);
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether a specific parameter index is in range.
    */
    bool CheckParameter(Int32 idx) const
    {
        return (idx >= 1) && (idx <= mParameters);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column index associated with the specified name.
    */
    Int32 GetColumnIndex(CSStr name, SQInteger length = -1);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CCStr ErrStr() const;

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CCStr ErrMsg() const;

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 ErrNo() const;

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 ExErrNo() const;
};

} // Namespace:: SqMod
