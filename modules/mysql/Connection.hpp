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
    ConnHnd         m_Handle; // Handle to the actual database connection.

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
        : m_Handle(acc)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Connection(const ConnHnd & conn)
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
     * Destructor.
    */
    ~Connection()
    {
        // Let the handle deal with closing the connection if necessary
    }

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
    Int32 Cmp(const Connection & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw exception if it doesn't exist.
    */
    void Validate() const;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed connection handle.
    */
    operator ConnHnd & ()
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed connection handle.
    */
    operator const ConnHnd & () const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the managed connection handle.
    */
    ConnHnd & GetHnd()
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the managed connection handle.
    */
    const ConnHnd & GetHnd() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether a successful connection was made or not.
    */
    bool Connected() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Disconnect from the currently connected database.
    */
    void Disconnect()
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Perform the requested operation
        m_Handle->Disconnect();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current error number.
    */
    SQInteger GetErrNo() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current error message.
    */
    CSStr GetErrStr() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received error number.
    */
    SQInteger GetLastErrNo() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return static_cast< SQInteger >(m_Handle->mErrNo);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received error message.
    */
    const String & GetLastErrStr() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mErrStr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection port number.
    */
    SQInteger GetPortNum() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return static_cast< SQInteger >(m_Handle->mPort);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection host address.
    */
    const String & GetHost() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mHost;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection user name.
    */
    const String & GetUser() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mUser;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection password.
    */
    const String & GetPass() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mPass;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the selected database name.
    */
    const String & GetName() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mName;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the selected database name.
    */
    void SetName(CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection socket.
    */
    const String & GetSocket() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mSocket;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection flags.
    */
    SQInteger GetFlags() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return static_cast< SQInteger >(m_Handle->mFlags);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL key.
    */
    const String & GetSSL_Key() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mSSL_Key;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate.
    */
    const String & GetSSL_Cert() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mSSL_Cert;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate authority.
    */
    const String & GetSSL_CA() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mSSL_CA;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL certificate authority path.
    */
    const String & GetSSL_CA_Path() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mSSL_CA_Path;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection SSL cipher.
    */
    const String & GetSSL_Cipher() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mSSL_Cipher;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the default character set for the managed connection.
    */
    const String & GetCharset() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mCharset;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the default character set for the managed connection.
    */
    void SetCharset(CSStr charset);

    /* --------------------------------------------------------------------------------------------
     * See whether auto-commit is enabled or not.
    */
    bool GetAutoCommit() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mAutoCommit;
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether auto-commit should be enabled or not.
    */
    void SetAutoCommit(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * See whether the connection is in the middle of a transaction.
    */
    bool GetInTransaction() const
    {
        // Validate the managed handle
        m_Handle.Validate();
        // Return the requested information
        return m_Handle->mInTransaction;
    }

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    Object Execute(CSStr query);

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
};

} // Namespace:: SqMod

#endif // _SQMYSQL_CONNECTION_HPP_
