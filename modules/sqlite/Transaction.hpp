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
    Transaction(const ConnHnd & db);

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
    ConnHnd     m_Connection; // The database connection handle where the transaction began.
    bool        m_Committed; // Whether changes were successfully committed to the database.
};

} // Namespace:: SqMod

#endif // _SQSQLITE_TRANSACTION_HPP_
