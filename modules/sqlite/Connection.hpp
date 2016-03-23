#ifndef _SQSQLITE_CONNECTION_HPP_
#define _SQSQLITE_CONNECTION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "SqAPI.h"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class used to manage a connection to an SQLite database.
*/
class Connection
{
protected:

    /* --------------------------------------------------------------------------------------------
     * Create the database connection resource.
    */
    void Create(CSStr name, Int32 flags, CSStr vfs);

    /* --------------------------------------------------------------------------------------------
     * Validate the document reference and throw an error if invalid.
    */
    void Validate() const;

private:

    // --------------------------------------------------------------------------------------------
    ConnHnd m_Handle; /* The handle to the managed database connection resource. */

public:

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    Connection();

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    Connection(CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    Connection(CSStr name, Int32 flags);

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    Connection(CSStr name, Int32 flags, CSStr vfs);

    /* --------------------------------------------------------------------------------------------
     * Direct handle constructor.
    */
    Connection(const ConnHnd & h)
        : m_Handle(h)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Connection(const Connection & o)
        : m_Handle(o.m_Handle)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Connection()
    {
        /* Let the reference manager destroy the connection when necessary. */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Connection & operator = (const Connection & o)
    {
        m_Handle = o.m_Handle;
        return *this;
    }

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
    operator sqlite3 * ()
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw connection handle.
    */
    operator sqlite3 * () const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Connection & o) const
    {
        if (m_Handle == m_Handle)
            return 0;
        else if (m_Handle.m_Hnd > o.m_Handle.m_Hnd)
            return 1;
        else
            return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return m_Handle->mName.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this connection is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated connection handle.
    */
    const ConnHnd & GetHandle() const
    {
        return m_Handle;
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
        m_Handle.Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received status code.
    */
    Int32 GetStatus() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return m_Handle->mStatus;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the flags used to create this database connection.
    */
    Int32 GetFlags() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return m_Handle->mFlags;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name used to create this database connection.
    */
    CSStr GetName() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return m_Handle->mName.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the virtual file system used to create this database connection.
    */
    CSStr GetVFS() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return m_Handle->mVFS.c_str();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 GetErrorCode() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return m_Handle.ErrNo();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 GetExtendedErrorCode() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return m_Handle.ExErrNo();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CSStr GetErrStr() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return m_Handle.ErrStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CSStr GetErrMsg() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return m_Handle.ErrMsg();
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(CSStr name)
    {
        if (m_Handle.m_Hnd)
            m_Handle->Create(name, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(CSStr name, Int32 flags)
    {
        if (m_Handle.m_Hnd)
            m_Handle->Create(name, flags, NULL);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to open the specified database.
    */
    void Open(CSStr name, Int32 flags, CSStr vfs)
    {
        if (m_Handle.m_Hnd)
            m_Handle->Create(name, flags, vfs);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    Int32 Exec(CSStr str);

    /* --------------------------------------------------------------------------------------------
     * Attempt to queue the specified query.
    */
    void Queue(CSStr str);

    /* --------------------------------------------------------------------------------------------
     * Attempt to create a statement from the specified query.
    */
    Object Query(CSStr str) const;

    /* --------------------------------------------------------------------------------------------
     * See if the database connection was opened in read-only mode.
    */
    bool IsReadOnly() const;

    /* --------------------------------------------------------------------------------------------
     * Shortcut to test if a table exists.
    */
    bool TableExists(CCStr name) const;

    /* --------------------------------------------------------------------------------------------
     * See if the database connection is or is not in auto-commit mode.
    */
    bool GetAutoCommit() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return sqlite3_get_autocommit(m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Get the row-id of the most recent successful INSERT into the database from the current connection.
    */
    Object GetLastInsertRowID() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the number of database rows that were changed, inserted or deleted
     * by the most recently completed SQL statement.
    */
    Int32 GetChanges() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return sqlite3_changes(m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the total number of row changes caused by INSERT, UPDATE or DELETE statements
     * since the database connection was opened.
    */
    Int32 GetTotalChanges() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return sqlite3_total_changes(m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * See if this database connection has tracing enabled.
    */
    bool GetTracing() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return m_Handle->mTrace;
    }

    /* --------------------------------------------------------------------------------------------
     * Activate or deactivate tracing on this database connection.
    */
    void SetTracing(bool toggle)
    {
        // Validate the handle
        Validate();
        // Check whether changes are necessary
        if (m_Handle->mTrace == toggle)
            return; /* No point in proceeding */
        // Do we have to disable it?
        else if (m_Handle->mTrace)
            sqlite3_trace(m_Handle, NULL, NULL);
        // Go ahead and enable tracing
        else
            sqlite3_trace(m_Handle, &Connection::TraceOutput, NULL);
    }

    /* --------------------------------------------------------------------------------------------
     * See if this database connection has profiling enabled.
    */
    bool GetProfiling() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return m_Handle->mProfile;
    }

    /* --------------------------------------------------------------------------------------------
     * Activate or deactivate profiling on this database connection.
    */
    void SetProfiling(bool toggle)
    {
        // Validate the handle
        Validate();
        // Check whether changes are necessary
        if (m_Handle->mProfile == toggle)
            return; /* No point in proceeding */
        // Do we have to disable it?
        else if (m_Handle->mProfile)
            sqlite3_profile(m_Handle, NULL, NULL);
        // Go ahead and enable profiling
        else
            sqlite3_profile(m_Handle, &Connection::ProfileOutput, NULL);
    }

    /* --------------------------------------------------------------------------------------------
     * Set a busy handler that sleeps for a specified amount of time when a table is locked.
    */
    void SetBusyTimeout(Int32 millis);

    /* --------------------------------------------------------------------------------------------
     * Causes any pending database operation to abort and return at its earliest opportunity.
    */
    void InterruptOperation() const
    {
        // Validate the handle
        Validate();
        // Perform the requested action
        sqlite3_interrupt(m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempts to free as much heap memory as possible from the database connection.
    */
    void ReleaseMemory() const
    {
        // Validate the handle
        Validate();
        // Perform the requested action
        sqlite3_db_release_memory(m_Handle);
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
    Int32 GetInfo(Int32 operation, bool highwater = false)
    {
        return GetInfo(operation, highwater, false);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns internal runtime status information associated with the current database connection.
    */
    Int32 GetInfo(Int32 operation, bool highwater = false, bool reset = false);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of queries in the queue.
    */
    Uint32 QueueSize() const
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return (Uint32)m_Handle->mQueue.size();
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
        // Validate the handle
        Validate();
        // Perform the requested operation
        m_Handle->mQueue.shrink_to_fit();
    }

    /* --------------------------------------------------------------------------------------------
     * Remove all queries from the queue without executing them.
    */
    void ClearQueue()
    {
        // Validate the handle
        Validate();
        // Perform the requested operation
        m_Handle->mQueue.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Remove the last query from the queue.
    */
    void PopQueue()
    {
        // Validate the handle
        Validate();
        // Perform the requested action
        if (!m_Handle->mQueue.empty())
            m_Handle->mQueue.pop_back();
    }

    /* --------------------------------------------------------------------------------------------
     * Flush all queries from the queue.
    */
    Int32 Flush()
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return Flush(m_Handle->mQueue.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Flush a specific amount of queries from the queue.
    */
    Int32 Flush(Uint32 num);

    /* --------------------------------------------------------------------------------------------
     * Flush all queries from the queue and handle errors manually.
    */
    Int32 Flush(Object & env, Function & func)
    {
        // Validate the handle
        Validate();
        // Return the requested information
        return Flush(m_Handle->mQueue.size(), env, func);
    }

    /* --------------------------------------------------------------------------------------------
     * Flush a specific amount of queries from the queue and handle errors manually.
    */
    Int32 Flush(Uint32 num, Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    static SQInteger ExecF(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attempt to queue the specified query.
    */
    static SQInteger QueueF(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attempt to create a statement from the specified query.
    */
    static SQInteger QueryF(HSQUIRRELVM vm);

protected:

    /* --------------------------------------------------------------------------------------------
     * Callback function for ActivateTracing()
    */
    static void TraceOutput(void * ptr, const char * sql);

    /* --------------------------------------------------------------------------------------------
     * Callback function for ActivateProfiling()
    */
    static void ProfileOutput(void * ptr, const char * sql, sqlite3_uint64 time);
};

} // Namespace:: SqMod

#endif // _SQSQLITE_CONNECTION_HPP_
