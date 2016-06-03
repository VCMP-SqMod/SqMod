// ------------------------------------------------------------------------------------------------
#include "Transaction.hpp"
#include "Connection.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Transaction::Transaction(const Connection & db)
    : Transaction(db.GetHandle())
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Transaction::Transaction(const ConnHnd & db)
    : m_Connection(db), m_Committed(false)
{
    // Was the specified database connection valid?
    if (!m_Connection)
    {
        STHROWF("Invalid connection handle");
    }
    // Attempt to begin transaction
    else if ((m_Connection = sqlite3_exec(m_Connection, "BEGIN", nullptr, nullptr, nullptr)) != SQLITE_OK)
    {
        STHROWF("Unable to begin transaction [%s]", m_Connection.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
Transaction::~Transaction()
{
    // Was this transaction successfully committed?
    if (m_Committed)
    {
        return; // We're done here!
    }
    // Attempt to roll back changes because this failed to commit
    if ((m_Connection = sqlite3_exec(m_Connection, "ROLLBACK", nullptr, nullptr, nullptr)) != SQLITE_OK)
    {
        STHROWF("Unable to rollback transaction [%s]", m_Connection.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
bool Transaction::Commit()
{
    // We shouldn't even be here if there wasn't a valid connection but let's be sure
    if (!m_Connection)
    {
        STHROWF("Invalid database connection");
    }
    // Was this transaction already committed?
    else if (m_Committed)
    {
        STHROWF("Transaction was already committed");
    }
    // Attempt to commit the change during this transaction
    else if ((m_Connection = sqlite3_exec(m_Connection, "COMMIT", nullptr, nullptr, nullptr)) != SQLITE_OK)
    {
        STHROWF("Unable to commit transaction [%s]", m_Connection.ErrMsg());
    }
    else
    {
        m_Committed = true; // Everything was committed successfully
    }
    // Return the result
    return m_Committed;
}

} // Namespace:: SqMod
