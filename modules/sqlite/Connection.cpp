// ------------------------------------------------------------------------------------------------
#include "Connection.hpp"
#include "Statement.hpp"

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
void Connection::TraceOutput(void * /*ptr*/, CCStr sql)
{
    SqMod_LogInf("SQLite Trace: %s", sql);
}

// ------------------------------------------------------------------------------------------------
void Connection::ProfileOutput(void * /*ptr*/, CCStr sql, sqlite3_uint64 time)
{
    SqMod_LogInf("SQLite profile (time: %llu): %s", time, sql);
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Connection::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite connection reference =>[%s:%d]", file, line);
    }
}
#else
void Connection::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite connection reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Connection::ValidateCreated(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite connection reference =>[%s:%d]", file, line);
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite connection =>[%s:%d]", file, line);
    }
}
#else
void Connection::ValidateCreated() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite connection reference");
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite connection");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ConnRef & Connection::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const ConnRef & Connection::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ConnRef & Connection::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const ConnRef & Connection::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
void Connection::Open(const StackStrF & name)
{
    // Should we create a connection handle?
    if (!m_Handle)
    {
        m_Handle = ConnRef(new ConnHnd());
    }
    // Make sure another database isn't opened
    if (SQMOD_GET_VALID(*this)->mPtr != nullptr)
    {
        STHROWF("Already referencing a valid database connection");
    }
    // Perform the requested operation
    m_Handle->Create(name.mPtr, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
}

// ------------------------------------------------------------------------------------------------
void Connection::Open(const StackStrF & name, Int32 flags)
{
    // Should we create a connection handle?
    if (!m_Handle)
    {
        m_Handle = ConnRef(new ConnHnd());
    }
    // Make sure another database isn't opened
    if (SQMOD_GET_VALID(*this)->mPtr != nullptr)
    {
        STHROWF("Already referencing a valid database connection");
    }
    // Perform the requested operation
    m_Handle->Create(name.mPtr, flags, nullptr);
}

// ------------------------------------------------------------------------------------------------
void Connection::Open(const StackStrF & name, Int32 flags, const StackStrF & vfs)
{
    // Should we create a connection handle?
    if (!m_Handle)
    {
        m_Handle = ConnRef(new ConnHnd());
    }
    // Make sure another database isn't opened
    if (SQMOD_GET_VALID(*this)->mPtr != nullptr)
    {
        STHROWF("Already referencing a valid database connection");
    }
    // Perform the requested operation
    m_Handle->Create(name.mPtr, flags, vfs.mPtr);
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::Exec(const StackStrF & str)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Attempt to execute the specified query
    m_Handle->mStatus = sqlite3_exec(m_Handle->mPtr, str.mPtr, nullptr, nullptr, nullptr);
    // Validate the execution result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF("Unable to execute query [%s]", m_Handle->ErrMsg());
    }
    // Return rows affected by this query
    return sqlite3_changes(m_Handle->mPtr);
}

// ------------------------------------------------------------------------------------------------
Object Connection::Query(const StackStrF & str) const
{
    SQMOD_VALIDATE_CREATED(*this);
    // Return the requested information
    return Object(new Statement(m_Handle, str));
}

// ------------------------------------------------------------------------------------------------
void Connection::Queue(const StackStrF & str)
{
    SQMOD_VALIDATE(*this);
    // Is there a query to commit?
    if (!str.mLen || IsQueryEmpty(str.mPtr))
    {
        STHROWF("No query string to queue");
    }
    // Add the specified string to the queue
    m_Handle->mQueue.emplace_back(str.mPtr, str.mLen);
}

// ------------------------------------------------------------------------------------------------
bool Connection::IsReadOnly() const
{
    // Request the desired information
    const int result = sqlite3_db_readonly(SQMOD_GET_CREATED(*this)->mPtr, "main");
    // Verify the result
    if (result == -1)
    {
        STHROWF("'main' is not the name of a database on connection");
    }
    // Return the requested information
    return (result != 1);
}

