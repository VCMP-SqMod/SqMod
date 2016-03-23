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
        STHROWF("Invalid SQLite connection reference");
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
        STHROWF("Unable to execute query [%s]", m_Handle.ErrMsg());
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
        STHROWF("No query string to queue");
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
        STHROWF("'main' is not the name of a database on connection");
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
        STHROWF("Unable to set busy timeout [%s]", m_Handle.ErrMsg());
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
        STHROWF("Unable to get runtime status information", m_Handle.ErrMsg());
    // Return the high-water value if requested
    else if (highwater)
        return hiwtr_value;
    // Return the requested information
    return cur_value;
}

// ------------------------------------------------------------------------------------------------
void Connection::ReserveQueue(Uint32 num)
{
    // Validate the handle
    Validate();
    // Perform the requested operation
    m_Handle->mQueue.reserve(m_Handle->mQueue.size() + num);
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
SQInteger Connection::ExecF(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the query value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing query value");
    }
    // The connection instance
    Connection * conn = nullptr;
    // Attempt to extract the argument values
    try
    {
        conn = Var< Connection * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Do we have a valid connection instance?
    if (!conn)
    {
        return sq_throwerror(vm, "Invalid SQLite connection instance");
    }
    // Do we have a valid connection identifier?
    else if (!conn->m_Handle)
    {
        return sq_throwerror(vm, "Invalid SQLite connection reference");
    }
    // Do we have enough values to call the format function?
    else if (top > 2)
    {
        SStr sql = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 2, &len, &sql);
        // Did the format failed?
        if (SQ_FAILED(ret))
        {
            return ret; // Propagate the exception
        }
        // Attempt to execute the specified query
        else if ((conn->m_Handle = sqlite3_exec(conn->m_Handle, sql, NULL, NULL, NULL)) != SQLITE_OK)
        {
            // Generate the query message first
            String msg("Unable to execute query ");
            // (we can't use FmtStr here because Squirrel doesn't make a copy of the message)
            msg.push_back('[');
            msg.append(conn->m_Handle.ErrMsg());
            msg.push_back(']');
            // Now throw the message
            return sq_throwerror(vm, msg.c_str());
        }
    }
    else
    {
        // Attempt to retrieve the value from the stack as a string
        Var< CSStr > sql(vm, 2);
        // See if the obtained value is a valid query string
        if (!sql.value)
        {
            return sq_throwerror(vm, "Unable to retrieve the query");
        }
        // Attempt to execute the specified query
        else if ((conn->m_Handle = sqlite3_exec(conn->m_Handle, sql.value, NULL, NULL, NULL)) != SQLITE_OK)
        {
            // Generate the query message first
            String msg("Unable to execute query ");
            // (we can't use FmtStr here because Squirrel doesn't make a copy of the message)
            msg.push_back('[');
            msg.append(conn->m_Handle.ErrMsg());
            msg.push_back(']');
            // Now throw the message
            return sq_throwerror(vm, msg.c_str());
        }
    }
    // Push the number of changes onto the stack
    sq_pushinteger(vm, sqlite3_changes(conn->m_Handle));
    // This function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Connection::QueueF(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the query value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing query value");
    }
    // The connection instance
    Connection * conn = nullptr;
    // Attempt to extract the argument values
    try
    {
        conn = Var< Connection * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Do we have a valid connection instance?
    if (!conn)
    {
        return sq_throwerror(vm, "Invalid SQLite connection instance");
    }
    // Do we have a valid connection identifier?
    else if (!conn->m_Handle)
    {
        return sq_throwerror(vm, "Invalid SQLite connection reference");
    }
    // Do we have enough values to call the format function?
    else if (top > 2)
    {
        SStr sql = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 2, &len, &sql);
        // Did the format failed?
        if (SQ_FAILED(ret))
        {
            return ret; // Propagate the exception
        }
        // Attempt to queue the specified query
        conn->m_Handle->mQueue.emplace_back(sql);
    }
    else
    {
        // Attempt to retrieve the value from the stack as a string
        Var< CSStr > sql(vm, 2);
        // See if the obtained value is a valid query string
        if (!sql.value)
        {
            return sq_throwerror(vm, "Unable to retrieve the query");
        }
        // Attempt to queue the specified query
        conn->m_Handle->mQueue.emplace_back(sql.value);
    }
    // This function does not return a value
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQInteger Connection::QueryF(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the query value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing query value");
    }
    // The connection instance
    Connection * conn = nullptr;
    // Attempt to extract the argument values
    try
    {
        conn = Var< Connection * >(vm, 1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Do we have a valid connection instance?
    if (!conn)
    {
        return sq_throwerror(vm, "Invalid SQLite connection instance");
    }
    // Do we have a valid connection identifier?
    else if (!conn->m_Handle)
    {
        return sq_throwerror(vm, "Invalid SQLite connection reference");
    }
    // Do we have enough values to call the format function?
    else if (top > 2)
    {
        SStr sql = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 2, &len, &sql);
        // Did the format failed?
        if (SQ_FAILED(ret))
        {
            return ret; // Propagate the exception
        }
        // Attempt to create a statement with the specified query
        try
        {
            ClassType< Statement >::PushInstance(vm, new Statement(conn->m_Handle, sql));
        }
        catch (const Sqrat::Exception & e)
        {
            return sq_throwerror(vm, e.Message().c_str());
        }
    }
    else
    {
        // Attempt to retrieve the value from the stack as a string
        Var< CSStr > sql(vm, 2);
        // See if the obtained value is a valid query string
        if (!sql.value)
        {
            return sq_throwerror(vm, "Unable to retrieve the query");
        }
        // Attempt to create a statement with the specified query
        try
        {
            ClassType< Statement >::PushInstance(vm, new Statement(conn->m_Handle, sql.value));
        }
        catch (const Sqrat::Exception & e)
        {
            return sq_throwerror(vm, e.Message().c_str());
        }
    }
    // This function returned a value
    return 1;
}

} // Namespace:: SqMod
