#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include "Library/Numeric/LongInt.hpp"
#include "Library/Utils/Buffer.hpp"
#include "Library/Chrono/Date.hpp"
#include "Library/Chrono/Datetime.hpp"
#include "Library/Chrono/Time.hpp"
#include "Library/Chrono/Timestamp.hpp"

// ------------------------------------------------------------------------------------------------
#include <utility>
#include <vector>
#include <map>

// ------------------------------------------------------------------------------------------------
#include <sqlite3.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class SQLiteConnection;
class SQLiteStatement;
class SQLiteParameter;
class SQLiteColumn;
class SQLiteTransaction;

/* ------------------------------------------------------------------------------------------------
 * Handle validation.
*/
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    #define SQMOD_THROW_CURRENT(x, a)       (x).ThrowCurrent(a, __FILE__, __LINE__)
    #define SQMOD_VALIDATE(x)               (x).Validate(__FILE__, __LINE__)
    #define SQMOD_VALIDATE_CREATED(x)       (x).ValidateCreated(__FILE__, __LINE__)
    #define SQMOD_VALIDATE_PARAM(x, i)      (x).ValidateParam((i), __FILE__, __LINE__)
    #define SQMOD_VALIDATE_COLUMN(x, i)     (x).ValidateColumn((i), __FILE__, __LINE__)
    #define SQMOD_VALIDATE_ROW(x)           (x).ValidateRow(__FILE__, __LINE__)
    #define SQMOD_GET_VALID(x)              (x).GetValid(__FILE__, __LINE__)
    #define SQMOD_GET_CREATED(x)            (x).GetCreated(__FILE__, __LINE__)
#else
    #define SQMOD_THROW_CURRENT(x, a)       (x).ThrowCurrent(a)
    #define SQMOD_VALIDATE(x)               (x).Validate()
    #define SQMOD_VALIDATE_CREATED(x)       (x).ValidateCreated()
    #define SQMOD_VALIDATE_PARAM(x, i)      (x).ValidateParam((i))
    #define SQMOD_VALIDATE_COLUMN(x, i)     (x).ValidateColumn((i))
    #define SQMOD_VALIDATE_ROW(x)           (x).ValidateRow()
    #define SQMOD_GET_VALID(x)              (x).GetValid()
    #define SQMOD_GET_CREATED(x)            (x).GetCreated()
#endif // _DEBUG

/* ------------------------------------------------------------------------------------------------
 * Helper macros for architecture differences.
*/

#ifdef _SQ64
    #define sqlite3_bind_integer    sqlite3_bind_int64
    #define sqlite3_column_integer  sqlite3_column_int64
#else
    #define sqlite3_bind_integer    sqlite3_bind_int
    #define sqlite3_column_integer  sqlite3_column_int
#endif

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
struct SQLiteConnHnd;
struct SQLiteStmtHnd;

/* ------------------------------------------------------------------------------------------------
 * Common typedefs.
*/
typedef SharedPtr< SQLiteConnHnd > ConnRef;
typedef SharedPtr< SQLiteStmtHnd > StmtRef;

/* ------------------------------------------------------------------------------------------------
 * Obtain a script object from a connection handle. (meant to avoid having to include the header)
*/
Object GetConnectionObj(const ConnRef & conn);

/* ------------------------------------------------------------------------------------------------
 * Obtain a script object from a statement handle. (meant to avoid having to include the header)
*/
Object GetStatementObj(const StmtRef & stmt);

/* ------------------------------------------------------------------------------------------------
 * Tests if a certain query string is empty.
*/
bool IsQueryEmpty(CSStr str);

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
CSStr EscapeString(StackStrF & str);

/* ------------------------------------------------------------------------------------------------
 * Retrieve the escaped version of the specified string using the supplied format specifier.
*/
CCStr EscapeStringEx(SQChar spec, StackStrF & str);

/* ------------------------------------------------------------------------------------------------
 * Convert the values from the specified array to a list of column names string.
*/
CCStr ArrayToQueryColumns(Array & arr);

/* ------------------------------------------------------------------------------------------------
 * Convert the keys from the specified array to a list of column names string.
*/
CCStr TableToQueryColumns(Table & tbl);

/* ------------------------------------------------------------------------------------------------
 * The structure that holds the data associated with a certain connection.
*/
struct SQLiteConnHnd
{
public:

