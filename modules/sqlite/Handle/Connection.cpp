// ------------------------------------------------------------------------------------------------
#include "Handle/Connection.hpp"

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
            SqMod_LogErr("Unable to close SQLite connection [%s]", sqlite3_errmsg(mPtr));
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
    mMemory = (mName.compare(_SC(":memory:")) == 0);
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
    QueryList::iterator itr = mQueue.begin();
    QueryList::iterator end = mQueue.begin() + num;
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
                SharedPtr< bool > ret = callback.Evaluate< bool, Int32, const String & >(mStatus, *itr);
                // Should we break here?
                if (!!ret && (*ret == false))
                {
                    break;
                }
            }
            catch (const Sqrat::Exception & e)
            {
                SqMod_LogErr("Squirrel error caught in flush handler [%s]", e.what());
            }
            catch (const std::exception & e)
            {
                SqMod_LogErr("Program error caught in flush handler [%s]", e.what());
            }
            catch (...)
            {
                SqMod_LogErr("Unknown error caught in flush handler");
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

} // Namespace:: SqMod
