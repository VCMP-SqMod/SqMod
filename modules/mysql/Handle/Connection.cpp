// ------------------------------------------------------------------------------------------------
#include "Handle/Connection.hpp"
#include "Account.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void ConnHnd::Validate() const
{
    // Is the handle valid?
    if ((m_Hnd == nullptr) || (m_Hnd->mPtr == nullptr))
    {
        STHROWF("Invalid MySQL connection reference");
    }
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ConnHnd::Handle::ThrowCurrent(CCStr act, CCStr file, Int32 line)
#else
    void ConnHnd::Handle::ThrowCurrent(CCStr act)
#endif // _DEBUG
{
    // Grab the error number and message
    mErrNo = mysql_errno(mPtr);
    mErrStr.assign(mysql_error(mPtr));
    // Throw the exception with the resulted message
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    throw Sqrat::Exception(FmtStr("%s (%u) : %s =>[%s:%d]", act,
                            mErrNo, mErrStr.c_str(), file, line));
#else
    throw Sqrat::Exception(FmtStr("%s (%u) : %s", act,
                            mErrNo, mErrStr.c_str()));
#endif // _DEBUG
}

// ------------------------------------------------------------------------------------------------
ConnHnd::Handle::Handle(const Account & acc)
    : mPtr(mysql_init(NULL))
    , mRef(0)
    , mErrNo(0)
    , mErrStr(_SC(""))
    , mPort(acc.GetPortNum())
    , mHost(acc.GetHost())
    , mUser(acc.GetUser())
    , mPass(acc.GetPass())
    , mName(acc.GetName())
    , mSocket(acc.GetSocket())
    , mFlags(acc.GetFlags())
    , mSSL_Key(acc.GetSSL_Key())
    , mSSL_Cert(acc.GetSSL_Cert())
    , mSSL_CA(acc.GetSSL_CA())
    , mSSL_CA_Path(acc.GetSSL_CA_Path())
    , mSSL_Cipher(acc.GetSSL_Cipher())
    , mCharset()
    , mQueue()
    , mAutoCommit(acc.GetAutoCommit())
    , mInTransaction(false)
{
    // See if a connection handle could be initialized
    if (!mPtr)
    {
        THROW_CURRENT_HND((*this), "Cannot create MYSQL object");
    }
    // Attempt to configure SSL if specified
    else if (!mSSL_Key.empty() && mysql_ssl_set(mPtr, mSSL_Key.c_str(), mSSL_Cert.c_str(), mSSL_CA.c_str(),
                                                mSSL_CA_Path.c_str(), mSSL_Cipher.c_str()) != 0)
    {
        THROW_CURRENT_HND((*this), "Cannot configure SSL");
    }
    // Attempt to connect to the specified server
    else if (!mysql_real_connect(mPtr, mHost.c_str(), mUser.c_str(), mPass.c_str(),
                            mName.empty() ? nullptr : mName.c_str(), mPort,
                            mSocket.empty() ? nullptr : mSocket.c_str(), mFlags))
    {
        THROW_CURRENT_HND((*this), "Cannot connect to database");
    }
    // Attempt configure the auto-commit option
    else if (mysql_autocommit(mPtr, mAutoCommit) != 0)
    {
        THROW_CURRENT_HND((*this), "Cannot configure auto-commit");
    }
    // Get iterators to the options container
    Account::Options::const_iterator itr = acc.GetOptions().cbegin();
    Account::Options::const_iterator end = acc.GetOptions().cend();
    // Process each option in the container
    for (String sql(128, 0); itr != end; ++itr)
    {
        // Prepare the SQL query that applies the option
        sql.assign("SET OPTION ");
        sql.append(itr->first);
        sql.append("=");
        sql.append(itr->second);
        // Execute the resulted query
        if (Execute(sql.c_str(), static_cast< Ulong >(sql.size())) != 1)
        {
            THROW_CURRENT_HND((*this), "Unable to apply option");
        }
    }
    MY_CHARSET_INFO charsetinfo;
    // Grab the information about the current character set
    mysql_get_character_set_info(mPtr, &charsetinfo);
    // We only need the character set name
    if (charsetinfo.name != nullptr)
    {
        mCharset.assign(charsetinfo.name);
    }
}

// ------------------------------------------------------------------------------------------------
ConnHnd::Handle::~Handle()
{

}

// ------------------------------------------------------------------------------------------------
void ConnHnd::Handle::Disconnect()
{
    if (mPtr != nullptr)
    {
        mysql_close(mPtr);
        // mysql_init() called mysql_thread_init() therefore it needs to clear memory
        // when the MYSQL handle is closed
        mysql_thread_end();
        // Prevent further use of this handle
        mPtr = nullptr;
    }
}

// ------------------------------------------------------------------------------------------------
Int32 ConnHnd::Handle::Flush(Uint32 /*num*/, Object & /*env*/, Function & /*func*/)
{
    return 0;
}

// ------------------------------------------------------------------------------------------------
Ulong ConnHnd::Handle::Execute(CSStr query, Ulong size)
{
    // Make sure that we are connected
    if (!mPtr)
    {
        STHROWF("Invalid MySQL connection");
    }
    // Make sure the specified query is valid
    else if (!query || *query == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Are we supposed to compute the size?
    else if (!size)
    {
        size = std::strlen(query);
    }
    // Attempt to execute the specified query
    else if (mysql_real_query(mPtr, query, size) != 0)
    {
        THROW_CURRENT_HND((*this), "Unable to execute query");
    }
    // Where the number of affected rows will be stored
    Ulong affected = 0UL;
    // Count the number of affected rows by any "upsert" statement
    while (true)
    {
        // Attempt to retrieve a buffered result set from the executed query
        ResType * result = mysql_store_result(mPtr);
        // If we have a result, then this was a SELECT statement and we should not count it
        // because it returns the number of selected rows and not modified/affected
        if (result)
        {
            // Just, free the memory associated with the obtained result set
            mysql_free_result(result);
        }
        // Non SELCT queries should have a field count of 0
        else if (mysql_field_count(mPtr) == 0)
        {
            // Sum the number of affected rows by this statement
            affected += mysql_affected_rows(mPtr);
        }
        else
        {
            THROW_CURRENT_HND((*this), "Unable to count affected rows");
        }
        // Prepare the next result from the executed query
        // If return code is 0 then we have a result ready to process
        const Int32 status = mysql_next_result(mPtr);
        // If return code is higher than 0 then an error occurred
        if (status > 0)
        {
            THROW_CURRENT_HND((*this), "Unable to prepare next result");
        }
        // If return code is less than 0 then there are no results left
        else if (status < 0)
        {
            break;
        }
    }
    // Return the number of affected rows
    return affected;
}

} // Namespace:: SqMod
