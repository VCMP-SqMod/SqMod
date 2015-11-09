#ifndef _LIBRARY_SQLITE_CONNECTION_HPP_
#define _LIBRARY_SQLITE_CONNECTION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Library/SQLite/Shared.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace SQLite {

/* ------------------------------------------------------------------------------------------------
 * Class used to manage a conenction to an SQLite database.
*/
class Connection
{
public:

    /* --------------------------------------------------------------------------------------------
     * The type of SQLite resource handle that this class uses.
    */
    typedef ConnectionHandle Handle;

    /* --------------------------------------------------------------------------------------------
     * Default constructor (invalid).
    */
    Connection();

    /* --------------------------------------------------------------------------------------------
     * Open a connection to a database using the specified path.
    */
    Connection(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Open a connection to a database using the specified path and flags.
    */
    Connection(const SQChar * path, SQInt32 flags);

    /* --------------------------------------------------------------------------------------------
     * Open a connection to a database using the specified path, flags and vfs.
    */
    Connection(const SQChar * path, SQInt32 flags, const SQChar * vfs);

    /* --------------------------------------------------------------------------------------------
     * Construct and reference an existing connection.
    */
    Connection(const Handle & hnd);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Connection(const Connection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Connection(Connection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Connection() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Connection & operator = (const Connection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Connection & operator = (Connection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to a handle reference.
    */
    operator const Handle & () const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to a raw SQLite connection handle.
    */
    operator sqlite3 * ()
    {
        return static_cast< sqlite3 * >(m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to a raw SQLite connection handle.
    */
    operator sqlite3 * () const
    {
        return static_cast< sqlite3 * >(m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Equality operator.
    */
    bool operator == (const Connection & o) const
    {
        return (m_Handle == o.m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Inequality operator.
    */
    bool operator != (const Connection & o) const
    {
        return (m_Handle != o.m_Handle);
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script to compare two instances of this type.
    */
    SQInt32 Cmp(const Connection & o) const;

    /* --------------------------------------------------------------------------------------------
     * Convert this type to a string.
    */
    const SQChar * ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global tag.
    */
    const SQChar * GetGlobalTag() const;

    /* --------------------------------------------------------------------------------------------
     * Change the global tag.
    */
    void SetGlobalTag(const SQChar * tag) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the global data.
    */
    SqObj & GetGlobalData() const;

    /* --------------------------------------------------------------------------------------------
     * Change the global data.
    */
    void SetGlobalData(SqObj & data) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local tag.
    */
    const SQChar * GetLocalTag() const;

    /* --------------------------------------------------------------------------------------------
     * Change the local tag.
    */
    void SetLocalTag(const SQChar * tag);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the local data.
    */
    SqObj & GetLocalData();

    /* --------------------------------------------------------------------------------------------
     * Change the local data.
    */
    void SetLocalData(SqObj & data);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the handle reference.
    */
    const Handle & GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to this handle.
    */
    SQUint32 GetRefs() const
    {
        return m_Handle ? m_Handle.Count() : 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Release the handle reference.
    */
    void Release()
    {
        m_Handle.Release();
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received error code.
    */
    SQInt32 GetStatus() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the flags used to create this database connection.
    */
    SQInt32 GetFlags() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the path used to create this database connection.
    */
    const SQChar * GetPath() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the virtual file system used to create this database connection.
    */
    const SQChar * GetVFS() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    const SQChar * GetErrStr() const;

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    const SQChar * GetErrMsg() const;

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    SQInt32 Exec(const SQChar * str);

    /* --------------------------------------------------------------------------------------------
     * Attempt to queue the specified query.
    */
    void Queue(const SQChar * str);

    /* --------------------------------------------------------------------------------------------
     * Shortcut to test if a table exists.
    */
    bool TableExists(const SQChar * name) const;

    /* --------------------------------------------------------------------------------------------
     * See if the database connection was opened in read-only mode.
    */
    bool IsReadOnly() const;

    /* --------------------------------------------------------------------------------------------
     * See if the database connection is or is not in autocommit mode.
    */
    bool GetAutoCommit() const;

    /* --------------------------------------------------------------------------------------------
     * Get the rowid of the most recent successful INSERT into the database from the current connection.
    */
    SLongInt GetLastInsertRowID() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the number of database rows that were changed, inserted or deleted
     * by the most recently completed SQL statement.
    */
    SQInt32 GetChanges() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the total number of row changes caused by INSERT, UPDATE or DELETE statements
     * since the database connection was opened.
    */
    SQInt32 GetTotalChanges() const;

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    SQInt32 GetErrorCode() const;

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    SQInt32 GetExtendedErrorCode() const;

    /* --------------------------------------------------------------------------------------------
     * See if this database connection has tracing enabled.
    */
    bool GetTracing() const;

    /* --------------------------------------------------------------------------------------------
     * Activate or deactivate tracing on this database connection.
    */
    void SetTracing(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See if this database connection has profiling enabled.
    */
    bool GetProfiling() const;

    /* --------------------------------------------------------------------------------------------
     * Activate or deactivate profiling on this database connection.
    */
    void SetProfiling(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Set a busy handler that sleeps for a specified amount of time when a table is locked.
    */
    void SetBusyTimeout(SQInteger millis) const;

    /* --------------------------------------------------------------------------------------------
     * Causes any pending database operation to abort and return at its earliest opportunity.
    */
    void InterruptOperation() const;

    /* --------------------------------------------------------------------------------------------
     * Attempts to free as much heap memory as possible from the database connection.
    */
    void ReleaseMemory() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the number of lookaside memory slots currently checked out.
    */
    SQInt32 GetNumberOfCheckedOutLookasideMemorySlots() const;

    /* --------------------------------------------------------------------------------------------
     *  Returns the approximate number of bytes of heap memory used by all pager caches.
    */
    SQInt32 GetHeapMemoryUsedByPagerCaches() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the approximate number of bytes of heap memory used to store the schema for
     * all databases associated with the connection - main, temp, and any ATTACH-ed databases.
    */
    SQInt32 GetHeapMemoryUsedToStoreSchemas() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the approximate number of bytes of heap and lookaside memory used by all
     * prepared statements associated with the database connection.
    */
    SQInt32 GetHeapAndLookasideMemoryUsedByPreparedStatements() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the number of pager cache hits that have occurred.
    */
    SQInt32 GetPagerCacheHitCount() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the number of pager cache misses that have occurred.
    */
    SQInt32 GetPagerCacheMissCount() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the number of dirty cache entries that have been written to disk.
    */
    SQInt32 GetNumberOfDirtyCacheEntries() const;

    /* --------------------------------------------------------------------------------------------
     * Returns zero if all foreign key constraints (deferred or immediate) have been resolved.
    */
    SQInt32 GetNumberOfUnresolvedForeignKeys() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the highest number of lookaside memory slots that has been checked out.
    */
    SQInt32 GetHighestNumberOfCheckedOutLookasideMemorySlots(bool reset);

    /* --------------------------------------------------------------------------------------------
     * Returns the number malloc attempts that were satisfied using lookaside memory.
    */
    SQInt32 GetLookasideMemoryHitCount(bool reset);

    /* --------------------------------------------------------------------------------------------
     * Returns the number malloc attempts that might have been satisfied using lookaside memory.
    */
    SQInt32 GetLookasideMemoryMissCountDueToSmallSlotSize(bool reset);

    /* --------------------------------------------------------------------------------------------
     * Returns the number malloc attempts that might have been satisfied using lookaside memory
    */
    SQInt32 GetLookasideMemoryMissCountDueToFullMemory(bool reset);

    /* --------------------------------------------------------------------------------------------
     * Move the whole database into memory.
    */
    Connection CopyToMemory();

    /* --------------------------------------------------------------------------------------------
     * Takes a snapshot of a database which is located in memory and saves it to a database file.
    */
    Connection CopyToDatabase(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    static SQInteger ExecF(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attempt to queue the specified query.
    */
    static SQInteger QueueF(HSQUIRRELVM vm);

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
    void TakeSnapshot(Handle & destination);

    /* --------------------------------------------------------------------------------------------
     * Returns internal runtime status information associated with the current database connection.
    */
    SQInt32 GetInfo(int operation, bool highwater = false, bool reset = false) const;

private:

    /* --------------------------------------------------------------------------------------------
     * Reference to the managed SQLite connection handle.
    */
    Handle m_Handle;

    /* --------------------------------------------------------------------------------------------
     * The local tag associated with this instance.
    */
    SqTag       m_Tag;

    /* --------------------------------------------------------------------------------------------
     * The local data associated with this instance.
    */
    SqObj       m_Data;
};

} // Namespace:: SQLite
} // Namespace:: SqMod

#endif // _LIBRARY_SQLITE_CONNECTION_HPP_
