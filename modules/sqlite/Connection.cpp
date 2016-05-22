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
    static const SQChar name[] = _SC("SqSQLiteConnection");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
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
    {
        m_Handle->Create(name, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    }
}

// ------------------------------------------------------------------------------------------------
Connection::Connection(CSStr name, Int32 flags)
    : m_Handle(name)
{
    if (m_Handle.m_Hnd)
    {
        m_Handle->Create(name, flags, nullptr);
    }
}

// ------------------------------------------------------------------------------------------------
Connection::Connection(CSStr name, Int32 flags, CSStr vfs)
    : m_Handle(name)
{
    if (m_Handle.m_Hnd)
    {
        m_Handle->Create(name, flags, vfs);
    }
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::Exec(CSStr str)
{
    // Validate the handle
    m_Handle.Validate();
    // Attempt to execute the specified query
    if ((m_Handle = sqlite3_exec(m_Handle, str, nullptr, nullptr, nullptr)) != SQLITE_OK)
    {
        STHROWF("Unable to execute query [%s]", m_Handle.ErrMsg());
    }
    // Return rows affected by this query
    return sqlite3_changes(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Object Connection::Query(CSStr str) const
{
    // Validate the handle
    m_Handle.Validate();
    // Return the requested information
    return Object(new Statement(m_Handle, str));
}

// ------------------------------------------------------------------------------------------------
void Connection::Queue(CSStr str)
{
    // Validate the handle
    m_Handle.Validate();
    // Is there a query to commit?
    if (IsQueryEmpty(str))
    {
        STHROWF("No query string to queue");
    }
    // Add the specified string to the queue
    m_Handle->mQueue.push_back(str);
}

// ------------------------------------------------------------------------------------------------
bool Connection::IsReadOnly() const
{
    // Validate the handle
    m_Handle.Validate();
    // Request the desired information
    const int result = sqlite3_db_readonly(m_Handle, "main");
    // Verify the result
    if (result == -1)
    {
        STHROWF("'main' is not the name of a database on connection");
    }
    // Return the requested information
    return (result != 1);
}

// ------------------------------------------------------------------------------------------------
bool Connection::TableExists(CCStr name) const
{
    // Validate the handle
    m_Handle.Validate();
    // Prepare a statement to inspect the master table
    Statement stmt(m_Handle, "SELECT count(*) FROM [sqlite_master] WHERE [type]='table' AND [name]=?");
    // Could the statement be created?
    if (stmt.IsValid())
    {
        // Bind the specified name onto the statement parameter
        stmt.IndexBindS(1, name);
        // Attempt to step the statement and obtain a value
        if (stmt.Step())
        {
            return (sqlite3_column_int(stmt, 0) == 1);
        }
    }
    // Assume it doesn't exist
    return false;
}

// ------------------------------------------------------------------------------------------------
Object Connection::GetLastInsertRowID() const
{
    // Validate the handle
    m_Handle.Validate();
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
    m_Handle.Validate();
    // Apply requested timeout
    if ((m_Handle = sqlite3_busy_timeout(m_Handle, millis)) != SQLITE_OK)
    {
        STHROWF("Unable to set busy timeout [%s]", m_Handle.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::GetInfo(Int32 operation, bool highwater, bool reset)
{
    // Don't even bother to continue if there's no valid connection handle
    m_Handle.Validate();
    // Where to retrieve the information
    Int32 cur_value;
    Int32 hiwtr_value;
    // Attempt to retrieve the specified information
    if ((m_Handle = sqlite3_db_status(m_Handle, operation, &cur_value, &hiwtr_value, reset)) != SQLITE_OK)
    {
        STHROWF("Unable to get runtime status information", m_Handle.ErrMsg());
    }
    // Return the high-water value if requested
    else if (highwater)
    {
        return hiwtr_value;
    }
    // Return the requested information
    return cur_value;
}

// ------------------------------------------------------------------------------------------------
void Connection::ReserveQueue(Uint32 num)
{
    // Validate the handle
    m_Handle.Validate();
    // Perform the requested operation
    m_Handle->mQueue.reserve(m_Handle->mQueue.size() + num);
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::Flush(Uint32 num)
{
    // Validate the handle
    m_Handle.Validate();
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
    m_Handle.Validate();
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
    else if (!(conn->m_Handle))
    {
        return sq_throwerror(vm, "Invalid SQLite connection reference");
    }
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Attempt to execute the specified query
    else if ((conn->m_Handle = sqlite3_exec(conn->m_Handle, val.mPtr, nullptr, nullptr, nullptr)) != SQLITE_OK)
    {
        return sq_throwerror(vm, FmtStr("Unable to execute query [%s]", conn->m_Handle.ErrMsg()));
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
    else if (!(conn->m_Handle))
    {
        return sq_throwerror(vm, "Invalid SQLite connection reference");
    }
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Attempt to queue the specified query
    conn->m_Handle->mQueue.emplace_back(val.mPtr);
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
    else if (!(conn->m_Handle))
    {
        return sq_throwerror(vm, "Invalid SQLite connection reference");
    }
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Attempt to create a statement with the specified query
    try
    {
        ClassType< Statement >::PushInstance(vm, new Statement(conn->m_Handle, val.mPtr));
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.Message().c_str());
    }
    // This function returned a value
    return 1;
}

} // Namespace:: SqMod
