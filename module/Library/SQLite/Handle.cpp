// ------------------------------------------------------------------------------------------------
#include "Library/SQLite/Handle.hpp"

#include <utility>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
ConnHnd::ConnHnd()
    : mPtr(nullptr)
    , mStatus(SQLITE_OK)
    , mQueue()
    , mFlags(0)
    , mName()
    , mVFS()
    , mMemory(false)
    , mTrace(false)
    , mProfile(false)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
ConnHnd::~ConnHnd()
{
    // Is there anything to close?
    if (mPtr != nullptr)
    {
        // Flush remaining queries in the queue and ignore the result
        Flush(mQueue.size(), NullObject(), NullFunction());
        // NOTE: Should we call sqlite3_interrupt(...) before closing?
        // Attempt to close the database
        if ((sqlite3_close(mPtr)) != SQLITE_OK)
        {
            LogErr("Unable to close SQLite connection [%s]", sqlite3_errmsg(mPtr));
        }
    }
}

// ------------------------------------------------------------------------------------------------
void ConnHnd::Create(CSStr name, Int32 flags, CSStr vfs)
{
    // Make sure a previous connection doesn't exist
    if (mPtr)
    {
        STHROWF("Unable to connect to database. Database already connected");
    }
    // Make sure the name is valid
    else if (!name || *name == '\0')
    {
        STHROWF("Unable to connect to database. The name is invalid");
    }
    // Attempt to create the database connection
    else if ((mStatus = sqlite3_open_v2(name, &mPtr, flags, vfs)) != SQLITE_OK)
    {
        // Grab the error message before destroying the handle
        String msg(sqlite3_errmsg(mPtr) ? sqlite3_errmsg(mPtr) : _SC("Unknown reason"));
        // Must be destroyed regardless of result
        sqlite3_close(mPtr);
        // Prevent further use of this handle
        mPtr = nullptr;
        // Now its safe to throw the error
        STHROWF("Unable to connect to database [%s]", msg.c_str());
    }
    // Let's save the specified information
    mName.assign(name);
    mFlags = flags;
    mVFS.assign(vfs ? vfs : _SC(""));
    // Optional check if database is initially stored in memory
    mMemory = (mName == _SC(":memory:"));
}

// ------------------------------------------------------------------------------------------------
Int32 ConnHnd::Flush(Uint32 num, Object & env, Function & func)
{
    // Do we even have a valid connection?
    if (!mPtr)
    {
        return -1; // No connection!
    }
    // Is there anything to flush?
    else if (!num || mQueue.empty())
    {
        return 0; // Nothing to process!
    }
    // Can we even flush that many?
    else if (num > mQueue.size())
    {
        num = mQueue.size();
    }
    // Generate the function that should be called upon error
    Function callback = Function(env.GetVM(), env.GetObject(), func.GetFunc());
    // Obtain iterators to the range of queries that should be flushed
    auto itr = mQueue.begin();
    auto end = mQueue.begin() + num;
    // Attempt to begin the flush transaction
    if ((mStatus = sqlite3_exec(mPtr, "BEGIN", nullptr, nullptr, nullptr)) != SQLITE_OK)
    {
        STHROWF("Unable to begin flush transaction [%s]", sqlite3_errmsg(mPtr));
    }
    // Process all queries within range of selection
    for (; itr != end; ++itr)
    {
        // Should we manually terminate this query?
        /*
        if (*(*itr).rbegin() != ';')
        {
            itr->push_back(';');
        }
        */
        // Attempt to execute the currently processed query string
        if ((mStatus = sqlite3_exec(mPtr, itr->c_str(), nullptr, nullptr, nullptr)) == SQLITE_OK)
        {
            continue;
        }
        // Do we have to execute any callback to resolve our issue?
        else if (!callback.IsNull())
        {
            try
            {
                // Ask the callback whether the query processing should end here
                SharedPtr< bool > ret = callback.Evaluate< bool >(mStatus, *itr);
                // Should we break here?
                if (!!ret && !(*ret))
                {
                    break;
                }
            }
            catch (const Sqrat::Exception & e)
            {
                LogErr("Squirrel error caught in flush handler [%s]", e.what());
            }
            catch (const std::exception & e)
            {
                LogErr("Program error caught in flush handler [%s]", e.what());
            }
            catch (...)
            {
                LogErr("Unknown error caught in flush handler");
            }
        }
    }
    // Erase all queries till end or till the point of failure (if any occurred)
    mQueue.erase(mQueue.begin(), itr);
    // Attempt to commit changes requested during transaction
    if ((mStatus = sqlite3_exec(mPtr, "COMMIT", nullptr, nullptr, nullptr)) == SQLITE_OK)
    {
        return sqlite3_changes(mPtr);
    }
    // Attempt to roll back erroneous changes
    else if ((mStatus = sqlite3_exec(mPtr, "ROLLBACK", nullptr, nullptr, nullptr)) != SQLITE_OK)
    {
        STHROWF("Unable to rollback flush transaction [%s]", sqlite3_errmsg(mPtr));
    }
    // The transaction failed somehow but we managed to rollback
    else
    {
        STHROWF("Unable to commit flush transaction [%s]", sqlite3_errmsg(mPtr));
    }
    // Operation failed
    return -1;
}