    // --------------------------------------------------------------------------------------------
    typedef sqlite3         Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     SQ_UNUSED_TYPEDEF(ConstPtr); // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     SQ_UNUSED_TYPEDEF(ConstRef); // Constant reference to the managed type.

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
    SQLiteConnHnd();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    SQLiteConnHnd(const SQLiteConnHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    SQLiteConnHnd(SQLiteConnHnd && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SQLiteConnHnd();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    SQLiteConnHnd & operator = (const SQLiteConnHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    SQLiteConnHnd & operator = (SQLiteConnHnd && o) = delete;

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
struct SQLiteStmtHnd
{
public:

    // --------------------------------------------------------------------------------------------
    typedef sqlite3_stmt    Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     SQ_UNUSED_TYPEDEF(ConstPtr); // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     SQ_UNUSED_TYPEDEF(ConstRef); // Constant reference to the managed type.

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
    explicit SQLiteStmtHnd(ConnRef conn);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    SQLiteStmtHnd(const SQLiteStmtHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    SQLiteStmtHnd(SQLiteStmtHnd && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SQLiteStmtHnd();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    SQLiteStmtHnd & operator = (const SQLiteStmtHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    SQLiteStmtHnd & operator = (SQLiteStmtHnd && o) = delete;

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

/* ------------------------------------------------------------------------------------------------
 * Used to manage and interact with a database connection.
*/
class SQLiteConnection
{
private:

    // --------------------------------------------------------------------------------------------
    ConnRef m_Handle; // Reference to the managed connection.

protected:

    /* --------------------------------------------------------------------------------------------
     * Callback function for ActivateTracing()
    */
    static void TraceOutput(void * ptr, CCStr sql);

    /* --------------------------------------------------------------------------------------------
     * Callback function for ActivateProfiling()
    */
    static void ProfileOutput(void * ptr, CCStr sql, sqlite3_uint64 time);

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(CCStr file, Int32 line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ConnRef & GetValid(CCStr file, Int32 line) const;
#else
    const ConnRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ConnRef & GetCreated(CCStr file, Int32 line) const;
#else
    const ConnRef & GetCreated() const;
#endif // _DEBUG

public:

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    SQLiteConnection()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit SQLiteConnection(StackStrF & name)
        : m_Handle(new SQLiteConnHnd())
    {
        // Effing signed/unsigned warnings everywhere. I just need it to shut up.
        constexpr unsigned OPEN_READWRITE_F = SQLITE_OPEN_READWRITE;
        constexpr unsigned OPEN_CREATE_F = SQLITE_OPEN_CREATE;
        SQMOD_GET_VALID(*this)->Create(name.mPtr, OPEN_READWRITE_F | OPEN_CREATE_F, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    SQLiteConnection(StackStrF & name, Int32 flags)
        : m_Handle(new SQLiteConnHnd())
    {
        SQMOD_GET_VALID(*this)->Create(name.mPtr, flags, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    SQLiteConnection(StackStrF & name, Int32 flags, StackStrF & vfs)
        : m_Handle(new SQLiteConnHnd())
    {
        SQMOD_GET_VALID(*this)->Create(name.mPtr, flags, vfs.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Direct handle constructor.
    */
    explicit SQLiteConnection(ConnRef c)
        : m_Handle(std::move(c))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SQLiteConnection(const SQLiteConnection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SQLiteConnection(SQLiteConnection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    SQLiteConnection & operator = (const SQLiteConnection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SQLiteConnection & operator = (SQLiteConnection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two connections.
    */
    bool operator == (const SQLiteConnection & o) const
    {
        return (m_Handle == o.m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two connections.
    */
    bool operator != (const SQLiteConnection & o) const
    {
        return (m_Handle != o.m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw connection handle.
    */
    operator sqlite3 * () //NOLINT (intentionally implicit)
    {
        return m_Handle ? m_Handle->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw connection handle.
    */
    operator sqlite3 * () const //NOLINT (intentionally implicit)
    {
        return m_Handle ? m_Handle->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const String & ToString() const
    {
        return m_Handle ? m_Handle->mName : NullString();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated connection handle.
    */
    const ConnRef & GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed connection handle is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed connection handle was connected.
    */
    bool IsConnected() const
    {
        return m_Handle && (m_Handle->mPtr != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this connection handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the associated database connection.
    */
    void Release()
    {
        m_Handle.Reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received status code.
    */
    Int32 GetStatus() const
    {
        return SQMOD_GET_VALID(*this)->mStatus;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the flags used to create this database connection.
    */
    Int32 GetFlags() const
    {
        return SQMOD_GET_VALID(*this)->mFlags;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name used to create this database connection.
    */
    const String & GetName() const
    {
        return SQMOD_GET_VALID(*this)->mName;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the virtual file system used to create this database connection.
    */
    const String & GetVFS() const
    {
        return SQMOD_GET_VALID(*this)->mVFS;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 GetErrorCode() const
    {
        return SQMOD_GET_VALID(*this)->ErrNo();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 GetExtendedErrorCode() const
    {
        return SQMOD_GET_VALID(*this)->ExErrNo();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CSStr GetErrStr() const
    {
        return SQMOD_GET_VALID(*this)->ErrStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CSStr GetErrMsg() const
    {
        return SQMOD_GET_VALID(*this)->ErrMsg();
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(StackStrF & name, Int32 flags);

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(StackStrF & name, Int32 flags, StackStrF & vfs);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    Int32 Exec(StackStrF & str);

    /* --------------------------------------------------------------------------------------------
     * Attempt to queue the specified query.
    */
    void Queue(StackStrF & str);

    /* --------------------------------------------------------------------------------------------
     * Attempt to create a statement from the specified query.
    */
    Object Query(StackStrF & str) const;

    /* --------------------------------------------------------------------------------------------
     * See if the database connection was opened in read-only mode.
    */
    bool IsReadOnly() const;

    /* --------------------------------------------------------------------------------------------
     * Shortcut to test if a table exists.
    */
    bool TableExists(StackStrF & name) const;

    /* --------------------------------------------------------------------------------------------
     * See if the database connection is or is not in auto-commit mode.
    */
    bool GetAutoCommit() const
    {
        return sqlite3_get_autocommit(SQMOD_GET_CREATED(*this)->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Get the row-id of the most recent successful INSERT into the database from the current connection.
    */
    Object GetLastInsertRowID() const
    {
        return Object(new SLongInt(sqlite3_last_insert_rowid(SQMOD_GET_CREATED(*this)->mPtr)));
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the number of database rows that were changed, inserted or deleted
     * by the most recently completed SQL statement.
    */
    Int32 GetChanges() const
    {
        return sqlite3_changes(SQMOD_GET_CREATED(*this)->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the total number of row changes caused by INSERT, UPDATE or DELETE statements
     * since the database connection was opened.
    */
    Int32 GetTotalChanges() const
    {
        return sqlite3_total_changes(SQMOD_GET_CREATED(*this)->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * See if this database connection has tracing enabled.
    */
    bool GetTracing() const
    {
        return SQMOD_GET_VALID(*this)->mTrace;
    }

    /* --------------------------------------------------------------------------------------------
     * Activate or deactivate tracing on this database connection.
    */
    void SetTracing(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * See if this database connection has profiling enabled.
    */
    bool GetProfiling() const
    {
        return SQMOD_GET_VALID(*this)->mProfile;
    }

    /* --------------------------------------------------------------------------------------------
     * Activate or deactivate profiling on this database connection.
    */
    void SetProfiling(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Set a busy handler that sleeps for a specified amount of time when a table is locked.
    */
    void SetBusyTimeout(Int32 millis);

    /* --------------------------------------------------------------------------------------------
     * Causes any pending database operation to abort and return at its earliest opportunity.
    */
    void InterruptOperation() const
    {
        sqlite3_interrupt(SQMOD_GET_CREATED(*this)->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempts to free as much heap memory as possible from the database connection.
    */
    void ReleaseMemory() const
    {
        sqlite3_db_release_memory(SQMOD_GET_CREATED(*this)->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns internal runtime status information associated with the current database connection.
    */
    Int32 GetInfo(Int32 operation)
    {
        return GetInfo(operation, false, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns internal runtime status information associated with the current database connection.
    */
    Int32 GetInfo(Int32 operation, bool highwater)
    {
        return GetInfo(operation, highwater, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns internal runtime status information associated with the current database connection.
    */
    Int32 GetInfo(Int32 operation, bool highwater, bool reset);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of queries in the queue.
    */
    Uint32 QueueSize() const
    {
        return ConvTo< Uint32 >::From(SQMOD_GET_VALID(*this)->mQueue.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Reserve space upfront for the specified amount of queries in the query queue.
    */
    void ReserveQueue(Uint32 num);

    /* --------------------------------------------------------------------------------------------
     * Release memory that is not occupied from the query queue.
    */
    void CompactQueue()
    {
        SQMOD_GET_VALID(*this)->mQueue.shrink_to_fit();
    }

    /* --------------------------------------------------------------------------------------------
     * Remove all queries from the queue without executing them.
    */
    void ClearQueue()
    {
        SQMOD_GET_VALID(*this)->mQueue.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Remove the last query from the queue.
    */
    void PopQueue();

    /* --------------------------------------------------------------------------------------------
     * Flush all queries from the queue.
    */
    Int32 Flush();

    /* --------------------------------------------------------------------------------------------
     * Flush a specific amount of queries from the queue.
    */
    Int32 Flush(SQInteger num);

    /* --------------------------------------------------------------------------------------------
     * Flush all queries from the queue and handle errors manually.
    */
    Int32 Flush(Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Flush a specific amount of queries from the queue and handle errors manually.
    */
    Int32 Flush(SQInteger num, Object & env, Function & func);
};

/* ------------------------------------------------------------------------------------------------
 * Used to manage and interact with parameters from a database statement.
*/
class SQLiteParameter
{
    // --------------------------------------------------------------------------------------------
    friend class SQLiteStatement;

private:

    // --------------------------------------------------------------------------------------------
    Int32       m_Index; // The index of the managed parameter.
    StmtRef     m_Handle; // Reference to the managed statement.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(CCStr file, Int32 line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetValid(CCStr file, Int32 line) const;
#else
    const StmtRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetCreated(CCStr file, Int32 line) const;
#else
    const StmtRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and parameter index, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateParam(Int32 idx, CCStr file, Int32 line) const;
#else
    void ValidateParam(Int32 idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(Int32 idx)
    {
        // Assign the index with a failsafe to invalid on error
        AutoAssign< Int32 > aa(m_Index, -1, idx);
        // Validate the obtained parameter index
        SQMOD_VALIDATE_PARAM(*this, idx);
        // Don't fall back to the invalid index anymore
        aa.Set(idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(CSStr name)
    {
        SetIndex(sqlite3_bind_parameter_index(SQMOD_GET_CREATED(*this)->mPtr, name));
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(const Object & param);

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    SQLiteParameter()
        : m_Index(0), m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * No parameter constructor.
    */
    explicit SQLiteParameter(StmtRef stmt)
        : m_Index(0), m_Handle(std::move(stmt))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Index constructor.
    */
    SQLiteParameter(StmtRef stmt, Int32 idx)
        : m_Index(idx), m_Handle(std::move(stmt))
    {
        SQMOD_VALIDATE_PARAM(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Name constructor.
    */
    SQLiteParameter(const StmtRef & stmt, CSStr name)
        : m_Index(stmt ? sqlite3_bind_parameter_index(stmt->mPtr, name) : 0), m_Handle(stmt)
    {
        SQMOD_VALIDATE_PARAM(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Dynamic constructor.
    */
    SQLiteParameter(StmtRef stmt, const Object & param)
        : m_Index(0), m_Handle(std::move(stmt))
    {
        if (!m_Handle)
        {
            STHROWF("Invalid SQLite statement reference");
        }
        // Extract the index
        SetIndex(param);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SQLiteParameter(const SQLiteParameter & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SQLiteParameter(SQLiteParameter && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    SQLiteParameter & operator = (const SQLiteParameter & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SQLiteParameter & operator = (SQLiteParameter && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two parameter indexes.
    */
    bool operator == (const SQLiteParameter & o) const
    {
        return (m_Index == o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two parameter indexes.
    */
    bool operator != (const SQLiteParameter & o) const
    {
        return (m_Index != o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        return m_Index >= 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        CSStr val = nullptr;
        // Can we attempt to return the parameter name?
        if (m_Handle && m_Index)
        {
            val = sqlite3_bind_parameter_name(m_Handle->mPtr, m_Index);
        }
        else
        {
            val = ToStrF(_SC("%d"), m_Index);
        }
        // Return the value if valid
        return val ? val : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * See whether this statement is valid.
    */
    bool IsValid() const
    {
        return m_Handle; // An invalid statement means an invalid parameter
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this statement handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated parameter index.
    */
    Int32 GetIndex() const
    {
        return m_Index;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated database statement.
    */
    Object GetStatement() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated database connection.
    */
    Object GetConnection() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the referenced parameter.
    */
    CSStr GetName() const
    {
        return sqlite3_bind_parameter_name(SQMOD_GET_CREATED(*this)->mPtr, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the associated database statement and parameter index.
    */
    void Release()
    {
        m_Handle.Reset();
        m_Index = 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a dynamic value at the referenced parameter index.
    */
    void SetValue(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a boolean value at the referenced parameter index.
    */
    void SetBool(bool value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a character value at the referenced parameter index.
    */
    void SetChar(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a native integer value at the referenced parameter index.
    */
    void SetInteger(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 8 bit integer value at the referenced parameter index.
    */
    void SetInt8(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 8 bit integer value at the referenced parameter index.
    */
    void SetUint8(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 16 bit integer value at the referenced parameter index.
    */
    void SetInt16(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 16 bit integer value at the referenced parameter index.
    */
    void SetUint16(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 32 bit integer value at the referenced parameter index.
    */
    void SetInt32(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 32 bit integer value at the referenced parameter index.
    */
    void SetUint32(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 64 bit integer value at the referenced parameter index.
    */
    void SetInt64(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 64 bit integer value at the referenced parameter index.
    */
    void SetUint64(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a native floating point value at the referenced parameter index.
    */
    void SetFloat(SQFloat value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a 32 bit floating point value at the referenced parameter index.
    */
    void SetFloat32(SQFloat value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a 64 bit floating point value at the referenced parameter index.
    */
    void SetFloat64(SQFloat value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a string value at the referenced parameter index.
    */
    void SetString(StackStrF & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a string value at the referenced parameter index.
    */
    void SetStringRaw(CSStr value, SQInteger length = -1);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a zeroed blob value at the referenced parameter index.
    */
    void SetZeroBlob(SQInteger size);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a blob value at the referenced parameter index.
    */
    void SetBlob(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a buffer value at the referenced parameter index.
    */
    void SetData(const SqBuffer & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a buffer value at the referenced parameter index.
    */
    void SetDataEx(const SqBuffer & value, SQInteger offset, SQInteger length);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date value at the referenced parameter index.
    */
    void SetDate(const Date & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date value at the referenced parameter index.
    */
    void SetDateEx(SQInteger year, SQInteger month, SQInteger day);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a time value at the referenced parameter index.
    */
    void SetTime(const Time & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a time value at the referenced parameter index.
    */
    void SetTimeEx(SQInteger hour, SQInteger minute, SQInteger second);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date-time value at the referenced parameter index.
    */
    void SetDatetime(const Datetime & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date-time value at the referenced parameter index.
    */
    void SetDatetimeEx(SQInteger year, SQInteger month, SQInteger day, SQInteger hour, SQInteger minute, SQInteger second);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the current timestamp at the referenced parameter index.
    */
    void SetNow();

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a null value at the referenced parameter index.
    */
    void SetNull();
};

/* ------------------------------------------------------------------------------------------------
 * Used to manage and interact with statement columns.
*/
class SQLiteColumn
{
    // --------------------------------------------------------------------------------------------
    friend class SQLiteStatement;

private:

    // --------------------------------------------------------------------------------------------
    Int32       m_Index; // The index of the managed column.
    StmtRef     m_Handle; // The statement where the column exist.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(CCStr file, Int32 line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetValid(CCStr file, Int32 line) const;
#else
    const StmtRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetCreated(CCStr file, Int32 line) const;
#else
    const StmtRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and column index, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateColumn(Int32 idx, CCStr file, Int32 line) const;
#else
    void ValidateColumn(Int32 idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and row, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateRow(CCStr file, Int32 line) const;
#else
    void ValidateRow() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(Int32 idx)
    {
        // Assign the index with a failsafe to invalid on error
        AutoAssign< Int32 > aa(m_Index, -1, idx);
        // Validate the obtained column index
        SQMOD_VALIDATE_COLUMN(*this, idx);
        // Don't fall back to the invalid index anymore
        aa.Set(idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(CSStr name)
    {
        SetIndex(SQMOD_GET_CREATED(*this)->GetColumnIndex(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(const Object & column);

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    SQLiteColumn()
        : m_Index(-1), m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * No column constructor.
    */
    explicit SQLiteColumn(StmtRef stmt)
        : m_Index(-1), m_Handle(std::move(stmt))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Index constructor.
    */
    SQLiteColumn(StmtRef  stmt, Int32 idx)
        : m_Index(idx), m_Handle(std::move(stmt))
    {
        SQMOD_VALIDATE_COLUMN(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Name constructor.
    */
    SQLiteColumn(const StmtRef & stmt, CSStr name)
        : m_Index(stmt ? stmt->GetColumnIndex(name) : -1), m_Handle(stmt)
    {
        SQMOD_VALIDATE_COLUMN(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Dynamic constructor.
    */
    SQLiteColumn(StmtRef  stmt, const Object & column)
        : m_Index(-1), m_Handle(std::move(stmt))
    {
        if (!m_Handle)
        {
            STHROWF("Invalid SQLite statement reference");
        }
        // Extract the index
        SetIndex(column);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SQLiteColumn(const SQLiteColumn & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SQLiteColumn(SQLiteColumn && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    SQLiteColumn & operator = (const SQLiteColumn & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SQLiteColumn & operator = (SQLiteColumn && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two table column indexes.
    */
    bool operator == (const SQLiteColumn & o) const
    {
        return (m_Index == o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two table column indexes.
    */
    bool operator != (const SQLiteColumn & o) const
    {
        return (m_Index != o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        return m_Index >= 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        CSStr val = nullptr;
        // Can we attempt to return the parameter name?
        if (m_Handle && m_Index)
        {
            val = sqlite3_column_name(m_Handle->mPtr, m_Index);
        }
        else
        {
            val = ToStrF(_SC("%d"), m_Index);
        }
        // Return the value if valid
        return val ? val : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the column is valid.
    */
    bool IsValid() const
    {
        return m_Handle; // An invalid statement means an invalid column
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the associated statement handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced column index.
    */
    Int32 GetIndex() const
    {
        return m_Index;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced database statement.
    */
    Object GetStatement() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced database connection.
    */
    Object GetConnection() const;

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the referenced database statement and column index.
    */
    void Release()
    {
        m_Handle.Reset();
        m_Index = -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether the referenced column is null.
    */
    bool IsNull() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the referenced column index.
    */
    CSStr GetName() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column origin name if the library was compiled with such feature.
    */
    CSStr GetOriginName() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type identifier of the referenced column index.
    */
    Int32 GetType() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the size in bytes of the referenced column index.
    */
    Int32 GetBytes() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a dynamic type.
    */
    Object GetValue() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a numeric type.
    */
    Object GetNumber() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a native script integer.
    */
    SQInteger GetInteger() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a native script floating point.
    */
    SQFloat GetFloat() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a long integer.
    */
    Object GetLong() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a string.
    */
    Object GetString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a boolean.
    */
    bool GetBoolean() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a character.
    */
    SQChar GetChar() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a memory buffer.
    */
    Object GetBuffer() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a memory blob.
    */
    Object GetBlob() const;
};

/* ------------------------------------------------------------------------------------------------
 * Used to manage and interact a database statement.
*/
class SQLiteStatement
{
private:

    // --------------------------------------------------------------------------------------------
    StmtRef m_Handle; // Reference to the managed statement.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(CCStr file, Int32 line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetValid(CCStr file, Int32 line) const;
#else
    const StmtRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetCreated(CCStr file, Int32 line) const;
#else
    const StmtRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and column index, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateColumn(Int32 idx, CCStr file, Int32 line) const;
#else
    void ValidateColumn(Int32 idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and parameter index, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateParam(Int32 idx, CCStr file, Int32 line) const;
#else
    void ValidateParam(Int32 idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and row, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateRow(CCStr file, Int32 line) const;
#else
    void ValidateRow() const;
#endif // _DEBUG

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    SQLiteStatement()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Construct a statement under the specified connection using the specified string.
    */
    SQLiteStatement(const ConnRef & connection, StackStrF & query)
        : m_Handle(new SQLiteStmtHnd(connection))
    {
        SQMOD_GET_VALID(*this)->Create(query.mPtr, query.mLen);
    }

    /* --------------------------------------------------------------------------------------------
     * Construct a statement under the specified connection using the specified string.
    */
    SQLiteStatement(const SQLiteConnection & connection, StackStrF & query);

    /* --------------------------------------------------------------------------------------------
     * Direct handle constructor.
    */
    explicit SQLiteStatement(StmtRef  s)
        : m_Handle(std::move(s))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SQLiteStatement(const SQLiteStatement & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SQLiteStatement(SQLiteStatement && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    SQLiteStatement & operator = (const SQLiteStatement & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SQLiteStatement & operator = (SQLiteStatement && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two connections.
    */
    bool operator == (const SQLiteStatement & o) const
    {
        return (m_Handle.Get() == o.m_Handle.Get());
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two connections.
    */
    bool operator != (const SQLiteStatement & o) const
    {
        return (m_Handle.Get() != o.m_Handle.Get());
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw connection handle.
    */
    operator sqlite3_stmt * () // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        return m_Handle ? m_Handle->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw connection handle.
    */
    operator sqlite3_stmt * () const // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
    {
        return m_Handle ? m_Handle->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const String & ToString() const
    {
        return m_Handle ? m_Handle->mQuery : NullString();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated statement handle.
    */
    const StmtRef & GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed statement is valid.
    */
    bool IsPrepared() const
    {
        return m_Handle && (m_Handle->mPtr != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this statement handle.
    */
    Uint32 GetReferences() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated database connection.
    */
    Object GetConnection() const;

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the associated database statement.
    */
    void Release()
    {
        m_Handle.Reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received status code.
    */
    Int32 GetStatus() const
    {
        return SQMOD_GET_VALID(*this)->mStatus;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 GetErrorCode() const
    {
        return SQMOD_GET_VALID(*this)->ErrNo();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 GetExtendedErrorCode() const
    {
        return SQMOD_GET_VALID(*this)->ExErrNo();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CSStr GetErrStr() const
    {
        return SQMOD_GET_VALID(*this)->ErrStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CSStr GetErrMsg() const
    {
        return SQMOD_GET_VALID(*this)->ErrMsg();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of requested columns.
    */
    Int32 GetColumns() const
    {
        return SQMOD_GET_VALID(*this)->mColumns;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of specified parameters.
    */
    Int32 GetParameters() const
    {
        return SQMOD_GET_VALID(*this)->mParameters;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the query string used to create this statement.
    */
    const String & GetQuery() const
    {
        return SQMOD_GET_VALID(*this)->mQuery;
    }

    /* --------------------------------------------------------------------------------------------
     * See if the last step retrieved a valid row.
    */
    bool GetGood() const
    {
        return SQMOD_GET_CREATED(*this)->mGood;
    }

    /* --------------------------------------------------------------------------------------------
     * See if there are any steps left.
    */
    bool GetDone() const
    {
        return SQMOD_GET_CREATED(*this)->mDone;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether a specific parameter index is within range.
    */
    bool CheckParameter(Int32 idx) const
    {
        return SQMOD_GET_VALID(*this)->CheckParameter(idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the parameter index associated with the specified name.
    */
    Int32 GetParameterIndex(StackStrF & name) const
    {
        return sqlite3_bind_parameter_index(SQMOD_GET_VALID(*this)->mPtr, name.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the parameter name associated with the specified index.
    */
    CSStr GetParameterName(Int32 idx) const
    {
        // Validate the specified index
        if (!idx)
        {
            STHROWF("Invalid parameter index (%d)", idx);
        }
        // Attempt to locate the name at the specified index
        CSStr name = sqlite3_bind_parameter_name(SQMOD_GET_VALID(*this)->mPtr, idx);
        // Validate the obtained string
        if (!name)
        {
            STHROWF("No such parameter exists (%d)", idx);
        }
        // Return the obtained string
        return name;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether a specific column index is within range.
    */
    bool CheckColumn(Int32 idx) const
    {
        return SQMOD_GET_VALID(*this)->CheckColumn(idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of columns available in the current row.
    */
    Int32 GetDataCount() const
    {
        return sqlite3_data_count(SQMOD_GET_VALID(*this)->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether the specified column is null.
    */
    bool IsColumnNull(Int32 idx) const
    {
        return (sqlite3_column_type(SQMOD_GET_VALID(*this)->mPtr, idx) == SQLITE_NULL);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column index associated with the specified name.
    */
    Int32 GetColumnIndex(StackStrF & name) const
    {
        return SQMOD_GET_VALID(*this)->GetColumnIndex(name.mPtr, name.mLen);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column name associated with the specified index.
    */
    CSStr GetColumnName(Int32 idx) const
    {
        return sqlite3_column_name(SQMOD_GET_VALID(*this)->mPtr, idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column origin name if the library was compiled with such feature.
    */
    CSStr GetColumnOriginName(Int32 idx) const
    {
#ifdef SQLITE_ENABLE_COLUMN_METADATA
        return sqlite3_column_origin_name(SQMOD_GET_VALID(*this)->mPtr, idx);
#else
        // The compiler moans when extra warnings are enabled
        SQMOD_UNUSED_VAR(idx);
        // Stop the execution here!
        STHROWF("The module was compiled without this feature");
        // We have to return something
        return _SC("");
#endif
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type identifier of the column associated with the specified index.
    */
    Int32 GetColumnType(Int32 idx) const
    {
        return sqlite3_column_type(SQMOD_GET_VALID(*this)->mPtr, idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the size in bytes of the column associated with the specified index.
    */
    Int32 GetColumnBytes(Int32 idx) const
    {
        return sqlite3_column_bytes(SQMOD_GET_VALID(*this)->mPtr, idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Reset the statement back to its initial position to be stepped again.
    */
    SQLiteStatement & Reset();

    /* --------------------------------------------------------------------------------------------
     * Clear any values binded to this statement.
    */
    SQLiteStatement & Clear();

    /* --------------------------------------------------------------------------------------------
     * Execute this statement and don't expect any rows to be returned.
    */
    Int32 Exec();

    /* --------------------------------------------------------------------------------------------
     * Step the statement and expect a row to be returned.
    */
    bool Step();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the parameter with the specified name or index.
    */
    Object GetParameter(const Object & param) const
    {
        return Object(new SQLiteParameter(m_Handle, param));
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a dynamic value at the specified parameter index.
    */
    SQLiteStatement & SetValue(const Object & param, const Object & value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetValue(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a boolean value at the specified parameter index.
    */
    SQLiteStatement & SetBool(const Object & param, bool value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetBool(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a character value at the specified parameter index.
    */
    SQLiteStatement & SetChar(const Object & param, SQInteger value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetChar(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a native integer value at the specified parameter index.
    */
    SQLiteStatement & SetInteger(const Object & param, SQInteger value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetInteger(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 8 bit integer value at the specified parameter index.
    */
    SQLiteStatement & SetInt8(const Object & param, SQInteger value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetInt8(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 8 bit integer value at the specified parameter index.
    */
    SQLiteStatement & SetUint8(const Object & param, SQInteger value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetUint8(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 16 bit integer value at the specified parameter index.
    */
    SQLiteStatement & SetInt16(const Object & param, SQInteger value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetInt16(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 16 bit integer value at the specified parameter index.
    */
    SQLiteStatement & SetUint16(const Object & param, SQInteger value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetUint16(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 32 bit integer value at the specified parameter index.
    */
    SQLiteStatement & SetInt32(const Object & param, SQInteger value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetInt32(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 32 bit integer value at the specified parameter index.
    */
    SQLiteStatement & SetUint32(const Object & param, SQInteger value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetUint32(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 64 bit integer value at the specified parameter index.
    */
    SQLiteStatement & SetInt64(const Object & param, const Object & value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetInt64(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 64 bit integer value at the specified parameter index.
    */
    SQLiteStatement & SetUint64(const Object & param, const Object & value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetUint64(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a native floating point value at the specified parameter index.
    */
    SQLiteStatement & SetFloat(const Object & param, SQFloat value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetFloat(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a 32 bit floating point value at the specified parameter index.
    */
    SQLiteStatement & SetFloat32(const Object & param, SQFloat value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetFloat32(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a 64 bit floating point value at the specified parameter index.
    */
    SQLiteStatement & SetFloat64(const Object & param, SQFloat value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetFloat64(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a string value at the specified parameter index.
    */
    SQLiteStatement & SetString(const Object & param, StackStrF & value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetString(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a zeroed blob value at the specified parameter index.
    */
    SQLiteStatement & SetZeroBlob(const Object & param, SQInteger size)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetZeroBlob(size);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a blob value at the specified parameter index.
    */
    SQLiteStatement & SetBlob(const Object & param, const Object & value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetBlob(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a buffer value at the specified parameter index.
    */
    SQLiteStatement & SetData(const Object & param, const Object & value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetData(value.Cast< const SqBuffer & >());
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date value at the specified parameter index.
    */
    SQLiteStatement & SetDate(const Object & param, const Object & value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetDate(value.Cast< const Date & >());
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date value at the specified parameter index.
    */
    SQLiteStatement & SetDateEx(const Object & param, SQInteger year, SQInteger month, SQInteger day)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetDateEx(year, month, day);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a time value at the specified parameter index.
    */
    SQLiteStatement & SetTime(const Object & param, const Object & value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetTime(value.Cast< const Time & >());
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a time value at the specified parameter index.
    */
    SQLiteStatement & SetTimeEx(const Object & param, SQInteger hour, SQInteger minute, SQInteger second)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetTimeEx(hour, minute, second);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date-time value at the specified parameter index.
    */
    SQLiteStatement & SetDatetime(const Object & param, const Object & value)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetDatetime(value.Cast< const Datetime & >());
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date-time value at the specified parameter index.
    */
    SQLiteStatement & SetDatetimeEx(const Object & param, SQInteger year, SQInteger month, SQInteger day,
                                    SQInteger hour, SQInteger minute, SQInteger second)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetDatetimeEx(year, month, day, hour, minute, second);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the current timestamp at the specified parameter index.
    */
    SQLiteStatement & SetNow(const Object & param)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetNow();
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a null value at the specified parameter index.
    */
    SQLiteStatement & SetNull(const Object & param)
    {
        SQLiteParameter(SQMOD_GET_CREATED(*this), param).SetNull();
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the values from an array starting at the specified index.
    */
    SQLiteStatement & SetArray(Int32 idx, const Array & arr);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the values from an associative container.
    */
    SQLiteStatement & SetTable(const Table & tbl);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column with the specified name or index.
    */
    Object GetColumn(const Object & column) const
    {
        return Object(new SQLiteColumn(m_Handle, column));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a dynamic type.
    */
    Object GetValue(const Object & column) const
    {
        return SQLiteColumn(SQMOD_GET_CREATED(*this), column).GetValue();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a numeric type.
    */
    Object GetNumber(const Object & column) const
    {
        return SQLiteColumn(SQMOD_GET_CREATED(*this), column).GetNumber();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a native script integer.
    */
    SQInteger GetInteger(const Object & column) const
    {
        return SQLiteColumn(SQMOD_GET_CREATED(*this), column).GetInteger();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a native script floating point.
    */
    SQFloat GetFloat(const Object & column) const
    {
        return SQLiteColumn(SQMOD_GET_CREATED(*this), column).GetFloat();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a long integer.
    */
    Object GetLong(const Object & column) const
    {
        return SQLiteColumn(SQMOD_GET_CREATED(*this), column).GetLong();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a string.
    */
    Object GetString(const Object & column) const
    {
        return SQLiteColumn(SQMOD_GET_CREATED(*this), column).GetString();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a boolean.
    */
    bool GetBoolean(const Object & column) const
    {
        return SQLiteColumn(SQMOD_GET_CREATED(*this), column).GetBoolean();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a character.
    */
    SQChar GetChar(const Object & column) const
    {
        return SQLiteColumn(SQMOD_GET_CREATED(*this), column).GetChar();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a memory buffer.
    */
    Object GetBuffer(const Object & column) const
    {
        return SQLiteColumn(SQMOD_GET_CREATED(*this), column).GetBuffer();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a memory blob.
    */
    Object GetBlob(const Object & column) const
    {
        return SQLiteColumn(SQMOD_GET_CREATED(*this), column).GetBlob();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an array container.
    */
    Array GetArray() const
    {
        return GetArray(0, SQMOD_GET_CREATED(*this)->mColumns);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an array container.
    */
    Array GetArray(Int32 min) const
    {
        return GetArray(min, SQMOD_GET_CREATED(*this)->mColumns);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an array container.
    */
    Array GetArray(Int32 min, Int32 max) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an associative container.
    */
    Table GetTable() const
    {
        // Is there something to return?
        if (SQMOD_GET_CREATED(*this)->mColumns > 0)
        {
            return GetTable(0, m_Handle->mColumns - 1);
        }
        // Fallback to empty table
        return NullTable();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an associative container.
    */
    Table GetTable(Int32 min) const
    {
        // Is there something to return?
        if (SQMOD_GET_CREATED(*this)->mColumns > 0)
        {
            return GetTable(min, m_Handle->mColumns - 1);
        }
        // Fallback to empty table
        return NullTable();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an associative container.
    */
    Table GetTable(Int32 min, Int32 max) const;
};

/* ------------------------------------------------------------------------------------------------
 * Implements the RAII pattern for database transactions.
*/
class SQLiteTransaction
{
public:

    /* --------------------------------------------------------------------------------------------
     * Construct by taking the handle from a connection.
    */
    explicit SQLiteTransaction(const SQLiteConnection & db);

    /* --------------------------------------------------------------------------------------------
     * Construct using the direct connection handle.
    */
    explicit SQLiteTransaction(ConnRef db);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    SQLiteTransaction(const SQLiteTransaction & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    SQLiteTransaction(SQLiteTransaction && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SQLiteTransaction();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    SQLiteTransaction & operator = (const SQLiteTransaction & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    SQLiteTransaction & operator = (SQLiteTransaction && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const String & ToString() const
    {
        return m_Handle ? m_Handle->mName : NullString();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to commit changes to the database.
    */
    bool Commit();

    /* --------------------------------------------------------------------------------------------
     * See whether the change during this transaction were successfully committed.
    */
    bool Commited() const
    {
        return m_Committed;
    }

private:

    // --------------------------------------------------------------------------------------------
    ConnRef     m_Handle; // The database connection handle where the transaction began.
    bool        m_Committed; // Whether changes were successfully committed to the database.
};

} // Namespace:: SqMod
