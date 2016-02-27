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
    bool Validate() const;

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
        if (Validate())
            return m_Handle->mName.c_str();
        // Request failed
        return _SC("");
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
        if (Validate())
            return m_Handle->mStatus;
        // Request failed
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the flags used to create this database connection.
    */
    Int32 GetFlags() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle->mFlags;
        // Request failed
        return 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name used to create this database connection.
    */
    CSStr GetName() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle->mName.c_str();
        // Request failed
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the virtual file system used to create this database connection.
    */
    CSStr GetVFS() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle->mVFS.c_str();
        // Request failed
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 GetErrorCode() const
    {
        if (Validate())
            return m_Handle.ErrNo();
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 GetExtendedErrorCode() const
    {
        if (Validate())
            return m_Handle.ExErrNo();
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CSStr GetErrStr() const
    {
        if (Validate())
            return m_Handle.ErrStr();
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CSStr GetErrMsg() const
    {
        if (Validate())
            return m_Handle.ErrMsg();
        return _SC("");
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
        // Request failed
        if (Validate())
            return sqlite3_get_autocommit(m_Handle);
        // Request failed
        return false;
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
        if (Validate())
            return sqlite3_changes(m_Handle);
        // Request failed
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the total number of row changes caused by INSERT, UPDATE or DELETE statements
     * since the database connection was opened.
    */
    Int32 GetTotalChanges() const
    {
        // Validate the handle
        if (Validate())
            return sqlite3_total_changes(m_Handle);
        // Request failed
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * See if this database connection has tracing enabled.
    */
    bool GetTracing() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle->mTrace;
        // Request failed
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Activate or deactivate tracing on this database connection.
    */
    void SetTracing(bool toggle)
    {
        // Validate the handle and check whether changes are necessary
        if (!Validate() || m_Handle->mTrace == toggle)
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
        if (Validate())
            return m_Handle->mProfile;
        // Request failed
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Activate or deactivate profiling on this database connection.
    */
    void SetProfiling(bool toggle)
    {
        // Validate the handle and check whether changes are necessary
        if (!Validate() || m_Handle->mProfile == toggle)
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
        if (Validate())
            sqlite3_interrupt(m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Attempts to free as much heap memory as possible from the database connection.
    */
    void ReleaseMemory() const
    {
        // Validate the handle
        if (Validate())
            sqlite3_db_release_memory(m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Move the whole database into memory.
    */
    Connection CopyToMemory();

    /* --------------------------------------------------------------------------------------------
     * Takes a snapshot of a database which is located in memory and saves it to a database file.
    */
    void CopyToDatabase(Connection & db);

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
        if (Validate())
            return (Uint32)m_Handle->mQueue.size();
        // Request failed
        return 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Remove all queries from the queue without executing them.
    */
    void ClearQueue() const
    {
        // Validate the handle
        if (Validate())
            m_Handle->mQueue.clear();
    }

    /* --------------------------------------------------------------------------------------------
     * Remove the last query from the queue.
    */
    void PopQueue() const
    {
        // Validate the handle
        if (Validate() && !m_Handle->mQueue.empty())
            m_Handle->mQueue.pop_back();
    }

    /* --------------------------------------------------------------------------------------------
     * Flush all queries from the queue.
    */
    Int32 Flush()
    {
        // Validate the handle
        if (Validate())
            return Flush(m_Handle->mQueue.size());
        // Request failed
        return 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Flush a specific amount of queries from the queue.
    */
    Int32 Flush(Uint32 num);

    /* --------------------------------------------------------------------------------------------
     * Flush a specific amount of queries from the queue.
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

    /* --------------------------------------------------------------------------------------------
     * Build and modify the structure of tables and other objects in the memory database.
    */
    static int ProcessDDLRow(void * db, int columns_count, char ** values, char ** columns);

    /* --------------------------------------------------------------------------------------------
     * Insert all data from the origin database into the memory database.
    */
    static int ProcessDMLRow(void * db, int columns_count, char ** values, char ** columns);

    /* --------------------------------------------------------------------------------------------
     * Takes and saves a snapshot of the memory database in a file.
    */
    void TakeSnapshot(ConnHnd & destination);
};

} // Namespace:: SqMod

#endif // _SQSQLITE_CONNECTION_HPP_