// ------------------------------------------------------------------------------------------------
bool Connection::TableExists(const StackStrF & name) const
{
    // Prepare a statement to inspect the master table
    Statement stmt(SQMOD_GET_CREATED(*this), "SELECT count(*) FROM [sqlite_master] WHERE [type]='table' AND [name]=?");
    // Could the statement be created?
    if (stmt.IsValid())
    {
        // Bind the specified name onto the statement parameter
        Parameter(stmt.GetHandle(), 1).SetString(name);
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
void Connection::SetTracing(bool toggle)
{
    // Check whether changes are necessary
    if (SQMOD_GET_CREATED(*this)->mTrace == toggle)
    {
        return; // No point in proceeding
    }
    // Do we have to disable it?
    else if (m_Handle->mTrace)
    {
        sqlite3_trace(m_Handle->mPtr, nullptr, nullptr);
    }
    // Go ahead and enable tracing
    else
    {
        sqlite3_trace(m_Handle->mPtr, &Connection::TraceOutput, nullptr);
    }
}

// ------------------------------------------------------------------------------------------------
void Connection::SetProfiling(bool toggle)
{
    // Check whether changes are necessary
    if (SQMOD_GET_CREATED(*this)->mProfile == toggle)
    {
        return; // No point in proceeding
    }
    // Do we have to disable it?
    else if (m_Handle->mProfile)
    {
        sqlite3_profile(m_Handle->mPtr, nullptr, nullptr);
    }
    // Go ahead and enable profiling
    else
    {
        sqlite3_profile(m_Handle->mPtr, &Connection::ProfileOutput, nullptr);
    }
}

// ------------------------------------------------------------------------------------------------
void Connection::SetBusyTimeout(Int32 millis)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Apply the requested timeout
    if ((m_Handle->mStatus = sqlite3_busy_timeout(m_Handle->mPtr, millis)) != SQLITE_OK)
    {
        STHROWF("Unable to set busy timeout [%s]", m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::GetInfo(Int32 operation, bool highwater, bool reset)
{
    // Where to retrieve the information
    Int32 cur_value;
    Int32 hiwtr_value;
    // Attempt to retrieve the specified information
    if ((m_Handle->mStatus = sqlite3_db_status(SQMOD_GET_CREATED(*this)->mPtr, operation, &cur_value, &hiwtr_value, reset)) != SQLITE_OK)
    {
        STHROWF("Unable to get runtime status information", m_Handle->ErrMsg());
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
    SQMOD_VALIDATE(*this);
    // Perform the requested operation
    m_Handle->mQueue.reserve(m_Handle->mQueue.size() + num);
}

// ------------------------------------------------------------------------------------------------
void Connection::PopQueue()
{
    SQMOD_VALIDATE(*this);
    // Perform the requested operation
    if (!SQMOD_GET_VALID(*this)->mQueue.empty())
    {
        m_Handle->mQueue.pop_back();
    }
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::Flush()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Perform the requested operation
    return m_Handle->Flush(m_Handle->mQueue.size(), NullObject(), NullFunction());
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::Flush(SQInteger num)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Perform the requested operation
    return m_Handle->Flush(ConvTo< Uint32 >::From(num), NullObject(), NullFunction());
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::Flush(Object & env, Function & func)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Perform the requested operation
    return m_Handle->Flush(m_Handle->mQueue.size(), env, func);
}

// ------------------------------------------------------------------------------------------------
Int32 Connection::Flush(SQInteger num, Object & env, Function & func)
{
    SQMOD_VALIDATE_CREATED(*this);
    // Perform the requested operation
    return m_Handle->Flush(ConvTo< Uint32 >::From(num), env, func);
}

// ================================================================================================
void Register_Connection(Table & sqlns)
{
    sqlns.Bind(_SC("Connection"),
        Class< Connection >(sqlns.GetVM(), _SC("SqSQLiteConnection"))
        // Constructors
        .Ctor()
        .Ctor< const StackStrF & >()
        .Ctor< const StackStrF &, Int32 >()
        .Ctor< const StackStrF &, Int32, const StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Connection::Typename)
        .Func(_SC("_tostring"), &Connection::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Connection::IsValid)
        .Prop(_SC("Connected"), &Connection::IsConnected)
        .Prop(_SC("References"), &Connection::GetRefCount)
        .Prop(_SC("Status"), &Connection::GetStatus)
        .Prop(_SC("Flags"), &Connection::GetFlags)
        .Prop(_SC("Name"), &Connection::GetName)
        .Prop(_SC("VFS"), &Connection::GetVFS)
        .Prop(_SC("ErrCode"), &Connection::GetErrorCode)
        .Prop(_SC("ExErrCode"), &Connection::GetExtendedErrorCode)
        .Prop(_SC("ExtendedErrCode"), &Connection::GetExtendedErrorCode)
        .Prop(_SC("ErrStr"), &Connection::GetErrStr)
        .Prop(_SC("ErrMsg"), &Connection::GetErrMsg)
        .Prop(_SC("ReadOnly"), &Connection::IsReadOnly)
        .Prop(_SC("Autocommit"), &Connection::GetAutoCommit)
        .Prop(_SC("LastInsertRowId"), &Connection::GetLastInsertRowID)
        .Prop(_SC("Changes"), &Connection::GetChanges)
        .Prop(_SC("TotalChanges"), &Connection::GetTotalChanges)
        .Prop(_SC("Trace"), &Connection::GetTracing, &Connection::SetTracing)
        .Prop(_SC("Profile"), &Connection::GetProfiling, &Connection::SetProfiling)
        .Prop(_SC("QueueSize"), &Connection::QueueSize)
        // Member Methods
        .Func(_SC("Release"), &Connection::Release)
        .FmtFunc(_SC("Exec"), &Connection::Exec)
        .FmtFunc(_SC("Queue"), &Connection::Queue)
        .FmtFunc(_SC("Query"), &Connection::Query)
        .FmtFunc(_SC("TableExists"), &Connection::TableExists)
        .Func(_SC("InterruptOperation"), &Connection::InterruptOperation)
        .Func(_SC("SetBusyTimeout"), &Connection::SetBusyTimeout)
        .Func(_SC("ReleaseMemory"), &Connection::ReleaseMemory)
        .Func(_SC("ReserveQueue"), &Connection::ReserveQueue)
        .Func(_SC("CompactQueue"), &Connection::CompactQueue)
        .Func(_SC("ClearQueue"), &Connection::ClearQueue)
        .Func(_SC("PopQueue"), &Connection::PopQueue)
        // Member Overloads
        .Overload< void (Connection::*)(const StackStrF &) >(_SC("Open"), &Connection::Open)
        .Overload< void (Connection::*)(const StackStrF &, Int32) >(_SC("Open"), &Connection::Open)
        .Overload< void (Connection::*)(const StackStrF &, Int32, const StackStrF &) >(_SC("Open"), &Connection::Open)
        .Overload< Int32 (Connection::*)(Int32) >(_SC("GetInfo"), &Connection::GetInfo)
        .Overload< Int32 (Connection::*)(Int32, bool) >(_SC("GetInfo"), &Connection::GetInfo)
        .Overload< Int32 (Connection::*)(Int32, bool, bool) >(_SC("GetInfo"), &Connection::GetInfo)
        .Overload< Int32 (Connection::*)(void) >(_SC("Flush"), &Connection::Flush)
        .Overload< Int32 (Connection::*)(SQInteger) >(_SC("Flush"), &Connection::Flush)
        .Overload< Int32 (Connection::*)(Object &, Function &) >(_SC("Flush"), &Connection::Flush)
        .Overload< Int32 (Connection::*)(SQInteger, Object &, Function &) >(_SC("Flush"), &Connection::Flush)
    );
}

} // Namespace:: SqMod
