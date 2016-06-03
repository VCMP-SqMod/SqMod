// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void StmtHnd::Validate() const
{
    // Is the handle valid?
    if ((m_Hnd == nullptr) || (m_Hnd->mPtr == nullptr))
    {
        STHROWF("Invalid SQLite statement reference");
    }
}

// ------------------------------------------------------------------------------------------------
StmtHnd::Handle::~Handle()
{
    // Is there anything to finalize?
    if (!mPtr)
    {
        return; // Nothing to finalize
    }
    // Are we dealing with a memory leak? Technically shouldn't reach this situation!
    else if (mRef != 0)
    {
        // Should we deal with undefined behavior instead? How bad is one statement left alive?
        _SqMod->LogErr("SQLite statement is still referenced (%s)", mQuery.c_str());
    }
    else
    {
        // Attempt to finalize the statement
        if ((sqlite3_finalize(mPtr)) != SQLITE_OK)
        {
            _SqMod->LogErr("Unable to finalize SQLite statement [%s]", mConn.ErrMsg());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void StmtHnd::Handle::Create(CSStr query)
{
    // Make sure a previous statement doesn't exist
    if (mPtr)
    {
        STHROWF("Unable to prepare statement. Statement already prepared");
    }
    // Is the specified database connection is valid?
    else if (!mConn)
    {
        STHROWF("Unable to prepare statement. Invalid connection handle");
    }
    // Save the query string and therefore multiple strlen(...) calls
    mQuery.assign(query ? query : _SC(""));
    // Is the specified query string we just saved, valid?
    if (mQuery.empty())
    {
        STHROWF("Unable to prepare statement. Invalid query string");
    }
    // Attempt to prepare a statement with the specified query string
    else if ((mStatus = sqlite3_prepare_v2(mConn, mQuery.c_str(), (Int32)mQuery.size(),
                                            &mPtr, nullptr)) != SQLITE_OK)
    {
        // Clear the query string since it failed
        mQuery.clear();
        // Explicitly make sure the handle is null
        mPtr = nullptr;
        // Now it's safe to throw the error
        STHROWF("Unable to prepare statement [%s]", mConn.ErrMsg());
    }
    else
    {
        // Obtain the number of available columns
        mColumns = sqlite3_column_count(mPtr);
    }
}

// ------------------------------------------------------------------------------------------------
Int32 StmtHnd::Handle::GetColumnIndex(CSStr name)
{
    // Validate the handle
    if (!mPtr)
    {
        STHROWF("Invalid SQLite statement");
    }
    // Are the names cached?
    else if (mIndexes.empty())
    {
        for (Int32 i = 0; i < mColumns; ++i)
        {
            // Get the column name at the current index
            CSStr name = (CSStr)sqlite3_column_name(mPtr, i);
            // Validate the name
            if (!name)
            {
                STHROWF("Unable to retrieve column name for index (%d)", i);
            }
            // Save it to guarantee the same lifetime as this instance
            else
            {
                mIndexes[name] = i;
            }
        }
    }
    // Attempt to find the specified column
    const Indexes::iterator itr = mIndexes.find(name);
    // Was there a column with the specified name?
    if (itr != mIndexes.end())
    {
        return itr->second;
    }
    // No such column exists (expecting the invoker to validate the result)
    return -1;
}

} // Namespace:: SqMod