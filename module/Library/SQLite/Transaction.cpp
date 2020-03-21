// ------------------------------------------------------------------------------------------------
#include "Library/SQLite/Transaction.hpp"

#include <utility>
#include "Library/SQLite/Connection.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqLiteTransaction"))

// ------------------------------------------------------------------------------------------------
Transaction::Transaction(const Connection & db)
    : Transaction(db.GetHandle())
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Transaction::Transaction(ConnRef  db)
    : m_Handle(std::move(db)), m_Committed(false)
{
    // Was the specified database connection valid?
    if (!m_Handle)
    {
        STHROWF("Invalid connection handle");
    }
    // Attempt to begin transaction
    m_Handle->mStatus = sqlite3_exec(m_Handle->mPtr, "BEGIN", nullptr, nullptr, nullptr);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF("Unable to begin transaction [%s]", m_Handle->ErrMsg());
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
    m_Handle->mStatus = sqlite3_exec(m_Handle->mPtr, "ROLLBACK", nullptr, nullptr, nullptr);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        // We cannot throw exceptions in destructor
        LogErr("Unable to rollback transaction [%s]", m_Handle->ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
bool Transaction::Commit()
{
    // We shouldn't even be here if there wasn't a valid connection but let's be sure
    if (!m_Handle)
    {
        STHROWF("Invalid database connection");
    }
    // Was this transaction already committed?
    else if (m_Committed)
    {
        STHROWF("Transaction was already committed");
    }
    // Attempt to commit the change during this transaction
    m_Handle->mStatus = sqlite3_exec(m_Handle->mPtr, "COMMIT", nullptr, nullptr, nullptr);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF("Unable to commit transaction [%s]", m_Handle->ErrMsg());
    }
    else
    {
        m_Committed = true; // Everything was committed successfully
    }
    // Return the result
    return m_Committed;
}

// ================================================================================================
void Register_Transaction(Table & sqlns)
{
    sqlns.Bind(_SC("Transaction"),
        Class< Transaction, NoCopy< Transaction > >(sqlns.GetVM(), Typename::Str)
        // Constructors
        .Ctor< const Connection & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Transaction::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Transaction::IsValid)
        .Prop(_SC("Committed"), &Transaction::Commited)
        // Member Methods
        .Func(_SC("Commit"), &Transaction::Commit)
    );
}

} // Namespace:: SqMod
