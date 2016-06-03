// ------------------------------------------------------------------------------------------------
#include "Connection.hpp"
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
Int32 Connection::Cmp(const Connection & o) const
{
    if (m_Handle == o.m_Handle)
    {
        return 0;
    }
    else if (m_Handle.HndPtr() > o.m_Handle.HndPtr())
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
CSStr Connection::ToString() const
{
    // Do we have a valid handle?
    if (m_Handle)
    {
        ToStringF("%s:%s@%s:%u", m_Handle->mUser.c_str(), m_Handle->mPass.c_str(),
                                    m_Handle->mHost.c_str(), m_Handle->mPort);
    }
    // Default to an epty string
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
SQInteger Connection::GetErrNo() const
{
    // Validate the managed handle
    m_Handle.Validate();
    // Return the requested information
    return static_cast< SQInteger >(mysql_errno(m_Handle));
}

// ------------------------------------------------------------------------------------------------
CSStr Connection::GetErrStr() const
{
    // Validate the managed handle
    m_Handle.Validate();
    // Return the requested information
    return mysql_error(m_Handle);
}

// ------------------------------------------------------------------------------------------------
void Connection::SetName(CSStr name)
{
    // Validate the managed handle
    m_Handle.Validate();
    // Attempt to select the database with the given name
    if (mysql_select_db(m_Handle, name) != 0)
    {
        THROW_CURRENT(m_Handle, "Cannot select database");
    }
    // Remember the database name
    m_Handle->mName.assign(name);
}

// ------------------------------------------------------------------------------------------------
void Connection::SetCharset(CSStr charset)
{
    // Validate the managed handle
    m_Handle.Validate();
    // Attempt to Set the default character set for the managed connection
    if (mysql_set_character_set(m_Handle, charset) != 0)
    {
        THROW_CURRENT(m_Handle, "Cannot apply character set");
    }
    // Remember the character set
    m_Handle->mCharset.assign(charset);
}

// ------------------------------------------------------------------------------------------------
void Connection::SetAutoCommit(bool toggle)
{
    // Validate the managed handle
    m_Handle.Validate();
    // Attempt to toggle auto-commit if necessary
    if (m_Handle->mAutoCommit != toggle && mysql_autocommit(m_Handle, toggle) != 0)
    {
        THROW_CURRENT(m_Handle, "Cannot toggle auto-commit");
    }
    else
    {
        m_Handle->mAutoCommit = toggle;
    }
}

// ------------------------------------------------------------------------------------------------
Object Connection::Execute(CSStr query)
{
    // Validate the managed handle
    m_Handle.Validate();
    // Perform the requested operation
    return MakeULongObj(m_Handle->Execute(query));
}

// ------------------------------------------------------------------------------------------------
Object Connection::Insert(CSStr query)
{
    // Validate the managed handle
    m_Handle.Validate();
    // Make sure the specified query is valid
    if (!query || *query == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    else if (mysql_real_query(m_Handle, query, std::strlen(query)) != 0)
    {
        THROW_CURRENT(m_Handle, "Unable to execute query");
    }
    // Return the identifier of the inserted row
    return MakeULongObj(mysql_insert_id(m_Handle));
}

// ------------------------------------------------------------------------------------------------
ResultSet Connection::Query(CSStr query)
{
    // Validate the managed handle
    m_Handle.Validate();
    // Make sure the specified query is valid
    if (!query || *query == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Attempt to execute the specified query
    else if (mysql_real_query(m_Handle, query, std::strlen(query)) != 0)
    {
        THROW_CURRENT(m_Handle, "Unable to execute query");
    }
    // Return the identifier of the inserted row
    return ResultSet(ResHnd(m_Handle));
}

// ------------------------------------------------------------------------------------------------
Statement Connection::GetStatement(CSStr query)
{
    // Validate the managed handle
    m_Handle.Validate();
    // Return the requested information
    return Statement(m_Handle, query);
}

// ------------------------------------------------------------------------------------------------
Transaction Connection::GetTransaction()
{
    return Transaction();
}

} // Namespace:: SqMod
