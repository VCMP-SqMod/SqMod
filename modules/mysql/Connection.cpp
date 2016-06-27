// ------------------------------------------------------------------------------------------------
#include "Connection.hpp"
#include "Account.hpp"
#include "Statement.hpp"
#include "ResultSet.hpp"
#include "Transaction.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

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
    return MakeULongObj(mysql_insert_id(m_Handle->mPtr));
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
Transaction Connection::GetTransaction()
{
    return Transaction();
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
        .Func(_SC("Transaction"), &Connection::GetTransaction)
    );
}

} // Namespace:: SqMod
