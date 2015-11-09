#include "Library/SQLite/Connection.hpp"
#include "Library/SQLite/Statement.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqstdstring.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace SQLite {

// ------------------------------------------------------------------------------------------------
Connection::Connection()
    : m_Handle()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Connection::Connection(const SQChar * path)
    : Connection(path, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Connection::Connection(const SQChar * path, SQInt32 flags)
    : Connection(path, flags, NULL)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Connection::Connection(const SQChar * path, SQInt32 flags, const SQChar * vfs)
    : m_Handle(path, flags, vfs)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Connection::Connection(const Handle & hnd)
    : m_Handle(hnd)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
SQInteger Connection::Cmp(const Connection & o) const
{
    if (m_Handle == o.m_Handle)
    {
        return 0;
    }
    else if (m_Handle && (m_Handle->Ptr > o.m_Handle->Ptr))
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * Connection::ToString() const
{
    if (m_Handle)
    {
        return m_Handle->Path.c_str();
    }

    return _SC("");
}

// ------------------------------------------------------------------------------------------------
const SQChar * Connection::GetGlobalTag() const
{
    if (m_Handle)
    {
        return m_Handle->Tag.c_str();
    }
    else
    {
        LogWrn("Attempting to <get connection tag> using an invalid reference");
    }

    return _SC("");
}

void Connection::SetGlobalTag(const SQChar * tag) const
{
    if (m_Handle)
    {
        m_Handle->Tag.assign(tag);
    }
    else
    {
        LogWrn("Attempting to <set connection tag> using an invalid reference");
    }
}

// ------------------------------------------------------------------------------------------------
SqObj & Connection::GetGlobalData() const
{
    if (m_Handle)
    {
        return m_Handle->Data;
    }
    else
    {
        LogWrn("Attempting to <get connection data> using an invalid reference");
    }

    return NullData();
}

void Connection::SetGlobalData(SqObj & data) const
{
    if (m_Handle)
    {
        m_Handle->Data = data;
    }
    else
    {
        LogWrn("Attempting to <set connection data> using an invalid reference");
    }

}

// ------------------------------------------------------------------------------------------------
const SQChar * Connection::GetLocalTag() const
{
    return m_Tag.c_str();
}

void Connection::SetLocalTag(const SQChar * tag)
{
    m_Tag = tag;
}

// ------------------------------------------------------------------------------------------------
SqObj & Connection::GetLocalData()
{
    return m_Data;
}

void Connection::SetLocalData(SqObj & data)
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetStatus() const
{
    if (m_Handle)
    {
        return m_Handle->Status;
    }
    else
    {
        LogWrn("Attempting to <get connection status> using an invalid connection: null");
    }

    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetFlags() const
{
    if (m_Handle)
    {
        return m_Handle->Flags;
    }
    else
    {
        LogWrn("Attempting to <get connection flags> using an invalid connection: null");
    }

    return 0;
}

// ------------------------------------------------------------------------------------------------
const SQChar * Connection::GetPath() const
{
    if (m_Handle)
    {
        return m_Handle->Path.c_str();
    }
    else
    {
        LogWrn("Attempting to <get connection file path> using an invalid connection: null");
    }

    return _SC("");
}

// ------------------------------------------------------------------------------------------------
const SQChar * Connection::GetVFS() const
{
    if (m_Handle)
    {
        return m_Handle->VFS.c_str();
    }
    else
    {
        LogWrn("Attempting to <get connection vfs string> using an invalid connection: null");
    }

    return _SC("");
}

// ------------------------------------------------------------------------------------------------
const SQChar * Connection::GetErrStr() const
{
    if (m_Handle)
    {
        return m_Handle.ErrStr();
    }
    else
    {
        LogWrn("Attempting to <get connection error string> using an invalid connection: null");
    }

    return _SC("");
}

// ------------------------------------------------------------------------------------------------
const SQChar * Connection::GetErrMsg() const
{
    if (m_Handle)
    {
        return m_Handle.ErrMsg();
    }
    else
    {
        LogWrn("Attempting to <get connection error message> using an invalid connection: null");
    }

    return _SC("");
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::Exec(const SQChar * str)
{
    if (m_Handle)
    {
        if ((m_Handle->Status = sqlite3_exec(m_Handle, static_cast< const char * >(str), NULL, NULL, NULL)) != SQLITE_OK)
        {
            LogErr("Unable to <execute database query> because: %s", m_Handle.ErrMsg());
        }
        else
        {
            return sqlite3_changes(m_Handle);
        }
    }
    else
    {
        LogWrn("Attempting to <execute database query> using an invalid connection: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
void Connection::Queue(const SQChar * str)
{
    if (m_Handle)
    {
        m_Handle->Queue.emplace(str);
    }
    else
    {
        LogWrn("Attempting to <queue database query> using an invalid connection: null");
    }
}

// ------------------------------------------------------------------------------------------------
bool Connection::TableExists(const SQChar * name) const
{
    if (m_Handle)
    {
        // Create the statement which counts the tables with the specified name
        Statement stmt(*this, "SELECT count(*) FROM [sqlite_master] WHERE [type]='table' AND [name]=?");
        // See if the statement could be created
        if (stmt.IsValid())
        {
            // Bind the table name
            stmt.IndexBindS(1, name);
            // Attempt to step the statement
            if (stmt.Step())
            {
                LogErr("Unable to <see if table exists> because : the statement could not be stepped");
                // Return the requested information
                return (sqlite3_column_int(stmt, 0) == 1);
            }
        }
        else
        {
            LogErr("Unable to <see if table exists> because : the statement could not be created");
        }
    }
    else
    {
        LogWrn("Attempting to <see if table exists> using an invalid connection: null");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
bool Connection::IsReadOnly() const
{
    if (m_Handle)
    {
        const int result = sqlite3_db_readonly(m_Handle, "main");
        // Verify the result
        if(result == -1)
        {
            LogErr("Unable to <see if database is read only> because : 'main' is not the name of a database on connection");
        }
        // Return the result
        else
        {
            return (result != 1);
        }
    }
    else
    {
        LogWrn("Attempting to <see if database is read only> using an invalid connection: null");
    }
    // It's invalid so at least fall-back to read-only
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Connection::GetAutoCommit() const
{
    if (m_Handle)
    {
        return sqlite3_get_autocommit(m_Handle);
    }
    else if (!m_Handle)
    {
        LogWrn("Attempting to <see if database has autocommit activated> using an invalid connection: null");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
SLongInt Connection::GetLastInsertRowID() const
{
    if (m_Handle)
    {
        return SLongInt(static_cast< SLongInt::Type >(sqlite3_last_insert_rowid(m_Handle)));
    }
    else if (!m_Handle)
    {
        LogWrn("Attempting to <get database changes> using an invalid connection: null");
    }

    return SLongInt(static_cast< SLongInt::Type >(SQMOD_UNKNOWN));
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetChanges() const
{
    if (m_Handle)
    {
        return sqlite3_changes(m_Handle);
    }
    else if (!m_Handle)
    {
        LogWrn("Attempting to <get database changes> using an invalid connection: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetTotalChanges() const
{
    if (m_Handle)
    {
        return sqlite3_total_changes(m_Handle);
    }
    else if (!m_Handle)
    {
        LogWrn("Attempting to <get database total changes> using an invalid connection: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetErrorCode() const
{
    if (m_Handle)
    {
        return sqlite3_errcode(m_Handle);
    }
    else
    {
        LogWrn("Attempting to <get database error code> using an invalid connection: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetExtendedErrorCode() const
{
    if (m_Handle)
    {
        return sqlite3_extended_errcode(m_Handle);
    }
    else
    {
        LogWrn("Attempting to <get database extended error code> using an invalid connection: null");
    }

    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
bool Connection::GetTracing() const
{
    if (m_Handle)
    {
        return m_Handle->Trace;
    }
    else
    {
        LogWrn("Attempting to <see if database has tracing activated> using an invalid connection: null");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void Connection::SetTracing(bool toggle) const
{
    if (m_Handle && m_Handle->Trace != toggle)
    {
        if (m_Handle->Trace)
        {
            sqlite3_trace(m_Handle, NULL, NULL);
        }
        else
        {
            sqlite3_trace(m_Handle, &Connection::TraceOutput, NULL);
        }
    }
    else if (!m_Handle)
    {
        LogWrn("Attempting to <activate database tracing> using an invalid connection: null");
    }
}

// ------------------------------------------------------------------------------------------------
bool Connection::GetProfiling() const
{
    if (m_Handle)
    {
        return m_Handle->Profile;
    }
    else
    {
        LogWrn("Attempting to <see if database has profiling activated> using an invalid connection: null");
    }

    return false;
}

// ------------------------------------------------------------------------------------------------
void Connection::SetProfiling(bool toggle) const
{
    if (m_Handle && m_Handle->Profile != toggle)
    {
        if (m_Handle->Profile)
        {
            sqlite3_profile(m_Handle, NULL, NULL);
        }
        else
        {
            sqlite3_profile(m_Handle, &Connection::ProfileOutput, NULL);
        }
    }
    else if (!m_Handle)
    {
        LogWrn("Attempting to <activate database profiling> using an invalid connection: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Connection::SetBusyTimeout(SQInteger millis) const
{
    if (m_Handle)
    {
        if ((m_Handle->Status = sqlite3_busy_timeout(m_Handle, millis)) != SQLITE_OK)
        {
            LogErr("Unable to <set database busy timeout> because : %s", m_Handle.ErrMsg());
        }
    }
    else
    {
        LogWrn("Attempting to <set database busy timeout> using an invalid connection: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Connection::InterruptOperation() const
{
    if (m_Handle)
    {
        sqlite3_interrupt(m_Handle);
    }
    else
    {
        LogWrn("Attempting to <interrupt database operation> using an invalid connection: null");
    }
}

// ------------------------------------------------------------------------------------------------
void Connection::ReleaseMemory() const
{
    if (m_Handle)
    {
        sqlite3_db_release_memory(m_Handle);
    }
    else
    {
        LogWrn("Attempting to <release database memory> using an invalid connection: null");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetNumberOfCheckedOutLookasideMemorySlots() const
{
    return GetInfo(SQLITE_DBSTATUS_LOOKASIDE_USED);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetHeapMemoryUsedByPagerCaches() const
{
    return GetInfo(SQLITE_DBSTATUS_CACHE_USED);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetHeapMemoryUsedToStoreSchemas() const
{
    return GetInfo(SQLITE_DBSTATUS_SCHEMA_USED);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetHeapAndLookasideMemoryUsedByPreparedStatements() const
{
    return GetInfo(SQLITE_DBSTATUS_STMT_USED);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetPagerCacheHitCount() const
{
    return GetInfo(SQLITE_DBSTATUS_CACHE_HIT);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetPagerCacheMissCount() const
{
    return GetInfo(SQLITE_DBSTATUS_CACHE_MISS);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetNumberOfDirtyCacheEntries() const
{
    return GetInfo(SQLITE_DBSTATUS_CACHE_WRITE);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetNumberOfUnresolvedForeignKeys() const
{
    return GetInfo(SQLITE_DBSTATUS_DEFERRED_FKS);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetHighestNumberOfCheckedOutLookasideMemorySlots(bool reset)
{
    return GetInfo(SQLITE_DBSTATUS_LOOKASIDE_USED, true, reset);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetLookasideMemoryHitCount(bool reset)
{
    return GetInfo(SQLITE_DBSTATUS_LOOKASIDE_HIT, true, reset);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetLookasideMemoryMissCountDueToSmallSlotSize(bool reset)
{
    return GetInfo(SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE, true, reset);
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetLookasideMemoryMissCountDueToFullMemory(bool reset)
{
    return GetInfo(SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL, true, reset);
}

// ------------------------------------------------------------------------------------------------
Connection Connection::CopyToMemory()
{
    if (m_Handle && !m_Handle->Memory)
    {
        // Attempt to open an in-memory database
        Handle db(":memory:", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        // See if the database could be opened
        if (!db)
        {
            LogErr("Unable to <open in-memory database> because : %s", db.ErrMsg());
        }
        // Begin a transaction to replicate the schema of origin database
        else if ((m_Handle->Status = sqlite3_exec(m_Handle, "BEGIN", NULL, NULL, NULL)) != SQLITE_OK)
        {
            LogErr("Unable to <begin database schema replication> because : %s", m_Handle.ErrMsg());
        }
        // Attempt to replicate the schema of origin database to the in-memory one
        else if ((m_Handle->Status = sqlite3_exec(m_Handle,
                    "SELECT [sql] FROM [sqlite_master] WHERE [sql] NOT NULL AND [tbl_name] != 'sqlite_sequence'",
                    &Connection::ProcessDDLRow, db, NULL)) != SQLITE_OK)
        {
            LogErr("Unable to <replicate database schema> because : %s", m_Handle.ErrMsg());
        }
        // Attempt to commit the changes to the database schema replication
        else if ((m_Handle->Status = sqlite3_exec(m_Handle, "COMMIT", NULL, NULL, NULL)) != SQLITE_OK)
        {
            LogErr("Unable to <commit database schema replication> because : %s", m_Handle.ErrMsg());
        }
        // Attempt to attach the origin database to the in-memory one
        else if ((db->Status = sqlite3_exec(db, ToStringF("ATTACH DATABASE '%s' as origin",
                                                m_Handle->Path.c_str()), NULL, NULL, NULL)) != SQLITE_OK)
        {
            LogErr("Unable to <attach database origin> because : %s", db.ErrMsg());
        }
        // Begin a transaction to replicate the data of origin database
        else if ((db->Status = sqlite3_exec(db, "BEGIN", NULL, NULL, NULL) != SQLITE_OK))
        {
            LogErr("Unable to <begin database data replication> because : %s", db.ErrMsg());
        }
        // Attempt to replicate the data of origin database to the in-memory one
        else if ((db->Status = sqlite3_exec(db, "SELECT [name] FROM [origin.sqlite_master] WHERE [type]='table'",
                        &Connection::ProcessDMLRow, db, NULL)) != SQLITE_OK)
        {
            LogErr("Unable to <replicate database data> because : %s", m_Handle.ErrMsg());
        }
        // Attempt to commit the changes to the database data replication
        else if ((db->Status = sqlite3_exec(db, "COMMIT", NULL, NULL, NULL)) != SQLITE_OK)
        {
            LogErr("Unable to <commit database data replication> because : %s", db.ErrMsg());
            // Attempt to rollback changes from the data copy operation
            if ((db->Status = sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL)) != SQLITE_OK)
            {
                LogErr("Unable to <rollback database data replication> because : %s", db.ErrMsg());
            }
            // Attempt to detach the disk origin from in-memory database
            else if ((db->Status = sqlite3_exec(db, "DETACH DATABASE origin", NULL, NULL, NULL)) != SQLITE_OK)
            {
                LogErr("Unable to <detach database origin> because : %s", db.ErrMsg());
            }
        }
        else
        {
            // Return the new in-memory database
            return db;
        }
    }
    else if (!m_Handle)
    {
        LogWrn("Attempting to <copy database to memory> using an invalid connection: null");
    }
    else
    {
        LogWrn("Attempting to <copy database to memory> for a database which is already in memory");
    }
    // Fall-back to an invalid connection
    return Connection();
}

// ------------------------------------------------------------------------------------------------
Connection Connection::CopyToDatabase(const SQChar * path)
{
    if (m_Handle)
    {
        // See if the specified name is valid
        if (strlen(path) <= 0)
        {
            LogWrn("Attempting to <replicate database> using an invalid path: null");
        }
        // Attempt to open the specified database
        Handle db(path, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        // See if the database could be opened
        if (!db)
        {
            LogErr("Unable to <open the specified database> because : %s", db.ErrMsg());
        }
        else
        {
            // Attempt to take snapshot of the database
            TakeSnapshot(db);
            // Return the database clone
            return db;
        }
    }
    else
    {
        LogWrn("Attempting to <replicate database> using an invalid connection: null");
    }
    // Fall-back to an invalid connection
    return Connection();
}

// ------------------------------------------------------------------------------------------------
void Connection::TraceOutput(void * ptr, const char * sql)
{
    SQMOD_UNUSED_VAR(ptr);
    LogInf("SQLite Trace: %s", sql);
}

// ------------------------------------------------------------------------------------------------
void Connection::ProfileOutput(void * ptr, const char * sql, sqlite3_uint64 time)
{
    SQMOD_UNUSED_VAR(ptr);
    LogInf("SQLite profile (time: %llu): %s", time, sql);
}

// ------------------------------------------------------------------------------------------------
int Connection::ProcessDDLRow(void * db, int columns_count, char ** values, char ** columns)
{
    SQMOD_UNUSED_VAR(columns);

    if(columns_count != 1)
    {
        LogErr("Error occurred during DDL: columns != 1");
        // Operation failed
        return -1;
    }
    // Execute the sql statement in values[0] in the received database connection
    if(sqlite3_exec(static_cast< sqlite3 * >(db), values[0], NULL, NULL, NULL) != SQLITE_OK)
    {
        LogErr("Error occurred during DDL execution: %s", sqlite3_errmsg(static_cast< sqlite3 * >(db)));
    }
    // Operation succeeded
    return 0;
}

// ------------------------------------------------------------------------------------------------
int Connection::ProcessDMLRow(void * db, int columns_count, char ** values, char ** columns)
{
    SQMOD_UNUSED_VAR(columns);

    if(columns_count != 1)
    {
        LogErr("Error occurred during DML: columns != 1");
        // Operation failed
        return -1;
    }
    // Generate the query string with the received values
    char * sql = sqlite3_mprintf("INSERT INTO main.%q SELECT * FROM origin.%q", values[0], values[0]);
    // Attempt to execute the generated query string to the received database connection
    if(sqlite3_exec(static_cast< sqlite3 * >(db), sql, NULL, NULL, NULL) != SQLITE_OK)
    {
        LogErr("Error occurred during DML execution: %s", sqlite3_errmsg(static_cast< sqlite3 * >(db)));
    }
    // Free the generated query string
    sqlite3_free(sql);
    // Operation succeeded
    return 0;
}

// ------------------------------------------------------------------------------------------------
void Connection::TakeSnapshot(Handle & destination)
{
    // Don't even bother to continue if there's no valid connection handle
    if (m_Handle && destination)
    {
        // Attempt to initialize a backup structure
        sqlite3_backup * backup = sqlite3_backup_init(destination, "main", m_Handle, "main");
        // See if the backup structure could be created
        if(backup)
        {
            // -1 to copy the entire source database to the destination
            if((m_Handle->Status = sqlite3_backup_step(backup, -1)) != SQLITE_DONE)
            {
                LogErr("Unable to <copy database source> because: %s", sqlite3_errmsg(destination));
            }
            // clean up resources allocated by sqlite3_backup_init()
            if((m_Handle->Status = sqlite3_backup_finish(backup)) != SQLITE_OK)
            {
                LogErr("Unable to <finalize database backup> because: %s", sqlite3_errmsg(destination));
            }
        }
    }
    else if (!destination)
    {
        LogWrn("Attempting to <take database snapshot> using an invalid destination: null");
    }
    else
    {
        LogWrn("Attempting to <take database snapshot> using an invalid connection: null");
    }
}

// ------------------------------------------------------------------------------------------------
SQInt32 Connection::GetInfo(int operation, bool highwater, bool reset) const
{
    // Don't even bother to continue if there's no valid connection handle
    if (m_Handle)
    {
        int cur_value;
        int hiwtr_value;
        // Attempt to retrieve the specified information
        if((m_Handle->Status = sqlite3_db_status(m_Handle, operation, &cur_value, &hiwtr_value, reset)) != SQLITE_OK)
        {
            LogErr("Unable to <get database runtime status information> because: %s", sqlite3_errmsg(m_Handle));
        }
        // Return what was requested
        return highwater ? hiwtr_value : cur_value;
    }
    else
    {
        LogWrn("Attempting to <get database runtime status information> using an invalid connection: null");
    }
    return SQMOD_UNKNOWN;
}

// ------------------------------------------------------------------------------------------------
SQInteger Connection::ExecF(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Are there any arguments on the stack?
    if (top <= 1)
    {
        LogErr("Attempting to <execute database query> without specifying a value");
        // Push a null value on the stack
        sq_pushnull(vm);
    }
    // Attempt to retrieve the connection instance
    Var< Connection & > inst(vm, 1);
    // Validate the connection instance
    if (!inst.value)
    {
        LogErr("Attempting to <execute database query> using an invalid connection: null");
        // Push a null value on the stack
        sq_pushnull(vm);
    }
    // Is there a single string or at least something that can convert to a string on the stack?
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        // Variable where the resulted string will be retrieved
        const SQChar * str = 0;
        // Attempt to retrieve the specified message from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &str)))
        {
            LogErr("Unable to <fetch database query> because : the string cannot be retrieved from the stack");
            // Push a null value on the stack
            sq_pushnull(vm);
        }
        else
        {
            // Attempt to execute the specified query string and push the result on the stack
            sq_pushinteger(vm, inst.value.Exec(str));
        }
        // Pop any pushed values pushed to the stack
        sq_settop(vm, top);
    }
    else if (top > 2)
    {
        // Variables containing the resulted string
        SQChar * str = NULL;
        SQInteger len = 0;
        // Attempt to call the format function with the passed arguments
        if (SQ_FAILED(sqstd_format(vm, 2, &len, &str)))
        {
            LogErr("Unable to <generate database query> because : %s", Error::Message(vm).c_str());
            // Push a null value on the stack
            sq_pushnull(vm);
        }
        else
        {
            // Attempt to execute the resulted query string and push the result on the stack
            sq_pushinteger(vm, inst.value.Exec(str));
        }
    }
    else
    {
        LogErr("Unable to <extract database query> from the specified value");
        // Push a null value on the stack
        sq_pushnull(vm);
    }
    // At this point everything went correctly (probably)
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Connection::QueueF(HSQUIRRELVM vm)
{
    const SQInteger top = sq_gettop(vm);
    // Are there any arguments on the stack?
    if (top <= 1)
    {
        LogErr("Attempting to <queue database query> without specifying a value");
    }
    // Attempt to retrieve the connection instance
    Var< Connection & > inst(vm, 1);
    // Validate the connection instance
    if (!inst.value)
    {
        LogErr("Attempting to <queue database query> using an invalid connection: null");
    }
    // Is there a single string or at least something that can convert to a string on the stack?
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        // Variable where the resulted string will be retrieved
        const SQChar * str = 0;
        // Attempt to retrieve the specified message from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &str)))
        {
            LogErr("Unable to <fetch database query> because : the string cannot be retrieved from the stack");
        }
        else
        {
            // Attempt to queue the specified query string
            inst.value.Queue(str);
        }
        // Pop any pushed values pushed to the stack
        sq_settop(vm, top);
    }
    else if (top > 2)
    {
        // Variables containing the resulted string
        SQChar * str = NULL;
        SQInteger len = 0;
        // Attempt to call the format function with the passed arguments
        if (SQ_FAILED(sqstd_format(vm, 2, &len, &str)))
        {
            LogErr("Unable to <generate database query> because : %s", Error::Message(vm).c_str());
        }
        else
        {
            // Attempt to queue the specified query string
            inst.value.Queue(str);
        }
    }
    else
    {
        LogErr("Unable to <extract database query> from the specified value");
    }
    // At this point everything went correctly (probably)
    return 0;
}

} // Namespace:: SQLite
} // Namespace:: SqMod
