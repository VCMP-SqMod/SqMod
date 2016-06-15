#ifndef _SQSQLITE_TRANSACTION_HPP_
#define _SQSQLITE_TRANSACTION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Connection.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Implements the RAII pattern for database transactions.
*/
class Transaction
{
public:

    /* --------------------------------------------------------------------------------------------
     * Construct by taking the handle from a connection.
    */
    Transaction(const Connection & db);

    /* --------------------------------------------------------------------------------------------
     * Construct using the direct connection handle.
    */
    Transaction(const ConnRef & db);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Transaction(const Transaction & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    Transaction(Transaction && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Transaction();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Transaction & operator = (const Transaction & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    Transaction & operator = (Transaction && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    const String & ToString() const
    {
        return m_Handle ? m_Handle->mName : NullString();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated statement handle.
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
     * Attempt to commit changes to the database.
    */
    bool Commit();

    /* --------------------------------------------------------------------------------------------
     * See whether the change during this transaction were successfully committed.
    */
    bool Commited() const
    {
        return m_Committed;
    }

private:

    // --------------------------------------------------------------------------------------------
    ConnRef     m_Handle; // The database connection handle where the transaction began.
    bool        m_Committed; // Whether changes were successfully committed to the database.
};

} // Namespace:: SqMod

#endif // _SQSQLITE_TRANSACTION_HPP_