// ------------------------------------------------------------------------------------------------
StmtHnd::StmtHnd(ConnRef conn)
    : mPtr(nullptr)
    , mStatus(SQLITE_OK)
    , mConn(std::move(conn))
    , mQuery()
    , mColumns(0)
    , mParameters(0)
    , mIndexes()
    , mGood(false)
    , mDone(false)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
StmtHnd::~StmtHnd()
{
    // Is there anything to finalize?
    if (mPtr != nullptr)
    {
        // Attempt to finalize the statement
        if ((sqlite3_finalize(mPtr)) != SQLITE_OK)
        {
            LogErr("Unable to finalize SQLite statement [%s]", mConn->ErrMsg());
        }
    }
}

// ------------------------------------------------------------------------------------------------
void StmtHnd::Create(CSStr query, SQInteger length)
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
    // Is the specified query string valid?
    else if (!query || !length || *query == '\0')
    {
        STHROWF("Unable to prepare statement. Invalid or empty query string");
    }
    // Save the query string
    mQuery.assign(query, length);
    // Attempt to prepare a statement with the specified query string
    if ((mStatus = sqlite3_prepare_v2(mConn->mPtr, mQuery.c_str(), ConvTo< Int32 >::From(mQuery.size()),
                                            &mPtr, nullptr)) != SQLITE_OK)
    {
        // Clear the query string since it failed
        mQuery.clear();
        // Explicitly make sure the handle is null
        mPtr = nullptr;
        // Now it's safe to throw the error
        STHROWF("Unable to prepare statement [%s]", mConn->ErrMsg());
    }
    else
    {
        // Obtain the number of available columns
        mColumns = sqlite3_column_count(mPtr);
        // Obtain the number of available parameters
        mParameters = sqlite3_bind_parameter_count(mPtr);
    }
}

// ------------------------------------------------------------------------------------------------
Int32 StmtHnd::GetColumnIndex(CSStr name, SQInteger length)
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
            auto column_name = static_cast< CSStr >(sqlite3_column_name(mPtr, i));
            // Validate the name
            if (!column_name)
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
    const String str(name, length < 0 ? std::strlen(name) : length);
    // Attempt to find the specified column
    const Indexes::iterator itr = mIndexes.find(str);
    // Was there a column with the specified name?
    if (itr != mIndexes.end())
    {
        return itr->second;
    }
    // No such column exists (expecting the invoker to validate the result)
    return -1;
}

// ------------------------------------------------------------------------------------------------
CCStr StmtHnd::ErrStr() const
{
    return mConn ? sqlite3_errstr(sqlite3_errcode(mConn->mPtr)) : _SC("");
}

// ------------------------------------------------------------------------------------------------
CCStr StmtHnd::ErrMsg() const
{
    return mConn ? sqlite3_errmsg(mConn->mPtr) : _SC("");
}

// ------------------------------------------------------------------------------------------------
Int32 StmtHnd::ErrNo() const
{
    return mConn ? sqlite3_errcode(mConn->mPtr) : SQLITE_NOMEM;
}

// ------------------------------------------------------------------------------------------------
Int32 StmtHnd::ExErrNo() const
{
    return mConn ? sqlite3_extended_errcode(mConn->mPtr) : SQLITE_NOMEM;
}

} // Namespace:: SqMod
