// ------------------------------------------------------------------------------------------------
#include "Library/MySQL/Connection.hpp"
#include "Library/MySQL/Account.hpp"
#include "Library/MySQL/Statement.hpp"
#include "Library/MySQL/ResultSet.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>
#include <vector>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Connection::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMySQLConnection");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Connection::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL connection reference =>[%s:%d]", file, line);
    }
}
#else
void Connection::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL connection reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Connection::ValidateCreated(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL connection reference =>[%s:%d]", file, line);
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid MySQL connection =>[%s:%d]", file, line);
    }
}
#else
void Connection::ValidateCreated() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL connection reference");
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid MySQL connection");
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
Object Connection::Insert(CSStr query)
{
    // Make sure the specified query is valid
    if (!query || *query == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    else if (mysql_real_query(SQMOD_GET_CREATED(*this)->mPtr, query, std::strlen(query)) != 0)
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Unable to execute MySQL query");
    }
    // Return the identifier of the inserted row
    return Object(SqTypeIdentity< ULongInt >{}, SqVM(), mysql_insert_id(m_Handle->mPtr));
}

// ------------------------------------------------------------------------------------------------
ResultSet Connection::Query(CSStr query)
{
    // Make sure the specified query is valid
    if (!query || *query == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    else if (mysql_real_query(SQMOD_GET_CREATED(*this)->mPtr, query, std::strlen(query)) != 0)
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Unable to execute MySQL query");
    }
    // Return the identifier of the inserted row
    return ResultSet(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Statement Connection::GetStatement(CSStr query)
{
    return Statement(SQMOD_GET_CREATED(*this), query);
}

// ------------------------------------------------------------------------------------------------
SQInteger Connection::ExecuteF(HSQUIRRELVM vm)
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
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid connection instance?
    if (!conn)
    {
        return sq_throwerror(vm, "Invalid MySQL connection instance");
    }
    // Validate the connection info
    try
    {
        SQMOD_VALIDATE_CREATED(*conn);
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Make sure the query string is valid
    else if (!(val.mPtr) || *(val.mPtr) == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    try
    {
        Var< ULongInt >::push(vm, ULongInt(conn->m_Handle->Execute(val.mPtr, val.mLen)));
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // This function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Connection::InsertF(HSQUIRRELVM vm)
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
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid connection instance?
    if (!conn)
    {
        return sq_throwerror(vm, "Invalid MySQL connection instance");
    }
    // Validate the connection info
    try
    {
        SQMOD_VALIDATE_CREATED(*conn);
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Make sure the query string is valid
    else if (!(val.mPtr) || *(val.mPtr) == '\0')
    {
        return sq_throwerror(vm, "Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    try
    {
        if (mysql_real_query(conn->m_Handle->mPtr, val.mPtr, val.mLen) != 0)
        {
            SQMOD_THROW_CURRENT(*(conn->m_Handle), "Unable to execute MySQL query");
        }
        // Return the identifier of the inserted row
        Var< ULongInt >::push(vm, ULongInt(mysql_insert_id(conn->m_Handle->mPtr)));
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // This function returned a value
    return 1;
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
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid connection instance?
    if (!conn)
    {
        return sq_throwerror(vm, "Invalid MySQL connection instance");
    }
    // Validate the connection info
    try
    {
        SQMOD_VALIDATE_CREATED(*conn);
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }
    // Make sure the query string is valid
    else if (!(val.mPtr) || *(val.mPtr) == '\0')
    {
        return sq_throwerror(vm, "Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    try
    {
        if (mysql_real_query(conn->m_Handle->mPtr, val.mPtr, val.mLen) != 0)
        {
            SQMOD_THROW_CURRENT(*(conn->m_Handle), "Unable to execute MySQL query");
        }
        // Return a new instance with the obtained result set
        Var< ResultSet * >::push(vm, new ResultSet(conn->m_Handle));
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.what());
    }
    // This function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
LightObj Connection::EscapeString(StackStrF & str)
{
    // Is there even a string to escape?
    if (str.mLen <= 0)
    {
        return LightObj(_SC(""), 0, str.mVM); // Default to empty string
    }
    // Allocate a buffer for the given string
    std::vector< SQChar > buffer(str.mLen * 2 + 1);
    // Attempt to ecape the specified string
    const Ulong len = mysql_real_escape_string(m_Handle->mPtr, buffer.data(), str.mPtr, str.mLen);
    // Return the resulted string
    return LightObj(buffer.data(), static_cast< SQInteger >(len), str.mVM);
}

// ================================================================================================
void Register_Connection(Table & sqlns)
{
    sqlns.Bind(_SC("Connection")
        , Class< Connection >(sqlns.GetVM(), _SC("SqMySQLConnection"))
        // Constructors
        .Ctor()
        .Ctor< const Account & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Connection::Cmp)
        .SquirrelFunc(_SC("_typename"), &Connection::Typename)
        .Func(_SC("_tostring"), &Connection::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Connection::IsValid)
        .Prop(_SC("Connected"), &Connection::IsConnected)
        .Prop(_SC("References"), &Connection::GetRefCount)
        .Prop(_SC("ErrNo"), &Connection::GetErrNo)
        .Prop(_SC("ErrStr"), &Connection::GetErrStr)
        .Prop(_SC("LastErrNo"), &Connection::GetLastErrNo)
        .Prop(_SC("LastErrStr"), &Connection::GetLastErrStr)
        .Prop(_SC("Port"), &Connection::GetPortNum)
        .Prop(_SC("Host"), &Connection::GetHost)
        .Prop(_SC("User"), &Connection::GetUser)
        .Prop(_SC("Pass"), &Connection::GetPass)
        .Prop(_SC("Name"), &Connection::GetName, &Connection::SetName)
        .Prop(_SC("Socket"), &Connection::GetSocket)
        .Prop(_SC("Flags"), &Connection::GetFlags)
        .Prop(_SC("SSL_Key"), &Connection::GetSSL_Key)
        .Prop(_SC("SSL_Cert"), &Connection::GetSSL_Cert)
        .Prop(_SC("SSL_CA"), &Connection::GetSSL_CA)
        .Prop(_SC("SSL_CA_Path"), &Connection::GetSSL_CA_Path)
        .Prop(_SC("SSL_Cipher"), &Connection::GetSSL_Cipher)
        .Prop(_SC("Charset"), &Connection::GetCharset, &Connection::SetCharset)
        .Prop(_SC("AutoCommit"), &Connection::GetAutoCommit, &Connection::SetAutoCommit)
        .Prop(_SC("InTransaction"), &Connection::GetInTransaction)
        // Member Methods
        .Func(_SC("Disconnect"), &Connection::Disconnect)
        .Func(_SC("SelectDb"), &Connection::SetName)
        .Func(_SC("Execute"), &Connection::Execute)
        .Func(_SC("Insert"), &Connection::Insert)
        .Func(_SC("Query"), &Connection::Query)
        .Func(_SC("Statement"), &Connection::GetStatement)
        //.Func(_SC("Transaction"), &Connection::GetTransaction)
        .FmtFunc(_SC("EscapeString"), &Connection::EscapeString)
        // Squirrel Methods
        .SquirrelFunc(_SC("ExecuteF"), &Connection::ExecuteF)
        .SquirrelFunc(_SC("InsertF"), &Connection::InsertF)
        .SquirrelFunc(_SC("QueryF"), &Connection::QueryF)
    );
}

} // Namespace:: SqMod
