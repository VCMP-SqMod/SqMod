#ifndef _SQMYSQL_ACCOUNT_HPP_
#define _SQMYSQL_ACCOUNT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * ...
*/
class Connection
{

protected:

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Connection(const Account & acc);

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
    ~Connection() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Connection & operator = (const Connection & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Connection & operator = (Connection && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Validate the managed connection handle and throw exception if it doesn't exist.
    */
    void Validate() const;

private:

    // --------------------------------------------------------------------------------------------
    ConnHnd         m_Handle; // Handle to the actual database connection.

public:

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the hosted connection handle.
    */
    operator ConnHnd ()
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the hosted connection handle.
    */
    operator const ConnHnd & () const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to connect to the database.
    */
    bool Connect();

    /* --------------------------------------------------------------------------------------------
     * Disconnect from the currently connected database.
    */
    void Disconnect();

    /* --------------------------------------------------------------------------------------------
     * See whether a successful connection was made or not.
    */
    bool Connected() const
    {
        return (m_Handle != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Get the account used to make the connection.
    */
    const Account::Ref & GetAccount() const
    {
        return m_Account;
    }

    /* --------------------------------------------------------------------------------------------
     * Get the server schema.
    */
    const String & GetSchema() const
    {
        return m_Schema;
    }

    /* --------------------------------------------------------------------------------------------
     * Set the server schema.
    */
    void SetSchema(const String & schema);

    /* --------------------------------------------------------------------------------------------
     * Get the character set.
    */
    const String & GetCharset() const
    {
        return m_Charset;
    }

    /* --------------------------------------------------------------------------------------------
     * Set the character set.
    */
    void SetCharset(const String & charset);

    /* --------------------------------------------------------------------------------------------
     * See whether auto-commit is enabled or not.
    */
    bool GetAutocommit() const
    {
        return m_Autocommit;
    }

    /* --------------------------------------------------------------------------------------------
     * Set whether auto-commit should be enabled or not.
    */
    void SetAutocommit(bool toggle);

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    Uint64 Execute(const String & query);

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    Uint64 Insert(const String & query);

    /* --------------------------------------------------------------------------------------------
     * Execute a query on the server.
    */
    Uint64 Query(const String & query);

    /* --------------------------------------------------------------------------------------------
     * 
    */
    StatementRef GetStatement(const String & query);
};

} // Namespace:: SqMod

#endif // _SQMYSQL_ACCOUNT_HPP_
