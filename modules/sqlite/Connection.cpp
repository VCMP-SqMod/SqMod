// ------------------------------------------------------------------------------------------------
#include "Connection.hpp"
#include "Statement.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Connection::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqSQLiteConnection");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void Connection::Validate() const
{
    // Is the handle valid?
    if (!m_Handle)
        SqThrowF("Invalid SQLite connection reference");
}

// ------------------------------------------------------------------------------------------------
Connection::Connection()
    : m_Handle()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Connection::Connection(CSStr name)
    : m_Handle(name)
{
    if (m_Handle.m_Hnd)
        m_Handle->Create(name, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
}

// ------------------------------------------------------------------------------------------------
Connection::Connection(CSStr name, Int32 flags)
    : m_Handle(name)
{
    if (m_Handle.m_Hnd)
        m_Handle->Create(name, flags, NULL);
}

// ------------------------------------------------------------------------------------------------
Connection::Connection(CSStr name, Int32 flags, CSStr vfs)
    : m_Handle(name)
{
    if (m_Handle.m_Hnd)
        m_Handle->Create(name, flags, vfs);
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::Exec(CSStr str)
{
    // Validate the handle
    Validate();
    // Attempt to execute the specified query
    if ((m_Handle = sqlite3_exec(m_Handle, str, NULL, NULL, NULL)) != SQLITE_OK)
        SqThrowF("Unable to execute query [%s]", m_Handle.ErrMsg());
    // Return rows affected by this query
    return sqlite3_changes(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Object Connection::Query(CSStr str) const
{
    // Validate the handle
    Validate();
    // Return the requested information
    return Object(new Statement(m_Handle, str));
}

// ------------------------------------------------------------------------------------------------
void Connection::Queue(CSStr str)
{
    // Validate the handle
    Validate();
    // Is there a query to commit?
    if (IsQueryEmpty(str))
        SqThrowF("No query string to queue");
    // Add the specified string to the queue
    m_Handle->mQueue.push_back(str);
}

// ------------------------------------------------------------------------------------------------
bool Connection::IsReadOnly() const
{
    // Validate the handle
    Validate();
    // Request the desired information
    const int result = sqlite3_db_readonly(m_Handle, "main");
    // Verify the result
    if (result == -1)
        SqThrowF("'main' is not the name of a database on connection");
    // Return the requested information
    return (result != 1);
}

// ------------------------------------------------------------------------------------------------
bool Connection::TableExists(CCStr name) const
{
    // Validate the handle
    Validate();
    // Prepare a statement to inspect the master table
    Statement stmt(m_Handle, "SELECT count(*) FROM [sqlite_master] WHERE [type]='table' AND [name]=?");
    // Could the statement be created?
    if (stmt.IsValid())
    {
        // Bind the specified name onto the statement parameter
        stmt.IndexBindS(1, name);
        // Attempt to step the statement and obtain a value
        if (stmt.Step())
            return (sqlite3_column_int(stmt, 0) == 1);
    }
    // Assume it doesn't exist
    return false;
}

// ------------------------------------------------------------------------------------------------
Object Connection::GetLastInsertRowID() const
{
    // Validate the handle
    Validate();
    // Obtain the initial stack size
    const StackGuard sg(_SqVM);
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushSLongObject(_SqVM, sqlite3_last_insert_rowid(m_Handle));
    // Get the object from the stack and return it
    return Var< Object >(_SqVM, -1).value;
}

// ------------------------------------------------------------------------------------------------
void Connection::SetBusyTimeout(Int32 millis)
{
    // Validate the handle
    Validate();
    // Apply requested timeout
    if ((m_Handle = sqlite3_busy_timeout(m_Handle, millis)) != SQLITE_OK)
        SqThrowF("Unable to set busy timeout [%s]", m_Handle.ErrMsg());
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::GetInfo(Int32 operation, bool highwater, bool reset)
{
    // Don't even bother to continue if there's no valid connection handle
    Validate();
    // Where to retrieve the information
    Int32 cur_value;
    Int32 hiwtr_value;
    // Attempt to retrieve the specified information
    if ((m_Handle = sqlite3_db_status(m_Handle, operation, &cur_value, &hiwtr_value, reset)) != SQLITE_OK)
        SqThrowF("Unable to get runtime status information", m_Handle.ErrMsg());
    // Return the high-water value if requested
    else if (highwater)
        return hiwtr_value;
    // Return the requested information
    return cur_value;
}

// ------------------------------------------------------------------------------------------------
Connection Connection::CopyToMemory()
{
    // Validate the handle
    Validate();
    // Is the database already in memory?
    if (m_Handle->mMemory)
        SqThrowF("The database is already in memory");
    // Destination database
    ConnHnd db(_SC(""));
    // Attempt to open the in-memory database
    db->Create(_SC(":memory:"), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    // Clear the temporary buffer
    GetTempBuff()[0] = 0;
    // Begin a transaction to replicate the schema of origin database
    if ((m_Handle = sqlite3_exec(m_Handle, "BEGIN", NULL, NULL, NULL)) != SQLITE_OK)
        SqThrowF("Unable to begin schema replication [%s]", m_Handle.ErrMsg());
    // Attempt to replicate the schema of origin database to the in-memory one
    else if ((m_Handle = sqlite3_exec(m_Handle,
                "SELECT [sql] FROM [sqlite_master] WHERE [sql] NOT NULL AND [tbl_name] != 'sqlite_sequence'",
                &Connection::ProcessDDLRow, db->mPtr, NULL)) != SQLITE_OK)
    {
        // Did the error occurred from the DDL process function?
        if (GetTempBuff()[0] != 0)
            // Throw the resulted message but also include the point where it failed
            SqThrowF("Unable to replicate schema [%s]", GetTempBuff());
        // Obtain the message from the connection handle if possible
        else
            SqThrowF("Unable to replicate schema [%s]", m_Handle.ErrMsg());
    }
    // Attempt to commit the changes to the database schema replication
    else if ((m_Handle = sqlite3_exec(m_Handle, "COMMIT", NULL, NULL, NULL)) != SQLITE_OK)
        SqThrowF("Unable to commit schema replication [%s]", m_Handle.ErrMsg());
    // Attempt to attach the origin database to the in-memory one
    else if ((db = sqlite3_exec(db, QFmtStr("ATTACH DATABASE '%q' as origin", m_Handle->mName.c_str()),
                                        NULL, NULL, NULL)) != SQLITE_OK)
        SqThrowF("Unable to attach origin [%s]", db.ErrMsg());
    // Begin a transaction to replicate the data of origin database
    else if ((db = sqlite3_exec(db, "BEGIN", NULL, NULL, NULL) != SQLITE_OK))
        SqThrowF("Unable to begin data replication [%s]", db.ErrMsg());
    // Attempt to replicate the data of origin database to the in-memory one
    else if ((db = sqlite3_exec(db, "SELECT [name] FROM [origin.sqlite_master] WHERE [type]='table'",
                    &Connection::ProcessDMLRow, db->mPtr, NULL)) != SQLITE_OK)
    {
        // Did the error occurred from the DML process function?
        if (GetTempBuff()[0] != 0)
        {
            // Throw the resulted message but also include the point where it failed
            SqThrowF("Unable to replicate data [%s]", GetTempBuff());
        }
        // Obtain the message from the connection handle if possible
        else
            SqThrowF("Unable to replicate data [%s]", db.ErrMsg());
    }
    // Attempt to commit the changes to the database data replication
    else if ((db = sqlite3_exec(db, "COMMIT", NULL, NULL, NULL)) != SQLITE_OK)
    {
        // Attempt to rollback changes from the data copy operation
        if ((db = sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL)) != SQLITE_OK)
            SqThrowF("Unable to rollback data replication [%s]", db.ErrMsg());
        // Attempt to detach the disk origin from in-memory database
        else if ((db = sqlite3_exec(db, "DETACH DATABASE origin", NULL, NULL, NULL)) != SQLITE_OK)
            SqThrowF("Unable to detach origin [%s]", db.ErrMsg());
        // Operation failed
        SqThrowF("Unable to commit data replication [%s]", db.ErrMsg());
    }
    // At this point everything went fine and the database instance should be returned
    return Connection(db);
}

// ------------------------------------------------------------------------------------------------
void Connection::CopyToDatabase(const Connection & db)
{
    // Make sure that we have two valid database handles
    Validate();
    db.Validate();
    // Attempt to take the snapshot and return the result
    TakeSnapshot(db.m_Handle);
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::Flush(Uint32 num)
{
    // Validate the handle
    Validate();
    // We need to supply a null callback
    Object env;
    Function func;
    // Attempt to flush the requested amount of queries
    return m_Handle->Flush(num, env, func);
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::Flush(Uint32 num, Object & env, Function & func)
{
    // Validate the handle
    Validate();
    // Attempt to flush the requested amount of queries
    return m_Handle->Flush(num, env, func);
}

// ------------------------------------------------------------------------------------------------
void Connection::TraceOutput(void * /*ptr*/, CCStr sql)
{
    _SqMod->LogInf("SQLite Trace: %s", sql);
}

void Connection::ProfileOutput(void * /*ptr*/, CCStr sql, sqlite3_uint64 time)
{
    _SqMod->LogInf("SQLite profile (time: %llu): %s", time, sql);
}

// ------------------------------------------------------------------------------------------------
int Connection::ProcessDDLRow(void * db, int columns_count, char ** values, char ** /*columns*/)
{
    // Make sure that exactly one column exists in the result
    if (columns_count != 1)
        FmtStr("Error occurred during DDL: columns != 1");
    // Execute the sql statement in values[0] in the received database connection
    else if (sqlite3_exec((sqlite3 *)db, values[0], NULL, NULL, NULL) != SQLITE_OK)
        FmtStr("Error occurred during DDL execution: %s", sqlite3_errmsg((sqlite3 *)db));
    else
        // Continue processing
        return 0;
    // Operation aborted
    return -1;
}

int Connection::ProcessDMLRow(void * db, int columns_count, char ** values, char ** /*columns*/)
{
    // Make sure that exactly one column exists in the result
    if (columns_count != 1)
    {
        FmtStr("Error occurred during DML: columns != 1");
        // Operation aborted
        return -1;
    }
    // Generate the query string with the received values
    char * sql = sqlite3_mprintf("INSERT INTO main.%q SELECT * FROM origin.%q", values[0], values[0]);
    // Attempt to execute the generated query string on the received database connection
    if (sqlite3_exec((sqlite3 *)db, sql, NULL, NULL, NULL) != SQLITE_OK)
        FmtStr("Error occurred during DML execution: %s", sqlite3_errmsg((sqlite3 *)db));
    else
    {
        // Free the generated query string
        sqlite3_free(sql);
        // Continue processing
        return 0;
    }
    // Free the generated query string
    sqlite3_free(sql);
    // Operation aborted
    return -1;
}

// ------------------------------------------------------------------------------------------------
void Connection::TakeSnapshot(const ConnHnd & destination)
{
    // Attempt to initialize a backup structure
    sqlite3_backup * backup = sqlite3_backup_init(destination, "main", m_Handle, "main");
    // See if the backup structure could be created
    if (!backup)
        SqThrowF("Unable to initialize the backup structure [%s]", destination.ErrMsg());
    // -1 to copy the entire source database to the destination
    if ((m_Handle = sqlite3_backup_step(backup, -1)) != SQLITE_DONE)
    {
        // Finalize the backup structure first
        sqlite3_backup_finish(backup);
        // Now it's safe to throw the error
        SqThrowF("Unable to copy source [%s]", m_Handle.ErrStr());
    }
    // Clean up resources allocated by sqlite3_backup_init()
    if ((m_Handle = sqlite3_backup_finish(backup)) != SQLITE_OK)
        SqThrowF("Unable to finalize backup [%s]", m_Handle.ErrStr());
}

// ------------------------------------------------------------------------------------------------
SQInteger Connection::ExecF(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Do we even have enough arguments?
    if (top <= 1)
        return sq_throwerror(vm, "Missing the query string");
    // Obtain the connection instance
    Var< Connection * > inst(vm, 1);
    // Do we have a valid connection instance?
    if (!inst.value)
        return sq_throwerror(vm, "Invalid connection instance");
    // Do we have a valid connection reference?
    else if (!inst.value->m_Handle)
        return sq_throwerror(vm, "Invalid SQLite connection reference");
    // Is the specified message value a string or something convertible to string?
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        CCStr sql = NULL;
        // Attempt to retrieve the string from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &sql)))
        {
            // If the value was converted to a string then pop the string
            sq_pop(vm, sq_gettop(vm) - top);
            // Now we can throw the error message
            return sq_throwerror(vm, "Unable to retrieve the query");
        }
        // Prevent the object from being destroyed once we pop it
        Var< Object > obj(vm, -1);
        // If the value was converted to a string then pop the string
        sq_pop(vm, sq_gettop(vm) - top);
        // Attempt to execute the specified query
        if ((inst.value->m_Handle = sqlite3_exec(inst.value->m_Handle, sql, NULL, NULL, NULL)) != SQLITE_OK)
        {
            // Generate the error message and throw the resulted string
            return sq_throwerror(vm, FmtStr("Unable to execute query [%s]", inst.value->m_Handle.ErrMsg()));
        }
        // Push the result onto the stack
        sq_pushinteger(vm, sqlite3_changes(inst.value->m_Handle));
    }
    // Do we have enough values to call the format function?
    else if (top > 2)
    {
        SStr sql = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 3, &len, &sql);
        // Did the format failed?
        if (SQ_FAILED(ret))
            return ret; // Propagate the exception
        // Attempt to execute the resulted query
        if ((inst.value->m_Handle = sqlite3_exec(inst.value->m_Handle, sql, NULL, NULL, NULL)) != SQLITE_OK)
        {
            // Generate the error message and throw the resulted string
            return sq_throwerror(vm, FmtStr("Unable to execute query [%s]", inst.value->m_Handle.ErrMsg()));
        }
        // Push the result onto the stack
        sq_pushinteger(vm, sqlite3_changes(inst.value->m_Handle));
    }
    // All methods of retrieving the string value failed
    else
        return sq_throwerror(vm, "Unable to extract the query string");
    // At this point we should have a return value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Connection::QueueF(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Do we even have enough arguments?
    if (top <= 1)
        return sq_throwerror(vm, "Missing the query string");
    // Obtain the connection instance
    Var< Connection * > inst(vm, 1);
    // Do we have a valid connection instance?
    if (!inst.value)
        return sq_throwerror(vm, "Invalid connection instance");
    // Do we have a valid connection reference?
    else if (!inst.value->m_Handle)
        return sq_throwerror(vm, "Invalid SQLite connection reference");
    // Is the specified message value a string or something convertible to string?
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        CCStr sql = NULL;
        // Attempt to retrieve the string from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &sql)))
        {
            // If the value was converted to a string then pop the string
            sq_pop(vm, sq_gettop(vm) - top);
            // Now we can throw the error message
            return sq_throwerror(vm, "Unable to retrieve the query");
        }
        // Is there even a query to queue?
        else if (IsQueryEmpty(sql))
        {
            // If the value was converted to a string then pop the string
            sq_pop(vm, sq_gettop(vm) - top);
            // Now we can throw the error message
            return sq_throwerror(vm,"No query to queue");
        }
        // Attempt to queue the specified query
        inst.value->m_Handle->mQueue.push_back(sql);
        // If the value was converted to a string then pop the string
        sq_pop(vm, sq_gettop(vm) - top);
    }
    // Do we have enough values to call the format function?
    else if (top > 2)
    {
        SStr sql = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 3, &len, &sql);
        // Did the format failed?
        if (SQ_FAILED(ret))
            return ret; // Propagate the exception
        // Is there even a query to queue?
        else if (IsQueryEmpty(sql))
            return sq_throwerror(vm,"No query to queue");
        // Attempt to queue the specified query
        inst.value->m_Handle->mQueue.push_back(sql);
    }
    // All methods of retrieving the string value failed
    else
        return sq_throwerror(vm, "Unable to extract the query string");
    // This function does not return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Connection::QueryF(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Do we even have enough arguments?
    if (top <= 1)
        return sq_throwerror(vm, "Missing the query string");
    // Obtain the connection instance
    Var< Connection * > inst(vm, 1);
    // Do we have a valid connection instance?
    if (!inst.value)
        return sq_throwerror(vm, "Invalid connection instance");
    // Do we have a valid connection reference?
    else if (!inst.value->m_Handle)
        return sq_throwerror(vm, "Invalid SQLite connection reference");
    // Is the specified message value a string or something convertible to string?
    else if (top == 2 && ((sq_gettype(vm, -1) == OT_STRING) || !SQ_FAILED(sq_tostring(vm, -1))))
    {
        CCStr sql = NULL;
        // Attempt to retrieve the string from the stack
        if (SQ_FAILED(sq_getstring(vm, -1, &sql)))
        {
            // If the value was converted to a string then pop the string
            sq_pop(vm, sq_gettop(vm) - top);
            // Now we can throw the error message
            return sq_throwerror(vm, "Unable to retrieve the query");
        }
        // Prevent the object from being destroyed once we pop it
        Var< Object > obj(vm, -1);
        // If the value was converted to a string then pop the string
        sq_pop(vm, sq_gettop(vm) - top);
        // Attempt to create a statement with the specified query
        try
        {
            ClassType< Statement >::PushInstance(vm, new Statement(inst.value->m_Handle, sql));
        }
        catch (const Sqrat::Exception & e)
        {
            return sq_throwerror(vm, e.Message().c_str());
        }
    }
    // Do we have enough values to call the format function?
    else if (top > 2)
    {
        SStr sql = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 3, &len, &sql);
        // Did the format failed?
        if (SQ_FAILED(ret))
            return ret; // Propagate the exception
        // Attempt to create a statement with the specified query
        try
        {
            ClassType< Statement >::PushInstance(vm, new Statement(inst.value->m_Handle, sql));
        }
        catch (const Sqrat::Exception & e)
        {
            return sq_throwerror(vm, e.Message().c_str());
        }
    }
    // All methods of retrieving the string value failed
    else
        return sq_throwerror(vm, "Unable to extract the query string");
    // At this point we should have a return value on the stack
    return 1;
}

} // Namespace:: SqMod
