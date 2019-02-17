#ifndef _SQMYSQL_CONNECTION_HPP_
#define _SQMYSQL_CONNECTION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Connection.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows management and interaction with a connection handle.
*/
class Connection
{
private:

    // --------------------------------------------------------------------------------------------
    ConnRef     m_Handle; // Reference to the actual database connection.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(CCStr file, Int32 line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ConnRef & GetValid(CCStr file, Int32 line) const;
#else
    const ConnRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ConnRef & GetCreated(CCStr file, Int32 line) const;
#else
    const ConnRef & GetCreated() const;
#endif // _DEBUG

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Connection()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Connection(const Account & acc)
        : m_Handle(new ConnHnd())
    {
        m_Handle->Create(acc);
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Connection(const ConnRef & conn)
        : m_Handle(conn)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Connection(const Connection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Connection(Connection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Connection & operator = (const Connection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Connection & operator = (Connection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Connection & o) const
    {
        if (m_Handle.Get() == o.m_Handle.Get())
        {
            return 0;
        }
        else if (m_Handle.Get() > o.m_Handle.Get())
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Handle ? mysql_get_host_info(m_Handle->mPtr) : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated connection handle.
    */
    const ConnRef & GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed connection handle was connected.
    */
    bool IsConnected() const
    {
        return m_Handle && (m_Handle->mPtr != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this connection handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current error number.
    */
    SQInteger GetErrNo() const
    {
        return static_cast< SQInteger >(mysql_errno(SQMOD_GET_CREATED(*this)->mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current error message.
    */
    CSStr GetErrStr() const
    {
        return mysql_error(SQMOD_GET_CREATED(*this)->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received error number.
    */
    SQInteger GetLastErrNo() const
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this)->mErrNo);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received error message.
    */
    const String & GetLastErrStr() const
    {
        return SQMOD_GET_VALID(*this)->mErrStr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection port number.
    */
    SQInteger GetPortNum() const
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this)->mPort);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection host address.
    */
    const String & GetHost() const
    {
        return SQMOD_GET_VALID(*this)->mHost;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection user name.
    */
    const String & GetUser() const
    {
        return SQMOD_GET_VALID(*this)->mUser;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection password.
    */
    const String & GetPass() const
    {
        return SQMOD_GET_VALID(*this)->mPass;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the selected database name.
    */
    const String & GetName() const
    {
        return SQMOD_GET_VALID(*this)->mName;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the selected database name.
    */
    void SetName(CSStr name)
    {
        // Validate the specified name
        if (!name)
        {
            STHROWF("Invalid MySQL database name");
        }
        // Attempt to select the database with the given name
        else if (mysql_select_db(SQMOD_GET_CREATED(*this)->mPtr, name) != 0)
        {
            SQMOD_THROW_CURRENT(*m_Handle, "Cannot select MySQL database");
        }
        // Remember the database name
        m_Handle->mName.assign(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection socket.
    */
    const String & GetSocket() const
    {
        return SQMOD_GET_VALID(*this)->mSocket;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection flags.
    */
    SQInteger GetFlags() const
    {
        return static_cast< SQInteger >(SQMOD_GET_VALID(*this)->mFlags);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL key.
    */
    const String & GetSSL_Key() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_Key;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate.
    */
    const String & GetSSL_Cert() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_Cert;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate authority.
    */
    const String & GetSSL_CA() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_CA;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate authority path.
    */
    const String & GetSSL_CA_Path() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_CA_Path;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL cipher.
    */
    const String & GetSSL_Cipher() const
    {
        return SQMOD_GET_VALID(*this)->mSSL_Cipher;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the default character set for the managed connection.
    */
    const String & GetCharset() const
    {
        return SQMOD_GET_VALID(*this)->mCharset;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the default character set for the managed connection.
    */
    void SetCharset(CSStr charset)
    {
        // Validate the specified string
        if (!charset)
        {
            STHROWF("Invalid charset string");
        }
        // Attempt to Set the default character set for the managed connection
        else if (mysql_set_character_set(SQMOD_GET_CREATED(*this)->mPtr, charset) != 0)
        {
            SQMOD_THROW_CURRENT(*m_Handle, "Cannot apply character set");
        }
        // Remember the character set
        m_Handle->mCharset.assign(charset);
    }

    /* --------------------------------------------------------------------------------------------
     * See whether auto-commit is enabled or not.
    */
    bool GetAutoCommit() const
    {
        return SQMOD_GET_VALID((*this))->mAutoCommit;
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether auto-commit should be enabled or not.
    */
    void SetAutoCommit(bool toggle)
    {
        // Attempt to toggle auto-commit if necessary
        if (SQMOD_GET_CREATED(*this)->mAutoCommit != toggle &&
            mysql_autocommit(m_Handle->mPtr, toggle) != 0)
        {
            SQMOD_THROW_CURRENT(*m_Handle, "Cannot toggle auto-commit");
        }
        else
        {
            m_Handle->mAutoCommit = toggle;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the connection is in the middle of a transaction.
    */
    bool GetInTransaction() const
    {
        return SQMOD_GET_VALID(*this)->mInTransaction;
    }

    /* --------------------------------------------------------------------------------------------
     * Disconnect from the currently connected database.
    */
    void Disconnect()
    {
        SQMOD_GET_CREATED(*this)->Disconnect();
    }

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    Object Execute(CSStr query)
    {
        return MakeULongObj(SQMOD_GET_CREATED(*this)->Execute(query));
    }

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    Object Insert(CSStr query);

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    ResultSet Query(CSStr query);

    /* --------------------------------------------------------------------------------------------
     * Create a new statement on the managed connection.
    */
    Statement GetStatement(CSStr query);

    /* --------------------------------------------------------------------------------------------
     * Create a new transaction on the managed connection.
    */
    Transaction GetTransaction();

    /* --------------------------------------------------------------------------------------------
     * Escape unwanted characters from a given string.
    */
    LightObj EscapeString(StackStrF & str);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    static SQInteger ExecuteF(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    static SQInteger InsertF(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Attempt to execute the specified query.
    */
    static SQInteger QueryF(HSQUIRRELVM vm);

};

} // Namespace:: SqMod

#endif // _SQMYSQL_CONNECTION_HPP_
