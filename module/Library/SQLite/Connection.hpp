#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/SQLite/Handle.hpp"

// ------------------------------------------------------------------------------------------------
#include <utility>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Used to manage and interact with a database connection.
*/
class Connection
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
    Connection()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    explicit Connection(StackStrF & name)
        : m_Handle(new ConnHnd())
    {
        // Effing signed/unsigned warnings everywhere. I just need it to shut up.
        constexpr unsigned OPEN_READWRITE_F = SQLITE_OPEN_READWRITE;
        constexpr unsigned OPEN_CREATE_F = SQLITE_OPEN_CREATE;
        SQMOD_GET_VALID(*this)->Create(name.mPtr, OPEN_READWRITE_F | OPEN_CREATE_F, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    Connection(StackStrF & name, Int32 flags)
        : m_Handle(new ConnHnd())
    {
        SQMOD_GET_VALID(*this)->Create(name.mPtr, flags, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    Connection(StackStrF & name, Int32 flags, StackStrF & vfs)
        : m_Handle(new ConnHnd())
    {
        SQMOD_GET_VALID(*this)->Create(name.mPtr, flags, vfs.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Direct handle constructor.
    */
    explicit Connection(ConnRef c)
        : m_Handle(std::move(c))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Connection(const Connection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Connection(Connection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Connection & operator = (const Connection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Connection & operator = (Connection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two connections.
    */
    bool operator == (const Connection & o) const
    {
        return (m_Handle == o.m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two connections.
    */
    bool operator != (const Connection & o) const
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

} // Namespace:: SqMod
